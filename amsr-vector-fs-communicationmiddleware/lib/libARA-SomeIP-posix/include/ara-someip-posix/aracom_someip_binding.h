/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2018 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  aracom_someip_binding.h
 *         \brief AraComSomeIpBinding serves as an anchor between the SOME/IP protocol via POSIX and the
 *                ara::com related high-level layers.
 *
 *         \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_ARACOM_SOMEIP_BINDING_H_
#define LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_ARACOM_SOMEIP_BINDING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "ara-someip-posix/aracom_someip_binding_interface.h"
#include "ara-someip-posix/aracom_someip_proxy_instance_factory.h"
#include "ara/com/backend_not_available_exception.h"
#include "ara/com/configuration/configuration.h"
#include "ara/com/deallocation_exception.h"
#include "ara/com/runtime.h"
#include "ara/log/logging.hpp"
#include "someip-posix-common/someip/config_model.h"
#include "someip-posix-common/someip/message_builder.h"
#include "someip-posix-common/someip/message_verification.h"
#include "someip-posix/someip_posix.h"
#include "vac/memory/object_pool.h"

namespace ara {
namespace com {
namespace someip_posix {

/**
 * \brief Realizes a concrete Binding for the proxy/skeleton communication via SOME/IP and serves as an
 * anchor between the application-side and the communication protocol of SOME/IP.
 * For each concrete binding like this, there is only one object of this type available at runtime (singleton).
 * \tparam SomeIpPosix Used SOME/IP transport layer
 * \tparam SomeIpPosixShutdownHandlerInterface Used SOME/IP transport layer shutdown handler interface
 */
template <typename SomeIpPosix = ::someip_posix::SomeIpPosix,
          typename SomeIpPosixShutdownHandlerInterface = ::someip_posix::SomeIpPosixShutdownHandlerInterface>
class AraComSomeIpBinding : public ara::com::someip_posix::AraComSomeIpBindingInterface,
                            SomeIpPosixShutdownHandlerInterface {
 public:
  /**
   * \brief Constructor of this binding initializes the attributes of this class.
   * \param config A reference to configuration to initialize this binding and its
   * transport protocol.
   */
  explicit AraComSomeIpBinding(const configuration::Configuration& config)
      : someip_posix_(BuildSomeIpPosixConfig(config), &ara::com::Runtime::getInstance().GetReactor()),
        sub_logger_(ara::log::CreateLogger("AraComSomeIpBinding", "")) {
    sub_logger_.LogDebug() << __func__;
    someip_posix_.SetSomeipPosixApplication(this);
    someip_posix_.SetShutdownHandler(this);
    proxy_instance_factory_pool_.reserve(
        kNumProxyInstanceFactories);  // TODO(PAASR-1548): Value should be read from configuration
  }

  /**
   * \brief The concrete binding shall be non-copyable and is stored in an intrusive
   * list.
   */
  AraComSomeIpBinding(const AraComSomeIpBinding& other) = delete;

  /**
   * \brief Destructor.
   */
  virtual ~AraComSomeIpBinding() {
    // Check that all handles to the factories are released
    for (AraComSomeIpProxyInstanceFactory& factory : proxy_instance_factory_pool_) {
      if (factory.GetReferenceCount() != 0) {
        sub_logger_.LogError() << "Some handles to ServiceProxyFactoryInterface are not released.\n";
        // TODO(PAASR-2407) Don't throw exceptions in destructors.
        throw ara::com::DeallocationException();
      }
    }
  }

  /**
   * \brief Generated method `Initialize` registers all the generated proxy and skeleton
   * classes with its corresponding binding and creates an interface for proxies or skeletons to use this
   * binding.
   * \remark Implementation of this method prototype is generated and shall be realized in a separate
   * translation unit (.cc). This method is then called by the Runtime, which loops through all the
   * existing bindings.
   */
  void Initialize();

  /**
   * \brief Execute a synchronous FindService call.
   *
   * \param proxy_id The ara::com::internal::ProxyId to search for
   * \param instance_id The ara::com::InstanceIdentifier for which to find a service
   *
   * \returns A container of HandleTypes for the currently known services
   */
  ServiceHandleContainer<HandleType> FindService(ara::com::internal::ProxyId proxy_id,
                                                 const ara::com::InstanceIdentifier& instance_id) override {
    sub_logger_.LogDebug() << __func__;

    ara::com::ServiceHandleContainer<ara::com::HandleType> handles;

    // Find its matching ServiceSomeIpProxyFactory to create the binding-related proxy-part, when the user
    // creates one proxy instance.
    if (someip_posix_.IsRunning()) {
      auto it = std::find_if(
          proxy_factories_.begin(), proxy_factories_.end(),
          [proxy_id](AraComSomeIpProxyFactoryInterface& factory) { return factory.GetProxyId() == proxy_id; });

      if (it != proxy_factories_.end()) {
        const someip_posix_common::someip::ServiceId someip_service_id = LookupSomeIpServiceId(*it);
        const someip_posix_common::someip::InstanceId someip_instance_id = LookupSomeIpInstanceId(instance_id);
        const someip_posix_common::someip::ServiceInstanceContainer service_instances =
            someip_posix_.FindService(someip_service_id, someip_instance_id);

        for (auto& service_instance : service_instances) {
          // Creating a HandleType for each instance result of FindService requires one AraComSomeIpProxyInstanceFactory
          // instance, that is part of the HandleType instance. With this link in the HandleType object,
          // we are able to create a binding the proxy frontend can
          // talk to, after a proxy has been instantiated.

          AraComSomeIpProxyInstanceFactory::shared_ptr instance_factory_ptr =
              FindOrMakeProxyInstanceFactory(service_instance.instance_id_, it->GetSelf());

          if (instance_id == ara::com::InstanceIdentifier::Any) {
            handles.emplace_back(LookupAraComInstanceId(service_instance.instance_id_), instance_factory_ptr);
          } else {
            handles.emplace_back(instance_id, instance_factory_ptr);
          }
        }
      }
    } else {
      // TODO(PAASR-2231) Move to separate class AraComSomeIpBindingStateManger
      throw ara::com::BackendNotAvailableException();
    }
    return handles;
  }

  /**
   *  \brief Triggers asynchronous SOME/IP service lookup according to the ARA::COM specification.
   *
   *  \param handle Created handle for this asynchronous FindService job.
   *  \param observer A reference to the observer object, that delegates the asynchronous notifications.
   */
  void StartFindService(const ara::com::FindServiceHandle& handle,
                        ara::com::internal::ServiceUpdateObserver& observer) override {
    const ara::com::internal::ProxyId proxy_id = handle.proxy_id;
    const ara::com::InstanceIdentifier instance_id = ara::com::InstanceIdentifier(handle.instance_id);

    if (someip_posix_.IsRunning()) {
      // Find its matching ServiceSomeIpProxyFactory to create the binding-related proxy-part, when the user
      // creates one proxy instance.
      auto it = std::find_if(
          proxy_factories_.begin(), proxy_factories_.end(),
          [proxy_id](AraComSomeIpProxyFactoryInterface& factory) { return factory.GetProxyId() == proxy_id; });

      if (it != proxy_factories_.end()) {
        const someip_posix_common::someip::ServiceId someip_service_id = LookupSomeIpServiceId(*it);
        const someip_posix_common::someip::InstanceId someip_instance_id = LookupSomeIpInstanceId(instance_id);
        // Directly assign a SOME/IP service ID to a corresponding ServiceProxy observer
        findservice_proxy_observers_.emplace(someip_service_id, &observer);
        // Inform the daemon about an active StartFindService
        someip_posix_.StartFindService(someip_service_id, someip_instance_id);
        StartFindServiceJobsMap::iterator it_jobs = findservice_jobs_.find(someip_service_id);

        if (it_jobs == findservice_jobs_.end()) {
          StartFindServiceJobsPerService request;
          request.push_back(std::make_pair(someip_instance_id, handle));
          findservice_jobs_.emplace(std::make_pair(someip_service_id, request));
        } else {
          it_jobs->second.push_back(std::make_pair(someip_instance_id, handle));
        }
      }
    } else {
      // TODO(PAASR-2231) Move to separate class AraComSomeIpBindingStateManger
      throw ara::com::BackendNotAvailableException();
    }

    // Get the initial snapshot of service instances, as specified in the ara::com SWS.
    ServiceHandleContainer<HandleType> handles = FindService(proxy_id, instance_id);
    // Forward the initial snapshot to the one handle, that has requested asynchronous notification,
    // without touching all the others. This part of the asynchronous version acts like it would be a synchronous
    // FindService.
    observer.OnUpdate(handle, handles);  // Initial snapshot - forwarded to the handler specified by the user.
  }
  /**
   * \brief Stop asynchronous service discovery.
   *
   * \param handle the handle for the asynchronous service discovery to cancel.
   */
  void StopFindService(const FindServiceHandle& handle) override {
    const ara::com::internal::ProxyId proxy_id = handle.proxy_id;
    const ara::com::InstanceIdentifier instance_id = ara::com::InstanceIdentifier(handle.instance_id);
    if (someip_posix_.IsRunning()) {
      auto it = std::find_if(
          proxy_factories_.begin(), proxy_factories_.end(),
          [proxy_id](AraComSomeIpProxyFactoryInterface& factory) { return factory.GetProxyId() == proxy_id; });

      if (it != proxy_factories_.end()) {
        const someip_posix_common::someip::ServiceId someip_service_id = LookupSomeIpServiceId(*it);
        const someip_posix_common::someip::InstanceId someip_instance_id = LookupSomeIpInstanceId(instance_id);
        someip_posix_.StopFindService(someip_service_id, someip_instance_id);
        StartFindServiceJobsMap::iterator it_jobs = findservice_jobs_.find(someip_service_id);

        // Remove the pending active FindService job associated with the handle passed as parameter.
        if (it_jobs != findservice_jobs_.end()) {
          StartFindServiceJobsPerService& jobs = it_jobs->second;
          StartFindServiceJobsPerService::iterator it_rem =
              std::find(jobs.begin(), jobs.end(), std::make_pair(someip_instance_id, handle));

          if (it_rem != jobs.end()) {
            jobs.erase(it_rem);
          }
        }
      }
    } else {
      throw ara::com::BackendNotAvailableException();
    }
  }

  /**
   * \brief Offer a service over the SOME/IP binding from the skeleton.
   *
   * \param skeleton_id the ara::com::internal::SkeletonId of the service to offer
   * \param instance_id The ara::com::InstanceIdentifier for which to offer a service
   * \param skeleton The related frontend skeleton
   *
   * \return A SkeletonImplInterface for the given Service, configured for the given instance_identifier or nullptr, if
   * no
   * such SkeletonImplInterface is available from the binding.
   */
  std::unique_ptr<internal::SkeletonImplInterface> OfferService(
      ara::com::internal::SkeletonId skeleton_id, const ara::com::InstanceIdentifier& instance_id,
      ara::com::internal::SkeletonInterface& skeleton) override {
    sub_logger_.LogDebug() << __func__;

    if (someip_posix_.IsRunning()) {
      // Find the SomeIpSkeletonFactory that will produce a Skeleton implementation for the correct service/version.
      auto it = std::find_if(skeleton_factories_.begin(), skeleton_factories_.end(),
                             [skeleton_id](AraComSomeIpSkeletonFactoryInterface& factory) {
                               return factory.GetSkeletonId() == skeleton_id;
                             });

      // Produce a Skeleton implementation parameterized to the given instance_id
      if (it != skeleton_factories_.end()) {
        const someip_posix_common::someip::InstanceId someip_instance_id = LookupSomeIpInstanceId(instance_id);
        std::unique_ptr<ara::com::internal::SkeletonImplInterface> skeleton_impl =
            it->create(someip_instance_id, skeleton);
        // Finally offer the service to the SOME/IP daemon.
        const someip_posix_common::someip::ServiceId someip_service_id = it->GetSomeIpServiceId();
        someip_posix_.OfferService(someip_service_id, someip_instance_id);
        return skeleton_impl;
      } else {
        return std::unique_ptr<internal::SkeletonImplInterface>();
      }
    } else {
      throw ara::com::BackendNotAvailableException();
    }
  }

  /**
   * \brief Cancel the offering of a specific service instance from the skeleton.
   *
   * \param skeleton_id the ara::com::internal::SkeletonId for which to stop offering a service
   * \param instance_id The ara::com::InstanceIdentifier for which to stop offering a service
   */
  void StopOfferService(ara::com::internal::SkeletonId skeleton_id,
                        const ara::com::InstanceIdentifier& instance_id) override {
    if (someip_posix_.IsRunning()) {
      auto it = std::find_if(skeleton_factories_.begin(), skeleton_factories_.end(),
                             [skeleton_id](AraComSomeIpSkeletonFactoryInterface& factory) {
                               return factory.GetSkeletonId() == skeleton_id;
                             });

      if (it != skeleton_factories_.end()) {
        const someip_posix_common::someip::ServiceId someip_service_id = it->GetSomeIpServiceId();
        const someip_posix_common::someip::InstanceId someip_instance_id = LookupSomeIpInstanceId(instance_id);
        const SkeletonBindingIdentity skeleton_identity(someip_service_id, someip_instance_id);
        someip_posix_.StopOfferService(std::get<0>(skeleton_identity), std::get<1>(skeleton_identity));
      }
    } else {
      throw ara::com::BackendNotAvailableException();
    }
  }

  /**
   * \brief Lets the SOME/IP binding know that a proxy wishes to receive an event of a service instance.
   *
   * \param service_id SOME/IP service id of the service.
   * \param instance_id SOME/IP instance id of the service.
   * \param event_id SOME/IP event id of the service.
   */
  void SubscribeEvent(someip_posix_common::someip::ServiceId service_id,
                      someip_posix_common::someip::InstanceId instance_id,
                      someip_posix_common::someip::EventId event_id) override {
    if (someip_posix_.IsRunning()) {
      someip_posix_.SubscribeEvent(service_id, instance_id, event_id);
    } else {
      throw ara::com::BackendNotAvailableException();
    }
  }

  /**
   * \brief Lets the SOME/IP binding know that an proxy does not wish to receive an event of a service instance.
   *
   * \param service_id SOME/IP service id of the service.
   * \param instance_id SOME/IP instance id of the service.
   * \param event_id SOME/IP event id of the service.
   */
  void UnsubscribeEvent(someip_posix_common::someip::ServiceId service_id,
                        someip_posix_common::someip::InstanceId instance_id,
                        someip_posix_common::someip::EventId event_id) override {
    if (someip_posix_.IsRunning()) {
      someip_posix_.UnsubscribeEvent(service_id, instance_id, event_id);
    } else {
      throw ara::com::BackendNotAvailableException();
    }
  }

  /**
   * \brief An instantiated skeleton implementation will send an event notification to the client.
   *
   * \param instance_id The SOME/IP instance id of the skeleton-binding
   * \param packet Contains the complete SomeIpPacket of header and payload.
   */
  void SendEventNotification(someip_posix_common::someip::InstanceId instance_id,
                             someip_posix_common::someip::SomeIpPacket&& packet) override {
    if (someip_posix_.IsRunning()) {
      someip_posix_.Send(instance_id, std::move(packet));
    } else {
      throw ara::com::BackendNotAvailableException();
    }
  }

  /**
   * \brief An instantiated proxy will send a method request to a server.
   *
   * \param instance_id The SOME/IP instance id of the proxy binding
   * \param packet Contains the complete SomeIpPacket of header and payload.
   */
  void SendMethodRequest(someip_posix_common::someip::InstanceId instance_id,
                         someip_posix_common::someip::SomeIpPacket&& packet) override {
    if (someip_posix_.IsRunning()) {
      someip_posix_.Send(instance_id, std::move(packet));
    } else {
      throw ara::com::BackendNotAvailableException();
    }
  }

  /**
   * \brief Is needed for infrastructural faults such as:
   * Deserialization fails or the service/method isn't implemented at all.
   * This method is mainly called by the server-side and only in case if it's no
   * fire & forget method.
   *
   * \uptrace PRS_SOMEIP_00190
   * \uptrace PRS_SOMEIP_00539
   * \uptrace PRS_SOMEIP_00188
   * \uptrace PRS_SOMEIP_00537
   * \uptrace PRS_SOMEIP_00171
   *
   * \param return_code The return code to identify the error.
   * \param instance_id The SOME/IP instance id of the proxy binding.
   * \param request_header The unmodified SOME/IP header from the request.
   * This SOME/IP header will be used for the error response.
   */
  void SendErrorResponseOnMethodRequest(
      const someip_posix_common::someip::ReturnCode return_code,
      const someip_posix_common::someip::InstanceId instance_id,
      const someip_posix_common::someip::SomeIpMessageHeader& request_header) override {
    someip_posix_common::someip::SomeIpPacket packet_response{new ::someip_posix_common::someip::PacketBuffer{}};
    packet_response =
        someip_posix_common::someip::CreateSomeIpErrorHeader(return_code, request_header, std::move(packet_response));

    if (someip_posix_.IsRunning()) {
      someip_posix_.Send(instance_id, std::move(packet_response));
    } else {
      throw ara::com::BackendNotAvailableException();
    }
  }

  /**
   * \brief On a method request/response, this method is called by the transport layer.
   *
   * \param instance_id the SOME/IP instance id which is normally mapped to one socket (IP address + port)
   * \param packet the SOME/IP message including header and payload
   */
  void HandleReceive(someip_posix_common::someip::InstanceId instance_id,
                     someip_posix_common::someip::SomeIpPacket packet) override {
    sub_logger_.LogDebug() << __func__;

    // Shortening
    namespace marshaller = someip_posix_common::someip::serialization;

    // Create root deserializer for this method request.
    RootDeserializerAlias deserializer{std::move(packet)};
    marshaller::SomeIpHeaderDeserializer<marshaller::BEPayloadNoLengthFieldPolicy, decltype(deserializer)>
        header_deserializer{&deserializer};
    const someip_posix_common::someip::SomeIpMessageHeader& header = header_deserializer.GetDeserializedHeader();

    switch (header.message_type_) {
      case someip_posix_common::someip::kResponse: {
        const someip_posix_common::someip::SomeIpReturnCode verify_return_code{
            someip_posix_common::someip::ResponseMessageVerification::DoInfrastructuralChecks(header)};
        if (verify_return_code == someip_posix_common::someip::SomeIpReturnCode::kOk) {
          RouteMethodResponse(instance_id, header, std::move(deserializer));  // delegate to concrete proxy-binding
        } else {
          sub_logger_.LogError() << "Method response message verification failed with return code "
                                 << verify_return_code << " - (" << std::hex << header.service_id_ << ", " << std::dec
                                 << instance_id << "): "
                                 << "method ID: " << header.method_id_ << ", session ID: " << header.session_id_;
        }
        break;
      }
      case someip_posix_common::someip::kNotification: {
        const someip_posix_common::someip::SomeIpReturnCode verify_return_code{
            someip_posix_common::someip::EventMessageVerification::DoInfrastructuralChecks(header)};
        if (verify_return_code == someip_posix_common::someip::SomeIpReturnCode::kOk) {
          RouteEventNotification(instance_id, header, std::move(deserializer));  // delegate to concrete proxy-binding
        } else {
          sub_logger_.LogError() << "Event message verification failed with return code " << verify_return_code
                                 << " - (" << std::hex << header.service_id_ << ", " << std::dec << instance_id << "): "
                                 << "event ID: " << header.method_id_ << ", session ID: " << header.session_id_;
        }

        break;
      }
      case someip_posix_common::someip::SomeIpMessageType::kRequest: {
        const someip_posix_common::someip::SomeIpReturnCode verify_return_code{
            someip_posix_common::someip::RequestMessageVerification::DoInfrastructuralChecks(header)};
        if (verify_return_code == someip_posix_common::someip::SomeIpReturnCode::kOk) {
          RouteMethodRequest(instance_id, header, std::move(deserializer));
        } else {
          // Infrastructural error
          SendErrorResponseOnMethodRequest(verify_return_code, instance_id, header);
          sub_logger_.LogError() << "Method request message verification failed with return code " << verify_return_code
                                 << " - (" << std::hex << header.service_id_ << ", " << std::dec << instance_id << "): "
                                 << "method ID: " << header.method_id_ << ", session ID: " << header.session_id_;
        }
        break;
      }
      case someip_posix_common::someip::SomeIpMessageType::kRequestNoReturn: {
        const someip_posix_common::someip::SomeIpReturnCode verify_return_code{
            someip_posix_common::someip::RequestNoReturnMessageVerification::DoInfrastructuralChecks(header)};
        if (verify_return_code == someip_posix_common::someip::SomeIpReturnCode::kOk) {
          RouteFireAndForgetMethodRequest(instance_id, header, std::move(deserializer));
        } else {
          // No error response message shall be sent for fire & forget methods. Print an error logging message only.
          sub_logger_.LogError() << "Fire & forget message verification failed with return code " << verify_return_code
                                 << " - (" << std::hex << header.service_id_ << ", " << std::dec << instance_id << "): "
                                 << "method ID: " << header.method_id_ << ", session ID: " << header.session_id_;
        }
        break;
      }
      case someip_posix_common::someip::kError: {
        const someip_posix_common::someip::SomeIpReturnCode verify_return_code{
            someip_posix_common::someip::ErrorMessageVerification::DoInfrastructuralChecks(header)};

        if (verify_return_code == someip_posix_common::someip::SomeIpReturnCode::kOk) {
          RouteMethodResponse(instance_id, header, std::move(deserializer));  // delegate to concrete proxy-binding
        } else {
          // No error response message shall be sent for fire & forget methods. Print an error logging message only.
          sub_logger_.LogError() << "Generic error response message verification failed with return code "
                                 << verify_return_code << ". Will not route this error response to a binding instance."
                                 << " - (" << std::hex << header.service_id_ << ", " << std::dec << instance_id << "): "
                                 << "method/event ID: " << header.method_id_ << ", session ID: " << header.session_id_;
        }
        break;
      }
      default: {
        sub_logger_.LogError() << "SOME/IP Binding: Unsupported message type: "
                               << static_cast<std::uint16_t>(header.message_type_);
        break;
      }
    }
  }

  /**
   * \brief Called on asynchronous notification about offered services.
   * HandleFindService is called every time a service instance is offered or has been stopped.
   *
   * \param service_instances Container of found service instances.
   */
  void HandleFindService(someip_posix_common::someip::ServiceInstanceContainer service_instances) override {
    sub_logger_.LogDebug() << __func__;
    // Treat each StartFindService job of a specific ServiceProxy as it would be a FindService call.
    // The service instances will arrive without any additional information from the communication layer.
    // The last service discovery entry contains the updated service instance.
    assert(!service_instances.empty());  // Identification entry at least.
    const someip_posix_common::someip::ServiceInstance update = service_instances.back();
    ProxyServiceObserverContainer::iterator it = findservice_proxy_observers_.find(update.service_id_);
    assert(it != findservice_proxy_observers_.end());
    ara::com::internal::ServiceUpdateObserver* proxy_observer = it->second;
    StartFindServiceJobsMap::iterator it_jobs = findservice_jobs_.find(update.service_id_);
    service_instances.pop_back();  // Remove the identification entry.

    if (it_jobs != findservice_jobs_.end()) {
      const StartFindServiceJobsPerService& job_entries = it_jobs->second;

      for (const auto& findservice_request : job_entries) {
        // Filter here, to prevent from calling a handler, that is requesting the service, but not the updated
        // instance ID.
        const someip_posix_common::someip::InstanceId requested_instance_id = findservice_request.first;

        if (requested_instance_id == update.instance_id_) {
          const ara::com::FindServiceHandle& handle = findservice_request.second;
          const ara::com::internal::ProxyId proxy_id = handle.proxy_id;
          auto factory_it = std::find_if(
              proxy_factories_.begin(), proxy_factories_.end(),
              [proxy_id](AraComSomeIpProxyFactoryInterface& factory) { return factory.GetProxyId() == proxy_id; });

          // Assemble one container of handles for each StartFindService job, that matches the criteria
          // of requested instance ID and the updated instance ID.
          ara::com::ServiceHandleContainer<ara::com::HandleType> aracom_handles;

          for (const auto& service_instance : service_instances) {
            sub_logger_.LogDebug() << "Placing entry for service ID: " << update.service_id_
                                   << ", with requested instance ID: " << requested_instance_id;

            AraComSomeIpProxyInstanceFactory::shared_ptr instance_factory_ptr =
                FindOrMakeProxyInstanceFactory(service_instance.instance_id_, factory_it->GetSelf());
            // TODO(visdtr) Map the concrete SOME/IP instance ID returned from the service discovery into
            // an ara::com InstanceIdentifier.
            const ara::com::InstanceIdentifier instance{handle.instance_id};
            aracom_handles.emplace_back(instance, instance_factory_ptr);
          }

          // Forward the service offer / stopped offer update to a concrete handler.
          proxy_observer->OnUpdate(findservice_request.second, aracom_handles);
        }
      }
    }
  }

  /**
   * \brief Called on asynchronous notification about event subscription state update.
   * HandleEventSubscriptionStateUpdate is called every time an event subscription state has been updated.
   *
   * \param event_subscription_state_entry The entry containing the event details with the current subscription state.
   */
  void HandleEventSubscriptionStateUpdate(
      const someip_posix_common::someip::EventSubscriptionState& event_subscription_state_entry) override {
    using ara::com::SubscriptionState;

    const someip_posix_common::someip::ServiceId service_id = event_subscription_state_entry.service_id_;
    const someip_posix_common::someip::InstanceId instance_id = event_subscription_state_entry.instance_id_;
    const someip_posix_common::someip::MethodId event_id = event_subscription_state_entry.event_id_;
    SubscriptionState state = SubscriptionState::kNotSubscribed;
    switch (event_subscription_state_entry.state_) {
      case someip_posix_common::someip::SubscriptionState::kSubscribed: {
        state = SubscriptionState::kSubscribed;
        break;
      }
      case someip_posix_common::someip::SubscriptionState::kNotSubscribed: {
        state = SubscriptionState::kNotSubscribed;
        break;
      }
      case someip_posix_common::someip::SubscriptionState::kSubscriptionPending: {
        state = SubscriptionState::kSubscriptionPending;
        break;
      }
      default: {
        assert(false);
        break;
      }
    }

    for (auto& proxy_binding : proxy_bindings_) {
      // Filter for service ID and instance ID, before the subscription state update is routed
      const auto& proxy_identity = proxy_binding.first;
      if ((std::get<0>(proxy_identity) == service_id) && (std::get<1>(proxy_identity) == instance_id)) {
        proxy_binding.second->HandleEventSubscriptionStateUpdate(event_id, state);
      }
    }
  }

  /**
   * \brief Convert a general ara::com::InstanceIdentifier to the respective SOME/IP-specific InstanceId.
   *
   * \param instance_id The ara::com::InstanceIdentifier which will be translated.
   *
   * \return the corresponding SOME/IP binding-specific instance identifier.
   */
  someip_posix_common::someip::InstanceId LookupSomeIpInstanceId(
      const ara::com::InstanceIdentifier& instance_id) override {
    // TODO(visdtr) Map the concrete SOME/IP instance ID returned from the service discovery into an ara::com
    // InstanceIdentifier.
    return instance_id.GetInstanceId();
  }

  /**
   * \brief Convert a SOME/IP-specific InstanceId to the respective general ara::com::InstanceIdentifier.
   *
   * \param instance_id The SOME/IP binding-specific instance identifier which will be translated.
   *
   * \return the corresponding ara::com::InstanceIdentifier .
   */
  ara::com::InstanceIdentifier LookupAraComInstanceId(
      const someip_posix_common::someip::InstanceId& instance_id) override {
    // TODO(visdtr) Map the concrete SOME/IP instance ID returned from the service discovery into an ara::com
    // InstanceIdentifier.
    return ara::com::InstanceIdentifier(instance_id);
  }

  /**
   * \brief Convert a AraComSomeIpProxyFactoryInterface to the respective SOME/IP-specific ServiceId
   *
   * \param factory The factory for getting the SOME/IP service identifier.
   *
   * \return the corresponding SOME/IP-specific service identifier.
   */
  someip_posix_common::someip::ServiceId LookupSomeIpServiceId(
      const AraComSomeIpProxyFactoryInterface& factory) override {
    return factory.GetSomeIpServiceId();
  }

  /**
   * \brief Getter for a new client id.
   *
   * \return A client id for a proxy binding.
   */
  someip_posix_common::someip::ClientId GetNextClientId() override {
    if (someip_posix_.IsRunning()) {
      return someip_posix_.GetClientId();
    } else {
      throw ara::com::BackendNotAvailableException();
    }
  }

  /**
   * \brief Registers one proxy binding on construction of a ServiceProxySomeIpBinding.
   *
   * \param proxy_identity Uniquely identify a proxy binding with a tuple of service id, instance id, client id.
   * \param proxy_binding A pointer to the constructed element to store in
   * the map proxy_bindings_. On method response / event notification, a look-up through the registered proxy bindings
   * occurs, to delegate the response.
   */
  void RegisterProxyBinding(ProxyBindingIdentity proxy_identity, AraComSomeIpProxyInterface* proxy_binding) override {
    assert(proxy_binding != nullptr);
    proxy_bindings_.emplace(proxy_identity, proxy_binding);

    someip_posix_.RequestService(std::get<0>(proxy_identity), std::get<1>(proxy_identity));
  }

  /**
   * \brief De-register one proxy binding from the map (e.g. a proxy service instance gets destructed).
   *
   * \param proxy_identity The key to search for and the associated value to erase from the map.
   */
  void DeRegisterProxyBinding(ProxyBindingIdentity proxy_identity) override {
    auto it = proxy_bindings_.find(proxy_identity);
    assert(it != proxy_bindings_.end());

    someip_posix_.ReleaseService(std::get<0>(proxy_identity), std::get<1>(proxy_identity));
    const someip_posix_common::someip::ClientId client_id = std::get<2>(proxy_identity);
    if (someip_posix_.IsRunning()) {
      someip_posix_.ReleaseClientId(client_id);
    } else {
      sub_logger_.LogError() << __func__ << " SOME/IP Daemon stopped.";
    }
    proxy_bindings_.erase(it);
  }

  /**
   * \brief Registers one skeleton binding on construction of a ServiceSkeletonSomeIpBinding.
   *
   * \param skeleton_identity Uniquely identify a proxy binding with a tuple of service id, instance id, client id.
   * \param skeleton_binding A pointer to the constructed element to store in
   * the map skeleton_bindings_. On method request / event request, a look-up through the registered skeleton
   * bindings happens, to delegate the request to the correct skeleton binding object.
   */
  void RegisterSkeletonBinding(SkeletonBindingIdentity skeleton_identity,
                               AraComSomeIpSkeletonInterface* skeleton_binding) override {
    assert(skeleton_binding != nullptr);
    skeleton_bindings_.emplace(skeleton_identity, skeleton_binding);
  }

  /**
   * \brief Actively de-register one skeleton binding (e.g. after StopOfferService gets called).
   *
   * \param skeleton_identity The key to search for and the associated value to erase from the map.
   */
  void DeRegisterSkeletonBinding(SkeletonBindingIdentity skeleton_identity) override {
    auto it = skeleton_bindings_.find(skeleton_identity);
    assert(it != skeleton_bindings_.end());
    skeleton_bindings_.erase(it);
  }

  /**
   * \brief Called when someip-posix is shutting down.
   *
   * \param instance The SomeIpPosix instance that is being shutdown.
   */
  void OnSomeIpPosixShutdown(SomeIpPosix& instance) override {
    sub_logger_.LogDebug() << __func__;
    (void)instance;
    // TODO(PAASR-2231) Move to separate class AraComSomeIpBindingStateManger
  }

  /**
   * \brief Processes next incoming packet for SOME/IP, when the Runtime is set to polling mode.
   *
   * \return true: one packet has been processed, false if there was no pending packet to process.
   */
  bool ProcessNextPacket() override { return someip_posix_.ProcessNextPacket(); }

 protected:
  /**
   * \brief Build the libSomeIP-posix specific configuration model from the abstract ara::com configuration model
   *
   * \param config The ara::com abstract configuration model.
   * \return A config data model for the SOME/IP related layers to access.
   */
  someip_posix_common::config::SomeIpPosixConfigModel BuildSomeIpPosixConfig(
      const configuration::Configuration& config) {
    /* Create libSomeIP-posix specific configuration model from ara::com model.
     * Configuration model shall be independent to avoid dependency to ara::com */
    someip_posix_common::config::SomeIpPosixConfigModel someipposix_config;
    someipposix_config.control_socket_path_ = config.GetControlSocketPath();
    someipposix_config.routing_socket_path_ = config.GetRoutingSocketPath();

    // Set processing mode (polling or SingleThreaded). Default: SingleThreaded
    if (config.GetProcessingMode() == configuration::RuntimeProcessingMode::kPolling) {
      someipposix_config.processing_mode_ = someip_posix_common::config::SomeIpBindingProcessingMode::kPolling;
    }

    return someipposix_config;
  }

 protected:
  /**
   * \brief Communication link to send and receive SOME/IP messages
   * \details Defined as protected member to make it unit-testable.
   **/
  SomeIpPosix someip_posix_;

 private:
  /**
   * \brief Delegate a method response to the correct proxy binding.
   *
   * \param instance_id The SOME/IP instance ID. The instance ID is normally mapped to one ip address + port.
   * \param header The already deserialized SOME/IP header.
   * \param deserializer Deserializer holding the serialized stream.
   */
  void RouteMethodResponse(someip_posix_common::someip::InstanceId instance_id,
                           const someip_posix_common::someip::SomeIpMessageHeader& header,
                           RootDeserializerAlias&& deserializer) {
    const someip_posix_common::someip::ServiceId sid = header.service_id_;
    const someip_posix_common::someip::ClientId cid = header.client_id_;
    auto it = proxy_bindings_.find(std::make_tuple(sid, instance_id, cid));

    if (it != proxy_bindings_.end()) {
      it->second->HandleMethodResponse(header, std::move(deserializer));
    } else {
      sub_logger_.LogError()
          << "SOME/IP Binding: Routing method response failed because no proxy binding is available - "
          << "service ID:" << sid << ", client ID:" << cid << ", instance ID:" << instance_id;
    }
  }

  /**
   * \brief Route an incoming fire & forget request from the client to the
   * specific skeleton-binding on the server-side. This will exclude SOME/IP
   * message verification as it is stated in the SOME/IP PRS.
   *
   * \param instance_id The SOME/IP instance ID, the client wants to address.
   * \param header The already deserialized SOME/IP header.
   * \param deserializer Deserializer holding the serialized stream.
   */
  void RouteFireAndForgetMethodRequest(someip_posix_common::someip::InstanceId instance_id,
                                       const someip_posix_common::someip::SomeIpMessageHeader& header,
                                       RootDeserializerAlias&& deserializer) {
    const someip_posix_common::someip::ServiceId sid = header.service_id_;
    auto it = skeleton_bindings_.find(std::make_tuple(sid, instance_id));

    if (it != skeleton_bindings_.end()) {
      it->second->HandleMethodRequest(header, std::move(deserializer));
    } else {
      sub_logger_.LogError() << "SOME/IP Binding: Routing fire & forget method request failed because no skeleton "
                                "binding instance is available - "
                             << "service ID:" << sid << ", instance ID:" << instance_id;
    }
  }

  /**
   * \brief Route an incoming request from the client to the specific skeleton-binding on the server-side.
   *
   * \param instance_id The SOME/IP instance ID, the client wants to address.
   * \param header The already deserialized SOME/IP header.
   * \param deserializer Deserializer holding the serialized stream.
   */
  void RouteMethodRequest(someip_posix_common::someip::InstanceId instance_id,
                          const someip_posix_common::someip::SomeIpMessageHeader& header,
                          RootDeserializerAlias&& deserializer) {
    const someip_posix_common::someip::ServiceId sid = header.service_id_;
    auto it = skeleton_bindings_.find(std::make_tuple(sid, instance_id));

    if (it != skeleton_bindings_.end()) {
      it->second->HandleMethodRequest(header, std::move(deserializer));
    } else {
      // If there is no service instance available, send an error message.
      SendErrorResponseOnMethodRequest(::someip_posix_common::someip::SomeIpReturnCode::kUnknownService, instance_id,
                                       header);
    }
  }

  /**
   * \brief Delegate an event notification to the correct proxy binding.
   *
   * \param instance_id The SOME/IP instance ID
   * \param header The already deserialized SOME/IP header.
   * \param deserializer Deserializer holding the serialized stream.
   */
  void RouteEventNotification(someip_posix_common::someip::InstanceId instance_id,
                              const ::someip_posix_common::someip::SomeIpMessageHeader& header,
                              RootDeserializerAlias&& deserializer) {
    const someip_posix_common::someip::ServiceId sid = header.service_id_;

    for (auto& proxy_binding : proxy_bindings_) {
      const auto& proxy_identity = proxy_binding.first;
      // Filter for service ID and instance ID, before an event is routed
      if ((std::get<0>(proxy_identity) == sid) && (std::get<1>(proxy_identity) == instance_id)) {
        proxy_binding.second->HandleEventNotification(header, std::move(deserializer));
      }
    }
  }

  /**
   * \brief Obtain a reference-counted ProxyInstanceFactory
   *
   * \param instance_id the InstanceId to create service instances for
   * \param proxy_factory_interface The factory for the ServiceInterface to create instances for
   *
   * \returns An intrusive_shared_ptr to an AraComSomeIpProxyInstanceFactory
   */
  AraComSomeIpProxyInstanceFactory::shared_ptr FindOrMakeProxyInstanceFactory(
      someip_posix_common::someip::InstanceId instance_id, AraComSomeIpProxyFactoryInterface* proxy_factory_interface) {
    // Verify if there is a factory instance available
    AraComSomeIpProxyInstanceFactory::PoolType::iterator pool_it = std::find_if(
        proxy_instance_factory_pool_.begin(), proxy_instance_factory_pool_.end(),
        [instance_id, proxy_factory_interface](const AraComSomeIpProxyInstanceFactory& instance_factory_it) {
          return instance_factory_it.compare(instance_id, proxy_factory_interface);
        });

    AraComSomeIpProxyInstanceFactory::shared_ptr instance_factory_ptr;

    if (pool_it == proxy_instance_factory_pool_.end()) {
      // Create a new factory instance
      proxy_instance_factory_pool_.emplace_back(instance_id, proxy_factory_interface, proxy_instance_factory_pool_);
      instance_factory_ptr = proxy_instance_factory_pool_.back();
    } else {
      instance_factory_ptr = *pool_it;
    }
    return instance_factory_ptr;
  }

  /**
   * \brief Constant for the number of concurrent proxy instance factories supported by this binding.
   *
   * This is a temporary value that should eventually be read from the configuration.
   * TODO(PAASR-1548): Remove this constant
   */
  static constexpr std::uint32_t kNumProxyInstanceFactories = 255U;

  /**
   * \brief Container for SomeIProxyFactory instances.
   */
  using ProxyFactoryContainer = vac::container::IntrusiveList<AraComSomeIpProxyFactoryInterface>;

  /**
   * \brief Container type for SomeIpSkeletonFactory instances.
   */
  using SkeletonFactoryContainer = vac::container::IntrusiveList<AraComSomeIpSkeletonFactoryInterface>;

  /**
   * \brief Container type for SomeIProxyFactory instances.
   *
   * There will be exactly one instance of a SomeIpProxyFactory for each SomeIpProxy Implementation within this binding.
   * There will be one SomeIpProxy Implementation for every supported combination of ServiceID/ServiceVersion of a
   * ServiceInterface.
   *
   * Example: For an application that is aware of ServiceProxyA, Version 1.0 and ServiceProxyB, Version 2.5 a total of
   * two SomeIpProxy Implementations and two SomeIpProxyFactories will exist in the entire program.
   */
  ProxyFactoryContainer proxy_factories_;

  /**
   * \brief Container type for SomeIpSkeletonFactory instances.
   *
   * The comments on multiplicity of factories from proxy_factories_ apply here as well.
   */
  SkeletonFactoryContainer skeleton_factories_;

  /**
   * \brief Registered proxy binding objects. This back-link is needed for the routing of method responses,
   * event notifications.
   */
  using ServiceProxySomeIpBindings = std::map<ProxyBindingIdentity, AraComSomeIpProxyInterface*>;

  /**
   * \brief Registered skeleton binding objects. This back-link is needed for the routing of method requests,
   * event requests.
   */
  using ServiceSkeletonSomeIpBindings = std::map<SkeletonBindingIdentity, AraComSomeIpSkeletonInterface*>;

  /**
   * Container of all proxy bindings registered.
   */
  ServiceProxySomeIpBindings proxy_bindings_;

  /**
   * \brief Container of all skeleton bindings registered after an OfferService from a service instance.
   */
  ServiceSkeletonSomeIpBindings skeleton_bindings_;

  /**
   * \brief A type-def for a container, that holds all active StartFindService jobs.
   * One service will be registered once, that belongs to one ServiceUpdateObserver of a ServiceProxy.
   */
  using ProxyServiceObserverContainer =
      std::map<someip_posix_common::someip::ServiceId, ara::com::internal::ServiceUpdateObserver*>;

  /**
   * \brief Active FindService jobs container, that holds the observer for each service identifier.
   */
  ProxyServiceObserverContainer findservice_proxy_observers_;

  /**
   * \brief For every service ID, there may be multiple StartFindServiceJobs pending.
   */
  using StartFindServiceJobsPerService =
      std::vector<std::pair<someip_posix_common::someip::InstanceId, ara::com::FindServiceHandle>>;

  /**
   * \brief StartFindService jobs
   * Cluster by SOME/IP service identifier
   */
  using StartFindServiceJobsMap = std::map<someip_posix_common::someip::ServiceId, StartFindServiceJobsPerService>;

  /** Active FindService jobs clustered by the service identifier, which leads to the FindServiceHandle. */
  StartFindServiceJobsMap findservice_jobs_;

  /** Reference to the logger */
  ara::log::Logger& sub_logger_;

  /** Container for reference-counted AraComSomeIpProxyInstanceFactory instances */
  AraComSomeIpProxyInstanceFactory::PoolType proxy_instance_factory_pool_;
};

}  // namespace someip_posix
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_ARACOM_SOMEIP_BINDING_H_

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
/**        \file  aracom_someip_binding_interface.h
 *         \brief Interface of AraComSomeIpBinding
 *
 *         \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_ARACOM_SOMEIP_BINDING_INTERFACE_H_
#define LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_ARACOM_SOMEIP_BINDING_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <tuple>
#include "ara-someip-posix/aracom_someip_proxy_factory_interface.h"
#include "ara-someip-posix/aracom_someip_proxy_interface.h"
#include "ara-someip-posix/aracom_someip_skeleton_factory_interface.h"
#include "ara-someip-posix/aracom_someip_skeleton_interface.h"
#include "ara/com/internal/binding_interface.h"
#include "someip-posix-common/someip/message.h"
#include "someip-posix/someip_posix.h"

namespace ara {
namespace com {
namespace someip_posix {

/// Uniquely identify a proxy binding with a tuple of service id, instance id, client id.
using ProxyBindingIdentity = std::tuple<someip_posix_common::someip::ServiceId, someip_posix_common::someip::InstanceId,
                                        someip_posix_common::someip::ClientId>;

/// For each offered service there is one binding-related skeleton-part, which processes
/// incoming method requests / event requests.
using SkeletonBindingIdentity =
    std::tuple<someip_posix_common::someip::ServiceId, someip_posix_common::someip::InstanceId>;

/**
 * \brief Realizes a concrete Binding for the proxy/skeleton communication via SOME/IP and serves as an
 * anchor between the application-side and the communication protocol of SOME/IP.
 * For each concrete binding like this, there is only one object of this type available at runtime (singleton).
 */
class AraComSomeIpBindingInterface : public ara::com::internal::BindingInterface,
                                     public ::someip_posix::SomeipPosixApplicationInterface {
 public:
  /**
 * \brief Destructor
 */
  virtual ~AraComSomeIpBindingInterface() = default;

  /**
   * \brief Generated method `Initialize` registers all the generated proxy and skeleton
   * classes with its corresponding binding and creates an interface for proxies or skeletons to use this
   * binding.
   * \remark Implementation of this method prototype is generated and shall be realized in a separate
   * translation unit (.cc). This method is then called by the Runtime, which loops through all the
   * existing bindings.
   */
  virtual void Initialize() = 0;

  /**
   * \brief Execute a synchronous FindService call.
   *
   * \param proxy_id The ara::com::internal::ProxyId to search for
   * \param instance_id The ara::com::InstanceIdentifier for which to find a service
   *
   * \returns A container of HandleTypes for the currently known services
   */
  virtual ServiceHandleContainer<HandleType> FindService(ara::com::internal::ProxyId proxy_id,
                                                         const ara::com::InstanceIdentifier& instance_id) = 0;

  /**
   *  \brief Triggers asynchronous SOME/IP service lookup according to the ARA::COM specification.
   *
   *  \param handle Created handle for this asynchronous FindService job.
   *  \param observer A reference to the observer object, that delegates the asynchronous notifications.
   */
  virtual void StartFindService(const ara::com::FindServiceHandle& handle,
                                ara::com::internal::ServiceUpdateObserver& observer) = 0;
  /**
   * \brief Stop asynchronous service discovery.
   *
   * \param handle the handle for the asynchronous service discovery to cancel.
   */
  virtual void StopFindService(const FindServiceHandle& handle) = 0;

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
  virtual std::unique_ptr<internal::SkeletonImplInterface> OfferService(
      ara::com::internal::SkeletonId skeleton_id, const ara::com::InstanceIdentifier& instance_id,
      ara::com::internal::SkeletonInterface& skeleton) = 0;

  /**
   * \brief Cancel the offering of a specific service instance from the skeleton.
   *
   * \param skeleton_id the ara::com::internal::SkeletonId for which to stop offering a service
   * \param instance_id The ara::com::InstanceIdentifier for which to stop offering a service
   */
  virtual void StopOfferService(ara::com::internal::SkeletonId skeleton_id,
                                const ara::com::InstanceIdentifier& instance_id) = 0;

  /**
   * \brief Lets the SOME/IP binding know that a proxy wishes to receive an event of a service instance.
   *
   * \param service_id SOME/IP service id of the service.
   * \param instance_id SOME/IP instance id of the service.
   * \param event_id SOME/IP event id of the service.
   */
  virtual void SubscribeEvent(someip_posix_common::someip::ServiceId service_id,
                              someip_posix_common::someip::InstanceId instance_id,
                              someip_posix_common::someip::EventId event_id) = 0;

  /**
   * \brief Lets the SOME/IP binding know that an proxy does not wish to receive an event of a service instance.
   *
   * \param service_id SOME/IP service id of the service.
   * \param instance_id SOME/IP instance id of the service.
   * \param event_id SOME/IP event id of the service.
   */
  virtual void UnsubscribeEvent(someip_posix_common::someip::ServiceId service_id,
                                someip_posix_common::someip::InstanceId instance_id,
                                someip_posix_common::someip::EventId event_id) = 0;

  /**
   * \brief An instantiated skeleton implementation will send an event notification to the client.
   *
   * \param instance_id The SOME/IP instance id of the skeleton-binding
   * \param packet Contains the complete SomeIpPacket of header and payload.
   */
  virtual void SendEventNotification(someip_posix_common::someip::InstanceId instance_id,
                                     someip_posix_common::someip::SomeIpPacket&& packet) = 0;

  /**
   * \brief An instantiated proxy will send a method request to a server.
   *
   * \param instance_id The SOME/IP instance id of the proxy binding
   * \param packet Contains the complete SomeIpPacket of header and payload.
   */
  virtual void SendMethodRequest(someip_posix_common::someip::InstanceId instance_id,
                                 someip_posix_common::someip::SomeIpPacket&& packet) = 0;

  /**
   * \brief Is needed for infrastructural faults such as:
   * Deserialization failed or the service/method isn't implemented.
   *
   * \param return_code The return code to write into the SOME/IP header.
   * \param instance_id The SOME/IP instance id of the proxy binding.
   * \param request_header The unmodified SOME/IP header from the request.
   * This SOME/IP header will be used for assembling the error response
   * and is partially being reused.
   */
  virtual void SendErrorResponseOnMethodRequest(
      const someip_posix_common::someip::ReturnCode return_code,
      const someip_posix_common::someip::InstanceId instance_id,
      const ::someip_posix_common::someip::SomeIpMessageHeader& request_header) = 0;

  /**
   * \brief On a method request/response, this method is called by the transport layer.
   *
   * \param instance_id the SOME/IP instance id which is normally mapped to one socket (IP address + port)
   * \param packet the SOME/IP message including header and payload
   */
  virtual void HandleReceive(someip_posix_common::someip::InstanceId instance_id,
                             someip_posix_common::someip::SomeIpPacket packet) = 0;

  /**
   * \brief Called on asynchronous notification about offered services.
   * HandleFindService is called every time a service instance is offered or has been stopped.
   *
   * \param service_instances Container of found service instances.
   */
  virtual void HandleFindService(someip_posix_common::someip::ServiceInstanceContainer service_instances) = 0;

  /**
   * \brief Called on asynchronous notification about event subscription state update.
   * HandleEventSubscriptionStateUpdate is called every time an event subscription state is updated.
   *
   * \param event_subscription_state_entry The entry containing the event details with the current subscription state.
   */
  virtual void HandleEventSubscriptionStateUpdate(
      const someip_posix_common::someip::EventSubscriptionState& event_subscription_state_entry) = 0;

  /**
   * \brief Convert a general ara::com::InstanceIdentifier to the respective SOME/IP-specific InstanceId.
   *
   * \param instance_id The ara::com::InstanceIdentifier which will be translated.
   *
   * \return the corresponding SOME/IP binding-specific instance identifier.
   */
  virtual someip_posix_common::someip::InstanceId LookupSomeIpInstanceId(
      const ara::com::InstanceIdentifier& instance_id) = 0;

  /**
   * \brief Convert a SOME/IP-specific InstanceId to the respective general ara::com::InstanceIdentifier.
   *
   * \param instance_id The SOME/IP binding-specific instance identifier which will be translated.
   *
   * \return the corresponding ara::com::InstanceIdentifier .
   */
  virtual ara::com::InstanceIdentifier LookupAraComInstanceId(
      const someip_posix_common::someip::InstanceId& instance_id) = 0;

  /**
   * \brief Convert a AraComSomeIpProxyFactoryInterface to the respective SOME/IP-specific ServiceId
   *
   * \param factory The factory for getting the SOME/IP service identifier.
   *
   * \return the corresponding SOME/IP-specific service identifier.
   */
  virtual someip_posix_common::someip::ServiceId LookupSomeIpServiceId(
      const AraComSomeIpProxyFactoryInterface& factory) = 0;

  /**
   * \brief Getter for a client id for a specific proxy-binding instance on SOME/IP level.
   *
   * \return a unique client id, that is used for identification of a proxy-binding on method/event responses.
   */
  virtual someip_posix_common::someip::ClientId GetNextClientId() = 0;

  /**
   * \brief Registers one proxy binding on construction of a ServiceProxySomeIpBinding.
   *
   * \param proxy_identity Uniquely identify a proxy binding with a tuple of service id, instance id, client id.
   * \param proxy_binding A pointer to the constructed element to store in
   * the map proxy_bindings_. On method response / event notification, a look-up through the registered proxy bindings
   * occurs, to delegate the response.
   */
  virtual void RegisterProxyBinding(ProxyBindingIdentity proxy_identity, AraComSomeIpProxyInterface* proxy_binding) = 0;

  /**
   * \brief De-register one proxy binding from the map (e.g. a proxy service instance gets destructed).
   *
   * \param proxy_identity The key to search for and the associated value to erase from the map.
   */
  virtual void DeRegisterProxyBinding(ProxyBindingIdentity proxy_identity) = 0;

  /**
   * \brief Registers one skeleton binding on construction of a ServiceSkeletonSomeIpBinding.
   *
   * \param skeleton_identity Uniquely identify a proxy binding with a tuple of service id, instance id, client id.
   * \param skeleton_binding A pointer to the constructed element to store in
   * the map skeleton_bindings_. On method request / event request, a look-up through the registered skeleton
   * bindings happens, to delegate the request to the correct skeleton binding object.
   */
  virtual void RegisterSkeletonBinding(SkeletonBindingIdentity skeleton_identity,
                                       AraComSomeIpSkeletonInterface* skeleton_binding) = 0;

  /**
   * \brief Actively de-register one skeleton binding (e.g. after StopOfferService gets called).
   *
   * \param skeleton_identity The key to search for and the associated value to erase from the map.
   */
  virtual void DeRegisterSkeletonBinding(SkeletonBindingIdentity skeleton_identity) = 0;
};

}  // namespace someip_posix
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_ARACOM_SOMEIP_BINDING_INTERFACE_H_

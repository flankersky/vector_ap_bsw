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
/**        \file  packet_router.h
 *        \brief  Packet router
 *
 *      \details  The packet router is responsible for forwarding SOME/IP method requests, method responses, and events
 *                to corresponding service consumers or providers.
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_PACKET_ROUTER_PACKET_ROUTER_H_
#define SRC_SOMEIPD_POSIX_PACKET_ROUTER_PACKET_ROUTER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <map>
#include <memory>
#include <tuple>
#include <vector>

#include "ara/log/logging.hpp"
#include "osabstraction/io/reactor_interface.h"
#include "someip-posix-common/someip/message.h"
#include "someipd-posix/configuration/configuration.h"
#include "someipd-posix/packet_router/packet_cache.h"
#include "someipd-posix/packet_router/packet_router_interface.h"
#include "vac/language/cpp14_backport.h"

namespace someipd_posix {
namespace packet_router {

/**
 * \brief Key for looking up the destination of a SOME/IP request message.
 */
struct RequestSinkKey {
  someip_posix_common::someip::ServiceId service_id;    ///< A SOME/IP service identifier
  someip_posix_common::someip::InstanceId instance_id;  ///< A SOME/IP service instance identifier

  /**
   * \brief Defines a strict weak ordering relationship for request sink keys.
   *
   * \param other A second key that is compared to.
   * \return true - This key is less than the second one,
   *         false - This key is not less than the second one.
   */
  bool operator<(const RequestSinkKey& other) const {
    return std::make_tuple(this->service_id, this->instance_id) < std::make_tuple(other.service_id, other.instance_id);
  }
};

/**
 * \brief Represents an information for routing a SOME/IP method response message back to the client.
 */
struct ResponseRoute {
  someip_posix_common::someip::ServiceId service_id_;    ///< A SOME/IP service identifier
  someip_posix_common::someip::InstanceId instance_id_;  ///< A SOME/IP service instance identifier
  someip_posix_common::someip::ClientId client_id_;      ///< A SOME/IP client identifier
  someip_posix_common::someip::SessionId session_id_;    ///< A SOME/IP session identifier
  std::shared_ptr<PacketSink> from_;                     ///< Packet sink that the request came from
};

/**
 * \brief Key for looking up the destination of a SOME/IP event.
 */
struct EventKey {
  someip_posix_common::someip::ServiceId service_id;    ///< A SOME/IP service identifier
  someip_posix_common::someip::InstanceId instance_id;  ///< A SOME/IP service instance identifier
  someip_posix_common::someip::EventId event_id;        ///< A SOME/IP event identifier

  /**
   * \brief Defines a strict weak ordering relationship for event keys.
   *
   * \param other A second key that is compared to.
   * \return true - This key is less than the second one,
   *         false - This key is not less than the second one.
   */
  bool operator<(const EventKey& other) const {
    return std::make_tuple(this->service_id, this->instance_id, this->event_id) <
           std::make_tuple(other.service_id, other.instance_id, other.event_id);
  }
};

/**
 * \brief Key for looking up the destination of a SOME/IP eventgroup.
 */
struct EventgroupKey {
  someip_posix_common::someip::ServiceId service_id;        ///< A SOME/IP service identifier
  someip_posix_common::someip::InstanceId instance_id;      ///< A SOME/IP service instance identifier
  someip_posix_common::someip::EventgroupId eventgroup_id;  ///< A SOME/IP eventgroup identifier

  /**
   * \brief Defines a strict weak ordering relationship for eventgroup keys.
   *
   * \param other A second key that is compared to.
   * \return true - This key is less than the second one,
   *         false - This key is not less than the second one.
   */
  bool operator<(const EventgroupKey& other) const {
    return std::make_tuple(this->service_id, this->instance_id, this->eventgroup_id) <
           std::make_tuple(other.service_id, other.instance_id, other.eventgroup_id);
  }
};

/**
 * \brief Key for looking up the field cache table of a given service instance.
 */
struct FieldCacheKey {
  someip_posix_common::someip::ServiceId service_id;    ///< A SOME/IP service identifier
  someip_posix_common::someip::InstanceId instance_id;  ///< A SOME/IP service instance identifier

  /**
   * \brief Defines a strict weak ordering relationship for request cache keys.
   *
   * \param other A second key that is compared to.
   * \return true - This key is less than the second one,
   *         false - This key is not less than the second one.
   */
  bool operator<(const FieldCacheKey& other) const {
    return std::make_tuple(this->service_id, this->instance_id) < std::make_tuple(other.service_id, other.instance_id);
  }
};

/**
 * \brief PacketRouter.
 */
class PacketRouter : public PacketRouterInterface {
 public:
  /**
   * \brief Constructor of PacketRouter.
   *
   * \param config A configuration.
   */
  explicit PacketRouter(const configuration::Configuration* config);
  /**
   * \brief Destructor of PacketRouter.
   */
  virtual ~PacketRouter() = default;
  /**
   * \brief PacketRouter is not copy-constructable.
   */
  PacketRouter(const PacketRouter& other) = delete;
  /**
   * \brief PacketRouter is not copy-assignable.
   */
  PacketRouter& operator=(const PacketRouter& other) = delete;
  /**
   * \brief Adds a new routing entry for SOME/IP method requests.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   * \param to A destination packet sink.
   */
  void AddRequestRoute(someip_posix_common::someip::ServiceId service_id,
                       someip_posix_common::someip::InstanceId instance_id, std::shared_ptr<PacketSink> to) override;
  /**
   * \brief Deletes a previously added routing entry for SOME/IP method requests.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   */
  void DeleteRequestRoute(someip_posix_common::someip::ServiceId service_id,
                          someip_posix_common::someip::InstanceId instance_id) override;
  /**
   * \brief Adds a new routing entry for a SOME/IP event.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   * \param event_id SOME/IP event id.
   * \param to A destination packet sink.
   */
  void AddEventRoute(someip_posix_common::someip::ServiceId service_id,
                     someip_posix_common::someip::InstanceId instance_id, someip_posix_common::someip::EventId event_id,
                     std::shared_ptr<PacketSink> to) override;
  /**
   * \brief Deletes a previously added routing entry for a SOME/IP event.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   * \param event_id SOME/IP event id.
   * \param to A destination packet sink.
   */
  void DeleteEventRoute(someip_posix_common::someip::ServiceId service_id,
                        someip_posix_common::someip::InstanceId instance_id,
                        someip_posix_common::someip::EventId event_id,
                        std::shared_ptr<packet_router::PacketSink> to) override;
  /**
   * \brief Adds a new routing entry for a SOME/IP eventgroup.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   * \param eventgroup_id SOME/IP eventgroup id.
   * \param to A destination packet sink.
   */
  void AddEventgroupRoute(someip_posix_common::someip::ServiceId service_id,
                          someip_posix_common::someip::InstanceId instance_id,
                          someip_posix_common::someip::EventgroupId eventgroup_id,
                          std::shared_ptr<PacketSink> to) override;
  /**
   * \brief Deletes a previously added routing entry for a SOME/IP eventgroup.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   * \param eventgroup_id SOME/IP eventgroup id.
   * \param to A destination packet sink.
   */
  void DeleteEventgroupRoute(someip_posix_common::someip::ServiceId service_id,
                             someip_posix_common::someip::InstanceId instance_id,
                             someip_posix_common::someip::EventgroupId eventgroup_id,
                             std::shared_ptr<packet_router::PacketSink> to) override;
  /**
   * \brief Forwards a SOME/IP message.
   *
   * \param instance_id SOME/IP instance id.
   * \param from A packet sink from which the passed SOME/IP message comes.
   * \param packet A SOME/IP message.
   */
  void Forward(someip_posix_common::someip::InstanceId instance_id, std::shared_ptr<PacketSink> from,
               Packet packet) override;
  /**
   * \brief Sends the given event-mapped field cached value to the passed sink.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   * \param event_id SOME/IP event id.
   * \param sink A packet sink.
   */
  void SendInitialEvent(const someip_posix_common::someip::ServiceId service_id,
                        const someip_posix_common::someip::InstanceId instance_id,
                        const someip_posix_common::someip::EventId event_id, PacketSink& sink) override;
  /**
   * \brief Sends all event-mapped field cached values of the given eventgroup to the passed TCP and UDP sinks.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   * \param eventgroup_id SOME/IP eventgroup id.
   * \param tcp_packet_sink TCP packet sink.
   * \param udp_packet_sink UDP packet sink.
   */
  void SendInitialEvents(const someip_posix_common::someip::ServiceId service_id,
                         const someip_posix_common::someip::InstanceId instance_id,
                         const someip_posix_common::someip::EventgroupId eventgroup_id, PacketSink* tcp_packet_sink,
                         PacketSink* udp_packet_sink) override;
  /**
   * \brief Cleans up request routing table entries for the passed packet sink.
   *
   * \param sink A packet sink.
   */
  void CleanUpRequestRoutingTableEntries(const PacketSink* sink) override;
  /**
   * \brief Cleans up response routing table entries for the passed packet sink.
   *
   * \param sink A packet sink.
   */
  void CleanUpResponseRoutingTableEntries(const PacketSink* sink) override;
  /**
   * \brief Cleans up event routing table entries for the passed packet sink.
   *
   * \param sink A packet sink.
   */
  void CleanUpEventRoutingTableEntries(const PacketSink* sink) override;
  /**
   * \brief Cleans up eventgroup routing table entries for the passed packet sink.
   *
   * \param sink A packet sink.
   */
  void CleanUpEventgroupRoutingTableEntries(const PacketSink* sink) override;

 protected:
  /**
   * \brief Field notification cache table.
   */
  using FieldCacheTable = std::map<FieldCacheKey, PacketCache<someip_posix_common::someip::EventId>>;
  /**
   * \brief Forwards a SOME/IP request message.
   *
   * \param instance_id SOME/IP instance id.
   * \param from A packet sink from which the passed SOME/IP request message comes.
   * \param packet A SOME/IP request message.
   */
  void ForwardRequest(someip_posix_common::someip::InstanceId instance_id, std::shared_ptr<PacketSink> from,
                      Packet packet);
  /**
   * \brief Forwards a SOME/IP fire & forget request message.
   *
   * \param instance_id SOME/IP instance id.
   * \param from A packet sink from which the passed SOME/IP request message comes.
   * \param packet A SOME/IP request message.
   */
  void ForwardRequestNoReturn(someip_posix_common::someip::InstanceId instance_id, std::shared_ptr<PacketSink> from,
                              Packet packet);
  /**
   * \brief Forwards a SOME/IP response message.
   *
   * \param instance_id SOME/IP instance id.
   * \param packet A SOME/IP response message.
   */
  void ForwardResponse(someip_posix_common::someip::InstanceId instance_id, Packet packet);
  /**
   * \brief Forwards a SOME/IP event message.
   *
   * \param instance_id SOME/IP instance id.
   * \param packet A SOME/IP event message.
   */
  void ForwardEvent(someip_posix_common::someip::InstanceId instance_id, Packet packet);
  /**
   * \brief Gets the field cache table.
   *
   * \return Field cache table.
   */
  const FieldCacheTable& GetFieldCacheTable();
  /**
   * \brief Cleans up field cache table entries for the passed service instance.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   */
  void CleanUpFieldCacheTableEntries(const someip_posix_common::someip::ServiceId service_id,
                                     const someip_posix_common::someip::InstanceId instance_id);

 private:
  /**
   * \brief Packet sink container.
   */
  using PacketSinkContainer = std::vector<std::shared_ptr<PacketSink>>;
  /**
   * \brief Request routing table.
   */
  using RequestRoutingTable = std::map<RequestSinkKey, std::shared_ptr<PacketSink>>;
  /**
   * \brief Response routing table.
   */
  using ResponseRoutingTable = std::vector<ResponseRoute>;
  /**
   * \brief Event routing table.
   */
  using EventRoutingTable = std::map<EventKey, PacketSinkContainer>;
  /**
   * \brief Eventgroup routing table.
   */
  using EventgroupRoutingTable = std::map<EventgroupKey, PacketSinkContainer>;
  /**
   * \brief A configuration.
   */
  const configuration::Configuration* config_;
  /**
   * \brief Out logger.
   */
  ara::log::Logger& logger_;
  /**
   * \brief Request routing table.
   */
  RequestRoutingTable request_routing_table_;
  /**
   * \brief Response routing table.
   */
  ResponseRoutingTable response_routing_table_;
  /**
   * \brief Event routing table.
   */
  EventRoutingTable event_routing_table_;
  /**
   * \brief Eventgroup routing table.
   */
  EventgroupRoutingTable eventgroup_routing_table_;
  /**
   * \brief Field value cache table.
   * Stores the values of the event-mapped-fields, to be sent to new field subscribers.
   */
  FieldCacheTable field_cache_table_;
};

}  // namespace packet_router
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_PACKET_ROUTER_PACKET_ROUTER_H_

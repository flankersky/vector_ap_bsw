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
/**        \file  packet_router_interface.h
 *        \brief  Packet Router Interface
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_PACKET_ROUTER_PACKET_ROUTER_INTERFACE_H_
#define SRC_SOMEIPD_POSIX_PACKET_ROUTER_PACKET_ROUTER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <memory>
#include <tuple>
#include "someip-posix-common/someip/someip_message.h"
#include "vac/memory/optional.h"

namespace someipd_posix {
namespace packet_router {

/**
 * \brief Represents a SOME/IP message.
 */
using Packet = std::shared_ptr<someip_posix_common::someip::SomeIpMessage>;

/**
 * \brief PacketSink.
 */
class PacketSink {
 public:
  /**
   * \brief Destructor of PacketSink.
   */
  virtual ~PacketSink() = default;

  /**
   * \brief Forwards a SOME/IP message to its destination.
   *
   * \param instance_id A SOME/IP service instance id.
   * \param packet A SOME/IP message.
   */
  virtual void Forward(someip_posix_common::someip::InstanceId instance_id, Packet packet) = 0;
};

/**
 * \brief PacketRouter.
 */
class PacketRouterInterface {
 public:
  /**
   * \brief Destructor of PacketRouterInterface.
   */
  virtual ~PacketRouterInterface() = default;

  /**
   * \brief Adds a new routing entry for SOME/IP method requests.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   * \param to A destination packet sink.
   */
  virtual void AddRequestRoute(someip_posix_common::someip::ServiceId service_id,
                               someip_posix_common::someip::InstanceId instance_id, std::shared_ptr<PacketSink> to) = 0;
  /**
   * \brief Deletes a previously added routing entry for SOME/IP method requests.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   */
  virtual void DeleteRequestRoute(someip_posix_common::someip::ServiceId service_id,
                                  someip_posix_common::someip::InstanceId instance_id) = 0;
  /**
   * \brief Adds a new routing entry for a SOME/IP event.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   * \param event_id SOME/IP event id.
   * \param to A destination packet sink.
   */
  virtual void AddEventRoute(someip_posix_common::someip::ServiceId service_id,
                             someip_posix_common::someip::InstanceId instance_id,
                             someip_posix_common::someip::EventId event_id, std::shared_ptr<PacketSink> to) = 0;
  /**
   * \brief Deletes a previously added routing entry for a SOME/IP event.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   * \param event_id SOME/IP event id.
   * \param to A destination packet sink.
   */
  virtual void DeleteEventRoute(someip_posix_common::someip::ServiceId service_id,
                                someip_posix_common::someip::InstanceId instance_id,
                                someip_posix_common::someip::EventId event_id,
                                std::shared_ptr<packet_router::PacketSink> to) = 0;
  /**
   * \brief Adds a new routing entry for a SOME/IP eventgroup.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   * \param eventgroup_id SOME/IP event id.
   * \param to A destination packet sink.
   */
  virtual void AddEventgroupRoute(someip_posix_common::someip::ServiceId service_id,
                                  someip_posix_common::someip::InstanceId instance_id,
                                  someip_posix_common::someip::EventgroupId eventgroup_id,
                                  std::shared_ptr<PacketSink> to) = 0;
  /**
   * \brief Deletes a previously added routing entry for a SOME/IP eventgroup.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   * \param eventgroup_id SOME/IP event id.
   * \param to A destination packet sink.
   */
  virtual void DeleteEventgroupRoute(someip_posix_common::someip::ServiceId service_id,
                                     someip_posix_common::someip::InstanceId instance_id,
                                     someip_posix_common::someip::EventgroupId eventgroup_id,
                                     std::shared_ptr<packet_router::PacketSink> to) = 0;
  /**
   * \brief Forwards a SOME/IP message.
   *
   * \param instance_id SOME/IP instance id.
   * \param from A packet sink from which the passed SOME/IP message comes.
   * \param packet A SOME/IP message.
   */
  virtual void Forward(someip_posix_common::someip::InstanceId instance_id, std::shared_ptr<PacketSink> from,
                       Packet packet) = 0;
  /**
   * \brief Sends the given event-mapped field cached value to the passed sink.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   * \param event_id SOME/IP event id.
   * \param sink A packet sink.
   */
  virtual void SendInitialEvent(const someip_posix_common::someip::ServiceId service_id,
                                const someip_posix_common::someip::InstanceId instance_id,
                                const someip_posix_common::someip::EventId event_id, PacketSink& sink) = 0;
  /**
   * \brief Sends all event-mapped field cached values of the given eventgroup to the passed TCP and UDP sinks.
   *
   * \param service_id SOME/IP service id.
   * \param instance_id SOME/IP instance id.
   * \param eventgroup_id SOME/IP eventgroup id.
   * \param tcp_packet_sink TCP packet sink.
   * \param udp_packet_sink UDP packet sink.
   */
  virtual void SendInitialEvents(const someip_posix_common::someip::ServiceId service_id,
                                 const someip_posix_common::someip::InstanceId instance_id,
                                 const someip_posix_common::someip::EventgroupId eventgroup_id,
                                 PacketSink* tcp_packet_sink, PacketSink* udp_packet_sink) = 0;
  /**
   * \brief Cleans up request routing table entries for the passed packet sink.
   *
   * \param sink A packet sink.
   */
  virtual void CleanUpRequestRoutingTableEntries(const PacketSink* sink) = 0;
  /**
   * \brief Cleans up response routing table entries for the passed packet sink.
   *
   * \param sink A packet sink.
   */
  virtual void CleanUpResponseRoutingTableEntries(const PacketSink* sink) = 0;
  /**
   * \brief Cleans up event routing table entries for the passed packet sink.
   *
   * \param sink A packet sink.
   */
  virtual void CleanUpEventRoutingTableEntries(const PacketSink* sink) = 0;
  /**
   * \brief Cleans up eventgroup routing table entries for the passed packet sink.
   *
   * \param sink A packet sink.
   */
  virtual void CleanUpEventgroupRoutingTableEntries(const PacketSink* sink) = 0;
};
}  // namespace packet_router
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_PACKET_ROUTER_PACKET_ROUTER_INTERFACE_H_

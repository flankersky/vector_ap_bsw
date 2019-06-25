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
/**        \file  someip-posix/someip_posix.h
 *        \brief  SomeIP POSIX
 *
 *      \details  This file provides an API for communication with the SomeIP daemon.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSOMEIP_POSIX_INCLUDE_SOMEIP_POSIX_SOMEIP_POSIX_H_
#define LIB_LIBSOMEIP_POSIX_INCLUDE_SOMEIP_POSIX_SOMEIP_POSIX_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include "ara/log/logging.hpp"
#include "osabstraction/io/network/socket/socket_connector.h"
#include "osabstraction/io/network/socket/unix_domain_datagram_socket.h"
#include "osabstraction/io/network/socket/unix_domain_stream_socket.h"
#include "osabstraction/io/reactor.h"
#include "someip-posix-common/someip/config_model.h"
#include "someip-posix-common/someip/someip_posix_types.h"
#include "someip-posix-common/someipd_posix/routing/message_reader.h"
#include "someip-posix/someip_posix_application_interface.h"
#include "vac/language/cpp14_backport.h"

namespace someip_posix {
class SomeIpPosix;

/**
 * \brief Interface for shutting down handling of SomeIpPosix
 */
class SomeIpPosixShutdownHandlerInterface {
 public:
  /**
   * \brief Destructor.
   */
  virtual ~SomeIpPosixShutdownHandlerInterface() = default;

  /**
   * \brief Called when someip-posix is shutting down.
   *
   * \param instance The SomeIpPosix instance that is being shutdown.
   */
  virtual void OnSomeIpPosixShutdown(SomeIpPosix& instance) = 0;
};

/**
 * \brief SomeIpPosix.
 *
 * This class represents the interface to the SOME/IP POSIX daemon.
 */
class SomeIpPosix : public osabstraction::io::EventHandler {
 public:
  /**
   * \brief Shutdown handler type.
   */
  using ShutdownHandler = SomeIpPosixShutdownHandlerInterface*;
  /**
   * \brief Constructor of SomeIpPosix.
   *
   * \param config A configuration.
   * \param reactor A reactor used for asynchronous event notification.
   */
  SomeIpPosix(const someip_posix_common::config::SomeIpPosixConfigModel& config,
              osabstraction::io::ReactorInterface* reactor);
  /**
   * \brief Destructor of SomeIpPosix.
   */
  ~SomeIpPosix();
  /**
   * \brief SomeIpPosix is not copy-constructable.
   */
  SomeIpPosix(const SomeIpPosix& other) = delete;
  /**
   * \brief SomeIpPosix is not copyable.
   */
  SomeIpPosix& operator=(const SomeIpPosix& other) = delete;

  /**
   * \brief Allocates an unused SOME/IP client id.
   *
   * \return unused client id.
   */
  someip_posix_common::someip::ClientId GetClientId();

  /**
   * \brief Releases a previously allocated SOME/IP client id.
   *
   * \param client_id A client id which was allocated previously.
   */
  void ReleaseClientId(someip_posix_common::someip::ClientId client_id);

  /**
   * \brief Offers a service instance to the SomeIP daemon.
   *
   * After calling this function, an application must be ready to process requests destined to this service instance.
   *
   * \param service_id SOME/IP service id of the offered service.
   * \param instance_id SOME/IP instance id of the offered service.
   */
  void OfferService(someip_posix_common::someip::ServiceId service_id,
                    someip_posix_common::someip::InstanceId instance_id);

  /**
   * \brief Stops offering a service instance to the SomeIP daemon.
   *
   * After calling this function, the SOME/IP daemon stops forwarding requests destined to this service instance.
   *
   * \param service_id SOME/IP service id of the offered service.
   * \param instance_id SOME/IP instance id of the offered service.
   */
  void StopOfferService(someip_posix_common::someip::ServiceId service_id,
                        someip_posix_common::someip::InstanceId instance_id);

  /**
    * \brief Requests a service instance from the SOME/IP daemon.
    *
    * After calling this function, an application must be ready to process requests destined to this service instance.
    *
    * \param service_id SOME/IP service id of the Requested service.
    * \param instance_id SOME/IP instance id of the Requested service.
    */
  void RequestService(someip_posix_common::someip::ServiceId service_id,
                      someip_posix_common::someip::InstanceId instance_id);

  /**
    * \brief Releases a service instance Requested from the SOME/IP daemon.
    *
    * After calling this function, an application must be ready to process requests destined to this service instance.
    *
    * \param service_id SOME/IP service id of the Released service.
    * \param instance_id SOME/IP instance id of the Released service.
    */
  void ReleaseService(someip_posix_common::someip::ServiceId service_id,
                      someip_posix_common::someip::InstanceId instance_id);

  /**
   * \brief Lets the SOME/IP daemon know that an application wishes to receive an event of a service instance.
   *
   * After calling this function, an application must be ready to process the event sent by this service instance.
   *
   * \param service_id SOME/IP service id of the service.
   * \param instance_id SOME/IP instance id of the service.
   * \param event_id SOME/IP event id of the service.
   */
  void SubscribeEvent(someip_posix_common::someip::ServiceId service_id,
                      someip_posix_common::someip::InstanceId instance_id,
                      someip_posix_common::someip::EventId event_id);

  /**
   * \brief Lets the SOME/IP daemon know that an application does not wish to receive an event of a service instance.
   *
   * After calling this function, the SOME/IP daemon stops forwarding events with the specified id
   * of the specified service instance.
   *
   * \param service_id SOME/IP service id of the service.
   * \param instance_id SOME/IP instance id of the service.
   * \param event_id SOME/IP event id of the service.
   */
  void UnsubscribeEvent(someip_posix_common::someip::ServiceId service_id,
                        someip_posix_common::someip::InstanceId instance_id,
                        someip_posix_common::someip::EventId event_id);

  /**
   * \brief Sends a SOME/IP message to the SomeIP daemon which will forward it.
   *
   * \param instance_id Instance id of the service to which the passed packet is destined or from which it originates.
   * \param packet SOME/IP message.
   */
  void Send(someip_posix_common::someip::InstanceId instance_id, someip_posix_common::someip::SomeIpPacket packet);

  /**
   * \brief Returns service instances offered at the moment.
   *
   * \param service_id SOME/IP service id of the service.
   * \param instance_id SOME/IP instance id of the service.
   * \return Container of service instances offered at the time of the call.
   */
  someip_posix_common::someip::ServiceInstanceContainer FindService(
      someip_posix_common::someip::ServiceId service_id, someip_posix_common::someip::InstanceId instance_id);

  /**
   * \brief Starts asynchronous notification about offered and stopped services.
   *
   * \param service_id SOME/IP service id of the service.
   * \param instance_id SOME/IP instance id of the service.
   */
  void StartFindService(someip_posix_common::someip::ServiceId service_id,
                        someip_posix_common::someip::InstanceId instance_id);

  /**
   * \brief Stops asynchronous notification for the client of offered and stopped services.
   *
   * \param service_id SOME/IP service id of the service.
   * \param instance_id SOME/IP instance id of the service.
   */
  void StopFindService(someip_posix_common::someip::ServiceId service_id,
                       someip_posix_common::someip::InstanceId instance_id);

  /**
   * \brief Sets a receive handler for incoming SOME/IP messages and asynchronous notifications of offered services.
   *
   * \param someip_posix_application A pointer to a handler object to which received SOME/IP messages and offered
   * service instances will be passed.
   */
  void SetSomeipPosixApplication(SomeipPosixApplicationInterface* someip_posix_application);

  /**
   * \brief Registers a shutdown handler.
   *
   * \param handler A shutdown handler.
   */
  void SetShutdownHandler(ShutdownHandler handler) { shutdown_handler_ = handler; }

  /**
   * \brief Resets the receive handler for incoming SOME/IP messages.
   */
  void UnsetSomeipPosixApplication();

  /**
   * \brief Checks if SomeIp Daemon is running.
   *
   * \return true if daemon is running and false otherwise.
   */
  bool IsRunning() const;

  /**
   * \brief Handle the next pending packet in polling mode.
   */
  bool ProcessNextPacket();

 private:
  /**
   * \brief A file descriptor event handler.
   *
   * \param handle A file descriptor.
   * \return Always returns true.
   */
  bool HandleRead(int handle) override;

  /**
   * \brief Processes routing messages received from the SomeIP daemon.
   *
   * \param message A routing message sent by the SomeIP daemon.
   */
  void ProcessRoutingMessage(someip_posix_common::someipd_posix::routing::MessageReader::Message&& message);

  /**
   * \brief Handler Function for the SocketConnector connecting the RoutingSocket
   * \param stream_socket The connected socket
   */
  void RoutingSocketConnectorHandler(osabstraction::io::network::socket::UnixDomainStreamSocket&& stream_socket);

  /**
   * \brief Reads a routing channel id sent by the SomeIP daemon.
   *
   * A routing channel is assigned by the SomeIP daemon to each routing communication channel between an application
   * and the SomeIP daemon. The routing channel id must be passed in some of the control messages sent
   * to the SomeIP daemon.
   *
   * \return The routing channel id of this SomeIpPosix instance.
   */
  void ReadRoutingChannelId();

  /**
   * \brief Maximum number of entries in a FindService response control message.
   */
  static constexpr int kMaxFindServiceResponseEntries{100};

  /**
   * \brief A reactor used for asynchronous event notification on file descriptors.
   */
  osabstraction::io::ReactorInterface* const reactor_;

  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;

  /**
   * \brief Lock for the control connection.
   */
  std::mutex control_socket_lock_;

  /**
   * \brief Socket for the control connection.
   */
  osabstraction::io::network::socket::UnixDomainDatagramSocket control_socket_;

  /**
   * \brief Lock for the routing connection.
   */
  std::mutex routing_socket_lock_;

  /**
   * \brief Socket for the routing communication.
   */
  osabstraction::io::network::socket::UnixDomainStreamSocket routing_socket_;

  /**
   * \brief Routing channel id provided by the SomeIP daemon.
   */
  std::uint32_t routing_channel_id_;

  /**
   * \brief socket_id unique id to distinguish between multiple someipd instantiations within a single process
   */
  static std::atomic<std::uint32_t> socket_id_;

  /**
   * \brief A pointer to a receive handler for incoming SOME/IP messages and incoming Service Discovery messages.
   */
  SomeipPosixApplicationInterface* someip_posix_application_;

  /**
   * \brief Reader for routing messages.
   */
  someip_posix_common::someipd_posix::routing::MessageReader routing_message_reader_;

  /**
   * \brief true if SOME/IP Posix is running, false otherwise.
   * TODO(PAASR-2151) replace with state machine
   */
  bool someip_posix_running_{true};

  /**
   * \brief A shutdown handler.
   */
  ShutdownHandler shutdown_handler_{nullptr};

  /**
   * \brief Reference to the SOME/IP related configuration owned by another layer.
   * SomeIpPosix only has read access to the configuration (const).
   * The configuration may change during run-time and the SomeIpPosix layer is able to access
   * the current configuration at any time.
   */
  const someip_posix_common::config::SomeIpPosixConfigModel& someip_config_;
};

}  // namespace someip_posix

#endif  // LIB_LIBSOMEIP_POSIX_INCLUDE_SOMEIP_POSIX_SOMEIP_POSIX_H_

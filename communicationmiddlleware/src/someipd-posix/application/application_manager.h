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
/**        \file  application_manager.h
 *        \brief  Application manager
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_APPLICATION_APPLICATION_MANAGER_H_
#define SRC_SOMEIPD_POSIX_APPLICATION_APPLICATION_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <bitset>
#include <limits>
#include <map>
#include <memory>
#include "ara/log/logging.hpp"
#include "osabstraction/io/network/socket/socket.h"
#include "osabstraction/io/network/socket/socket_acceptor.h"
#include "osabstraction/io/network/socket/unix_domain_datagram_socket.h"
#include "osabstraction/io/network/socket/unix_domain_stream_server_socket.h"
#include "osabstraction/io/reactor_interface.h"
#include "someip-posix-common/someipd_posix/control/message.h"
#include "someipd-posix/application/application.h"
#include "someipd-posix/application/application_event_handler.h"
#include "someipd-posix/application/event_subscription_state_update_manager.h"
#include "someipd-posix/application/findservice_update_manager.h"
#include "someipd-posix/configuration/configuration.h"
#include "someipd-posix/packet_router/packet_router_interface.h"
#include "someipd-posix/service_discovery/service_discovery.h"

namespace someipd_posix {
namespace application {

/**
 * \brief ApplicationManager.
 */
class ApplicationManager : public osabstraction::io::EventHandler {
 public:
  /**
   * \brief Constructor of ApplicationManager.
   *
   * \param config A configuration.
   * \param reactor A reactor for asynchronous event notification on file descriptors.
   * \param packet_router A packet router.
   * \param service_discovery A service discovery.
   */
  ApplicationManager(const configuration::Configuration& config, osabstraction::io::ReactorInterface* reactor,
                     packet_router::PacketRouterInterface* packet_router,
                     service_discovery::ServiceDiscovery* service_discovery);
  /**
   * \brief Destructor of ApplicationManager.
   */
  virtual ~ApplicationManager();
  /**
   * \brief ApplicationManager is not copy-constructable.
   */
  ApplicationManager(const ApplicationManager& other) = delete;
  /**
   * \brief ApplicationManager is not copy-assignable.
   */
  ApplicationManager& operator=(const ApplicationManager& other) = delete;

  /**
   * \brief Type for the collection of registered applications of the manager.
   */
  using ApplicationMap = std::map<someip_posix_common::someip::ChannelId, std::shared_ptr<Application>>;

  /**
   * \brief Type forwarding for a smart ptr to an application instance.
   */
  using ApplicationPtr = std::shared_ptr<Application>;

  /**
   * \brief Return the application that holds a given handle
   *
   * \param handle A socket file descriptor associated with an application.
   * \return A pointer to an Application or an empty pointer, if no application was found for the given handle.
   */
  ApplicationPtr GetApplication(const int handle) const;

 private:
  /**
   * \brief Maximum number of simultaneous clients.
   */
  static constexpr std::uint16_t kMaxClientIds = std::numeric_limits<std::uint16_t>::max();
  /**
   * \brief Maximum length of a control message.
   */
  static constexpr std::uint16_t kMaxControlPacketSize = std::numeric_limits<std::uint16_t>::max();

  /**
   * \brief Called when an application is shutting down.
   *
   * \param application An application that is shutting down.
   */
  void OnApplicationShutdown(Application* application);
  /**
   * \brief Creates a new instance of application and activates it.
   *
   * \param socket A socket representing a connection to an application.
   */
  void AcceptApplication(osabstraction::io::network::socket::UnixDomainStreamSocket&& socket);
  /**
   * \brief Handler for asynchronous event notifications on the socket for control messages.
   *
   * \param handle A file descriptor representing the socket for control messages.
   * \return Always returns true.
   */
  bool HandleRead(someip_posix_common::someip::ChannelId handle) override;
  /**
   * \brief Dispatches an arrived control message to the corresponding control message handler.
   *
   * \param header A pointer to a received control message.
   * \param remote_address The address of the remote peer. Required for sending a response.
   */
  void ProcessControlPacket(const someip_posix_common::someipd_posix::control::MessageHeader* header,
                            const osabstraction::io::network::address::SocketAddress& remote_address);
  /**
   * \brief GetClientId control message handler.
   *
   * \param application The application which sent the request.
   * \param header A pointer to a received control message.
   * \param remote_address The address of the remote peer. Required for sending a response.
   */
  void GetClientIdHandler(ApplicationPtr application,
                          const someip_posix_common::someipd_posix::control::MessageHeader* header,
                          const osabstraction::io::network::address::SocketAddress& remote_address);
  /**
   * \brief ReleaseClientId control message handler.
   *
   * \param application The application which sent the request.
   * \param header A pointer to a received control message.
   * \param remote_address The address of the remote peer. Required for sending a response.
   */
  void ReleaseClientIdHandler(ApplicationPtr application,
                              const someip_posix_common::someipd_posix::control::MessageHeader* header,
                              const osabstraction::io::network::address::SocketAddress& remote_address);
  /**
   * \brief OfferService control message handler.
   *
   * \param application The application which sent the request.
   * \param header A pointer to a received control message.
   * \param remote_address The address of the remote peer. Required for sending a response.
   */
  void OfferServiceHandler(ApplicationPtr application,
                           const someip_posix_common::someipd_posix::control::MessageHeader* header,
                           const osabstraction::io::network::address::SocketAddress& remote_address);
  /**
   * \brief StopOfferService control message handler.
   *
   * \param application The application which sent the request.
   * \param header A pointer to a received control message.
   * \param remote_address The address of the remote peer. Required for sending a response.
   */
  void StopOfferServiceHandler(ApplicationPtr application,
                               const someip_posix_common::someipd_posix::control::MessageHeader* header,
                               const osabstraction::io::network::address::SocketAddress& remote_address);
  /**
   * \brief FindService control message handler.
   *
   * \param application The application which sent the request.
   * \param header A pointer to a received control message.
   * \param remote_address The address of the remote peer. Required for sending a response.
   */
  void FindServiceHandler(ApplicationPtr application,
                          const someip_posix_common::someipd_posix::control::MessageHeader* header,
                          const osabstraction::io::network::address::SocketAddress& remote_address);
  /**
   * \brief StartFindService control message handler.
   *
   * \param application The application client (consumer) which sent the request to get a notification about
   * service changes.
   * \param header A pointer to the received control message `StartFindService()`.
   * \param remote_address The address of the remote peer. Required for sending a response. In the case
   * of asynchronous update, the remote address is not needed, because the service discovery update
   * is sent via the routing socket.
   */
  void StartFindServiceHandler(ApplicationPtr application,
                               const someip_posix_common::someipd_posix::control::MessageHeader* header,
                               const osabstraction::io::network::address::SocketAddress& remote_address);

  /**
   * \brief StopFindService control message handler.
   *
   * \param application The application which sent the request, stops the asynchronous notification.
   * \param header A pointer to the received control message `StopFindService()`.
   * \param remote_address The address of the remote peer. Required for sending a response. In the case
   * of asynchronous update, the remote address is not needed, because the service discovery update
   * is sent via the routing socket.
   */
  void StopFindServiceHandler(ApplicationPtr application,
                              const someip_posix_common::someipd_posix::control::MessageHeader* header,
                              const osabstraction::io::network::address::SocketAddress& remote_address);
  /**
   * \brief SubscribeEvent control message handler.
   *
   * \param application The application which sent the request.
   * \param header A pointer to a received control message.
   * \param remote_address The address of the remote peer. Required for sending a response.
   */
  void SubscribeEventHandler(ApplicationPtr application,
                             const someip_posix_common::someipd_posix::control::MessageHeader* header,
                             const osabstraction::io::network::address::SocketAddress& remote_address);
  /**
   * \brief UnsubscribeEvent control message handler.
   *
   * \param application The application which sent the request.
   * \param header A pointer to a received control message.
   * \param remote_address The address of the remote peer. Required for sending a response.
   */
  void UnsubscribeEventHandler(ApplicationPtr application,
                               const someip_posix_common::someipd_posix::control::MessageHeader* header,
                               const osabstraction::io::network::address::SocketAddress& remote_address);
  /**
   * \brief RequestService control message handler.
   *
   * \param application The application which sent the request.
   * \param header A pointer to a received control message.
   * \param remote_address The address of the remote peer. Required for sending a response.
   */
  void RequestService(ApplicationPtr application,
                      const someip_posix_common::someipd_posix::control::MessageHeader* header,
                      const osabstraction::io::network::address::SocketAddress& remote_address);
  /**
   * \brief ReleaseService control message handler.
   *
   * \param application The application which sent the request.
   * \param header A pointer to a received control message.
   * \param remote_address The address of the remote peer. Required for sending a response.
   */
  void ReleaseService(ApplicationPtr application,
                      const someip_posix_common::someipd_posix::control::MessageHeader* header,
                      const osabstraction::io::network::address::SocketAddress& remote_address);
  /**
   * \brief Allocate an unused client id.
   *
   * \return A free client id.
   */
  someip_posix_common::someip::ClientId GetFreeClientId();
  /**
   * \brief A configuration.
   */
  const configuration::Configuration& config_;
  /**
   * \brief A reactor for asynchronous event notification on file descriptors.
   */
  osabstraction::io::ReactorInterface* const reactor_;
  /**
   * \brief A packet router.
   */
  packet_router::PacketRouterInterface* const packet_router_;
  /**
   * \brief EventHandler that delegates back to applications owned by this ApplicationManager.
   */
  ApplicationEventHandler application_event_handler_;
  /**
   * \brief A service discovery.
   */
  service_discovery::ServiceDiscovery* const service_discovery_;

  /** \brief subLogger for globally we use subLogger to create context Id for the logging messages
   *
   */
  ara::log::Logger& subLogger;
  /**
   * \brief Socket acceptor for routing communication with applications.
   */
  osabstraction::io::network::socket::SocketAcceptor<osabstraction::io::network::socket::UnixDomainStreamServerSocket,
                                                     ApplicationManager, &ApplicationManager::AcceptApplication>
      routing_socket_acceptor_;
  /**
   * \brief Socket for control communication with applications.
   */
  osabstraction::io::network::socket::UnixDomainDatagramSocket control_socket_;
  /**
   * \brief Buffer for incoming control messages.
   */
  std::array<std::uint8_t, kMaxControlPacketSize> control_packet_buffer_;
  /**
   * \brief Container for each application connection.
   */
  ApplicationMap applications_;
  /**
   * \brief Client id bit mask.
   */
  std::bitset<kMaxClientIds> client_ids_;
  /**
   * \brief Find Service Update Manager.
   */
  FindServiceUpdateManager find_service_update_manager_;
  /**
   * \brief Event State Subscription State Update Monitor
   */
  EventSubscriptionStateUpdateManager event_state_subscription_update_manager_;
};

}  // namespace application
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_APPLICATION_APPLICATION_MANAGER_H_

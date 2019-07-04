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
/**        \file  service_discovery_tcp_server.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_TCP_SERVER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_TCP_SERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/network/socket/socket_acceptor.h"
#include "osabstraction/io/network/socket/tcp_server_socket.h"
#include "osabstraction/io/network/socket/tcp_socket.h"
#include "osabstraction/io/reactor.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

class ServiceDiscoveryTcpEndpoint;

/**
 * \brief ServiceDiscoveryTcpServer.
 */
class ServiceDiscoveryTcpServer {
 public:
  /**
   * \brief A TCP socket type.
   */
  using Socket = osabstraction::io::network::socket::TCPSocket;
  /**
   * \brief A TCP endpoint type.
   */
  using ServiceDiscoveryTcpEndpointPtr = ServiceDiscoveryTcpEndpoint*;
  /**
   * \brief Constructor of ServiceDiscoveryTcpServer.
   *
   * \param reactor A reactor.
   * \param endpoint A TCP endpoint this server belongs to.
   */
  ServiceDiscoveryTcpServer(osabstraction::io::ReactorInterface* reactor, ServiceDiscoveryTcpEndpointPtr endpoint);

  /**
   * \brief Delete copy constructor
   */
  ServiceDiscoveryTcpServer(const ServiceDiscoveryTcpServer&) = delete;

  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryTcpServer& operator=(const ServiceDiscoveryTcpServer&) = delete;
  /**
   * \brief Destructor of ServiceDiscoveryTcpServer.
   */
  virtual ~ServiceDiscoveryTcpServer();

 private:
  /**
   * \brief A TCP server socket type.
   */
  using ServerSocket = osabstraction::io::network::socket::TCPServerSocket;
  /**
   * \brief Called when a new TCP connection is accepted.
   *
   * \param socket A TCP socket representing the established connection.
   */
  void OnAccept(Socket&& socket);
  /**
   * \brief A TCP acceptor type.
   */
  using SocketAcceptor = osabstraction::io::network::socket::SocketAcceptor<ServerSocket, ServiceDiscoveryTcpServer,
                                                                            &ServiceDiscoveryTcpServer::OnAccept>;
  /**
   * \brief A reactor.
   */
  osabstraction::io::ReactorInterface* reactor_;
  /**
   * \brief A TCP endpoint this server belongs to.
   */
  ServiceDiscoveryTcpEndpointPtr endpoint_;
  /**
   * \brief A TCP socket acceptor for incoming connections.
   */
  SocketAcceptor acceptor_;
};

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_TCP_SERVER_H_

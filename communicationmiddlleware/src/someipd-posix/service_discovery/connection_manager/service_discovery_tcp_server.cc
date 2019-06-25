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
/**        \file  service_discovery_tcp_server.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "someipd-posix/service_discovery/connection_manager/service_discovery_tcp_endpoint.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_tcp_server.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

ServiceDiscoveryTcpServer::ServiceDiscoveryTcpServer(osabstraction::io::ReactorInterface* reactor,
                                                     ServiceDiscoveryTcpEndpointPtr endpoint)
    : reactor_(reactor),
      endpoint_(endpoint),
      acceptor_(osabstraction::io::network::address::IPSocketAddress::FromHostAndPort(
                    endpoint_->GetAddress(), std::to_string(endpoint_->GetPort())),
                *this) {
  reactor_->RegisterEventHandler(acceptor_.GetHandle(), &acceptor_, osabstraction::io::kReadEvent, nullptr);
}

ServiceDiscoveryTcpServer::~ServiceDiscoveryTcpServer() {
  reactor_->UnregisterEventHandler(acceptor_.GetHandle(), osabstraction::io::kReadEvent);
}

void ServiceDiscoveryTcpServer::OnAccept(Socket&& socket) { endpoint_->AcceptConnection(std::move(socket)); }

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix

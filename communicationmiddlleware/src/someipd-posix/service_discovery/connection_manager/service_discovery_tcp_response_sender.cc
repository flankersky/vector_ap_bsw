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
/**        \file  service_discovery_tcp_response_sender.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "someipd-posix/service_discovery/connection_manager/service_discovery_tcp_connection.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_tcp_endpoint.h"
#include "someipd-posix/service_discovery/connection_manager/service_discovery_tcp_response_sender.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

ServiceDiscoveryTcpResponseSender::ServiceDiscoveryTcpResponseSender(ServiceDiscoveryTcpEndpointPtr endpoint,
                                                                     ServiceDiscoveryTcpConnectionPtr connection)
    : endpoint_(endpoint), connection_(connection) {
  endpoint_->AcquireConnection(connection_);
}

ServiceDiscoveryTcpResponseSender::~ServiceDiscoveryTcpResponseSender() { endpoint_->ReleaseConnection(connection_); }

void ServiceDiscoveryTcpResponseSender::Forward(someip_posix_common::someip::InstanceId instance_id,
                                                packet_router::Packet packet) {
  connection_->Forward(instance_id, std::move(packet));
}

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix

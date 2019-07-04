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
/**        \file  service_discovery_tcp_connection_state_change_handler.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_TCP_CONNECTION_STATE_CHANGE_HANDLER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_TCP_CONNECTION_STATE_CHANGE_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {

class ServiceDiscoveryTcpSender;

/**
 * \brief ServiceDiscoveryTcpConnectionStateChangeHandler.
 */
class ServiceDiscoveryTcpConnectionStateChangeHandler {
 public:
  /**
   * \brief A TCP sender pointer type.
   */
  using ServiceDiscoveryTcpSenderPtr = ServiceDiscoveryTcpSender*;
  /**
   * \brief Destructor of ServiceDiscoveryTcpConnectionStateChangeHandler.
   */
  virtual ~ServiceDiscoveryTcpConnectionStateChangeHandler() {}
  /**
   * \brief Called on connect.
   *
   * \param sender A TCP sender.
   */
  virtual void OnConnect(ServiceDiscoveryTcpSenderPtr sender) = 0;
  /**
   * \brief Called on disconnect.
   *
   * \param sender A TCP sender.
   */
  virtual void OnDisconnect(ServiceDiscoveryTcpSenderPtr sender) = 0;
};

}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_SERVICE_DISCOVERY_TCP_CONNECTION_STATE_CHANGE_HANDLER_H_

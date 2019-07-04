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
/**        \file  ara/com/configuration/configuration.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/configuration/configuration.h"

namespace ara {
namespace com {
namespace configuration {

const Configuration::SocketPath& Configuration::GetControlSocketPath() const noexcept { return control_socket_path_; }

const Configuration::SocketPath& Configuration::GetRoutingSocketPath() const noexcept { return routing_socket_path_; }

RuntimeProcessingMode Configuration::GetProcessingMode() const noexcept { return processing_mode_; }

const Configuration::ThreadPoolConfigContainer& Configuration::GetThreadPools() const noexcept {
  return thread_pool_configs_;
}

Configuration& Configuration::operator<<(const ThreadPoolConfig& thread_pool_config) {
  // Store the thread pool config
  thread_pool_configs_.emplace_back(thread_pool_config);

  return *this;
}

}  // namespace configuration
}  // namespace com
}  // namespace ara

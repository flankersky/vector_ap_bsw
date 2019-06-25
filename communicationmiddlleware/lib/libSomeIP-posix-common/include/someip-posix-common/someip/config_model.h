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
/**        \file  config_model.h
 *        \brief  Relevant configuration parameters for SOME/IP binding-related layers
 *        of the middleware.
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_CONFIG_MODEL_H_
#define LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_CONFIG_MODEL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip-posix-common/someip/someip_posix_types.h"

namespace someip_posix_common {
namespace config {

/**
 * \brief Handling of incoming packets for the SOME/IP binding may differ from a general processing mode on ara::com
 * level.
 */
enum class SomeIpBindingProcessingMode { kPolling, kSingleThreaded };

/**
 * \brief Default control socket path.
 */
static constexpr const char* kDefaultControlSocketPath{"/tmp/someipd-posix-control.socket"};

/**
 * \brief Default routing socket path.
 */
static constexpr const char* kDefaultRoutingSocketPath{"/tmp/someipd-posix-routing.socket"};

/**
 * \brief Default processing mode: kSingleThreaded: One reactor thread does a blocking read on the routing socket.
 */
static constexpr SomeIpBindingProcessingMode kDefaultProcessingMode{SomeIpBindingProcessingMode::kSingleThreaded};

/**
 * \brief Specific configuration for the usage of SOME/IP as one transport binding.
 */
class SomeIpPosixConfigModel {
 public:
  /**
   * \brief Control socket path for the SOME/IP binding.
   */
  ::someip_posix_common::someip::SocketPath control_socket_path_{kDefaultControlSocketPath};

  /**
   * \brief Routing socket path for the SOME/IP binding.
   */
  ::someip_posix_common::someip::SocketPath routing_socket_path_{kDefaultRoutingSocketPath};

  /**
   * \brief In dependence on the processing mode, the SomeIpPosix and its sockets must be configured appropriately.
   */
  SomeIpBindingProcessingMode processing_mode_{kDefaultProcessingMode};
};

}  // namespace config
}  // namespace someip_posix_common

#endif  // LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_CONFIG_MODEL_H_

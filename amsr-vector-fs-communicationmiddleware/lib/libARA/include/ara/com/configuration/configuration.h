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
/**        \file  ara/com/configuration/configuration.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_CONFIGURATION_CONFIGURATION_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_CONFIGURATION_CONFIGURATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include <vector>
#include "ara/com/configuration/thread_pool_config.h"
#include "ara/log/logging.hpp"

namespace ara {
namespace com {
namespace configuration {

/**
 * \brief Each Runtime may have its specific processing mode.
 * - kPolling: In processing mode kPolling no reactor thread is handling events within the bindings. The application
 * developer must poll for incoming data by calling ProcessNextPacket() which does *one step* for each binding.
 * - kSingleThreaded: In single threaded mode, the reactor is provided by the middleware and does a blocking read.
 * Incoming requests will be routed to the application immediately.
 */
enum class RuntimeProcessingMode : std::uint32_t { kPolling = 0U, kSingleThreaded = 1U };

/**
 * \brief ara::com Configuration
 */
class Configuration {
 public:
  /**
   * \brief Type-alias for the thread-pool configuration objects
   */
  using ThreadPoolConfigContainer = std::vector<ThreadPoolConfig>;

  /**
   * \brief Default control socket path
   */
  static constexpr const char* kDefaultControlSocketPath{"/tmp/someipd-posix-control.socket"};

  /**
   * \brief Default routing socket path
   */
  static constexpr const char* kDefaultRoutingSocketPath{"/tmp/someipd-posix-routing.socket"};

  /**
   * \brief Type-alias for the socket path
   */
  using SocketPath = std::string;

  /**
   * \brief Destructor
   */
  virtual ~Configuration() = default;

  /**
   * \brief Returns the path of the control socket.
   * \return Path to control socket.
   */
  const SocketPath& GetControlSocketPath() const noexcept;
  /**
   * \brief Returns the path of the routing socket.
   * \return Path to routing socket.
   */
  const SocketPath& GetRoutingSocketPath() const noexcept;

  /**
   * \brief Return the processing mode
   * \return The runtime processing mode
   */
  RuntimeProcessingMode GetProcessingMode() const noexcept;

  /**
   * \brief Get configured thread pools.
   * \return Container of thread pool configurations
   */
  const ThreadPoolConfigContainer& GetThreadPools() const noexcept;

 protected:
  /**
   * \brief Add a thread-pool configuration to the global configuration. This only adds thread-pools that are not
   * already part of the global configuration.
   * \param thread_pool_config A single thread-pool configuration
   * \return The global configuration.
   */
  Configuration& operator<<(const ThreadPoolConfig& thread_pool_config);

  // TODO(PAASR-3135) Move binding specific configuration parameters to binding libraries
  /**
   * \brief Control socket path
   */
  SocketPath control_socket_path_;

  /**
   * \brief Routing socket path
   */
  SocketPath routing_socket_path_;

  /**
   * \brief Processing mode of runtime
   */
  RuntimeProcessingMode processing_mode_;

  /**
   * \brief All thread-pools are hold in the Runtime object.
   */
  ThreadPoolConfigContainer thread_pool_configs_;
};

}  // namespace configuration
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_CONFIGURATION_CONFIGURATION_H_

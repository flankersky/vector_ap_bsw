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
/**        \file  ara/com/configuration/json_configuration.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/configuration/json_configuration.h"
#include "ara/per/internal/json_parser.h"

namespace ara {
namespace com {
namespace configuration {

JsonConfiguration::JsonConfiguration(const JsonFilePath& application_config_json_path)
    : Configuration(), application_config_json_path_(application_config_json_path) {
  ReadJsonConfiguration();
}

void JsonConfiguration::ReadJsonConfiguration() {
  ara::log::Logger& logger = ara::log::CreateLogger("JsonConfiguration", "");
  logger.LogDebug() << __func__ << ":" << __LINE__ << " Reading ara::com JSON configuration "
                    << application_config_json_path_;

  // SOME/IP daemon configurations
  ara::per::internal::json::JsonDocument application_json_document =
      ara::per::internal::json::LoadFile(application_config_json_path_);
  const auto& application_json = application_json_document.GetObject();

  // Parse the ara::com socket paths
  ReadSocketPaths(application_json);

  // Parse the runtime processing mode
  ReadProcessingMode(application_json);

  // Parse thread pools
  if (application_json.HasMember("thread_pools")) {
    const auto& thread_pools_configuration = application_json["thread_pools"];
    if (!thread_pools_configuration.IsArray()) {
      logger.LogError() << "Thread pool configuration structure not in expected format! Must be \"thread_pools\": []";
      throw std::runtime_error("Incorrect configuration structure for section 'thread_pools'");
    }
    if (thread_pools_configuration.Size() == 0U) {
      logger.LogInfo() << "No user specific thread pools configured. Falling back to default thread pool.";
    }

    // Reads in all application-specific configuration files and appends the global configuration
    for (const auto& thread_pool_configuration : thread_pools_configuration.GetArray()) {
      *this << ReadThreadPool(thread_pool_configuration);
    }
  }
}

void JsonConfiguration::ReadSocketPaths(const ara::per::internal::json::JsonObject& application_json) {
  ara::log::Logger& logger = ara::log::CreateLogger("JsonConfiguration", "");
  logger.LogDebug() << __func__ << ":" << __LINE__ << " Reading ara::com socket paths configuration";

  // Read optional control socket path
  if (application_json.HasMember("control_socket_path")) {
    if (application_json["control_socket_path"].IsString()) {
      control_socket_path_ = application_json["control_socket_path"].GetString();
    } else {
      logger.LogError()
          << "Control socket path not in expected format! Must be \"control_socket_path\": \"<PathToControlSocket>\"";
      throw std::runtime_error("Incorrect configuration structure for 'control_socket_path'");
    }
  } else {
    logger.LogInfo() << "Control socket path file descriptor not configured. Falling back to default path "
                     << std::string(kDefaultControlSocketPath);
    control_socket_path_ = std::string(kDefaultControlSocketPath);
  }

  // Read optional routing socket path
  if (application_json.HasMember("routing_socket_path")) {
    if (application_json["routing_socket_path"].IsString()) {
      routing_socket_path_ = application_json["routing_socket_path"].GetString();
    } else {
      logger.LogError()
          << "Routing socket path not in expected format! Must be \"routing_socket_path\": \"<PathToRoutingSocket>\"";
      throw std::runtime_error("Incorrect configuration structure for 'routing_socket_path'");
    }

  } else {
    logger.LogInfo() << "Routing socket path file descriptor not configured. Falling back to default path "
                     << std::string(kDefaultRoutingSocketPath);
    routing_socket_path_ = std::string(kDefaultRoutingSocketPath);
  }
}

void JsonConfiguration::ReadProcessingMode(const ara::per::internal::json::JsonObject& application_json) {
  ara::log::Logger& logger = ara::log::CreateLogger("JsonConfiguration", "");
  logger.LogDebug() << __func__ << ":" << __LINE__ << " Reading ara::com runtime processing mode";

  // Parse optional attribute 'processing_mode'
  if (application_json.HasMember("processing_mode")) {
    // Check format
    if (!application_json["processing_mode"].IsUint()) {
      logger.LogError()
          << "Processing mode configuration structure not in expected format! Must be \"processing_mode\": 0";
      throw std::runtime_error("Incorrect configuration structure for processing mode 'processing_mode'");
    }

    // Type alias for underlying processing mode enum
    using RuntimeProcessingModeEnumType = typename std::underlying_type<RuntimeProcessingMode>::type;

    // Read processing mode and convert to enum
    const auto processing_mode_num = application_json["processing_mode"].GetUint();
    static_assert(std::numeric_limits<decltype(processing_mode_num)>::max() <=
                      std::numeric_limits<RuntimeProcessingModeEnumType>::max(),
                  "Deduced type for the processing_mode_num does not match underlying type range of enum class "
                  "RuntimeProcessingMode.");

    switch (processing_mode_num) {
      case static_cast<RuntimeProcessingModeEnumType>(RuntimeProcessingMode::kPolling): {
        logger.LogDebug() << "Setting ara::com processing mode to 'Polling'.";
        processing_mode_ = RuntimeProcessingMode::kPolling;
        break;
      }
      // Fall-through
      case static_cast<RuntimeProcessingModeEnumType>(RuntimeProcessingMode::kSingleThreaded): {
        logger.LogDebug() << "Setting ara::com processing mode to 'SingleThreaded'.";
        processing_mode_ = RuntimeProcessingMode::kSingleThreaded;
        break;
      }
      default: {
        logger.LogError() << "Processing mode configuration has unexpected / unsupported processing mode value: "
                          << static_cast<std::uint16_t>(processing_mode_num);
        throw std::runtime_error("Unexpected / unsupported processing mode value in 'processing_mode'");
        break;
      }
    }
  } else {
    logger.LogInfo() << "No processing_mode configured. Falling back to default processing mode 'SingleThreaded'.";
    processing_mode_ = RuntimeProcessingMode::kSingleThreaded;
  }
}

}  // namespace configuration
}  // namespace com
}  // namespace ara

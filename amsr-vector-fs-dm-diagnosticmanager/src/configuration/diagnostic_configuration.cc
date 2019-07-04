/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  diagnostic_configuration.cc
 *        \brief  diagnostic_configuration.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <stdexcept>
#include <string>
#include <utility>

#include "ara/log/logging.hpp"
#include "configuration/diagnostic_configuration.h"

namespace amsr {
namespace diag {
namespace configuration {

DiagnosticConfiguration::DiagnosticConfiguration() {}

void DiagnosticConfiguration::LoadConfigurationFile(const std::string& path_to_configuration) {
  // TODO(PAASR-2204): Rework DM configuration.

  // Opening JSON file and get corresponding object.
  const ara::per::internal::json::JsonDocument jsonDoc = ara::per::internal::json::LoadFile(path_to_configuration);
  ara::per::internal::json::JsonValue::ConstObject jsonObj = jsonDoc.GetObject();

  // Get relativ path folder where configuration is located.
  const std::string::size_type last_separation = path_to_configuration.find_last_of("/");
  std::string configDirectory;
  if (last_separation == std::string::npos) {
    configDirectory = "";
  } else {
    configDirectory = path_to_configuration.substr(0, last_separation + 1);
  }
  ara::log::LogDebug() << "DiagnosticConfiguration::" << __func__ << ": configuration directory: " << configDirectory;

  // Parsing the configuration files for each Diagnostic managers.
  if (jsonObj.HasMember("DextFiles")) {
    ara::per::internal::json::JsonValue::ConstArray dextFiles = jsonObj["DextFiles"].GetArray();
    // Reserve enough memory in vector.
    dext_configurations.reserve(dextFiles.Size());
    // Open and parse every dext files.
    for (ara::per::internal::json::JsonValue::ConstArray::ValueType& dextFile : dextFiles) {
      std::string dextPath = dextFile.GetString();
      DextConfiguration dextConfig;
      const ara::per::internal::json::JsonDocument jsonDocDext =
          ara::per::internal::json::LoadFile(configDirectory + dextPath);
      dextConfig.ParseDext(jsonDocDext.GetObject());
      dext_configurations.emplace_back(std::move(dextConfig));
    }
  } else {
    throw std::runtime_error("MetaConfig : Links to Dext files not found.");
  }

  // Parsing the configuration file for the generic parameters.
  if (jsonObj.HasMember("GenericParameters")) {
    std::string generic_parameters_path = jsonObj["GenericParameters"].GetString();
    ara::per::internal::json::JsonDocument generic_parameters_doc =
        ara::per::internal::json::LoadFile(configDirectory + generic_parameters_path);
    generic_configuration_.ParseGenericParameters(generic_parameters_doc.GetObject());
  } else {
    throw std::runtime_error("MetaConfig : Link to genericParameters file not found.");
  }
}

void DiagnosticConfiguration::ParseDextFile(ara::per::internal::json::JsonObject config_obj) {}

}  // namespace configuration
}  // namespace diag
}  // namespace amsr

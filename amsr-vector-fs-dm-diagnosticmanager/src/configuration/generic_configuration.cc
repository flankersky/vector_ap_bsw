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
/**        \file  generic_configuration.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "generic_configuration.h"

#include <stdexcept>

#include "ara/log/logging.hpp"

namespace amsr {
namespace diag {
namespace configuration {
void GenericConfiguration::ParseGenericParameters(ara::per::internal::json::JsonObject do_ip_obj) {
  // Parse LogOutput object.
  if (!do_ip_obj.HasMember("LogOutput")) {
    throw std::runtime_error("GenericConfiguration::ParseGenericParameters: Configuration for LogOutput not found.");
  }
  ara::log::LogDebug() << "Parsing LogOutput.";
  this->ParseLogOutput(do_ip_obj["LogOutput"].GetObject());
}

void GenericConfiguration::ParseLogOutput(ara::per::internal::json::JsonObject do_ip_obj) {
  // Get maximal number of payload bytes to be printed.
  if (!do_ip_obj.HasMember("MaxPayloadBytes")) {
    ara::log::LogWarn() << "GenericConfiguration::ParseLogOutput: Maximal number of payload bytes to be printed to the "
                           "output is missing: default value is used.";
    this->log_output_configuration.max_payload_bytes = kDefaultLogMaxNumberPayloadBytes;
  } else {
    this->log_output_configuration.max_payload_bytes = do_ip_obj["MaxPayloadBytes"].GetUint();
  }
}

}  // namespace configuration
}  // namespace diag
}  // namespace amsr

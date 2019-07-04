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
/**        \file  diagnostic_configuration.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_CONFIGURATION_DIAGNOSTIC_CONFIGURATION_H_
#define SRC_CONFIGURATION_DIAGNOSTIC_CONFIGURATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ara/per/internal/json_parser.h>
#include <vac/testing/test_adapter.h>
#include <string>
#include <vector>

#include "configuration/dext_configuration.h"
#include "configuration/generic_configuration.h"

namespace amsr {
namespace diag {
namespace configuration {
/**
 * \brief Implementation of Configuration Manager.
 */
class DiagnosticConfiguration {
 public:
  /**
   * \brief Constructor.
   */
  DiagnosticConfiguration();

  /**
   * \brief Destructor.
   */
  ~DiagnosticConfiguration() = default;

  /**
   * \brief This method is used to load the configuration file and then parse the JSON object.
   */
  void LoadConfigurationFile(const std::string& path_to_configuration);

 private:
  /**
   * \brief This method is used to parse the Dext files.
   */
  void ParseDextFile(ara::per::internal::json::JsonObject config_obj);

 public:
  /**
   * \brief array of DextConfigurations.
   */
  std::vector<DextConfiguration> dext_configurations;

  /**
   * \brief Generic configuration values.
   */
  GenericConfiguration generic_configuration_;

 private:
  // TODO(PAASR-2208): enable tests.
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DiagnosticConfigurationTest, DISABLED_ParseDoIPChannelOK);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DiagnosticConfigurationTest, DISABLED_ParseDoIPChannelKO);
};

}  // namespace configuration
}  // namespace diag
}  // namespace amsr

#endif  // SRC_CONFIGURATION_DIAGNOSTIC_CONFIGURATION_H_

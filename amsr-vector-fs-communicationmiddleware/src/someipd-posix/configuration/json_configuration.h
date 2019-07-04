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
/**        \file  someipd-posix/configuration/json_configuration.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_CONFIGURATION_JSON_CONFIGURATION_H_
#define SRC_SOMEIPD_POSIX_CONFIGURATION_JSON_CONFIGURATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include "ara/per/internal/json_parser.h"
#include "someipd-posix/configuration/configuration.h"

namespace someipd_posix {
namespace configuration {

/**
 * \brief Decouples the internal configuration data model from the JSON functionality
 * by deriving from Configuration base class.
 */
class JsonConfiguration : public Configuration {
 public:
  /**
   * \brief Constructor reads in the JSON configurations and transfers the information
   * contained into the internal data model stored in the attributes of the base object
   * of type `Configuration`.
   *
   * \param cfg_path A path to the configuration file someipd-posix.json.
   */
  explicit JsonConfiguration(const std::string& cfg_path) : Configuration(), cfg_path_{cfg_path} {
    ReadJsonConfigurations();
  }

 private:
  /**
   * \brief Parses JSON configuration files.
   */
  void ReadJsonConfigurations();
  /**
   * \brief Reads the SOME/IP daemon's configuration.
   *
   * \param config A JSON object representing someipd-posix.json.
   * \param config_gen A JSON object representing someipd-posix-gen.json.
   */
  void ReadDaemon(const ara::per::internal::json::JsonObject& config,
                  const ara::per::internal::json::JsonObject& config_gen);
  /**
   * \brief Reads configuration of static SD.
   *
   * \param config A JSON object representing someipd-posix.json.
   */
  StaticServiceDiscovery ReadStaticServiceDiscovery(const ara::per::internal::json::JsonObject& config) const;
  /**
   * \brief A path to the configuration file someipd-posix.json that holds
   * all socket descriptor settings and the paths to the application configurations.
   */
  std::string cfg_path_;
};

}  // namespace configuration
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_CONFIGURATION_JSON_CONFIGURATION_H_

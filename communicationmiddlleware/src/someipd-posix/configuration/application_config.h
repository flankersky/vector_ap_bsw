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
/**        \file  application_config.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_CONFIGURATION_APPLICATION_CONFIG_H_
#define SRC_SOMEIPD_POSIX_CONFIGURATION_APPLICATION_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/log/logging.hpp"
#include "ara/per/internal/json_parser.h"
#include "someipd-posix/configuration/configuration_types.h"

namespace someipd_posix {
namespace configuration {

/**
 * \brief One distinct configuration of one application that later can
 * be merged with other application configurations into one global configuration model.
 */
class ApplicationConfig : public ConfigurationTypesAndDefs {
 public:
  /**
   * \brief Constructor transfers the configuration into an internal model.
   * It extracts the necessary info from the config file.
   *
   * \param app The configuration for the application.
   * \param app_gen An object that holds all the information from the generated
   * file.
   */
  ApplicationConfig(const ara::per::internal::json::JsonDocument& app,
                    const ara::per::internal::json::JsonDocument& app_gen)
      : logger_(ara::log::CreateLogger("Configuration", "")), app_gen_{app_gen} {
    (void)app;
  }
  /**
   * \brief Reads configuration of services.
   *
   * \return A collection of services extracted from the configuration file this application
   * requests or provides.
   */
  ServiceContainer ReadServices() const;
  /**
   * \brief Reads configuration of required service instances.
   *
   * \return A collection of required service instances.
   */
  RequiredServiceInstanceContainer ReadRequiredServiceInstances() const;
  /**
   * \brief Reads configuration of provided service instances.
   */
  ProvidedServiceInstanceContainer ReadProvidedServiceInstances() const;
  /**
   * \brief Reads configuration of network endpoints.
   */
  NetworkEndpointContainer ReadNetworkEndpoints() const;

 private:
  /**
   * \brief An instance of a logger.
   */
  ara::log::Logger& logger_;
  /**
   * \brief Storing a reference to the application-gen.json
   */
  const ara::per::internal::json::JsonDocument& app_gen_;


  //rapidjson::Document& getDoc(const char* path);
};

}  // namespace configuration
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_CONFIGURATION_APPLICATION_CONFIG_H_

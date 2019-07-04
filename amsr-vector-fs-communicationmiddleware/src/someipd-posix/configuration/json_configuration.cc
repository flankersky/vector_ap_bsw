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
/**        \file  someipd-posix/configuration/json_configuration.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/configuration/json_configuration.h"
#include <algorithm>
#include "someipd-posix/configuration/application_config.h"

namespace someipd_posix {
namespace configuration {

void JsonConfiguration::ReadJsonConfigurations() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  printf("ReadJsonConfigurations into\n");
  // SOME/IP daemon configurations
  ara::per::internal::json::JsonDocument someipd = ara::per::internal::json::LoadFile(cfg_path_);
  printf("ReadJsonConfigurations into 1\n");
  const std::string::size_type ext_pos = cfg_path_.find_last_of(".");
  printf("ReadJsonConfigurations into 2\n");
  const std::string someipd_config_gen_path = cfg_path_.substr(0, ext_pos) + "-gen" + cfg_path_.substr(ext_pos);
  printf("ReadJsonConfigurations into 3\n");
  ara::per::internal::json::JsonDocument someipd_gen = ara::per::internal::json::LoadFile(someipd_config_gen_path);
  printf("ReadJsonConfigurations into 4\n");
  ReadDaemon(someipd.GetObject(), someipd_gen.GetObject());
  printf("ReadJsonConfigurations into 5\n");

  // Read in the application configurations and convert them from JSON into one internal data model.
  const auto& appl_config_obj = someipd.GetObject();
  printf("ReadJsonConfigurations into 6\n");
  const auto& app_configurations = appl_config_obj["applications"];
  printf("ReadJsonConfigurations into 7\n");

  if (!app_configurations.IsArray()) {
    printf("ReadJsonConfigurations into 8\n");
    logger_.LogError() << "Application configuration structure not in expected format! Must be \"applications\": []\n";
    throw std::runtime_error("No configuration section 'applications' found");
  }
  printf("ReadJsonConfigurations into 9\n");

  if (app_configurations.Size() == 0U) {
    logger_.LogInfo()
        << "No applications specified in someipd-posix.json. Assuming IPC between client and server via daemon.\n";
  }
  printf("ReadJsonConfigurations into 10\n");

  // Reads in all application-specific configuration files and appends the global configuration
  for (const auto& app_config : app_configurations.GetArray()) {
    printf("ReadJsonConfigurations into 11\n");
    const std::string appl_config_path{std::string(app_config.GetString()) + "/etc/application.json"};
    std::cout<<std::string(app_config.GetString()) + "/etc/application.json"<<std::endl;
    const std::string appl_gen_config_path{std::string(app_config.GetString()) + "/etc/application-gen.json"};
    std::cout<<std::string(app_config.GetString()) + "/etc/application-gen.json"<<std::endl;
    printf("ReadJsonConfigurations into 12\n");
    ara::per::internal::json::JsonDocument application = ara::per::internal::json::LoadFile(appl_config_path);
    printf("ReadJsonConfigurations into 13\n");
    ara::per::internal::json::JsonDocument application_gen = ara::per::internal::json::LoadFile(appl_gen_config_path);
    printf("ReadJsonConfigurations into 14\n");
    const ApplicationConfig app_model{application, application_gen};
    printf("ReadJsonConfigurations into 15\n");
    *this << app_model.ReadServices();
    printf("ReadJsonConfigurations into 16\n");
    *this << app_model.ReadRequiredServiceInstances();
    printf("ReadJsonConfigurations into 17\n");
    *this << app_model.ReadProvidedServiceInstances();
    printf("ReadJsonConfigurations into 18\n");
    *this << app_model.ReadNetworkEndpoints();
    printf("ReadJsonConfigurations into 19\n");
  }
  printf("ReadJsonConfigurations into 20\n");
}

void JsonConfiguration::ReadDaemon(const ara::per::internal::json::JsonObject& config,
                                   const ara::per::internal::json::JsonObject& config_gen) {
  (void)config_gen;
  logger_.LogDebug() << __func__ << ":" << __LINE__;

  if (config.HasMember("control_socket_path") && config["control_socket_path"].IsString()) {
    control_socket_path_ = config["control_socket_path"].GetString();
  } else {
    logger_.LogError() << "Control file descriptor not configured... Setting it automatically to: "
                          "/tmp/someipd-posix-control.socket";
    control_socket_path_ = "/tmp/someipd-posix-control.socket";
  }

  if (config.HasMember("routing_socket_path") && config["routing_socket_path"].IsString()) {
    routing_socket_path_ = config["routing_socket_path"].GetString();
  } else {
    logger_.LogError() << "Routing file descriptor not configured... Setting it automatically to: "
                          "/tmp/someipd-posix-routing.socket";
    routing_socket_path_ = "/tmp/someipd-posix-routing.socket";
  }

  *this << ReadStaticServiceDiscovery(config);
}

ConfigurationTypesAndDefs::StaticServiceDiscovery JsonConfiguration::ReadStaticServiceDiscovery(
    const ara::per::internal::json::JsonObject& config) const {
  logger_.LogDebug() << __func__ << ":" << __LINE__;

  StaticServiceDiscovery static_service_discovery{};

  if (!config.HasMember("static_service_discovery")) {
    return static_service_discovery;
  }

  assert(config["static_service_discovery"].IsObject());
  const auto& sdc = config["static_service_discovery"];
  assert(sdc.HasMember("enable") && sdc["enable"].IsBool());
  static_service_discovery.enable_ = sdc["enable"].GetBool();
  assert(sdc.HasMember("endpoints") && sdc["endpoints"].IsArray());
  for (const auto& rnec : sdc["endpoints"].GetArray()) {
    logger_.LogDebug() << __func__ << ":" << __LINE__;
    RemoteNetworkEndpoint remote_network_endpoint;
    assert(rnec.HasMember("address") && rnec["address"].IsString());
    remote_network_endpoint.address_ = rnec["address"].GetString();
    assert(rnec.HasMember("tcp_port") || rnec.HasMember("udp_port"));
    assert(!rnec.HasMember("tcp_port") || rnec["tcp_port"].IsUint());
    assert(!rnec.HasMember("udp_port") || rnec["udp_port"].IsUint());
    if (rnec.HasMember("tcp_port")) {
      remote_network_endpoint.tcp_port_ = static_cast<std::uint16_t>(rnec["tcp_port"].GetUint());
      assert(remote_network_endpoint.tcp_port_ != kInvalidPort);
    } else {
      remote_network_endpoint.tcp_port_ = kInvalidPort;
    }
    if (rnec.HasMember("udp_port")) {
      remote_network_endpoint.udp_port_ = static_cast<std::uint16_t>(rnec["udp_port"].GetUint());
      assert(remote_network_endpoint.udp_port_ != kInvalidPort);
    } else {
      remote_network_endpoint.udp_port_ = kInvalidPort;
    }
    /* Parse required service instances */
    logger_.LogDebug() << __func__ << ":" << __LINE__;
    assert(rnec.HasMember("required_service_instances") && rnec["required_service_instances"].IsArray());
    for (const auto& rsic : rnec["required_service_instances"].GetArray()) {
      ServiceInstance required_service_instance;
      assert(rsic.HasMember("service_id") && rsic["service_id"].IsUint());
      required_service_instance.service_id_ = static_cast<std::uint16_t>(rsic["service_id"].GetUint());
      assert(rsic.HasMember("instance_id") && rsic["instance_id"].IsUint());
      required_service_instance.instance_id_ = static_cast<std::uint16_t>(rsic["instance_id"].GetUint());
      assert(rsic.HasMember("major_version") && rsic["major_version"].IsUint());
      required_service_instance.major_version_ = static_cast<std::uint8_t>(rsic["major_version"].GetUint());
      assert(rsic.HasMember("minor_version") && rsic["minor_version"].IsUint());
      required_service_instance.minor_version_ = static_cast<std::uint32_t>(rsic["minor_version"].GetUint());
      remote_network_endpoint.required_service_instances_.push_back(required_service_instance);
    }
    static_service_discovery.endpoints_.push_back(remote_network_endpoint);
  }

  return static_service_discovery;
}

}  // namespace configuration
}  // namespace someipd_posix

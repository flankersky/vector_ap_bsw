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
/**        \file  config.cc
 *        \brief  Implementation of the configuration class
 *
 *      \details  Implementation to read in a json configuration file, to configure client and server
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ara/per/internal/json_parser.h>
#include <ara/log/logging.hpp>

#include "ara/rest/config.h"

namespace ara {
namespace rest {
namespace config {

namespace additional {

String DetermineAddressVersion(String address) {
  String result = address;
  bool ipv6 = false;
  for (auto &character : result) {
    if (character == ':') {
      ipv6 = true;
      break;
    }
  }
  // check if ipv6 address is enclosed by brackets
  if (ipv6 && result[0] != '[') {
    result = "[" + result + "]";
  }
  return result;
}

}  // namespace additional

Binding::Binding(String id, String type, TransportProtocol_t tp) : Id_(id), type_(type), transport_protocol_(tp) {}

ClientBinding::ClientBinding(String id, String type, TransportProtocol_t tp, Authorization_t auth, Proxy_t proxy,
                             bool has_proxy)
    : Binding(id, type, tp), authorization_(auth), proxy_(proxy), has_proxy_(has_proxy) {}

ServerBinding::ServerBinding(String id, String type, TransportProtocol_t tp, String address, int port)
    : Binding(id, type, tp), address_(address), port_(port) {}

Configuration::Configuration(String id, String type) : Id_(id), type_(type) {}

ClientConfiguration::ClientConfiguration(String id, String type, String binding_id)
    : Configuration(id, type), BindingId_(binding_id) {}

ServerConfiguration::ServerConfiguration(String id, String type, std::vector<String> binding_ids)
    : Configuration(id, type), BindingIds_(binding_ids) {}

String ConfigurationHandler::config_file = "etc/configuration.json";

ConfigurationHandler::ConfigurationHandler(String path)
    : log_(ara::log::CreateLogger("42", "ara::rest ConfigurationHandler")) {
  log_.LogDebug() << "ConfigurationHandler::ConfigurationHandler Load config file: " << path;
  ara::per::internal::json::JsonDocument config_json = ara::per::internal::json::LoadFile(path);

  for (auto &configuration_entry : config_json["Configurations"].GetArray()) {
    if (strcmp(configuration_entry["Type"].GetString(), "client") == 0) {
      log_.LogDebug() << "ConfigurationHandler::ConfigurationHandler Found client config.";
      String client_identifier = configuration_entry["Identifier"].GetString();
      String client_binding_id = configuration_entry["BindingId"].GetString();
      log_.LogDebug() << "ConfigurationHandler::ConfigurationHandler Found client_identifier: " << client_identifier;
      log_.LogDebug() << "ConfigurationHandler::ConfigurationHandler Found client_binding_id: " << client_binding_id;
      // iterate bindings and find the one with corresponding binding id
      for (auto &binding_entry : config_json["Bindings"].GetArray()) {
        if (strcmp(binding_entry["Identifier"].GetString(), client_binding_id.c_str()) == 0) {
          log_.LogDebug() << "ConfigurationHandler::ConfigurationHandler Found matching client_binding_id.";
          Authorization_t auth;
          auth.Adress_ = binding_entry["Authorization"]["Address"].GetString();
          auth.Port_ = binding_entry["Authorization"]["Port"].GetInt();

          Proxy_t proxy;
          bool has_proxy = true;
          if (binding_entry.HasMember("Proxy")) {
            log_.LogDebug() << "ConfigurationHandler::ConfigurationHandler Found client proxy config.";
            proxy.Adress_ = binding_entry["Proxy"]["Address"].GetString();
            proxy.Port_ = binding_entry["Proxy"]["Port"].GetInt();
            if (binding_entry["Proxy"].HasMember("Username") && binding_entry["Proxy"].HasMember("Password")) {
              log_.LogDebug() << "ConfigurationHandler::ConfigurationHandler Found client proxy credentials.";
              proxy.Username_ = binding_entry["Proxy"]["Username"].GetString();
              proxy.Password_ = binding_entry["Proxy"]["Password"].GetString();
              proxy.Has_credentials_ = true;
            } else {
              proxy.Has_credentials_ = false;
            }
          } else {
            // proxy not available
            has_proxy = false;
          }

          client_configs_.emplace_back(client_identifier, configuration_entry["Type"].GetString(),
                                       binding_entry["Identifier"].GetString());

          client_bindings_.emplace_back(binding_entry["Identifier"].GetString(), binding_entry["Type"].GetString(),
                                        binding_entry["TransportProtocol"].GetString(), auth, proxy, has_proxy);
        }
      }
    } else if (strcmp(configuration_entry["Type"].GetString(), "server") == 0) {
      log_.LogDebug() << "ConfigurationHandler::ConfigurationHandler Found server config.";
      std::vector<String> server_binding_ids;
      for (auto &bindingids : configuration_entry["BindingIds"].GetArray()) {
        server_binding_ids.emplace_back(bindingids.GetString());
      }
      server_configs_.emplace_back(configuration_entry["Identifier"].GetString(),
                                   configuration_entry["Type"].GetString(), server_binding_ids);
      // iterate bindings to find corresponding bindings ids
      for (auto &binding_entry : config_json["Bindings"].GetArray()) {
        for (auto binding_id : server_binding_ids) {
          if (strcmp(binding_entry["Identifier"].GetString(), binding_id.c_str()) == 0) {
            log_.LogDebug() << "ConfigurationHandler::ConfigurationHandler Found matching server_binding_id:"
                            << binding_id;
            // alter ipv6 version into url notation
            String server_address = additional::DetermineAddressVersion(binding_entry["Address"].GetString());
            String identifier = binding_entry["Identifier"].GetString();
            String type = binding_entry["Type"].GetString();
            String protocol = binding_entry["TransportProtocol"].GetString();
            int port = binding_entry["Port"].GetInt();
            log_.LogDebug() << "ConfigurationHandler::ConfigurationHandler For " << binding_id
                            << " using server_address: " << server_address << ":" << port
                            << " TransportProtocol: " << protocol;

            server_bindings_.emplace_back(identifier, type, protocol, server_address, port);
          }
        }
      }
    } else {
      log_.LogError() << "Configuration type is unknown. Valid types are [client, server]";
    }
  }
}

std::vector<String> ConfigurationHandler::GetServerBindingIds(const String &config_id) {
  std::vector<String> result;
  for (auto &config : server_configs_) {
    if (config.Id_.compare(config_id) == 0) {
      result = config.BindingIds_;
    }

    if (result.empty()) {
      log_.LogError() << "ConfigurationHandler::GetServerBindingIds: config id: " << config_id
                      << " not found in config file.";
    }
  }

  return result;
}

ServerBinding ConfigurationHandler::GetServerBinding(const String &binding_id) {
  ServerBinding result;
  for (auto &binding : server_bindings_) {
    if (binding.Id_.compare(binding_id) == 0) {
      result = binding;
    }
  }
  return result;
}

String ConfigurationHandler::GetClientBindingIds(const String &config_id) {
  String result;
  for (auto &config : client_configs_) {
    if (config.Id_.compare(config_id) == 0) {
      result = config.BindingId_;
    }

    if (result.empty()) {
      log_.LogError() << "ConfigurationHandler::GetClientBindingIds: config id: " << config_id
                      << " not found in config file. \n";
    }
  }

  return result;
}

ClientBinding ConfigurationHandler::GetClientBinding(const String &binding_id) {
  ClientBinding result;
  for (auto &binding : client_bindings_) {
    if (binding.Id_.compare(binding_id) == 0) {
      result = binding;
    }
  }
  return result;
}

}  // namespace config
}  // namespace rest
}  // namespace ara

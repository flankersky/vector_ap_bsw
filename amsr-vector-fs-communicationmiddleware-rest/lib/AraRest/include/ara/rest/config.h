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
/**        \file  config.h
 *        \brief  Class for configuration handling
 *
 *      \details  A class to read in a configuration file to configure client and server
 *
 *********************************************************************************************************************/
#ifndef LIB_ARAREST_INCLUDE_ARA_REST_CONFIG_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_CONFIG_H_
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ara/log/logging.hpp>
#include <iostream>
#include <vector>

#include "ara/rest/string.h"

namespace ara {
namespace rest {
namespace config {

namespace test {

class Configuration_ConstructConfigurationHandler_Test; /**> forward declaration for testing */
class Configuration_ConfigurationFunctions_Test;        /**> forward declaration for testing */

}  // namespace test

namespace additional {
/**
 * Checks if the given String is a ipv4 or ipv6 address and alters the ipv6 address to url notation (if it is not
 * already in url notation)
 * @param address A string which represents an ip adress
 * @return The ipv4 adress as given or the ipv6 adress in url notation
 */
String DetermineAddressVersion(String address);
}

/**
 * Typedef for transport protocol
 */
typedef String TransportProtocol_t;

/**
 * \brief Configuration parameters for host authorization
 *
 * This value represents the default authorization part of a Uri for all
 * client requests. An authorization component within a given Uri shall
 * override these values.
 */
struct Authorization_t {
  String Adress_;  ///< Denotes an IP address or FQDN of a proxy
  int Port_;       ///< Denotes an IP port number of a server
};

/**
 * \brief Configuration parameters for proxy settings
 *
 * This value represents the configuration parameters for a proxy instance.
 */
struct Proxy_t {
  String Adress_;         ///< Denotes an IP address of a proxy
  int Port_;              ///< Denotes an IP port number of a proxy
  bool Has_credentials_;  ///< Flag indicated if password and username settings are given
  String Password_;       ///< Denotes a password of a proxy
  String Username_;       ///< Denotes a username of a proxy
};

/** \brief Base type for protocol- and end-point specific
 *
 * A binding represents all the configuration parameters for either a
 * client or a server endpoint.
 */
struct Binding {
  /**
   * Default CTOR
   */
  Binding() = default;
  /**
   * Constructor
   * \param id Identifier for the binding
   * \param type The type of the binding (server/client)
   * \param tp The transport protocol of the binding (e.g. Http)
   */
  Binding(String id, String type, TransportProtocol_t tp);
  /**
   * \brief Uniquely identifies a binding configuration block
   */
  String Id_;

  /**
   * \brief Type of the binding (client or server)
   */
  String type_;

  /**
   * \brief Denotes the transport protocol of the given binding
   *
   * When a client is instantiated, it shall also instantiate
   * a suitable transport protocol binding which is uniquely identified
   * by this parameter.
   *
   * The protocol binding shall give access to the subsequent configuration
   * paramaters upon instantiation.
   */
  TransportProtocol_t transport_protocol_;
};

/**
 * Specific binding for clients
 */
struct ClientBinding : Binding {
  /**
  * Default CTOR
  */
  ClientBinding() = default;

  /**
   * Constructor
   * \param id Identifier for the binding
   * \param type The type of the binding (server/client)
   * \param tp The transport protocol of the binding (e.g. Http)
   * \param auth The authorization, containing address and port
   * \param proxy The proxy,containing address and port
   * \param has_proxy Whether (optional) proxy data provided or not
   */
  ClientBinding(String id, String type, TransportProtocol_t tp, Authorization_t auth, Proxy_t proxy, bool has_proxy);
  Authorization_t authorization_;  ///<  Configuration parameters for host authorization
  Proxy_t proxy_;                  ///<  Configuration parameters for proxy authorization
  bool has_proxy_;                 ///<  Whether proxy data is available or not
};

/**
 * Binding configuration for a server
 */
struct ServerBinding : Binding {
  /**
   * Default CTOR
   */
  ServerBinding() = default;
  /**
   * Constructor
   * \param id Identifier for the binding
   * \param type The type of the binding (server/client)
   * \param tp The transport protocol of the binding (e.g. Http)
   * \param address The address of the server
   * \param port The port of the server
   */
  ServerBinding(String id, String type, TransportProtocol_t tp, String address, int port);
  String address_;  ///<  Configuration parameters for server address
  int port_;        ///<  Configuration parameters for server port
};

/**
 * \brief Represents the configuration for a HTTP-based client or server endpoint
 *
 *  A binding is uniquely identified by its Id.
 */
struct HttpBinding : Binding {};

/** \brief Represents endpoint configurations
 *
 *  Endpoints configurations are uniquely identified by means of an identifier.  Depending on
 *  whether an endpoint is a client or server, a respective configuration record shall be looked-up.
 */
struct Configuration {
 public:
  /**
   * Constructor
   * \param id Identifier of the configuration
   * \param type
   */
  Configuration(String id, String type);

  /**
   * Default destructor
   */
  ~Configuration() {}

  /**
   * Unique identifier of a configuration record
   */
  String Id_;

  /**
   * Type of the configuration (server or client)
   */
  String type_;
};

/**
 * \brief Represents a client configuration record
 *
 * Only ClientConfiguration records are valid for client endpoints
 *
 */
struct ClientConfiguration : Configuration {
  /**
   * Constructor
   * \param id Identifier for the Client configuration
   * \param type Type of the configuration (client/server)
   * \param binding_id The binding id, determining which binding belongs to this configuration
   */
  ClientConfiguration(String id, String type, String binding_id);
  /**
   * The binding id, determining which binding belongs to this configuration
   */
  String BindingId_;
};

/**
 * \brief Represents a client configuration record
 *
 * Only ClientConfiguration records are valid for server endpoints
 *
 */
struct ServerConfiguration : Configuration {
  /**
   * Constructor
   * \param id Identifier for the Server configuration
   * \param type Type of the configuration (client/server)
   * \param binding_ids The binding ids (vector of binding identifier), determining which bindings belong to this
   * configuration
   */
  ServerConfiguration(String id, String type, std::vector<String> binding_ids);

  /**
   * The binding ids (vector of binding identifier), determining which bindings belong to this configuration
   */
  std::vector<String> BindingIds_;
};

/**
 * Configuration handler handles parsing of the config-json file and saves all available bindings and configs
 */
class ConfigurationHandler {
 public:
  /**
   * Configuration file
   */
  static String config_file;
  /**
   * Creates a singleton instance of ConfigurationHanlder if not already existing
   * \return
   */
  static ConfigurationHandler& Instance() {
    static ConfigurationHandler configHandlerInstance(config_file);

    return configHandlerInstance;
  }

  /**
   * Returns all ServerBindings read in from the configuration file
   * \param config_id
   * \return
   */
  std::vector<String> GetServerBindingIds(const String& config_id);

  /**
   * Returns the ServerBinding object linked to the given binding id
   * \param binding_id
   * \return
   */
  ServerBinding GetServerBinding(const String& binding_id);

  /**
   * TODO
   * @param config_id
   * @return
   */
  String GetClientBindingIds(const String& config_id);

  /**
   * TODO
   * @param binding_id
   * @return
   */
  ClientBinding GetClientBinding(const String& binding_id);

  // delete copy and move constructors and assign operators
  ConfigurationHandler(ConfigurationHandler const&) = delete;             // Copy construct deleted
  ConfigurationHandler(ConfigurationHandler&&) = delete;                  // Move construct deleted
  ConfigurationHandler& operator=(ConfigurationHandler const&) = delete;  // Copy assign deleted
  ConfigurationHandler& operator=(ConfigurationHandler&&) = delete;       // Move assign deleted

 protected:
  /**
   * Constructor
   * \param path The path to the config-json file plus the name of the file
   */
  explicit ConfigurationHandler(String path);

  /**
   * Destructor
   */
  virtual ~ConfigurationHandler() = default;

 private:
  /**
   * All available client configurations
   */
  std::vector<ClientConfiguration> client_configs_;

  /**
   * All available client bindings
   */
  std::vector<ClientBinding> client_bindings_;

  /**
   * All available server configurations
   */
  std::vector<ServerConfiguration> server_configs_;

  /**
   * All available server bindings
   */
  std::vector<ServerBinding> server_bindings_;

  /**
   * \brief The logger instance
   */
  ara::log::Logger& log_;

  friend class test::Configuration_ConstructConfigurationHandler_Test;  //< Friend decleration for testing
  friend class test::Configuration_ConfigurationFunctions_Test;         //< Friend decleration for testing
};
}  // namespace config
}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_CONFIG_H_

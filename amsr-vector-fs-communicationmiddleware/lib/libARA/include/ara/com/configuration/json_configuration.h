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
/**        \file  ara/com/configuration/json_configuration.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_CONFIGURATION_JSON_CONFIGURATION_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_CONFIGURATION_JSON_CONFIGURATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits>
#include <string>
#include "ara/com/configuration/configuration.h"
#include "ara/com/configuration/thread_pool_config.h"
#include "ara/log/logging.hpp"
#include "ara/per/internal/json_parser.h"

namespace ara {
namespace com {
namespace configuration {

/**
 * \brief JSON format specific configuration abstraction of the ara::com configuration ara::com
 */
class JsonConfiguration : public Configuration {
 public:
  /**
   * \brief Type for the JSON file path
   */
  using JsonFilePath = std::string;

  /**
   * \brief Constructor of JsonConfiguration. Parses ara::com application.json.
   * \param application_config_json_path Path to application.json to be parsed.
   */
  explicit JsonConfiguration(const JsonFilePath& application_config_json_path);

  /**
   * \brief Destructor
   */
  virtual ~JsonConfiguration() = default;

 private:
  /**
   * \brief Parses the JSON configuration file.
   */
  void ReadJsonConfiguration();

  /**
   * \brief Parse the ara::com application socket paths from JSON
   * \param application_json Application JSON configuration
   */
  void ReadSocketPaths(const ara::per::internal::json::JsonObject& application_json);

  /**
   * \brief Parse the ara::com runtime processing mode parameter
   * \param application_json Application JSON configuration
   */
  void ReadProcessingMode(const ara::per::internal::json::JsonObject& application_json);

  /**
   * \brief Parse a single ara::com application thread pool configuration from JSON
   * \tparam ConfigObject type template for thread_pool_configuration parameter to avoid complex RapidJSON object type.
   * \param thread_pool_configuration Single thread-pool configuration
   */
  template <typename ConfigObject>
  ThreadPoolConfig ReadThreadPool(const ConfigObject& thread_pool_configuration) {
    ara::log::Logger& logger = ara::log::CreateLogger("JsonConfiguration", "");
    logger.LogDebug() << __func__ << ":" << __LINE__ << " Reading ara::com thread pool configuration";

    // Check format and structure of mandatory parameters
    if (!thread_pool_configuration.HasMember("pool_id") || !thread_pool_configuration["pool_id"].IsUint()) {
      logger.LogError() << "Thread pool ID configuration structure not in expected format! Must be \"pool_id\": 42";
      throw std::runtime_error("Incorrect configuration structure for thread pool 'pool_id'");
    }
    if (!thread_pool_configuration.HasMember("num_threads") || !thread_pool_configuration["num_threads"].IsUint()) {
      logger.LogError() << "Thread pool number of threads configuration structure not in expected format! Must be "
                           "\"num_threads\": 42";
      throw std::runtime_error("Incorrect configuration structure for thread pool 'num_threads'");
    }
    if (!thread_pool_configuration.HasMember("max_tasks") || !thread_pool_configuration["max_tasks"].IsUint()) {
      logger.LogError() << "Thread pool max. number of tasks configuration structure not in expected format! Must be "
                           "\"max_tasks\": 42";
      throw std::runtime_error("Incorrect configuration structure for thread pool 'max_tasks'");
    }
    if (!thread_pool_configuration.HasMember("assign_to") || !thread_pool_configuration["assign_to"].IsArray() ||
        !(thread_pool_configuration["assign_to"].GetArray().Size() > 0)) {
      logger.LogError() << "Thread pool assignment configuration structure not in expected format! Must be "
                           "\"assign_to\": [ { <assignment params> }, ... ]";
      throw std::runtime_error("Incorrect configuration structure for thread pool assignment 'assign_to'");
    }

    // Read parameters check limits
    const auto pool_id = thread_pool_configuration["pool_id"].GetUint();
    const auto num_threads = thread_pool_configuration["num_threads"].GetUint();
    const auto max_tasks = thread_pool_configuration["max_tasks"].GetUint();
    logger.LogDebug() << "Reading configuration for thread-pool ID: " << pool_id
                      << ", number of threads: " << num_threads << ", max tasks: " << max_tasks;

    if (pool_id > std::numeric_limits<ThreadPoolConfig::PoolId>::max()) {
      logger.LogError() << "Thread pool ID exceeds allowed limit of max. "
                        << static_cast<std::uint16_t>(std::numeric_limits<ThreadPoolConfig::PoolId>::max());
      throw std::runtime_error("Incorrect configuration of pool ID: Exceeds allowed limit");
    }
    if (num_threads > std::numeric_limits<ThreadPoolConfig::ThreadCount>::max()) {
      logger.LogError() << "Thread pool number of threads exceeds allowed limit of max. "
                        << static_cast<std::uint16_t>(std::numeric_limits<ThreadPoolConfig::ThreadCount>::max());
      throw std::runtime_error("Incorrect configuration of pool thread count: Exceeds allowed limit");
    }

    // Create the thread pool configuration
    ThreadPoolConfig cfg{};
    const bool sched_policy_valid{thread_pool_configuration.HasMember("sched_policy") &&
                                  thread_pool_configuration["sched_policy"].IsString()};
    const bool sched_priority_valid{thread_pool_configuration.HasMember("sched_priority") &&
                                    thread_pool_configuration["sched_priority"].IsUint()};
    bool sched_specific{false};
    ara::com::internal::RtSchedPolicy policy{};
    ara::com::internal::SchedPriority priority{};

    if (sched_policy_valid && sched_priority_valid) {
      sched_specific = true;
      const std::string policy_str = thread_pool_configuration["sched_policy"].GetString();
      priority = thread_pool_configuration["sched_priority"].GetInt();

      // Map the string to a policy of the enumeration class.
      if (policy_str == "SCHED_RR") {
        policy = ara::com::internal::RtSchedPolicy::kRoundRobin;
      } else if (policy_str == "SCHED_FIFO") {
        policy = ara::com::internal::RtSchedPolicy::kFifo;
      } else {
        policy = ara::com::internal::RtSchedPolicy::kFifo;
        logger.LogWarn() << "Wrong RT scheduling settings. Policies available: SCHED_RR or SCHED_FIFO. "
                            "Falling back to SCHED_FIFO by default.";
      }
    }

    if (sched_specific) {
      cfg = ThreadPoolConfig{static_cast<ThreadPoolConfig::PoolId>(pool_id),
                             static_cast<ThreadPoolConfig::ThreadCount>(num_threads), max_tasks, policy, priority};
    } else {
      cfg = ThreadPoolConfig{static_cast<ThreadPoolConfig::PoolId>(pool_id),
                             static_cast<ThreadPoolConfig::ThreadCount>(num_threads), max_tasks};
    }

    // Read assignment configuration(s)
    for (const auto& assignment_config : thread_pool_configuration["assign_to"].GetArray()) {
      cfg << ReadThreadPoolAssignment(assignment_config);
    }

    return cfg;
  }

  /**
   * \brief Parse a single ara::com thread pool instance assignment configuration from JSON
   * \tparam ConfigObject type template for assignment_configuration parameter to avoid complex RapidJSON object type.
   * \param assignment_configuration Single thread-pool assignment configuration.
   */
  template <typename ConfigObject>
  ThreadPoolAssignmentConfig ReadThreadPoolAssignment(const ConfigObject& assignment_configuration) {
    ara::log::Logger& logger = ara::log::CreateLogger("JsonConfiguration", "");
    logger.LogDebug() << __func__ << ":" << __LINE__ << " Reading ara::com thread pool assignment configuration";

    // Check JSON structure
    if (!assignment_configuration.HasMember("name") || !assignment_configuration["name"].IsString()) {
      logger.LogError()
          << "Thread pool assignment name configuration structure not in expected format! Must be \"name\": "
             "\"<ServiceInterface class name>\"";
      throw std::runtime_error("Incorrect configuration structure for thread pool assignment 'name'");
    }
    if (!assignment_configuration.HasMember("instance_identifier") ||
        !assignment_configuration["instance_identifier"].IsString()) {
      logger.LogError() << "Thread pool assignment instance identifier configuration structure not in expected format! "
                           "Must be \"instance_identifier\": \"<InstanceIdentifier>\"";
      throw std::runtime_error("Incorrect configuration structure for thread pool assignment 'instance_identifier'");
    }

    // Read parameters
    const std::string service_interface_name = assignment_configuration["name"].GetString();
    const std::string instance_identifier = assignment_configuration["instance_identifier"].GetString();
    logger.LogDebug() << "Reading configuration for thread-pool assignment name: " << service_interface_name
                      << ", instance identifier: " << instance_identifier;

    // Create the thread pool assignment configuration
    ThreadPoolAssignmentConfig cfg{service_interface_name, instance_identifier};
    return cfg;
  }

  /**
   * \brief Path to the application.json
   */
  const JsonFilePath application_config_json_path_;
};

}  // namespace configuration
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_CONFIGURATION_JSON_CONFIGURATION_H_

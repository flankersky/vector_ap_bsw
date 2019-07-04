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
/**        \file  generic_configuration.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_CONFIGURATION_GENERIC_CONFIGURATION_H_
#define SRC_CONFIGURATION_GENERIC_CONFIGURATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ara/per/internal/json_parser.h>
#include <vac/testing/test_adapter.h>

namespace amsr {
namespace diag {
namespace configuration {
/**
 * \brief Default Maximal number of payload bytes to be written to the log.
 */
constexpr std::size_t kDefaultLogMaxNumberPayloadBytes = 10;

/**
 * \brief Contains configuration values of generic parameters for logging information.
 */
struct LogOutputConfiguration {
  /**
   * \brief IP address for udp.
   */
  std::size_t max_payload_bytes{0};

  /**
   * \brief Overloaded constructor.
   */
  explicit LogOutputConfiguration(std::size_t max_number_payload_bytes) : max_payload_bytes(max_number_payload_bytes) {}

  /**
   * \brief Constructor.
   */
  LogOutputConfiguration() {}
};

/**
 * \brief Contains configuration structures for all generic parameters.
 */
struct GenericConfiguration {
  /**
   * \brief generic configuration of output.
   */
  LogOutputConfiguration log_output_configuration;

  /**
   * \brief Destructor.
   */
  VIRTUALMOCK ~GenericConfiguration() = default;

  /**
   * \brief This method is used to parse the generic parameters.
   */
  VIRTUALMOCK void ParseGenericParameters(ara::per::internal::json::JsonObject do_ip_obj);

  /**
   * \brief Constructor.
   */
  GenericConfiguration() {}

 private:
  /**
   * \brief This method is used to parse the LogOutput Object.
   */
  VIRTUALMOCK void ParseLogOutput(ara::per::internal::json::JsonObject do_ip_obj);

 private:
  FRIEND_TEST(GenericConfiguration, ReadLogOutputParameters);
  FRIEND_TEST(GenericConfiguration, MaxPayloadBytesMissing);
};

}  // namespace configuration
}  // namespace diag
}  // namespace amsr
#endif  // SRC_CONFIGURATION_GENERIC_CONFIGURATION_H_

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
/**        \file  doip_channel_configuration.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_CONFIGURATION_DOIP_CHANNEL_CONFIGURATION_H_
#define LIB_LIBDOIP_SRC_CONFIGURATION_DOIP_CHANNEL_CONFIGURATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ara/per/internal/json_parser.h>
#include <vac/memory/leaky_array_allocator.h>
#include <vac/testing/test_adapter.h>
#include <string>
#include <vector>

namespace amsr {
namespace diag {
namespace configuration {
/**
 * \brief Contains configuration values for UDPChannel.
 */
struct UDPChannelConfiguration {
  /**
   * \brief IP address for udp.
   */
  std::string udp_ip_address;
  /**
   * \brief Broadcast address for udp.
   */
  std::string udp_broadcast_address;
  /**
   * \brief Overloaded constructor.
   */
  UDPChannelConfiguration(std::string ip_address, std::string broadcast_address)
      : udp_ip_address(ip_address), udp_broadcast_address(broadcast_address) {}
  /**
   * \brief Constructor.
   */
  UDPChannelConfiguration() {}
};
/**
 * \brief Contains configuration values for TCPChannel.
 */
struct TCPChannelConfiguration {
  /**
   * \brief IP address for tcp.
   */
  std::string tcp_ip_address;
  /**
   * \brief Maximal number of tcp channels.
   */
  uint8_t max_tcp_channels{0};
  /**
   * \brief Overloaded constructor.
   */
  TCPChannelConfiguration(std::string ip_address, uint8_t max_channels)
      : tcp_ip_address(ip_address), max_tcp_channels(max_channels) {}
  /**
   * \brief Constructor.
   */
  TCPChannelConfiguration() {}
};

/**
 * \brief Contains configuration values for DoIPChannel.
 */
struct DoIPChannelConfiguration {
  /**
   * \brief Type definition for vector of target addresses
   */
  using VectorTargetAddress = std::vector<uint16_t, vac::memory::LeakyArrayAllocator<uint16_t>>;
  /**
   * \brief Maximal length of message.
   */
  uint32_t max_message_length{0};
  /**
   * \brief Type of node.
   */
  uint8_t node_type{0};
  /**
   * \brief Primary target address
   */
  uint16_t primary_target_address{0};
  /**
   * \brief vector of used target addresses
   */
  VectorTargetAddress vector_target_address;
  /**
   * \brief udp channel configuration.
   */
  struct UDPChannelConfiguration udp_channel_configuration;
  /**
   * \brief tcp channel configuration.
   */
  struct TCPChannelConfiguration tcp_channel_configuration;

 public:
  /**
   * \brief Constructor.
   */
  DoIPChannelConfiguration();
  /**
   * \brief Overloaded constructor.
   */
  DoIPChannelConfiguration(uint32_t max_msg_length, uint8_t type_node, uint16_t primary_ta);
  /**
   * \brief Destructor.
   */
  VIRTUALMOCK ~DoIPChannelConfiguration() = default;

  /**
   * \brief This method is used to load the DoIP Channel configuration.
   * \param path_doip_config path to the configuration file.
   */
  void LoadDoIPChannelConfiguration(const std::string& path_doip_config);

 private:
  /**
   * \brief This method is used to parse the DoIPChannel Object.
   */
  VIRTUALMOCK void ParseDoIPChannel(ara::per::internal::json::JsonObject do_ip_obj);
  /**
   * \brief This method is used to parse the TCPChannel Object.
   */
  VIRTUALMOCK void ParseTCPChannel(ara::per::internal::json::JsonObject tcp_obj);
  /**
   * \brief This method is used to parse the UDPChannel Object.
   */
  VIRTUALMOCK void ParseUDPChannel(ara::per::internal::json::JsonObject udp_obj);
  /**
   * \brief This method is used to parse the array of target addresses.
   */
  VIRTUALMOCK void ParseTargetAddresses(const ara::per::internal::json::Value& json_target_addresses);

 private:
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPChannelConfigurationTest, ParseTCPUDPChannelOK);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPChannelConfigurationTest, ParseUDPChannelKO);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPChannelConfigurationTest, ParseTCPChannelKO);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPChannelConfigurationTest, ParseNodeTypeKO);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPChannelConfigurationTest, ParseMaxMessageLengthKO);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPChannelConfigurationTest, ParsePrimaryTAKO);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPChannelConfigurationTest, ParseArrayTAKO);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPChannelConfigurationTest, ParseUDPChannelOK);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPChannelConfigurationTest, ParseUDPIPmissing);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPChannelConfigurationTest, ParseUDPBroadcastmissing);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPChannelConfigurationTest, ParseUDPPortmissing);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPChannelConfigurationTest, ParseTCPChannelOK);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPChannelConfigurationTest, ParseTCPIPmissing);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPChannelConfigurationTest, ParseTCPPortmissing);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPChannelConfigurationTest, ParseTCPMaxConnectMissing);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPChannelConfigurationTest, ParseTCPMaxConnectExceedsUINT8MAX);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPChannelConfigurationTest, ParseTargetAddresses);
};
}  // namespace configuration
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_CONFIGURATION_DOIP_CHANNEL_CONFIGURATION_H_

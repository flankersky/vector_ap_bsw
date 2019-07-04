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
/**        \file  doip_channel_configuration.cc
 *        \brief  doip_channel_configuration.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <stdexcept>
#include <string>

#include "ara/log/logging.hpp"
#include "configuration/doip_channel_configuration.h"

namespace amsr {
namespace diag {
namespace configuration {

DoIPChannelConfiguration::DoIPChannelConfiguration(uint32_t max_msg_length, uint8_t type_node, uint16_t primary_ta)
    : max_message_length(max_msg_length), node_type(type_node), primary_target_address(primary_ta) {}

DoIPChannelConfiguration::DoIPChannelConfiguration() {}

void DoIPChannelConfiguration::LoadDoIPChannelConfiguration(const std::string& path_doip_config) {
  ara::per::internal::json::JsonDocument do_ip_config_doc = ara::per::internal::json::LoadFile(path_doip_config);
  ara::per::internal::json::JsonObject config_obj = do_ip_config_doc.GetObject();
  if (config_obj.HasMember("DoIPChannel")) {
    // Parse DoIPChannel object.
    ara::log::LogDebug() << "Parsing DoIPChannel.";
    ParseDoIPChannel(config_obj["DoIPChannel"].GetObject());
  } else {
    throw std::runtime_error("Configuration for DoIP not found.");
  }
}

void DoIPChannelConfiguration::ParseDoIPChannel(ara::per::internal::json::JsonObject do_ip_obj) {
  // Parse UDPChannel object.
  if (do_ip_obj.HasMember("UDPChannel")) {
    ara::log::LogDebug() << "Parsing UDPChannel.";
    this->ParseUDPChannel(do_ip_obj["UDPChannel"].GetObject());
  } else {
    throw std::runtime_error("Configuration for UDP channel not found.");
  }

  // Parse TCPPChannel object.
  if (do_ip_obj.HasMember("TCPChannel")) {
    ara::log::LogDebug() << "Parsing TCPChannel.";
    this->ParseTCPChannel(do_ip_obj["TCPChannel"].GetObject());
  } else {
    throw std::runtime_error("Configuration for TCP channel not found.");
  }

  // Get type of node.
  if (do_ip_obj.HasMember("NodeType")) {
    ara::log::LogDebug() << "Get type of node.";
    this->node_type = static_cast<uint8_t>(do_ip_obj["NodeType"].GetUint());
  } else {
    throw std::runtime_error("Configuration for type of node not found.");
  }

  // Get maximal length of message.
  if (do_ip_obj.HasMember("MaxMessageLength")) {
    ara::log::LogDebug() << "Get maximal length of message.";
    this->max_message_length = do_ip_obj["MaxMessageLength"].GetUint();
  } else {
    throw std::runtime_error("Configuration for message maximal length not found.");
  }

  // Get primary target address.
  if (do_ip_obj.HasMember("PrimaryTargetAddress")) {
    ara::log::LogDebug() << "Get primary target address.";
    this->primary_target_address = static_cast<uint16_t>(do_ip_obj["PrimaryTargetAddress"].GetUint());
  } else {
    throw std::runtime_error("Configuration for primary target address not found.");
  }

  // Get all of target addresses.
  if (do_ip_obj.HasMember("TargetAddressArray")) {
    ara::log::LogDebug() << "Get all of target addresses.";
    ParseTargetAddresses(do_ip_obj["TargetAddressArray"]);
  } else {
    throw std::runtime_error("Configuration for target addresses not found.");
  }
}

void DoIPChannelConfiguration::ParseTargetAddresses(const ara::per::internal::json::Value& json_target_addresses) {
  ara::per::internal::json::Value::ConstArray json_array_target_addresses = json_target_addresses.GetArray();
  vector_target_address.reserve(json_array_target_addresses.Size());
  for (ara::per::internal::json::Value::ConstArray::ValueType& ta : json_array_target_addresses) {
    vector_target_address.push_back(static_cast<uint16_t>(ta.GetUint()));
  }
}

void DoIPChannelConfiguration::ParseUDPChannel(ara::per::internal::json::JsonObject udp_obj) {
  // Get IP address for udp.
  if (udp_obj.HasMember("IPAddress")) {
    ara::log::LogDebug() << "Get IP address for udp";
    this->udp_channel_configuration.udp_ip_address = udp_obj["IPAddress"].GetString();
  } else {
    throw std::runtime_error("Configuration for UDP IP address not found.");
  }
  // Get broadcast address for udp.
  if (udp_obj.HasMember("BroadcastAddress")) {
    ara::log::LogDebug() << "Get broadcast address for udp";
    this->udp_channel_configuration.udp_broadcast_address = udp_obj["BroadcastAddress"].GetString();
  } else {
    throw std::runtime_error("Configuration for UDP broadcast address not found.");
  }
}

void DoIPChannelConfiguration::ParseTCPChannel(ara::per::internal::json::JsonObject tcp_obj) {
  // Get IP address for udp.
  if (tcp_obj.HasMember("IPAddress")) {
    ara::log::LogDebug() << "Get IP address for tcp";
    this->tcp_channel_configuration.tcp_ip_address = tcp_obj["IPAddress"].GetString();
  } else {
    throw std::runtime_error("Configuration for TCP IP address not found.");
  }

  // Get max number of channels.
  if (tcp_obj.HasMember("MaxChannels")) {
    ara::log::LogDebug() << "Get max number of channels.";
    uint32_t max_channel_value = tcp_obj["MaxChannels"].GetUint();
    if (max_channel_value > UINT8_MAX) {
      throw std::overflow_error("MaxChannels > UINT8_MAX");
    }
    this->tcp_channel_configuration.max_tcp_channels = static_cast<uint8_t>(max_channel_value);
  } else {
    throw std::runtime_error("Configuration for maximal number of channel not found.");
  }
}

}  // namespace configuration
}  // namespace diag
}  // namespace amsr

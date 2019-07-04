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
/**        \file  dext_configuration.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/
#include <algorithm>
#include <limits>
#include <stdexcept>

#include "ara/log/logging.hpp"

#include "configuration/dext_configuration.h"

namespace amsr {
namespace diag {
namespace configuration {

void DextConfiguration::ParseDext(ara::per::internal::json::JsonValue::ConstObject dext_obj) {
  // Parse the target address.
  if (dext_obj.HasMember("TargetAddress")) {
    ara::log::LogDebug() << "DextConfiguration: Get target address.";
    target_address = static_cast<std::uint16_t>(dext_obj["TargetAddress"].GetUint());
  } else {
    throw std::runtime_error("TargetAddress for diagnostic manager is not available in configuration file.");
  }

  // Parse the conversations number.
  if (dext_obj.HasMember("NumberConversations")) {
    ara::log::LogDebug() << "DextConfiguration: Get NumberConversations.";
    number_conversations = static_cast<std::uint8_t>(dext_obj["NumberConversations"].GetUint());
  } else {
    throw std::runtime_error("Conversations number for diagnostic manager is not available in configuration file.");
  }

  // Parse the length of UDS message.
  if (dext_obj.HasMember("UdsMessageLength")) {
    ara::log::LogDebug() << "DextConfiguration: Get length of uds message.";
    uds_message_length = static_cast<std::uint32_t>(dext_obj["UdsMessageLength"].GetUint());
  } else {
    throw std::runtime_error("Conversations number for diagnostic manager is not available in configuration file.");
  }

  // Parse the length of UDS message.
  if (!dext_obj.HasMember("MaxNumberOfResponsePending")) {
    throw std::runtime_error("MaxNumberOfResponsePending is not available in configuration file.");
  }

  max_number_of_response_pending_responses = GetUInt8Value(dext_obj, "MaxNumberOfResponsePending");

  // Parse the sessions.
  if (dext_obj.HasMember("Sessions")) {
    ara::log::LogDebug() << "DextConfiguration: Get sessions.";
    ParseSessions(dext_obj["Sessions"]);
  } else {
    throw std::runtime_error("Configuration of sessions for diagnostic manager is not available.");
  }

  // Parse the services.
  if (dext_obj.HasMember("ServiceTable")) {
    ara::log::LogDebug() << "DextConfiguration: Get service table.";
    ParseServices(dext_obj["ServiceTable"]);
  } else {
    throw std::runtime_error("Configuration of service table for diagnostic manager is not available.");
  }

  // Parse Did Table if available.
  // If Read/Write Did services are not supported, there is no need for the DidTable.
  if (dext_obj.HasMember("DidTable")) {
    ara::log::LogDebug() << "DextConfiguration: Get DID table.";
    if (!dext_obj["DidTable"].IsObject()) {
      throw std::runtime_error("Json value DidTable is not an object in the configuration file.");
    }
    ParseDidsTableObject(dext_obj["DidTable"].GetObject());
  } else {
    ara::log::LogInfo() << "No configuration of DID table for diagnostic manager is available.";
  }

  // Parse the Rid Table if available.
  if (dext_obj.HasMember("RidTable")) {
    ara::log::LogDebug() << "DextConfiguration: Get RID table.";
    ParseRidTable(dext_obj["RidTable"]);
  } else {
    ara::log::LogInfo() << "No RID table for diagnostic manager is available.";
  }
}

void DextConfiguration::ParseSessions(const ara::per::internal::json::Value& json_value_sessions) {
  ara::per::internal::json::JsonValue::ConstArray array = json_value_sessions.GetArray();
  std::size_t number_sessions = array.Size();
  sessions.reserve(number_sessions);
  // Parse every member of the array and push it in the vector of sessions.
  for (ara::per::internal::json::JsonValue::ConstArray::ValueType& json_session : array) {
    SessionConfiguration session;
    if ((json_session.GetObject()).HasMember("Id")) {
      ara::log::LogDebug() << "DextConfiguration: Get session id.";
      session.session_id = static_cast<std::uint8_t>((json_session.GetObject())["Id"].GetUint());
    } else {
      throw std::runtime_error("Session Id not found.");
    }
    if ((json_session.GetObject()).HasMember("P2Time")) {
      ara::log::LogDebug() << "DextConfiguration: Get p2 duration.";
      session.p2_time = std::chrono::milliseconds((json_session.GetObject())["P2Time"].GetUint());
    } else {
      throw std::runtime_error("P2Time not found.");
    }
    if ((json_session.GetObject()).HasMember("P2StarTime")) {
      ara::log::LogDebug() << "DextConfiguration: Get p2 star duration.";
      session.p2_star_time = std::chrono::milliseconds((json_session.GetObject())["P2StarTime"].GetUint());
    } else {
      throw std::runtime_error("P2 Star Time not found.");
    }
    sessions.push_back(session);
  }
}

ServiceConfiguration::ListSubServices DextConfiguration::ParseSubServices(
    const ara::per::internal::json::Value& json_value_sub_services) {
  ara::per::internal::json::JsonValue::ConstArray array = json_value_sub_services.GetArray();
  std::size_t number_sub_services = array.Size();
  ServiceConfiguration::ListSubServices list_sub_services;
  list_sub_services.reserve(number_sub_services);
  // Parse every member of the array and push it in the vector of sub services.
  for (ara::per::internal::json::JsonValue::ConstArray::ValueType& json_sub_service : array) {
    SubServiceConfiguration sub_service;
    // Get ID.
    sub_service.id = GetUInt8Value(json_sub_service.GetObject(), "Id");

    // Get Handler.
    sub_service.handler_info = ParseHandlerInfo(json_sub_service.GetObject());

    // Get Preconditions.
    if (json_sub_service.GetObject().HasMember("Preconditions")) {
      sub_service.preconditions.emplace(ParsePreconditions(json_sub_service.GetObject()));
    }

    list_sub_services.emplace_back(std::move(sub_service));
  }

  return list_sub_services;
}

void DextConfiguration::ParseServices(const ara::per::internal::json::Value& json_value_services) {
  ara::per::internal::json::JsonValue::ConstArray array = json_value_services.GetArray();
  std::size_t number_services = array.Size();
  services.reserve(number_services);
  // Parse every member of the array and push it in the vector of sessions.
  for (ara::per::internal::json::JsonValue::ConstArray::ValueType& json_service : array) {
    ServiceConfiguration service;
    // Get ID.
    service.id = GetUInt8Value(json_service.GetObject(), "Id");

    // Get Handler.
    service.handler_info = ParseHandlerInfo(json_service.GetObject());

    // Get Preconditions.
    if (json_service.GetObject().HasMember("Preconditions")) {
      service.preconditions.emplace(ParsePreconditions(json_service.GetObject()));
    }

    // Check if Service has subfunctions.
    if (json_service.GetObject().HasMember("HasSubFunctions")) {
      service.has_sub_functions = GetBoolValue(json_service.GetObject(), "HasSubFunctions");
    }

    // Get the subservices.
    if (json_service.GetObject().HasMember("SubService")) {
      service.sub_services = ParseSubServices(json_service.GetObject()["SubService"]);
    }

    services.emplace_back(std::move(service));
  }
}

HandlerInfo DextConfiguration::ParseHandlerInfo(ara::per::internal::json::JsonValue::ConstObject json_object) {
  HandlerInfo handler_info;
  // Get handler Object.
  ara::per::internal::json::JsonValue::ConstObject handler_object = GetJsonValue(json_object, "Handler").GetObject();

  // Get Handler Type.
  handler_info.handler_type = GetStringValue(handler_object, "HandlerType");

  // Get instance.
  handler_info.instance = GetStringValue(handler_object, "Instance");

  return handler_info;
}

Preconditions DextConfiguration::ParsePreconditions(ara::per::internal::json::JsonValue::ConstObject json_object) {
  using JsonValue = ara::per::internal::json::JsonValue;
  Preconditions preconditions;
  // Get Preconditions Object.
  JsonValue::ConstObject preconditions_object = GetJsonValue(json_object, "Preconditions").GetObject();

  // Get Sessions if it exists.
  if (preconditions_object.HasMember("Sessions") == true) {
    const JsonValue& json_value = preconditions_object["Sessions"];
    if (!json_value.IsArray()) {
      throw std::runtime_error("DextConfiguration::ParsePreconditions: Sessions is not an array");
    }
    JsonValue::ConstArray sessions_array = json_value.GetArray();
    preconditions.sessions_ids.reserve(sessions_array.Size());
    for (ara::per::internal::json::JsonValue::ConstArray::ValueType& json_session : sessions_array) {
      if (!json_session.IsUint()) {
        throw std::runtime_error("DextConfiguration::ParsePreconditions: Sessions does not contain integer");
      }
      preconditions.sessions_ids.emplace_back(json_session.GetUint());
    }
  }

  // Get SecurityAccessLevels if it exists.
  if (preconditions_object.HasMember("SecurityAccessLevels") == true) {
    const JsonValue& json_value = preconditions_object["SecurityAccessLevels"];
    if (!json_value.IsArray()) {
      throw std::runtime_error("DextConfiguration::ParsePreconditions: SecurityAccessLevels is not an array");
    }
    JsonValue::ConstArray security_access_levels_array = json_value.GetArray();
    preconditions.security_access_levels_ids.reserve(security_access_levels_array.Size());
    for (ara::per::internal::json::JsonValue::ConstArray::ValueType& json_value_security :
         security_access_levels_array) {
      if (!json_value_security.IsUint()) {
        throw std::runtime_error(
            "DextConfiguration::ParsePreconditions: SecurityAccessLevels does not contain integer");
      }
      preconditions.security_access_levels_ids.emplace_back(json_value_security.GetUint());
    }
  }
  return preconditions;
}

DataElementConfiguration DextConfiguration::ParseDataElementObject(
    ara::per::internal::json::JsonValue::ConstObject json_object) {
  DataElementConfiguration data_element;

  // Get maximal length.
  data_element.max_length = GetUInt8Value(json_object, "MaxLength");

  // Get minimal length.
  data_element.min_length = GetUInt8Value(json_object, "MinLength");

  // Get Handler.
  data_element.handler = ParseHandlerInfo(json_object);

  return data_element;
}

DidConfiguration DextConfiguration::ParseDidConfigurationObject(
    ara::per::internal::json::JsonValue::ConstObject json_object) {
  DidConfiguration did_configuration;

  // Get ID.
  did_configuration.id = GetUInt16Value(json_object, "Id");

  // Get did data elements.
  const ara::per::internal::json::JsonValue& json_value = GetJsonValue(json_object, "DidDataElements");
  ara::per::internal::json::JsonValue::ConstArray array_data_elements = json_value.GetArray();
  did_configuration.did_data_elements.reserve(array_data_elements.Size());
  for (ara::per::internal::json::JsonValue::ConstArray::ValueType& data_element_json : array_data_elements) {
    if (!data_element_json.IsObject()) {
      throw std::runtime_error("Json value DidDataElements is not an array of object in the configuration file.");
    }
    did_configuration.did_data_elements.emplace_back(ParseDataElementObject(data_element_json.GetObject()));
  }

  // Get Write and Read operations configurations.
  // Check if there is at least one of the both operations.
  if (!json_object.HasMember("Read") && !json_object.HasMember("Write")) {
    throw std::runtime_error("DextConfiguration::ParseDidConfigurationObject: No Operation for did: '" +
                             std::to_string(did_configuration.id) + "' found");
  }
  // Get Read preconditions.
  if (json_object.HasMember("Read")) {
    did_configuration.read_preconditions.emplace(ParsePreconditions(json_object["Read"].GetObject()));
  }
  // Get Write preconditions.
  if (json_object.HasMember("Write")) {
    did_configuration.write_preconditions.emplace(ParsePreconditions(json_object["Write"].GetObject()));
  }

  return did_configuration;
}

void DextConfiguration::ParseRidTable(const ara::per::internal::json::Value& json_value_rid_table) {
  if (!json_value_rid_table.IsArray()) {
    throw std::runtime_error("DextConfiguration::ParseRidTable: 'RidTable' is not an array");
  }
  ara::per::internal::json::JsonValue::ConstArray array_rids = json_value_rid_table.GetArray();
  rids_table.reserve(array_rids.Size());

  // Parse every member of the array and push it in the vector of rids.
  for (ara::per::internal::json::JsonValue::ConstArray::ValueType& json_rid_conf : array_rids) {
    rids_table.emplace_back(ParseRidConfiguration(json_rid_conf));
  }
}

RidConfiguration DextConfiguration::ParseRidConfiguration(
    ara::per::internal::json::JsonValue::ConstArray::ValueType& json_rid_conf) {
  RidConfiguration rid_configuration;
  // Get ID.
  rid_configuration.id = GetUInt16Value(json_rid_conf.GetObject(), "Id");

  // Get Handler.
  rid_configuration.handler_info = ParseHandlerInfo(json_rid_conf.GetObject());

  // Get Preconditions.
  if (json_rid_conf.GetObject().HasMember("Preconditions")) {
    rid_configuration.preconditions.emplace(ParsePreconditions(json_rid_conf.GetObject()));
  }

  // Get configuration for Start, Stop and RequestResults operations.
  rid_configuration.start_operation = ParseRidOperationObject(json_rid_conf.GetObject(), "Start");
  rid_configuration.stop_operation = ParseRidOperationObject(json_rid_conf.GetObject(), "Stop");
  rid_configuration.request_results_operation = ParseRidOperationObject(json_rid_conf.GetObject(), "RequestResults");

  // At least one of the operations should be available.
  if (!rid_configuration.start_operation.has_value() && !rid_configuration.stop_operation.has_value() &&
      !rid_configuration.request_results_operation.has_value()) {
    throw std::runtime_error("DextConfiguration::ParseRidConfiguration: rid '" + std::to_string(rid_configuration.id) +
                             "' no operation available");
  }

  return rid_configuration;
}

vac::memory::optional<RidOperationConfiguration> DextConfiguration::ParseRidOperationObject(
    ara::per::internal::json::JsonValue::ConstObject json_object, std::string operation) {
  vac::memory::optional<RidOperationConfiguration> rid_operation_configuration;
  // Check if configuration for operation exists.
  if (!json_object.HasMember(operation.c_str())) {
    return rid_operation_configuration;
  }

  // Check if configuration for operation is stored in a json object.
  if (!json_object[operation.c_str()].IsObject()) {
    throw std::runtime_error("DextConfiguration::ParseRidOperationObject: Json Value for operation " + operation +
                             " is not a json object");
  }

  // RidConfiguration can now be instantiated.
  rid_operation_configuration.emplace();
  // Get the operation's object.
  ara::per::internal::json::JsonValue::ConstObject json_object_operation = json_object[operation.c_str()].GetObject();

  // Check configuration for requested data.
  rid_operation_configuration->req_data = ParseRidOperationDataObject(json_object_operation, "RidRequestData");

  // Check configuration for returned data.
  rid_operation_configuration->res_data = ParseRidOperationDataObject(json_object_operation, "RidResponseData");

  // Check if preconditions are available.
  if (json_object_operation.HasMember("Preconditions")) {
    rid_operation_configuration->preconditions.emplace(ParsePreconditions(json_object_operation));
  }
  return rid_operation_configuration;
}

vac::memory::optional<RidOperationConfiguration::RidData> DextConfiguration::ParseRidOperationDataObject(
    ara::per::internal::json::JsonValue::ConstObject json_object_operation, std::string data) {
  vac::memory::optional<RidOperationConfiguration::RidData> result;

  if (!json_object_operation.HasMember(data.c_str())) {
    return result;
  }

  result.emplace(ParseRidDataObject(json_object_operation[data.c_str()].GetObject()));
  return result;
}

RidOperationConfiguration::RidData DextConfiguration::ParseRidDataObject(
    ara::per::internal::json::JsonValue::ConstObject json_data) {
  std::uint8_t min_length = GetUInt8Value(json_data, "MinLength");
  std::uint8_t max_length = GetUInt8Value(json_data, "MaxLength");
  RidOperationConfiguration::RidData rid_data{min_length, max_length};
  return rid_data;
}

void DextConfiguration::ParseDidsTableObject(ara::per::internal::json::JsonValue::ConstObject json_object) {
  // Get Max number of dids to read.
  dids_table.max_number_dids_to_read = GetUInt16Value(json_object, "MaxNumberOfDidsToRead");

  // Get did data elements.
  const ara::per::internal::json::JsonValue& json_value = GetJsonValue(json_object, "Dids");
  ara::per::internal::json::JsonValue::ConstArray array_dids = json_value.GetArray();
  dids_table.dids.reserve(array_dids.Size());
  for (ara::per::internal::json::JsonValue::ConstArray::ValueType& did_json : array_dids) {
    if (!did_json.IsObject()) {
      throw std::runtime_error("Json value Dids is not an array of object in the configuration file.");
    }
    dids_table.dids.emplace_back(ParseDidConfigurationObject(did_json.GetObject()));
  }
}

std::uint8_t DextConfiguration::GetUInt8Value(ara::per::internal::json::JsonValue::ConstObject json_object,
                                              std::string name) {
  // GetUInt32Value throws an exception when not available or not UInt32
  std::uint32_t temp_value = GetUInt32Value(json_object, name);

  if (temp_value > std::numeric_limits<std::uint8_t>::max()) {
    std::string error_msg = "'" + name + "' is greater than 255!";
    throw std::range_error(error_msg);
  }

  return static_cast<std::uint8_t>(temp_value);
}

std::uint16_t DextConfiguration::GetUInt16Value(ara::per::internal::json::JsonValue::ConstObject json_object,
                                                std::string name) {
  // GetUInt32Value throws an exception when not available or not UInt32
  std::uint32_t temp_value = GetUInt32Value(json_object, name);

  if (temp_value > std::numeric_limits<std::uint16_t>::max()) {
    std::string error_msg = "'" + name + "' = " + std::to_string(temp_value) + " is greater than 65535!";
    throw std::range_error(error_msg);
  }

  return static_cast<std::uint16_t>(temp_value);
}

std::string DextConfiguration::GetStringValue(ara::per::internal::json::JsonValue::ConstObject json_object,
                                              std::string name) {
  // GetJsonValue throws an exception when not available
  const ara::per::internal::json::JsonValue& json_value = GetJsonValue(json_object, name);

  if (json_value.IsString() == false) {
    std::string error_msg = "'" + name + "' is not a string!";
    throw std::range_error(error_msg);
  }

  std::string value = json_value.GetString();
  ara::log::LogDebug() << "DextConfiguration: " << name << " is '" << value << "'";
  return value;
}

bool DextConfiguration::GetBoolValue(ara::per::internal::json::JsonValue::ConstObject json_object, std::string name) {
  // GetJsonValue throws an exception when not available
  const ara::per::internal::json::JsonValue& json_value = GetJsonValue(json_object, name);

  if (json_value.IsBool() == false) {
    std::string error_msg = "'" + name + "' is not a bool!";
    throw std::range_error(error_msg);
  }

  bool value = json_value.GetBool();
  ara::log::LogDebug() << "DextConfiguration: " << name << " is '" << value << "'";
  return value;
}

std::uint32_t DextConfiguration::GetUInt32Value(ara::per::internal::json::JsonValue::ConstObject json_object,
                                                std::string name) {
  // GetJsonValue throws an exception when not available
  const ara::per::internal::json::JsonValue& json_value = GetJsonValue(json_object, name);

  if (json_value.IsNumber() == false) {
    std::string error_msg = "'" + name + "' is not a number!";
    throw std::range_error(error_msg);
  }

  if (json_value.IsUint() == false) {
    std::string error_msg = "'" + name + "' is not an unsigned integer!";
    throw std::range_error(error_msg);
  }

  std::uint32_t value = json_value.GetUint();
  ara::log::LogDebug() << "DextConfiguration: " << name << " is '" << value << "'";
  return value;
}

const ara::per::internal::json::JsonValue& DextConfiguration::GetJsonValue(
    ara::per::internal::json::JsonValue::ConstObject json_object, std::string name) {
  CheckIsAvailable(json_object, name);

  return json_object[name.c_str()];
}

void DextConfiguration::CheckIsAvailable(ara::per::internal::json::JsonValue::ConstObject json_object,
                                         std::string name) {
  if (json_object.HasMember(name.c_str()) == false) {
    std::string error_msg = "'" + name + "' is not available in configuration file!";
    throw std::runtime_error(error_msg);
  }
}

}  // namespace configuration
}  // namespace diag
}  // namespace amsr

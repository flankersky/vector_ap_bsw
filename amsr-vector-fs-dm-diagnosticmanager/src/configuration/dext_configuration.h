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
/**        \file  dext_configuration.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_CONFIGURATION_DEXT_CONFIGURATION_H_
#define SRC_CONFIGURATION_DEXT_CONFIGURATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <ara/per/internal/json_parser.h>
#include <vac/container/static_vector.h>
#include <vac/memory/leaky_array_allocator.h>
#include <vac/memory/optional.h>
#include <vac/testing/test_adapter.h>
#include <chrono>
#include <string>
#include <utility>
#include <vector>

namespace amsr {
namespace diag {
namespace configuration {
/**
 * \brief Contains the preconditions.
 */
struct Preconditions {
 public:
  // TODO(PAASR-2651) : Analyze where and how the LeakyArrayAllocator shall be replaced by adequate implementations.
  /**
   * \brief Type definition for list of sessions or security access levels IDs.
   */
  using ListIDs = vac::container::StaticVector<std::uint8_t, vac::memory::LeakyArrayAllocator<std::uint8_t>>;

  /**
   * \brief List of Sessions IDs.
   */
  ListIDs sessions_ids;

  /**
   * \brief List security access levels IDs.
   */
  ListIDs security_access_levels_ids;

  /**
   * \brief default ctor.
   */
  Preconditions() = default;

  /**
   * \brief move constructor.
   */
  Preconditions(Preconditions&&) = default;

  /**
   * \brief move assigment operator.
   */
  Preconditions& operator=(Preconditions&&) = default;

  // Remove copy constructor and operator since static containers cannot be copied.
  Preconditions(const Preconditions&) = delete;
  Preconditions& operator=(const Preconditions&) = delete;
};

/**
 * \brief Contains the Handler information.
 */
struct HandlerInfo {
  /**
   * \brief Handler Type.
   */
  std::string handler_type;

  /**
   * \brief Instance.
   */
  std::string instance;

  /**
   * \brief default ctor.
   */
  HandlerInfo() = default;

  /**
   * \brief move constructor.
   */
  HandlerInfo(HandlerInfo&&) = default;

  /**
   * \brief move assigment operator.
   */
  HandlerInfo& operator=(HandlerInfo&&) = default;

  // Remove copy constructor and operator since static containers cannot be copied.
  HandlerInfo(const HandlerInfo&) = delete;
  HandlerInfo& operator=(const HandlerInfo&) = delete;
};

/**
 * \brief Contains configuration information of a Session.
 */
struct SessionConfiguration {
  /**
   * \brief Session id.
   */
  std::uint8_t session_id{0};

  /**
   * \brief Duration of P2 in ms.
   */
  std::chrono::milliseconds p2_time{};

  /**
   * \brief Duration of P2 star in ms.
   */
  std::chrono::milliseconds p2_star_time{};

  /**
   * \brief Constructor.
   */
  SessionConfiguration() {}
  /**
   * \brief Overloaded constructor.
   */
  SessionConfiguration(std::uint8_t sessionId, std::uint16_t p2Time, std::uint16_t p2StarTime)
      : session_id(sessionId), p2_time(p2Time), p2_star_time(p2StarTime) {}
};

/**
 * \brief Contains configuration information of a sub service.
 */
struct SubServiceConfiguration {
  /**
   * \brief Service id.
   */
  std::uint8_t id{0};

  /**
   * \brief Handler.
   */
  HandlerInfo handler_info;

  /**
   * \brief Preconditions.
   */
  vac::memory::optional<Preconditions> preconditions;

  /**
   * \brief Constructor.
   */
  SubServiceConfiguration() {}

  /**
   * \brief move constructor.
   */
  SubServiceConfiguration(SubServiceConfiguration&&) = default;

  /**
   * \brief move assignment operator.
   */
  SubServiceConfiguration& operator=(SubServiceConfiguration&&) = default;

  // Remove copy constructor and operator since static containers cannot be copied.
  SubServiceConfiguration(const SubServiceConfiguration&) = delete;
  SubServiceConfiguration& operator=(const SubServiceConfiguration&) = delete;
};

/**
 * \brief Contains configuration information of a service.
 */
struct ServiceConfiguration {
  /**
   * \brief Service id.
   */
  std::uint8_t id{0};

  /**
   * \brief Handler.
   */
  HandlerInfo handler_info;

  /**
   * \brief Preconditions.
   */
  vac::memory::optional<Preconditions> preconditions;

  /**
   * \brief Type definition for list of sub services.
   */
  using ListSubServices =
      vac::container::StaticVector<SubServiceConfiguration, vac::memory::LeakyArrayAllocator<SubServiceConfiguration>>;

  /**
   * \brief List of available sub service configurations.
   */
  ListSubServices sub_services;

  /**
   * \brief Boolean indicating if service has subfunctions.
   */
  bool has_sub_functions{false};

  /**
   * \brief Constructor.
   */
  ServiceConfiguration() {}

  /**
   * \brief move constructor.
   */
  ServiceConfiguration(ServiceConfiguration&&) = default;

  /**
   * \brief move assignment operator.
   */
  ServiceConfiguration& operator=(ServiceConfiguration&&) = default;

  // Remove copy constructor and operator since static containers cannot be copied.
  ServiceConfiguration(const ServiceConfiguration&) = delete;
  ServiceConfiguration& operator=(const ServiceConfiguration&) = delete;
};

/**
 * \brief Contains configuration for RID operation.
 */
struct RidOperationConfiguration {
  /**
   * \brief Struct containing Rid data information.
   */
  struct RidData {
    /**
     * \brief Minimum length.
     */
    std::uint8_t min_length;

    /**
     * \brief Maximal length.
     */
    std::uint8_t max_length;
  };

  /**
   * \brief Requested data.
   */
  vac::memory::optional<RidData> req_data;

  /**
   * \brief Returned data.
   */
  vac::memory::optional<RidData> res_data;

  /**
   * \brief Preconditions for current operation.
   */
  vac::memory::optional<Preconditions> preconditions;

  /**
   * \brief Default constructor.
   */
  RidOperationConfiguration() = default;

  /**
   * \brief move constructor.
   */
  RidOperationConfiguration(RidOperationConfiguration&&) = default;

  /**
   * \brief move assignment operator.
   */
  RidOperationConfiguration& operator=(RidOperationConfiguration&&) = default;

  // Remove copy constructor and operator since static containers cannot be copied.
  RidOperationConfiguration(const RidOperationConfiguration&) = delete;
  RidOperationConfiguration& operator=(const RidOperationConfiguration&) = delete;
};

/**
 * \brief Configuration of a routine service.
 */
struct RidConfiguration {
  /**
   * \brief Routine identifier.
   */
  std::uint16_t id{0};

  /**
   * \brief Handler.
   */
  HandlerInfo handler_info;

  /**
   * \brief Preconditions.
   */
  vac::memory::optional<Preconditions> preconditions;

  /**
   * \brief Configuration for Start operation.
   */
  vac::memory::optional<RidOperationConfiguration> start_operation;

  /**
   * \brief Configuration for stop operation.
   */
  vac::memory::optional<RidOperationConfiguration> stop_operation;

  /**
   * \brief Configuration for RequestResults operation.
   */
  vac::memory::optional<RidOperationConfiguration> request_results_operation;

  /**
   * \brief Default constructor.
   */
  RidConfiguration() = default;

  /**
   * \brief move constructor.
   */
  RidConfiguration(RidConfiguration&&) = default;

  /**
   * \brief move assignment operator.
   */
  RidConfiguration& operator=(RidConfiguration&&) = default;

  // Remove copy constructor and operator since static containers cannot be copied.
  RidConfiguration(const RidConfiguration&) = delete;
  RidConfiguration& operator=(const RidConfiguration&) = delete;
};

/**
 * \brief Contains configuration of data element.
 */
struct DataElementConfiguration {
  /**
   * \brief Maximal length.
   */
  std::uint8_t max_length{0};

  /**
   * \brief Minimal length.
   */
  std::uint8_t min_length{0};

  /**
   * \brief Handler.
   */
  HandlerInfo handler;

  /**
   * \brief Ctor.
   */
  DataElementConfiguration() = default;

  /**
   * \brief move constructor.
   */
  DataElementConfiguration(DataElementConfiguration&&) = default;

  /**
   * \brief move assignment operator.
   */
  DataElementConfiguration& operator=(DataElementConfiguration&&) = default;

  // Remove copy constructor and operator since static containers cannot be copied.
  DataElementConfiguration(const DataElementConfiguration&) = delete;
  DataElementConfiguration& operator=(const DataElementConfiguration&) = delete;
};

/**
 * \brief Contains configuration of a DID.
 */
struct DidConfiguration {
  /**
   * \brief Type definition for list of Did data elements.
   */
  using DidDataElements = vac::container::StaticVector<DataElementConfiguration,
                                                       vac::memory::LeakyArrayAllocator<DataElementConfiguration>>;

  /**
   * \brief data identifier.
   */
  std::uint16_t id{0};

  /**
   * \brief Table of DidDataElements.
   */
  DidDataElements did_data_elements;

  /**
   * \brief Preconditions for read operation.
   */
  vac::memory::optional<Preconditions> read_preconditions;

  /**
   * \brief Preconditions for write operation.
   */
  vac::memory::optional<Preconditions> write_preconditions;

  /**
   * \brief Ctor.
   */
  DidConfiguration() = default;

  /**
   * \brief move constructor.
   */
  DidConfiguration(DidConfiguration&&) = default;

  /**
   * \brief move assignment operator.
   */
  DidConfiguration& operator=(DidConfiguration&&) = default;

  // Remove copy constructor and operator since static containers cannot be copied.
  DidConfiguration(const DidConfiguration&) = delete;
  DidConfiguration& operator=(const DidConfiguration&) = delete;
};

/**
 * \brief Contains configuration for Did Table.
 */
struct DidsTableConfiguration {
  /**
     * \brief Type definition for list of Dids.
     */
  using Dids = vac::container::StaticVector<DidConfiguration, vac::memory::LeakyArrayAllocator<DidConfiguration>>;

  /**
   * \brief Maximal number of Dids to read.
   */
  std::uint16_t max_number_dids_to_read{1};

  /**
   * \brief Table of Dids.
   */
  Dids dids;

  /**
  * \brief Ctor.
  */
  DidsTableConfiguration() = default;

  /**
   * \brief move constructor.
   */
  DidsTableConfiguration(DidsTableConfiguration&&) = default;

  /**
   * \brief move assignment operator.
   */
  DidsTableConfiguration& operator=(DidsTableConfiguration&&) = default;

  // Remove copy constructor and operator since static containers cannot be copied.
  DidsTableConfiguration(const DidsTableConfiguration&) = delete;
  DidsTableConfiguration& operator=(const DidsTableConfiguration&) = delete;
};

/**
 * \brief Contains configuration information of a diagnostic manager.
 */
struct DextConfiguration {
  // TODO(PAASR-2651) : Analyze where and how the LeakyArrayAllocator shall be replaced by adequate implementations.
  /**
   * \brief Type definition for array of sessions
   */
  using SessionsArray = std::vector<SessionConfiguration, vac::memory::LeakyArrayAllocator<SessionConfiguration>>;

  /**
   * \brief Type definition for array of services
   */
  using ServicesArray =
      vac::container::StaticVector<ServiceConfiguration, vac::memory::LeakyArrayAllocator<ServiceConfiguration>>;

  /**
   * \brief Type definition for list of Rids.
   */
  using Rids = vac::container::StaticVector<RidConfiguration, vac::memory::LeakyArrayAllocator<RidConfiguration>>;

  /**
   * \brief Length of uds message.
   */
  std::uint32_t uds_message_length{0u};
  /**
   * \brief target address of diagnostic manager.
   */
  std::uint16_t target_address{0u};

  /**
   * \brief number of conversations of diagnostic manager.
   */
  std::uint8_t number_conversations{0u};

  /**
   * The number of supported response pending messages.
   */
  std::uint8_t max_number_of_response_pending_responses{0};

  /**
   * \brief Array of sessions.
   */
  SessionsArray sessions;

  /**
   * \brief Array of services.
   */
  ServicesArray services;

  /**
   * \brief Table of Dids.
   */
  DidsTableConfiguration dids_table;

  /**
   * \brief Table of Rids.
   */
  Rids rids_table;

  /**
   * \brief Constructor.
   */
  DextConfiguration() {}
  /**
   * \brief Overloaded constructor.
   */
  DextConfiguration(std::uint32_t UdsMessageLength, std::uint16_t targetAddress, std::uint8_t numberConversations,
                    SessionConfiguration session)
      : uds_message_length(UdsMessageLength), target_address(targetAddress), number_conversations(numberConversations) {
    sessions.reserve(1);
    sessions.push_back(session);
  }
  /**
 * \brief move constructor.
 */
  DextConfiguration(DextConfiguration&&) = default;

  /**
   * \brief move assignment operator.
   */
  DextConfiguration& operator=(DextConfiguration&&) = default;

  // Remove copy constructor and operator since static containers cannot be copied.
  DextConfiguration(const DextConfiguration&) = delete;
  DextConfiguration& operator=(const DextConfiguration&) = delete;

  /**
   * \brief Destructor.
   */
  VIRTUALMOCK ~DextConfiguration() = default;
  /**
   * \brief This method is used to parse the Dext Object.
   */
  void ParseDext(ara::per::internal::json::JsonValue::ConstObject dext_obj);

 private:
  /**
   * \brief This method is used to parse the session objects.
   */
  VIRTUALMOCK void ParseSessions(const ara::per::internal::json::Value& json_value_sessions);

  /**
   * \brief This method is used to parse the sub services objects.
   * \param json_services json::Value containing the services information.
   * \return list of sub services.
   */
  VIRTUALMOCK ServiceConfiguration::ListSubServices ParseSubServices(
      const ara::per::internal::json::Value& json_value_sub_services);

  /**
   * \brief This method is used to parse the services objects.
   * \param json_services json::Value containing the services information.
   */
  VIRTUALMOCK void ParseServices(const ara::per::internal::json::Value& json_value_services);

  /**
   * \brief This method is used to parse the handler information.
   * \param json_object sonValue::Object containing the handler information.
   * \return handler information.
   */
  VIRTUALMOCK HandlerInfo ParseHandlerInfo(ara::per::internal::json::JsonValue::ConstObject json_object);

  /**
   * \brief This method is used to parse the preconditions objects.
   * \param json_object sonValue::Object containing the preconditions information.
   * \return preconditions.
   */
  VIRTUALMOCK Preconditions ParsePreconditions(ara::per::internal::json::JsonValue::ConstObject json_object);

  /**
   * \brief This method is used to parse the DataElement objects.
   * \param json_object sonValue::Object containing the DataElements information.
   * For this method the json_object should directly access the inside parameter of the object : json_object =
   * base_json_object[DataElement].GetObject()
   * \return DataElementConfiguration.
   */
  VIRTUALMOCK DataElementConfiguration
  ParseDataElementObject(ara::per::internal::json::JsonValue::ConstObject json_object);

  /**
   * \brief This method is used to parse the DidConfiguration objects.
   * \param json_object sonValue::Object containing the DidConfiguration information.For this method the json_object
   * should directly access the inside parameter of the object : json_object =
   * base_json_object[DidConfiguration].GetObject()
   * \return DidConfiguration.
   * \throws std::runtime_error is thrown when json value "DidDataElements" is not an array of object.
   */
  VIRTUALMOCK DidConfiguration
  ParseDidConfigurationObject(ara::per::internal::json::JsonValue::ConstObject json_object);

  /**
   * \brief This method is used to parse the DidsTableConfiguration objects.
   * \param json_object sonValue::Object containing the DidsTableConfiguration information. For this method the
   * json_object should directly access the inside parameter of the object : json_object =
   * base_json_object[DidsTable].GetObject()
   * \throws std::runtime_error is thrown when Json value "Dids" is not an array of object.
   * \return DidsTableConfiguration.
   */
  VIRTUALMOCK void ParseDidsTableObject(ara::per::internal::json::JsonValue::ConstObject json_object);

  /**
   * \brief This method is used to parse the RidTable objects.
   * \param json_value_rid_table json::Value containing the rids information.
   * \throws std::runtime_error is thrown when json_value_rid_table is not an array of object.
   */
  VIRTUALMOCK void ParseRidTable(const ara::per::internal::json::Value& json_value_rid_table);

  /**
   * \brief This method is used to parse Rid object.
   * \param json_rid_conf json::Value containing the rid information.
   */
  VIRTUALMOCK RidConfiguration
  ParseRidConfiguration(ara::per::internal::json::JsonValue::ConstArray::ValueType& json_rid_conf);

  /**
   * \brief This method is used to parse the configuration of the different Rid operations.
   * \param json_object sonValue::Object containing the operations information. For this method the
   * json_object should directly access the inside parameter of the object : json_object =
   * base_json_object[Rid].GetObject()
   * \throws std::runtime_error is thrown when operation json value is not an json object.
   */
  VIRTUALMOCK vac::memory::optional<RidOperationConfiguration> ParseRidOperationObject(
      ara::per::internal::json::JsonValue::ConstObject json_object, std::string operation);

  /**
   * \brief This method is used to parse the configuration of the different Rid operations data.
   * \param json_object_operation jsonValue::Object containing the operations information. For this method the
   * json_object should directly access the inside parameter of the object : json_object =
   * base_json_object[RidOperation].GetObject().
   * \param data name of object data to be parsed.
   * \return optional containing the parsed information of data element.
   */
  VIRTUALMOCK vac::memory::optional<RidOperationConfiguration::RidData> ParseRidOperationDataObject(
      ara::per::internal::json::JsonValue::ConstObject json_object_operation, std::string data);

  /**
   * \brief This method is used to parse the configuration of the different Rid operations data.
   * \param json_object_operation jsonValue::Object containing the operations information. For this method the
   * json_object should directly access the inside parameter of the object : json_object =
   * base_json_object[RidOperationData].GetObject().
   * \return parsed RidData.
   */
  VIRTUALMOCK RidOperationConfiguration::RidData ParseRidDataObject(
      ara::per::internal::json::JsonValue::ConstObject json_data);

  /**
     * \brief Gets the value as byte or throws an exception
     * Checks if a JsonValue with the request value is available and returns it or throws an exception.
     * \param json_object the parent object that is expected to contain the parameter
     * \param name The name of a JSON parameter
     * \returns the value as byte (when possible)
     * \throws std::runtime_error is thrown when the provided value is not available
     * \throws std::range_error when the value is available but not a number or not unsigned
     */
  static std::uint8_t GetUInt8Value(ara::per::internal::json::JsonValue::ConstObject json_object, std::string name);

  /**
       * \brief Gets the value as 16 bits value or throws an exception
       * Checks if a JsonValue with the request value is available and returns it or throws an exception.
       * \param json_object the parent object that is expected to contain the parameter
       * \param name The name of a JSON parameter
       * \returns the 16 bits value (when possible)
       * \throws std::runtime_error is thrown when the provided value is not available
       * \throws std::range_error when the value is available but not a number or not unsigned
       */
  static std::uint16_t GetUInt16Value(ara::per::internal::json::JsonValue::ConstObject json_object, std::string name);

  /**
  * \brief Gets the value as string or throws an exception
  * Checks if a JsonValue with the request value is available and returns it or throws an exception.
  * \param json_object the parent object that is expected to contain the parameter
  * \param name The name of a JSON parameter
  * \returns the value as string (when possible)
  * \throws std::range_error when the value is available but not a string
  * \throws std::runtime_error is thrown when the provided value is not available
  */
  static std::string GetStringValue(ara::per::internal::json::JsonValue::ConstObject json_object, std::string name);

  /**
  * \brief Gets the value as bool or throws an exception
  * Checks if a JsonValue with the request value is available and returns it or throws an exception.
  * \param json_object the parent object that is expected to contain the parameter
  * \param name The name of a JSON parameter
  * \returns the value as bool (when possible)
  * \throws std::range_error when the value is available but not a bool
  * \throws std::runtime_error is thrown when the provided value is not available
  */
  bool GetBoolValue(ara::per::internal::json::JsonValue::ConstObject json_object, std::string name);

  /**
   * \brief Gets the value as unsigned integer or throws an exception
   * Checks if a JsonValue with the request value is available and returns it or throws an exception.
   * \param json_object the parent object that is expected to contain the parameter
   * \param name The name of a JSON parameter
   * \returns the value as unsigned integer (when possible)
   * \throws std::runtime_error is thrown when the provided value is not available
   * \throws std::range_error when the value is available but not a number or not unsigned
   */
  static std::uint32_t GetUInt32Value(ara::per::internal::json::JsonValue::ConstObject json_object, std::string name);

  /**
   * Checks if a JsonValue with the request value is available and returns it or throws an exception.
   * \param json_object the parent object that is expected to contain the parameter
   * \param name The name of a JSON parameter
   * \returns A reference to the found JsonValue
   * \throws std::runtime_error is thrown when the provided value is not available
   */
  static const ara::per::internal::json::JsonValue& GetJsonValue(
      ara::per::internal::json::JsonValue::ConstObject json_object, std::string name);

  /**
   * \brief Checks if the provided value is available
   * \param json_object the parent object that is expected to contain the parameter
   * \param name The name of a JSON parameter
   * \throws std::runtime_error is thrown when the provided value is not available
   */
  static void CheckIsAvailable(ara::per::internal::json::JsonValue::ConstObject json_object, std::string name);

  // friend declaration for testing
  FRIEND_TEST(DextConfiguration, ParseSessions);
  FRIEND_TEST(DextConfiguration, SessionIdMissing);
  FRIEND_TEST(DextConfiguration, P2StarTimerMissing);
  FRIEND_TEST(DextConfiguration, P2TimerMissing);
  FRIEND_TEST(DextConfiguration, ParseServices);
  FRIEND_TEST(DextConfiguration, ParseHandlerInfo);
  FRIEND_TEST(DextConfiguration, ParsePreconditions);
  FRIEND_TEST(DextConfiguration, ParseSubServices);
  FRIEND_TEST(DextConfiguration, ParseDidsTableObject);
  FRIEND_TEST(DextConfiguration, DidsNotObjectArray);
  FRIEND_TEST(DextConfiguration, ParseDidConfigurationObject);
  FRIEND_TEST(DextConfiguration, ParseRidTable);
  FRIEND_TEST(DextConfiguration, ParseRidOperationObject);
  FRIEND_TEST(DextConfiguration, ParseRidOperationNotObject);
  FRIEND_TEST(DextConfiguration, ParseRidOperationNotAvailable);
  FRIEND_TEST(DextConfiguration, ParseDidConfigurationObjectNoReadWrite);
  FRIEND_TEST(DextConfiguration, DidDataElementsNotObjectArray);
  FRIEND_TEST(DextConfiguration, ParseDataElementObject);
  FRIEND_TEST(DextConfiguration, ParsePreconditionsSessionsNotArray);
  FRIEND_TEST(DextConfiguration, ParsePreconditionsSecurityAccessLevelsNotArray);
  FRIEND_TEST(DextConfiguration, ParsePreconditionsSessionsNotUintArray);
  FRIEND_TEST(DextConfiguration, ParsePreconditionsSecurityAccessLevelsNotUintArray);
};

}  // namespace configuration
}  // namespace diag
}  // namespace amsr

#endif  // SRC_CONFIGURATION_DEXT_CONFIGURATION_H_

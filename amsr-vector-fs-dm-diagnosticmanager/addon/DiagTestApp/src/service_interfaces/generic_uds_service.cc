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
/**        \file  generic_uds_service.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "generic_uds_service.h"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <thread>
#include <utility>

#include "ara/com/promise.h"
#include "ara/log/logging.hpp"

namespace diag_test_application {
namespace service_interfaces {

static std::string ToHexString(std::uint8_t byte) {
  std::stringstream stream;
  // print the byte as std::uint16_t to force a readable number (std::uint8_t is interpreted as character)
  stream << std::hex << std::setw(2) << std::setfill('0') << static_cast<std::uint16_t>(byte);
  return stream.str();
}

static std::string ToHexString(const guds::DataArrayType& data) {
  std::stringstream stream;
  stream << std::hex << std::setw(2) << std::setfill('0');
  for (std::stringstream::char_type byte : data) {
    // print the byte as std::uint16_t to force a readable number (std::uint8_t is interpreted as character)
    stream << static_cast<std::uint16_t>(byte);
  }
  return stream.str();
}

ara::com::Future<guds::skeleton::methods::Service::Output> GenericUDSService::Service(
    const guds::uint8& service_id, const guds::DataArrayType& request_data, const guds::MetaInfoType& /* MetaInfo */) {
  ara::log::LogDebug() << __func__ << " >> SID: '" << std::to_string(service_id) << "', RequestData size: '"
                       << request_data.size() << "'";

  ara::com::Promise<guds::skeleton::methods::Service::Output> promise;
  guds::skeleton::methods::Service::Output output;

  // check SID
  switch (service_id) {
    case kResponsePendingTestServiceId:  // test service (0xBA[timer-multiplicator: 1 byte])
      // length check: 0xBA request data must have one byte
      if (request_data.size() == 1) {
        Create0xBAPosResponse(request_data, output.ResponseData);
        promise.set_value(output);
      } else {
        CreateNRC(promise, NegativeResponseCodes::kIMLOIF);
      }
      break;
    default:  // send NRC ServiceNotSupported
      CreateNRC(promise, NegativeResponseCodes::kSNS);
  }
  return promise.get_future();
}

void GenericUDSService::Create0xBAPosResponse(const guds::DataArrayType& request_data, guds::DataArrayType& response) {
  assert(!request_data.empty());

  // Positive response SID: request SID + 0x40
  constexpr std::uint8_t kResponseSID = kResponsePendingTestServiceId + kPositiveResponseServiceIdModifier;

  const std::uint8_t data_byte = request_data[0];
  int sleep_time_in_ms = data_byte * 100;

  ara::log::LogDebug() << "GenericUDSService::" << __func__ << " >> Going to sleep for '"
                       << std::to_string(sleep_time_in_ms) << "' milliseconds.";

  std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time_in_ms));

  ara::log::LogDebug() << "GenericUDSService::" << __func__ << " >> Sleep finished.";

  response.resize(1);
  // prepare response
  response[0] = kResponseSID;

  ara::log::LogInfo() << __func__ << " >> Response (size: '" << response.size() << "'): 0x" << ToHexString(response);
}

void GenericUDSService::CreateNRC(ara::com::Promise<guds::skeleton::methods::Service::Output>& promise,
                                  NegativeResponseCodes nrc) {
  ara::log::LogDebug() << __func__ << " >> Preparing negative response:"
                       << " NRC='0x" << ToHexString(static_cast<std::uint8_t>(nrc)) << "'";

  using UDSServiceFailed =
      ara::diag::service_interfaces::generic_uds_service::skeleton::application_errors::UDSServiceFailed;
  using UDSResponseCodeType = ara::diag::service_interfaces::generic_uds_service::UDSResponseCodeType;

  promise.set_exception(std::make_exception_ptr(UDSServiceFailed{static_cast<UDSResponseCodeType>(nrc)}));
}

}  // namespace service_interfaces
}  // namespace diag_test_application

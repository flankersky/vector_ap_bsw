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
/**
 *      \file  data_identifier_F190.cc
 *      \brief  Contains the implementation of DataIdentifier4711
 *
 *      \details  The implementation of the VIN DID data identifeir.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "data_identifier_F190.h"

#include <algorithm>
#include <exception>
#include <stdexcept>
#include <string>
#include "ara/log/logging.hpp"

namespace diag_test_application {
namespace service_interfaces {

/*
// Helper to get the static size of std::array
template <typename T, size_t N>
struct array_size<std::array<T, N>> {
  static size_t constexpr size = N;
};
*/

DataIdentifierF190::DataIdentifierF190(const ara::com::InstanceIdentifier& instance_id)
    : ara::diag::service_interfaces::data_identifier_F190::DataIdentifier_SWCL_A_DID_F190Skeleton(instance_id) {
  vin_.fill(0x00);
}

ara::com::Future<ara::diag::service_interfaces::data_identifier_F190::skeleton::methods::Read::Output>
DataIdentifierF190::Read(const ara::diag::service_interfaces::data_identifier_F190::MetaInfoType& /*MetaInfo*/) {
  assert(vin_.size() == kVehicleIdentificationNumberSize);
  // create temporary buffer with an additional element for '\0' and initialize with zero
  std::array<VinDataType::value_type, kVehicleIdentificationNumberSize + 1> temp_vin_tostring{};
  assert(temp_vin_tostring.front() == 0x00);
  std::copy(vin_.cbegin(), vin_.cend(), temp_vin_tostring.begin());
  assert(temp_vin_tostring.back() == 0x00);

  // and copy the current VIN to a temporary buffer buffer for printing
  std::string vin_as_string{temp_vin_tostring.begin(), temp_vin_tostring.end()};

  ara::log::LogDebug() << "DataIdentifierF190::" << __func__ << " >> OUT-Parameters: VIN='" << vin_as_string.c_str()
                       << "'.";

  ara::com::Promise<ara::diag::service_interfaces::data_identifier_F190::skeleton::methods::Read::Output> promise;
  ara::diag::service_interfaces::data_identifier_F190::skeleton::methods::Read::Output output;

  assert(vin_.size() == output.dataRecord_vin.size());
  static_assert(output.dataRecord_vin.size() == kVehicleIdentificationNumberSize,
                "Size of dataRecord_vin not as expected!");

  std::copy_n(vin_.begin(), vin_.size(), output.dataRecord_vin.begin());

  promise.set_value(output);
  ara::log::LogDebug() << "DataIdentifierF190::" << __func__ << " >> finished returning future.";
  return promise.get_future();
}

ara::com::Future<ara::diag::service_interfaces::data_identifier_F190::skeleton::methods::Write::Output>
DataIdentifierF190::Write(
    const ara::diag::service_interfaces::data_identifier_F190::MetaInfoType& /*MetaInfo*/,
    const ara::diag::service_interfaces::data_identifier_F190::fixed_size_array_with_17_uint8_items& dataRecord_vin) {
  ara::log::LogDebug() << "DataIdentifierF190::" << __FUNCTION__ << " >> IN-Parameters: VIN (size "
                       << std::to_string(dataRecord_vin.size()) << ").";
  ara::com::Promise<ara::diag::service_interfaces::data_identifier_F190::skeleton::methods::Write::Output> promise;

  try {
    ara::diag::service_interfaces::data_identifier_F190::skeleton::methods::Write::Output output;

    // input VIN and member should be always of the same size
    assert(dataRecord_vin.size() == vin_.size());
    static_assert(dataRecord_vin.size() == kVehicleIdentificationNumberSize, "Size of dataRecord_vin not as expected!");

    std::copy_n(dataRecord_vin.begin(), vin_.size(), vin_.begin());
    promise.set_value(output);
  } catch (const std::exception& ex) {
    ara::log::LogError() << "DataIdentifierF190::" << __FUNCTION__ << " >> Exception: " << ex.what();
    promise.set_exception(std::make_exception_ptr(ex));
  }

  ara::log::LogDebug() << "DataIdentifierF190::" << __func__ << " >> finished returning future.";
  return promise.get_future();
}

ara::com::Future<ara::diag::service_interfaces::data_identifier_F190::skeleton::methods::Cancel::Output>
DataIdentifierF190::Cancel(const ara::diag::service_interfaces::data_identifier_F190::MetaInfoType& /*MetaInfo*/) {
  ara::log::LogDebug() << "DataIdentifierF190::" << __FUNCTION__;
  ara::com::Promise<ara::diag::service_interfaces::data_identifier_F190::skeleton::methods::Cancel::Output> promise;
  ara::diag::service_interfaces::data_identifier_F190::skeleton::methods::Cancel::Output output;
  promise.set_value(output);
  ara::log::LogDebug() << "DataIdentifierF190::" << __func__ << " >> finished returning future.";
  return promise.get_future();
}

}  // namespace service_interfaces
}  // namespace diag_test_application

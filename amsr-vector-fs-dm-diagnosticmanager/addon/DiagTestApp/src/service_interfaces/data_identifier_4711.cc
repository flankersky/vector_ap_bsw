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
/**        \file  data_identifier_4711.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "data_identifier_4711.h"

#include <string>

#include "ara/log/logging.hpp"

namespace diag_test_application {
namespace service_interfaces {

DataIdentifier4711::DataIdentifier4711(const ara::com::InstanceIdentifier& instance_id)
    : DataIdentifier_SWCL_A_DID_4711Skeleton(instance_id), data_element_one_(0), data_element_two_(0) {}

ara::com::Future<ara::diag::service_interfaces::data_identifier::skeleton::methods::Read::Output>
DataIdentifier4711::Read(const ara::diag::service_interfaces::data_identifier::MetaInfoType& /*MetaInfo*/) {
  ara::log::LogDebug() << __FUNCTION__ << " >> OUT-Parameters: DataElementOne: " << std::to_string(data_element_one_)
                       << ", DataElementTwo: " << std::to_string(data_element_two_) << ".";

  ara::com::Promise<ara::diag::service_interfaces::data_identifier::skeleton::methods::Read::Output> promise;
  ara::diag::service_interfaces::data_identifier::skeleton::methods::Read::Output output;

  output.dataRecord_DataElement_one = data_element_one_;
  output.dataRecord_DataElement_two = data_element_two_;

  promise.set_value(output);
  return promise.get_future();
}

ara::com::Future<ara::diag::service_interfaces::data_identifier::skeleton::methods::Write::Output>
DataIdentifier4711::Write(const ara::diag::service_interfaces::data_identifier::MetaInfoType& /*MetaInfo*/,
                          const ara::diag::service_interfaces::data_identifier::uint8& dataRecord_DataElement_one,
                          const ara::diag::service_interfaces::data_identifier::uint16& dataRecord_DataElement_two) {
  ara::log::LogDebug() << __func__
                       << " >> IN-Parameters: DataElementOne: " << std::to_string(dataRecord_DataElement_one)
                       << ", DataElementTwo: " << std::to_string(dataRecord_DataElement_two) << ".";
  ara::com::Promise<ara::diag::service_interfaces::data_identifier::skeleton::methods::Write::Output> promise;
  ara::diag::service_interfaces::data_identifier::skeleton::methods::Write::Output output;

  data_element_one_ = dataRecord_DataElement_one;
  data_element_two_ = dataRecord_DataElement_two;

  promise.set_value(output);
  return promise.get_future();
}

ara::com::Future<ara::diag::service_interfaces::data_identifier::skeleton::methods::Cancel::Output>
DataIdentifier4711::Cancel(const ara::diag::service_interfaces::data_identifier::MetaInfoType& /*MetaInfo*/) {
  ara::log::LogDebug() << __func__;
  ara::com::Promise<ara::diag::service_interfaces::data_identifier::skeleton::methods::Cancel::Output> promise;
  ara::diag::service_interfaces::data_identifier::skeleton::methods::Cancel::Output output;
  promise.set_value(output);
  return promise.get_future();
}

}  // namespace service_interfaces
}  // namespace diag_test_application

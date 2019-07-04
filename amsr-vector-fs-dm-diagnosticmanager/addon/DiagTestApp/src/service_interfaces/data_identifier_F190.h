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
 *      \file  data_identifier_F190.h
 *      \brief  Implementation of DID 0xF190
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTAPP_SRC_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_H_
#define ADDON_DIAGTESTAPP_SRC_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <array>
#include "ara/diag/service_interfaces/data_identifier_F190/DataIdentifier_SWCL_A_DID_F190_skeleton.h"

namespace diag_test_application {
namespace service_interfaces {

/**
 * \brief Implementation class for GenericUDSService skeleton.
 * This implementation of the GenericUDSService skeleton is intended to show the concept
 * of writing Diagnostic Manager extensions. The current implementation is a demo of
 * the UDS SessionService ($10). The demo implementation returns  that
 */
class DataIdentifierF190
    : public ara::diag::service_interfaces::data_identifier_F190::DataIdentifier_SWCL_A_DID_F190Skeleton {
 public:
  /**
   * The size of the Vehicle Identification Number (VIN)
   */
  static constexpr std::size_t kVehicleIdentificationNumberSize = 17;

  /**
   * \brief Constructor.
   */
  explicit DataIdentifierF190(const ara::com::InstanceIdentifier& instance_id);

  /**
   * \brief Destructor.
   */
  virtual ~DataIdentifierF190() = default;

  DataIdentifierF190(const DataIdentifierF190&) = delete;
  DataIdentifierF190(const DataIdentifierF190&&) = delete;
  DataIdentifierF190& operator=(const DataIdentifierF190&) = delete;
  DataIdentifierF190& operator=(const DataIdentifierF190&&) = delete;

  /**
   * \brief Read did method.
   */
  ara::com::Future<ara::diag::service_interfaces::data_identifier_F190::skeleton::methods::Read::Output> Read(
      const ara::diag::service_interfaces::data_identifier_F190::MetaInfoType& MetaInfo) override;

  /**
   * \brief Write did method.
   */
  ara::com::Future<ara::diag::service_interfaces::data_identifier_F190::skeleton::methods::Write::Output> Write(
      const ara::diag::service_interfaces::data_identifier_F190::MetaInfoType& MetaInfo,
      const ara::diag::service_interfaces::data_identifier_F190::fixed_size_array_with_17_uint8_items& dataRecord_vin)
      override;
  /**
   * \brief Cancel method.
   */
  ara::com::Future<ara::diag::service_interfaces::data_identifier_F190::skeleton::methods::Cancel::Output> Cancel(
      const ara::diag::service_interfaces::data_identifier_F190::MetaInfoType& MetaInfo) override;

 private:
  using VinElementType = ara::diag::service_interfaces::data_identifier_F190::uint8;
  using VinDataType = std::array<VinElementType, kVehicleIdentificationNumberSize>;
  /**
   * The Vehicle Identification Number (VIN) - 17 bytes
   */
  VinDataType vin_;

  FRIEND_TEST(DataIdentifierF190Tests, CheckConstructorAndDestructor);
  FRIEND_TEST(DataIdentifierF190Tests, CheckReadMethod);
  FRIEND_TEST(DataIdentifierF190Tests, CheckWriteMethod);
  FRIEND_TEST(DataIdentifierF190Tests, CheckCancelMethod);
};

}  // namespace service_interfaces
}  // namespace diag_test_application

#endif  // ADDON_DIAGTESTAPP_SRC_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_H_

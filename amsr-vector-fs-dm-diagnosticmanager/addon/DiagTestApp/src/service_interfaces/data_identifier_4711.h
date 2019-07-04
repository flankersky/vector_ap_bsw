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
/**        \file  data_identifier_4711.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTAPP_SRC_SERVICE_INTERFACES_DATA_IDENTIFIER_4711_H_
#define ADDON_DIAGTESTAPP_SRC_SERVICE_INTERFACES_DATA_IDENTIFIER_4711_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ara/diag/service_interfaces/data_identifier/DataIdentifier_SWCL_A_DID_4711_skeleton.h"

namespace diag_test_application {
namespace service_interfaces {

/**
 * \brief Implementation class for DID F190 skeleton.
 */
class DataIdentifier4711
    : public ara::diag::service_interfaces::data_identifier::DataIdentifier_SWCL_A_DID_4711Skeleton {
 public:
  /**
   * \brief Constructor.
   */
  explicit DataIdentifier4711(const ara::com::InstanceIdentifier& instance_id);

  /**
   * \brief Destructor.
   */
  virtual ~DataIdentifier4711() = default;

  DataIdentifier4711(const DataIdentifier4711&) = delete;

  DataIdentifier4711& operator=(const DataIdentifier4711&) = delete;

  /**
   * \brief Read did method.
   */
  ara::com::Future<ara::diag::service_interfaces::data_identifier::skeleton::methods::Read::Output> Read(
      const ara::diag::service_interfaces::data_identifier::MetaInfoType& MetaInfo) override;

  /**
   * \brief Write did method.
   */
  ara::com::Future<ara::diag::service_interfaces::data_identifier::skeleton::methods::Write::Output> Write(
      const ara::diag::service_interfaces::data_identifier::MetaInfoType& MetaInfo,
      const ara::diag::service_interfaces::data_identifier::uint8& dataRecord_DataElement_one,
      const ara::diag::service_interfaces::data_identifier::uint16& dataRecord_DataElement_two) override;
  /**
   * \brief Cancel method.
   */
  ara::com::Future<ara::diag::service_interfaces::data_identifier::skeleton::methods::Cancel::Output> Cancel(
      const ara::diag::service_interfaces::data_identifier::MetaInfoType& MetaInfo) override;

 private:
  /**
   * \brief Data Element One
   */
  ara::diag::service_interfaces::data_identifier::uint8 data_element_one_;

  /**
   * \brief Data Element Two
   */
  ara::diag::service_interfaces::data_identifier::uint16 data_element_two_;

  FRIEND_TEST(DataIdentifier4711Test, CheckConstructorAndDestructor);
  FRIEND_TEST(DataIdentifier4711Test, CheckReadMethod);
  FRIEND_TEST(DataIdentifier4711Test, CheckWriteMethod);
  FRIEND_TEST(DataIdentifier4711Test, CheckCancelMethod);
};

}  // namespace service_interfaces
}  // namespace diag_test_application

#endif  // ADDON_DIAGTESTAPP_SRC_SERVICE_INTERFACES_DATA_IDENTIFIER_4711_H_

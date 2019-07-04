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
/**        \file  did_info.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_DATA_DID_INFO_H_
#define SRC_SERVER_DATA_DID_INFO_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "configuration/dext_configuration.h"
#include "did_data_element_info.h"
#include "did_operation.h"
#include "server/conversation/access/access_state.h"
#include "server/service/service_processing_context.h"
#include "udstransport/uds_negative_response_code.h"
#include "vac/container/static_list.h"
#include "vac/memory/optional.h"
#include "vac/memory/three_phase_allocator.h"

namespace amsr {
namespace diag {
namespace server {
namespace data {
/**
 * \brief Implementation of DidInfo Class.
 */
class DidInfo {
 public:
  /**
   * \brief Type definiton for access category.
   */
  using AccessCategory = conversation::access::AccessCategory;

  /**
   * DID value type 0x0000 - 0xFFFF
   */
  using Did = std::uint16_t;

  /**
   * \brief Type definition for list of DidDataElementInfo.
   */
  using DataElementInfoList =
      vac::container::StaticList<DidDataElementInfo, vac::memory::ThreePhaseAllocator<DidDataElementInfo>>;

  /**
   * \brief Ctor.
   */
  explicit DidInfo(const configuration::DidConfiguration& did_conf);

  // Remove copy/move ctor/assignment.
  DidInfo(const DidInfo&) = delete;
  DidInfo& operator=(const DidInfo&) = delete;
  DidInfo(const DidInfo&&) = delete;
  DidInfo& operator=(const DidInfo&&) = delete;

  /**
   * \brief Check the conditions for the operation in current context.
   * \param processing_context current processing context
   * \param operation_type operation type
   * \return negative response code
   */
  ara::diag::udstransport::UdsNegativeResponseCode CheckConditions(
      const service::ServiceProcessingContext& processing_context, DidOperation::Type operation_type) const;

  /**
   * \brief Check the conditions for the operation in current context against the specified access category.
   * \param access_category access category (session or security level)
   * \param processing_context current processing context
   * \param operation_type operation type
   * \return negative response code
   */
  ara::diag::udstransport::UdsNegativeResponseCode CheckConditionsFor(
      AccessCategory access_category, const service::ServiceProcessingContext& processing_context,
      DidOperation::Type operation_type) const;

  /**
   * \brief Return the data identifier.
   */
  DidInfo::Did GetDid() const { return did_; }

  /**
   * \brief Return the maximal size of the data elements.
   * \return std:size_t max size of all data elements
   */
  std::size_t GetMaxSize() const;

  /**
   * \brief Return the minimal size of the data elements.
   * \return std:size_t min size of all data elements
   */
  std::size_t GetMinSize() const;

  /**
   * \brief Get the list of data element.
   */
  const DataElementInfoList& GetDataElementInfoList() const { return data_element_info_list_; }

 private:
  /**
   * \brief Return the did operation associated to the operation type.
   */
  const vac::memory::optional<DidOperation>& GetDidOperation(DidOperation::Type operation_type) const;

  /**
   * \brief data identifier.
   */
  Did did_;

  /**
   * \brief Read operation
   */
  vac::memory::optional<DidOperation> read_operation_;

  /**
   * \brief Write operation
   */
  vac::memory::optional<DidOperation> write_operation_;

  /**
   * \brief List of DidDataElementInfo.
   */
  DataElementInfoList data_element_info_list_;

 private:
  FRIEND_TEST(DidInfoTestFixture, ConstructOnlyReadOperation);
  FRIEND_TEST(DidInfoTestFixture, ConstructOnlyWriteOperation);
  FRIEND_TEST(DidInfoTestFixture, ConstructReadWriteOperation);
};
}  // namespace data
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_DATA_DID_INFO_H_

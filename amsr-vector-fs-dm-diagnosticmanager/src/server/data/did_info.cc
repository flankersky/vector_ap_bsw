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
/**        \file  did_info.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "did_info.h"

namespace amsr {
namespace diag {
namespace server {
namespace data {
DidInfo::DidInfo(const configuration::DidConfiguration& did_conf) : did_(did_conf.id) {
  // Instantiate operations if available.
  if (did_conf.read_preconditions.has_value()) {
    read_operation_.emplace(did_conf.read_preconditions);
  }
  if (did_conf.write_preconditions.has_value()) {
    write_operation_.emplace(did_conf.write_preconditions);
  }

  assert(read_operation_.has_value() || write_operation_.has_value());

  // Construct the list of data element information.
  data_element_info_list_.reserve(did_conf.did_data_elements.size());
  for (const auto& data_elem_config : did_conf.did_data_elements) {
    data_element_info_list_.emplace_back(data_elem_config);
  }
}

ara::diag::udstransport::UdsNegativeResponseCode DidInfo::CheckConditions(
    const service::ServiceProcessingContext& processing_context, DidOperation::Type operation_type) const {
  ara::diag::udstransport::UdsNegativeResponseCode check_session =
      CheckConditionsFor(AccessCategory::kSession, processing_context, operation_type);
  if (check_session != ara::diag::udstransport::UdsNegativeResponseCode::kPositiveResponse) {
    // Operation is not allowed in current sessions. Method can return with NRC code.
    ara::log::LogWarn() << "DidInfo::" << __func__ << ": Operation " << static_cast<std::uint16_t>(operation_type)
                        << " not allowed in current session.";
    return check_session;
  }
  // Operation is allowed in current session. Security level has now to be checked.
  return CheckConditionsFor(AccessCategory::kSecurityLevel, processing_context, operation_type);
}

ara::diag::udstransport::UdsNegativeResponseCode DidInfo::CheckConditionsFor(
    AccessCategory access_category, const service::ServiceProcessingContext& processing_context,
    DidOperation::Type operation_type) const {
  const vac::memory::optional<DidOperation>& did_operation = GetDidOperation(operation_type);

  // Get the preconditions of the current operation
  if (!did_operation.has_value()) {
    // operation is not available
    return ara::diag::udstransport::UdsNegativeResponseCode::kRequestOutOfRange;
  }
  const vac::memory::optional<diagpreconditions::DiagPreconditions>& preconditions_optional =
      did_operation.value().GetPreconditions();

  // If optional hasn't any value, return positive response.
  if (!preconditions_optional.has_value()) {
    return ara::diag::udstransport::UdsNegativeResponseCode::kPositiveResponse;
  }

  const diagpreconditions::DiagPreconditions& preconditions = preconditions_optional.value();
  // Check if conditions are fulfilled for this access category.
  if (!preconditions.IsAllowedIn(processing_context, access_category)) {
    switch (access_category) {
      case AccessCategory::kSession:
        return ara::diag::udstransport::UdsNegativeResponseCode::kRequestOutOfRange;

      case AccessCategory::kSecurityLevel:
        return ara::diag::udstransport::UdsNegativeResponseCode::kSecurityAccessDenied;

      default:
        throw std::runtime_error("DidInfo::CheckConditionsFor: unknown access category");
    }
  }

  return ara::diag::udstransport::UdsNegativeResponseCode::kPositiveResponse;
}

std::size_t DidInfo::GetMaxSize() const {
  return std::accumulate(
      data_element_info_list_.begin(), data_element_info_list_.end(), 0,
      [](std::size_t size, DataElementInfoList::const_reference data_elem) { return size + data_elem.GetMaxSize(); });
}

std::size_t DidInfo::GetMinSize() const {
  return std::accumulate(
      data_element_info_list_.begin(), data_element_info_list_.end(), 0,
      [](std::size_t size, DataElementInfoList::const_reference data_elem) { return size + data_elem.GetMinSize(); });
}

const vac::memory::optional<DidOperation>& DidInfo::GetDidOperation(DidOperation::Type operation_type) const {
  switch (operation_type) {
    case DidOperation::Type::kRead:
      return read_operation_;
    case DidOperation::Type::kWrite:
      return write_operation_;
    default:
      throw std::runtime_error("DidInfo::GetDidOperation: unknown operation type");
  }
}

}  // namespace data
}  // namespace server
}  // namespace diag
}  // namespace amsr

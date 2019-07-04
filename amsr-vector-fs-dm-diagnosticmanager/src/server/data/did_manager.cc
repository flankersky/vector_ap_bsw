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
/**        \file  did_manager.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <utility>

#include "did_manager.h"

#include "ara/log/logging.hpp"

namespace amsr {
namespace diag {
namespace server {
namespace data {

DidManager::DidManager(const configuration::DidsTableConfiguration& dids_table_conf) {
  // Initialize the did_info_map_
  did_info_map_.reserve(dids_table_conf.dids.size());
  for (const auto& did_config : dids_table_conf.dids) {
    did_info_map_.emplace(std::piecewise_construct, std::forward_as_tuple(did_config.id),
                          std::forward_as_tuple(did_config));
  }
}

vac::memory::optional<operationhandler::ReadDidOperationHandler> DidManager::CreateOpHandlerIfDidReadable(
    const DidInfo::Did did, service::ServiceProcessingContext& processing_context) {
  vac::memory::optional<operationhandler::ReadDidOperationHandler> result;

  const DidInfo* did_info = GetDidInfoIfSupported(did, processing_context, data::DidOperation::Type::kRead);
  if (did_info != nullptr) {
    ara::log::LogDebug() << "DidManager::" << __func__ << ": Did is readable.";
    result.emplace(*did_info);
  }
  return result;
}

vac::memory::optional<operationhandler::WriteDidOperationHandler> DidManager::CreateOpHandlerIfDidWriteable(
    const DidInfo::Did did, service::ServiceProcessingContext& processing_context) {
  vac::memory::optional<operationhandler::WriteDidOperationHandler> result;

  const DidInfo* did_info = GetDidInfoIfSupported(did, processing_context, data::DidOperation::Type::kWrite);
  if (did_info != nullptr) {
    ara::log::LogDebug() << "DidManager::" << __func__ << ": Did is writable.";
    result.emplace(*did_info);
  }
  return result;
}

const DidInfo* DidManager::GetDidInfoIfSupported(const DidInfo::Did did,
                                                 service::ServiceProcessingContext& processing_context,
                                                 data::DidOperation::Type operation) {
  // Get the DidInfo associated to the did.
  DidInfoMap::iterator did_info_it = did_info_map_.find(did);
  if (did_info_it == did_info_map_.end()) {
    ara::log::LogDebug() << "DidManager::" << __func__ << ": Did not found.";
    return nullptr;
  }

  // Check if write is supported in active session.
  const DidInfo& did_info = did_info_it->second;
  ara::diag::udstransport::UdsNegativeResponseCode nrc =
      did_info.CheckConditionsFor(conversation::access::AccessCategory::kSession, processing_context, operation);
  if (nrc != ara::diag::udstransport::UdsNegativeResponseCode::kPositiveResponse) {
    return nullptr;
  }

  return &did_info;
}

}  // namespace data
}  // namespace server
}  // namespace diag
}  // namespace amsr

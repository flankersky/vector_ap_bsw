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
/**        \file  rid_manager.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "rid_manager.h"
#include "ara/log/logging.hpp"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {
namespace routine {
namespace ridmanager {

RidManager::RidManager(const configuration::DextConfiguration::Rids& rids_table) {
  // Initialize the rid_info_map_
  rid_info_map_.reserve(rids_table.size());
  for (const configuration::RidConfiguration& rid_config : rids_table) {
    rid_info_map_.emplace(std::piecewise_construct, std::forward_as_tuple(rid_config.id),
                          std::forward_as_tuple(rid_config));
  }
}

vac::memory::optional<Routine> RidManager::CreateRoutineIfAvailable(Rid rid) {
  using optional = vac::memory::optional<Routine>;
  optional result;

  const ridmanager::RoutineInfo* routine_info = GetRidInfoIfSupported(rid);
  if (routine_info) {
    result.emplace(*routine_info);
  }
  return result;
}

const ridmanager::RoutineInfo* RidManager::GetRidInfoIfSupported(Rid rid) {
  // Get info associated to the rid.
  RidInfoMap::iterator routine_info_it = rid_info_map_.find(rid);
  if (routine_info_it == rid_info_map_.end()) {
    ara::log::LogDebug() << "RidManager::" << __func__ << ": RID not found.";
    return nullptr;
  }

  return &routine_info_it->second;
}

}  // namespace ridmanager
}  // namespace routine
}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

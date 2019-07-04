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
/**        \file  rid_manager.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_HANDLER_ROUTINE_RIDMANAGER_RID_MANAGER_H_
#define SRC_SERVER_SERVICE_HANDLER_ROUTINE_RIDMANAGER_RID_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <server/service/handler/routine/routine_types.h>
#include "configuration/dext_configuration.h"
#include "server/service/handler/routine/ridmanager/routine_info.h"
#include "server/service/handler/routine/routine.h"
#include "server/service/service_processing_context.h"
#include "vac/container/static_map.h"
#include "vac/memory/optional.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {
namespace routine {
namespace ridmanager {

/**
 * \brief RID management.
 * If the RID is available and handled by the DM, the method IsRidAvailable returns a Routine for this RID.
 * Otherwise an empty optional<Routine> is returned.
 */
class RidManager {
 public:
  /**
   * \brief Supported RID of example.
   */
  static constexpr Rid kSupportedRid0x3009 = 0x3009;

  /**
   * \brief Constructor.
   */
  explicit RidManager(const configuration::DextConfiguration::Rids& rids_table);

  VIRTUALMOCK ~RidManager() = default;

  RidManager(const RidManager&) = delete;
  RidManager& operator=(const RidManager&) = delete;
  RidManager(RidManager&&) = delete;
  RidManager& operator=(RidManager&&) = delete;

  /**
   * \brief Creates a new Routine object if RID is available
   * \param rid RID
   * \return optional containing new Routine if available, otherwise empty
   */
  VIRTUALMOCK vac::memory::optional<Routine> CreateRoutineIfAvailable(Rid rid);

 private:
  /**
   * \brief Return the RoutineInfo associated to the Rid if available. Otherwise nullptr.
   * \param rid RID
   * \param processing_context service processing context
   * \return pointer to the routine info.
   */
  const ridmanager::RoutineInfo* GetRidInfoIfSupported(Rid rid);

  /**
   * \brief Type definition for a map associating a Rid with its information.
   */
  using RidInfoMap = vac::container::StaticMap<Rid, ridmanager::RoutineInfo>;

  /**
   * \brief Map associating a Rid with its information.
   */
  RidInfoMap rid_info_map_;
};

}  // namespace ridmanager
}  // namespace routine
}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_HANDLER_ROUTINE_RIDMANAGER_RID_MANAGER_H_

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
/**        \file  routine_info.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_HANDLER_ROUTINE_RIDMANAGER_ROUTINE_INFO_H_
#define SRC_SERVER_SERVICE_HANDLER_ROUTINE_RIDMANAGER_ROUTINE_INFO_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "configuration/dext_configuration.h"
#include "diagpreconditions/diag_preconditions.h"
#include "routine_operation.h"
#include "server/service/handler/routine/routine_types.h"
#include "server/service/service_processing_context.h"
#include "udstransport/uds_negative_response_code.h"
#include "vac/memory/optional.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {
namespace routine {
namespace ridmanager {

/**
 * \brief Implementation of RoutineInfo class.
 * This class contains configuration for a specific routine.
 */
class RoutineInfo {
 public:
  /**
   * \brief Type definition for Uds NRC.
   */
  using UdsNegativeResponseCode = ara::diag::udstransport::UdsNegativeResponseCode;

  /**
   * \brief Definition of kInternalStr constant to "Internal".
   */
  static constexpr char kInternalStr[] = "Internal";

  /**
   * \brief Definition of kExternalStr constant to "External".
   */
  static constexpr char kExternalStr[] = "External";

  /**
   * \brief Enumeration for routine handler types.
   */
  enum class HandlerType : std::uint8_t { kInternal = 0x00, kExternal = 0x01 };

  /**
   * \brief constructor.
   */
  explicit RoutineInfo(const configuration::RidConfiguration& rid_conf);

  // Remove copy/move ctor/assignment.
  RoutineInfo(const RoutineInfo&) = delete;
  RoutineInfo& operator=(const RoutineInfo&) = delete;
  RoutineInfo(const RoutineInfo&&) = delete;
  RoutineInfo& operator=(const RoutineInfo&&) = delete;

  /**
   * \brief Return the Rid.
   */
  Rid GetRid() const { return rid_; }

  /**
   * \brief Check if conditions are fulfilled for this routine.
   * \param context current processing context.
   * \return NRC code.
   */
  UdsNegativeResponseCode CheckConditions(const ServiceProcessingContext& context) const;

  /**
   * \brief Check if conditions are fulfilled for requested operation.
   * \param context current processing context.
   * \param operation requested operation.
   * \return NRC code.
   */
  UdsNegativeResponseCode CheckConditions(const ServiceProcessingContext& context,
                                          RoutineOperationType operation) const;

  /**
   * \brief Return the RoutineOperation for selected operation.
   * \param operation requested operation.
   * \return optinal containing the routine operation.
   */
  const vac::memory::optional<RoutineOperation>& GetRoutineOperation(RoutineOperationType operation) const;

  /**
   * \brief Get the handler type.
   * \return the handler type.
   */
  HandlerType GetHandlerType() const { return handler_type_; }

 private:
  /**
   * \brief Convert the handler type give as string to a HandlerType.
   */
  HandlerType GetHandlerTypeFromStr(const amsr::diag::configuration::HandlerInfo& handler_info) const;

  /**
   * \brief Rid.
   */
  Rid rid_;

  /**
   * \brief Handler type.
   */
  HandlerType handler_type_;

  /**
   * \brief Configuration for Start routine operation.
   */
  vac::memory::optional<RoutineOperation> start_operation_;

  /**
   * \brief Configuration for Stop routine operation.
   */
  vac::memory::optional<RoutineOperation> stop_operation_;

  /**
   * \brief Configuration for RequestResults routine operation.
   */
  vac::memory::optional<RoutineOperation> request_results_operation_;

  /**
   * \brief Preconditions for this routine.
   */
  vac::memory::optional<diagpreconditions::DiagPreconditions> preconditions_;
};

}  // namespace ridmanager
}  // namespace routine
}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_HANDLER_ROUTINE_RIDMANAGER_ROUTINE_INFO_H_

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
/**        \file  routine.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_HANDLER_ROUTINE_ROUTINE_H_
#define SRC_SERVER_SERVICE_HANDLER_ROUTINE_ROUTINE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <server/service/handler/routine/routine_types.h>
#include <string>

#include "operationhandler/routine_operation_handler.h"
#include "ridmanager/routine_info.h"
#include "server/service/processor/routinepollingtask/routine_polling_task.h"
#include "server/service/service_processing_context.h"
#include "udstransport/uds_message.h"
#include "udstransport/uds_negative_response_code.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {
namespace routine {
/**
 * \brief Implementation of Routine class.
 * This class manages a specific RID by checking conditions and returning the polling task
 * to communicate with the distant service.
 */
class Routine final {
 public:
  /**
   * \brief Logical not of suppress positive response bit.
   */
  static constexpr std::uint8_t kNotSuppressPosRspMsgIndicationBit = 0x7F;

  /**
   * \brief Size of SI + SF + RID
   */
  static constexpr std::size_t kMinLengthRoutineRequest = 4;

  /**
   *\brief Constructor.
   */
  explicit Routine(const ridmanager::RoutineInfo& routine_conf_info) : routine_conf_info_(routine_conf_info) {}

  /**
   * \brief Check if conditions for this Rid are full filled.
   * \param context service processing context.
   *
   * \return NRC code.
   */
  ara::diag::udstransport::UdsNegativeResponseCode CheckConditions(const ServiceProcessingContext& context) const {
    return routine_conf_info_.CheckConditions(context);
  }

  /**
   * \brief Return an operation handler if the operation is available.
   * \param operation the operation type.
   * \return optional containing the operation handler if available.
   */
  vac::memory::optional<operationhandler::RoutineOperationHandler> CreateOperationHandlerIfAvailable(
      RoutineOperationType operation);

  /**
   * \brief Returs the RID.
   */
  Rid GetRID() const { return routine_conf_info_.GetRid(); }

  /**
   * \brief Return operation value to string in hexadecimal format.
   */
  static std::string ToString(RoutineOperationType operation);

 private:
  /**
   * \brief  Reference to the routine configuration.
   */
  const ridmanager::RoutineInfo& routine_conf_info_;

 private:
  FRIEND_TEST(Routine, IsOperationSupportedStart);
  FRIEND_TEST(Routine, IsOperationSupportedStop);
  FRIEND_TEST(Routine, IsOperationSupportedRequestResults);
  FRIEND_TEST(Routine, IsOperationSupportedDefault);
};
}  // namespace routine
}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_HANDLER_ROUTINE_ROUTINE_H_

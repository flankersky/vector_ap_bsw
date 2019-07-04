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
/**        \file  routine_operation_handler.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_HANDLER_ROUTINE_OPERATIONHANDLER_ROUTINE_OPERATION_HANDLER_H_
#define SRC_SERVER_SERVICE_HANDLER_ROUTINE_OPERATIONHANDLER_ROUTINE_OPERATION_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>

#include "routine_operation_handler_polling_task.h"
#include "server/service/handler/routine/ridmanager/routine_operation.h"
#include "server/service/handler/routine/routine_types.h"
#include "udstransport/uds_negative_response_code.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {
namespace routine {
namespace operationhandler {
/**
 * \brief Implementation of RoutineOperationHandler class handling a specific routine operation.
 */
class RoutineOperationHandler {
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
   * \brief Constructor.
   */
  RoutineOperationHandler(const ridmanager::RoutineOperation& routine_operation, RoutineOperationType operation_type,
                          ridmanager::RoutineInfo::HandlerType handler_type)
      : routine_operation_(routine_operation),
        polling_task_(operation_type, handler_type),
        operation_type_(operation_type),
        handler_type_(handler_type) {}

  // Remove copy ctor/operator.
  RoutineOperationHandler(const RoutineOperationHandler&) = delete;
  RoutineOperationHandler& operator=(const RoutineOperationHandler&) = delete;

  /**
   * \brief Default move constructor.
   */
  RoutineOperationHandler(RoutineOperationHandler&&) = default;

  /**
   * \brief default move operator.
   */
  RoutineOperationHandler& operator=(RoutineOperationHandler&&) = default;

  /**
   * \brief Check if preconditions are fulfilled for this operation.
   * \param context service processing context.
   *
   * \return NRC code.
   */
  ara::diag::udstransport::UdsNegativeResponseCode CheckConditions(const service::ServiceProcessingContext& context) {
    return routine_operation_.CheckConditions(context);
  }

  /**
   * \brief Start operation.
   * \param in_buffer A view of the buffer containing input data.
   * \param out_buffer A view of the buffer for response data.
   * \param processing_context The processing context.
   * \return pointer to the polling task.
   */
  common::DiagnosticServicePollingTask& StartOperation(
      common::DiagnosticServicePollingTask::BufferTypeIn in_buffer,
      common::DiagnosticServicePollingTask::BufferTypeOut out_buffer,
      const server::service::ServiceProcessingContext& processing_context) {
    polling_task_.Init(in_buffer, out_buffer, processing_context);
    polling_task_.Start();
    return polling_task_;
  }

  /**
   * \brief Check if conditions for this Rid operation are full filled.
   * \param context service processing context.
   * \param payload payload of the UDS request.
   *
   * \return NRC code.
   */
  ara::diag::udstransport::UdsNegativeResponseCode CheckConditions(
      const ServiceProcessingContext& context, const ara::diag::udstransport::ByteVector& payload) const;

  /**
   * \brief Return the minimal length of request data  for requested operation.
   * \param operation requested operation.
   * \return minimal length  of request data.
   */
  std::uint8_t GetRequestMinSize(RoutineOperationType operation) const {
    return routine_operation_.GetRequestData().GetMinLength();
  }

  /**
   * \brief Return the maximal length of request data  for requested operation.
   * \param operation requested operation.
   * \return maximal length  of request data.
   */
  std::uint8_t GetRequestMaxSize(RoutineOperationType operation) const {
    return routine_operation_.GetRequestData().GetMaxLength();
  }

  /**
   * \brief Return the minimal length of response data  for requested operation.
   * \param operation requested operation.
   * \return minimal length  of response data.
   */
  std::uint8_t GetResponseMinSize(RoutineOperationType operation) const {
    return routine_operation_.GetResponseData().GetMinLength();
  }

  /**
   * \brief Return the maximal length of response data  for requested operation.
   * \param operation requested operation.
   * \return maximal length  of response data.
   */
  std::uint8_t GetResponseMaxSize(RoutineOperationType operation) const {
    return routine_operation_.GetResponseData().GetMaxLength();
  }

  /**
   * \brief Return operation value to string in hexadecimal format.
   */
  static std::string ToString(RoutineOperationType operation);

 private:
  /**
   * \brief Reference to the operation configuration.
   */
  const ridmanager::RoutineOperation& routine_operation_;

  /**
   * \brief The polling task used by the processor.
   */
  RoutineOperationHandlerPollingTask polling_task_;

  /**
   * \brief current operation type.
   */
  RoutineOperationType operation_type_;

  /**
   * \brief the handler type
   */
  ridmanager::RoutineInfo::HandlerType handler_type_;
};

}  // namespace operationhandler
}  // namespace routine
}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_HANDLER_ROUTINE_OPERATIONHANDLER_ROUTINE_OPERATION_HANDLER_H_

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
/**        \file  write_did_operation_handler.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_DATA_OPERATIONHANDLER_WRITE_DID_OPERATION_HANDLER_H_
#define SRC_SERVER_DATA_OPERATIONHANDLER_WRITE_DID_OPERATION_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/log/logging.hpp"
#include "did_operation_handler.h"
#include "server/service/processor/writedataservice/write_did_polling_task.h"
#include "write_did_operation_handler_polling_task.h"

namespace amsr {
namespace diag {
namespace server {
namespace data {
namespace operationhandler {

/**
 * \brief Write did operation handler.
 */
class WriteDidOperationHandler : public DidOperationHandler {
 public:
  /**
     * \brief Construcor.
     */
  explicit WriteDidOperationHandler(const DidInfo& did_info)
      : DidOperationHandler(did_info), write_polling_task_(did_info.GetDataElementInfoList(), did_info.GetDid()) {}

  // Remove copy constructor/assignment.
  WriteDidOperationHandler(const WriteDidOperationHandler&) = delete;
  WriteDidOperationHandler& operator=(const WriteDidOperationHandler&) = delete;

  /**
   * \brief Move constructor.
   */
  WriteDidOperationHandler(WriteDidOperationHandler&&) = default;

  /**
   * \brief move operator.
   */
  WriteDidOperationHandler& operator=(WriteDidOperationHandler&&) = default;

  /**
   * \brief Check if the conditions for the current request are fulfilled.
   * * ISO 14229-1 2013(E) Figure 21: Evaluate mandatory checks (Active session and security access level).
   * \param processing_context current service processing context.
   * \return NRC
   */
  ara::diag::udstransport::UdsNegativeResponseCode CheckConditions(
      const service::ServiceProcessingContext& processing_context) const override {
    // Check conditions against sessions and security access level.
    // Conditions against active sessions should already have been checked by the processor but are currently rechecked
    // while calling CheckConditions.

    return did_info_.CheckConditions(processing_context, DidOperation::Type::kWrite);
  }

  /**
   * \brief Start writing data
   * \param in_buffer buffer where data to write are stored.
   * \param processing_context current service processing context.
   * \return write did polling task.
   */
  service::processor::WriteDidPollingTask& StartWriting(service::processor::WriteDidPollingTask::BufferTypeIn in_buffer,
                                                        const service::ServiceProcessingContext& processing_context) {
    write_polling_task_.Init(in_buffer, processing_context);
    write_polling_task_.Start();
    return write_polling_task_;
  }

 private:
  /**
   * \brief The actual polling task returned to the processor selecting the delegated read polling task.
   */
  WriteDidOperationHandlerPollingTask write_polling_task_;
};
}  // namespace operationhandler
}  // namespace data
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_DATA_OPERATIONHANDLER_WRITE_DID_OPERATION_HANDLER_H_

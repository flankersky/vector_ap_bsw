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
/**        \file  read_did_operation_handler.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_DATA_OPERATIONHANDLER_READ_DID_OPERATION_HANDLER_H_
#define SRC_SERVER_DATA_OPERATIONHANDLER_READ_DID_OPERATION_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/log/logging.hpp"
#include "did_operation_handler.h"
#include "read_did_operation_handler_polling_task.h"
#include "server/service/processor/readdataservice/read_did_polling_task.h"

namespace amsr {
namespace diag {
namespace server {
namespace data {
namespace operationhandler {

/**
 * \brief Abstract class for operation handlers.
 */
class ReadDidOperationHandler : public DidOperationHandler {
 public:
  /**
   * \brief Construcor.
   */
  explicit ReadDidOperationHandler(const DidInfo& did_info)
      : DidOperationHandler(did_info), read_polling_task_(did_info.GetDataElementInfoList(), did_info.GetDid()) {}

  // Remove copy/move constructor/assignment.
  ReadDidOperationHandler(const ReadDidOperationHandler&) = delete;
  ReadDidOperationHandler& operator=(const ReadDidOperationHandler&) = delete;

  /**
   * \brief Move constructor.
   */
  ReadDidOperationHandler(ReadDidOperationHandler&&) = default;

  /**
   * \brief Move operator.
   */
  ReadDidOperationHandler& operator=(ReadDidOperationHandler&&) = default;

  /**
   * \brief Check if the conditions for the current request are fulfilled.
   * ISO 14229-1 2013(E) Figure 15: Evaluate mandatory checks (Active session and security access level).
   * \param processing_context current service processing context.
   * \return NRC
   */
  ara::diag::udstransport::UdsNegativeResponseCode CheckConditions(
      const service::ServiceProcessingContext& processing_context) const override {
    // Check conditions against sessions and securtity access level.
    // Conditions against active sessions should already have been checked by the processor but are currently rechecked
    // while calling CheckConditions.

    return did_info_.CheckConditions(processing_context, DidOperation::Type::kRead);
  }

  /**
   * \brief Start reading data
   * \param out_buffer buffer where to store the read data
   * \param processing_context current service processing context.
   * \return read did polling task.
   */
  service::processor::ReadDidPollingTask& StartReading(service::processor::ReadDidPollingTask::BufferTypeOut out_buffer,
                                                       const service::ServiceProcessingContext& processing_context) {
    read_polling_task_.Init(out_buffer, processing_context);
    read_polling_task_.Start();
    return read_polling_task_;
  }

 private:
  /**
   * \brief The actual polling task returned to the processor selecting the delegated read polling task.
   */
  ReadDidOperationHandlerPollingTask read_polling_task_;
};
}  // namespace operationhandler
}  // namespace data
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_DATA_OPERATIONHANDLER_READ_DID_OPERATION_HANDLER_H_

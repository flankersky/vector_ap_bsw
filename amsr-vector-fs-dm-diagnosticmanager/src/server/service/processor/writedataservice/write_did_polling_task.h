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
/**        \file  write_did_polling_task.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_PROCESSOR_WRITEDATASERVICE_WRITE_DID_POLLING_TASK_H_
#define SRC_SERVER_SERVICE_PROCESSOR_WRITEDATASERVICE_WRITE_DID_POLLING_TASK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "common/diagnostic_service_polling_task.h"
#include "udstransport/uds_negative_response_code.h"
#include "vac/container/array_view.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace processor {
/**
 * \brief Marker interface for reading DID data from Proxy as polling task.
 */
class WriteDidPollingTask : public virtual common::DiagnosticServicePollingTask {
 public:
  /**
   * \brief Pointer type
   */
  using Ptr = std::unique_ptr<WriteDidPollingTask>;

  virtual ~WriteDidPollingTask() = default;

  /**
   * \brief Simplified initialization method for writing polling tasks.
   * Initializes the tasks with the buffer that stores the DID data to be written.
   * This simplified version takes only the required input buffer, because no output buffer is needed.
   * \param in_buffer The view onto the buffer of the data to be written.
   * \param processing_context service processing context
   */
  virtual void Init(BufferTypeIn in_buffer, const service::ServiceProcessingContext& processing_context) = 0;
};

}  // namespace processor
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_PROCESSOR_WRITEDATASERVICE_WRITE_DID_POLLING_TASK_H_

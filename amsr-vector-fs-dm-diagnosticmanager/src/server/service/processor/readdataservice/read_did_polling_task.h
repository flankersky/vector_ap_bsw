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
/**        \file  read_did_polling_task.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_PROCESSOR_READDATASERVICE_READ_DID_POLLING_TASK_H_
#define SRC_SERVER_SERVICE_PROCESSOR_READDATASERVICE_READ_DID_POLLING_TASK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "common/diagnostic_service_polling_task.h"
#include "server/service/service_processing_context.h"
#include "udstransport/uds_negative_response_code.h"
#include "vac/container/array_view.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace processor {
/**
 * \brief Marker interface for reading DID data as polling tasks.
 */
class ReadDidPollingTask : public virtual common::DiagnosticServicePollingTask {
 public:
  /**
   * \brief Pointer type
   */
  using Ptr = std::unique_ptr<ReadDidPollingTask>;

  virtual ~ReadDidPollingTask() = default;

  /**
   * \brief Simplified initialization method for reading.
   * Initializes the tasks with the buffer the DID data is written to while reading.
   * \param out_buffer The view onto the buffer used to store the read data.
   * \param processing_context service processing context
   */
  virtual void Init(BufferTypeOut out_buffer, const service::ServiceProcessingContext& processing_context) = 0;
};

}  // namespace processor
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_PROCESSOR_READDATASERVICE_READ_DID_POLLING_TASK_H_

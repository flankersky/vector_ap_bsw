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
/**        \file  routine_polling_task.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_PROCESSOR_ROUTINEPOLLINGTASK_ROUTINE_POLLING_TASK_H_
#define SRC_SERVER_SERVICE_PROCESSOR_ROUTINEPOLLINGTASK_ROUTINE_POLLING_TASK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "common/diagnostic_service_polling_task_base.h"
#include "udstransport/uds_negative_response_code.h"
#include "vac/container/array_view.h"
#include "vac/memory/optional.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace processor {
namespace routinepollingtask {
/**
 * \brief Abstract class RoutinePollingTask
 */
class RoutinePollingTask : public amsr::diag::common::DiagnosticServicePollingTask {
 public:
  /**
   * \brief Type definition for pointer to RoutinePollingTask
   */
  using Ptr = std::unique_ptr<RoutinePollingTask>;

  virtual ~RoutinePollingTask() = default;

 protected:
  /**
   * \brief The view onto the buffer that contains input data.
   */
  BufferTypeIn in_buffer_;

  /**
   * \brief The view onto the buffer four output.
   */
  BufferTypeOut out_buffer_;
};
}  // namespace routinepollingtask
}  // namespace processor
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_PROCESSOR_ROUTINEPOLLINGTASK_ROUTINE_POLLING_TASK_H_

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
/**        \file  ipc_polling_task.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_DATA_IPC_IPC_POLLING_TASK_H_
#define SRC_SERVER_DATA_IPC_IPC_POLLING_TASK_H_

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
namespace data {
namespace ipc {

/**
 * \brief Marker interface for IPC polling tasks.
 */
class IpcPollingTask : public virtual amsr::diag::common::DiagnosticServicePollingTask {
 public:
  /**
   * \brief Pointer type
   */
  using Ptr = std::unique_ptr<IpcPollingTask>;

  virtual ~IpcPollingTask() = default;
};

}  // namespace ipc
}  // namespace data
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_DATA_IPC_IPC_POLLING_TASK_H_

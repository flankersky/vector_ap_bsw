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
/**        \file  diagnostic_service_polling_task.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDM_INCLUDE_COMMON_DIAGNOSTIC_SERVICE_POLLING_TASK_BASE_H_
#define LIB_LIBDM_INCLUDE_COMMON_DIAGNOSTIC_SERVICE_POLLING_TASK_BASE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "vac/memory/optional.h"

#include "common/diagnostic_service_polling_task.h"
#include "common/polling_task_base.h"

namespace amsr {
namespace diag {
namespace common {

/**
 * \brief Basic implementation of DiagnosticServicePollingTask.
 * Abstract class that extends PollingTaskBase and provides a basic implementation of the DiagnosticServicePollingTask
 * interface. DiagnosticServicePollingTaskBase provides a basic implementation of the GetNRC method.
 */
class DiagnosticServicePollingTaskBase : public common::PollingTaskBase,
                                         public virtual common::DiagnosticServicePollingTask {
 public:
  /**
   * \brief Returns the NRC in case an error occurred while reading.
   * \return The NRC
   * \throws logic_error if method is called without previously reported failure.
   */
  ara::diag::udstransport::UdsNegativeResponseCode GetNRC() const override {
    if (!nrc_.has_value()) {
      throw std::logic_error("DiagnosticServicePollingTask::GetNRC() called without reported failure.");
    }
    return nrc_.value();
  }

 protected:
  /**
   * \brief Optional for possible Negative Response Code.
   */
  vac::memory::optional<ara::diag::udstransport::UdsNegativeResponseCode> nrc_;
};

} /* namespace common */
} /* namespace diag */
} /* namespace amsr */

#endif  // LIB_LIBDM_INCLUDE_COMMON_DIAGNOSTIC_SERVICE_POLLING_TASK_BASE_H_

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

#ifndef LIB_LIBDM_INCLUDE_COMMON_DIAGNOSTIC_SERVICE_POLLING_TASK_H_
#define LIB_LIBDM_INCLUDE_COMMON_DIAGNOSTIC_SERVICE_POLLING_TASK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vac/container/array_view.h"

#include "common/polling_task.h"
#include "server/service/service_processing_context.h"
#include "udstransport/uds_negative_response_code.h"

namespace amsr {
namespace diag {
namespace common {

/**
 * \brief Interface for tasks that are processed by polling mechanism and provide a NRC (negative return code).
 */
class DiagnosticServicePollingTask : public virtual common::PollingTask {
 public:
  /**
    * \brief Input Buffer type
    */
  using BufferTypeIn = vac::container::const_array_view<std::uint8_t>;
  /**
   * \brief Output Buffer type
   */
  using BufferTypeOut = vac::container::array_view<std::uint8_t>;

  /**
   * \brief Initializes the tasks with the input and output buffer needed for processing.
   * \param in_buffer The view onto the buffer with input data (e. g. for write DID or routine control).
   * \remarks in_buffer is empty for read DID operations
   * \param out_buffer The view onto the buffer used to store the received data (e. g. for read DID or routine control).
   * \remarks out_buffer will be empty for write DID operations
   * \param processing_context service processing context
   */
  virtual void Init(BufferTypeIn in_buffer, BufferTypeOut out_buffer,
                    const server::service::ServiceProcessingContext& processing_context) = 0;

  /**
   * \brief Returns the NRC in case an error occurred while reading.
   * \return The NRC
   * \throws logic_error if method is called without previously reported failure.
   */
  virtual ara::diag::udstransport::UdsNegativeResponseCode GetNRC() const = 0;
};

} /* namespace common */
} /* namespace diag */
} /* namespace amsr */

#endif  // LIB_LIBDM_INCLUDE_COMMON_DIAGNOSTIC_SERVICE_POLLING_TASK_H_

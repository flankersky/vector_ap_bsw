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
/**        \file  datapolling_task.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_DATA_DATA_POLLING_TASK_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_DATA_DATA_POLLING_TASK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <memory>

#include "vac/container/array_view.h"

#include "common/polling_task_base.h"
#include "udstransport/uds_negative_response_code.h"

namespace diag_test_mediator {
namespace data {

/**
 * The timeout in that the cancel condition is checked by ContinueAction.
 */
constexpr std::chrono::nanoseconds kWaitTimeFuture{10};

/**
 * \brief Abstract class for reading DID data as polling task.
 */
class DataPollingTask : public amsr::diag::common::PollingTaskBase {
 public:
  /**
   * \brief Pointer type
   */
  using Ptr = std::unique_ptr<DataPollingTask>;
  /**
   * \brief Buffer type
   */
  using InBufferType = vac::container::const_array_view<std::uint8_t>;
  /**
   * \brief Buffer type
   */
  using OutBufferType = vac::container::array_view<std::uint8_t>;

  virtual ~DataPollingTask() = default;

  /**
   * \brief Initializes the tasks with the buffer the DID data is written to while reading.
   * TODO(vismhe) param meta_info meta info to pass to the application
   * \param in_buffer The view onto the buffer used pass to the mediator.
   * \param out_buffer The view onto the buffer used to store the received data.
   */
  virtual void Init(InBufferType in_buffer, OutBufferType out_buffer) = 0;

  /**
   * \brief Returns the NRC in case an error occurred while reading.
   * \return The NRC
   * \throws logic_error if method is called without previously reported failure.
   */
  virtual ara::diag::udstransport::UdsNegativeResponseCode GetNRC() const = 0;
};

}  // namespace data
}  // namespace diag_test_mediator

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_DATA_DATA_POLLING_TASK_H_

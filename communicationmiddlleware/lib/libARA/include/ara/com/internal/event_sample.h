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
/**        \file  event_sample.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_EVENT_SAMPLE_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_EVENT_SAMPLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/e2e/e2exf/e2e_result.h"

namespace ara {
namespace com {
namespace internal {

/**
 * \brief Representation of an event sample including the event sample data and other information like E2E check
 * results.
 * \tparam SampleType Data type of a single event sample
 */
template <typename SampleType>
class EventSample {
 public:
  /**
   * \brief Constructor of EventSample
   * \param data Single event sample data
   * \param e2e_check_status E2E check status of a single event sample
   */
  EventSample(const SampleType& data, const ara::e2e::state_machine::E2ECheckStatus e2e_check_status)
      : data_{data}, e2e_check_status_{e2e_check_status} {}

  /**
   * \brief Get the sample data
   * \return The actual sample value.
   */
  const SampleType& GetData() { return data_; }

  /**
   * \brief Get E2E check status
   * \return E2E check status of a single event sample
   */
  const ara::e2e::state_machine::E2ECheckStatus& GetE2ECheckStatus() { return e2e_check_status_; }

 private:
  /**
   * \brief Event sample data
   */
  const SampleType data_;

  /**
   * \brief E2E check status
   */
  const ara::e2e::state_machine::E2ECheckStatus e2e_check_status_;
};

}  // namespace internal
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_EVENT_SAMPLE_H_

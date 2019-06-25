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
/**        \file  sample_ptr.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_SAMPLE_PTR_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_SAMPLE_PTR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "ara/e2e/e2e_types.h"

namespace ara {
namespace com {

/**
 * \brief SamplePtr implementation.
 *
 * \tparam SampleType Type of the sample (e.g. uint32, std::vector<T>, struct X)
 * \uptrace SWS_CM_00306
 */
template <typename SampleType>
class SamplePtr {
 public:
  /**
   * \brief Generic constructor for creating the memory and setting the value of the sample.
   *
   * \param sample Sample value
   * \param e2e_check_status E2E check status
   */
  explicit SamplePtr(const SampleType&& sample, ara::e2e::state_machine::E2ECheckStatus e2e_check_status)
      : sample_{std::make_shared<SampleType>(std::move(sample))}, e2e_check_status_{e2e_check_status} {}

  /**
   * \brief E2E Status for this sample.
   *
   * \uptrace SWS_CM_90420
   * \return the E2E check status for this concrete sample.
   */
  ara::e2e::state_machine::CheckStatus GetE2ECheckStatus() const noexcept { return e2e_check_status_; }

  /**
   * \brief Dereference operator * for getting the sample value.
   *
   * \return the actual sample value.
   */
  SampleType& operator*() const noexcept { return *sample_; }

  /**
   * \brief Dereference operator -> for getting the sample value.
   *
   * \return the actual sample value.
   */
  SampleType* operator->() const noexcept { return sample_.operator->(); }

 private:
  /**
   * \brief An object of this type contains the shared ptr to this sample value.
   */
  std::shared_ptr<SampleType> sample_;

  /**
   * \brief E2ECheckStatus of one sample.
   */
  ara::e2e::state_machine::CheckStatus e2e_check_status_;
};

}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_SAMPLE_PTR_H_

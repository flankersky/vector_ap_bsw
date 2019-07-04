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
/**        \file  did_operation.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_DATA_DID_OPERATION_H_
#define SRC_SERVER_DATA_DID_OPERATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "configuration/dext_configuration.h"
#include "diagpreconditions/diag_preconditions.h"
#include "vac/memory/optional.h"

namespace amsr {
namespace diag {
namespace server {
namespace data {
/**
 * \brief Implementation of Class DidOperation.
 */
class DidOperation {
 public:
  /**
   * \brief Operation Type for Read/Write DID
   */
  enum class Type { kRead, kWrite };

  /**
   * \brief Ctor.
   */
  explicit DidOperation(const vac::memory::optional<configuration::Preconditions>& preconditions_config) {
    if (preconditions_config.has_value()) {
      preconditions_.emplace(preconditions_config.value());
    }
  }

  // Remove copy/move ctor/assignment.
  DidOperation(const DidOperation&) = delete;
  DidOperation(const DidOperation&&) = delete;
  DidOperation& operator=(const DidOperation&) = delete;
  DidOperation& operator=(const DidOperation&&) = delete;

  /**
   * \brief Return the preconditions.
   */
  const vac::memory::optional<diagpreconditions::DiagPreconditions>& GetPreconditions() const { return preconditions_; }

 private:
  /**
   * \brief Preconditions associated to the Did operation.
   */
  vac::memory::optional<diagpreconditions::DiagPreconditions> preconditions_;
};

}  // namespace data
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_DATA_DID_OPERATION_H_

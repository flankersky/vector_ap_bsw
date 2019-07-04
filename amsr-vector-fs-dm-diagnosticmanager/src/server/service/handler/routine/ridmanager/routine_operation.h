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
/**        \file  routine_operation.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_HANDLER_ROUTINE_RIDMANAGER_ROUTINE_OPERATION_H_
#define SRC_SERVER_SERVICE_HANDLER_ROUTINE_RIDMANAGER_ROUTINE_OPERATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "configuration/dext_configuration.h"
#include "diagpreconditions/diag_preconditions.h"
#include "server/service/service_processing_context.h"
#include "udstransport/uds_negative_response_code.h"
#include "vac/memory/optional.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {
namespace routine {
namespace ridmanager {

/**
 * \brief Implementation of RoutineOperation class.
 * This class contains information about a specific routine operation.
 */
class RoutineOperation {
 public:
  /**
   * \brief Class containing lengths (min and max) of data used by the operation.
   */
  class RidData final {
   public:
    /**
     * \brief Constructor.
     */
    explicit RidData(configuration::RidOperationConfiguration::RidData rid_data)
        : min_length_(rid_data.min_length), max_length_(rid_data.max_length) {}

    /**
     * \brief Get min length.
     */
    std::uint8_t GetMinLength() const { return min_length_; }

    /**
     * \brief Get max length.
     */
    std::uint8_t GetMaxLength() const { return max_length_; }

   private:
    /**
     * \brief min length.
     */
    std::uint8_t min_length_{0};

    /**
     * \brief max length.
     */
    std::uint8_t max_length_{0};
  };

  /**
   * \brief Constructor.
   */
  explicit RoutineOperation(const configuration::RidOperationConfiguration& operation_conf);

  // Remove copy/move constructor/operator
  RoutineOperation(const RoutineOperation&) = delete;
  RoutineOperation& operator=(const RoutineOperation&) = delete;

  /**
   * \brief Move constructor.
   */
  RoutineOperation(RoutineOperation&&) = default;

  /**
   * \brief Move operator.
   */
  RoutineOperation& operator=(RoutineOperation&&) = default;

  /**
   * \brief Check if conditions are fulfilled for this operation.
   * \param context current processing context.
   * \return NRC code.
   */
  ara::diag::udstransport::UdsNegativeResponseCode CheckConditions(const ServiceProcessingContext& context) const;

  /**
   * \brief Return the response data.
   */
  const RidData& GetResponseData() const;

  /**
   * \brief Return the request data.
   */
  const RidData& GetRequestData() const;

 private:
  /**
   * \brief preconditions for the operation.
   */
  vac::memory::optional<diagpreconditions::DiagPreconditions> preconditions_;

  /**
   * \brief rid data for Start operation.
   */
  vac::memory::optional<RidData> response_data_;

  /**
   * \brief rid data for Stop operation.
   */
  vac::memory::optional<RidData> request_data_;
};

}  // namespace ridmanager
}  // namespace routine
}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr
#endif  // SRC_SERVER_SERVICE_HANDLER_ROUTINE_RIDMANAGER_ROUTINE_OPERATION_H_

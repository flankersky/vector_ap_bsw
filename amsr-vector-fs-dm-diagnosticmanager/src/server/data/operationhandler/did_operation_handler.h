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
/**        \file  did_operation_handler.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_DATA_OPERATIONHANDLER_DID_OPERATION_HANDLER_H_
#define SRC_SERVER_DATA_OPERATIONHANDLER_DID_OPERATION_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "server/data/did_info.h"
#include "server/service/service_processing_context.h"
#include "udstransport/uds_negative_response_code.h"

namespace amsr {
namespace diag {
namespace server {
namespace data {
namespace operationhandler {

/**
 * \brief Abstract class for operation handlers.
 */
class DidOperationHandler {
 public:
  /**
   * \brief Constructor
   * \param did_info reference to the DidInfo.
   */
  explicit DidOperationHandler(const DidInfo& did_info) : did_info_(did_info) {}

  // Remove copy/move constructor/assignment.
  DidOperationHandler(const DidOperationHandler&) = delete;
  DidOperationHandler& operator=(const DidOperationHandler&) = delete;

  /**
   * \brief Move constructor.
   */
  DidOperationHandler(DidOperationHandler&&) = default;

  /**
   * \brief Move operator.
   */
  DidOperationHandler& operator=(DidOperationHandler&&) = default;

  /**
   * \brief Destructor.
   */
  virtual ~DidOperationHandler() = default;

  /**
   * \brief Check if the conditions for the current request are fulfilled.
   * \param processing_context current service processing context.
   * \return NRC
   */
  virtual ara::diag::udstransport::UdsNegativeResponseCode CheckConditions(
      const service::ServiceProcessingContext& processing_context) const = 0;

  /**
   * \brief Return the maximal size of the data elements.
   * \return std:size_t max size of all data elements
   */
  std::size_t GetMaxSize() const { return did_info_.GetMaxSize(); }

  /**
   * \brief Return the minimal size of the data elements.
   * \return std:size_t min size of all data elements
   */
  std::size_t GetMinSize() const { return did_info_.GetMinSize(); }

  /**
   * \brief Return the current did.
   */
  DidInfo::Did GetDid() const { return did_info_.GetDid(); }

 protected:
  /**
   * \brief Reference to DidInfor.
   */
  const DidInfo& did_info_;
};
}  // namespace operationhandler
}  // namespace data
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_DATA_OPERATIONHANDLER_DID_OPERATION_HANDLER_H_

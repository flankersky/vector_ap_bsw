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
/**        \file  uds_response.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_PROCESSOR_CONTEXT_UDS_RESPONSE_H_
#define SRC_SERVER_SERVICE_PROCESSOR_CONTEXT_UDS_RESPONSE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "udstransport/uds_message.h"
#include "udstransport/uds_negative_response_code.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace processor {
namespace context {

/**
 * \brief Context for UDS response handling.
 */
class UdsResponse {
 public:
  virtual ~UdsResponse() = default;

  /**
     * \brief Acquires the buffer for a new empty UDS message.
     * \param payload_size payload size of the message
     * \return new UDS message, nullptr if the buffer cannot be acquired
     */
  virtual ara::diag::udstransport::UdsMessage::Ptr AcquireResponseBuffer(std::size_t payload_size) = 0;

  /**
   * \brief Finish processing of UDS message.
   * \param message Pointer to the UDS message.
   */
  virtual void FinishProcessing(ara::diag::udstransport::UdsMessage::Ptr message) = 0;

  /**
   * \brief Finish processing of UDS message with a NRC.
   * \param nrc Negative response code.
   */
  virtual void FinishProcessing(ara::diag::udstransport::UdsNegativeResponseCode nrc) = 0;

  /**
   * \brief Sets the SPRMIB (Suppress Positive Response Message Indication Bit) state.
   * \param suppress_positive_response true to suppress a positive response; otherwise false (default/initial value)
   */
  virtual void SetSuppressBit(bool suppress_positive_response) = 0;
};

}  // namespace context
}  // namespace processor
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_PROCESSOR_CONTEXT_UDS_RESPONSE_H_

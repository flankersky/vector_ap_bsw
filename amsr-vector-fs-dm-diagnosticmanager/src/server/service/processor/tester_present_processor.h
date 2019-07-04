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
/**
 *      \file  tester_present_processor.h
 *      \brief Contains the implementation of the TesterPresentProcessor
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_PROCESSOR_TESTER_PRESENT_PROCESSOR_H_
#define SRC_SERVER_SERVICE_PROCESSOR_TESTER_PRESENT_PROCESSOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <assert.h>
#include <utility>

#include "server/conversation/uds_message_provider.h"
#include "server/service/processor/service_processor_base.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace processor {

/**
 * \brief Service processor for UDS SID 0x10.
 */
class TesterPresentProcessor : public ServiceProcessorBase {
 public:
  /**
   * TesterPresent Service UDS ServiceID
   */
  static constexpr std::uint8_t kTesterPresentRequestSid = 0x3E;

  /**
   * Positive response message size
   */
  static constexpr std::uint8_t kTesterPresentPositveResponseSid = 0x7E;

  /**
   * Positive response message size
   */
  static constexpr std::uint8_t kSupressPositiveResponseBit = 0x80;

  /**
   * TesterPresent Request UDS ServiceID
   */
  static constexpr std::size_t kTesterPresentRequestMsgSize = 2;

  /**
   * TesterPresent Response message size
   */
  static constexpr std::size_t kTesterPresentResponseMsgSize = 2;

  /**
   * \copydoc ServiceProcessorBase::ServiceProcessorBase()
   * \param deleter_context DeleterContext
   * \remarks uds_message payload must have size 2
   * \remarks first byte of uds_message payload must be 0x3E
   */
  TesterPresentProcessor(ara::diag::udstransport::UdsMessage::Ptr uds_message,
                         ServiceProcessingContext& processing_context,
                         vac::memory::SmartObjectPoolDeleterContext* deleter_context = nullptr);

  TesterPresentProcessor(const TesterPresentProcessor& that) = delete;
  TesterPresentProcessor& operator=(const TesterPresentProcessor& that) = delete;

  /**
   * \brief Handle the uds_message.
   */
  ProcessingStatus HandleMessage() override;

  /**
   * \brief Cancel.
   */
  void Cancel() override {}

  /**
   * \brief On state change.
   */
  void OnStateChange() override {}

  /**
   * \brief Post handling.
   */
  void PostHandling() override {}

 private:
  /**
   * Checks the SPRMIB bit and stores it at the conversation.
   */
  ProcessingStatus HandleSubfunction();

  /**
   * Creates the positive response and provides it to the processing contexts (finishes the transaction)
   */
  void FinishWithPositiveResponse(uint8_t subfunction);

  /**
   * \brief Checks if the provided UDS message has the minimum length.
   * uds_message == null_ptr : returns false
   * payload_size < 2 bytes: returns false
   * \return true when the message is valid, otherwise false
   */
  static bool CheckRequestMinimumLength(const ara::diag::udstransport::UdsMessage::Ptr& uds_message);

  /**
   * \brief Checks if the sub-function is supported
   * \return true when sub-function == 0x00 OR sub-function == 0x80; otherwise false
   */
  static bool IsValidSubfunction(uint8_t subfunction);

  /**
   * \brief Writes the positive response information to the given payload byte vector.
   * \remarks see positive response 0x7E for UDS service ID 0x3E in ISO 14229-1
   * \param payload target payload byte vector
   * \param sub_function subfunction code (2nd byte)
   */
  static void WritePositiveResponseToPayload(ara::diag::udstransport::ByteVector& payload, std::uint8_t sub_function);
};

}  // namespace processor
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_PROCESSOR_TESTER_PRESENT_PROCESSOR_H_

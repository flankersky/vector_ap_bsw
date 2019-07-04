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
/**        \file  read_did_processor.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_PROCESSOR_READ_DID_PROCESSOR_H_
#define SRC_SERVER_SERVICE_PROCESSOR_READ_DID_PROCESSOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "common/multi_byte_type.h"
#include "server/data/did_info.h"
#include "server/data/did_operation.h"
#include "server/data/operationhandler/read_did_operation_handler.h"
#include "server/service/processor/readdataservice/read_did_polling_task.h"
#include "server/service/processor/service_processor_base.h"
#include "vac/container/array_view.h"
#include "vac/container/static_list.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace processor {

/**
 * \brief Service processor for UDS SID 0x22.
 */
class ReadDidProcessor : public processor::ServiceProcessorBase {
 public:
  /**
   * Read DID UDS ServiceID
   */
  static constexpr std::uint8_t kReadDidSid = 0x22;

  /**
   * Positive response ServiceID for ReadDidService
   */
  static constexpr std::uint8_t kReadDidPositiveResponseSid = 0x62;

  /**
   * DiagnosticSessionControl Service UDS ServiceID
   */
  static constexpr std::size_t kDidSizeInBytes = 2;

  /**
   * DidOperation::Type is always kReadDidSid.
   */
  static constexpr data::DidOperation::Type kOperationType = data::DidOperation::Type::kRead;

  /**
   * Handle message status
   */
  enum class HandleMessageStatus : std::uint8_t { kFirstRun, kContinue, kDone };

  /**
   * \brief Constructor.
   * \param uds_message UDS message, the ServiceProcessor takes ownership of.
   * must be at least 1, first byte of uds_message payload must be 0x3E.
   * \param processing_context reference to the corresponding conversation
   * \param max_number_of_dids the maximum number of DIDs the ReadDidProcessor accepts in a single UDS message.
   * \param deleter_context DeleterContext
   */
  ReadDidProcessor(ara::diag::udstransport::UdsMessage::Ptr uds_message, ServiceProcessingContext& processing_context,
                   const std::size_t max_number_of_dids,
                   vac::memory::SmartObjectPoolDeleterContext* deleter_context = nullptr)
      : ServiceProcessorBase(std::move(uds_message), processing_context, deleter_context),
        max_number_of_dids_(max_number_of_dids),
        max_payload_length_(kDidSizeInBytes * max_number_of_dids),
        read_did_polling_task_(nullptr) {
    assert(uds_message_->GetPayload()[0] == ReadDidProcessor::kReadDidSid);
    available_data_identifiers_.reserve(max_number_of_dids_);
  }

  ProcessingStatus HandleMessage() override;

  void Cancel() override {}

  void OnStateChange() override {}

  void PostHandling() override {}

 private:
  /**
     * \brief Handles the payload of the positive response message.
     */
  class PositiveResponseHandler {
   public:
    PositiveResponseHandler() : curr_pos_(0) {}

    virtual ~PositiveResponseHandler() = default;

    /**
     * Initializes the PositiveResponseManager with the given message payload.
     * \param response_payload response message payload
     */
    void Init(vac::container::array_view<std::uint8_t> response_payload);

    /**
     * Returns a sub-view of the array_view starting from the current position and jumps to the position after this
     * sub-view.
     * \param sub_view_size size of the sub-view
     * \return the requested sub-view
     * \throws out_of_range exception if sub-view would exceed limits of payload.
     */
    vac::container::array_view<std::uint8_t> GetSubArrayViewForWriting(const std::size_t sub_view_size) {
      const std::size_t new_pos = curr_pos_ + sub_view_size;
      if (new_pos > payload_.size()) {
        throw std::out_of_range("Requested sub-view exceeds limits of payload.");
      } else {
        vac::container::array_view<std::uint8_t> sub_view(payload_.begin() + curr_pos_, sub_view_size);
        curr_pos_ += sub_view_size;
        return sub_view;
      }
    }

    /**
     * Writes one Byte to current position of the UDS message response and jumps to the next one.
     * \param value byte to be written
     */
    void WriteOneByteToResponse(std::uint8_t value) {
      assert(!ResponseMessageIsFull());
      payload_.at(curr_pos_) = value;
      ++curr_pos_;
    }

    /**
     * Writes to the next two bytes starting from the current position of the UDS message response and jumps to the next
     * one.
     * \param did DID to be written.
     */
    void WriteDidToResponse(data::DidInfo::Did did) {
      const std::uint8_t high_byte = amsr::diag::common::GetByte(did, 1);
      this->WriteOneByteToResponse(high_byte);
      const std::uint8_t low_byte = amsr::diag::common::GetByte(did, 0);
      this->WriteOneByteToResponse(low_byte);
    }

    /**
     * Checks if response buffer is full.
     * \return true if no space is left in the UDS message, else false.
     */
    bool ResponseMessageIsFull() const { return curr_pos_ >= payload_.size(); }

   private:
    std::size_t curr_pos_;
    vac::container::array_view<uint8_t> payload_;
  };
  ProcessingStatus CheckRequestAndPrepareReading();

  /**
    * \brief Length check on UDS message payload.
    * Checks if length of payload (after SID) is: (1) at least 2, (2) a multiple of 2, (3) does not exceed the
    * maximum
    * payload size.
    * \return true if length ok, false otherwise
    */
  bool IsPayloadLengthOk() const;

  /**
   * Acquires all available DIDs and stores them into valid_data_identifiers_
   * \param payload_view payload view without SID
   */
  void AcquireAvailableDids(vac::container::const_array_view<uint8_t> payload_view);

  /**
   * Checks all available DataIdentifiers.
   * \return kPositiveResponse if check was ok for all available DataIdentifiers, else the corresponding Negative
   * Response Code
   */
  ara::diag::udstransport::UdsNegativeResponseCode CheckConditionsForAvailableDataIdentifiers();

  /**
   * \brief Returns the size of the UDS response message.
   * \return total number of bytes of the UDS message response for available DIds
   */
  std::size_t GetResponseSizeForAvailableDids();

  /**
   * \brief Starts a new PollingTask for the current DID.
   */
  void StartPollingTaskForNextDataIdentifier();

  /**
   * Evaluates the status read_did_polling_task_
   * \param polling_status Polling status that shall be evaluated
   * \return processing status of the processor
   */
  ProcessingStatus EvaluatePollingTaskStatus(const common::PollingStatus polling_status);

  /**
   * Defines the maximum maximum number of DIDs.
   */
  const std::size_t max_number_of_dids_;

  /**
   * Defines the maximum payload length (2*maximum number of DIDs).
   */
  const std::size_t max_payload_length_;

  /**
   * Records the current status of HandleMessage calls.
   */
  HandleMessageStatus handle_message_status_{HandleMessageStatus::kFirstRun};

  /**
   * \brief Type definition for List of ReadDidOperationHandlers.
   */
  using ReadDidOperationHandlerList = vac::container::StaticList<data::operationhandler::ReadDidOperationHandler>;
  /**
   * Buffer for available DataIdentifiers.
   */
  // TODO(PAASR-2983): ReadDidProcessor shall not use dynamic memory.
  ReadDidOperationHandlerList available_data_identifiers_;

  PositiveResponseHandler response_handler_;

  /**
   * Response message
   */
  ara::diag::udstransport::UdsMessage::Ptr response_message_;

  ReadDidPollingTask* read_did_polling_task_{nullptr};

  FRIEND_TEST(ReadDidServicePositiveResponseHandlerTest, ConstructorInitializesWithEmptyPayload);
  FRIEND_TEST(ReadDidServicePositiveResponseHandlerTest, WriteOneByteToResponseWritesToReferencedPayload);
  FRIEND_TEST(ReadDidServicePositiveResponseHandlerTest, WriteTwoDidsToResponse);
  FRIEND_TEST(ReadDidServicePositiveResponseHandlerTest,
              GetSubArrayViewForWritingThrowsExceptionIfRequestedSizeIsLargerThanParentSize);
  FRIEND_TEST(ReadDidServicePositiveResponseHandlerTest,
              GetSubArrayViewForWritingThrowsExceptionIfRequestedSizeIsNotAvailableAnymore);
  FRIEND_TEST(ReadDidServicePositiveResponseHandlerTest,
              GetSubArrayViewForWritingProvidesWriteAccessToReferencedPayload);
};

}  // namespace processor
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_PROCESSOR_READ_DID_PROCESSOR_H_

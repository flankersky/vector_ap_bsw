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
/**        \file  write_did_processor.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_PROCESSOR_WRITE_DID_PROCESSOR_H_
#define SRC_SERVER_SERVICE_PROCESSOR_WRITE_DID_PROCESSOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "server/data/did_info.h"
#include "server/data/did_manager.h"
#include "server/data/did_operation.h"
#include "server/service/processor/service_processor_base.h"
#include "server/service/processor/writedataservice/write_did_polling_task.h"
#include "udstransport/uds_message.h"
#include "vac/container/array_view.h"
#include "vac/memory/optional.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace processor {
/**
 * \brief Service processor for UDS SID 0x2E.
 */
class WriteDidProcessor : public processor::ServiceProcessorBase {
 public:
  /**
   * \brief DiagnosticSessionControl Service UDS ServiceID.
   */
  static constexpr std::uint8_t kWriteDidSid = 0x2E;

  /**
  * \brief Size of DID in bytes.
  */
  static constexpr std::size_t kDidSizeInBytes = 2;

  /**
   * \brief minimum size of payload. (SID + DID + DATA)
   */
  static constexpr std::size_t kMinSizePayload = 4;

  /**
   *\brief Total size of DID and SID.
   */
  static constexpr std::size_t kSidDidSizeInBytes = 3;

  /**
   * DidOperation::Type is always kWriteDidSid.
   */
  static constexpr data::DidOperation::Type kOperationType = data::DidOperation::Type::kWrite;

  /**
   * Positive response ServiceID for WriteDidService
   */
  static constexpr std::uint8_t kWriteDidPositiveResponseSid = 0x6E;

  /**
   * Handle message status
   */
  enum class HandleMessageStatus : std::uint8_t { kFirstRun, kContinue, kDone };

  /**
   * \copydoc ServiceProcessorBase::ServiceProcessorBase()
   * \param deleter_context DeleterContext
   * \remark first byte of payload must be 0x2E
   */
  WriteDidProcessor(ara::diag::udstransport::UdsMessage::Ptr uds_message, ServiceProcessingContext& processing_context,
                    vac::memory::SmartObjectPoolDeleterContext* deleter_context = nullptr)
      : ServiceProcessorBase(std::move(uds_message), processing_context, deleter_context),
        uds_message_pos_response_(nullptr),
        handle_message_status_(HandleMessageStatus::kFirstRun),
        write_did_polling_task_(nullptr) {
    assert(uds_message_->GetPayload()[0] == WriteDidProcessor::kWriteDidSid);
  }

  WriteDidProcessor(const WriteDidProcessor& that) = delete;
  WriteDidProcessor& operator=(const WriteDidProcessor& that) = delete;

  ProcessingStatus HandleMessage() override;

  void Cancel() override {}

  void OnStateChange() override {}

  void PostHandling() override {}

 private:
  /**
    * Evaluates the status read_did_polling_task_
    * \param polling_status Polling status that shall be evaluated
    * \return processing status of the processor
    */
  ProcessingStatus EvaluatePollingTaskStatus(const common::PollingStatus polling_status);

  /**
   * \brief Method returning the DID.
   *
   * \param payload_view
   */
  data::DidInfo::Did AcquireAvailableDid();

  /**
   * \brief Get the size of the buffer for the positive response.
   */
  std::size_t GetResponseSizeForAvailableDid();

  /**
   * \brief Prepare the positive response UDS message.
   *
   * \param payload_view
   */
  ProcessingStatus PreparePositiveResponseUDSMessage();

  /**
   * \brief Method called the first time HandleMessage is called.
   */
  ProcessingStatus CheckRequestAndPrepareWriting();

  /**
   * \brief Start the write polling task.
   */
  void StartWritePollingTask();

  /**
   * \brief Return the expected payload size of the incoming UDS message.
   */
  std::size_t GetExpectedRequestPayloadSize();

 private:
  /**
   * \brief Optional containing the data identifier.
   */
  vac::memory::optional<data::operationhandler::WriteDidOperationHandler> did_operation_handler_;

  /**
   * \brief UDS Message containing the positive response.
   */
  ara::diag::udstransport::UdsMessage::Ptr uds_message_pos_response_;

  /**
   * \brief Current status of handle message method.
   */
  HandleMessageStatus handle_message_status_;

  /**
   * \brief pointer to the write did polling task.
   */
  service::processor::WriteDidPollingTask* write_did_polling_task_{nullptr};
};

}  // namespace processor
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_PROCESSOR_WRITE_DID_PROCESSOR_H_

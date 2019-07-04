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
/**        \file  routine_control_processor.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_PROCESSOR_ROUTINE_CONTROL_PROCESSOR_H_
#define SRC_SERVER_SERVICE_PROCESSOR_ROUTINE_CONTROL_PROCESSOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "server/service/handler/routine/operationhandler/routine_operation_handler.h"
#include "server/service/handler/routine/ridmanager/rid_manager.h"
#include "server/service/handler/routine/routine.h"
#include "service_processor_base.h"
#include "vac/memory/optional.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace processor {
/**
 * \brief Implementation of routine control processor.
 */
class RoutineControlProcessor final : public ServiceProcessorBase {
 public:
  /**
   * \brief Size of SI + SF + RID
   */
  static constexpr std::size_t kMinLengthRoutineRequest = 4;

  /**
   * \brief Logical not of suppress positive response bit.
   */
  static constexpr std::uint8_t kNotSuppressPosRspMsgIndicationBit = 0x7F;

  /**
   * \brief Type definition for routine control sid.
   */
  static constexpr std::uint8_t kRoutineControlSid = 0x31;

  /**
   * \brief Type definition for SID of routine control positive response.
   */
  static constexpr std::uint8_t kRoutineControlPositiveResponseSid = 0x71;

  /**
   * Handle message status
   */
  enum class HandleMessageStatus : std::uint8_t { kFirstRun, kContinue, kDone };

  /**
   * \brief Constructor.
   * Sets the DeleterContext to the provided.
   * \param uds_message UDS message, the ServiceProcessor takes ownership of.
   * must be at least 1.
   * \param processing_context reference to the corresponding conversation
   * \param deleter_context DeleterContext
   * \param rid_manager reference to the rid manager.
   */
  RoutineControlProcessor(handler::routine::ridmanager::RidManager& rid_manager,
                          ara::diag::udstransport::UdsMessage::Ptr uds_message,
                          ServiceProcessingContext& processing_context,
                          vac::memory::SmartObjectPoolDeleterContext* deleter_context = nullptr)
      : ServiceProcessorBase::ServiceProcessorBase(std::move(uds_message), processing_context, deleter_context),
        rid_manager_(rid_manager) {}

  RoutineControlProcessor(const RoutineControlProcessor&) = delete;
  RoutineControlProcessor(const RoutineControlProcessor&&) = delete;
  RoutineControlProcessor& operator=(const RoutineControlProcessor&) = delete;
  RoutineControlProcessor& operator=(const RoutineControlProcessor&&) = delete;

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
   * \brief Handle uds message during first call to HandleMessage.
   */
  ProcessingStatus HandleMessageFirstRun();

  /**
   * \brief Acquire the available RID.
   */
  handler::routine::Rid AcquireAvailableRid() const;

  /**
   * \brief Start the polling task.
   */
  ProcessingStatus StartPollingTask();

  /**
   * \brief Prepare the positive response.
   */
  ProcessingStatus PreparePositiveResponseUDSMessage();

  /**
   * \brief Get routine operation.
   */
  handler::routine::RoutineOperationType GetOperation();

  /**
   * \brief Evaluate Task status.
   */
  ProcessingStatus EvaluatePollingTaskStatus(const common::PollingStatus polling_status);

 private:
  /**
   * \brief Reference to the RID manager.
   */
  handler::routine::ridmanager::RidManager& rid_manager_;

  /**
   * \brief Handle message status;
   */
  HandleMessageStatus handle_message_status_ = HandleMessageStatus::kFirstRun;

  /**
   * \brief Type definition for optional of RoutineOperationHandler.
   */
  using OptionalRoutineOperationHandler =
      vac::memory::optional<handler::routine::operationhandler::RoutineOperationHandler>;

  /**
   * \brief Operation Handler return by the routine.
   */
  OptionalRoutineOperationHandler routine_operation_handler_;

  /**
   * \brief uds message in case of postive response.
   */
  ara::diag::udstransport::UdsMessage::Ptr uds_message_pos_response_;

  /**
   * \brief operation.
   */
  vac::memory::optional<handler::routine::RoutineOperationType> operation_;

  /**
   * \brief Pointer to the routine polling task.
   */
  common::DiagnosticServicePollingTask* routine_polling_task_{nullptr};

 private:
  // TODO(PAASR-3012): Reenable disabled tests during restructuration of RidManager
  FRIEND_TEST(RoutineControlProcessorTestFixture, DISABLED_PreparePositiveResponseUDSMessage);
  FRIEND_TEST(RoutineControlProcessorTestFixture, DISABLED_HandleMessageFirstRun);
  FRIEND_TEST(RoutineControlProcessorTestFixture, DISABLED_HandleMessageSecondRun);
  FRIEND_TEST(RoutineControlProcessorTestFixture, DISABLED_EvaluatePollingTaskStatus);
};

}  // namespace processor
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr
#endif  // SRC_SERVER_SERVICE_PROCESSOR_ROUTINE_CONTROL_PROCESSOR_H_

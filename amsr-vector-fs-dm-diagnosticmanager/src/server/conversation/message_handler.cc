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
/**        \file  message_handler.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "server/conversation/message_handler.h"
#include <chrono>

#include "ara/log/logging.hpp"
#include "server/conversation/conversation.h"
#include "server/conversation/conversation_manager.h"
#include "server/diagnostic_server.h"
#include "server/service/processor/service_processor.h"
#include "server/service/service_dispatcher.h"

namespace amsr {
namespace diag {
namespace server {
namespace conversation {

MessageHandler::MessageHandler(Conversation& conversation)
    : conversation_(conversation), uds_message_(nullptr), exit_requested_(false), thread_(&MessageHandler::Run, this) {}

void MessageHandler::HandleMessage(ara::diag::udstransport::UdsMessage::Ptr uds_message) {
  ara::log::LogDebug() << "MessageHandler::" << __func__;
  std::unique_lock<std::mutex> lk(mutex_);
  ara::log::LogDebug() << "MessageHandler::" << __func__ << " : After lock.";
  uds_message_ = std::move(uds_message);
  condition_variable_.notify_one();
}

void MessageHandler::Shutdown() {
  exit_requested_ = true;
  {
    std::lock_guard<std::mutex> lk(mutex_);
    condition_variable_.notify_one();
  }
  if (thread_.joinable()) {
    thread_.join();
  } else {
    ara::log::LogError() << "MessageHandler::Shutdown: Handler thread is not joinable!";
  }
}

void MessageHandler::Run() {
  ara::log::LogDebug() << "MessageHandler::" << __func__;
  // process incoming requests until exit is requested
  while (!exit_requested_) {
    // handle exceptions (e. g. from ara::com) gracefully
    try {
      std::unique_lock<std::mutex> lk(mutex_);
      // wait for the next message
      condition_variable_.wait(lk, [this]() { return (this->exit_requested_ || (this->uds_message_ != nullptr)); });
      if (!exit_requested_) {
        ara::log::LogDebug() << "MessageHandler::" << __func__ << " : inside conditioning on exist_requested.";
        // Get the service dispatcher.
        service::ServiceDispatcher& service_dispatcher =
            conversation_.GetConversationManager().GetDiagnosticServer().GetServiceDispatcher();

        // Get the pair of ServiceProcessor and NRC
        service::ServiceDispatcher::PairProcessorNRC pair_processor_nrc =
            service_dispatcher.FindServiceProcessor(std::move(uds_message_), conversation_);

        // unlock the mutex, because the UDS message (uds_message_) is now owned by the service processor
        lk.unlock();

        // check if the message can be proccessed
        ara::diag::udstransport::UdsNegativeResponseCode nrc_code = pair_processor_nrc.second;
        if (nrc_code != ara::diag::udstransport::UdsNegativeResponseCode::kPositiveResponse) {
          // stop further processing (no matching ServiceProcessor instance found)
          conversation_.FinishProcessing(nrc_code);
        } else {
          assert(pair_processor_nrc.first);
          // process the positive response
          ProcessMessage(std::move(pair_processor_nrc.first));
        }
      }
    } catch (const std::exception& ex) {
      // exception while handling a message (log + call FinishProcessing)
      ara::log::LogError() << "MessageHandler::Run: Exception occurred while handling UDS message: " << ex.what();

      // call finish processing to signal the problem to the tester (kGeneralReject: 0x10  -> ara::com not running)
      // Alternatives could be busyRepeatRequest (0x21), conditionsNotCorrect (0x22) or a code from the manufacturer
      // specific range (e. g. reservedForSpecificConditionsNotCorrect range; see Table A.1 in ISO 14229-1 for details)
      conversation_.FinishProcessing(ara::diag::udstransport::UdsNegativeResponseCode::kGeneralReject);
    }
  }
}

void MessageHandler::ProcessMessage(amsr::diag::server::service::processor::ServiceProcessor::Ptr service_processor) {
  // check if the provided ServiceProcessor is valid
  if (service_processor == nullptr) {
    throw std::invalid_argument("MessageHandler::ProcessMessage: Provided service_processor is nullptr!");
  }

  // Call HandleMessage of processor until kDone Status is received.
  std::chrono::milliseconds sleep_duration(10);
  while (!exit_requested_ && (service_processor->HandleMessage() != service::processor::ProcessingStatus::kDone)) {
    std::this_thread::sleep_for(sleep_duration);
  }
}

MessageHandler::~MessageHandler() {
  // Call thread_.join() if Shutdown has not been called.
  exit_requested_ = true;
  {
    std::lock_guard<std::mutex> lk(mutex_);
    condition_variable_.notify_one();
  }
  if (thread_.joinable()) {
    ara::log::LogError()
        << "MessageHandler::~MessageHandler : Shutdown has not been previously called. Joining thread ...";
    thread_.join();
  }
}

}  // namespace conversation
}  // namespace server
}  // namespace diag
}  // namespace amsr

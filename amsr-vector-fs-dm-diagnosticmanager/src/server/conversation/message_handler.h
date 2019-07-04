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
/**        \file  message_handler.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_CONVERSATION_MESSAGE_HANDLER_H_
#define SRC_SERVER_CONVERSATION_MESSAGE_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>

#include "udstransport/uds_message.h"
#include "udstransport/uds_negative_response_code.h"
#include "vac/memory/smart_base_type_object_pool.h"

namespace amsr {
namespace diag {
namespace server {

namespace service {
namespace processor {

// forward declaration (required to solve circular dependency to Conversion)
class ServiceProcessor;

}  // namespace processor
}  // namespace service

namespace conversation {

class Conversation;  //< forward declaration (required to solve circular dependency to Conversion)

/**
 * \brief Implementation of Message Handler class.
 */
class MessageHandler {
 public:
  /**
   * \brief Constructor.
   */
  explicit MessageHandler(Conversation& conversation);

  /**
   * \brief Destructor.
   */
  virtual ~MessageHandler();

  /**
   * \brief Inform the thread that a message is ready to be processed.
   */
  VIRTUALMOCK void HandleMessage(ara::diag::udstransport::UdsMessage::Ptr uds_message);

  /**
   * \brief Shutdown to be called when application is terminating.
   * Running thread is joined.
   */
  void Shutdown();

 private:
  /**
   * \brief Run method for thread.
   */
  void Run();

  void ProcessMessage(
      vac::memory::SmartBaseTypeObjectPoolUniquePtr<amsr::diag::server::service::processor::ServiceProcessor>
          service_processor);

  /**
   * \brief Processes the message dependent to the provided input.
   * \param service_processor: A unique_ptr with the service processor
   */
  // void HandleMessage(amsr::diag::server::service::processor::ServiceProcessor::Ptr service_processor);

  /**
   * \brief Reference to conversation owning this message handler.
   */
  Conversation& conversation_;

  /**
   * \brief uds message to be processed.
   */
  ara::diag::udstransport::UdsMessage::Ptr uds_message_;

  /**
   * \brief atomic boolean for shutdown
   */
  std::atomic_bool exit_requested_;

  /**
   * \brief mutex for the condition variable
   */
  std::mutex mutex_;

  /**
   * \brief condition variable
   */
  std::condition_variable condition_variable_;

  /**
   * \brief thread to be executed.
   */
  std::thread thread_;
};

}  // namespace conversation
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_CONVERSATION_MESSAGE_HANDLER_H_

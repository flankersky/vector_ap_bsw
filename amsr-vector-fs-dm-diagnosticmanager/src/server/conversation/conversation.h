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
 *      \file  conversation.h
 *      \brief  Contains the definition of the Conversation class
 *
 *      \details  The Conversation implements the "diagnostic protocol" of the DM specification.
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_CONVERSATION_CONVERSATION_H_
#define SRC_SERVER_CONVERSATION_CONVERSATION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <mutex>
#include <string>
#include <utility>

#include "ara/log/logging.hpp"
#include "vac/memory/optional.h"
#include "vac/testing/test_adapter.h"
#include "vac/timer/timer_manager.h"

#include "configuration/generic_configuration.h"
#include "server/conversation/message_handler.h"
#include "server/conversation/state_manager.h"
#include "server/conversation/timers/conversation_timer_p2.h"
#include "server/conversation/timers/conversation_timer_s3.h"
#include "server/service/service_processing_context.h"
#include "udstransport/uds_message.h"
#include "udstransport/uds_negative_response_code.h"
#include "udstransport/uds_transport_protocol_manager.h"
#include "utility/notificationmanagement/observable_value_using_notification_manager.h"

namespace amsr {
namespace diag {
namespace server {
namespace conversation {

// forward declaration (to resolve circular dependencies)
class ConversationManager;

/**
 * \brief Implementation of conversation.
 */
class Conversation : public service::ServiceProcessingContext,
                     public timers::ConversationTimerP2EventHandler,
                     public timers::ConversationTimerS3EventHandler {
 public:
  /**
   * \brief Enumeration of possible Conversation states.
   */
  enum class ConversationState : std::uint8_t {
    kIdle = 0x00,     /**<  Initial state, a message can be handled */
    kProcess = 0x01,  /**<  A message is currently processed */
    kReceive = 0x02,  /**<  The buffer for a message was successfully reserved */
    kTransmit = 0x03, /**<  A message was processed and the response is send */
    kFree = 0x04      /**<  Conversation is free and can be reused, a message can be handled */
  };

  /**
   * \brief type definition of IndicationResult.
   */
  using IndicationResult = ara::diag::udstransport::UdsTransportProtocolMgr::IndicationResult;

  /**
   *  \brief Constructor.
   */
  Conversation(ara::diag::udstransport::UdsTransportProtocolMgr::GlobalChannelIdentifier channel_id,
               ara::diag::udstransport::UdsMessage::Address source_address, vac::timer::TimerManager& timer_manager,
               ConversationManager& conversation_manager, std::uint8_t response_pending_limit);

  virtual ~Conversation() = default;

  Conversation(const Conversation&) = delete;
  Conversation& operator=(const Conversation&) = delete;

  /**
   * \brief Shutdown to be called when application is terminating
   */
  void Shutdown();

  //--- API used by UdsTransportProtocolMgr

  /**
   * \brief Indicate a new UDS Message.
   * The method is called before handling of a received UDS message is started to acquire the memory for the message.
   * Called by the UdsTransportProtocolMgr implementation when the HandleMessage event is received.
   * \remarks When the method is successfully executed the state is changed to ConversationState::kReceive
   * \param size necessary size of buffer.
   * \param target_address_type the target address type.
   * \return std::pair of IndicationResult and UdsMessage unique pointer. When successfully executed an unique_ptr that
   * owns a UDS message of the requested size is returned with Indication result = kIndicationOk; otherwise nullptr is
   * returned and kIndicationOverflow.
   */
  ara::diag::udstransport::UdsTransportProtocolMgr::IndicationPair IndicateMessage(
      const std::size_t size, const ara::diag::udstransport::UdsMessage::TargetAddressType target_address_type);

  /**
   * \brief Handles a UDS message according to state.
   * Called by the UdsTransportProtocolMgr implementation when the HandleMessage event is received.
   * \remarks When the method is successfully executed the state is changed to ConversationState::kProcess
   * \param message The UDS message.
   */
  VIRTUALMOCK void HandleMessage(ara::diag::udstransport::UdsMessage::Ptr message);

  /**
   *  \brief Transmit confirmation of messages send to the tester.
   *  Called by the UdsTransportProtocolMgr implementation when the TransmitConfirmation event is received.
   * \remarks When the method is successfully executed the state is changed to ConversationState::kIdle.
   *          When the current session is SessionId::kDefault the state is automatically changed
   *          to ConversationState::kFree, so that the Conversation can be reused.
   *  \param result The transmission result
   */
  VIRTUALMOCK void TransmitConfirmation(ara::diag::udstransport::UdsTransportProtocolMgr::TransmissionResult result);

  //--- ServiceProcessingContext implementation (used by MessageHandler)

  /**
   * \brief Finishes the processing of a message with an error code.
   * The method shall be called to signal an error to the tester.
   * \remarks When the method is successfully executed the state is changed to ConversationState::kTransmit
   * \param nrc The negative response code
   */
  void FinishProcessing(ara::diag::udstransport::UdsNegativeResponseCode nrc) override;

  /**
   * \brief Finishes the processing of a message with a positive response (when SPRMIB is not set)
   * The method shall be called to signal the successful processing of an UDS message with a positive
   * response to the tester.
   * When the request was received with active SPRMIB (Suppress Positive Response Message Indication Bit)
   * no positive will be send to the tester.
   * \remarks When the method is successfully executed the state is changed to ConversationState::kTransmit
   * \param message The received message
   */
  void FinishProcessing(ara::diag::udstransport::UdsMessage::Ptr message) override;

  ara::diag::udstransport::UdsMessage::Ptr AcquireResponseBuffer(std::size_t payload_size) override;

  /**
   * \brief Sets the SPRMIB (Suppress Positive Response Message Indication Bit) state.
   * \param suppress_positive_response true to suppress a positive response; otherwise false (default/initial value)
   */
  void SetSuppressBit(bool suppress_positive_response) override;

  //--- public setter and getter functions

  /**
   * \brief Getter for session Id.
   */
  std::uint8_t GetSessionId() const { return session_.Get(); }

  /**
   * \brief Setter for the session Id.
   * Sets the current session state (SessionId) of this Conversation instance.
   * \remarks When the state is changed to the default session the state of the conversation is changed to kFree.
   * \param session the session of the conversation
   */
  void SetSessionId(std::uint8_t session);

  /**
   * \brief Getter for channel ID.
   */
  const ara::diag::udstransport::UdsTransportProtocolMgr::GlobalChannelIdentifier& GetConnectionId() const {
    return channel_id_;
  }

  /**
   * \brief Setter for channel ID.
   */
  void SetConnectionId(const ara::diag::udstransport::UdsTransportProtocolMgr::GlobalChannelIdentifier& channel_id) {
    channel_id_ = channel_id;
  }

  /**
   * \brief Getter for source address.
   */
  ara::diag::udstransport::UdsMessage::Address GetSourceAddress() const { return source_address_; }

  /**
   * \brief Setter for source address.
   */
  void SetSourceAddress(ara::diag::udstransport::UdsMessage::Address source_address) {
    source_address_ = source_address;
  }

  /**
   * \brief Return a reference to the conversation manager.
   */
  VIRTUALMOCK ConversationManager& GetConversationManager();

  /**
   * \brief Return a Reference to the message handler.
   */
  VIRTUALMOCK MessageHandler& GetMessageHandler();

  /**
   *\brief Return a reference to the Did Manager.
   */
  data::DidManager& GetDidManager() const override;

  /**
   *\brief Return a reference to pool of proxys.
   */
  proxys::ProxysPool& GetProxysPool() const override;

  /**
   * \returns the current conversation state.
   */
  ConversationState GetConversationState() const { return current_state_; }

  access::ObservableAccessState& GetAccessState(access::AccessCategory category) override;

  const access::ObservableAccessState& GetAccessState(access::AccessCategory category) const override;

  /**
   * \brief Setter for private member log_max_number_payload_bytes_
   * \param max_payload_bytes maximal number of payload bytes to be written to the output.
   */
  static void SetLogMaxNumberPayloadBytes(std::size_t max_payload_bytes) {
    log_max_number_payload_bytes_ = max_payload_bytes;
  }

 private:
  /**
   * Gets the currently active session identifier.
   */
  std::uint8_t GetActiveSession() const;

  /**
   * \brief Set rx_sid.
   */
  void SetRxSid(std::uint8_t rx_sid);

  /**
   * \brief Get rx_sid.
   */
  std::uint8_t GetRxSid() const;

  /**
   * \brief Transmit UdsMessage to the tester.
   * \param message The to be sent UDS message
   * \param tx_lock lock that is released before the message is given to the transport layer
   */
  VIRTUALMOCK void Transmit(ara::diag::udstransport::UdsMessage::Ptr message, std::unique_lock<std::mutex>& tx_lock);

  /** Initializes the response pending (P2 + P2star) timers and helper variables. */
  void InitializeP2Timers();

  /** \brief Handler of the P2 and P2* event. */
  bool OnP2TimerElapsed(const vac::timer::Timer& owning_timer) override;

  /** Stops the P2 and the P2* timer. */
  void StopP2Timer();

  /** Starts the S3 timer. */
  void StartS3Timer();

  /** \brief Handler of the S3 timer event. */
  bool OnS3TimerElapsed() override;

  /** Stops the S3 timer. */
  void StopS3Timer();

  /**
   * Sends the response pending message (a negative response with code 0x78) to the tester.
   * \param tx_lock lock that is released before the message is given to the transport layer
   */
  void SendResponsePendingMessage(std::unique_lock<std::mutex>& tx_lock);

  /**
   * \brief Send the corresponding negative response code.
   * Builds a negative response message and sends it to the tester by calling the Transmit method
   * \param request_sid The service ID of the request (e. g. 0x10, 0x22, 0x27, ...)
   * \param nrc The negative response code
   * \param tx_lock lock that is released before the message is given to the transport layer
   */
  void SendNegativeResponse(uint8_t request_sid, ara::diag::udstransport::UdsNegativeResponseCode nrc,
                            std::unique_lock<std::mutex>& tx_lock);

  /**
    * Changes the Conversation state and does state change post processing when needed
    * \param state The new Conversation state
    */
  void ChangeState(ConversationState state);

  /**
   * \returns true when the provided message is functional and matches the pattern 0x3E80; otherwise false
   */
  static bool IsFunctionalTesterPresentWithActiveSuppressBit(const ara::diag::udstransport::UdsMessage& message);

  /**
   * \returns true when the provided message is functional and matches the pattern 0x3E80; otherwise false
   */
  static std::string ToString(ConversationState state);

 private:
  using AccessStateType = utility::notificationmanagement::ObservableValueUsingNotificationManager<
      std::uint8_t, access::AccessCategory, service::ServiceProcessingContext, access::AccessCategoryMask>;

  /**
  * \brief p2 timer.
  */
  timers::ConversationTimerP2 p2_;

  /** The P2 star interval */
  std::chrono::milliseconds p2star_time_;

  /**
  * \brief s3 timer.
  */
  timers::ConversationTimerS3 s3_;

  /**
  * \brief Session ID.
  */
  AccessStateType session_;

  /**
  * \brief Security ID.
  */
  AccessStateType security_level_;

  /**
  * \brief The current state of the conversation.
  */
  ConversationState current_state_{ConversationState::kFree};

  /**
  * \brief Global channel ID.
  */
  ara::diag::udstransport::UdsTransportProtocolMgr::GlobalChannelIdentifier channel_id_;

  /**
  * \brief Source address of tester.
  */
  ara::diag::udstransport::UdsMessage::Address source_address_{0};

  /**
   * \brief Reference to conversation manager.
   */
  ConversationManager& conversation_manager_;

  /**
   * \brief Service Identifier.
   * No default value can be set for the rx_sid_.
   * Default value is replaced with empty optional.
   */
  vac::memory::optional<std::uint8_t> rx_sid_;

  /**
   * Specifies if a positive response shall be suppressed
   * Default: false - do NOT suppress the positive response
   */
  bool suppress_positive_response_{false};

  /** The max. supported response pending messages (0x00: off; 0x01 - 0xFE: limit; 0xFF: no limit) */
  const std::uint8_t max_number_of_response_pending_{0};

  /** Flag to track if a response pending is currently processed. */
  bool response_pending_active_{false};

  /** Counter for send response pending messages */
  std::uint32_t response_pending_counter_{0};

  /**
   * Mutex for a critical section required to synchronize the sending of responses.
   * \remarks Currently we do NOT know if the TranmitConfirmation is called on the same thread or on a different one.
   * The std::mutex is not reentrant, so we release the critical section before we call the DoIP layer and acquire it
   * again when we get a TransmitConfirmation.
   */
  std::mutex finish_processing_mutex_;

  /**
   * \brief Message handler.
   */
  MessageHandler message_handler_;

  /**
   * \brief max number of payload bytes to be written to the output
   */
  static std::size_t log_max_number_payload_bytes_;

  FRIEND_TEST(ConversationManagerTestFixture, CheckGetOrCreateConversationRecyclesConversationsInStateFree);
  FRIEND_TEST(ConversationManagerTestFixture, CheckGetOrCreateConversationReturnsNullptrWhenListIsFull);
  FRIEND_TEST(ConversationTestFixture, CheckInitialStateOfConversation);
  FRIEND_TEST(ConversationTestFixture, CheckShutdown);
  FRIEND_TEST(ConversationTestFixture, CheckSetSessionIdChangesSessionStateToFree);
  FRIEND_TEST(ConversationTestFixture, CheckSetAndGetRxSid);
  FRIEND_TEST(ConversationTestFixture, CheckGetRxSidThrowsExceptionWhenSetRxSidWasNotCalled);
  FRIEND_TEST(ConversationTestFixture, CheckSetSuppressBit);
  FRIEND_TEST(ConversationTestFixture, CheckIndicateMessageWithMessageSizeZero);
  FRIEND_TEST(ConversationTestFixture, CheckIndicateMessageWithMessageSizeTwo);
  FRIEND_TEST(ConversationTestFixture, CheckIndicateMessageWithMessageSizeTen);
  FRIEND_TEST(ConversationTestFixture, CheckIndicateMessageWhenConversationStateIsFree);
  FRIEND_TEST(ConversationTestFixture, CheckIndicateMessageWhenConversationStateIsProcess);
  FRIEND_TEST(ConversationTestFixture, CheckIndicateMessageWhenConversationStateIsReceive);
  FRIEND_TEST(ConversationTestFixture, CheckIndicateMessageWhenConversationStateIsTransmit);
  FRIEND_TEST(ConversationTestFixture, CheckHandleMessage);
  FRIEND_TEST(ConversationTestFixture, CheckHandleMessageFailsWhenConversationStateIsNotReceive);
  FRIEND_TEST(ConversationTestFixture, CheckHandleMessageFailsWhenTheProvidedMessageIsNull);
  FRIEND_TEST(ConversationTestFixture, CheckHandleMessageFailsWhenTheProvidedMessageIsEmpty);
  FRIEND_TEST(ConversationTestFixture, CheckHandleMessageWithFuncTpAndActiveSupressBitInProgrammingSession);
  FRIEND_TEST(ConversationTestFixture, CheckHandleMessageWithFuncTpAndActiveSupressBitInDefaultSession);
  FRIEND_TEST(ConversationTestFixture, CheckSendNegativeResponseCreatesValidNegativeResponse);
  FRIEND_TEST(ConversationTestFixture, CheckFinishProcessingNrcFailsConversationStateIsFree);
  FRIEND_TEST(ConversationTestFixture, CheckFinishProcessingNrcFailsConversationStateIsIdle);
  FRIEND_TEST(ConversationTestFixture, CheckFinishProcessingNrcFailsConversationStateIsReceive);
  FRIEND_TEST(ConversationTestFixture, CheckFinishProcessingNrcFailsConversationStateIsTransmit);
  FRIEND_TEST(ConversationTestFixture, CheckFinishProcessingNrcCallsTransmitWithNegativeResponse);
  FRIEND_TEST(ConversationTestFixture, CheckFinishProcessingPositiveResponseCallsTransmit);
  FRIEND_TEST(ConversationTestFixture, CheckFinishProcessingDoesNotCallTransmitWhenPosRespSuppressed);
  FRIEND_TEST(ConversationTestFixture, CheckFinishProcessingFailsWhenConversationStateIsFree);
  FRIEND_TEST(ConversationTestFixture, CheckFinishProcessingFailsWhenConversationStateIsIdle);
  FRIEND_TEST(ConversationTestFixture, CheckFinishProcessingFailsWhenConversationStateIsReceive);
  FRIEND_TEST(ConversationTestFixture, CheckFinishProcessingFailsWhenConversationStateIsTransmit);
  FRIEND_TEST(ConversationTestFixture, CheckTransmitConfirmation);
  FRIEND_TEST(ConversationTestFixture, CheckTransmitConfirmationFailsWhenConversationStateIsNotTransmit);
  FRIEND_TEST(ConversationTestFixture, CheckConversationStateToString);
  FRIEND_TEST(ConversationTestFixture, CheckIsFunctionalTesterPresentWithActiveSuppressBit);
  FRIEND_TEST(ConversationTestFixture, CheckS3TimerWillElapseAndSessionWillChangeToDefault);
  FRIEND_TEST(ConversationTestFixture, CheckStopS3TimerWillCallStopAndSessionWillNotChange);
};

}  // namespace conversation
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_CONVERSATION_CONVERSATION_H_

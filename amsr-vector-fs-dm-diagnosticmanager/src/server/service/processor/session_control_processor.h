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
/**        \file  session_control_processor.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_PROCESSOR_SESSION_CONTROL_PROCESSOR_H_
#define SRC_SERVER_SERVICE_PROCESSOR_SESSION_CONTROL_PROCESSOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "server/conversation/uds_message_provider.h"
#include "server/service/processor/context/session_configuration.h"
#include "server/service/processor/context/uds_response.h"

#include "server/service/processor/service_processor_base.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace processor {

/**
 * \brief Service processor for UDS SID 0x10.
 */
class SessionControlProcessor : public ServiceProcessorBase {
 public:
  /**
   * Configuration DTO
   */
  struct Configuration final {
    /// Context for sending responses to UDS requests
    ServiceProcessingContext& uds_response_context;
    /// Context to query session configuration
    const context::SessionConfiguration& session_config;
    /// Context for accessing session, security level etc of a conversation
    context::ConversationState& conversation_access_state;
  };

  /**
   * DiagnosticSessionControl Service UDS ServiceID
   */
  static constexpr std::uint8_t kDiagnosticSessionControlSid = 0x10;

  /**
   * Positive response UDS ServiceID
   */
  static constexpr std::uint8_t kPositveResponseSid = 0x50;

  /**
   * \brief Constructor for processing a single UDS request
   * \param uds_message UDS message, the ServiceProcessor takes ownership of.
   * \param processor_config processor configuration
   * \param deleter_context DeleterContext
   * \remark first byte of uds_message payload must be 0x10.
   */
  SessionControlProcessor(ara::diag::udstransport::UdsMessage::Ptr uds_message, Configuration processor_config,
                          vac::memory::SmartObjectPoolDeleterContext* deleter_context = nullptr);

  SessionControlProcessor(const SessionControlProcessor& that) = delete;
  SessionControlProcessor& operator=(const SessionControlProcessor& that) = delete;

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
   * \brief Performs length check on UDS message payload (must be 2)
   * \return true if length is ok, otherwise false
   */
  bool CheckUdsMessageLength() const;

  /**
   * Create UDS positive response message for the given sub function.
   * \param subfunction
   * \return positive response message
   */
  ara::diag::udstransport::UdsMessage::Ptr CreatePositiveResponseMessage(std::uint8_t subfunction) const;

  /**
   * Sets the session access state.
   * \param subfunction
   */
  void SetNewSessionInConversation(std::uint8_t subfunction);

  /**
   * \brief Writes the positive response information to the given payload byte vector.
   * see positive response 0x50 for UDS service ID 0x10 in ISO 14229-1:2013(E)
   * \param payload target payload byte vector
   * \param sub_function subfunction code (2nd byte)
   * \param p2 Default  P2 Server_max   timing  supported  by  the server for the activated diagnostic session. (3rd and
   * 4th byte)
   * \param p2_star Enhanced  (NRC  0x78)  P2 Server_max   supported by  the  server  for  the  activated  diagnostic
   * session (5th and 6th byte)
   */
  static void WritePositiveResponseToPayload(ara::diag::udstransport::ByteVector& payload, std::uint8_t sub_function,
                                             std::uint16_t p2, std::uint16_t p2_star);

  /**
   * \brief Converts the given time value to unsigned integer as tens of milliseconds
   */
  static std::uint16_t ConvertToTensOfMilliseconds(std::chrono::milliseconds time);
  /**
   * \brief Converts the given time value to usigned integer.
   */
  static std::uint16_t ConvertToMilliseconds(std::chrono::milliseconds time);

  /// Configuration object holding all the processing Context
  const Configuration config_;
};

}  // namespace processor
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_PROCESSOR_SESSION_CONTROL_PROCESSOR_H_

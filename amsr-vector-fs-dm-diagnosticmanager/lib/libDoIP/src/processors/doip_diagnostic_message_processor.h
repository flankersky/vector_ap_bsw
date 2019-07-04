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
/**        \file  doip_diagnostic_message_processor.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_PROCESSORS_DOIP_DIAGNOSTIC_MESSAGE_PROCESSOR_H_
#define LIB_LIBDOIP_SRC_PROCESSORS_DOIP_DIAGNOSTIC_MESSAGE_PROCESSOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "doip_channel.h"
#include "doip_message.h"
#include "doip_tcp_message_processor.h"
#include "udstransport/uds_message.h"
#include "udstransport/uds_transport_protocol_manager.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace processors {

/**
 * \brief Enumerator for Diagnostic Message response code.
 */
enum class DiagnosticMessageResponseCode : uint8_t {
  kConfirmationAck = 0x00,
  kInvalidSA = 0x02,
  kUnknownTA = 0x03,
  kMessageTooLarge = 0x04,
  kOutOfMemory = 0x05,
  kTargetUnreachable = 0x06,       // This is optional.
  kUnknownnetwork = 0x07,          // This is optional.
  kTransportProtocolError = 0x08,  // This is optional.
  kReserved = 0xff
};

/**
 * \brief Constant for reading the first 4 bytes of a Diagnostic message.
 */
constexpr std::size_t kReadFirst4Bytes = 4;

/**
 * \brief Begin of functional group logical addresses.
 */
constexpr uint16_t kBeginFunctionalAddress = 0xE400;

/**
 * \brief End of functional group logical addresses.
 */
constexpr uint16_t kEndFunctionalAddress = 0xEFFF;

/**
 * \brief Message processor for DoIP Diagnostic message.
 */
class DoIPDiagnosticMessageProcessor : public DoIPTcpMessageProcessor {
 public:
  /**
   * \brief type IndicationPair definition
   */
  using IndicationPair = ara::diag::udstransport::UdsTransportProtocolMgr::IndicationPair;

  /**
   * \brief Constructor.
   */
  explicit DoIPDiagnosticMessageProcessor(PayloadType payload_type) : DoIPTcpMessageProcessor(payload_type) {}

  virtual ~DoIPDiagnosticMessageProcessor() = default;

  bool IsValidPayloadLength(uint32_t length) const override;

  void HandleMessage(const DoIPMessage& msg, DoIPChannel& channel) const override;

  /**
   * \brief Perform the Diagnostic message pre-processing. This is described in Figure 10 of ISO standard 13400.
   *
   * \param msg The partially received message (only the 4 first bytes of the Payload is read).
   * \param channel The channel that the message is received on.
   *
   * \return The action the channel should do upon Diagnostic message Handler processing.
   */
  static ActionUponDiagnosticMessageHandler DiagnosticMessagePreprocessor(const DoIPMessage& msg, DoIPChannel& channel);
  /**
   * \brief Ask the UdsTransportProtocolMgr for a UDS storage to be used for packing the UDS payload inside.
   *
   * \param msg The DoIP message to be processed.
   * \param channel The channel determines the context for the message processing.
   * \param target_address The target address.
   *
   * \return True if the UdsTransportProtocol manager reply with a non-null pointer.
   */
  static bool AskForUdsMessageStorage(const DoIPMessage& msg, DoIPChannel& channel, uint16_t target_address);

  /**
   * \brief Calculate the TA type.
   *
   * \param target_address The target address.
   *
   * \return The target address type.
   */
  static ara::diag::udstransport::UdsMessage::TargetAddressType GetTaType(uint16_t target_address);
};

}  // namespace processors
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_PROCESSORS_DOIP_DIAGNOSTIC_MESSAGE_PROCESSOR_H_

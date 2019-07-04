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
/**        \file  doip_channel.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_DOIP_CHANNEL_H_
#define LIB_LIBDOIP_SRC_DOIP_CHANNEL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <chrono>
#include <mutex>
#include <utility>
#include <vector>

#include "osabstraction/io/event_handler.h"
#include "osabstraction/io/network/socket/tcp_socket.h"
#include "osabstraction/io/reactor.h"
#include "vac/memory/optional.h"
#include "vac/statemachine/state_owner.h"
#include "vac/timer/timer_manager.h"

#include "channelreader/doip_diagnostic_message_reader.h"
#include "channelreader/doip_header_reader.h"
#include "channelreader/doip_message_reader.h"
#include "channelreader/doip_non_diagnostic_message_reader.h"
#include "channelreader/doip_pre_diagnostic_message_reader.h"
#include "doip_channel_reader_context.h"
#include "doip_handler.h"
#include "processors/doip_diagnostic_message_processor.h"
#include "processors/doip_routing_activation_request_processor.h"
#include "tcp/doip_channel_state.h"
#include "tcp/doip_channel_timer.h"
#include "udstransport/uds_message.h"
#include "udstransport/uds_transport_protocol_types.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {

class DoIPTransportProtocolHandler;  // forward declaration

namespace tcp {
class DoIPChannelStatePool;  // forward declaration
class DoIPTCPHandler;        // forward declaration
}

namespace processors {
enum class DiagnosticMessageResponseCode : uint8_t;  // forward declaration
}

/**
 * \brief kMaxNonDiagnosticTCPMessageSize the maximum size of TCP message (excluding the Diagnostic message and its
 * responses) received/sent according to the standard (excluding the 8 bytes of the Generic Header).
 */
constexpr uint8_t kMaxNonDiagnosticTCPMessageSize =
    33;  // 33 (the biggest Message is the Vehicle Announcement Message which belongs to UDP)
         // not 13 (the biggest Message in TCP is the Routing Activation Response excluding the Diagnostic Message)
         // as to consider reading and discarding messages in case of invalid message payload types.

/**
 * \brief kTCPInitialInactivity time within which the DoIPChannel should receive a Routing Activation Request ms.
 */
constexpr std::chrono::milliseconds kTCPInitialInactivity{2000};

/**
 * \brief kTCPInitialInactivity time within which the DoIPChannel should receive a Routing Activation Request ms.
 */
constexpr std::chrono::milliseconds kTCPGeneralInactivity{5000};

/**
 * \brief Initial value for Source Address, which is ISO/SAE reserved.
 */
constexpr uint16_t kSourceAddressInit = 0x0000;

/**
 * \brief reserved value by the ISO 13400.
 */
constexpr uint8_t kReservedValue = 0x00;

/**
 * \brief Size of source and target addresses in the Diagnostic message payload.
 */
constexpr uint32_t kSaTaSize = 4;

/**
 * \brief Size of array for discarding diagnostic message.
 */
constexpr std::size_t kBufferSizeForDiscardingMessages = 1024;

/**
 * \brief Constant for the size of DoIP routing activation response message's payload size.
 */
constexpr std::size_t kRoutingResponseMessagePayloadSize = 9;

/**
 * \brief Constant for the size of DoIP diagnostic message response's payload size.
 */
constexpr std::size_t kDiagResponseMessagePayloadSize = 5;

/**
 * \brief DoIP realization of abstract Connection.
 */
class DoIPChannel : public osabstraction::io::EventHandler,
                    public DoIPChannelReaderContext,
                    public vac::statemachine::StateOwner<tcp::DoIPChannelState> {
 public:
  /**
   * \brief Typedef representing the clock type this timer operates on.
   */
  using Clock = std::chrono::system_clock;
  /**
   * \brief Constructor.
   */
  DoIPChannel(osabstraction::io::ReactorInterface* reactor, vac::timer::TimerManager* timer_manager,
              tcp::DoIPTCPHandler& tcp_handler, tcp::DoIPChannelStatePool& state_pool,
              ara::diag::udstransport::ChannelID channel_id);

  DoIPChannel(const DoIPChannel&) = delete;
  DoIPChannel(const DoIPChannel&&) = delete;
  DoIPChannel& operator=(const DoIPChannel&) = delete;
  DoIPChannel& operator=(const DoIPChannel&&) = delete;

  /**
   * \brief Destructor
   */
  virtual ~DoIPChannel();

  /**
   * \brief Returns the Context used by context depending State methods.
   */
  DoIPChannel& GetContext() { return *this; }

  /**
   * \brief Handle reading from TCP sockets.
   *
   * \param handle A handle to the socket.
   *
   * \return true if everything is OK, false otherwise.
   */
  bool HandleRead(int handle) override;

  /**
   * \brief Register the TCP Socket attached to the DoIPChannel.
   */
  VIRTUALMOCK void RegisterTCPSocket(osabstraction::io::network::socket::TCPSocket& tcp_socket);

  /**
   * \brief Start the Inactivity timer.
   *
   * \param timeout The time interval that is used as time out.
   */
  void StartInactivityTimer(Clock::duration timeout);

  /**
   * \brief Stop the Inactivity timer.
   */
  void StopInactivityTimer();

  /**
   * \brief Getter for the TCPHandler
   */
  tcp::DoIPTCPHandler& GetTcpHandler() override;

  /**
   * \brief Getter for the source address.
   */
  uint16_t GetSourceAddress();

  /**
   * \brief Setter for the source address.
   */
  void SetSourceAddress(uint16_t source_address);

  /**
   * \brief Prepare Routing Activation response.
   *
   * \param source_address The source address comes with the Routing activation request.
   * \param response_code The response code to be sent in the Routing activation response.
   *
   * \return unique pointer to the created response message.
   */
  Ptr CreateRoutingActivationResponse(uint16_t source_address, processors::RoutingActivationResponseCode response_code);

  /**
   * \brief Prepare Diagnostic Message response.
   *
   * \param source_address The source address.
   * \param target_address The target address.
   * \param response_code The response code.
   *
   * \return unique pointer to the created response message.
   */
  Ptr CreateDiagnosticMessageResponse(uint16_t source_address, uint16_t target_address,
                                      processors::DiagnosticMessageResponseCode response_code);
  /**
   * \brief Send Response.
   */
  void SendResponse();

  /**
   * \brief Send Response comes from the active reader using the attached doip_msg.
   *
   * \param response_msg The DoIP response message that the DoIPChannel should use to send the response.
   */
  void SendResponse(DoIPMessage& response_msg) override;

  void SetNextReader(ReaderType reader_type, Ptr doip_msg) override;

  /**
   * \brief Getter for the Channel Id.
   */
  ara::diag::udstransport::ChannelID GetChannelId() { return channel_id_; }

  /**
   * \brief Setter for the UDS message pointer.
   */
  void SetUdsMessage(ara::diag::udstransport::UdsMessage::Ptr uds_message) override;

  /**
   * \brief Release the UDS message pointer.
   *
   * \return The UDS message pointer.
   */
  ara::diag::udstransport::UdsMessage::Ptr ReleaseUdsMessage() override;

  /**
   * \brief Invoke the diagnostic message preprocessor and return its action to the caller.
   *
   * \param msg The DoIP message currently being received.
   *
   * \return action to be taken upon the preprocessing of the diagnostic message.
   */
  ActionUponDiagnosticMessageHandler InvokeDiagnosticMessagePreprocessor(const DoIPMessage& msg) override;

  /**
   * \brief Transmit messages to the tester.
   *
   * \param message A pointer to the UDS message.
   */
  VIRTUALMOCK void Transmit(ara::diag::udstransport::UdsMessage::Ptr message);

  /**
   * \brief Setter for the Channel Id.
   */
  VIRTUALMOCK void SetChannelId(ara::diag::udstransport::ChannelID channel_id) { channel_id_ = channel_id; }

  /**
   * \brief Setter for the Diagnostic message response.
   */
  void SetDiagnosticResponseCode(processors::DiagnosticMessageResponseCode diagnostic_response_code) {
    diagnostic_response_code_ = diagnostic_response_code;
  }

  /**
   * \brief Getter for the Diagnostic message response.
   */
  processors::DiagnosticMessageResponseCode GetDiagnosticResponseCode() { return diagnostic_response_code_; }

  /**
   * \brief Reset the channel.
   */
  void ResetDoIPChannel();

 private:
  /**
   * \brief Read specific number of bytes from the message body from Connection.
   *
   * \param size_to_read The size needed to be read from the message body.
   * \param position_to_write The location in the buffer_ to write on.
   *
   * \return False if the connection is not available; true if read the complete length of bytes or less.
   */
  bool ReadSpecificLengthFromConnection(std::size_t size_to_read, std::size_t position_to_write);

  /**
   * \brief Closes the TCP Socket attached to the DoIPChannel.
   */
  void CloseTCPSocket();

  /**
   * \brief Notifies the Transport protocol Manager of UDS Message reception Failure.
   */
  void NotifyTransportProtocolMgrWithFailure();

  /**
   * \brief Reset the message reader members.
   */
  void ResetReader();

  /**
   * \brief Create NACK Messages.
   *
   * \param nack_code The NACK code to be used.
   */
  void CreateNACK(DoIPNACKCode nack_code);

  /**
   * \brief Initialize the DoIPChannel reader. It would initiatize the active reader and register the array view
   * returned into the buffer_ and initialize the bytes_read_ parameter to be ready for reading from the socket.
   */
  void InitializeReader();

  /**
   * \brief Determines if there are obsolete bytes that need to be flushed from the socket.
   * \return true, if at least one bytes needs to be discarded from the socket, else false.
   */
  bool AllObsoleteBytesAreDiscardedFromSocket() const { return params_from_readers_.number_of_bytes_to_discard == 0; }

  /**
   * \brief Determines if a new reader is set that needs to be initialized
   * This is true under one of the following conditions:
   * (1) It's the first call of the HandleReadMethod
   * (2) The processing of the DoIPMessage was just finished
   * \return true if one of the two conditions is fulfilled, else false
   */
  bool NewReaderSetUponFirstCallOrDoIPMessageWasJustFinished() const;

  /**
   * \brief Determines if a new reader is set that needs to be initialized.
   * This is true under one of the following conditions:
   * (1) The Header reader finished reading and require the following reader to begin working (Prediagnostic or
   * Non-diagnostic reader).
   * (2) The Pre-diagnostic reader is finished and the Diagnostic reader should begin working.
   * \return true if one of the three conditions is fulfilled, else false
   */
  bool NewReaderIsSetWhileReadingTheMessage() const;

  /**
     * \brief Determines if the DoIPChannel has filled completely the buffer from the reader.
     * \return true if buffer is full, else false
     */
  bool BufferFromReaderIsFull() const { return bytes_read_ == buffer_.size(); }

  /**
   * \brief Tries to fill the buffer from the reader with bytes requested from the socket.
   * If all requested bytes from the buffer could be received from the socket the buffer is full afterwards.
   *
   * \return True if it was possible to fill the buffer or part of it, false if can't read at all.
   */
  bool TryToFillBufferRequestedFromReaderWithBytesFromSocket();

  /**
   * \brief Tries to flush obsolete bytes from the socket.
   * If the request could be fully satisfied the the DoIPMessage can processed further, else this methods needs to be
   * called again.
   */
  void TryToDiscardAllObsoleteBytesFromTheSocket();

  /**
   * Finalizes the processing of the current message if the action is either "kCloseTheSocket" or "kFinalize"
   * \param action action to performed
   * \return true if the socket was closed or the finalized DoIPmessage was sent, else false
   */
  bool TryToFinalizeDoIPMessageProcessing(channelreader::ActionForReader action);

  /**
   * Flushes a certain number of bytes (up to the bufferSize) and discarding them.
   * \param bytes_to_flush number of bytes to discard
   * \return returns the number of actually flushed bytes
   */
  std::size_t FlushSocket(std::size_t bytes_to_flush);

  /**
   * \brief Getter for the TCP Socket. By design the TCP Socket should have a value before calling this method. I.e. the
   * RegisterTCPSocket method would be called by the TCP handler.
   * \return the tcp socket.
   */
  VIRTUALMOCK osabstraction::io::network::socket::TCPSocket& GetTCPSocket() {
    assert(tcp_socket_.has_value());
    return tcp_socket_.value();
  }

  /**
   * \brief A reactor for asynchronous event notification on file descriptors.
   */
  osabstraction::io::ReactorInterface* const reactor_;

  /**
   * \brief Timer manager.
   */
  vac::timer::TimerManager* const timer_manager_;

  /**
   * \brief The TCP Handler.
   */
  tcp::DoIPTCPHandler& tcp_handler_;

  /**
   * \brief A Tcp-SocketAcceptor which handles TCP-connection requests.
   */
  vac::memory::optional<osabstraction::io::network::socket::TCPSocket> tcp_socket_;

  /**
   * \brief DoIPMessage to carry the received DoIP Message and responses to be sent.
   */
  DoIPMessage doip_msg_;

  /**
   * \brief flag to indicate if a complete message is available.
   */
  bool is_message_available_ = false;

  /**
   * \brief flag to indicate if a message header is available.
   */
  bool is_header_available_ = false;

  /**
   * \brief number of bytes read from socket till now.
   */
  std::size_t bytes_read_ = 0;

  /**
   * \brief message header data.
   */
  std::array<uint8_t, kGenericHeaderNoOfBytes> header_;

  /**
   * \brief pointer to memory location in which message payload will be saved.
   */
  ara::diag::udstransport::ByteVector buffer_;

  /*
   * \brief the Connection time which is used for both Initial and General Inactivities.
   */
  tcp::DoIPChannelTimer inactivity_timer_;

  /**
   * \brief source address.
   */
  uint16_t source_address_;

  /**
   * \brief Channel ID.
   */
  ara::diag::udstransport::ChannelID channel_id_;

  /**
   * \brief The UDS message pointer.
   */
  ara::diag::udstransport::UdsMessage::Ptr uds_message_;

  /**
   * \brief The diagnostic message response code.
   */
  processors::DiagnosticMessageResponseCode diagnostic_response_code_;

  /**
   * \brief The mutex for synchonization of parallel transmits.
   */
  std::mutex transmit_mutex_;

  /**
   * \brief The DoIP message pointer.
   */
  Ptr doip_msg_ptr_;

  /**
   * \brief Paramters come from Readers. This include: the size to be discarded, if any, and the action to be taken by
   * the DoIP reader.
   */
  channelreader::ContextNeededParams params_from_readers_;

  /**
   * \brief The currently active reader.
   */
  channelreader::DoIPMessageReader* active_reader_{&header_reader_};

  /**
   * \brief The Header reader. The reader responsible for reading the DoIP message Header.
   */
  channelreader::DoIPHeaderReader header_reader_;

  /**
   * \brief The Non-Diagnostic reader. The reader responsible for reading the DoIP message body for all the DoIP message
   * type except the Diagnostic message.
   */
  channelreader::DoIPNonDiagnosticMessageReader non_diag_reader_;

  /**
   * \brief The Pre-Diagnostic reader. The reader responsible for reading the first 4 bytes (containing both source and
   * target addresses) of the Diagnostic message.
   */
  channelreader::DoIPPreDiagnosticMessageReader pre_diag_reader_;

  /**
   * \brief The Diagnostic Message reader. The reader responsible for reading the rest of the Diagnostic message body
   * (except the first 4 bytes of the body).
   */
  channelreader::DoIPDiagnosticMessageReader diag_reader_;

  FRIEND_TEST(DoIPChannelTestFixture, NewReaderSetUponFirstCallOrDoIPMessageWasJustFinishedWillReturnFinishedReading);
  FRIEND_TEST(DoIPChannelTestFixture, SendResponseWillCallSendResponseOfTcpHandler);
  FRIEND_TEST(DoIPChannelTestFixture, SendResponseWillCallSendResponseOfTcpHandlerAndCatchSystemException);
  FRIEND_TEST(DoIPChannelTestFixture, ReadSpecificLengthFromConnectionWillReceiveDataFromSocket);
  FRIEND_TEST(DoIPChannelTestFixture, ReadSpecificLengthFromConnectionWillCatchSystemErrorException);
  FRIEND_TEST(DoIPChannelTestFixture, ReadSpecificLengthFromConnectionWillCatchSocketEOFException);
  FRIEND_TEST(DoIPChannelTestFixture, FlushSocketWillReceiveDataFromSocket);
  FRIEND_TEST(DoIPChannelTestFixture, FlushSocketWillReceiveDataFromSocketEqualsTokBufferSizeForDiscardingMessages);
  FRIEND_TEST(DoIPChannelTestFixture, FlushSocketWillCatchSystemErrorException);
  FRIEND_TEST(DoIPChannelTestFixture, FlushSocketWillCatchSocketEOFException);
  FRIEND_TEST(DoIPChannelTestFixture, HandleReadThrowsRuntime_errorExceptionWhenActiveReaderIsNullptr);
  FRIEND_TEST(DoIPChannelTestFixture, HandleReadReceivesDoIPMsgAndSendNACK);
  FRIEND_TEST(DoIPChannelTestFixture, HandleReadWillResetChannelWhenSocketThrowsException);
  FRIEND_TEST(DoIPChannelTestFixture, HandleReadWillDiscardBytesIfNumberOfBytesToDiscardHasValue);
  FRIEND_TEST(DoIPChannelTestFixture, SetNextReaderWillAssignReaderAccordingToType);
  FRIEND_TEST(DoIPChannelTestFixture, SetUdsMessageWillSetUdsMessagePtr);
  FRIEND_TEST(DoIPChannelTestFixture, ReleaseUdsMessageWillReleaseUdsMessage);
};

}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_DOIP_CHANNEL_H_

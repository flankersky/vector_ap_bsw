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
/**        \file  doip_channel_reader_context.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_DOIP_CHANNEL_READER_CONTEXT_H_
#define LIB_LIBDOIP_SRC_DOIP_CHANNEL_READER_CONTEXT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "doip_message.h"
#include "udstransport/uds_message.h"
#include "udstransport/uds_transport_protocol_types.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {

namespace tcp {
class DoIPTCPHandler;  // forward declaration
}

/**
 * \brief ReaderType the readers types that are supported by the DoIPReader.
 */
enum class ReaderType : uint8_t {
  kHeaderReader,                ///< The Header reader.
  kNonDiagnosticMessageReader,  ///< The non-diag message reader.
  kPreDiagnosticMessageReader,  ///< The Pre-diagnostic message reader.
  kDiagnosticMessageReader      ///< The Diagnostic message reader.
};

/**
 * \brief Enumerator for the action to be done by the channel upon the processing of the Diagnostic message handler.
 */
enum class ActionUponDiagnosticMessageHandler : uint8_t { kProceedWithMessage, kDiscardMessage, kCloseSocket };

/**
 * \brief DoIP Channel reader context. This class will include all prototypes of the needed methods to facilitate the
 * readers' functionality.
 */
class DoIPChannelReaderContext {
 public:
  /**
   * \brief Unique pointer to DoIPMessage.
   */
  using Ptr = std::unique_ptr<DoIPMessage>;

  DoIPChannelReaderContext() = default;
  virtual ~DoIPChannelReaderContext() = default;

  DoIPChannelReaderContext(const DoIPChannelReaderContext&) = delete;
  DoIPChannelReaderContext(const DoIPChannelReaderContext&&) = delete;
  DoIPChannelReaderContext& operator=(const DoIPChannelReaderContext&) = delete;
  DoIPChannelReaderContext& operator=(const DoIPChannelReaderContext&&) = delete;

  /**
   * \brief Send Response. This method is responsible of sending responses to the external tester (handle sending
   * responses through the TCP socket).
   */
  virtual void SendResponse(DoIPMessage& response_msg) = 0;

  /**
   * \brief Set the next active reader. This method is responsible to set the next active reader according to the
   * reader_type. This next active reader will be used to continue reading the DoIP message or be used to read the next
   * message (case of changing the active reader to the HeaderReader).
   */
  virtual void SetNextReader(ReaderType reader_type, Ptr doip_msg) = 0;

  /**
   * \brief Getter for the TCP Handler.
   */
  virtual tcp::DoIPTCPHandler& GetTcpHandler() = 0;

  /**
   * \brief Invoke the diagnostic message preprocessor. This method should invoke the diagnostic message preprocessor
   * static method (DiagnosticMessagePreprocessor, which is responsible for applying the DoIP Diagnostic message Handler
   * found in DoIP standard ISO 13400-2:2012 Figure 10) and returns the action provided by the preprocessor.
   */
  virtual ActionUponDiagnosticMessageHandler InvokeDiagnosticMessagePreprocessor(const DoIPMessage& msg) = 0;

  /**
   * \brief Set the UDS message pointer to a valid Uds message.
   */
  virtual void SetUdsMessage(ara::diag::udstransport::UdsMessage::Ptr uds_message) = 0;

  /**
   * \brief Release the UDS message pointer.
   *
   * \return The UDS message pointer.
   */
  virtual ara::diag::udstransport::UdsMessage::Ptr ReleaseUdsMessage() = 0;
};

}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_DOIP_CHANNEL_READER_CONTEXT_H_

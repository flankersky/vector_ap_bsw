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
/**        \file  doip_diagnostic_message_reader.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_CHANNELREADER_DOIP_DIAGNOSTIC_MESSAGE_READER_H_
#define LIB_LIBDOIP_SRC_CHANNELREADER_DOIP_DIAGNOSTIC_MESSAGE_READER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "doip_message_reader.h"
#include "udstransport/uds_message.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace channelreader {

/**
 * \brief DoIP Diagnostic Message reader.
 */
class DoIPDiagnosticMessageReader : public DoIPMessageReader {
 public:
  /**
   * \brief Constructor. Assign the context to the DoIPChannel which should be the owner of the readers.
   *
   * \param context The context (DoIPChannel) that should be the owner of the reader.
   */
  explicit DoIPDiagnosticMessageReader(DoIPChannelReaderContext& context) : context_(context) {}

  DoIPDiagnosticMessageReader(const DoIPDiagnosticMessageReader&) = delete;
  DoIPDiagnosticMessageReader(const DoIPDiagnosticMessageReader&&) = delete;
  DoIPDiagnosticMessageReader& operator=(const DoIPDiagnosticMessageReader&) = delete;
  DoIPDiagnosticMessageReader& operator=(const DoIPDiagnosticMessageReader&&) = delete;

  /**
   * \brief Initialize: provide the needed parameter to begin reading this part of the message. That parameter is
   * the payload container (Uds Message) to reserve the Payload inside. The reader would ask the Uds Transport Protocol
   * Manager for a Uds Message to fill using IndicateMessage API. The Payload container (Uds message) will be packed in
   * an array view.
   *
   * \param doip_msg Unique pointer to the DoIP message to be used if there are needed parameters (such as the header
   * part).
   *
   * \return Array view on the buffer that is to be used for writing data by the DoIPChannel reader.
   */
  ara::diag::udstransport::ByteVector Initialize(Ptr doip_msg) override;

  /**
   * \brief Evaluate the read part of the message. This method should be called when the DoIP channel reader finished
   * reading the request size of data. This Method will call the Uds Transport Protocol Manager with the API
   * HandlMessage, change the reader to HeaderReader, and will return the action for the Reader Finished reading.
   *
   * \return The action the reader should after evaluation.
   */
  ContextNeededParams Evaluate() override;

  /**
   * \brief Reset the message reader. The main objective is to return back the DoIP message to the DoIP reader.
   */
  void Reset() override;

 private:
  /**
   * \brief The DoIP Channel context.
   */
  DoIPChannelReaderContext& context_;

  /**
   * \brief Unique pointer to the doip message ongoing.
   */
  Ptr doip_msg_;

  /**
   * \brief Unique pointer to the Uds message.
   */
  ara::diag::udstransport::UdsMessage::Ptr uds_message_;

  FRIEND_TEST(DoIPDiagReaderTestFixture, InitializeReturnArrayViewToaValidUdsMessage);
  FRIEND_TEST(DoIPDiagReaderTestFixture, EvaluateSendReaderParamsAndSetUdsmessage);
};

}  // namespace channelreader
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_CHANNELREADER_DOIP_DIAGNOSTIC_MESSAGE_READER_H_

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
/**        \file  doip_header_reader.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_CHANNELREADER_DOIP_HEADER_READER_H_
#define LIB_LIBDOIP_SRC_CHANNELREADER_DOIP_HEADER_READER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>

#include "doip_handler.h"
#include "doip_message_reader.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace channelreader {

/**
 * \brief DoIP Header reader. This reader is responsible of reading and processing the header part of any DoIP message
 * comes through the TCP socket. The reader will provide the DoIPChannel reader with an array view to its internal
 * buffer_ to write the header data on it.
 * When the DoIPChannel reader finishes reading these header bytes, it will call the Evaluate method. The Evaluate
 * method will evaluate the received header bytes. The Evaluate method will send a NACK code to the external tester if
 * needed and send the action required from the DoIPChannel reader. For example, it can ask it to close the socket or
 * may be discard this message. It will then change the active reader to according to the previously mentioned action or
 * according to the payload type, if the action is to continue reading.
 */
class DoIPHeaderReader : public DoIPMessageReader {
 public:
  /**
   * \brief Constructor. Assign the context to the DoIPChannel which should be the owner of the readers.
   */
  explicit DoIPHeaderReader(DoIPChannelReaderContext& context) : context_(context) {}

  DoIPHeaderReader(const DoIPHeaderReader&) = delete;
  DoIPHeaderReader(const DoIPHeaderReader&&) = delete;
  DoIPHeaderReader& operator=(const DoIPHeaderReader&) = delete;
  DoIPHeaderReader& operator=(const DoIPHeaderReader&&) = delete;

  virtual ~DoIPHeaderReader() = default;

  /**
   * \brief Initialize: Provide an array view to the buffer_ to be used in reading the header part of the DoIP Message.
   */
  ara::diag::udstransport::ByteVector Initialize(Ptr doip_msg) override;

  /**
   * \brief Evaluate the Header. The reader will evaluate the received header. If it has problems, it will send a NACK
   * to the tester and return to the DoIP reader what to do. If it is OK, the reader will switch the active reader
   * according to the payload type and return to the reader the action of Continue reading.
   *
   * \return The Parameters that the DoIPChannel reader is in need from the header processing. This includes the that
   * the DoIPChannel reader should take and the size of data to be discarded if any.
   */
  ContextNeededParams Evaluate() override;

  /**
   * \brief Reset the message reader. The main objective is to return back the DoIP message to the DoIP reader.
   */
  void Reset() override;

 private:
  /**
   * \brief Fill the DoIP message with the header data.
   */
  VIRTUALMOCK void FillMessageHeader();

  /**
   * \brief Prepare Nack Message.
   *
   * \param nack_code The Negative acknowledge code.
   * \param response_msg The response message holder to write the NACK on.
   */
  VIRTUALMOCK void PrepareNACK(DoIPNACKCode nack_code, DoIPMessage& response_msg);

  /**
   * \brief The Header buffer.
   */
  std::array<uint8_t, kGenericHeaderNoOfBytes> buffer_;

  /**
   * \brief The DoIP Channel context.
   */
  DoIPChannelReaderContext& context_;

  /**
   * \brief Unique pointer to the doip message ongoing.
   */
  Ptr doip_msg_;

  FRIEND_TEST(DoIPHeaderReaderTestFixture, InitializeReturnValidArrayViewAndUpdateDoIPMessage);
  FRIEND_TEST(DoIPHeaderReaderTestFixture, PrepareNACKWillProvideValidDoIPMessageWithNACKCode);
  FRIEND_TEST(DoIPHeaderReaderTestFixture, FillMessageHeaderWillFillDoIPMessageWithBufferData);
  FRIEND_TEST(DoIPHeaderReaderTestFixture, EvaluateValidHeaderWithNonDiagnosticMessage);
  FRIEND_TEST(DoIPHeaderReaderTestFixture, EvaluateValidHeaderWithDiagnosticMessage);
  FRIEND_TEST(DoIPHeaderReaderTestFixture, EvaluateIncorrectPatternHeader);
  FRIEND_TEST(DoIPHeaderReaderTestFixture, EvaluateInvalidPayloadLengthHeader);
};

}  // namespace channelreader
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_CHANNELREADER_DOIP_HEADER_READER_H_

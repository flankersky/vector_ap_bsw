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
/**        \file  doip_pre_diagnostic_message_reader.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_CHANNELREADER_DOIP_PRE_DIAGNOSTIC_MESSAGE_READER_H_
#define LIB_LIBDOIP_SRC_CHANNELREADER_DOIP_PRE_DIAGNOSTIC_MESSAGE_READER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>

#include "doip_message_reader.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace channelreader {

/**
 * \brief Number of bytes used for storing the Source (SA) and Target (TA) addresses.
 */
constexpr uint32_t kSaTaBytes = 4;

/**
 * \brief DoIP pre-diagnostic reader. This reader prepares data (mainly reading SA & TA data) for the diagnostic reader.
 */
class DoIPPreDiagnosticMessageReader : public DoIPMessageReader {
 public:
  /**
   * \brief Constructor. Assign the context to the DoIPChannel which should be the owner of the readers.
   */
  explicit DoIPPreDiagnosticMessageReader(DoIPChannelReaderContext& context) : context_(context) {}

  DoIPPreDiagnosticMessageReader(const DoIPPreDiagnosticMessageReader&) = delete;
  DoIPPreDiagnosticMessageReader(const DoIPPreDiagnosticMessageReader&&) = delete;
  DoIPPreDiagnosticMessageReader& operator=(const DoIPPreDiagnosticMessageReader&) = delete;
  DoIPPreDiagnosticMessageReader& operator=(const DoIPPreDiagnosticMessageReader&&) = delete;

  /**
   * \brief Initialize: provide the needed parameter to begin reading this part of the message. That parameter is the
   * payload container to reserve the Payload inside. The Payload container will be packed in an array view.
   *
   * \param doip_msg Unique pointer to the DoIP message to be used if there are needed parameters (such as the header
   * part).
   *
   * \return Array view on the buffer that is to be used for writing data by the DoIPChannel reader.
   */
  ara::diag::udstransport::ByteVector Initialize(Ptr doip_msg) override;

  /**
   * \brief Evaluate the read part of the message. This should be called when the reading phase (done by the DoIPReader)
   * is finished.
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
   * \brief The Header buffer.
   */
  std::array<uint8_t, kSaTaBytes> buffer_;

  /**
   * \brief The DoIP Channel context.
   */
  DoIPChannelReaderContext& context_;

  /**
   * \brief Unique pointer to the doip message ongoing.
   */
  Ptr doip_msg_;

  FRIEND_TEST(DoIPPreDiagReaderTestFixture, InitializeReturnValidArrayViewAndUpdateDoIPMessage);
  FRIEND_TEST(DoIPPreDiagReaderTestFixture, EvaluateWithActionToProceedWithDiagMessage);
  FRIEND_TEST(DoIPPreDiagReaderTestFixture, EvaluateWithActionToDiscardMessage);
  FRIEND_TEST(DoIPPreDiagReaderTestFixture, EvaluateWithActionToCloseSocket);
};

}  // namespace channelreader
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_CHANNELREADER_DOIP_PRE_DIAGNOSTIC_MESSAGE_READER_H_

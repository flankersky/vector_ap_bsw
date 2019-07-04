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
/**        \file  doip_message_reader.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_CHANNELREADER_DOIP_MESSAGE_READER_H_
#define LIB_LIBDOIP_SRC_CHANNELREADER_DOIP_MESSAGE_READER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "doip_channel_reader_context.h"
#include "doip_message.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace channelreader {

/**
 * \brief ActionForReader the action that the DoIPReader should take.
 */
enum class ActionForReader : uint8_t {
  kContinueReading,  ///< Continue reading the DoIP message.
  kFinishedReading,  ///< The current message has been read completely.
  kCloseSocket       ///< Close Socket.
};

/**
 * \brief Struct for the return values from readers to the context. The main purpose is to be used in case of discarding
 * data from the socket.
 */
struct ContextNeededParams {
  /**
   * \brief Nothing to discard. Used to indicate that no discarding is needed.
   */
  static constexpr std::size_t kNothingToDiscard = 0;

  /**
   * \brief Size to be discarded. 0 means nothing to be discarded.
   */
  std::size_t number_of_bytes_to_discard;

  /**
   * \brief Action that the DoIPChannel reader should do.
   */
  ActionForReader action;
};

/**
 * \brief This DoIPMessageReader class is the abstract class that all of the message readers will inherit from.
 * Its purpose it to:
 * 1. provide an array view to the Channel Reader to read data from socket and place it in that array view.
 * 2. Evaluate the read part of the message (that it is concerned of), and trigger the needed action.
 * It doesn't handle the complete DoIP message but only part of it. For example, the Header reader will only read the
 * header of the message. The pre-diagnostic message reader will read only the first 4 bytes of the payload of the
 * message. More details will be found in the readers themselves.
 */
class DoIPMessageReader {
 public:
  /**
   * \brief Unique pointer to DoIPMessage.
   */
  using Ptr = std::unique_ptr<DoIPMessage>;

  DoIPMessageReader() = default;
  virtual ~DoIPMessageReader() = default;

  DoIPMessageReader(const DoIPMessageReader&) = delete;
  DoIPMessageReader(const DoIPMessageReader&&) = delete;
  DoIPMessageReader& operator=(const DoIPMessageReader&) = delete;
  DoIPMessageReader& operator=(const DoIPMessageReader&&) = delete;

  /**
   * \brief Initialize: provide the needed parameter to begin reading this part of the message. That parameter is the
   * payload container to reserve the Payload inside. The Payload container will be packed in an array view.
   *
   * \param doip_msg Unique pointer to the DoIP message to be used if there are needed parameters (such as the header
   * part).
   *
   * \return Array view on the buffer that is to be used for writing data by the DoIPChannel reader.
   */
  virtual ara::diag::udstransport::ByteVector Initialize(Ptr doip_msg) = 0;

  /**
   * \brief Evaluate the read part of the message. This should be called when the reading phase (done by the DoIPReader)
   * is finished.
   *
   * \return The return parameters needed for the DoIPChannel reader (context) which should include the number of bytes
   * to be discarded and the action the reader should after evaluation.
   */
  virtual ContextNeededParams Evaluate() = 0;

  /**
   * \brief Reset the message reader. The main objective is to return back the DoIP message to the DoIP reader.
   */
  virtual void Reset() = 0;
};

}  // namespace channelreader
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_CHANNELREADER_DOIP_MESSAGE_READER_H_

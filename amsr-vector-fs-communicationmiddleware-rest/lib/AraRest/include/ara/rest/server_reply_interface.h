/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2018 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  server_reply_interface.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_SERVER_REPLY_INTERFACE_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_SERVER_REPLY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/rest/server.h"
#include "ara/rest/task.h"

namespace ara {
namespace rest {

class ServerRequest;  //< fwd

/**
 * Interface for generic ServerReply implementations
 */
class ServerReplyInterface {
 public:
  /**
   * Default CTOR
   */
  ServerReplyInterface() = default;

  ServerReplyInterface(const ServerReplyInterface&) = delete;             ///< Non-copyable
  ServerReplyInterface& operator=(const ServerReplyInterface&) = delete;  ///< Non-copy-assignable

  /**
   * Virtual Destructor
   */
  virtual ~ServerReplyInterface() = default;

  /**
   * \brief Send a reply to the peer that has issued the request
   *
   *  If this function is not invoked explicitly, the endpoint will transmit a default reply.
   *  If Redirect() has been before called, these functions must be used.
   *
   * \param data payload to be transmitted
   * \param status
   * \return a task waiting for the transmission to complete
   */
  virtual Task<void> Send(const Pointer<ogm::Object>& data = {}, int status = 200) = 0;

  /**
   *  \brief Send a reply to the peer that has issued the request
   *
   *  Same a other Send(), only with move semantics
   *
   * \param data payload to be transmitted
   * \param status
   * \return a task waiting for the transmission to complete
   */
  virtual Task<void> Send(Pointer<ogm::Object>&& data, int status) = 0;

  /**
   * \brief Issues a redirect command to the connected client
   *
   * Must not be called after Send().
   *
   * \param uri location to redirect to
   */
  virtual Task<void> Redirect(const Uri& uri) = 0;
};

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_SERVER_REPLY_INTERFACE_H_

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
/**        \file  server_event_interface.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_SERVER_EVENT_INTERFACE_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_SERVER_EVENT_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/rest/task.h"

namespace ara {
namespace rest {

class ServerEvent;  //< fwd

/**
 * TODO
 */
class ServerEventInterface {
 public:
  /**
   * TODO
   */
  ServerEventInterface() = default;

  ServerEventInterface(const ServerEventInterface&) = delete;             ///< Non-copyable
  ServerEventInterface& operator=(const ServerEventInterface&) = delete;  ///< Non-copy-assignable

  /**
   * Virtual Destructor
   */
  virtual ~ServerEventInterface() = default;

  /** \brief Issues a change notification to its corresponding Server
   *
   *  \return a task waiting for the notification to complete. True denotes a successful notification.
   */
  virtual Task<void> Notify() = 0;

  /** \brief Cancels an event subscription by issuing a cancelation request.
   *
   * A subscription can also be terminated (but not canceled) by destroying the correspond Event
   * object.
   */
  virtual Task<void> Unsubscribe() = 0;

  /**
   * \brief sends data to the client via websocket
   *
   * Sends data via websocket to the coresponding client to notify about an event
   *
   * \param data
   */
  virtual Task<void> Send(String data) = 0;

  /**
   * TODO
   * \return
   */
  virtual Task<void> ConfirmSubscription() = 0;

  /**
   * TODO
   * \return
   */
  virtual Task<void> ConfirmUnsubscription() = 0;

  /**
   * TODO
   * \return
   */
  virtual Task<void> ConfirmResubscription() = 0;

  /**
   * TODO
   * \param server_event_ptr
   */
  virtual void PropagateMovedPointer(ServerEvent* server_event_ptr) = 0;

  /**
   * TODO
   * \param uri
   */
  virtual void PropagateDeletedEvent(String uri) = 0;

  /**
   * \brief propagates that the server is stoped or at least the websocket connection is closed
   */
  virtual void StopPeriodicNotification() = 0;

 private:
};

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_SERVER_EVENT_INTERFACE_H_

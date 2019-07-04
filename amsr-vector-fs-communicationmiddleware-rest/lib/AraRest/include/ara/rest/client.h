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
/**        \file  client.h
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_CLIENT_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_CLIENT_H_

#include <memory>
#include <system_error>

#include "ara/rest/allocator.h"

#include "ara/rest/client_interface.h"
#include "ara/rest/client_types.h"
#include "ara/rest/endpoint.h"
#include "ara/rest/function.h"
#include "ara/rest/header.h"
#include "ara/rest/pointer.h"
#include "ara/rest/task.h"
#include "ara/rest/uri.h"

namespace ara {
namespace rest {
/** \brief Represents a client session with a particular server
 *
 *  A client represents all resources related to communication. By configuration a
 *  client is bound to one particular transport binding. Although the configuration
 *  may bind a client to one particular server by default, a client may connect
 *  to arbitrary servers of the same transport protocol.
 *  A client manages the request/reply cycles to servers but also manages event subscriptions.
 *  How requests and replies are delivered, or the handling of events is depended on the
 *  capabilities and constraints of the transport protocol.
 *
 */
class Client {
 public:
  /** \brief Constructs a client
   *
   *  \param conf_id identifier for a corresponding configuration record
   *  \param alloc allocator for dynamic memory
   */
  explicit Client(const String& conf_id, Allocator* alloc = GetDefaultAllocator());

  Client(const Client&) = delete;             ///< non-copy-constructible
  Client& operator=(const Client&) = delete;  ///< non-copy-assignable

  /** \brief Requests a client shutdown
   *
   * If shutting down gracefully, the client waits for all transactions to finish.
   * If not, then all connections must be terminated instantly.
   *
   * Note that Stop() does not switch Client() into an invalid state. It only
   * terminates all ongoing transmissions. Events are unaffected by this and
   * remain valid until explicitly invalidated by subscription cancellation or
   * destruction.
   *
   *  \param policy shutdown policy
   *  \return a task waiting for shutdown to complete
   */
  Task<void> Stop(ShutdownPolicy policy = ShutdownPolicy::kGraceful);

  // The reply lives as long as the client lives.
  // Whether client maintains an internal buffer or objects
  // are allocated globally and therefore persistent is implementation defined.

  /** \brief Issues a request to a peer
   *
   *  Issues a request to the peer either specified in the client configuration record or the URI of
   *  the request. The configuration record is identified by the id specified in the Client
   *  constructor. If Uri::Authority is set, it overwrites the configuration record.
   *
   *  \param req a request message
   *  \return a task waiting for the corresponding reply
   */
  Task<Pointer<Reply>> Send(const Request& req);

  /** \brief Performs an event subscription
   *
   * An event is uniquely identified by its Uri. A subscription to an event means
   * that if preconditions are met a notification is issued whose message payload
   * is identical to the result set obtained by issuing a GET request on the Uri.
   *
   * Different notification policies
   *
   *  \param uri the event to subscribe to
   *  \param policy the notification policy
   *  \param time time bound as a parameter of the notification policy
   *  \param notify user-defined event notification handler function
   *  \param state user-define subscription state observer function
   *  \return a task waiting for the Event construction and subscription Reply.
   */
  Task<Event> Subscribe(const Uri& uri, EventPolicy policy, duration_t time,
                        const Function<client::NotificationHandlerType>& notify,
                        const Function<client::SubscriptionStateHandlerType>& state = {});

  /** \brief Obtain client status
   *
   * \return status of the client
   */
  std::error_code GetError() const noexcept;

  /** \brief Observe status changes
   *
   *  \param hnd user-defined handler function to called on status changes
   */
  void ObserveError(const Function<void(std::error_code)>& hnd);

 private:
  /**
   * \brief The underlying binding instance.
   */
  Pointer<ClientInterface> client_;
};
}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_CLIENT_H_

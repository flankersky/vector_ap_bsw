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
/**        \file  client_interface.h
 *        \brief  Interface for Client binding implementation
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_CLIENT_INTERFACE_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_CLIENT_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/rest/client_types.h"
#include "ara/rest/endpoint.h"
#include "ara/rest/function.h"
#include "ara/rest/pointer.h"
#include "ara/rest/task.h"
#include "ara/rest/uri.h"

namespace ara {
namespace rest {
class Reply;    //< fwd
class Request;  //< fwd

/**
 * Interface for Client binding implementation
 */
class ClientInterface {
 public:
  /**
   * TODO
   */
  ClientInterface() : error_(), error_observer_() {}

  explicit ClientInterface(const ClientInterface&) = delete;    ///< Client is non-copy-constructible
  ClientInterface& operator=(const ClientInterface&) = delete;  ///< Client is non-copy-assignable

  /**
   * Destructor
   */
  virtual ~ClientInterface() = default;

  /**
   * TODO
   * @param req
   * @return
   */
  virtual Task<Pointer<Reply>> Send(const Request& req) = 0;

  /**
   * TODO
   * @param policy
   * @return
   */
  virtual Task<void> Stop(ShutdownPolicy policy = ShutdownPolicy::kGraceful) = 0;

  /**
   * \brief Performs an event subscription
   *
   *  \param uri the event to subscribe to
   *  \param policy the notification policy
   *  \param time time bound as a parameter of the notification policy
   *  \param notify user-defined event notification handler function
   *  \param state user-define subscription state observer function
   *  \return a task waiting for the Event construction and subscription Reply.
   */
  virtual Task<Event> Subscribe(const Uri& uri, EventPolicy policy, duration_t time,
                                const Function<client::NotificationHandlerType>& notify,
                                const Function<client::SubscriptionStateHandlerType>& state = {}) = 0;

  /** \brief Obtain client status
   *
   * \return status of the client
   */
  std::error_code GetError() const noexcept { return error_; }

  /** \brief Observe status changes
   *
   *  \param hnd user-defined handler function to called on status changes
   */
  void ObserveError(const Function<void(std::error_code)>& hnd) { error_observer_ = hnd; }

 protected:
  /**
   * \brief Client error
   */
  std::error_code error_;

  /**
   * \brief Sets the client status and calls the observer function
   */
  void SetError(std::error_code error) {
    error_ = error;
    if (error_observer_) {
      error_observer_(error_);
    }
  }

  /**
   * \brief Error observer function
   */
  Function<void(std::error_code)> error_observer_;
};

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_CLIENT_INTERFACE_H_

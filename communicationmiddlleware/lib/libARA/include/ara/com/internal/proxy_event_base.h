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
/**        \file  proxy_event_base.h
 *        \brief  TODO
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_PROXY_EVENT_BASE_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_PROXY_EVENT_BASE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <mutex>
#include <type_traits>
#include "ara/com/internal/types.h"

namespace ara {
namespace com {
namespace internal {

/**
 * \brief Base-class of proxy-side event. Handles event receive handlers.
 */
class ProxyEventBase {
 public:
  /**
   * \brief Destructor
   */
  virtual ~ProxyEventBase() {}

  /**
   * \brief Set event receive handler, notified on reception of new event samples.
   * \uptrace SWS_CM_00181
   * \param handler user-defined receive handler
   */
  void SetReceiveHandler(ara::com::EventReceiveHandler handler) {
    std::lock_guard<std::recursive_mutex> guard(receive_handler_lock_);
    receive_handler_ = handler;
  }

  /**
   * \brief Deregister event receive handler
   * \uptrace SWS_CM_00183
   */
  void UnsetReceiveHandler() {
    std::lock_guard<std::recursive_mutex> guard(receive_handler_lock_);
    receive_handler_ = nullptr;
  }

  /**
   * \brief Notify event receive handler on reception of a new event sample. Internal use only.
   * \uptrace SWS_CM_00182
   */
  void Notify() const {
    std::lock_guard<std::recursive_mutex> guard(receive_handler_lock_);
    if (receive_handler_) {
      // Do not use receive_handler_ after call of handler. Due to recursive mutex the handler could replace itself.
      receive_handler_();
    }
  }

  /**
   * \brief Query current subscription state.
   *
   * \return current state of the subscription.
   */
  ara::com::SubscriptionState GetSubscriptionState() const {
    std::lock_guard<std::recursive_mutex> guard(subscription_state_lock_);
    return subscription_state_;
  }

  /**
   * \brief Set a subscription state change handler, which shall get called by the Communication Management
   * implementation as soon as the subscription state of this event has changed.
   * Communication Management implementation will serialize calls to the registered handler. If multiple changes of the
   * subscription state take place during the runtime of a previous call to a handler, the Communication Management
   * aggregates all changes to one call with the last/effective state.
   * Handler may be overwritten during runtime.
   *
   * \param handler user-defined function
   **/
  void SetSubscriptionStateChangeHandler(ara::com::SubscriptionStateChangeHandler* handler) {
    std::lock_guard<std::recursive_mutex> guard(subscription_state_lock_);
    subscription_state_change_handler_ = handler;
  }

  /**
   * \brief Remove handler set by \see SetSubscriptionStateChangeHandler
   **/
  void UnsetSubscriptionStateChangeHandler() {
    std::lock_guard<std::recursive_mutex> guard(subscription_state_lock_);
    subscription_state_change_handler_ = nullptr;
  }

  /**
   * \brief Called upon event subscription state update (internal use only)
   */
  void NotifySubscriptionStateUpdate(const ara::com::SubscriptionState state) {
    bool state_changed_ = (state != subscription_state_);

    std::lock_guard<std::recursive_mutex> guard(subscription_state_lock_);
    subscription_state_ = state;
    if (state_changed_ && subscription_state_change_handler_) {
      // Do not use change handler after call to OnChange(). Due to recursive mutex the handler could replace itself.
      subscription_state_change_handler_->OnChange(state);
    }
  }

 protected:
  /**
   * \brief Constructor
   */
  ProxyEventBase() {}

 private:
  /**
   * Event receive handler
   */
  ara::com::EventReceiveHandler receive_handler_;

  /**
   * Recursive mutex to protect concurrent modification of receive_handler_.
   * A recursive mutex is used to avoid possible dead-locks caused by calls to other APIs by the receive handler.
   * For example if the receive handler de-registers the handler by calling UnsetReceiveHandler().
   *
   * Violation of 'HIC++ rule 18.3.3 Do not use std::recursive_mutex' currently accepted due to ara::com design.
   */
  mutable std::recursive_mutex receive_handler_lock_;

  /**
   * Event subscription state change handler
   */
  ara::com::SubscriptionStateChangeHandler* subscription_state_change_handler_{nullptr};

  /**
   * Current event subscription state
   */
  ara::com::SubscriptionState subscription_state_{ara::com::SubscriptionState::kNotSubscribed};

  /**
   * Recursive mutex to protect concurrent modification of subscription_state_ and subscription_state_change_handler_.
   * A recursive mutex is used to avoid possible dead-locks caused by calls to other APIs by the subscription state
   * change handler. For example if the subscription state change handler de-registers the handler by calling
   * UnsetSubscriptionStateChangeHandler().
   *
   * Violation of 'HIC++ rule 18.3.3 Do not use std::recursive_mutex' currently accepted due to ara::com design.
   */
  mutable std::recursive_mutex subscription_state_lock_;
};

}  // namespace internal
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_PROXY_EVENT_BASE_H_

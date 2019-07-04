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
/**        \file  notification_manager.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_UTILITY_NOTIFICATIONMANAGEMENT_NOTIFICATION_MANAGER_H_
#define SRC_UTILITY_NOTIFICATIONMANAGEMENT_NOTIFICATION_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "value_subscriber.h"

namespace amsr {
namespace diag {
namespace utility {
namespace notificationmanagement {

/**
 * \brief Manages all subscribed instances and notifies them an value change.
 */
template <class ChangeInfo, class Category = typename ChangeInfo::ValueCategory>
class NotificationManager {
 public:
  /** Type of subscribed instances. */
  using Subscriber = ValueSubscriber<ChangeInfo>;

  virtual ~NotificationManager() = default;

  /**
   * \brief Notifies all subscribed instance with the given change information.
   * \param change_info change information
   */
  virtual void NotifyValueChange(ChangeInfo change_info) const = 0;

  /**
   * \brief Subscribes a given instance for the given change category.
   * \param subscriber subscriber instance
   * \param category category for the subscriber.
   */
  virtual void Subscribe(Subscriber& subscriber, Category category) = 0;

  /**
   * \brief Unsubscribes a given instance.
   * \param subscriber subscriber instance
   */
  virtual void Unsubscribe(const Subscriber& subscriber) = 0;
};

}  // namespace notificationmanagement
}  // namespace utility
}  // namespace diag
}  // namespace amsr

#endif  // SRC_UTILITY_NOTIFICATIONMANAGEMENT_NOTIFICATION_MANAGER_H_

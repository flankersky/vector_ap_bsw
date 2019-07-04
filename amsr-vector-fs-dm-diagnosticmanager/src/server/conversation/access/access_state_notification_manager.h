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
/**        \file  access_state_notification_manager.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_CONVERSATION_ACCESS_ACCESS_STATE_NOTIFICATION_MANAGER_H_
#define SRC_SERVER_CONVERSATION_ACCESS_ACCESS_STATE_NOTIFICATION_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "access_state.h"
#include "category_mask.h"
#include "utility/notificationmanagement/notification_manager.h"
#include "utility/notificationmanagement/value_subscriber.h"
#include "vac/container/static_vector.h"
#include "vac/memory/three_phase_allocator.h"

namespace amsr {
namespace diag {
namespace server {
namespace conversation {
namespace access {

/**
 * \brief NotficationManager for AccessStates
 */
class AccessStateNotificationManager
    : public utility::notificationmanagement::NotificationManager<AccessStateChangeInfo, AccessCategoryMask> {
 public:
  /**
   * Initializes the NotificationManager with the maximum number of subscribers.
   * \param maximum_number_of_subscribers
   */
  explicit AccessStateNotificationManager(std::size_t maximum_number_of_subscribers) {
    subscribers_.reserve(maximum_number_of_subscribers);
  }
  virtual ~AccessStateNotificationManager() = default;

  AccessStateNotificationManager(const AccessStateNotificationManager& that) = delete;
  AccessStateNotificationManager& operator=(const AccessStateNotificationManager&) = delete;
  AccessStateNotificationManager(AccessStateNotificationManager&&) = delete;
  AccessStateNotificationManager& operator=(AccessStateNotificationManager&&) = delete;

  /**
   * \brief Notifies all instances subscribed to the corresponding category.
   * \remark If a subscribed instance does not exist anymore, this method will result in an access violation
   * \param change_info change information with category
   */
  void NotifyValueChange(AccessStateChangeInfo change_info) const override;

  /**
   * \brief Subscribes a given instance for the given category mask.
   * \remark If the maximum number of subscribers is exceeded std::bad_alloc will be thrown. If the subscriber is
   * subscribed already the category mask will be overwritten.
   * \param subscriber subscriber instance
   * \param category_mask category mask holding the information about the categories the subscriber needs to be assigned
   * to
   */
  void Subscribe(ObservableAccessStateSubscriber& subscriber, AccessCategoryMask category_mask) override;

  /**
   * \brief Unsubscribes a given instance. If the instance is not found the list of subscribed instances remains
   * unchanged.
   * \param subscriber subscriber instance
   */
  void Unsubscribe(const ObservableAccessStateSubscriber& subscriber) override;

  /**
   * \brief Checks if the number of subscribers has reached the maximum number of subscribers.
   * \return true means not subscribers can be registered
   */
  bool IsFull() const { return subscribers_.size() == subscribers_.capacity(); }

 private:
  /// Subscriber and category mask holding the information about the categories the subscriber is assigned to
  using SubscriberMaskPair = std::pair<ObservableAccessStateSubscriber*, AccessCategoryMask>;
  /// Container for the subscribers and the category mask
  using SubscriberList =
      vac::container::StaticVector<SubscriberMaskPair, vac::memory::ThreePhaseAllocator<SubscriberMaskPair>>;
  /// Iterator for the SubscriberList
  using SubscriberIterator = SubscriberList::iterator;

  /**
   * Searches for a certain subscriber in the subscriber list.
   * \param subscriber
   * \return iterator pointing to the subscriber, else iterator pointer to the element after the final list member
   */
  SubscriberIterator FindSubscriber(const ObservableAccessStateSubscriber& subscriber);

  SubscriberList subscribers_;
};

}  // namespace access
}  // namespace conversation
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_CONVERSATION_ACCESS_ACCESS_STATE_NOTIFICATION_MANAGER_H_

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
/**        \file  access_state_notification_manager.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <assert.h>

#include "server/conversation/access/access_state_notification_manager.h"

namespace amsr {
namespace diag {
namespace server {
namespace conversation {
namespace access {

void AccessStateNotificationManager::NotifyValueChange(AccessStateChangeInfo change_info) const {
  const AccessCategory category = change_info.category;
  for (SubscriberList::const_iterator iter = subscribers_.cbegin(); iter != subscribers_.cend(); ++iter) {
    if (iter->second.IsMasked(category)) {
      iter->first->OnValueChange(change_info);
    }
  }
}

void AccessStateNotificationManager::Subscribe(ObservableAccessStateSubscriber& subscriber,
                                               AccessCategoryMask category_mask) {
  SubscriberList::iterator iter = FindSubscriber(subscriber);
  if (iter != subscribers_.end()) {
    iter->second = category_mask;
  } else {
    subscribers_.push_back(std::make_pair(&subscriber, category_mask));
  }
}

void AccessStateNotificationManager::Unsubscribe(const ObservableAccessStateSubscriber& subscriber) {
  SubscriberList::iterator iter = FindSubscriber(subscriber);
  if (iter != subscribers_.end()) {
    subscribers_.erase(iter);
  }
}

AccessStateNotificationManager::SubscriberIterator AccessStateNotificationManager::FindSubscriber(
    const ObservableAccessStateSubscriber& subscriber) {
  SubscriberList::iterator iter = subscribers_.begin();
  while (iter != subscribers_.end()) {
    if (iter->first == &subscriber) {
      break;
    }
    ++iter;
  }
  return iter;
}

}  // namespace access
}  // namespace conversation
}  // namespace server
}  // namespace diag
}  // namespace amsr

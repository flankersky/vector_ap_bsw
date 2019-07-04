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
/**        \file  observable_value_using_notification_manager.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_UTILITY_NOTIFICATIONMANAGEMENT_OBSERVABLE_VALUE_USING_NOTIFICATION_MANAGER_H_
#define SRC_UTILITY_NOTIFICATIONMANAGEMENT_OBSERVABLE_VALUE_USING_NOTIFICATION_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "notification_manager.h"
#include "observable_value.h"
#include "value_change_info.h"
#include "value_subscriber.h"

namespace amsr {
namespace diag {
namespace utility {
namespace notificationmanagement {

/**
 * \brief Notifies all subscribers on value change using a NotificationManager
 */
template <class Value, class Category, class Context, class CategoryMask = Category>
class ObservableValueUsingNotificationManager : public ObservableValue<Value, Category, Context> {
 public:
  /** value type */
  using ValueType = typename ObservableValue<Value, Category, Context>::ValueType;
  /** category type*/
  using ValueCategory = typename ObservableValue<Value, Category, Context>::ValueCategory;
  /** context type*/
  using ValueContext = typename ObservableValue<Value, Category, Context>::ValueContext;

  /** Value change information type. */
  using ValueChangeInfoType = ValueChangeInfo<ValueType, ValueCategory, ValueContext>;
  /** Notification manager type type. */
  using NotificationManagerType = NotificationManager<ValueChangeInfoType, CategoryMask>;

  /**
   * \brief Constructs a new published values with the given parameters.
   * It notifies all subscribers via the given notification manager with the initial_value_info.
   * \param initial_value_info initial value with category and context
   * \param notification_manager manager holding all the subscribers
   */
  ObservableValueUsingNotificationManager(const ValueChangeInfoType initial_value_info,
                                          NotificationManagerType& notification_manager)
      : value_(initial_value_info.new_value),
        category_(initial_value_info.category),
        context_(initial_value_info.context),
        notification_manager_(notification_manager) {
    notification_manager_.NotifyValueChange(initial_value_info);
  }
  virtual ~ObservableValueUsingNotificationManager() = default;

  ObservableValueUsingNotificationManager(const ObservableValueUsingNotificationManager& that) = delete;
  ObservableValueUsingNotificationManager& operator=(const ObservableValueUsingNotificationManager&) = delete;
  ObservableValueUsingNotificationManager(ObservableValueUsingNotificationManager&&) = delete;
  ObservableValueUsingNotificationManager& operator=(ObservableValueUsingNotificationManager&&) = delete;

  ValueType Get() const override { return value_; }

  ValueCategory GetCategory() const override { return category_; }

  const ValueContext& GetContext() const override { return context_; }

  /**
   * \brief Sets the new value and notifies all subscribes via the notification manager set in the constructor.
   * \param new_value new values to be set
   */
  void Set(const ValueType new_value) override {
    const ValueChangeInfoType change_info = ValueChangeInfoType{value_, new_value, category_, context_};
    value_ = new_value;
    notification_manager_.NotifyValueChange(change_info);
  }

 private:
  ValueType value_;
  ValueCategory category_;
  const ValueContext& context_;
  NotificationManagerType& notification_manager_;
};

}  // namespace notificationmanagement
}  // namespace utility
}  // namespace diag
}  // namespace amsr

#endif  // SRC_UTILITY_NOTIFICATIONMANAGEMENT_OBSERVABLE_VALUE_USING_NOTIFICATION_MANAGER_H_

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
/**        \file  observable_value.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_UTILITY_NOTIFICATIONMANAGEMENT_OBSERVABLE_VALUE_H_
#define SRC_UTILITY_NOTIFICATIONMANAGEMENT_OBSERVABLE_VALUE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "notification_manager.h"
#include "value_change_info.h"
#include "value_subscriber.h"

namespace amsr {
namespace diag {
namespace utility {
namespace notificationmanagement {

/**
 * \brief Notifies all subscribers on value change.
 */
template <class Value, class Category, class Context>
class ObservableValue {
 public:
  /** value type */
  using ValueType = Value;
  /** category type*/
  using ValueCategory = Category;
  /** context type*/
  using ValueContext = Context;

  virtual ~ObservableValue() = default;

  /** \brief Getter for the current value */
  virtual ValueType Get() const = 0;

  /** \brief Getter for the value category */
  virtual ValueCategory GetCategory() const = 0;

  /** \brief Getter for the value context */
  virtual const ValueContext& GetContext() const = 0;

  /**
   * \brief Sets the new value and notifies all subscribes.
   * \param new_value new values to be set
   */
  virtual void Set(const ValueType new_value) = 0;
};

}  // namespace notificationmanagement
}  // namespace utility
}  // namespace diag
}  // namespace amsr

#endif  // SRC_UTILITY_NOTIFICATIONMANAGEMENT_OBSERVABLE_VALUE_H_

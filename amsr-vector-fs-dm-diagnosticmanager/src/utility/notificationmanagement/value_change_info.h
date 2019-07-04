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
/**        \file  value_change_info.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_UTILITY_NOTIFICATIONMANAGEMENT_VALUE_CHANGE_INFO_H_
#define SRC_UTILITY_NOTIFICATIONMANAGEMENT_VALUE_CHANGE_INFO_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace diag {
namespace utility {
namespace notificationmanagement {

/**
 * \brief DataTransferObject for notification of value change.
 */
template <class Value, class Category, class Context>
struct ValueChangeInfo final {
  /** value type */
  using ValueType = Value;
  /** category type*/
  using ValueCategory = Category;
  /** context type*/
  using ValueContext = Context;

  /** old value */
  Value old_value;
  /** new value */
  Value new_value;
  /** value category */
  Category category;
  /** value context */
  const Context& context;

  /**
   * \brief Operator overloading
   * All members are compared by value, expect for context which is compared by address.
   * \param other object to compare, all memb
   * \return true if all members are equal,
   */
  inline bool operator==(const ValueChangeInfo& other) const {
    return (old_value == other.old_value) && (new_value == other.new_value) && (category == other.category) &&
           (&context == &other.context);
  }
};

}  // namespace notificationmanagement
}  // namespace utility
}  // namespace diag
}  // namespace amsr

#endif  // SRC_UTILITY_NOTIFICATIONMANAGEMENT_VALUE_CHANGE_INFO_H_

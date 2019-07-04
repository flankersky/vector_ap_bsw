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
/**        \file  value_subscriber.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_UTILITY_NOTIFICATIONMANAGEMENT_VALUE_SUBSCRIBER_H_
#define SRC_UTILITY_NOTIFICATIONMANAGEMENT_VALUE_SUBSCRIBER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace diag {
namespace utility {
namespace notificationmanagement {

/**
 * \brief Interface allowing a subscriber to be notified about a value change.
 */
template <class ValueChangeInfo>
class ValueSubscriber {
 public:
  virtual ~ValueSubscriber() = default;

  /**
   * \brief Notifies the subscriber about a value change.
   * \param change_info value change information objec.
   */
  virtual void OnValueChange(const ValueChangeInfo change_info) = 0;
};

}  // namespace notificationmanagement
}  // namespace utility
}  // namespace diag
}  // namespace amsr

#endif  // SRC_UTILITY_NOTIFICATIONMANAGEMENT_VALUE_SUBSCRIBER_H_

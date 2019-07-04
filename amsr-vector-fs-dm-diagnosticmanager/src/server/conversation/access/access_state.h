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
/**        \file  access_state.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_CONVERSATION_ACCESS_ACCESS_STATE_H_
#define SRC_SERVER_CONVERSATION_ACCESS_ACCESS_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "category_mask.h"
#include "utility/notificationmanagement/observable_value.h"
#include "utility/notificationmanagement/value_change_info.h"
#include "utility/notificationmanagement/value_subscriber.h"

namespace amsr {
namespace diag {
namespace server {

namespace service {
// forward declaration
class ServiceProcessingContext;
}  // namespace service

namespace conversation {

namespace access {

/** Access category for session, security level etc. */
enum class AccessCategory : std::uint8_t { kSession = 1, kSecurityLevel = 2 };

/** Mask for AccessCategory */
using AccessCategoryMask = CategoryMask<AccessCategory>;

/** Access state implementing a notification mechanism for subscribers */
using ObservableAccessState =
    utility::notificationmanagement::ObservableValue<std::uint8_t, AccessCategory, service::ServiceProcessingContext>;

/** ValueChangeInfo for Access States */
using AccessStateChangeInfo = utility::notificationmanagement::ValueChangeInfo<
    ObservableAccessState::ValueType, ObservableAccessState::ValueCategory, ObservableAccessState::ValueContext>;

/** Subscriber for Access States */
using ObservableAccessStateSubscriber = utility::notificationmanagement::ValueSubscriber<AccessStateChangeInfo>;

}  // namespace access
}  // namespace conversation
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_CONVERSATION_ACCESS_ACCESS_STATE_H_

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
/**        \file  doip_channel_state_pool.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>

#include "tcp/doip_channel_state_pool.h"
#include "tcp/doip_channel_states_impl.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace tcp {

DoIPChannelStatePool::DoIPChannelStatePool()
    : state_listen_(Handle::kListen),
      state_initialized_(Handle::kInitialized),
      state_registered_authentication_pending_(Handle::kRegisteredAuthenticationPending),
      state_registered_confirmation_pending_(Handle::kRegisteredConfirmationPending),
      state_registered_routing_active_(Handle::kRegisteredRoutingActive),
      state_finalize_(Handle::kFinalize) {}

DoIPChannelState* DoIPChannelStatePool::GetState(const Handle handle) {
  switch (handle) {
    case Handle::kListen:
      return &state_listen_;
    case Handle::kInitialized:
      return &state_initialized_;
    case Handle::kRegisteredAuthenticationPending:
      return &state_registered_authentication_pending_;
    case Handle::kRegisteredConfirmationPending:
      return &state_registered_confirmation_pending_;
    case Handle::kRegisteredRoutingActive:
      return &state_registered_routing_active_;
    case Handle::kFinalize:
      return &state_finalize_;
    default:
      throw std::out_of_range("Requested DoIP Connection State not supported.");
  }
}

bool DoIPChannelStatePool::IsValid(const Handle handle) const {
  switch (handle) {
    case Handle::kListen:
    case Handle::kInitialized:
    case Handle::kRegisteredAuthenticationPending:
    case Handle::kRegisteredConfirmationPending:
    case Handle::kRegisteredRoutingActive:
    case Handle::kFinalize:
      return true;
    default:
      return false;
  }
}

}  // namespace tcp
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

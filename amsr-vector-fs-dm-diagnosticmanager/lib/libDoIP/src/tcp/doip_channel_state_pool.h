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
/**        \file  doip_channel_state_pool.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_TCP_DOIP_CHANNEL_STATE_POOL_H_
#define LIB_LIBDOIP_SRC_TCP_DOIP_CHANNEL_STATE_POOL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>

#include "vac/statemachine/state_pool.h"

#include "tcp/doip_channel_state.h"
#include "tcp/doip_channel_states_impl.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace tcp {

/**
 * \brief Pool of DoIP Connection States
 */
class DoIPChannelStatePool : public vac::statemachine::StatePool<DoIPChannelState> {
 public:
  /**
   * \brief Constructor
   */
  DoIPChannelStatePool();

  /**
   * \brief Returns State of given handle.
   *
   * \param handle The handle.
   * \return The State with given handle.
   *
   * If handle does not refer to a State, exception of type std::out_of_range is thrown.
   */
  DoIPChannelState* GetState(const Handle handle) override;

  /**
   * \brief Checks if handle refers to an existing state.
   *
   * \param handle The handle.
   * \return true if state exists with given handle, otherwise returns false.
   */
  bool IsValid(const Handle handle) const override;

 private:
  /**
   * \brief DoIP Connection State implementation for StateHandle::kListen
   */
  DoIPChannelStateListen state_listen_;

  /**
   * \brief DoIP Connection State implementation for StateHandle::kInitialized
   */
  DoIPChannelStateInitialized state_initialized_;

  /**
   * \brief DoIP Connection State implementation for StateHandle::kRegisteredAuthenticationPending
   */
  DoIPChannelStateRegisteredAuthenticationPending state_registered_authentication_pending_;

  /**
   * \brief DoIP Connection State implementation for StateHandle::kRegisteredConfirmationPending
   */
  DoIPChannelStateRegisteredConfirmationPending state_registered_confirmation_pending_;

  /**
   * \brief DoIP Connection State implementation for StateHandle::kRegisteredRoutingActive
   */
  DoIPChannelStateRegisteredRoutingActive state_registered_routing_active_;

  /**
   * \brief DoIP Connection State implementation for StateHandle::kFinalize
   */
  DoIPChannelStateFinalize state_finalize_;
};

}  // namespace tcp
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_TCP_DOIP_CHANNEL_STATE_POOL_H_

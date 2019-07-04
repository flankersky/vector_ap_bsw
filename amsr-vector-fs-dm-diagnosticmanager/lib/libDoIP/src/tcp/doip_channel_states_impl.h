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
/**        \file  doip_channel_states_impl.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_TCP_DOIP_CHANNEL_STATES_IMPL_H_
#define LIB_LIBDOIP_SRC_TCP_DOIP_CHANNEL_STATES_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "tcp/doip_channel_state.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
class DoIPChannel;  // forward declaration
namespace tcp {

/**
 * \brief DoIP Connection State implementation for StateHandle::kListen
 */
class DoIPChannelStateListen : public DoIPChannelState {
  // Inherit constructor
  using DoIPChannelState::DoIPChannelState;

  void OnEnter(DoIPChannel& channel) override {}

  void OnLeave(DoIPChannel& channel) override {}

  bool IsValidChange(const StateHandle handle) const override;

  void HandleMessage(const DoIPMessage& message, DoIPChannel& channel) const override {}
};

/**
 * \brief DoIP Connection State implementation for StateHandle::kInitiallized
 */
class DoIPChannelStateInitialized : public DoIPChannelState {
  // Inherit constructor
  using DoIPChannelState::DoIPChannelState;

  void OnEnter(DoIPChannel& channel) override;

  void OnLeave(DoIPChannel& channel) override;

  bool IsValidChange(const StateHandle handle) const override;

  void HandleMessage(const DoIPMessage& message, DoIPChannel& channel) const override;
};

/**
 * \brief DoIP Connection State implementation for StateHandle::kRegisteredAuthenticationPending
 */
class DoIPChannelStateRegisteredAuthenticationPending : public DoIPChannelState {
  // Inherit constructor
  using DoIPChannelState::DoIPChannelState;

  void OnEnter(DoIPChannel& channel) override;

  void OnLeave(DoIPChannel& channel) override {}

  bool IsValidChange(const StateHandle handle) const override;

  void HandleMessage(const DoIPMessage& message, DoIPChannel& channel) const override;
};

/**
 * \brief DoIP Connection State implementation for StateHandle::kRegisteredConfirmationPending
 */
class DoIPChannelStateRegisteredConfirmationPending : public DoIPChannelState {
  // Inherit constructor
  using DoIPChannelState::DoIPChannelState;

  void OnEnter(DoIPChannel& channel) override;

  void OnLeave(DoIPChannel& channel) override {}

  bool IsValidChange(const StateHandle handle) const override;

  void HandleMessage(const DoIPMessage& message, DoIPChannel& channel) const override;
};

/**
 * \brief DoIP Connection State implementation for StateHandle::kRegisteredRoutingActive
 */
class DoIPChannelStateRegisteredRoutingActive : public DoIPChannelState {
  // Inherit constructor
  using DoIPChannelState::DoIPChannelState;

  void OnEnter(DoIPChannel& channel) override;

  void OnLeave(DoIPChannel& channel) override {}

  bool IsValidChange(const StateHandle handle) const override;

  void HandleMessage(const DoIPMessage& message, DoIPChannel& channel) const override;
};

/**
 * \brief DoIP Connection State implementation for StateHandle::kFinalize
 */
class DoIPChannelStateFinalize : public DoIPChannelState {
  // Inherit constructor
  using DoIPChannelState::DoIPChannelState;

  void OnEnter(DoIPChannel& channel) override;

  void OnLeave(DoIPChannel& channel) override;

  bool IsValidChange(const StateHandle handle) const override;

  void HandleMessage(const DoIPMessage& message, DoIPChannel& channel) const override {}
};

}  // namespace tcp
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_TCP_DOIP_CHANNEL_STATES_IMPL_H_

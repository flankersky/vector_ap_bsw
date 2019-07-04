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
/**        \file  doip_channel_state.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_TCP_DOIP_CHANNEL_STATE_H_
#define LIB_LIBDOIP_SRC_TCP_DOIP_CHANNEL_STATE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "vac/statemachine/state.h"

#include "doip_message.h"

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
class DoIPChannel;  // forward declaration

namespace tcp {

/**
 * State identifiers for DoIP Connection States
 */
enum class StateHandle : std::uint8_t {
  kListen = 0,
  kInitialized = 1,
  kRegisteredAuthenticationPending = 2,
  kRegisteredConfirmationPending = 3,
  kRegisteredRoutingActive = 4,
  kFinalize = 5
};

/**
 * \brief Abstract class for DoIP Connection States
 *
 * The context of the states is DoIPChannel, the Handle type is StateHandle.
 */
class DoIPChannelState : public vac::statemachine::State<StateHandle, DoIPChannel> {
 public:
  /**
   * Adopt constructor of base class.
   */
  using vac::statemachine::State<StateHandle, DoIPChannel>::State;

  /**
   * Pointer to states.
   */
  using Ptr = std::unique_ptr<DoIPChannelState>;

  /**
   * \brief Handles a DoIP message according to state.
   *
   * \param message The DoIP message.
   * \param channel The context of the states.
   */
  virtual void HandleMessage(const DoIPMessage& message, DoIPChannel& channel) const = 0;
};

}  // namespace tcp
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_TCP_DOIP_CHANNEL_STATE_H_

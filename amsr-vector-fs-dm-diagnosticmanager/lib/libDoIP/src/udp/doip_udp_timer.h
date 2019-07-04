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
/**        \file  doip_udp_timer.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDOIP_SRC_UDP_DOIP_UDP_TIMER_H_
#define LIB_LIBDOIP_SRC_UDP_DOIP_UDP_TIMER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <osabstraction/io/network/address/socket_address.h>
#include <vac/testing/test_adapter.h>
#include <vac/timer/timer.h>

namespace amsr {
namespace diag {
namespace udstransport {
namespace doip {
namespace udp {

class DoIPUDPHandler;  // forward declaration

/**
 * \brief Timer subclass
 */
class DoIPUDPTimer : public vac::timer::Timer {
 public:
  /**
   * \brief constructor taking a timer_manager this Timer is associated with.
   */
  explicit DoIPUDPTimer(DoIPUDPHandler* udp_handler, vac::timer::TimerManager* timer_manager);

  /**
   * \brief Handle Timer
   */
  bool HandleTimer();

  /**
   * \brief Setter to the member count_down_
   */
  void SetCountDown(uint8_t count_down) { count_down_ = count_down; }

  /**
   * \brief Setter to the member remote_address_.
   */
  void SetRemoteAddress(osabstraction::io::network::address::SocketAddress remote_address) {
    remote_address_ = remote_address;
  }

  /**
   * \brief Setter to the member broadcast_.
   */
  void SetBroadcast(bool broadcast) { broadcast_ = broadcast; }

 private:
  /**
   * \brief the DoIP_UDP_Handler
   */
  DoIPUDPHandler* const udp_handler_;

  /**
   * \brief counter for number of sent times
   */
  uint8_t count_down_;

  /**
    * \brief Remote address from which the last request came.
    */
  osabstraction::io::network::address::SocketAddress remote_address_;

  /**
   * \brief Broadcast flag to indicate if broadcasting or not.
   */
  bool broadcast_;

  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPUDPTimerTest, SendVehicleIdentMessageOK);
  /**> \brief Friend declaration for testing. */
  FRIEND_TEST(DoIPUDPTimerTest, SendVehicleIdentMessageKO);
};

}  // namespace udp
}  // namespace doip
}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

#endif  // LIB_LIBDOIP_SRC_UDP_DOIP_UDP_TIMER_H_

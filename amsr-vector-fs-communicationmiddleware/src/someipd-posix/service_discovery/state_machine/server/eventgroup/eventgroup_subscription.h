/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2018 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  eventgroup_subscription.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_EVENTGROUP_EVENTGROUP_SUBSCRIPTION_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_EVENTGROUP_EVENTGROUP_SUBSCRIPTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <string>
#include <vector>
#include "vac/timer/timer.h"
#include "vac/timer/timer_manager.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {
namespace eventgroup {

// Forward declaration
struct EventgroupSubscription;

/**
 * \brief Interface for handling the expiry of eventgroup subscription
 */
class EventgroupSubscriptionExpiryHandler {
 public:
  virtual ~EventgroupSubscriptionExpiryHandler() = default;
  /**
   * \brief Called on eventgroup subscription expiration.
   *
   * \param egs An eventgroup subscription which expired.
   */
  virtual void OnSubscribeEventgroupExpired(EventgroupSubscription* egs) = 0;
};

/**
 * \brief Eventgroup TTL timer.
 */
class EventgroupSubscriptionTimer : public vac::timer::Timer {
 public:
  /**
   * \brief Constructor of Timer.
   *
   * \param timer_manager A timer manager.
   * \param handler Timer expiry handler.
   * \param egs The eventgroup subscription to be timed out.
   */
  EventgroupSubscriptionTimer(vac::timer::TimerManager* timer_manager, EventgroupSubscriptionExpiryHandler* handler,
                              EventgroupSubscription& egs)
      : vac::timer::Timer(timer_manager), handler_(handler), egs_(egs) {}

  /**
   * \brief Delete copy constructor.
   */
  EventgroupSubscriptionTimer(const EventgroupSubscriptionTimer&) = delete;
  /**
   * \brief Delete copy assignment operator.
   */
  EventgroupSubscriptionTimer& operator=(const EventgroupSubscriptionTimer&) = delete;
  /**
   * \brief Called on timer expiration.
   *
   * \return always false. Return value is already ignored for oneshot timers.
   */
  bool HandleTimer() override {
    handler_->OnSubscribeEventgroupExpired(&egs_);
    return false;
  }

 private:
  /**
   * \brief A timer callback.
   */
  EventgroupSubscriptionExpiryHandler* handler_;

  /**
   * \brief The eventgroup subscription.
   */
  EventgroupSubscription& egs_;
};

/**
 * \brief Represents an eventgroup subscription.
 */
struct EventgroupSubscription {
  /**
   * \brief Constructor of EventgroupSubscription.
   *
   * \param timer_manager The time manager instance.
   * \param tcp_address A subscriber's IP address.
   * \param tcp_port A subscriber's TCP port.
   * \param udp_address A subscriber's IP address.
   * \param udp_port A subscriber's TCP port.
   * \param eventgroup_id A SOME/IP eventgroup identifier.
   * \param handler Handler to be called on ttl timeout.
   */
  EventgroupSubscription(vac::timer::TimerManager* timer_manager, const std::string& tcp_address,
                         std::uint16_t tcp_port, const std::string& udp_address, std::uint16_t udp_port,
                         std::uint16_t eventgroup_id, EventgroupSubscriptionExpiryHandler* handler)
      : handler_(handler),
        tcp_address_(tcp_address),
        tcp_port_(tcp_port),
        udp_address_(udp_address),
        udp_port_(udp_port),
        eventgroup_id_(eventgroup_id),
        ttl_timeout_timer_(timer_manager, handler_, *this) {}

  /**
   * \brief Delete copy constructor.
   */
  EventgroupSubscription(const EventgroupSubscription&) = delete;
  /**
   * \brief Delete copy assignment operator.
   */
  EventgroupSubscription& operator=(const EventgroupSubscription&) = delete;

  /**
   * \brief Resets TTL timeout timer.
   *
   * \param seconds A number of seconds after which the TTL timeout timer should trigger again.
   */
  void ResetTtlTimeoutTimer(std::uint32_t seconds) {
    ttl_timeout_timer_.Stop();
    ttl_timeout_timer_.SetOneShot(vac::timer::Timer::Clock::now() + std::chrono::seconds(seconds));
    ttl_timeout_timer_.Start();
  }
  EventgroupSubscriptionExpiryHandler* handler_;   ///< TTL timeout handler
  std::string tcp_address_;                        ///< Subscriber's IP address
  std::uint16_t tcp_port_;                         ///< Subscriber's TCP port number
  std::string udp_address_;                        ///< Subscriber's IP address
  std::uint16_t udp_port_;                         ///< Subscriber's UDP port number
  std::uint16_t eventgroup_id_;                    ///< A SOME/IP eventgroup identifier
  EventgroupSubscriptionTimer ttl_timeout_timer_;  ///< A TTL timer for the subscription timeout
};

/**
 * \brief A container of eventgroup subscriptions.
 */
using EventgroupSubscriptionContainer = std::vector<std::unique_ptr<EventgroupSubscription>>;

}  // namespace eventgroup
}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_EVENTGROUP_EVENTGROUP_SUBSCRIPTION_H_

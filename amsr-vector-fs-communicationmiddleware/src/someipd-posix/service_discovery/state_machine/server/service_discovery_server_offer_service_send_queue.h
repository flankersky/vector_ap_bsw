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
/**        \file  service_discovery_server_offer_service_send_queue.h
 *        \brief  SD Offer Service Send Queue
 *
 *      \details  This class manages sending OfferService messages with a given delay through
 *                ServiceDiscoveryMessageSender. It contains a queue to store messages together a timer for each
 *                message. It avoids also duplicate sending of the same message.
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_OFFER_SERVICE_SEND_QUEUE_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_OFFER_SERVICE_SEND_QUEUE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include "ara/log/logging.hpp"
#include "someipd-posix/service_discovery/service_discovery_message_sender.h"
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_message_builder.h"
#include "vac/container/static_list.h"
#include "vac/timer/timer.h"
#include "vac/timer/timer_manager.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {

/**
 * \brief A Queue managing sending all offer-service messages that need to be sent at some time in the future.
 */
class ServiceDiscoveryServerOfferServiceSendQueue {
 private:
  /**
   * \brief Unicast Message Sending Timer.
   */
  class UnicastMessageSenderTimer : public vac::timer::Timer {
   public:
    /**
     * \brief Constructor of Timer.
     *
     * \param timer_manager A timer manager.
     * \param send_queue The message send queue.
     * \param to_address The address the message receiver.
     * \param to_port The port of the message receiver.
     */
    UnicastMessageSenderTimer(vac::timer::TimerManager& timer_manager,
                              ServiceDiscoveryServerOfferServiceSendQueue& send_queue, const std::string& to_address,
                              std::uint16_t to_port)
        : vac::timer::Timer(&timer_manager), to_address_(to_address), to_port_(to_port), send_queue_(send_queue) {}

    /**
     * \brief Called on timer expiration.
     *  It sends a unicast offer service to the receiver specified by to_address_, to_port_, and then removes itself
     * from the send queue.
     *
     * \return always false.  Return value is already ignored for oneshot timers.
     */
    bool HandleTimer() override {
      send_queue_.SendUnicastMessage(to_address_, to_port_);
      auto it =
          std::find(send_queue_.unicast_message_timers_.begin(), send_queue_.unicast_message_timers_.end(), *this);
      assert(it != send_queue_.unicast_message_timers_.end());
      // This statement MUST be the last one before returning !!!
      send_queue_.unicast_message_timers_.erase(it);
      return false;
    }

    /**
     * \brief Resets the message sending time point.
     *
     * \param time_point The point of time at which the timer should trigger again.
     */
    void Reset(vac::timer::Timer::Clock::time_point& time_point) {
      Stop();
      SetOneShot(time_point);
      Start();
    }

    /**
     * \brief Comparator operator.
     * The comparison is done only with respect to the unicast address and port.
     *
     * \param other The timer to compare to.
     */
    bool operator==(const UnicastMessageSenderTimer& other) const {
      return (other.to_address_ == to_address_) && (other.to_port_ == to_port_);
    }

    const std::string to_address_;    ///< Receiver address (in case of unicast)
    const std::uint16_t to_port_{0};  ///< Receiver port (in case of unicast)

   private:
    ServiceDiscoveryServerOfferServiceSendQueue& send_queue_;  ///< OfferService send queue, used for timer callback.
  };

  /**
   * \brief Multicast Message Sending Timer.
   */
  class MulticastMessageSenderTimer : public vac::timer::Timer {
   public:
    /**
     * \brief Constructor of Timer.
     *
     * \param timer_manager A timer manager.
     * \param send_queue The message send queue
     */
    MulticastMessageSenderTimer(vac::timer::TimerManager& timer_manager,
                                ServiceDiscoveryServerOfferServiceSendQueue& send_queue)
        : vac::timer::Timer(&timer_manager), send_queue_(send_queue) {}

    /**
     * \brief Called on timer expiration.
     *  It sends a multicast offer service.
     *
     * \return always false.  Return value is already ignored for oneshot timers.
     */
    bool HandleTimer() override {
      send_queue_.SendMulticastMessage();
      return false;
    }

    /**
     * \brief Resets the message sending time point.
     *
     * \param time_point The point of time at which the timer should trigger again.
     */
    void Reset(vac::timer::Timer::Clock::time_point& time_point) {
      Stop();
      SetOneShot(time_point);
      Start();
    }

   private:
    ServiceDiscoveryServerOfferServiceSendQueue& send_queue_;  ///< OfferService send queue, used for timer callback.
  };

 public:
  /**
   * \brief Constructor
   *
   * \param message_builder SD Server message builder.
   * \param message_sender A message sender.
   * \param timer_manager A timer manager.
   */
  ServiceDiscoveryServerOfferServiceSendQueue(ServiceDiscoveryServerMessageBuilderInterface& message_builder,
                                              ServiceDiscoveryMessageSender& message_sender,
                                              vac::timer::TimerManager& timer_manager);
  /**
   * \brief Adds a timer to send a unicast offer-service message to the unicast timers' queue. Duplicated messages (to
   * the same receiver) are sent only once with the smaller delay.
   * In case the message queue is full, the first set timer within the queue is fired directly.
   *
   * \param to_address The address the message receiver.
   * \param to_port The port of the message receiver.
   * \param delay The duration until this message should be sent.
   */
  void AddToUnicastSendQueue(const std::string& to_address, std::uint16_t to_port,
                             vac::timer::Timer::Clock::duration delay);

  /**
   * \brief Sets the multicast timer with the given delay. If it was already set with a smaller duration, the new
   * duration is ignored. Otherwise, if it was running with a larger duration, it will be reset with the given duration.
   *
   * \param delay The duration until this message should be sent.
   */
  void AddToMulticastSendQueue(vac::timer::Timer::Clock::duration delay);

 private:
  /**
   * \brief Send a unicast offer-service message.
   *
   * \param to_address The address the message receiver.
   * \param to_port The port of the message receiver.
   */
  void SendUnicastMessage(const std::string& to_address, std::uint16_t to_port);
  /**
   * \brief Send a multicast offer-service message.
   */
  void SendMulticastMessage();
  /**
   * \brief SD Server message builder
   */
  ServiceDiscoveryServerMessageBuilderInterface& message_builder_;
  /**
   * \brief A message sender.
   */
  ServiceDiscoveryMessageSender& message_sender_;
  /**
   * \brief A timer manager for message sending.
   */
  vac::timer::TimerManager& timer_manager_;
  /**
   * \brief A container type for unicast message sender timers.
   */
  using UnicastMessageSenderTimerContainer = vac::container::StaticList<UnicastMessageSenderTimer>;
  /**
   * \brief Maximum Size of the Message Container
   */
  static constexpr int kMessagesContainerSize = 256;
  /**
   * \brief A container of unicast message sender timers.
   */
  UnicastMessageSenderTimerContainer unicast_message_timers_;
  /**
   * \brief A multicast message sender timer.
   */
  MulticastMessageSenderTimer multicast_message_timer_;
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;

  FRIEND_TEST(ServiceDiscoveryServerOfferServiceSendQueueFixture, QueueErasion);
  FRIEND_TEST(ServiceDiscoveryServerOfferServiceSendQueueFixture, QueueOverflow);
};

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_SERVER_SERVICE_DISCOVERY_SERVER_OFFER_SERVICE_SEND_QUEUE_H_

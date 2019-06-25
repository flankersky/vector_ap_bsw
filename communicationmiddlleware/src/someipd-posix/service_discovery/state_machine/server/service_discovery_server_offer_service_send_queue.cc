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
/**        \file  service_discovery_server_offer_service_send_queue.cc
 *        \brief  SD Offer Service Send Queue
 *
 *      \details  This class manages sending OfferService messages with a given delay through
 *                ServiceDiscoveryMessageSender. It contains a queue to store messages together a timer for each
  *               message. It avoids also duplicate sending of the same message.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/server/service_discovery_server_offer_service_send_queue.h"
#include <algorithm>
#include "someipd-posix/service_discovery/service_discovery_entry.h"
#include "someipd-posix/service_discovery/service_discovery_option.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace server {

ServiceDiscoveryServerOfferServiceSendQueue::ServiceDiscoveryServerOfferServiceSendQueue(
    ServiceDiscoveryServerMessageBuilderInterface& message_builder, ServiceDiscoveryMessageSender& message_sender,
    vac::timer::TimerManager& timer_manager)
    : message_builder_(message_builder),
      message_sender_(message_sender),
      timer_manager_(timer_manager),
      multicast_message_timer_(timer_manager, *this),
      logger_(ara::log::CreateLogger("SDServerOfferServiceSendQueue", "")) {
  unicast_message_timers_.reserve(kMessagesContainerSize);
}

void ServiceDiscoveryServerOfferServiceSendQueue::AddToUnicastSendQueue(const std::string& to_address,
                                                                        std::uint16_t to_port,
                                                                        vac::timer::Timer::Clock::duration delay) {
  if (delay == vac::timer::Timer::Clock::duration::zero()) {
    SendUnicastMessage(to_address, to_port);
  } else {
    vac::timer::Timer::Clock::time_point time_point = vac::timer::Timer::Clock::now() + delay;

    auto it = std::find_if(unicast_message_timers_.begin(), unicast_message_timers_.end(),
                           [&to_address, &to_port](const UnicastMessageSenderTimer& other) {
                             return (other.to_address_ == to_address) && (other.to_port_ == to_port);
                           });

    if (it == unicast_message_timers_.end()) {
      if (unicast_message_timers_.full()) {
        // All timers are currently busy ? just fire the timer that started first.
        logger_.LogDebug() << "Queue full, dispatching first message";

        auto& first_timer = unicast_message_timers_.front();
        first_timer.Stop();
        SendUnicastMessage(first_timer.to_address_, first_timer.to_port_);
        unicast_message_timers_.pop_front();
      }
      unicast_message_timers_.emplace_back(timer_manager_, *this, to_address, to_port);
      unicast_message_timers_.back().Reset(time_point);
    } else {
      // Message already exist, override or ignore.
      if (it->GetNextExpiry() > time_point) {
        it->Reset(time_point);
        logger_.LogDebug() << "Message already exists, dispatching time updated";
      } else {
        logger_.LogDebug() << "Message already exists, ignored";
      }
    }
  }
}

void ServiceDiscoveryServerOfferServiceSendQueue::AddToMulticastSendQueue(vac::timer::Timer::Clock::duration delay) {
  if (delay == vac::timer::Timer::Clock::duration::zero()) {
    SendMulticastMessage();
  } else {
    vac::timer::Timer::Clock::time_point time_point = vac::timer::Timer::Clock::now() + delay;
    if (multicast_message_timer_.IsExpired()) {
      multicast_message_timer_.Reset(time_point);
    } else {
      // Time already running, override or ignore.
      if (multicast_message_timer_.GetNextExpiry() > time_point) {
        multicast_message_timer_.Reset(time_point);
        logger_.LogDebug() << "Message already exists, dispatching time updated";
      } else {
        logger_.LogDebug() << "Message already exists, ignored";
      }
    }
  }
}

void ServiceDiscoveryServerOfferServiceSendQueue::SendUnicastMessage(const std::string& to_address,
                                                                     std::uint16_t to_port) {
  const auto entries_and_options = message_builder_.MakeOfferServiceMessage();
  assert(!entries_and_options.first.empty());
  const auto service_id{entries_and_options.first[0].service_id_};
  const auto instance_id{entries_and_options.first[0].instance_id_};
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " sending unicast offer message (" << std::hex << service_id
                     << "," << instance_id << std::dec << ") to " << to_address << ":" << to_port;
  message_sender_.SendUnicastMessage(to_address, to_port, entries_and_options.first, entries_and_options.second);
}

void ServiceDiscoveryServerOfferServiceSendQueue::SendMulticastMessage() {
  const auto entries_and_options = message_builder_.MakeOfferServiceMessage();
  assert(!entries_and_options.first.empty());
  const auto service_id{entries_and_options.first[0].service_id_};
  const auto instance_id{entries_and_options.first[0].instance_id_};
  logger_.LogDebug() << __func__ << ":" << __LINE__ << " sending multicast offer message (" << std::hex << service_id
                     << "," << instance_id << std::dec << ")";
  message_sender_.SendMulticastMessage(entries_and_options.first, entries_and_options.second);
}

}  // namespace server
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

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
/**        \file  transmission_trigger_message_queue.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "someipd-posix/service_discovery/connection_manager/transmission_trigger/transmission_trigger_message_queue.h"
#include <cassert>

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {
namespace transmission_trigger {

TransmissionTriggerMessageQueue::TransmissionTriggerMessageQueue(
    vac::timer::TimerManager* timer_manager, const TransmissionTriggerPolicyManager& policy_manager,
    TransmissionTriggerMessageQueueContext& context, TransmissionTriggerBufferSize length_threshold_in_bytes)
    : Timer(timer_manager),
      logger_(ara::log::CreateLogger("TransmissionTriggerMessageQueue", "")),
      policy_manager_{policy_manager},
      context_{context},
      length_threshold_in_bytes_{length_threshold_in_bytes},
      current_length_in_bytes_{0U},
      is_trigger_timeout_pending_{false},
      next_trigger_timeout_{} {}

void TransmissionTriggerMessageQueue::EnqueueMessage(InstanceId instance_id, packet_router::Packet message) {
  auto message_header{message->GetHeader()};
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": (" << std::hex << message_header.service_id_ << ","
                     << instance_id << "," << message_header.method_id_ << "," << message_header.client_id_ << ","
                     << message_header.session_id_ << std::dec << ")";
  auto policy{policy_manager_.FindBestMatchingPolicy(instance_id, message_header)};
  message_queue_.push_back(message);
  current_length_in_bytes_ += message->GetTotalSize();

  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": current queue length in bytes " << current_length_in_bytes_;
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": trigger mode " << static_cast<int>(policy.GetTriggerMode());

  if ((current_length_in_bytes_ >= length_threshold_in_bytes_) ||
      (policy.GetTriggerMode() == TransmissionTriggerMode::kTransmissionTriggerModeAlways)) {
    logger_.LogDebug() << __func__ << ":" << __LINE__ << ": trigger fired";
    StopTriggerTimeout();
    FlushMessageQueue();
    context_.OnSendDatagram();
  } else {
    auto trigger_timeout{policy.GetTriggerTimeout()};
    if (trigger_timeout.second) {
      StartTriggerTimeout(trigger_timeout.first);
    }
  }
}

TransmissionTriggerMessageQueue::DatagramOptional TransmissionTriggerMessageQueue::DequeueNextDatagram() {
  MessageContainer datagram{};
  std::size_t length{0U};
  // Build the next datagram
  while (!datagram_message_queue_.empty() && (length < length_threshold_in_bytes_)) {
    assert(datagram_message_queue_.front() != nullptr);
    auto next_message{datagram_message_queue_.front()};

    length += next_message->GetTotalSize();

    datagram_message_queue_.erase(datagram_message_queue_.begin());
    datagram.push_back(std::move(next_message));
  }
  if (length == 0U) {
    return {{}, false};
  } else {
    return {datagram, true};
  }
}

void TransmissionTriggerMessageQueue::FlushMessageQueue() {
  // Move messages to datagram message queue
  datagram_message_queue_.insert(datagram_message_queue_.end(), message_queue_.begin(), message_queue_.end());
  message_queue_.clear();
  current_length_in_bytes_ = 0U;
}

void TransmissionTriggerMessageQueue::StartTriggerTimeout(TimerDelay delay) {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": start trigger timeout in "
                     << std::chrono::duration_cast<std::chrono::milliseconds>(delay).count() << " ms";

  if (is_trigger_timeout_pending_) {
    // Does the new timeout fire earlier than the current one ?
    if ((vac::timer::Timer::Clock::now() + delay) < next_trigger_timeout_) {
      logger_.LogDebug() << __func__ << ":" << __LINE__ << ": reset trigger timeout";
      // Restart timeout
      Timer::Stop();
      Timer::SetOneShot(delay);
      Timer::Start();
    }
  } else {
    // Activate timeout
    is_trigger_timeout_pending_ = true;
    next_trigger_timeout_ = vac::timer::Timer::Clock::now() + delay;
    Timer::SetOneShot(delay);
    Timer::Start();
  }
}

void TransmissionTriggerMessageQueue::StopTriggerTimeout() {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": stop trigger timeout";
  Timer::Stop();
  is_trigger_timeout_pending_ = false;
}

bool TransmissionTriggerMessageQueue::HandleTimer() {
  logger_.LogDebug() << __func__ << ":" << __LINE__ << ": trigger timeout";
  FlushMessageQueue();
  context_.OnSendDatagram();
  return false;
}

}  // namespace transmission_trigger
}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix

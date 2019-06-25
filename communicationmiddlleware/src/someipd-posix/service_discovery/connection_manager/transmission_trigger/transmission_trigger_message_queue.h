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
/**        \file  transmission_trigger_message_queue.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_TRANSMISSION_TRIGGER_TRANSMISSION_TRIGGER_MESSAGE_QUEUE_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_TRANSMISSION_TRIGGER_TRANSMISSION_TRIGGER_MESSAGE_QUEUE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <utility>
#include <vector>
#include "ara/log/logging.hpp"
#include "someip-posix-common/someip/message.h"
#include "someipd-posix/packet_router/packet_router_interface.h"
#include "someipd-posix/service_discovery/connection_manager/transmission_trigger/transmission_trigger_policy_manager.h"
#include "vac/timer/timer.h"

namespace someipd_posix {
namespace service_discovery {
namespace connection_manager {
namespace transmission_trigger {

/**
 * \brief Context for transmission trigger message queue.
 */
class TransmissionTriggerMessageQueueContext {
 public:
  /**
   * \brief Destructor of TransmissionTriggerMessageQueueContext.
   */
  virtual ~TransmissionTriggerMessageQueueContext() = default;
  /**
   * \brief Called when new datagrams are waiting for transmission.
   */
  virtual void OnSendDatagram() = 0;
};

/**
 * \brief SOME/IP message queue with transmission triggers.
 *
 * \see AUTOSAR_SWS_SocketAdaptor
 */
class TransmissionTriggerMessageQueue : public vac::timer::Timer {
 public:
  /**
   * \brief SOME/IP service identifier type.
   */
  using ServiceId = someip_posix_common::someip::ServiceId;
  /**
   * \brief SOME/IP instance identifier type.
   */
  using InstanceId = someip_posix_common::someip::InstanceId;
  /**
   * \brief SOME/IP method/event identifier type.
   */
  using MethodId = someip_posix_common::someip::MethodId;
  /**
   * \brief Queue type for SOME/IP messages.
   */
  using MessageContainer = std::vector<packet_router::Packet>;
  /**
   * \brief A timer delay.
   */
  using TimerDelay = TransmissionTriggerPolicy::TimerDelay;
  /**
   * \brief Represents a single UDP datagram.
   */
  using DatagramOptional = std::pair<MessageContainer, bool>;
  /**
   * \brief Transmission trigger buffer size.
   */
  using TransmissionTriggerBufferSize = someip_posix_common::someip::TransmissionTriggerBufferSize;

  /**
   * \brief Constructor of TransmissionTriggerMessageQueue.
   *
   * \param timer_manager A timer manager.
   * \param policy_manager A policy manager.
   * \param context A context.
   * \param length_threshold_in_bytes_ A maximum length of a single datagram.
   */
  TransmissionTriggerMessageQueue(vac::timer::TimerManager* timer_manager,
                                  const TransmissionTriggerPolicyManager& policy_manager,
                                  TransmissionTriggerMessageQueueContext& context,
                                  TransmissionTriggerBufferSize length_threshold_in_bytes_);
  /**
   * \brief Enqueues a new SOME/IP message.
   *
   * \param instance_id A SOME/IP instance identifier.
   * \param message A SOME/IP message.
   */
  void EnqueueMessage(InstanceId instance_id, packet_router::Packet message);
  /**
   * \brief Returns the next datagram waiting for transmission.
   *
   * \return A congtainer of SOME/IP messages which comprise a single UDP datagram.
   */
  DatagramOptional DequeueNextDatagram();

 private:
  /**
   * \brief A time point.
   */
  using TimePoint = vac::timer::Timer::Clock::time_point;

  /**
   * \brief Flushes all pending SOME/IP messages.
   */
  void FlushMessageQueue();
  /**
   * \brief Start a transmission trigger timeout.
   *
   * \param A timer delay.
   */
  void StartTriggerTimeout(TimerDelay delay);
  /**
   * \brief Stops the transmission trigger timeout.
   */
  void StopTriggerTimeout();
  /**
   * \brief A transmission trigger timeout handler.
   *
   * \return false.
   */
  bool HandleTimer() override;

  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
  /**
   * \brief A transmission trigger policy manager.
   */
  const TransmissionTriggerPolicyManager& policy_manager_;
  /**
   * \brief A transmission trigger message queue context.
   */
  TransmissionTriggerMessageQueueContext& context_;
  /**
   * \brief A maximum length of a single datagram.
   */
  const TransmissionTriggerBufferSize length_threshold_in_bytes_;
  /**
   * \brief The total length in bytes of all messages stored in the queue.
   */
  std::size_t current_length_in_bytes_;
  /**
   * \brief A queue of SOME/IP messages waiting for trigger.
   */
  MessageContainer message_queue_;
  /**
   * \brief A queue of SOME/IP messages waiting for transmission.
   */
  MessageContainer datagram_message_queue_;
  /**
   * \brief Indicates whether the transmission trigger timeout is active.
   */
  bool is_trigger_timeout_pending_;
  /**
   * \brief The next time point when the trigger timeout shall fire.
   */
  TimePoint next_trigger_timeout_;
};

}  // namespace transmission_trigger
}  // namespace connection_manager
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_CONNECTION_MANAGER_TRANSMISSION_TRIGGER_TRANSMISSION_TRIGGER_MESSAGE_QUEUE_H_

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
/**        \file
 *        \brief  Signal receiver implementation for PikeOS.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/process/signalreceiver.h"
#include <signal.h>   // raise()
#include <stdexcept>  // runtime_error()
#include "osabstraction/messagequeue/queuebuilder.h"
#include "osabstraction/process/process.h"

namespace osabstraction {
namespace process {

SignalReceiver::SignalReceiver() : thread_(), signal_queue_() {
  osabstraction::messagequeue::QueueBuilder builder;

  signal_queue_ = builder.SetId(kSignalQueuePath)
                      .SetCreate()
                      .SetQueueLength(kSignalQueueLength)
                      .SetMessageSize(kSignalQueueMsgSize)
                      .BuildReceiverQueue();

  /* #00 Create receiver thread. */
  if (::pthread_create(&thread_, NULL, ReceiverThread, &signal_queue_) != 0) {
    throw std::runtime_error("Could not create signal receiver thread.");
  }
}

SignalReceiver::~SignalReceiver() {
  /* #00 Cancel thread. */
  ::pthread_cancel(thread_);
}

SignalReceiver::SignalReceiver(SignalReceiver&& other)
    : thread_(std::move(other.thread_)), signal_queue_(std::move(other.signal_queue_)) {}

SignalReceiver& SignalReceiver::operator=(SignalReceiver&& other) {
  std::swap(other, *this);
  return *this;
}

void* SignalReceiver::ReceiverThread(void* queue_ptr) {
  osabstraction::messagequeue::ReceiverQueue* queue =
      reinterpret_cast<osabstraction::messagequeue::ReceiverQueue*>(queue_ptr);

  /* #00 Detach, so the creator thread is not forced to join this thread. */
  if (::pthread_detach(::pthread_self()) != 0) {
    throw std::runtime_error("Could not detach signal receiver thread.");
  }
  /* #00 Set name. */
  if (::pthread_setname_np(::pthread_self(), "signal_receiver") != 0) {
    throw std::runtime_error("Could not set name of signal receiver thread.");
  }

  /* #00 Block all signals for this thread. */
  sigset_t signals;

  if (sigfillset(&signals) != 0) {
    throw std::runtime_error("Could not create signal mask for signal receiver thread.");
  }
  if (pthread_sigmask(SIG_BLOCK, &signals, NULL) != 0) {
    throw std::runtime_error("Could not set signal mask for receiver thread.");
  }

  /* #00 Block for signal messages. */
  for (;;) {
    SignalMessage message;
    queue->Receive(&message, sizeof(message));

    /* #00 Send signal to executing process. */
    if (raise(message.posix_signal) != 0) {
      throw std::runtime_error("Could not send signal to process.");
    }
  }
}

}  // namespace process
}  // namespace osabstraction

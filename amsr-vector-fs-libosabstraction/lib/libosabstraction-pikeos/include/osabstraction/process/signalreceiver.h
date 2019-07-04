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

#ifndef LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_PROCESS_SIGNALRECEIVER_H_
#define LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_PROCESS_SIGNALRECEIVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <pthread.h>  // pthread_*
#include "osabstraction/messagequeue/receiverqueue.h"
#include "osabstraction/process/signalreceiverinterface.h"

namespace osabstraction {
namespace process {

/**
 * \brief Signal message which is used to transfer signals via PikeOS Queuing Port.
 */
struct SignalMessage {
  /**
   * \brief POSIX signal.
   */
  int posix_signal;
};

/**
 * \brief Path which is used for the sender queue.
 * The path has the following syntax on sender side: "/signals_<PID>"
 * The path has the following syntax on receiver side: "/signals_"
 */
constexpr static const char* kSignalQueuePath = "/signals_";

/**
 * \brief Signal queue length.
 */
constexpr unsigned int kSignalQueueLength = 10;

/**
 * \brief Signal queue message size.
 */
constexpr unsigned int kSignalQueueMsgSize = sizeof(SignalMessage);

/**
 * \brief Signal receiver for PikeOS.
 *
 * The signal receiver on PikeOS listens on a dedicated QPort for signal messages. If a signal message is received
 * it is translated into a POSIX signal.
 */
class SignalReceiver final {
 public:
  /**
   * \brief Creates the signal receiver.
   * Creates a thread which blocks for signal messages on the signal transfer queue.
   */
  SignalReceiver();

  /**
   * \brief Destroys the signal receiver.
   * Cancels the thread.
   */
  ~SignalReceiver();

  /**
   * \brief Move constructor.
   */
  SignalReceiver(SignalReceiver&& other);

  /**
   * \brief Move assignment operator.
   */
  SignalReceiver& operator=(SignalReceiver&& other);

 private:
  SignalReceiver(const SignalReceiver& other) = delete;
  SignalReceiver& operator=(const SignalReceiver& other) = delete;

  /**
   * \brief Receives signal messages and translates them into POSIX signals.
   * \param[in|out] queue_ptr   Pointer to the receiver queue on which to listen.
   */
  static void* ReceiverThread(void* queue_ptr);

  /**
   * \brief Thread descriptor.
   */
  pthread_t thread_;

  /**
   * \brief Queue which is used for signal transfer.
   */
  osabstraction::messagequeue::ReceiverQueue signal_queue_;
};

}  // namespace process
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_PROCESS_SIGNALRECEIVER_H_

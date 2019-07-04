/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2018 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file
 *        \brief  Implementation of the process interface for PikeOS.
 *
 *      \details  Implements the process interface for PikeOS.
 *
 *********************************************************************************************************************/

#include "osabstraction/process/process.h"

#include <limits.h>    // PATH_MAX
#include <signal.h>    // SIGTERM
#include <sys/stat.h>  // stat()
#include <unistd.h>    // access()

#include <cstring>    // string()
#include <stdexcept>  // runtime_error()
#include <utility>    // swap()

#include "osabstraction/messagequeue/queuebuilder.h"
#include "osabstraction/process/signalreceiver.h"

namespace osabstraction {
namespace process {

/**
 * \brief Maximum path length.
 * According to POSIX IEEE Std 1003.1 2016 Edition, PATH_MAX includes the terminating null character.
 */
constexpr unsigned int kMaxPathLength = PATH_MAX;

ProcessId GetProcessId() { return internal::GetPartitionId(); }

void IgnoreChildTerminationSignal() {}

Process::Process(ProgramImage image, Arguments args) : partition_(-1), is_running_(false), signal_queue_() {
  /* #00 Check that program image is not nullptr. */
  if (image == nullptr) {
    throw std::runtime_error("No program image given");
  }
  /* #00 If program image path to long, throw exception. */
  if ((std::strlen(image) + 1) > kMaxPathLength) {
    throw std::range_error("Given program image path is too long.");
  }

  /* #00 Check that given program image exists. */
  struct stat file_status;
  if (stat(image, &file_status) != 0) {
    throw std::runtime_error(std::string("File status could not be retrieved using stat(). File path is: ") + image);
  }
  if (!S_ISREG(file_status.st_mode)) {
    throw std::runtime_error(std::string("Given file is not a regular file. File path is: ") + image);
  }

  /* #00 Get a partition from the partition pool. */
  partition_ = internal::PartitionPool::GetInstance().GetPartition();

  /* #00 Open queue for sending termination requests. */
  osabstraction::messagequeue::QueueBuilder queue_builder;
  std::array<char, kMaxPathLength> signal_queue_path;
  int bytes_written =
      std::snprintf(signal_queue_path.data(), signal_queue_path.size(), "%s%d", kSignalQueuePath, partition_.GetId());
  if (bytes_written < 0 || bytes_written >= static_cast<int>(sizeof(signal_queue_path))) {
    throw std::runtime_error("Path to qport for sending termination requests is too long.");
  }
  signal_queue_ = queue_builder.SetId(signal_queue_path.data())
                      .SetCreate()
                      .SetQueueLength(kSignalQueueLength)
                      .SetMessageSize(kSignalQueueMsgSize)
                      .BuildSenderQueue();

  /* #00 Critical section: */
  {
    std::unique_lock<std::mutex> lock{LoadMemory::GetInstance().mutex_};

    /* #00 Load application image into shared memory. */
    LoadMemory::GetInstance().LoadFile(image);

    /* #00 Start partition. */
    partition_.Start();
  }

  /* TODO(visasl): Implement argument passing via message queue abstraction. */
  (void)args;

  /* TODO(visasl): Implement wait for "application successfully loaded" message. */

  /* #00 Set process state to running. */
  is_running_ = true;
}

Process::Process(Process&& other)
    : partition_(std::move(other.partition_)),
      is_running_(std::move(other.is_running_)),
      signal_queue_(std::move(other.signal_queue_)) {}

Process& Process::operator=(Process&& other) {
  std::swap(other, *this);
  return *this;
}

Process::LoadMemory& Process::LoadMemory::GetInstance() {
  static LoadMemory instance;
  return instance;
}

bool Process::IsRunning() {
  /* #00 If process is assumed to be running: */
  if (is_running_) {
    /* #00 Update running state. */
    is_running_ = partition_.IsRunning();
  }

  /* #00 Return running state. */
  return is_running_;
}

void Process::SendTerminationRequest() {
  /* #00 Send termination request via qport. */
  static const SignalMessage termination_request = {SIGTERM};
  signal_queue_.Send(&termination_request, sizeof(termination_request));
}

ProcessId Process::GetId() const { return partition_.GetId(); }

Process ProcessBuilder::Build() {
  /* #00 Create process with given settings and return it. */
  return Process(image_, args_);
}

}  // namespace process
}  // namespace osabstraction

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
/**        \file  routine_control_3009.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "routine_control_3009.h"

#include <chrono>
#include "ara/log/logging.hpp"
namespace diag_test_application {
namespace service_interfaces {

RoutineControl3009::~RoutineControl3009() {
  exit_requested_ = true;
  {
    std::lock_guard<std::mutex> lk(mutex_);
    cond_var_.notify_one();
  }
  if (thread_.joinable()) {
    thread_.join();
  }
}

void RoutineControl3009::Initialize() { thread_ = std::thread(&RoutineControl3009::Run, this); }

void RoutineControl3009::Shutdown() {
  exit_requested_ = true;
  {
    std::lock_guard<std::mutex> lk(mutex_);
    cond_var_.notify_one();
  }
  if (thread_.joinable()) {
    thread_.join();
  }
}

ara::com::Future<ara::diag::service_interfaces::routine_control_3009::skeleton::methods::Start::Output>
RoutineControl3009::Start(const ara::diag::service_interfaces::routine_control_3009::MetaInfoType &MetaInfo,
                          const ara::diag::service_interfaces::routine_control_3009::boolean &Req_Reset,
                          const ara::diag::service_interfaces::routine_control_3009::uint32 &Req_CounterLimit) {
  ara::com::Promise<ara::diag::service_interfaces::routine_control_3009::skeleton::methods::Start::Output> promise;
  ara::diag::service_interfaces::routine_control_3009::skeleton::methods::Start::Output output;

  ara::log::LogDebug() << "RoutineControl3009::" << __func__ << ": counter=" << counter_;
  std::lock_guard<std::mutex> lk(mutex_);
  // Reset counter if requested.
  if (Req_Reset) {
    counter_ = 0;
  }

  // Change counter limit
  counter_limit_ = Req_CounterLimit;

  // Check if counter is already greater than requested time.
  if (counter_ >= counter_limit_) {
    promise.set_exception(std::make_exception_ptr(
        ara::diag::service_interfaces::routine_control_3009::skeleton::application_errors::UDSServiceFailedOnStart{
            static_cast<ara::diag::service_interfaces::routine_control_3009::UDSResponseCodeType>(
                ara::diag::service_interfaces::routine_control_3009::UDSResponseCodeType::kRequestOutOfRange)}));
    return promise.get_future();
  }

  // Change the state of the counter
  counter_state_ = CounterState::kStarted;

  // Notify thread that Timer has been started.
  cond_var_.notify_one();

  promise.set_value(output);
  return promise.get_future();
}

ara::com::Future<ara::diag::service_interfaces::routine_control_3009::skeleton::methods::Stop::Output>
RoutineControl3009::Stop(const ara::diag::service_interfaces::routine_control_3009::MetaInfoType &MetaInfo) {
  ara::com::Promise<ara::diag::service_interfaces::routine_control_3009::skeleton::methods::Stop::Output> promise;
  ara::diag::service_interfaces::routine_control_3009::skeleton::methods::Stop::Output output;

  ara::log::LogDebug() << "RoutineControl3009::" << __func__;
  // Lock the mutex.
  std::lock_guard<std::mutex> lk(mutex_);

  // Change the state of the counter
  counter_state_ = CounterState::kStopped;

  // Get the current counter and return.
  output.Resp_RemainingCounts = counter_limit_ - counter_;
  ara::log::LogDebug() << "RoutineControl3009::" << __func__ << ": remaining time = " << output.Resp_RemainingCounts;
  promise.set_value(output);
  return promise.get_future();
}

ara::com::Future<ara::diag::service_interfaces::routine_control_3009::skeleton::methods::RequestResults::Output>
RoutineControl3009::RequestResults(const ara::diag::service_interfaces::routine_control_3009::MetaInfoType &MetaInfo) {
  ara::com::Promise<ara::diag::service_interfaces::routine_control_3009::skeleton::methods::RequestResults::Output>
      promise;
  ara::diag::service_interfaces::routine_control_3009::skeleton::methods::RequestResults::Output output;

  ara::log::LogDebug() << "RoutineControl3009::" << __func__;
  // Lock the mutex.
  std::lock_guard<std::mutex> lk(mutex_);

  // Get the current counter and return.
  output.Resp_RemainingCounts = counter_limit_ - counter_;
  ara::log::LogDebug() << "RoutineControl3009::" << __func__ << ": remaining time = " << output.Resp_RemainingCounts;
  promise.set_value(output);
  return promise.get_future();
}

void RoutineControl3009::Run() {
  std::chrono::milliseconds sleep_time{1};
  while (!exit_requested_) {
    std::unique_lock<std::mutex> lk(mutex_);
    // Wait for the counter to be started or the exit to be requested.
    cond_var_.wait(lk, [this]() { return this->exit_requested_ || (this->counter_state_ == CounterState::kStarted); });

    while (!exit_requested_ && (counter_state_ == CounterState::kStarted)) {
      // Unlock mutex and sleep for 1 ms.
      lk.unlock();
      std::this_thread::sleep_for(sleep_time);

      // Lock mutex and decrement counter if count is still running and no exit has been requested.
      lk.lock();
      if (!exit_requested_ && (counter_state_ == CounterState::kStarted)) {
        if (counter_ != counter_limit_) {
          ++counter_;
        } else {
          // Stop the count.
          counter_state_ = CounterState::kStopped;
        }
      }
    }
  }
}

}  // namespace service_interfaces
}  // namespace diag_test_application

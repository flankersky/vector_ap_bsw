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
/**        \file  skeleton_base.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/internal/skeleton_base.h"
#include "ara/com/future.h"
#include "ara/com/promise.h"

namespace ara {
namespace com {
namespace internal {

ara::com::Future<bool> SkeletonBase::ProcessNextMethodCall() {
  ara::com::Promise<bool> promise;
  ara::com::Future<bool> future = promise.get_future();
  bool pending_request{false};

  if (processing_mode_ == ara::com::MethodCallProcessingMode::kPoll) {
    // Get the next method request from the skeleton buffer
    std::unique_ptr<Task> request{};

    if (!request_buffer_.empty()) {
      request = std::move(request_buffer_.front());
      request_buffer_.pop();
      if (request) {
        pending_request = true;
      } else {
        ara::log::LogFatal() << "Couldn't process request because of nullptr address.";
      }
    }

    if (pending_request) {
      struct FutureTask : public ara::com::internal::Task {
        FutureTask(std::unique_ptr<Task> task, ara::com::Promise<bool> p)
            : task_{std::move(task)}, processed_{std::move(p)} {}

        void operator()() {
          if (task_) {
            (*task_)();
          }
          // After the original task is being processed,
          // serve the promise.
          processed_.set_value(true);
        }

        std::unique_ptr<Task> task_;
        ara::com::Promise<bool> processed_;
      };

      // Transfer the task from the skeleton-buffer to the thread-pool assigned.
      thread_pool_.AddTask(FutureTask(std::move(request), std::move(promise)));
    } else {
      // Serve the promise immediately because there's no pending request to process.
      promise.set_value(pending_request);
    }
  } else {
    // In reference to the AraCom explanatory the method shall throw an exception
    // if this is called in event-driven mode.
    throw std::logic_error("ProcessNextMethodCall in event-driven processing-mode shall not be called.");
  }

  return future;
}

}  // namespace internal
}  // namespace com
}  // namespace ara

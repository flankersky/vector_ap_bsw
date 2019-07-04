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
/**        \file  skeleton_base.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_SKELETON_BASE_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_SKELETON_BASE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <utility>
#include "ara/com/future.h"
#include "ara/com/internal/skeleton_request_handling.h"
#include "ara/com/internal/thread_pool.h"
#include "ara/com/runtime.h"
#include "ara/com/types.h"

namespace ara {
namespace com {
namespace internal {

/**
 * \brief SkeletonBase is the base of all skeleton interfaces.
 * The base class provides config-independent features all service interfaces have in common.
 * This includes threading and processing modes. The common behavior is shifted to the base
 * class to be able to test the threading model and processing modes without the need of
 * generated sources in unit-tests.
 */
class SkeletonBase {
 public:
  /**
   * \brief Constructor sets the processing mode for this skeleton object.
   * \uptrace SWS_CM_00198
   * \tparam Number of characters.
   * \param instance_id ara::com instance identifier of this service.
   * \param mode Processing mode to set for this skeleton object.
   * \param name Interface name of the generated skeleton / proxy.
   */
  template <std::size_t N>
  SkeletonBase(const ara::com::InstanceIdentifier instance_id, const ara::com::MethodCallProcessingMode mode,
               const char (&name)[N])
      : instance_id_{instance_id},
        processing_mode_{mode},
        thread_pool_{Runtime::getInstance().RequestThreadPoolAssignment(name, instance_id)} {
    if ((processing_mode_ == ara::com::MethodCallProcessingMode::kEventSingleThread) &&
        (thread_pool_.GetNumberOfWorkerThreads() != 1U)) {
      throw std::logic_error(
          "In processing mode kEventSingleThread the assigned thread-pool shall be limited to one worker thread to "
          "ensure sequential request processing.");
    }

    if (processing_mode_ == ara::com::MethodCallProcessingMode::kPoll) {
      request_buffer_.reserve(1024U);
    }

    ara::log::LogDebug() << "Thread-Pool assigned: " << std::dec
                         << static_cast<std::uint16_t>(thread_pool_.GetPoolId());
  }

  /**
   * \brief Virtual destructor
   */
  virtual ~SkeletonBase() = default;

  /**
   * \brief Fetches the next call from the Communication Management and executes
   * it. This method is only available in polling mode and throws an exception in
   * event mode.
   * \uptrace SWS_CM_00199
   * \return The future contains a boolean value of true, if there is a method request pending in
   * the global request queue, which is being processed, false if there was no
   * method request pending. The promise's value is set as soon as the request has been processed.
   */
  ara::com::Future<bool> ProcessNextMethodCall();

  /**
   * \brief Enqueues an incoming method request with its parameters in polling-mode or
   * directly forwards the method-call in event-driven mode to the thread-pool of this service instance.
   * A binding-specific implementation shall not know anything about the processing-mode
   * chosen in the frontend. The frontend decides how to proceed based on the processing-mode
   * given in the constructor of this object.
   *
   * \tparam Request Deduced type for each method. Every request is modeled as an object of a specific type. This shall
   * be generic to decouple frontend and binding-related part as best as possible.
   * \param request This object contains all the information necessary to:
   * 1. Call the concrete method in the frontend; the binding is aware of the method to call and stores this information
   * in the functor.
   * 2. Get the return value with a given type from the future, because the return type
   * is also known to the backend.
   * 3. Call the response path in the binding-related part with the information provided within this
   * object.
   * \return true if a request was enqueued, false if not.
   */
  template <typename Request>
  bool HandleMethodRequest(Request&& request) {
    bool handled{false};

    if ((processing_mode_ == ara::com::MethodCallProcessingMode::kEvent) ||
        (processing_mode_ == ara::com::MethodCallProcessingMode::kEventSingleThread)) {
      // Pass the method request to the assigned thread-pool for processing
      // as soon as a worker thread is available.
      thread_pool_.AddTask(request);
    } else {
      using Task_ = typename std::decay<Request>::type;  // Without any decoration
      std::unique_ptr<Task_> request_{new Task_(std::move(request))};
      {
        // In kPoll the request first is hold back and must be buffered in the skeleton first
        // Then, on a ProcessNextMethodCall() by the user, the buffered request will transferred
        // to the thread-pool assigned to this skeleton.
        request_buffer_.push(std::move(request_));
      }
      handled = true;
    }

    return handled;
  }

 protected:
  /**
   * \brief Instance represented by this skeleton object.
   */
  const ara::com::InstanceIdentifier instance_id_;

  /**
   * Method call processing mode used by this skeleton instance.
   */
  const ara::com::MethodCallProcessingMode processing_mode_;

  /**
   * \brief Reference to the thread pool assigned to this service skeleton instance.
   */
  ara::com::internal::ThreadPool& thread_pool_;

  /**
   * \brief Type-alias for the buffer that gets filled on incoming method requests
   * from clients in kPoll processing mode.
   */
  using RequestBuffer = ara::com::internal::StaticQueue<std::unique_ptr<Task>>;

  /**
   * \brief Skeleton request buffer for all incoming method requests if kPoll is active.
   */
  RequestBuffer request_buffer_;
};
}  // namespace internal
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_SKELETON_BASE_H_

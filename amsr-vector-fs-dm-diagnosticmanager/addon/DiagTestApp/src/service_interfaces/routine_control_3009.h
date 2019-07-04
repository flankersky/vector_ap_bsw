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
/**        \file  routine_control_3009.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTAPP_SRC_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_H_
#define ADDON_DIAGTESTAPP_SRC_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_skeleton.h"

namespace diag_test_application {
namespace service_interfaces {

/**
 * \brief Implementation of RoutineControl3009 Class
 */
class RoutineControl3009
    : public ara::diag::service_interfaces::routine_control_3009::RoutineControl_SWCL_A_RID_3009Skeleton {
 public:
  /**
   * \brief Constructor
   */
  explicit RoutineControl3009(const ara::com::InstanceIdentifier &instance_id)
      : RoutineControl_SWCL_A_RID_3009Skeleton(instance_id),
        counter_state_(CounterState::kStopped),
        counter_(0),
        counter_limit_(0),
        exit_requested_(false) {}

  /**
   * \brief Destructor
   */
  virtual ~RoutineControl3009();

  /**
   * \brief Init method to start the thread using move operator.
   */
  void Initialize();

  /**
   * \brief Shutdown method to terminate the thread
   */
  void Shutdown();

  RoutineControl3009(const RoutineControl3009 &) = delete;
  RoutineControl3009 &operator=(const RoutineControl3009 &) = delete;
  RoutineControl3009(const RoutineControl3009 &&) = delete;
  RoutineControl3009 &operator=(const RoutineControl3009 &&) = delete;

  /**
  * \brief Start method
  */
  ara::com::Future<ara::diag::service_interfaces::routine_control_3009::skeleton::methods::Start::Output> Start(
      const ara::diag::service_interfaces::routine_control_3009::MetaInfoType &MetaInfo,
      const ara::diag::service_interfaces::routine_control_3009::boolean &Req_Reset,
      const ara::diag::service_interfaces::routine_control_3009::uint32 &Req_CounterLimit) override;

  /**
   * \brief Stop method
   */
  ara::com::Future<ara::diag::service_interfaces::routine_control_3009::skeleton::methods::Stop::Output> Stop(
      const ara::diag::service_interfaces::routine_control_3009::MetaInfoType &MetaInfo) override;

  /**
   * \brief RequestResults method
   */
  ara::com::Future<ara::diag::service_interfaces::routine_control_3009::skeleton::methods::RequestResults::Output>
  RequestResults(const ara::diag::service_interfaces::routine_control_3009::MetaInfoType &MetaInfo) override;

 private:
  /**
   * \brief Enum for counter state
   */
  enum class CounterState : std::uint8_t { kStopped = 0x00, kStarted = 0x01 };

  /**
   * \brief counter state
   */
  CounterState counter_state_;

  /**
   *\brief mutex to access counter state
   */
  std::mutex mutex_;

  /**
   * \brief condition variable to wake the thread up
   */
  std::condition_variable cond_var_;

  /**
   * \brief thread managing the counter
   */
  std::thread thread_;

  /**
   * \brief Counter
   */
  std::uint32_t counter_;

  /**
   * \brief Counter limit value
   */
  std::uint32_t counter_limit_;

  /**
   * \brief boolean to request exit
   */
  std::atomic_bool exit_requested_;

  /**
   * \brief Run method for thread
   */
  void Run();

 private:
  FRIEND_TEST(RoutineControl3009, Instantiation);
  FRIEND_TEST(RoutineControl3009, Start);
  FRIEND_TEST(RoutineControl3009, Stop);
  FRIEND_TEST(RoutineControl3009, RequestResults);
  FRIEND_TEST(RoutineControl3009, CounterReachedLimit);
  FRIEND_TEST(RoutineControl3009, CounterGreaterLimit);
};

}  // namespace service_interfaces
}  // namespace diag_test_application
#endif  // ADDON_DIAGTESTAPP_SRC_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_H_

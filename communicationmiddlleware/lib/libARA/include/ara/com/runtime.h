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
/**        \file  runtime.h
 *        \brief  Contains central runtime module of the ara::com API
 *
 *      \details  This module plays a central role for the ara::com. It manages all bindings and connects them to the
 *                corresponding proxy and skeleton classes (see broker pattern). The generated interfaces
 *                (proxy/skeleton) delegating their middleware interactions (like service discovery) to the runtime.
 *                The runtime then dispatches every request to the binding.
 *          \see  AUTOSAR_SWS_CommunicationManagement chapter 7.1.2
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_RUNTIME_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_RUNTIME_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <atomic>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>

#include "ara/com/configuration/json_configuration.h"
#include "ara/com/internal/binding_pool.h"
#include "ara/com/internal/thread_pool.h"
#include "osabstraction/io/reactor.h"
#include "vac/container/static_vector.h"
#include "vac/timer/timer_manager.h"

namespace ara {
namespace com {

/**
 * \brief The Runtime class creates necessary binding instances.
 * Runtime is derived from base class RuntimeConfigModel as it has the right to change configuration parameters.
 */
class Runtime {
 public:
  /**
   * \brief Each service instance can be assigned to / uses a specific thread-pool. This pair is
   * used for identification of a given element (service interface).
   */
  using ServiceInstanceAssignment = std::pair<const std::string, const ara::com::InstanceIdentifier>;

  /**
   * \brief Each thread-pool contained in the Runtime is assigned to a number of service instances identified by the
   * service interface name and the ara::com::InstanceIdentifier.
   * The thread-pools act as a key. Each thread-pool as a key has multiple service instances assigned as the value
   * (static vector). Each element of this vector represents one service instance that refers to the thread-pool to use.
   */
  using ThreadPoolMapElement =
      std::pair<ara::com::internal::ThreadPool, vac::container::StaticVector<ServiceInstanceAssignment>>;

  /**
   * \brief Forward Json file path type from configuration
   */
  using JsonFilePath = typename configuration::JsonConfiguration::JsonFilePath;

  /**
    * \brief Returns an instance of Runtime class
    * \return Runtime instance
    */
  static Runtime& getInstance();

  /** \brief Initializes internal runtime components
   *  To reset all bindings, this may be called multiple times. This will destroy and reconstruct all
   *  registered bindings.
   *
   * \param json_config_path Path to the JSON configuration (application.json) to read in for ara::com specific and
   * binding-related parameters.
   */
  void Initialize(const JsonFilePath& json_config_path);

  /**
   * \brief Initialization without user-defined JSON configuration
   */
  void Initialize();

  /**
   * \brief A skeleton calls this Runtime method on construction to get the thread-pool assigned to the concrete
   * service instance. If there's no reference to a user-specific thread-pool for this object, the default thread-pool
   * is returned as reference.
   *
   * \tparam N Number of characters for the name string. This is automatically deduced by the compiler when the string
   * is given as parameter.
   * \param name The name as a string of the generated service interface given by the concrete service instance.
   * \param instance ara::com::InstanceIdenfifier of the service interface instance.
   * \return the thread-pool assigned to this service interface instance. In case of no user-specific assignment, the
   * default thread-pool is returned as a reference.
   */
  template <int N>
  ara::com::internal::ThreadPool& RequestThreadPoolAssignment(const char (&name)[N],
                                                              const ara::com::InstanceIdentifier instance) {
    ara::log::Logger& logger = ara::log::CreateLogger("Runtime", "");
    const std::string interface_name{name};
    logger.LogDebug() << __func__ << " - request from service interface " << interface_name << ", instance identifier "
                      << instance.GetInstanceId();

    auto it = std::find_if(thread_pools_.begin(), thread_pools_.end(), [&](ThreadPoolMapElement& pool_element) {
      const auto assign_it =
          std::find_if(pool_element.second.cbegin(), pool_element.second.cend(),
                       [&](const ServiceInstanceAssignment& service_mapping) {
                         return (service_mapping == ServiceInstanceAssignment(interface_name, instance));
                       });
      return (assign_it != pool_element.second.cend());
    });

    if (it != thread_pools_.end()) {
      return it->first;
    } else {
      return default_thread_pool_;
    }
  }

  /**
   * \brief Destructor
   */
  ~Runtime();

  /**
   * \brief Runtime shall be non-copyable.
   */
  Runtime(Runtime const&) = delete;

  /**
   * \brief Delete assignment operator
   */
  void operator=(Runtime const&) = delete;

  /**
  * \brief Returns a reference to the underlying bindingpool.
  *
  * \returns The instance of the BindingPool
  */
  ara::com::internal::BindingPool& GetBindingPool() const;

  /**
   * \brief Returns a reference to the Reactor.
   *
   * \returns The reference to the reactor
   */
  osabstraction::io::Reactor& GetReactor() { return reactor_; }

  /**
   * \brief Returns a reference to the timer manager.
   *
   * \returns The reference to the timer manager
   */
  vac::timer::TimerManager& GetTimerManager() { return timer_manager_; }

  /**
   * \brief Check whether the Runtime is up and running or not. This is relevant for applications with multiple threads
   * that are active as long as the Runtime is initialized.
   *
   * \return true if this Runtime instance is initialized, false if Runtime is not initialized.
   */
  bool IsRunning() const { return !runtime_done_; }

  /**
   * \brief Does one step for each binding. Takes the next packet on the socket in full polling mode on all bindings for
   * this Runtime and routes it to the desired proxy / skeleton instance.
   * \remark This method shall only invoke the processing of a pending packet of a concrete binding in kPolling mode.
   * \remark Thread-Safety: This method may be called concurrently (from multiple threads in the application).
   * Currently a lock hinders from acessing bindings concurrently in full polling mode.
   *
   * \return a boolean flag that signalizes if one of the registered bindings had a packet which was routed, false if
   * there was no packet to process in any of the registered bindings.
   */
  bool ProcessNextPacket();

 private:
  /**
   * \brief Constructor is private because of the singleton-pattern for Runtime.
   */
  Runtime()
      : logger_(ara::log::CreateLogger("Runtime", "")),
        timer_manager_(&reactor_),
        binding_pool_{nullptr},
        runtime_done_{true} {}

  /**
   * \brief Initializes the thread-pools.
   */
  void InitializeThreadPools();

  /**
   * \brief Initializes the reactor thread.
   */
  void InitializeReactorThread();

  /**
   * \brief Generated method to add all BindingInterfaces present in this application to the binding_pool_.
   */
  void InitializeBindings();

  /**
   * \brief Generated method to destroy all present BindingInterfaces present in this application.
   *
   * Important: The bindings must be destroyed here before the reactor_ attribute gets destroyed.
   * Otherwise possible calls in binding destructors to the reactor (e.g. UnregisterEventHandler()) will fail.
   */
  void DeinitializeBindings();

  /**
   * \brief An instance of a logger.
   */
  ara::log::Logger& logger_;

  /**
   * \brief Abstract ara::com configuration model
   */
  configuration::Configuration config_;

  /**
    * \brief reactor_done_ globally declare if reactor created or not
    */
  std::atomic<bool> reactor_done_;

  /**
    * \brief reactor_ object
    */
  osabstraction::io::Reactor reactor_;

  /**
    * \brief timer_manager_ object
    */
  vac::timer::TimerManager timer_manager_;

  /**
    * \brief reactor_thread_ reactor thread
    */
  std::thread reactor_thread_;

  /**
   * \brief The pool class for creating binding objects
   */
  std::unique_ptr<ara::com::internal::BindingPool> binding_pool_;

  /**
   * \brief Signalizes if the Runtime instance is still alive for multi-threaded applications.
   */
  std::atomic<bool> runtime_done_{false};

  /**
   * \brief Lock for concurrent access of Runtime methods.
   */
  std::mutex runtime_lock_;

  /**
   * \brief One default thread-pool if there are no user-defined thread-pools configured.
   */
  ara::com::internal::ThreadPool default_thread_pool_;

  /**
   * \brief Type-alias for the thread-pool container. The static vector is filled during initialization time once.
   * All user-specific thread-pools will be emplaced and the service interface instances using these thread-pools
   * will be assigned to the individual elements.
   */
  using ThreadPoolContainer = vac::container::StaticVector<ThreadPoolMapElement>;

  /**
   * \brief All thread-pools are hold in the Runtime object.
   */
  ThreadPoolContainer thread_pools_;
};

}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_RUNTIME_H_

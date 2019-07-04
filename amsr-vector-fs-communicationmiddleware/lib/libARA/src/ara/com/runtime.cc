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
/**        \file  runtime.cc
 *        \brief
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/runtime.h"
#include <limits>

namespace ara {
namespace com {

Runtime& Runtime::getInstance() {
  static Runtime instance;
  return instance;
}

void Runtime::Initialize(const JsonFilePath& json_config_path) {
  logger_.LogInfo() << "zhuang 1";

  logger_.LogDebug() << __func__ << ":" << __LINE__;
  logger_.LogInfo() << "json_config_path" <<json_config_path.c_str();

  // Parse configuration from JSON and build abstract configuration model.
  config_ = configuration::JsonConfiguration{json_config_path};

  Initialize();
}

void Runtime::Initialize() {
  logger_.LogInfo() << "zhuang 2";

  logger_.LogDebug() << __func__ << ":" << __LINE__;

  std::lock_guard<std::mutex> lock(runtime_lock_);
  logger_.LogInfo() << "zhuang 3";

  // Re-init prevention
  if (!runtime_done_) {
    logger_.LogError()
        << "Runtime re-init prevention: Runtime has been already initialized. Runtime initialization is only "
           "possible in re-configuration / start-up phase.";
    return;
  }
  logger_.LogInfo() << "zhuang 4";

  InitializeThreadPools();
  logger_.LogInfo() << "zhuang 5";

  InitializeReactorThread();
  logger_.LogInfo() << "zhuang 6";

  // Instantiating the binding pool and initialize all bindings.
  binding_pool_.reset(new ara::com::internal::BindingPool());
  logger_.LogInfo() << "zhuang 7";
  InitializeBindings();
  logger_.LogInfo() << "zhuang 8";
  // Flag that signalizes that runtime and bindings have been initialized.
  runtime_done_ = false;
}

Runtime::~Runtime() {
  // Shut down bindings only, when runtime was initialized properly.
  if (!runtime_done_) {
    DeinitializeBindings();
  }

  reactor_done_ = true;
  reactor_.Unblock();
  if (reactor_thread_.joinable()) {
    reactor_thread_.join();
  }

  runtime_done_ = true;
}

ara::com::internal::BindingPool& Runtime::GetBindingPool() const {
  if (!binding_pool_) {
    throw std::runtime_error("Runtime not initialized");
  }
  return *binding_pool_;
}

bool Runtime::ProcessNextPacket() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;

  std::lock_guard<std::mutex> lock(runtime_lock_);
  bool processed{false};

  if (config_.GetProcessingMode() == configuration::RuntimeProcessingMode::kPolling) {
    processed = binding_pool_->ProcessNextPacket();
  } else {
    logger_.LogError() << "ProcessNextPacket is only available in full polling mode. Re-configuration mandatory.";
  }

  return processed;
}

void Runtime::InitializeThreadPools() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;

  // Initialize default thread-pool
  default_thread_pool_.Initialize(
      configuration::ThreadPoolConfig{configuration::ThreadPoolConfig::kDefaultThreadPoolId,
                                      configuration::ThreadPoolConfig::kDefaultThreadPoolNumThreads,
                                      configuration::ThreadPoolConfig::kDefaultThreadPoolMaxTask});

  // Initialize user-defined thread-pools
  const auto& thread_pool_configs = config_.GetThreadPools();
  thread_pools_.reserve(thread_pool_configs.size());
  for (const auto& thread_pool_config : thread_pool_configs) {
    logger_.LogDebug() << "Initializing thread-pool id: " << static_cast<std::uint16_t>(thread_pool_config.GetPoolId());

    // Create new thread-pool
    thread_pools_.emplace_back();

    // We can call back() here directly - static_vector
    // throws an exception if emplacing isn't possible.
    thread_pools_.back().first.Initialize(thread_pool_config);

    // Initialize thread-pool assignment
    const auto& assignment_configs = thread_pool_config.GetAssignmentConfigs();
    thread_pools_.back().second.reserve(assignment_configs.size());
    for (const auto& assignment_config : assignment_configs) {
      thread_pools_.back().second.emplace_back(assignment_config.GetName(), assignment_config.GetInstanceIdentifier());
    }
  }
}

void Runtime::InitializeReactorThread() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;

  if (config_.GetProcessingMode() == configuration::RuntimeProcessingMode::kSingleThreaded) {
    // Instantiate reactor thread in single threaded mode
    reactor_thread_ = (std::thread([this]() {
      while (!this->reactor_done_) {
        const std::pair<bool, struct timeval> expiry = timer_manager_.GetNextExpiry();
        if (expiry.first) {
          this->reactor_.HandleEvents(&expiry.second);
        } else {
          this->reactor_.HandleEvents(nullptr);
        }
        timer_manager_.HandleTimerExpiry();
      }
    }));
  } else {
    // Do not instantiate a reactor thread in polling mode.
    // User will control event handling by ProcessNextPacket() API
    reactor_done_ = true;
  }
}

}  // namespace com
}  // namespace ara

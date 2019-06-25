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
/**        \file  binding_pool.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/internal/binding_pool.h"
#include <algorithm>

namespace ara {
namespace com {
namespace internal {

BindingPool::BindingPool() : logger_(ara::log::CreateLogger("BindingPool", "")) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
}

void BindingPool::StartFindService(const ara::com::FindServiceHandle& handle,
                                   ara::com::internal::ServiceUpdateObserver& observer) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  for (auto& binding : binding_interfaces_) {
    binding.StartFindService(handle, observer);
  }
}

void BindingPool::StopFindService(const ara::com::FindServiceHandle& handle) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  for (auto& binding : binding_interfaces_) {
    binding.StopFindService(handle);
  }
}

ara::com::ServiceHandleContainer<ara::com::HandleType> BindingPool::FindService(
    ara::com::internal::ProxyId proxy_id, const ara::com::InstanceIdentifier& instance_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  ara::com::ServiceHandleContainer<ara::com::HandleType> service_instances;  // contains all instances of all bindings
  for (auto& binding : binding_interfaces_) {
    auto binding_instances = binding.FindService(proxy_id, instance_id);  // instance of one binding returned
    service_instances.insert(std::end(service_instances), std::begin(binding_instances), std::end(binding_instances));
  }
  return service_instances;
}

BindingPool::SkeletonImplContainer BindingPool::OfferService(ara::com::internal::SkeletonId skeleton_id,
                                                             const ara::com::InstanceIdentifier& instance_id,
                                                             ara::com::internal::SkeletonInterface& skeleton) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;

  SkeletonImplContainer impl_collection;
  for (auto& binding : binding_interfaces_) {
    SkeletonImplPtr impl_binding = binding.OfferService(skeleton_id, instance_id, skeleton);
    impl_collection.push_back(std::move(impl_binding));
  }

  return impl_collection;
}

void BindingPool::StopOfferService(ara::com::internal::SkeletonId skeleton_id,
                                   const ara::com::InstanceIdentifier& instance_id) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  ara::com::ServiceHandleContainer<ara::com::HandleType> service_instances;  // contains all instances of all bindings
  for (auto& binding : binding_interfaces_) {
    binding.StopOfferService(skeleton_id, instance_id);
  }
}

void BindingPool::AddBinding(BindingInterface& binding_interface) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  binding_interfaces_.insert(binding_interfaces_.end(), binding_interface);
}

void BindingPool::RemoveBinding(BindingInterface& binding_interface) {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  binding_interface.EraseFromList();
}

bool BindingPool::ProcessNextPacket() {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  bool packet_processed{false};
  for (auto& binding : binding_interfaces_) {
    packet_processed |= binding.ProcessNextPacket();
  }
  return packet_processed;
}

}  // namespace internal
}  // namespace com
}  // namespace ara

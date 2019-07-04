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
/**        \file  binding_pool.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_BINDING_POOL_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_BINDING_POOL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include "ara/com/configuration/configuration.h"
#include "ara/com/internal/binding_interface.h"
#include "ara/com/internal/skeleton_interface.h"
#include "ara/com/internal/types.h"
#include "ara/log/logging.hpp"
#include "vac/container/intrusive_list.h"

namespace ara {
namespace com {

namespace internal {

/** \brief Maintains a set of bindings
 *  The binding pool creates and maintains protocol-specific bindings which in turn
 *  are used to instantiate proxies/skeletons that find to their respective interfaces.
 *
 *  Implementation details:
 *  BindingPool contains a static data member which is a list of registered bindings.
 *  All bindings that are statically compiled in will automatically register to this
 *  list. Note that bindings will not be instantiated upon registration. Only once
 *  the binding pool is created all bindings will be instantiated.
 *  To dynamically bind and create bindings (say by loading a shared object dynamically
 *  after main() has been entered)
 */
class BindingPool {
 public:
  /// Typedef for the container of bindings
  using BindingContainer = vac::container::IntrusiveList<BindingInterface>;

  /// Pointer type of unique ownership for a skeleton implementation interface
  /// created on an OfferService.
  using SkeletonImplPtr = std::unique_ptr<ara::com::internal::SkeletonImplInterface>;

  /// Typedef for a container of skeleton binding implementations after an OfferService
  using SkeletonImplContainer = std::vector<SkeletonImplPtr>;

  /**
   * \brief Constructor initializes the logging.
   */
  BindingPool();

  /**
   * \brief Register an additional Binding with the BindingPool.
   *
   * \param binding_interface A reference to an implementation of binding_interface.
   */
  void AddBinding(BindingInterface& binding_interface);

  /**
   * \brief Deregister an additional Binding with the BindingPool.
   *
   * \param binding_interface A reference to an implementation of binding_interface.
   */
  void RemoveBinding(BindingInterface& binding_interface);

  /**
   * \brief Loops through all bindings to start an asynchronous FindService.
   *
   * \param handle A FindServiceHandle created by an observer of the ServiceProxy.
   * \param observer A reference to the observer object of one ServiceProxy, which is called on service updates.
   */
  void StartFindService(const ara::com::FindServiceHandle& handle, ara::com::internal::ServiceUpdateObserver& observer);

  /**
   * \brief Stops an asynchronous FindService job.
   *
   * \param handle The FindServiceHandle is forwarded to the binding, to identify which action to stop.
   */
  void StopFindService(const ara::com::FindServiceHandle& handle);

  /**
   * \brief Loops through all bindings to find service instances.
   *
   * \param proxy_id A unique proxy identifier provided by the static proxy function.
   * This parameter is an abstract ara::com proxy identifier type and is independent from the SOME/IP service
   * identifier.
   * \param instance_id A instance identifier provided by the static proxy function.
   * This parameter is an abstract, ara::com-specific instance identifier, mapped to a concrete binding-specific
   * instance identifier.
   *
   * \return the service instances collected as handle types in a container.
   */
  ara::com::ServiceHandleContainer<ara::com::HandleType> FindService(ara::com::internal::ProxyId proxy_id,
                                                                     const ara::com::InstanceIdentifier& instance_id);

  /**
   * \brief Loops through all bindings and offer a service
   *
   * \param skeleton_id A unique skeleton identifier provided by skeleton
   * \param instance_id A instance identifier provided by the skeleton
   * \param skeleton The related skeleton frontend
   *
   * \return A collection of binding-related skeleton implementations for event notification and
   * removal on a stopped offer.
   */
  SkeletonImplContainer OfferService(ara::com::internal::SkeletonId skeleton_id,
                                     const ara::com::InstanceIdentifier& instance_id,
                                     ara::com::internal::SkeletonInterface& skeleton);

  /**
   * \brief Loops through all bindings and stop offering a service
   *
   * \param skeleton_id A unique skeleton identifier not offered anymore
   * \param instance_id A instance identifier not offered anymore
   */
  void StopOfferService(ara::com::internal::SkeletonId skeleton_id, const ara::com::InstanceIdentifier& instance_id);

  /**
   * \brief Invokes a processing of an incoming packet on all registered bindings.
   *
   * \return true: one of the bindings processed a pending packet - false: none of the bindings had a packet to process.
   */
  bool ProcessNextPacket();

 private:
  /// Container for all the registered bindings
  BindingContainer binding_interfaces_;

  /// Reference to the logger
  ara::log::Logger& logger_;
};

}  // namespace internal
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_BINDING_POOL_H_

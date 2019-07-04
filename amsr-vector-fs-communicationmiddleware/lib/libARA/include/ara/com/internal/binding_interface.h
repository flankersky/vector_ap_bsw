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
/**        \file  binding_interface.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_BINDING_INTERFACE_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_BINDING_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "ara/com/internal/service_update_observer.h"
#include "ara/com/internal/skeleton_impl_interface.h"
#include "ara/com/internal/skeleton_interface.h"

#include "ara/com/types.h"
#include "vac/container/intrusive_list.h"

namespace ara {
namespace com {
namespace internal {

/**
 * \brief Base class of all binding builders
 */
class BindingInterface : public vac::container::IntrusiveListNode<BindingInterface> {
 public:
  /**
   * \brief Constructor
   */
  BindingInterface() : vac::container::IntrusiveListNode<BindingInterface>() {}

  /**
   * \brief Destructor
   */
  virtual ~BindingInterface() = default;

  /**
   * \brief Execute a synchronous FindService call.
   *
   * \param proxy_id The ara::com::internal::ProxyId to search for
   * \param instance_id The ara::com::InstanceIdentifier for which to find a service
   *
   * \returns HandleTypes for the currently known services
   */
  virtual ServiceHandleContainer<HandleType> FindService(ara::com::internal::ProxyId proxy_id,
                                                         const ara::com::InstanceIdentifier& instance_id) = 0;

  /**
   *  \brief Triggers asynchronous SOME/IP service lookup according to the ARA::COM specification.
   *  Implemented and overridden in a concrete binding, which is derived from BindingInterface.
   *
   *  \param handle The FindService handle created from the observer.
   *  \param observer The observer of a specific ServiceProxy.
   */
  virtual void StartFindService(const ara::com::FindServiceHandle& handle,
                                ara::com::internal::ServiceUpdateObserver& observer) = 0;
  /**
   * \brief Stop asynchronous service discovery.
   *
   * \param handle The handle to identify, which asynchronous action to stop.
   */
  virtual void StopFindService(const FindServiceHandle& handle) = 0;

  /**
   * \brief Offer a service over this binding.
   *
   * \param skeleton_id the ara::com::internal::SkeletonId of the service to offer
   * \param instance_id The ara::com::InstanceIdentifier for which to offer a service
   * \param skeleton The related frontend skeleton
   *
   * \return A SkeletonImplInterface for the given Service, configured for the given instance_identifier or nullptr, if
   * no such SkeletonImplInterface is available from the binding.
   */
  virtual std::unique_ptr<SkeletonImplInterface> OfferService(ara::com::internal::SkeletonId skeleton_id,
                                                              const ara::com::InstanceIdentifier& instance_id,
                                                              ara::com::internal::SkeletonInterface& skeleton) = 0;

  /**
   * \brief Cancel the offering of a specific service.
   *
   * \param skeleton_id the ara::com::internal::SkeletonId to search for
   * \param instance_id The ara::com::InstanceIdentifier for which to find a service
   */
  virtual void StopOfferService(ara::com::internal::SkeletonId skeleton_id,
                                const ara::com::InstanceIdentifier& instance_id) = 0;

  /**
   * \brief Invoke ProcessNextPacket call on this binding in polling mode.
   *
   * \return true if a packet within this binding was pending and it has been processed; false if there was no packet
   * pending.
   */
  virtual bool ProcessNextPacket() = 0;
};

}  // namespace internal
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_BINDING_INTERFACE_H_

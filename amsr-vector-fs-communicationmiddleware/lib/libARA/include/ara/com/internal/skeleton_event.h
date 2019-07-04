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
/**        \file  skeleton_event.h
 *        \brief  TODO
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_SKELETON_EVENT_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_SKELETON_EVENT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/internal/types.h"
#include "vac/language/cpp14_backport.h"

namespace ara {
namespace com {
namespace internal {

/**
 * \brief Generic SkeletonEvent class which unifies common behavior.
 *
 * \tparam Skeleton The type of the concrete skeleton frontend
 * \tparam EventDataType The data type that is sent from the skeleton to the client.
 */
template <typename Skeleton, typename EventDataType, typename Interface,
          void (Interface::*SendEvent)(const EventDataType&)>
class SkeletonEvent {
 public:
  /**
   * \brief Shortcut for the events data type.
   */
  using SampleType = EventDataType;

  /**
   * \brief Default constructor must set the reference to the skeleton.
   *
   * \param skeleton A reference to the skeleton object.
   */
  explicit SkeletonEvent(Skeleton* skeleton) : skeleton_(skeleton) {}

  /**
   * \brief Sending event data.
   *
   * \param data A reference to event data allocated by the service application developer.
   */
  void Send(const SampleType& data) {
    // Get the binding implementations (backends) that are retreived on an OfferService of the skeleton
    // object.
    typename Skeleton::SkeletonImplInterfaceCollection& interfaces = skeleton_->GetImplementationInterfaces();
    for (auto& interface : interfaces) {
      // Sends an event to all the binding implementations.
      // Each binding implementation of a concrete service must provide the definition for
      // the Send(const SampleType& data).
      (interface.get()->*SendEvent)(data);
    }
  }

  /**
   * \brief Allocate event data of unique ownership for sending out. The implementation
   * of method Allocate is provided by the binding.
   *
   * \uptrace SWS_CM_00163
   * \return Requested memory provided by the middleware.
   */
  ara::com::SampleAllocateePtr<SampleType> Allocate() { return vac::language::make_unique<SampleType>(); }

  /**
   * \brief Second variant of the Send method, which requires the call of method Allocate
   * before.
   *
   * \uptrace SWS_CM_00163
   * \param data A pointer of unique ownership to the data provided by the concrete
   * binding implementation.
   */
  void Send(ara::com::SampleAllocateePtr<SampleType> data) { Send(*data); }

  /// A reference to the skeleton instance to call the binding-specific interfaces, when a send is called
  /// or the binding implementation of the skeleton should allocate memory of type SampleType.
  Skeleton* skeleton_;
};

}  // namespace internal
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_SKELETON_EVENT_H_

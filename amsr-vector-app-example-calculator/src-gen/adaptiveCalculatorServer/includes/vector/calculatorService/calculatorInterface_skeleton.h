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
/**        \file  calculatorInterface_skeleton.h
 *        \brief  Skeleton for service 'calculatorInterface'.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADAPTIVECALCULATORSERVER_INCLUDES_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_SKELETON_H_
#define ADAPTIVECALCULATORSERVER_INCLUDES_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_SKELETON_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <vector>
#include "ara/com/future.h"
#include "ara/com/internal/skeleton_base.h"
#include "ara/com/internal/skeleton_event.h"
#include "ara/com/internal/skeleton_field.h"
#include "ara/com/internal/skeleton_interface.h"
#include "ara/com/internal/skeleton_request_handling.h"
#include "vector/calculatorService/calculatorInterface_skeleton_impl_interface.h"

namespace vector {
namespace calculatorService {

/** Forward declaration for inserting as a type into the template class SkeletonEvent */
class CalculatorInterfaceSkeleton;

namespace skeleton {

namespace methods {}  // namespace methods

namespace events {

/** \brief Type-alias for service event 'divisionByZero', that is part of the skeleton. */
using EventDivisionByZero =
    ara::com::internal::SkeletonEvent<CalculatorInterfaceSkeleton, vector::calculatorService::boolean,
                                      CalculatorInterfaceSkeletonImplInterface,
                                      &CalculatorInterfaceSkeletonImplInterface::SendEventDivisionByZero>;

}  // namespace events

namespace fields {
/**
 * \brief Type-alias for the notification of field 'divideResult'.
 */
using FieldNotifierdivideResult =
    ara::com::internal::SkeletonEvent<CalculatorInterfaceSkeleton, vector::calculatorService::uint32,
                                      CalculatorInterfaceSkeletonImplInterface,
                                      &CalculatorInterfaceSkeletonImplInterface::UpdateFieldDivideResult>;

/**
 * \brief Type-alias for the notifier configuration of field 'divideResult'.
 */
using FieldNotifierConfigdivideResult = ara::com::internal::HasNotifier<true, FieldNotifierdivideResult>;

/**
 * \brief Type-alias for the getter configuration of field 'divideResult'.
 */
using FieldGetterConfigdivideResult = ara::com::internal::HasGetter<true, void>;

/**
 * \brief Type-alias for the setter configuration of field 'divideResult'.
 */
using FieldSetterConfigdivideResult = ara::com::internal::HasSetter<true, void>;

/**
 * \brief Type-alias for the parameterization of field 'divideResult'.
 */
using FieldConfigdivideResult =
    ara::com::internal::FieldParameters<CalculatorInterfaceSkeleton, FieldNotifierConfigdivideResult,
                                        FieldGetterConfigdivideResult, FieldSetterConfigdivideResult>;

/** \brief Type-alias for service field 'divideResult', that is part of the skeleton. */
using FieldDivideResult = ara::com::internal::SkeletonField<vector::calculatorService::uint32, FieldConfigdivideResult>;

}  // namespace fields
}  // namespace skeleton

/**
 * \brief Skeleton interface class for the service 'calculatorInterface'.
 * \remark generated
 */
class CalculatorInterfaceSkeleton : public ara::com::internal::SkeletonBase,
                                    public ara::com::internal::SkeletonInterface {
 public:
  /**
   * \brief Pointer of unique ownership, which is instantiated for a skeleton implementation, after an OfferService call
   * from this object.
   */
  using SkeletonImplUniquePtr = std::unique_ptr<CalculatorInterfaceSkeletonImplInterface>;

  /** Type-alias for the collection of implementation interfaces, returned from an OfferService call */
  using SkeletonImplInterfaceCollection = std::vector<SkeletonImplUniquePtr>;

  /**
   * \brief Constructor of calculatorInterfaceSkeleton.
   * \param instance The service instance ID provided by this skeleton.
   * \param mode The method processing mode used by this skeleton. Default: Event-driven processing
   */
  explicit CalculatorInterfaceSkeleton(
      ara::com::InstanceIdentifier instance,
      ara::com::MethodCallProcessingMode mode = ara::com::MethodCallProcessingMode::kEvent);

  /**
   * \brief Default destructor
   */
  virtual ~CalculatorInterfaceSkeleton() = default;

  /**
   * \brief Get the unique skeleton type identifier. Unique for the service 'calculatorInterface'.
   * \return Unique skeleton ID
   */
  constexpr static ara::com::internal::SkeletonId GetSkeletonIdStatic() { return &skeleton_id_; }

  /**
   * \brief Get the unique skeleton type identifier. Unique for the service 'calculatorInterface'.
   * \return Unique skeleton ID
   */
  ara::com::internal::SkeletonId GetSkeletonId() const override { return GetSkeletonIdStatic(); }

  /**
   * \brief Offer the service 'calculatorInterface'.
   */
  void OfferService();

  /**
   * \brief Stop offering the service 'calculatorInterface'.
   */
  void StopOfferService();

  /**
   * \brief Getter for all binding-specific implementations provided after a OfferService call.
   * \return The collection of implementation interfaces to send the event via multiple bindings.
   */
  SkeletonImplInterfaceCollection& GetImplementationInterfaces() { return impl_interfaces_; }

  /* ---- Methods -------------------------------------------------------------------------------------------------- */

  /**
   * \brief Prototype of skeleton service method 'subtract'.
   * \param arg1 IN parameter of type uint32
   * \param arg2 IN parameter of type uint32
   * \return ara::com::Future with output struct arguments
   *         result OUT parameter of type uint32
   */
  virtual ara::com::Future<skeleton::methods::Subtract::Output> subtract(const uint32& arg1, const uint32& arg2) = 0;

  /**
   * \brief Prototype of skeleton service method 'add'.
   * \param arg1 IN parameter of type uint32
   * \param arg2 IN parameter of type uint32
   * \return ara::com::Future with output struct arguments
   *         result OUT parameter of type uint32
   */
  virtual ara::com::Future<skeleton::methods::Add::Output> add(const uint32& arg1, const uint32& arg2) = 0;

  /**
   * \brief Prototype of skeleton service method 'divide'.
   * \param divident IN parameter of type uint32
   * \param divisor IN parameter of type uint32
   * \return ara::com::Future with output struct arguments
   *         result OUT parameter of type uint32
   */
  virtual ara::com::Future<skeleton::methods::Divide::Output> divide(const uint32& divident, const uint32& divisor) = 0;

  /* ---- Events --------------------------------------------------------------------------------------------------- */

  /**
   * \brief Prototype of skeleton event 'divisionByZero'.
   * \param data Event sample of type boolean (boolean has the value space required to support the mathematical concept
   * of binary-valued logic: {true, false}.)
   * \return A skeleton event object supporting event sample and subscription
   */
  skeleton::events::EventDivisionByZero divisionByZero;

  /* ---- Fields --------------------------------------------------------------------------------------------------- */

  /**
   * \brief Prototype of skeleton field 'divideResult'.
   * \param data Field data of type uint32 (uint32 represents integers with a minimum value of 0 and a maximum value of
   * 4294967295. The order-relation on uint32 is: x < y if y - x is positive. uint32 has a lexical representation
   * consisting of a finite-length sequence of decimal digits (#x30-#x39). For example: 1, 0, 12234567, 104400.)
   * \return A skeleton event object supporting field operations Get, Set, Update and handler registration.
   */
  skeleton::fields::FieldDivideResult divideResult;

 private:
  /** Skeleton type identifier. Address of this member uniquely identifies the proxy type */
  static ara::com::internal::SkeletonIdObject skeleton_id_;

  /**
   * \brief Each skeleton holds a collection of implementation interfaces after an OfferService call
   * from this instance. The binding pool will then return a collection of all implementation interfaces.
   */
  SkeletonImplInterfaceCollection impl_interfaces_;
};  // class CalculatorInterfaceSkeleton

}  // namespace calculatorService
}  // namespace vector

#endif  // ADAPTIVECALCULATORSERVER_INCLUDES_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_SKELETON_H_

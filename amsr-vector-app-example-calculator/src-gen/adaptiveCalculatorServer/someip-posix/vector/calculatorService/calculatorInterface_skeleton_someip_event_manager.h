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
/**        \file  calculatorInterface_skeleton_someip_event_manager.h
 *        \brief  SOME/IP skeleton event handling for events and field notifications of service 'calculatorInterface'
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADAPTIVECALCULATORSERVER_SOMEIP_POSIX_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_SKELETON_SOMEIP_EVENT_MANAGER_H_
#define ADAPTIVECALCULATORSERVER_SOMEIP_POSIX_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_SKELETON_SOMEIP_EVENT_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara-someip-posix/e2e_marshalling.h"
#include "someip-posix-common/someip/marshalling.h"
#include "vector/calculatorService/calculatorInterface.h"
#include "vector/calculatorService/calculatorInterface_skeleton.h"

namespace vector {
namespace calculatorService {

// Forward-declaration for back-reference
class CalculatorInterfaceSkeletonSomeIpBinding;

/* ---- Event 'divisionByZero' ------------------------------------------- */
/**
 * \brief SOME/IP Skeleton event manager for event 'divisionByZero'.
 * \details Handles event and field notification serialization.
 * \remark generated
 */
class CalculatorInterfaceSkeletonSomeIpEventManagerDivisionByZero {
 public:
  /**
   * \brief Constructor
   * \param skeleton_binding Related SOME/IP skeleton binding to be used
   */
  explicit CalculatorInterfaceSkeletonSomeIpEventManagerDivisionByZero(
      CalculatorInterfaceSkeletonSomeIpBinding& skeleton_binding);

  /**
   * \brief Send a event sample of skeleton event 'divisionByZero'.
   * \param data Event sample of type boolean (boolean has the value space required to support the mathematical concept
   * of binary-valued logic: {true, false}.)
   */
  void Send(const vector::calculatorService::boolean& data);

 private:
  /** Skeleton binding for event transmission */
  CalculatorInterfaceSkeletonSomeIpBinding& skeleton_binding_;
};

/* ---- Field notifier 'divideResult' ------------------------------------------- */
/**
 * \brief SOME/IP Skeleton event manager for event 'divideResult'.
 * \details Handles event and field notification serialization.
 * \remark generated
 */
class CalculatorInterfaceSkeletonSomeIpFieldNotifierDivideResult {
 public:
  /**
   * \brief Constructor
   * \param skeleton_binding Related SOME/IP skeleton binding to be used
   */
  explicit CalculatorInterfaceSkeletonSomeIpFieldNotifierDivideResult(
      CalculatorInterfaceSkeletonSomeIpBinding& skeleton_binding);

  /**
   * \brief Send a event sample of skeleton event 'divideResult'.
   * \param data Event sample of type uint32 (uint32 represents integers with a minimum value of 0 and a maximum value
   * of 4294967295. The order-relation on uint32 is: x < y if y - x is positive. uint32 has a lexical representation
   * consisting of a finite-length sequence of decimal digits (#x30-#x39). For example: 1, 0, 12234567, 104400.)
   */
  void Send(const vector::calculatorService::uint32& data);

 private:
  /** Skeleton binding for event transmission */
  CalculatorInterfaceSkeletonSomeIpBinding& skeleton_binding_;
};

}  // namespace calculatorService
}  // namespace vector

#endif  // ADAPTIVECALCULATORSERVER_SOMEIP_POSIX_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_SKELETON_SOMEIP_EVENT_MANAGER_H_

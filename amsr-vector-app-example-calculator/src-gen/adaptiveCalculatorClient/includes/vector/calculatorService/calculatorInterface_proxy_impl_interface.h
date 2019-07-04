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
/**        \file  calculatorInterface_proxy_impl_interface.h
 *        \brief  Proxy implementation interface of service 'calculatorInterface'.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADAPTIVECALCULATORCLIENT_INCLUDES_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_PROXY_IMPL_INTERFACE_H_
#define ADAPTIVECALCULATORCLIENT_INCLUDES_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_PROXY_IMPL_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/future.h"
#include "ara/com/internal/proxy_impl_event.h"
#include "vector/calculatorService/calculatorInterface.h"

namespace vector {
namespace calculatorService {

/**
 * \brief Proxy implementation interface for the Service 'calculatorInterface'
 * \remark generated
 */
class CalculatorInterfaceProxyImplInterface : public ara::com::internal::ProxyImplInterface {
 public:
  /**
   * \brief Default destructor
   */
  virtual ~CalculatorInterfaceProxyImplInterface() = default;

  /* ---- Methods -------------------------------------------------------------------------------------------------- */

  /**
   * \brief Pure virtual implementation interface of service method 'subtract'.
   * \param arg1 IN parameter of type uint32
   * \param arg2 IN parameter of type uint32
   * \return ara::com::Future with output struct arguments
   *         result OUT parameter of type uint32
   */
  virtual ara::com::Future<proxy::methods::Subtract::Output> MethodSubtract(uint32& arg1, uint32& arg2) = 0;

  /**
   * \brief Pure virtual implementation interface of service method 'add'.
   * \param arg1 IN parameter of type uint32
   * \param arg2 IN parameter of type uint32
   * \return ara::com::Future with output struct arguments
   *         result OUT parameter of type uint32
   */
  virtual ara::com::Future<proxy::methods::Add::Output> MethodAdd(uint32& arg1, uint32& arg2) = 0;

  /**
   * \brief Pure virtual implementation interface of service method 'divide'.
   * \param divident IN parameter of type uint32
   * \param divisor IN parameter of type uint32
   * \return ara::com::Future with output struct arguments
   *         result OUT parameter of type uint32
   */
  virtual ara::com::Future<proxy::methods::Divide::Output> MethodDivide(uint32& divident, uint32& divisor) = 0;

  /* ---- Events --------------------------------------------------------------------------------------------------- */

  /**
   * \brief Get the event manager object for the service event 'divisionByZero'
   * \return A proxy event object supporting event sample and subscription.
   */
  virtual ara::com::internal::ProxyImplEvent<vector::calculatorService::boolean>& GetEventManagerDivisionByZero() = 0;

  /* ---- Fields --------------------------------------------------------------------------------------------------- */

  /* ---- Field 'divideResult' ---- */
  /**
   * \brief Get the field notifier object for the service field 'divideResult'
   * \return A proxy event object supporting event sample and subscription.
   */
  virtual ara::com::internal::ProxyImplEvent<vector::calculatorService::uint32>& GetFieldNotifierDivideResult() = 0;

  /**
   * \brief Getter for the field 'divideResult'
   */
  virtual ara::com::Future<proxy::fields::DivideResult::Output> FieldDivideResultMethodGet() = 0;

  /**
   * \brief Setter for the field 'divideResult'
   * \param value Field value of type uint32 (uint32 represents integers with a minimum value of 0 and a maximum value
   * of 4294967295. The order-relation on uint32 is: x < y if y - x is positive. uint32 has a lexical representation
   * consisting of a finite-length sequence of decimal digits (#x30-#x39). For example: 1, 0, 12234567, 104400.)
   */
  virtual ara::com::Future<proxy::fields::DivideResult::Output> FieldDivideResultMethodSet(
      vector::calculatorService::uint32& value) = 0;
};

}  // namespace calculatorService
}  // namespace vector

#endif  // ADAPTIVECALCULATORCLIENT_INCLUDES_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_PROXY_IMPL_INTERFACE_H_

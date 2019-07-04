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
/**        \file  calculatorInterface_skeleton_impl_interface.h
 *        \brief  Skeleton implementation interface of service 'calculatorInterface'.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADAPTIVECALCULATORSERVER_INCLUDES_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_SKELETON_IMPL_INTERFACE_H_
#define ADAPTIVECALCULATORSERVER_INCLUDES_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_SKELETON_IMPL_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "ara/com/future.h"
#include "ara/com/instance_identifier.h"
#include "ara/com/internal/skeleton_impl_interface.h"
#include "vector/calculatorService/calculatorInterface.h"

namespace vector {
namespace calculatorService {

/**
 * \brief Skeleton implementation interface for the Service 'calculatorInterface'
 * \remark generated
 */
class CalculatorInterfaceSkeletonImplInterface : public ara::com::internal::SkeletonImplInterface {
 public:
  /**
   * \brief Default destructor
   */
  virtual ~CalculatorInterfaceSkeletonImplInterface() = default;

  /* ---- Events --------------------------------------------------------------------------------------------------- */

  /**
   * \brief Pure virtual interface for sending samples of service event 'divisionByZero'.
   * \details This method shall be defined in a concrete binding-specific skeleton implementation.
   * \param data Event sample of type boolean (boolean has the value space required to support the mathematical concept
   * of binary-valued logic: {true, false}.)
   */
  virtual void SendEventDivisionByZero(const vector::calculatorService::boolean& data) = 0;

  /* ---- Fields --------------------------------------------------------------------------------------------------- */

  /**
   * \brief Pure virtual interface for updating service field 'divideResult'.
   * \details This method shall be defined in a concrete binding-specific skeleton implementation.
   * \param data Field data oof type uint32 (uint32 represents integers with a minimum value of 0 and a maximum value of
   * 4294967295. The order-relation on uint32 is: x < y if y - x is positive. uint32 has a lexical representation
   * consisting of a finite-length sequence of decimal digits (#x30-#x39). For example: 1, 0, 12234567, 104400.)
   */
  virtual void UpdateFieldDivideResult(const vector::calculatorService::uint32& data) = 0;
};

}  // namespace calculatorService
}  // namespace vector

#endif  // ADAPTIVECALCULATORSERVER_INCLUDES_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_SKELETON_IMPL_INTERFACE_H_

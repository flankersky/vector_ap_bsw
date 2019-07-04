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
/**        \file  impl_type_boolean.h
 *        \brief
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADAPTIVECALCULATORSERVER_INCLUDES_VECTOR_CALCULATORSERVICE_IMPL_TYPE_BOOLEAN_H_
#define ADAPTIVECALCULATORSERVER_INCLUDES_VECTOR_CALCULATORSERVICE_IMPL_TYPE_BOOLEAN_H_

#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace vector {
namespace calculatorService {

/**
 * \brief Type boolean.
 * \details boolean has the value space required to support the mathematical concept of binary-valued logic: {true,
 * false}.
 * \remark generated
 *
 * IMPLEMENTATION-DATA-TYPE /AUTOSAR_Platform/ImplementationDataTypes/boolean
 * in the namespace of
 * SERVICE-INTERFACE /vector/calculatorService/calculatorInterface
 */
using boolean = bool;

}  // namespace calculatorService
}  // namespace vector

#endif  // ADAPTIVECALCULATORSERVER_INCLUDES_VECTOR_CALCULATORSERVICE_IMPL_TYPE_BOOLEAN_H_

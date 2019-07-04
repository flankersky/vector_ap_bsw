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
/**        \file  function.h
 *        \brief  Class for configuration handling
 *
 *      \details  A class to read in a configuration file to configure client and server
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_FUNCTION_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_FUNCTION_H_

#include <cstddef>
#include <functional>
#include <memory>

namespace ara {
namespace rest {

/** \brief A generalized function pointer
 *
 * All ara::rest internal components use Function instead of std::function.
 * The rationale is that resource-control in std::function is very low.
 * At a later time ara::rest may provide a custom replacment.
 */
template <typename T>
using Function = std::function<T>;

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_FUNCTION_H_

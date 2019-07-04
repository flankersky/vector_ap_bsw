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
/**        \file  pointer.h
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_POINTER_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_POINTER_H_

#include <memory>

namespace ara {
namespace rest {

/** \brief The equivalent if std::unique_ptr for ara::rest internal uses
 *
 * For all internal purposes this type shall be favored over std::unique_ptr for compatible
 * future replacement. A custom unique_ptr may be required to meet the requirements of a safe
 * memory concept.
 *
 * For now, Pointer is an alias for std::unique_ptr.
 *
 */
template <typename T>
using Pointer = std::unique_ptr<T>;

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_POINTER_H_

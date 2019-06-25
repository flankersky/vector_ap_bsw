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
/**        \file  writeaccessor.h
 *        \brief  The Persistency cluster shall provide an abstract class for write accessor operations.
 *
 *      \details  The Persistency cluster shall provide an abstract class for write accessor operations. This class
 *                shall be called WriteAccessor and shall be defined in the writeaccessor.h header.
 *
 *********************************************************************************************************************/

#ifndef LIB_PERSISTENCY_INCLUDE_ARA_PER_WRITEACCESSOR_H_
#define LIB_PERSISTENCY_INCLUDE_ARA_PER_WRITEACCESSOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <climits>
#include <string>

#include "vac/container/string_view.h"

#include "ara/per/basicoperations.h"
#include "ara/per/readwriteaccessor.h"

namespace ara {
namespace per {

/**
 * \brief  Class for write accessor operations directly using the implementation of ReadWriteAccessor
 *
 * \trace SWS_PER_00121
 * \trace SWS_PER_00130
 */
using WriteAccessor = ReadWriteAccessor;

/**
 * \brief The function writes newline to the file and calls flush().
 *        Please note: This function is not a member of the WriteAccessor class.
 * \param wa Object to be modified.
 * \return Returns the WriteAccessor object.
 *
 * \trace SWS_PER_00127
 */
inline WriteAccessor& endl(WriteAccessor& wa) {  // NOLINT
  wa.write("\n", sizeof("\n"));
  wa.flush();
  return wa;
}

/**
 * \brief The function flush calls flush().
 *        Please note: This function is not a member of the WriteAccessor class.
 * \param wa Object to be modified.
 * \return Returns the WriteAccessor object.
 *
 * \trace SWS_PER_00128
 */
inline WriteAccessor& flush(WriteAccessor& wa) {  // NOLINT
  wa.flush();
  return wa;
}

}  // namespace per
}  // namespace ara

#endif  // LIB_PERSISTENCY_INCLUDE_ARA_PER_WRITEACCESSOR_H_

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
/**        \file  basicoperations.cc
 *        \brief  Implementation of basicoperation functions which are common for all child classes
 *
 *      \details  Implementation of basicoperation functions which are common for all child classes
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/per/basicoperations.h"

namespace ara {
namespace per {

BasicOperations::OpenMode operator|(BasicOperations::OpenMode const& left, BasicOperations::OpenMode const& right) {
  return BasicOperations::OpenMode(static_cast<uint8_t>(left) | static_cast<uint8_t>(right));
}

BasicOperations::OpenMode operator&(BasicOperations::OpenMode const& left, BasicOperations::OpenMode const& right) {
  return BasicOperations::OpenMode(static_cast<uint8_t>(left) & static_cast<uint8_t>(right));
}

}  // namespace per
}  // namespace ara

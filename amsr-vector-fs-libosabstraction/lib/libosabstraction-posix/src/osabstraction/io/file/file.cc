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
/**        \file
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/file/file.h"

#include <unistd.h>
#include <system_error>

namespace osabstraction {
namespace io {
namespace file {

void File::Lock() {
  off_t offset = 0;
  int flag = lockf(handle_, F_LOCK, offset);

  if (flag == -1) {
    throw std::system_error(errno, std::generic_category());
  }
}
void File::Unlock() {
  off_t offset = 0;
  int flag = lockf(handle_, F_ULOCK, offset);

  if (flag == -1) {
    throw std::system_error(errno, std::generic_category());
  }
}

}  // namespace file
}  // namespace io
}  // namespace osabstraction

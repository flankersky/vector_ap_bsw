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
/**        \file  libosabstraction-interface/src/osabstraction/io/file_descriptor.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <fcntl.h>
#include <unistd.h>

#include <system_error>

#include "osabstraction/io/file_descriptor.h"

namespace osabstraction {
namespace io {

void FileDescriptor::Close() {
  if (handle_ != kInvalidFileDescriptor) {
    ::close(handle_);
    handle_ = kInvalidFileDescriptor;
  }
}

void FileDescriptor::SetBlockingMode(bool blocking) {
  int flags = ::fcntl(handle_, F_GETFL, 0);
  if (flags == -1) {
    throw std::system_error(errno, std::generic_category());
  }

  if (blocking) {
    flags &= ~O_NONBLOCK;
  } else {
    flags |= O_NONBLOCK;
  }

  int ret = ::fcntl(handle_, F_SETFL, flags);
  if (ret == -1) {
    throw std::system_error(errno, std::generic_category());
  }
}

}  // namespace io
}  // namespace osabstraction

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

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_FILE_FILE_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_FILE_FILE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/file_descriptor.h"

namespace osabstraction {
namespace io {
namespace file {

/**
 * \brief File wrapper
 */
class File : public FileDescriptor {
 public:
  /**
   * \brief Constructor to create a File object from a given filedescriptor
   */
  explicit File(int handle) : FileDescriptor(handle) {}
  /**
   * \brief Locks file (blocking mode)
   *
   * The file is automatically released during destruction.
   */
  void Lock();
  /**
   * \brief Locks file (blocking mode)
   *
   * The file is automatically released during destruction.
   */
  inline void lock() { Lock(); }
  /**
   * \brief Unlocks file manually
   */
  void Unlock();
  /**
   * \brief Unlocks file manually
   */
  inline void unlock() { Unlock(); }
};

}  // namespace file
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_FILE_FILE_H_

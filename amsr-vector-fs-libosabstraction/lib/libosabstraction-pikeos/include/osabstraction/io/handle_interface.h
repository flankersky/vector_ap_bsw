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
 *        \brief  Abstract IO handle interface.
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_HANDLE_INTERFACE_H_
#define LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_HANDLE_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/time.h>  // struct timeval
#include <functional>

namespace osabstraction {
namespace io {

/**
 * \brief Type definition of an IO handle.
 */
enum HandleType {
  kIpSocketHandle, /**< Handle is an IP Socket. */
  kIpcSocketHandle /**< Handle is an IPC Socket. */
};

/**
 * \brief The handle used for event handlers.
 */
class HandleInterface {
 public:
  /**
   * \brief Returns the type of the IO Handle.
   */
  virtual HandleType GetType() const = 0;

  /**
   * \brief Returns the raw file descriptor.
   */
  virtual int GetRawFiledescriptor() const = 0;

  /**
   * \brief Destructor.
   */
  virtual ~HandleInterface() {}
};

}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_HANDLE_INTERFACE_H_

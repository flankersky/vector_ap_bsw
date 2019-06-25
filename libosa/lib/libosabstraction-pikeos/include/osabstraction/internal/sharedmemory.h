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
 *        \brief  Interface to work with PikeOS shared memory objects.
 *
 *      \details  Shared memory objects can be accessed by multiple resource partitions.
 *                They are created by the PikeOS PSSW at boot time.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_INTERNAL_SHAREDMEMORY_H_
#define LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_INTERNAL_SHAREDMEMORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/types.h>

#include <memory>

namespace osabstraction {
namespace internal {

/**
 * \brief PikeOS shared memory object.
 */
class SharedMemory {
 public:
  /**
   * \brief Opens shared memory.
   *
   * \param[in] name  Name of shared memory object which shall be opened.
   */
  explicit SharedMemory(const char* name);

  /**
   * \brief Copy constructor.
   */
  SharedMemory(const SharedMemory& other) = default;

  /**
   * \brief Assignment operator.
   */
  SharedMemory& operator=(const SharedMemory& other) = default;

  /**
   * \brief Move constructor.
   */
  SharedMemory(SharedMemory&& other);

  /**
   * \brief Move assignment operator.
   */
  SharedMemory& operator=(SharedMemory&& other);

  /**
   * \brief Closes shared memory.
   */
  virtual ~SharedMemory();

  /**
   * \brief Loads a file into shared memory.
   */
  void LoadFile(const char* file_path);

 private:
  /**
   * \brief Pointer to the shared memory mapped into address space.
   */
  char* shm_;

  /**
   * \brief Size of the shared memory object.
   */
  off_t size_;
};

}  // namespace internal
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_INTERNAL_SHAREDMEMORY_H_

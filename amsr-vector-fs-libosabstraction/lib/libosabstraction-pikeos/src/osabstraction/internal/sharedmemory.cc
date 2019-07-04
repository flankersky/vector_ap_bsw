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
 *        \brief  Implementation of the PikeOS shared memory object interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/internal/sharedmemory.h"

#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdexcept>

#include "osabstraction/io/file_descriptor.h"

namespace osabstraction {
namespace internal {

SharedMemory::SharedMemory(const char* name) : shm_(nullptr), size_(0) {
  struct stat shm_status;

  /* #00 Open shared memory object with given name for writing. */
  io::FileDescriptor fd(shm_open(name, O_RDWR, 0));
  if (fd.GetHandle() == -1) {
    throw std::runtime_error(std::string("Could not open shared memory: ") + name);
  }

  /* #00 Get shared memory object size. */
  if (fstat(fd.GetHandle(), &shm_status) == -1) {
    throw std::runtime_error(std::string("Could not get size of shared memory object: ") + name);
  }
  size_ = shm_status.st_size;

  /* #00 Map shared memory object into address space.
   * Access shall allow writing, writing shall change the underlying object. */
  shm_ =
      reinterpret_cast<char*>(mmap(nullptr, shm_status.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd.GetHandle(), 0));
}

SharedMemory::SharedMemory(SharedMemory&& other) : shm_(std::move(other.shm_)), size_(std::move(other.size_)) {
  other.shm_ = nullptr;
}

SharedMemory& SharedMemory::operator=(SharedMemory&& other) {
  std::swap(other, *this);
  return *this;
}

SharedMemory::~SharedMemory() {
  /* #00 If shared memory object is mapped into address space, unmap it. */
  if (shm_ != nullptr) {
    if (munmap(shm_, size_) == -1) {
      throw std::runtime_error("Could not unmap shared memory object.");
    }
  }
}

void SharedMemory::LoadFile(const char* file_path) {
  /* #00 Open given file. */
  io::FileDescriptor fd(open(file_path, O_RDONLY));
  if (fd.GetHandle() == -1) {
    throw std::runtime_error(std::string("Could not open file for loading into shared memory object: ") + file_path);
  }

  /* #00 Check that file fits into shared memory object. */
  struct stat file_status;
  if (fstat(fd.GetHandle(), &file_status) == -1) {
    throw std::runtime_error(std::string("Could not get size of given file: ") + file_path);
  }
  if (file_status.st_size > size_) {
    throw std::runtime_error(std::string("Given file does not fit into shared memory object: ") + file_path);
  }

  /* #00 Read file into shared memory. */
  char* copy_ptr = shm_;
  ssize_t bytes_read = 0;
  while ((bytes_read = read(fd.GetHandle(), copy_ptr, size_)) > 0) {
    copy_ptr += bytes_read;
  }

  /* #00 If nothing copied, throw exception. */
  if ((copy_ptr - shm_) == 0) {
    throw std::runtime_error(std::string("Nothing copied into shared memory object: ") + file_path);
  }
}

}  // namespace internal
}  // namespace osabstraction

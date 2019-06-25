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
/**        \file  readwriteaccessor.cc
 *        \brief  Implementation of ReadWriteAccessor
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

#include "ara/per/common.h"
#include "ara/per/readwriteaccessor.h"

namespace ara {
namespace per {

ReadWriteAccessor::ReadWriteAccessor(std::string const& filename, BasicOperations::OpenMode const& openmode,
                                     vac::memory::SmartObjectPoolDeleterContext* ptr)
    : ReadAccessor(ptr), file_name_(), io_file_stream_(filename.data(), common::CalculateOpenModeMask(openmode)) {
  CopyFileName(vac::container::string_view(filename));
}

ReadWriteAccessor::ReadWriteAccessor(vac::container::string_view filename, BasicOperations::OpenMode const& openmode,
                                     vac::memory::SmartObjectPoolDeleterContext* ptr)
    : ReadAccessor(ptr), file_name_(), io_file_stream_(filename.data(), common::CalculateOpenModeMask(openmode)) {
  CopyFileName(filename);
}

void ReadWriteAccessor::fsync() {
  flush();
  auto fd = open(file_name_.data(), O_APPEND);
  fdatasync(fd);
}

std::size_t ReadWriteAccessor::write(char const* const s, std::size_t const n) {
  std::size_t result = 0;
  std::ios::streampos const old_pos = io_file_stream_.tellp();
  io_file_stream_.write(s, n);
  std::ios::streampos const new_pos = io_file_stream_.tellp();
  if (io_file_stream_.fail() || io_file_stream_.bad()) {
    result = 0;
  } else {
    /* check if new_pos and old_pos exceeds the maximum possible value of type std::size_t.
     * This check is necessary because the streamoff value can exceed the size of a std::size_t on 32 bit systems
     * (because streamoff is forced to be 64bit wide. Therefor this is a bug in the persistency sws.
     */
    if (std::make_unsigned<std::ios::streamoff>::type(new_pos) <= std::numeric_limits<std::size_t>::max() &&
        std::make_unsigned<std::ios::streamoff>::type(old_pos) <= std::numeric_limits<std::size_t>::max()) {
      result = static_cast<std::size_t>(new_pos) - static_cast<std::size_t>(old_pos);
    } else {
      throw std::runtime_error("Actual positions in stream exceeds size of sstd::size_t");
    }
  }
  return result;
}

void ReadWriteAccessor::flush() { io_file_stream_.flush(); }

ReadWriteAccessor& ReadWriteAccessor::operator<<(std::string const& string) {
  io_file_stream_ << string;
  return *this;
}

ReadWriteAccessor& ReadWriteAccessor::operator<<(ReadWriteAccessor& (*op)(ReadWriteAccessor&)) {
  (*op)(*this);
  return *this;
}

std::size_t ReadWriteAccessor::read(char* const s, std::size_t const n) {
  std::size_t result = 0;
  io_file_stream_.read(s, n);
  if (io_file_stream_.eof() && io_file_stream_.fail()) {
    result = io_file_stream_.gcount();
  } else {
    result = n;
  }
  return result;
}

ReadWriteAccessor& ReadWriteAccessor::getline(std::string& string, char const delim) {
  std::getline(io_file_stream_, string, delim);
  return *this;
}

ReadWriteAccessor& ReadWriteAccessor::operator>>(std::string& string) {
  io_file_stream_ >> string;
  return *this;
}

std::size_t ReadWriteAccessor::tell() {
  /* get current position from std api */
  std::ios::streamoff tell_return = io_file_stream_.tellg();

  /* check for negativ result of return the value */
  if (tell_return > -1) {
    /* check if tell_return exceeds the maximum possible value of type std::size_t.
     * This check is necessary because the streamoff value can exceed the size of a std::size_t on 32 bit systems
     * (because streamoff is forced to be 64bit wide. Therefor this is a bug in the persistency sws.
     */
    if (std::make_unsigned<std::ios::streamoff>::type(tell_return) <= std::numeric_limits<std::size_t>::max()) {
      return static_cast<std::size_t>(tell_return);
    } else {
      throw std::runtime_error("Actual position in stream exceeds size of std::size_t");
    }
  } else {
    throw std::runtime_error("Failed to get current position in file");
  }
}

void ReadWriteAccessor::seek(std::size_t const pos) {
  if (!eof()) {
    io_file_stream_.seekg(pos);
  }
}

void ReadWriteAccessor::seek(std::size_t const off, SeekDirection const dir) {
  std::ios_base::seekdir sd = common::ConvertSeekDirection(dir);
  io_file_stream_.seekg(off, sd);
}

bool ReadWriteAccessor::good() { return io_file_stream_.good(); }

bool ReadWriteAccessor::eof() { return io_file_stream_.eof(); }

bool ReadWriteAccessor::fail() { return io_file_stream_.fail(); }

bool ReadWriteAccessor::bad() { return io_file_stream_.bad(); }

bool ReadWriteAccessor::operator!() { return io_file_stream_.fail() || io_file_stream_.bad(); }

ReadWriteAccessor::operator bool() { return !(io_file_stream_.fail() || io_file_stream_.bad()); }

void ReadWriteAccessor::clear() { io_file_stream_.clear(); }

void ReadWriteAccessor::CopyFileName(vac::container::string_view filename) {
  if (filename.size() < file_name_.size()) {
    std::copy(filename.begin(), filename.end(), file_name_.begin());
    /* ensure null termination */
    file_name_[filename.size()] = '\0';

  } else {
    throw std::runtime_error("Filename exceeds size of PATH_MAX.");
  }
}

}  // namespace per
}  // namespace ara

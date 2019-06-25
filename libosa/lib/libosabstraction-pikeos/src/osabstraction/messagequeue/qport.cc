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
 *        \brief  PikeOS Queuing Port implementation.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/messagequeue/qport.h"
#include <fcntl.h>      // open()
#include <limits.h>     // PATH_MAX
#include <sys/qport.h>  // ioctl()
#include <sys/stat.h>   // fstat()
#include <unistd.h>     // close()
#include <algorithm>    // move
#include <array>        // array
#include <cstdio>       // snprintf()
#include <stdexcept>    // runtime_error
#include <utility>      // swap

namespace osabstraction {
namespace messagequeue {

/**
 * \brief Invalid qport descriptor.
 */
constexpr const int kInvalidDescriptor = -1;

/**
 * \brief Maximum qport path length.
 * According to POSIX IEEE Std 1003.1 2016 Edition, PATH_MAX includes the terminating null character.
 */
constexpr const int kMaxQPortPathLength = PATH_MAX;

QPort::QPort() : qport_descriptor_(kInvalidDescriptor), timeout_set_(false), timeout_value_(0) {}

QPort::QPort(const QPortSettings& settings)
    : qport_descriptor_(kInvalidDescriptor), timeout_set_(false), timeout_value_(0) {
  /* #00 Prepend qport prefix to id. */
  std::array<char, kMaxQPortPathLength> qport_path;
  int bytes_written = std::snprintf(qport_path.data(), qport_path.size(), "%s%s", "/qport", settings.id);
  if (bytes_written < 0 || bytes_written >= static_cast<int>(qport_path.size())) {
    throw std::runtime_error("Path to qport is too long.");
  }

  /* #00 Open the qport. */
  qport_descriptor_ = open(qport_path.data(), settings.flags);

  /* #00 If open failed, throw exception. */
  if (qport_descriptor_ == kInvalidDescriptor) {
    throw std::runtime_error("Opening qport failed.");
  }

  /* #00 If checks shall be performed: */
  if (settings.perform_checks) {
    /* #00 Check that the given attributes are compatible with the configured qport using fstat(). */
    struct stat qport_attributes;
    if (fstat(qport_descriptor_, &qport_attributes) == -1) {
      throw std::runtime_error("Could not fetch qport attributes for verification.");
    }

    if (qport_attributes.st_blksize != static_cast<int>(settings.msgsize)) {
      throw std::runtime_error("Configured message size of qport does not match to requested size.");
    }

    if (qport_attributes.st_blocks != settings.length) {
      throw std::runtime_error("Configured length of qport does not match to requested length.");
    }
  }
}

QPort::~QPort() {
  /* #00 If qport is open, close it. */
  if (qport_descriptor_ != kInvalidDescriptor) {
    close(qport_descriptor_);
  }
}

QPort::QPort(QPort&& other)
    : qport_descriptor_(std::move(other.qport_descriptor_)),
      timeout_set_(std::move(other.timeout_set_)),
      timeout_value_(std::move(other.timeout_value_)) {
  /* #00 Move qport descriptor. Set other descriptor to invalid. */
  other.qport_descriptor_ = kInvalidDescriptor;
}

QPort& QPort::operator=(QPort&& other) {
  /* #00 Swap qport descriptors. */
  std::swap(qport_descriptor_, other.qport_descriptor_);
  return *this;
}

bool QPort::IsOpen() const {
  /* #00 Return whether the qport descriptor is valid. */
  return qport_descriptor_ != kInvalidDescriptor;
}

int QPort::GetHandle() const { return qport_descriptor_; }

void QPort::SetTimeout(std::chrono::nanoseconds timeout) {
  /* #00 If given timeout does not match current timeout: */
  if (!timeout_set_ || (timeout_value_ != timeout)) {
    /* #00 Set timeout to given timeout by using ioctl(). */
    struct timespec tm;
    constexpr std::chrono::seconds sec(1);

    tm.tv_sec =
        std::chrono::duration_cast<std::chrono::nanoseconds>(timeout).count() / std::chrono::nanoseconds(sec).count();
    tm.tv_nsec = timeout.count() % std::chrono::nanoseconds(sec).count();

    if (ioctl(qport_descriptor_, QPORT_STOUT, &tm) == -1) {
      std::runtime_error("Could not set timeout for qport.");
    }
    timeout_set_ = true;
    timeout_value_ = timeout;
  }
}

void QPort::ClearTimeout() {
  /* #00 If timeout is set: */
  if (timeout_set_) {
    /* #00 Set timeout to infinite by using nullptr in ioctl(). */
    if (ioctl(qport_descriptor_, QPORT_STOUT, nullptr) == -1) {
      std::runtime_error("Could not set infinite timeout for qport.");
    }
    timeout_set_ = false;
  }
}

}  // namespace messagequeue
}  // namespace osabstraction

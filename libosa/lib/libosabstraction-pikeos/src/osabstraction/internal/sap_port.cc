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
 *        \brief  Implementation of the IPC socket interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/internal/sap_port.h"
#include <fcntl.h>  // open()
#include <sys/qport.h>
#include <unistd.h>  // close()
#include <cassert>
#include <cstring>
#include <iostream>
#include <system_error>
#include "ipc_driver.h"  // SAP_IPC_*

namespace osabstraction {
namespace internal {

/**
 * \brief      The possible configuration option bits for the SAP ports.
 */
enum OptionType {
  kBlock = 1,
};

/**
 * \brief      Invalid qport descriptor.
 */
constexpr const int kInvalidDescriptor = -1;

/**
 * \brief      Virtual filename for the stream stream.
 */
constexpr const char* kStreamQPortPath = "/qport/STREAM";

/**
 * \brief      Virtual filename for the datagram stream.
 */
constexpr const char* kDatagramQPortPath = "/qport/STREAM";

SapPort::SapPort() : handle_(kInvalidDescriptor), identifier_(0) {}

SapPort::SapPort(std::uint32_t identifier) : handle_(kInvalidDescriptor), identifier_(identifier) {}

void SapPort::Open(bool stream) {
  if (handle_ == kInvalidDescriptor) {
    handle_ = ::open((stream ? kStreamQPortPath : kDatagramQPortPath), O_WRONLY);
    if (handle_ == kInvalidDescriptor) {
      throw std::system_error(errno, std::generic_category(), "Opening IPC socket port failed");
    }
    if (!stream) {
      uint32_t flags = FLAGS_DGRAM;
      int ret = ::ioctl(handle_, SAP_IPC_SETFLAGS, &flags);
      if (ret < 0) {
        throw std::system_error(errno, std::generic_category(), "SapPort::Open() failed");
      }
    }
  }

  if (identifier_ == 0) {
    RequestId();
  }
}

void SapPort::Listen() {
  int ret = ::ioctl(handle_, SAP_IPC_LISTEN, nullptr);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category(), "SapPort::Listen() failed");
  }
}

std::pair<qport_sap_sockaddr_ipv4_t, std::uint32_t> SapPort::Accept() {
  accept_info_t accept_info;
  qport_sap_sockaddr_ipv4_t remote_addr;
  int ret = ::ioctl(handle_, SAP_IPC_ACCEPT, &accept_info);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category(), "SapPort::Accept() failed");
  }
  assert(accept_info.client_address.u.header->length <= sizeof(qport_sap_sockaddr_ipv4_t));
  std::memcpy(&remote_addr, &accept_info.client_address, accept_info.client_address.u.header->length);
  return std::make_pair(remote_addr, accept_info.connection_identifier);
}

void SapPort::Connect(const qport_sap_sockaddr_ipv4_t& addr) {
  sap_ipc_addr_t driver_addr;
  assert(addr.length <= sizeof(driver_addr));
  memcpy(&driver_addr, &addr, addr.length);
  int ret = ::ioctl(handle_, SAP_IPC_CONNECT, &driver_addr);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category(), "SapPort::Connect() failed");
  }
}

std::size_t SapPort::Peek(void* buffer, std::size_t size, const qport_sap_sockaddr_ipv4_t& remote_addr,
                          qport_sap_sockaddr_ipv4_t& remote_addr_out) {
  sap_ipc_addr_t driver_addr;
  sap_ipc_read_param rd;

  assert(remote_addr.length <= sizeof(driver_addr));
  std::memcpy(&driver_addr, &remote_addr, remote_addr.length);

  rd.in.buffer = buffer;
  rd.in.buffer_size = size;

  rd.in.address = &driver_addr;
  rd.in.address_size = sizeof(driver_addr);

  int ret = ::ioctl(handle_, SAP_IPC_PEEK, &rd);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category(), "SapPort::Peek() failed");
  }
  assert(driver_addr.u.header->length <= sizeof(qport_sap_sockaddr_ipv4_t));
  std::memcpy(&remote_addr_out, &driver_addr, driver_addr.u.header->length);

  return rd.out;
}

std::size_t SapPort::Receive(void* buffer, std::size_t size, const qport_sap_sockaddr_ipv4_t& remote_addr,
                             qport_sap_sockaddr_ipv4_t& remote_addr_out) {
  sap_ipc_addr_t driver_addr;
  sap_ipc_read_param rd;

  assert(remote_addr.length <= sizeof(driver_addr));
  std::memcpy(&driver_addr, &remote_addr, remote_addr.length);

  rd.in.buffer = buffer;
  rd.in.buffer_size = size;

  rd.in.address = &driver_addr;
  rd.in.address_size = sizeof(driver_addr);

  int ret = ::ioctl(handle_, SAP_IPC_READ, &rd);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category(), "SapPort::Receive() failed");
  }
  assert(driver_addr.u.header->length <= sizeof(qport_sap_sockaddr_ipv4_t));
  std::memcpy(&remote_addr_out, &driver_addr, driver_addr.u.header->length);

  return rd.out;
}

std::size_t SapPort::Send(const void* buffer, std::size_t size, const qport_sap_sockaddr_ipv4_t& remote_addr) {
  sap_ipc_addr_t driver_addr;
  sap_ipc_write_param_t wr;

  assert(remote_addr.length <= sizeof(driver_addr));
  std::memcpy(&driver_addr, &remote_addr, remote_addr.length);

  wr.in.buffer = buffer;
  wr.in.buffer_size = size;

  wr.in.address = &driver_addr;
  wr.in.address_size = sizeof(driver_addr);

  int ret = ::ioctl(handle_, SAP_IPC_WRITE, &wr);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category(), "SapPort::Send() failed");
  }
  return wr.out;
}

void SapPort::SetBlocking(bool block) {
  std::uint32_t cur_flags = GetFlags();
  cur_flags &= ~(OptionType::kBlock);
  cur_flags |= (block ? OptionType::kBlock : 0);
  SetFlags(cur_flags);
}

void SapPort::SetFlags(std::uint32_t flags) {
  int ret = ::ioctl(handle_, SAP_IPC_SETFLAGS, &flags);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category(), "SapPort::SetFlags() failed");
  }
}

std::uint32_t SapPort::GetFlags() {
  uint32_t result;
  int ret = ::ioctl(handle_, SAP_IPC_GETFLAGS, &result);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category(), "SapPort::GetFlags() failed");
  }
  return result;
}

std::pair<EventType, std::uint32_t> SapPort::Wait(std::uint32_t event_mask) {
  sap_ipc_wait_param_t wait;
  wait.in = event_mask;
  int ret = ::ioctl(handle_, SAP_IPC_WAIT, &wait);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category(), "SapPort::Wait() failed");
  }
  return std::make_pair(toEventType(wait.out.ret_events), static_cast<std::uint32_t>(wait.out.identifier));
}

std::uint32_t SapPort::GetId() { return identifier_; }

void SapPort::RequestId() {
  ipc_identifier_t result;
  int ret = ::ioctl(handle_, SAP_IPC_GETID, &result);
  if (ret < 0) {
    throw std::system_error(errno, std::generic_category(), "SapPort::RequestId() failed");
  }
  identifier_ = static_cast<std::uint32_t>(result);
}

void SapPort::Close() {
  if (handle_ != kInvalidDescriptor) {
    ::close(handle_);
  }
}

EventType SapPort::toEventType(std::uint32_t events) {
  switch (events) {
    case SELECT_EVENT_NONE:
      return EventType::kNone;
    case SELECT_EVENT_READ:
      return EventType::kRead;
    case SELECT_EVENT_WRITE:
      return EventType::kWrite;
    case SELECT_EVENT_CONNECT:
      return EventType::kConnect;
    case SELECT_EVENT_CLOSE:
      return EventType::kClose;
    default:
      throw std::runtime_error("SapPort: toEventType(): enexpected event value.");
  }
}

}  // namespace internal
}  // namespace osabstraction

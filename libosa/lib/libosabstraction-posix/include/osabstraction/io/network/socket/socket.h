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
 *        \brief  Socket
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_SOCKET_SOCKET_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_SOCKET_SOCKET_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/uio.h>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include "osabstraction/io/file_descriptor.h"
#include "osabstraction/io/network/address/ipv4_socket_address.h"
#include "osabstraction/io/network/address/ipv6_socket_address.h"
#include "osabstraction/io/network/address/socket_address.h"
#include "vac/container/array_view.h"

namespace osabstraction {
namespace io {
namespace network {
namespace socket {

/**
 * \brief Socket type.
 */
enum class SocketType {
  /**
   * \brief Connection-oriented socket type.
   */
  kStream,
  /**
   * \brief Connection-less socket type.
   */
  kDatagram
};

/**
 * \brief Abstract socket interface.
 */
class Socket : public FileDescriptor {
 public:
  /**
   * \brief Container for struct iovec arrays of arbitrary fixed length
   */
  using IovecContainer = vac::container::array_view<struct iovec>;

  /**
   * \brief Constructor to create a BSDSocket from a given filedescriptor
   */
  explicit Socket(int handle);

  /**
   * \brief Binds a socket to specified local address.
   *
   * \param local_address A local socket address to which the socket should be bound.
   */
  virtual void Bind(const address::SocketAddress& local_address);

  /**
   * \brief Returns the local socket address.
   *
   * \return The local socket address.
   */
  virtual address::SocketAddress GetLocalAddress() const;

  /**
   * \brief Returns the remote socket address.
   *
   * \return The remote socket address.
   */
  virtual address::SocketAddress GetRemoteAddress() const;

  /**
   * \brief Returns peek of received data.
   *
   * \param buffer A buffer where the received data is stored.
   * \param size The length of data to receive.
   * \return The length of the received data.
   */
  virtual std::size_t Peek(void* buffer, std::size_t size);

  /**
   * \brief Returns received data.
   *
   * \param buffer A buffer where the received data is stored.
   * \param size The length of data to receive.
   * \return The length of the received data.
   */
  virtual std::size_t Receive(void* buffer, std::size_t size);

  /**
   * \brief Returns peek of received data.
   *
   * \param buffer A buffer where the received data is stored.
   * \param size The length of data to receive.
   * \param remote_address The remote address of the socket that sent the received data.
   * \return The length of the received data.
   */
  virtual ssize_t Peek(void* buffer, std::size_t size, address::SocketAddress* remote_address);

  /**
   * \brief Returns received data.
   *
   * \param buffer A buffer where the received data is stored.
   * \param size The length of data to receive.
   * \param remote_address The remote address of the socket that sent the received data.
   * \return The length of the received data.
   */
  virtual ssize_t Receive(void* buffer, std::size_t size, address::SocketAddress* remote_address);

  /**
   * \brief Returns peek of received data.
   *
   * \param iovec A vector of buffers where the received data is stored.
   * \return The length of the received data.
   */
  virtual std::size_t Peek(const IovecContainer iovec);

  /**
   * \brief Returns received data.
   *
   * \param iovec A vector of buffers where the received data is stored.
   * \return The length of the received data.
   */
  virtual std::size_t Receive(const IovecContainer iovec);

  /**
   * \brief Returns peek of received data.
   *
   * \param iovec A vector of buffers where the received data is stored.
   * \param remote_address The remote address of the socket that sent the received data.
   * \return The length of the received data.
   */
  virtual std::size_t Peek(const IovecContainer iovec, address::SocketAddress* remote_address);

  /**
   * \brief Returns received data.
   *
   * \param iovec A vector of buffers where the received data is stored.
   * \param remote_address The remote address of the socket that sent the received data.
   * \return The length of the received data.
   */
  virtual std::size_t Receive(const IovecContainer iovec, address::SocketAddress* remote_address);

  /**
   * \brief Sends data to the connected remote peer.
   *
   * \param buffer A buffer where the data to be sent is stored.
   * \param size The length of data to send.
   * \return The length of data sent to the remote peer.
   */
  virtual std::size_t Send(const void* buffer, std::size_t size);

  /**
   * \brief Sends data to specified remote peer(s).
   *
   * \param buffer A buffer where the data to be sent is stored.
   * \param size The length of data to send.
   * \param remote_address The remote address of the socket to which the passed data should be sent.
   * \return The length of data sent to remote peer(s).
   */
  virtual std::size_t Send(const void* buffer, std::size_t size, const address::SocketAddress& remote_address);

  /**
   * \brief Sends data to the connected remote peer.
   *
   * \param iovec A vector of buffers where the data to be sent is stored.
   * \return The length of data sent to the remote peer.
   */
  virtual std::size_t Send(const IovecContainer iovec);

  /**
   * \brief Sends data to specified remote peer(s).
   *
   * \param iovec A vector of buffers where the data to be sent is stored.
   * \param remote_address The remote address of the socket to which the passed data should be sent.
   * \return The length of data sent to remote peer(s).
   */
  virtual std::size_t Send(const IovecContainer iovec, const address::SocketAddress& remote_address);

  /**
   * \brief Allows reusing of local addresses.
   *
   * \param reuse Allows reusing of local addresses if true and disallows otherwise.
   */
  virtual void SetReuseAddress(bool reuse);

  /**
   * \brief Set the Quality of Service (QoS) option for this socket object.
   *
   * \param priority The QoS priority to set.
   */
  void SetPriority(const int priority);

  /**
   * \brief Indicates if a socket is connected to another remote socket or not.
   *
   * \return true if the socket is connected and false otherwise.
   */
  virtual bool IsConnected();

  /**
   * \brief Converts a socket type to the corresponding Unix socket type.
   *
   * \param type A socket type.
   * \return The corresponding Unix socket type.
   */
  static int toUnixSocketType(SocketType type) {
    if (type == SocketType::kStream) {
      return SOCK_STREAM;
    } else {
      return SOCK_DGRAM;
    }
  }

  /**
   * \brief Converts a Unix socket type to the corresponding socket type.
   *
   * \param type A Unix socket type.
   * \return The corresponding socket type.
   */
  static SocketType toSocketType(int type) {
    if (type == SOCK_STREAM) {
      return SocketType::kStream;
    } else if (type == SOCK_DGRAM) {
      return SocketType::kDatagram;
    } else {
      throw std::invalid_argument("invalid Unix socket type");
    }
  }

  /**
   * \brief Returns the index of a network inteface identified by its IP address.
   *
   * \param address An IP address.
   * \return A pair of a boolean, that indicates the success, and an interface index.
   */
  static std::pair<bool, unsigned int> InterfaceAddressToIndex(const std::string& address);

  /**
   * \brief Calculates the total length of data contained in an I/O vector container.
   *
   * \param iovc An I/O vector container.
   * \return The total length of data contained in an I/O vector container.
   */
  static std::size_t GetDataLengthOfIovecContainer(const IovecContainer& iovc) {
    return std::accumulate(iovc.cbegin(), iovc.cend(), 0U,
                           [](std::size_t result, const struct iovec& i) { return result + i.iov_len; });
  }

 protected:
  Socket() = default;

  /**
   * \brief Contructor for internal use.
   *
   * \param address_family The address_family (UNIX, INET, INET6) for which to create a socket
   * \param socket_type the communication type (DGRAM, STREAM) used for communication
   */
  Socket(address::SocketAddressFamily address_family, SocketType socket_type);

  /**
   * \brief A socket type.
   */
  SocketType type_;

  /**
   * \brief Indicates whether the socket handle shall be unlinked.
   */
  bool shall_unlink_ = true;

 private:
  /**
   * \brief Returns received data.
   *
   * \param buffer A buffer where the received data is stored.
   * \param size The length of data to receive.
   * \param flags The flags to determine the exact behavior of Receive for example Peek
   * \return The length of the received data.
   */
  virtual std::size_t Receive(void* buffer, std::size_t size, int flags);

  /**
   * \brief Returns received data.
   *
   * \param buffer A buffer where the received data is stored.
   * \param size The length of data to receive.
   * \param remote_address The remote address of the socket that sent the received data.
   * \param flags The flags to determine the exact behavior of Receive for example Peek
   * \return The length of the received data.
   */
  virtual ssize_t Receive(void* buffer, std::size_t size, address::SocketAddress* remote_address, int flags);

  /**
   * \brief Returns received data.
   *
   * \param iovec A vector of buffers where the received data is stored.
   * \param flags The flags to determine the exact behavior of Receive for example Peek
   * \return The length of the received data.
   */
  virtual std::size_t Receive(const IovecContainer iovec, int flags);

  /**
   * \brief Returns received data.
   *
   * \param iovec A vector of buffers where the received data is stored.
   * \param remote_address The remote address of the socket that sent the received data.
   * \param flags The flags to determine the exact behavior of Receive for example Peek
   * \return The length of the received data.
   */
  virtual std::size_t Receive(const IovecContainer iovec, address::SocketAddress* remote_address, int flags);
};

}  // namespace socket
}  // namespace network
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_SOCKET_SOCKET_H_

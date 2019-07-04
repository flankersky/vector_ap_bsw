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
 *        \brief  Implementation of the socket interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_SOCKET_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_SOCKET_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include <utility>  // pair
#include "osabstraction/io/file_descriptor.h"
#include "osabstraction/io/net/address/socket_address.h"
#include "osabstraction/io/net/socket/socket_interface.h"
#include "vac/container/string_view.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

/**
 * \brief  Implementation of the socket interface.
 */
class Socket : public virtual SocketInterface, public osabstraction::io::FileDescriptor {
 public:
  /**
    * \brief Constructor to create a BSDSocket from a given filedescriptor.
    */
  explicit Socket(int handle);

  /**
   * \brief Binds a socket to specified local address.
   *
   * \param[in] local_address A local socket address to which the socket should be bound.
   */
  void Bind(const address::SocketAddress& local_address) override;

  /**
   * \brief Closes the socket.
   */
  void Close() override;

  /**
   * \brief Returns the local socket address.
   *
   * \return The local socket address.
   */
  address::SocketAddress GetLocalAddress() const override;

  /**
   * \brief Returns the remote socket address.
   *
   * \return The remote socket address.
   */
  address::SocketAddress GetRemoteAddress() const override;

  /**
   * \brief Returns peek of received data.
   *
   * \param[out]  buffer  A buffer where the received data is stored.
   * \param[in]   size    The length of data to receive.
   * \return The length of the received data.
   */
  std::size_t Peek(void* buffer, std::size_t size) override;

  /**
   * \brief Returns received data.
   *
   * \param[out]  buffer  A buffer where the received data is stored.
   * \param[in]   size    The length of data to receive.
   * \return The length of the received data.
   */
  std::size_t Receive(void* buffer, std::size_t size) override;

  /**
   * \brief Returns peek of received data.
   *
   * \param[out]  buffer  A buffer where the received data is stored.
   * \param[in]   size    The length of data to receive.
   * \param[out]  remote_address The remote address of the socket that sent the received data.
   * \return The length of the received data.
   */
  std::size_t Peek(void* buffer, std::size_t size, address::SocketAddress& remote_address) override;

  /**
   * \brief Returns received data.
   *
   * \param[out]  buffer  A buffer where the received data is stored.
   * \param[in]   size    The length of data to receive.
   * \param[out]  remote_address The remote address of the socket that sent the received data.
   * \return The length of the received data.
   */
  std::size_t Receive(void* buffer, std::size_t size, address::SocketAddress& remote_address) override;

  /**
   * \brief Returns peek of received data.
   *
   * \param[in,out] iovec A vector of buffers where the received data is stored.
   * \return The length of the received data.
   */
  std::size_t Peek(const IovecContainer iovec) override;

  /**
   * \brief Returns received data.
   *
   * \param[in,out] iovec A vector of buffers where the received data is stored.
   * \return The length of the received data.
   */
  std::size_t Receive(const IovecContainer iovec) override;

  /**
   * \brief Returns peek of received data.
   *
   * \param[in,out] iovec A vector of buffers where the received data is stored.
   * \param[out] remote_address The remote address of the socket that sent the received data.
   * \return The length of the received data.
   */
  std::size_t Peek(const IovecContainer iovec, address::SocketAddress& remote_address) override;

  /**
   * \brief Returns received data.
   *
   * \param[in,out] iovec A vector of buffers where the received data is stored.
   * \param[out] remote_address The remote address of the socket that sent the received data.
   * \return The length of the received data.
   */
  std::size_t Receive(const IovecContainer iovec, address::SocketAddress& remote_address) override;

  /**
   * \brief Sends data to the connected remote peer.
   *
   * \param[in] buffer A buffer where the data to be sent is stored.
   * \param[in] size The length of data to send.
   * \return The length of data sent to the remote peer.
   */
  std::size_t Send(const void* buffer, std::size_t size) override;

  /**
   * \brief Sends data to specified remote peer(s).
   *
   * \param[in] buffer A buffer where the data to be sent is stored.
   * \param[in] size The length of data to send.
   * \param[out] remote_address The remote address of the socket to which the passed data should be sent.
   * \return The length of data sent to remote peer(s).
   */
  std::size_t Send(const void* buffer, std::size_t size, const address::SocketAddress& remote_address) override;

  /**
   * \brief Sends data to the connected remote peer.
   *
   * \param iovec A vector of buffers where the data to be sent is stored.
   * \return The length of data sent to the remote peer.
   */
  std::size_t Send(const IovecContainer iovec) override;

  /**
   * \brief Sends data to specified remote peer(s).
   *
   * \param[in] iovec A vector of buffers where the data to be sent is stored.
   * \param[out] remote_address The remote address of the socket to which the passed data should be sent.
   * \return The length of data sent to remote peer(s).
   */
  std::size_t Send(const IovecContainer iovec, const address::SocketAddress& remote_address) override;

  /**
   * \brief Allows reusing of local addresses.
   *
   * \param[in] reuse Allows reusing of local addresses if true and disallows otherwise.
   */
  void SetReuseAddress(bool reuse) override;

  /**
   * \brief Indicates if a socket is connected to another remote socket or not.
   *
   * \return true if the socket is connected and false otherwise.
   */
  bool IsConnected() override;

  /**
   * \brief POSIX internal implementation details which are used by other classes.
   */
  struct internal {
    /**
     * \brief Returns the index of a network interface identified by its IP address.
     *
     * \param address An IP address.
     * \return A pair of a boolean, that indicates the success, and an interface index.
     */
    static std::pair<bool, unsigned int> InterfaceAddressToIndex(const vac::container::string_view& address);
  };

  /**
   * \brief Set the priority / Quality of Service (QoS) option for this socket object.
   *
   * \param priority The QoS priority to set for this socket [1 .. 7].
   */
  void SetPriority(const int priority) override;

 protected:
  Socket() = default;

  /**
   * \brief Contructor for internal use.
   *
   * \param[in] address_family The address_family (UNIX, INET, INET6) for which to create a socket
   * \param[in] socket_type the communication type (DGRAM, STREAM) used for communication
   */
  Socket(address::SocketAddressFamily address_family, SocketType socket_type);

  /**
   * \brief Converts a socket type to the corresponding Unix socket type.
   *
   * \param[in] type A socket type.
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
   * \param[out] buffer A buffer where the received data is stored.
   * \param[in] size The length of data to receive.
   * \param[in] flags The flags to determine the exact behavior of Receive for example Peek
   * \return The length of the received data.
   */
  virtual std::size_t Receive(void* buffer, std::size_t size, int flags);

  /**
   * \brief Returns received data.
   *
   * \param[out] buffer A buffer where the received data is stored.
   * \param[in] size The length of data to receive.
   * \param[out] remote_address The remote address of the socket that sent the received data.
   * \param[in] flags The flags to determine the exact behavior of Receive for example Peek
   * \return The length of the received data.
   */
  virtual std::size_t Receive(void* buffer, std::size_t size, address::SocketAddress& remote_address, int flags);

  /**
   * \brief Returns received data.
   *
   * \param[in,out] iovec A vector of buffers where the received data is stored.
   * \param[in] flags The flags to determine the exact behavior of Receive for example Peek
   * \return The length of the received data.
   */
  virtual std::size_t Receive(const IovecContainer iovec, int flags);

  /**
   * \brief Returns received data.
   *
   * \param[in,out] iovec A vector of buffers where the received data is stored.
   * \param[out] remote_address The remote address of the socket that sent the received data.
   * \param[in] flags The flags to determine the exact behavior of Receive for example Peek
   * \return The length of the received data.
   */
  virtual std::size_t Receive(const IovecContainer iovec, address::SocketAddress& remote_address, int flags);
};

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_SOCKET_H_

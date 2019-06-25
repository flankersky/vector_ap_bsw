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
 *        \brief  Abstract interface to work with different types of sockets.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_SOCKET_INTERFACE_H_
#define LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_SOCKET_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/socket.h>  // struct iovec
#include <cstddef>       // size_t
#include <memory>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
#include "vac/container/array_view.h"

namespace osabstraction {
namespace io {
namespace net {

namespace address {
class SocketAddress;
}  // namespace address

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
 * \brief Container for struct iovec arrays of arbitrary fixed length
 */
using IovecContainer = vac::container::array_view<struct iovec>;

/**
 * \brief Calculates the total length of data contained in an I/O vector container.
 *
 * \param iovc An I/O vector container.
 * \return The total length of data contained in an I/O vector container.
 */
inline std::size_t GetDataLengthOfIovecContainer(const IovecContainer& iovc) {
  return std::accumulate(iovc.cbegin(), iovc.cend(), 0U,
                         [](std::size_t result, const struct iovec& i) { return result + i.iov_len; });
}

/**
 * \brief Abstract socket interface.
 */
class SocketInterface {
 public:
  /**
   * \brief Destructor.
   */
  virtual ~SocketInterface() = default;

  /**
   * \brief Binds a socket to specified local address.
   *
   * \param local_address A local socket address to which the socket should be bound.
   */
  virtual void Bind(const address::SocketAddress& local_address) = 0;

  /**
   * \brief Closes the socket.
   */
  virtual void Close() = 0;

  /**
   * \brief Returns the local socket address.
   *
   * \return The local socket address.
   */
  virtual address::SocketAddress GetLocalAddress() const = 0;

  /**
   * \brief Returns the remote socket address.
   *
   * \return The remote socket address.
   */
  virtual address::SocketAddress GetRemoteAddress() const = 0;

  /**
   * \brief Returns peek of received data.
   *
   * \param buffer A buffer where the received data is stored.
   * \param size The length of data to receive.
   * \return The length of the received data.
   */
  virtual std::size_t Peek(void* buffer, std::size_t size) = 0;

  /**
   * \brief Returns received data.
   *
   * \param buffer A buffer where the received data is stored.
   * \param size The length of data to receive.
   * \return The length of the received data.
   */
  virtual std::size_t Receive(void* buffer, std::size_t size) = 0;

  /**
   * \brief Returns peek of received data.
   *
   * \param buffer A buffer where the received data is stored.
   * \param size The length of data to receive.
   * \param remote_address The remote address of the socket that sent the received data.
   * \return The length of the received data.
   */
  virtual std::size_t Peek(void* buffer, std::size_t size, address::SocketAddress& remote_address) = 0;

  /**
   * \brief Returns received data.
   *
   * \param buffer A buffer where the received data is stored.
   * \param size The length of data to receive.
   * \param remote_address The remote address of the socket that sent the received data.
   * \return The length of the received data.
   */
  virtual std::size_t Receive(void* buffer, std::size_t size, address::SocketAddress& remote_address) = 0;

  /**
   * \brief Returns peek of received data.
   *
   * \param iovec A vector of buffers where the received data is stored.
   * \return The length of the received data.
   */
  virtual std::size_t Peek(const IovecContainer iovec) = 0;

  /**
   * \brief Returns received data.
   *
   * \param iovec A vector of buffers where the received data is stored.
   * \return The length of the received data.
   */
  virtual std::size_t Receive(const IovecContainer iovec) = 0;

  /**
   * \brief Returns peek of received data.
   *
   * \param iovec A vector of buffers where the received data is stored.
   * \param remote_address The remote address of the socket that sent the received data.
   * \return The length of the received data.
   */
  virtual std::size_t Peek(const IovecContainer iovec, address::SocketAddress& remote_address) = 0;

  /**
   * \brief Returns received data.
   *
   * \param iovec A vector of buffers where the received data is stored.
   * \param remote_address The remote address of the socket that sent the received data.
   * \return The length of the received data.
   */
  virtual std::size_t Receive(const IovecContainer iovec, address::SocketAddress& remote_address) = 0;

  /**
   * \brief Sends data to the connected remote peer.
   *
   * \param buffer A buffer where the data to be sent is stored.
   * \param size The length of data to send.
   * \return The length of data sent to the remote peer.
   */
  virtual std::size_t Send(const void* buffer, std::size_t size) = 0;

  /**
   * \brief Sends data to specified remote peer(s).
   *
   * \param buffer A buffer where the data to be sent is stored.
   * \param size The length of data to send.
   * \param remote_address The remote address of the socket to which the passed data should be sent.
   * \return The length of data sent to remote peer(s).
   */
  virtual std::size_t Send(const void* buffer, std::size_t size, const address::SocketAddress& remote_address) = 0;

  /**
   * \brief Sends data to the connected remote peer.
   *
   * \param iovec A vector of buffers where the data to be sent is stored.
   * \return The length of data sent to the remote peer.
   */
  virtual std::size_t Send(const IovecContainer iovec) = 0;

  /**
   * \brief Sends data to specified remote peer(s).
   *
   * \param iovec A vector of buffers where the data to be sent is stored.
   * \param remote_address The remote address of the socket to which the passed data should be sent.
   * \return The length of data sent to remote peer(s).
   */
  virtual std::size_t Send(const IovecContainer iovec, const address::SocketAddress& remote_address) = 0;

  /**
   * \brief Allows reusing of local addresses.
   *
   * \param reuse Allows reusing of local addresses if true and disallows otherwise.
   */
  virtual void SetReuseAddress(bool reuse) = 0;

  /**
   * \brief Set the priority / Quality of Service (QoS) option for this socket object.
   *
   * \param priority The QoS priority to set.
   */
  virtual void SetPriority(const int priority) = 0;

  /**
   * \brief Indicates if a socket is connected to another remote socket or not.
   *
   * \return true if the socket is connected and false otherwise.
   */
  virtual bool IsConnected() = 0;
};

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_SOCKET_INTERFACE_H_

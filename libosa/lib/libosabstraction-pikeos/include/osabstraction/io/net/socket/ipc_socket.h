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

#ifndef LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_IPC_SOCKET_H_
#define LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_IPC_SOCKET_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/internal/sap_port.h"
#include "osabstraction/io/file_descriptor.h"
#include "osabstraction/io/handle_interface.h"
#include "osabstraction/io/net/address/ipc_socket_address.h"
#include "osabstraction/io/net/address/socket_address.h"
#include "osabstraction/io/net/socket/socket_interface.h"

namespace osabstraction {
namespace io {
namespace net {
namespace socket {

/**
 * \brief      Implementation of the socket interface.
 */
class IpcSocket : public HandleInterface, public virtual SocketInterface, public osabstraction::io::FileDescriptor {
 public:
  /**
   * \brief      Default contructor.
   */
  IpcSocket();

  /**
   * \brief      Copy contstructor.
   */
  IpcSocket(const IpcSocket&) = delete;

  /**
   * \brief      Copy assignment operator.
   */
  IpcSocket& operator=(const IpcSocket&) = delete;

  /**
   * \brief      Move constructor.
   */
  IpcSocket(IpcSocket&&) = default;

  /**
   * \brief      Move assignment operator.
   */
  IpcSocket& operator=(IpcSocket&&) = default;

  /**
   * \brief      Creates a IPC socket from local and remote address.
   *
   * \param[in]  local_address   The local socket address.
   * \param[in]  remote_address  The remote socket address.
   * \param[in]  id              The identifier of the new connection.
   */
  IpcSocket(const address::SocketAddress& local_address, const address::SocketAddress& remote_address,
            std::uint32_t id);

  /**
   * \brief Returns the type of the IO Handle.
   */
  HandleType GetType() const override { return HandleType::kIpcSocketHandle; }

  /**
   * \brief Returns the raw file descriptor.
   */
  int GetRawFiledescriptor() const override { return port_.GetHandle(); }

  /**
   * \brief      Binds a socket to specified local address.
   *
   * \param[in]  local_address  A local socket address to which the socket should be bound.
   */
  void Bind(const address::SocketAddress& local_address) override;

  /**
   * \brief      Returns the local socket address.
   *
   * \return     The local socket address.
   */
  address::SocketAddress GetLocalAddress() const override;

  /**
   * \brief      Returns the remote socket address.
   *
   * \return     The remote socket address.
   */
  address::SocketAddress GetRemoteAddress() const override;

  /**
   * \brief      Returns peek of received data.
   *
   * \param[out] buffer  A buffer where the received data is stored.
   * \param[in]  size    The length of data to receive.
   *
   * \return     The length of the received data.
   */
  std::size_t Peek(void* buffer, std::size_t size) override;

  /**
   * \brief      Returns peek of received data.
   *
   * \param[out] buffer          A buffer where the received data is stored.
   * \param[in]  size            The length of data to receive.
   * \param[out] remote_address  The remote address of the socket that sent the
   *                             received data.
   *
   * \return     The length of the received data.
   */
  std::size_t Peek(void* buffer, std::size_t size, address::SocketAddress& remote_address) override;

  /**
   * \brief      Returns peek of received data.
   *
   * \param[in]  iovec  A vector of buffers where the received data is stored.
   *
   * \return     The length of the received data.
   */
  std::size_t Peek(const IovecContainer iovec) override;

  /**
   * \brief      Returns peek of received data.
   *
   * \param[in]  iovec           A vector of buffers where the received data is
   *                             stored.
   * \param[out] remote_address  The remote address of the socket that sent the
   *                             received data.
   *
   * \return     The length of the received data.
   */
  std::size_t Peek(const IovecContainer iovec, address::SocketAddress& remote_address) override;

  /**
   * \brief      Returns received data.
   *
   * \param[out] buffer  A buffer where the received data is stored.
   * \param[in]  size    The length of data to receive.
   *
   * \return     The length of the received data.
   */
  std::size_t Receive(void* buffer, std::size_t size) override;

  /**
   * \brief      Returns received data.
   *
   * \param[out] buffer          A buffer where the received data is stored.
   * \param[in]  size            The length of data to receive.
   * \param[out] remote_address  The remote address of the socket that sent the
   *                             received data.
   *
   * \return     The length of the received data.
   */
  std::size_t Receive(void* buffer, std::size_t size, address::SocketAddress& remote_address) override;

  /**
   * \brief         Returns received data.
   *
   * \param[in,out] iovec  A vector of buffers where the received data is
   *                       stored.
   *
   * \return        The length of the received data.
   */
  std::size_t Receive(const IovecContainer iovec) override;

  /**
   * \brief         Returns received data.
   *
   * \param[in,out] iovec           A vector of buffers where the received data
   *                                is stored.
   * \param[out]    remote_address  The remote address of the socket that sent
   *                                the received data.
   *
   * \return        The length of the received data.
   */
  std::size_t Receive(const IovecContainer iovec, address::SocketAddress& remote_address) override;

  /**
   * \brief      Sends data to the connected remote peer.
   *
   * \param[in]  buffer  A buffer where the data to be sent is stored.
   * \param[in]  size    The length of data to send.
   *
   * \return     The length of data sent to the remote peer.
   */
  std::size_t Send(const void* buffer, std::size_t size) override;

  /**
   * \brief      Sends data to specified remote peer(s).
   *
   * \param[in]  buffer          A buffer where the data to be sent is stored.
   * \param[in]  size            The length of data to send.
   * \param[out] remote_address  The remote address of the socket to which the
   *                             passed data should be sent.
   *
   * \return     The length of data sent to remote peer(s).
   */
  std::size_t Send(const void* buffer, std::size_t size, const address::SocketAddress& remote_address) override;

  /**
   * \brief      Sends data to the connected remote peer.
   *
   * \param[in]  iovec  A vector of buffers where the data to be sent is stored.
   *
   * \return     The length of data sent to the remote peer.
   */
  std::size_t Send(const IovecContainer iovec) override;

  /**
   * \brief      Sends data to specified remote peer(s).
   *
   * \param[in]  iovec           A vector of buffers where the data to be sent
   *                             is stored.
   * \param[out] remote_address  The remote address of the socket to which the
   *                             passed data should be sent.
   *
   * \return     The length of data sent to remote peer(s).
   */
  std::size_t Send(const IovecContainer iovec, const address::SocketAddress& remote_address) override;

  /**
   * \brief      Allows reusing of local addresses.
   *
   * \param[in]  reuse  Allows reusing of local addresses if true and disallows
   *                    otherwise.
   */
  void SetReuseAddress(bool reuse) override { (void)reuse; }

  /**
   * \brief      Indicates if a socket is connected to another remote socket or
   *             not.
   *
   * \return     true if the socket is connected and false otherwise.
   */
  bool IsConnected() override { return connected_; }

  /**
   * \brief      Sets blocking mode of a socket.
   *
   * \param[in]  blocking  Enables blocking mode if true and disables it
   *                       otherwise.
   */
  void SetBlockingMode(bool blocking);

  /**
   * \brief      Gets the SAP port.
   *
   * \return     The internal SAP port.
   */
  internal::SapPort& GetSapPort() { return port_; }

  /**
   * \brief Closes the socket.
   */
  void Close() override { port_.Close(); };

  /**
   * \brief Set the priority.
   *
   * Empty implementation. Currently not supported.
   */
  void SetPriority(const int) override {}

 protected:
  /**
   * \brief      SAP port.
   */
  internal::SapPort port_;

  /**
   * \brief      Describes the local address of the socket.
   */
  address::SocketAddress local_addr_;

  /**
   * \brief      Describes the remote address of the socket.
   */
  address::SocketAddress remote_addr_;

  /**
   * \brief      Connection state of the socket.
   */
  bool connected_;

 private:
  /**
   * \brief      Returns the received data.
   *
   * \param[out] buffer          A buffer where the received data is stored.
   * \param[in]  size            The length of data to receive.
   * \param[out] remote_address  The remote address of the socket that sent the
   *                             received data.
   * \param[in]  peek            If true, the received data will not be removed
   *                             from the socket queue.
   *
   * \return     The length of the received data.
   */
  std::size_t Read(void* buffer, std::size_t size, address::SocketAddress& remote_address, bool peek);
};

}  // namespace socket
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_IO_NET_SOCKET_IPC_SOCKET_H_

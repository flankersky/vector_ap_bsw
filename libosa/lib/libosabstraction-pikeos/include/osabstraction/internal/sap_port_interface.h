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
 *        \brief  Interface to SAP port driver for PikeOS.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_INTERNAL_SAP_PORT_INTERFACE_H_
#define LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_INTERNAL_SAP_PORT_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <stddef.h>
#include <sys/qport.h>
#include <cstddef>    // size_t
#include <cstdint>    // uint32_t
#include <stdexcept>  // runtime_error
#include <utility>    // pair<>

namespace osabstraction {
namespace internal {

/**
 * \brief      Type of the event occured during Wait() call.
 */
enum EventType {
  /** No event occured. */
  kNone = 0,
  /** Read without blocking would be possible. */
  kRead = 1,
  /** Write without blocking would be possible. */
  kWrite = 2,
  /** Connect witout blocking would be possible. */
  kConnect = 4,
  /** The socket was closed. */
  kClose = 8
};

/**
 * \brief      Abstracts interface for SAP ports.
 */
class SapPortInterface {
 public:
  /**
   * \brief      Typedef for the file handle.
   */
  using HandleType = int;

  /**
   * \brief      Destructor.
   */
  virtual ~SapPortInterface() = default;

  /**
   * \brief      Opens the file input/output file handles.
   */
  virtual void Open(bool dgram) = 0;

  /**
   * \brief      Changes the state of the socket to enable incoming connections.
   */
  virtual void Listen() = 0;

  /**
   * \brief      Accepts an incoming connection and returns the client's remote
   *             address.
   *
   * \return     The remote address of the accepted client and the new connection ID.
   */
  virtual std::pair<qport_sap_sockaddr_ipv4_t, std::uint32_t> Accept() = 0;

  /**
   * \brief      Initiates a connection to a server.
   *
   * \param[in]  addr  The remote address of the server.
   */
  virtual void Connect(const qport_sap_sockaddr_ipv4_t& addr) = 0;

  /**
   * \brief      Returns peek of received data.
   *
   * \param      buffer           A buffer where the received data is stored.
   * \param[in]  size             The length of data to receive.
   * \param[in]  remote_addr      The address to receive from.
   * \param[out] remote_addr_out  The remote address of the port that sent the
   *                              received data.
   *
   * \return     The length of the received data.
   */
  virtual std::size_t Peek(void* buffer, std::size_t size, const qport_sap_sockaddr_ipv4_t& remote_addr,
                           qport_sap_sockaddr_ipv4_t& remote_addr_out) = 0;

  /**
   * \brief      Returns received data.
   *
   * \param      buffer           A buffer where the received data is stored
   * \param[in]  size             The length of data to receive.
   * \param[in]  remote_addr      The address to receive from.
   * \param[out] remote_addr_out  The remote address of the port that sent the
   *                              received data.
   *
   * \return     The length of the received data.
   */
  virtual std::size_t Receive(void* buffer, std::size_t size, const qport_sap_sockaddr_ipv4_t& remote_addr,
                              qport_sap_sockaddr_ipv4_t& remote_addr_out) = 0;

  /**
   * \brief      Sends data to the connected remote peer.
   *
   * \param[in]  buffer       A buffer where the data to be sent is stored.
   * \param[in]  size         The length of data to send.
   * \param[in]  remote_addr  he remote address of the port to which the passed
   *                          data should be sent.
   *
   * \return     The length of data sent to remote peer(s).
   */
  virtual std::size_t Send(const void* buffer, std::size_t size, const qport_sap_sockaddr_ipv4_t& remote_addr) = 0;

  /**
   * \brief      Sets if the read/write operation will block if no data/free space is available.
   *
   * \param[in]  block  true if the operation should block, false otherwise.
   */
  virtual void SetBlocking(bool block) = 0;

  /**
   * \brief      Blocks until an event occurs on the SAP port.
   *
   * \param[in]  event_mask  The options used to filter events.
   *
   * \return     The occured event and the connection ID the event occured on.
   */
  virtual std::pair<EventType, std::uint32_t> Wait(std::uint32_t event_mask) = 0;
};

}  // namespace internal
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_INTERNAL_SAP_PORT_INTERFACE_H_

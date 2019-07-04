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
 *        \brief  Implements SAP port driver interface.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_INTERNAL_SAP_PORT_H_
#define LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_INTERNAL_SAP_PORT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>  // pair<>
#include "osabstraction/internal/sap_port_interface.h"

namespace osabstraction {
namespace internal {

/**
 * \brief      Implementation of the SAP port interface.
 */
class SapPort : virtual public SapPortInterface {
 public:
  /**
   * \brief      Default constructor.
   */
  SapPort();

  /**
   * \brief      Destructor.
   */
  ~SapPort() { Close(); }

  /**
   * \brief      Build a SapPort with predefined identifier.
   *
   * \param[in]  identifier  The identifier to use.
   */
  explicit SapPort(std::uint32_t identifier);

  /**
   * \brief      Copy contstructor.
   */
  SapPort(const SapPort&) = delete;

  /**
   * \brief      Copy assignment operator.
   */
  SapPort& operator=(const SapPort&) = delete;

  /**
   * \brief      Move constructor.
   */
  SapPort(SapPort&& other) : handle_(other.handle_), identifier_(other.identifier_) {
    other.handle_ = -1;
    other.identifier_ = 0;
  }

  /**
   * \brief      Move assignment operator.
   */
  SapPort& operator=(SapPort&& other) {
    Close();
    handle_ = other.handle_;
    identifier_ = other.identifier_;
    other.handle_ = -1;
    other.identifier_ = 0;

    return *this;
  }

  /**
   * \brief      Opens the file input/output file handles.
   */
  void Open(bool dgram) override;

  /**
   * \brief      Changes the state of the socket to enable incoming connections.
   */
  void Listen() override;

  /**
   * \brief      Accepts an incoming connection and returns the client's remote
   *             address.
   *
   * \return     The remote address of the accepted client.
   */
  std::pair<qport_sap_sockaddr_ipv4_t, std::uint32_t> Accept() override;

  /**
   * \brief      Initiates a connection to a server.
   *
   * \param[in]  addr  The remote address of the server.
   */
  void Connect(const qport_sap_sockaddr_ipv4_t& addr) override;

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
  std::size_t Peek(void* buffer, std::size_t size, const qport_sap_sockaddr_ipv4_t& remote_addr,
                   qport_sap_sockaddr_ipv4_t& remote_addr_out) override;

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
  std::size_t Receive(void* buffer, std::size_t size, const qport_sap_sockaddr_ipv4_t& remote_addr,
                      qport_sap_sockaddr_ipv4_t& remote_addr_out) override;

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
  std::size_t Send(const void* buffer, std::size_t size, const qport_sap_sockaddr_ipv4_t& remote_addr) override;

  /**
   * \brief      Sets if the read/write operation will block if no data/free space is available.
   *
   * \param[in]  block  true if the operation should block, false otherwise.
   */
  void SetBlocking(bool block) override;

  /**
   * \brief      Blocks until an event occurs on the SAP port.
   *
   * \param[in]  event_mask  The options used to filter events.
   *
   * \return     The occured event and connection identifier.
   */
  std::pair<EventType, std::uint32_t> Wait(std::uint32_t event_mask) override;

  /**
   * \brief      Returns the internal identifier of the SAP port.
   *
   * \return     The internal identifier.
   */
  std::uint32_t GetId();

  /**
   * \brief      Converts the event bit representation to EventType.
   *
   * \param[in]  event  The event to convert.
   *
   * \return     EventType value.
   */
  static EventType toEventType(std::uint32_t event);

  /**
   * \brief      Closes the open handle.
   */
  void Close();

  /**
   * \brief Returns the raw file descriptor.
   */
  int GetHandle() const { return handle_; }

 protected:
  /**
   * \brief      File handle for the QPort.
   */
  HandleType handle_;

  /**
   * \brief      Internal port identifier.
   */
  std::uint32_t identifier_;

 private:
  /**
  * \brief      Change the options flags of the SAP port.
  *
  * \param[in]  flags  The options flags to set.
  */
  void SetFlags(std::uint32_t flags);

  /**
   * \brief      Returns the current options flags of the SAP port.
   *
   * \return     The active options flags.
   */
  std::uint32_t GetFlags();

  /**
   * \brief      Requests the internal identifier of the SAP port from the driver.
   */
  void RequestId();
};

}  // namespace internal
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_INTERNAL_SAP_PORT_H_

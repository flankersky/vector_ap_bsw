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
/**        \file  service_discovery_endpoint.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_ENDPOINT_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_ENDPOINT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <string>
#include <utility>
#include <vector>
#include "ara/log/logging.hpp"
#include "osabstraction/io/network/socket/udp_socket.h"
#include "osabstraction/io/reactor_interface.h"
#include "someip-posix-common/someip/message.h"
#include "someip-posix-common/someip/someip_datagram_message_reader.h"
#include "someipd-posix/service_discovery/service_discovery_endpoint_observer.h"
#include "someipd-posix/service_discovery/service_discovery_entry.h"
#include "someipd-posix/service_discovery/service_discovery_message_sender.h"
#include "someipd-posix/service_discovery/service_discovery_option.h"
#include "someipd-posix/service_discovery/service_discovery_reboot_management.h"

namespace someipd_posix {
namespace service_discovery {

/**
 * \brief ServiceDiscoveryEndpoint.
 */
class ServiceDiscoveryEndpoint : public osabstraction::io::EventHandler, public ServiceDiscoveryMessageSender {
 public:
  /**
   * \brief Constructor of ServiceDiscoveryEndpoint.
   *
   * \param reactor A reactor for asynchronous event notification on file descriptors.
   * \param unicast_address A unicast IP address used for sending and receiving of SD messages.
   * \param multicast_address A multicast IP address used for receiving of SD messages.
   * \param port A UDP port.
   * \param max_datagram_length The maximum length of a UDP datagram received or sent.
   * \param sd_endpoint_observer ServiceDiscoveryEndpointObserver object for actions on reception of a SOME/IP SD
   * message.
   */
  ServiceDiscoveryEndpoint(osabstraction::io::ReactorInterface* reactor, const std::string& unicast_address,
                           const std::string& multicast_address, std::uint16_t port, std::size_t max_datagram_length,
                           ServiceDiscoveryEndpointObserver& sd_endpoint_observer);

  /**
   * \brief Destructor of ServiceDiscoveryEndpoint.
   */
  ~ServiceDiscoveryEndpoint();
  /**
   * \brief Delete copy constructor.
   */
  explicit ServiceDiscoveryEndpoint(const ServiceDiscoveryEndpoint&) = delete;
  /**
   * \brief Delete copy assignment operator.
   */
  ServiceDiscoveryEndpoint& operator=(const ServiceDiscoveryEndpoint&) = delete;
  /**
   * \brief Delete move constructor.
   */
  explicit ServiceDiscoveryEndpoint(ServiceDiscoveryEndpoint&&) = delete;
  /**
   * \brief Delete move assignment operator.
   */
  ServiceDiscoveryEndpoint& operator=(ServiceDiscoveryEndpoint&&) = delete;
  /**
   * \brief Returns the unicast IP address of an SD endpoint.
   *
   * \return An IP address.
   */
  const std::string& GetUnicastAddress() const { return ucast_address_; }

  /**
   * \brief Sends a unicast SOME/IP SD message to the provided IP address and port.
   *
   * \param to_address An IP address identifying the destination.
   * \param to_port A UDP port identifying the destination.
   * \param entries A container of entries which will be sent in the SOME/IP SD message.
   * \param options A container of options which will be sent in the SOME/IP SD message.
   */
  void SendUnicastMessage(const std::string& to_address, std::uint16_t to_port,
                          const ServiceDiscoveryEntryContainer& entries,
                          const ServiceDiscoveryOptionContainer& options) override;
  /**
   * \brief Sends a multicast SOME/IP SD message to the provided IP address and port.
   *
   * \param entries A container of entries which will be sent in the SOME/IP SD message.
   * \param options A container of options which will be sent in the SOME/IP SD message.
   */
  void SendMulticastMessage(const ServiceDiscoveryEntryContainer& entries,
                            const ServiceDiscoveryOptionContainer& options) override;

 private:
  /**
   * \brief Represents a single UDP datagram.
   */
  using DatagramBuffer = std::vector<std::uint8_t>;
  /**
   * \brief An iterator of a datagram buffer.
   */
  using DatagramBufferIterator = DatagramBuffer::iterator;
  /**
   * \brief A SOME/IP message type.
   */
  using SomeIpMessage = someip_posix_common::someip::SomeIpMessage;
  /**
   * \brief Parses and processes a single received SOME/IP-SD message.
   *
   * \param packet A SOME/IP-SD packet.
   * \param is_multicast Specifies whether a SOME/IP-SD message is a multicast one.
   */
  void ProcessSdMessage(SomeIpMessage&& packet, bool is_multicast);
  /**
   * \brief Reconstructs SD entries container from a SD entries array buffer.
   *
   * \param buffer A buffer containing SD entries array.
   * \param buffer_length The length of the SD entries buffer.
   * \return A pair of a boolean which specifies whether the parsing has been successful or not and an SD entry
   * container.
   */
  std::pair<bool, ServiceDiscoveryEntryContainer> ParseSdEntries(const std::uint8_t* buffer, std::size_t buffer_length);
  /**
   * \brief Reconstructs SD options container from a SD options array buffer.
   *
   * \param buffer A buffer containing SD options array.
   * \param buffer_length The length of the SD options buffer.
   * \return A pair of a boolean which specifies whether the parsing has been successful or not and an SD option
   * container.
   */
  std::pair<bool, ServiceDiscoveryOptionContainer> ParseSdOptions(const std::uint8_t* buffer,
                                                                  std::size_t buffer_length);
  /**
   * \brief Reconstructs SD FindService entry from a SD entry buffer.
   *
   * \param buffer A buffer containing SD entry.
   * \param buffer_length The length of the SD entry buffer.
   * \return A pair containing a boolean value which indicates the success and a FindService entry.
   */
  std::pair<bool, ServiceDiscoveryEntry> ParseSdFindServiceEntry(const std::uint8_t* buffer, std::size_t buffer_length);
  /**
   * \brief Reconstructs SD OfferService entry from a SD entry buffer.
   *
   * \param buffer A buffer containing SD entry.
   * \param buffer_length The length of the SD entry buffer.
   * \return A pair containing a boolean value which indicates the success and a OfferService entry.
   */
  std::pair<bool, ServiceDiscoveryEntry> ParseSdOfferServiceEntry(const std::uint8_t* buffer,
                                                                  std::size_t buffer_length);
  /**
   * \brief Reconstructs SD SubscribeEventgroup entry from a SD entry buffer.
   *
   * \param buffer A buffer containing SD entry.
   * \param buffer_length The length of the SD entry buffer.
   * \return A pair containing a boolean value which indicates the success and a SubscribeEventgroup entry.
   */
  std::pair<bool, ServiceDiscoveryEntry> ParseSdSubscribeEventgroupEntry(const std::uint8_t* buffer,
                                                                         std::size_t buffer_length);
  /**
   * \brief Reconstructs SD SubscribeEventgroupAck entry from a SD entry buffer.
   *
   * \param buffer A buffer containing SD entry.
   * \param buffer_length The length of the SD entry buffer.
   * \return A pair containing a boolean value which indicates the success and a SubscribeEventgroupAck entry.
   */
  std::pair<bool, ServiceDiscoveryEntry> ParseSdSubscribeEventgroupAckEntry(const std::uint8_t* buffer,
                                                                            std::size_t buffer_length);
  /**
   * \brief Reconstructs SD IPv4 endpoint option from a SD option buffer.
   *
   * \param buffer A buffer containing SD option.
   * \param buffer_length The length of the SD option buffer.
   * \return A pair containing a boolean value which indicates the success and a IPv4 endpoint option.
   */
  std::pair<bool, ServiceDiscoveryOption> ParseSdIPv4EndpointOption(const std::uint8_t* buffer,
                                                                    std::size_t buffer_length);
  /**
   * \brief Reconstructs SD IPv6 endpoint option from a SD option buffer.
   *
   * \param buffer A buffer containing SD option.
   * \param buffer_length The length of the SD option buffer.
   * \return A pair containing a boolean value which indicates the success and a IPv4 endpoint option.
   */
  std::pair<bool, ServiceDiscoveryOption> ParseSdIPv6EndpointOption(const std::uint8_t* buffer,
                                                                    std::size_t buffer_length);
  /**
   * \brief Serializes a SOME/IP SD message header into a datagram buffer.
   *
   * \param pos An iterator into a datagram buffer.
   * \param body_length Length of body of a SOME/IP SD message.
   * \param session_id session identifier.
   * \return Returns the next free position in the datagram buffer.
   */
  DatagramBufferIterator SerializeSdMessageHeader(DatagramBufferIterator pos, std::size_t body_length,
                                                  someip_posix_common::someip::SessionId session_id);
  /**
   * \brief Serializes a single SOME/IP SD message into a datagram buffer.
   *
   * \param pos An iterator into a datagram buffer.
   * \param entries A container of SOME/IP SD entries.
   * \param options A container of SOME/IP SD options.
   * \param reboot_flag reboot flag for reboot detection.
   * \return Returns the next free position in the datagram buffer.
   */
  DatagramBufferIterator SerializeSdMessage(DatagramBufferIterator pos, const ServiceDiscoveryEntryContainer& entries,
                                            const ServiceDiscoveryOptionContainer& options, bool reboot_flag);
  /**
   * \brief Serializes a single SOME/IP SD FindService entry into a datagram buffer.
   *
   * \param pos An iterator into a datagram buffer.
   * \param entries A SOME/IP SD entry.
   * \return Returns the next free position in the datagram buffer.
   */
  DatagramBufferIterator SerializeSdFindServiceEntry(DatagramBufferIterator pos, const ServiceDiscoveryEntry& entry);
  /**
   * \brief Serializes a single SOME/IP SD OfferService entry into a datagram buffer.
   *
   * \param pos An iterator into a datagram buffer.
   * \param entries A SOME/IP SD entry.
   * \return Returns the next free position in the datagram buffer.
   */
  DatagramBufferIterator SerializeSdOfferServiceEntry(DatagramBufferIterator pos, const ServiceDiscoveryEntry& entry);
  /**
   * \brief Serializes a single SOME/IP SD SubscribeEventgroup entry into a datagram buffer.
   *
   * \param pos An iterator into a datagram buffer.
   * \param entries A SOME/IP SD entry.
   * \return Returns the next free position in the datagram buffer.
   */
  DatagramBufferIterator SerializeSdSubscribeEventgroupEntry(DatagramBufferIterator pos,
                                                             const ServiceDiscoveryEntry& entry);
  /**
   * \brief Serializes a single SOME/IP SD SubscribeEventgroupAck entry into a datagram buffer.
   *
   * \param pos An iterator into a datagram buffer.
   * \param entries A SOME/IP SD entry.
   * \return Returns the next free position in the datagram buffer.
   */
  DatagramBufferIterator SerializeSdSubscribeEventgroupAckEntry(DatagramBufferIterator pos,
                                                                const ServiceDiscoveryEntry& entry);
  /**
   * \brief Serializes a single SOME/IP SD IPv4 endpoint option into a datagram buffer.
   *
   * \param pos An iterator into a datagram buffer.
   * \param entries A SOME/IP SD endpoint option.
   * \return Returns the next free position in the datagram buffer.
   */
  DatagramBufferIterator SerializeSdIPv4EndpointOption(DatagramBufferIterator pos,
                                                       const ServiceDiscoveryOption& option);
  /**
   * \brief Serializes a single SOME/IP SD IPv6 endpoint option into a datagram buffer.
   *
   * \param pos An iterator into a datagram buffer.
   * \param entries A SOME/IP SD endpoint option.
   * \return Returns the next free position in the datagram buffer.
   */
  DatagramBufferIterator SerializeSdIPv6EndpointOption(DatagramBufferIterator pos,
                                                       const ServiceDiscoveryOption& option);
  /**
   * \brief Handler for asynchronous event notification on UDP sockets.
   *
   * \param handle A file descriptor.
   * \return Returns false if the socket was closed, true otherwise.
   */
  bool HandleRead(int handle) override;

  /**
   * \brief Process the reboot detection.
   * \uptrace PRS_SOMEIPSD_00258
   * \param old_reboot_param  last reboot_param (session_id and reboot flag) value received.
   * \param new_reboot_param  values of the current packet (session_id and reboot flag) received
   * \return true if reboot detected otherwise false
   */
  bool IsRebootDetected(const RebootParam& old_reboot_param, const RebootParam& new_reboot_param) const;
  /**
   * \brief A reactor which used for asynchronous event notification on file descriptors.
   */
  osabstraction::io::ReactorInterface* reactor_;
  /**
   * \brief Our logger.
   */
  ara::log::Logger& logger_;
  /**
   * \brief A unicast IP address used for sending and receiving of SD messages.
   */
  std::string ucast_address_;
  /**
   * \brief A multicast IP address used for sending and receiving of SD messages.
   */
  std::string mcast_address_;
  /**
   * \brief A UDP port.
   */
  std::uint16_t port_;
  /**
   * \brief Maximum length of a UDP datagram.
   */
  std::size_t max_dgram_length_;
  /**
   * \brief An IP unicast socket address.
   */
  osabstraction::io::network::address::IPSocketAddress ucast_socket_address_;
  /**
   * \brief An IP multicast socket address.
   */
  osabstraction::io::network::address::IPSocketAddress mcast_socket_address_;
  /**
   * \brief A unicast UDP socket.
   */
  osabstraction::io::network::socket::UDPSocket ucast_socket_;
  /**
   * \brief A multicast UDP socket.
   */
  osabstraction::io::network::socket::UDPSocket mcast_socket_;
  /**
   * \brief Message Receiver Observer.
   */
  ServiceDiscoveryEndpointObserver& sd_endpoint_observer_;
  /**
   * \brief A SOME/IP message reader.
   */
  someip_posix_common::someip::SomeIpDatagramMessageReader message_reader_;
  /**
   * \brief A buffer for mutlicast SD messages.
   */
  DatagramBuffer send_mcast_dgram_buf_;
  /**
   * \brief Next free position in the datgram buffer for multicast SOME/IP SD messages.
   */
  DatagramBufferIterator send_mcast_dgram_buf_pos_;
  /**
   * \brief A buffer for unicast SD messages.
   */
  DatagramBuffer send_ucast_dgram_buf_;
  /**
   * \brief Reboot Manager.
   */
  ServiceDiscoveryRebootManagement reboot_manager_;
};

}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_SERVICE_DISCOVERY_ENDPOINT_H_

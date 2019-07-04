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
/**        \file  service_discovery_client_offer.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_OFFER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_OFFER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include <utility>
#include "someipd-posix/service_discovery/service_discovery_entry.h"
#include "someipd-posix/service_discovery/service_discovery_option.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {

/**
 * \brief Represents a valid Service Discovery offer with all corresponding data describing it.
 */
class ServiceDiscoveryClientOffer {
 public:
  /**
   * \brief A type for endpoints.
   */
  using EndpointType = std::pair<std::string, std::uint16_t>;
  /**
   * \brief Constructor of ServiceDiscoveryClientOffer.
   *
   * Creates an empty and invalid offer.
   */
  ServiceDiscoveryClientOffer();
  /**
   * \brief Constructor of ServiceDiscoveryClientOffer.
   *
   * Creates a valid offer.
   *
   * \param sd_address The IP address of service discovery that sent the offer.
   * \param sd_port The UDP port of service discovery that sent the offer.
   * \param entry An OfferService entry.
   * \param options A container of options belonging to the offer.
   * \param is_multicast Indicates whether the received 'OfferService' message has been sent via multicast.
   */
  ServiceDiscoveryClientOffer(const std::string& sd_address, std::uint16_t sd_port, const ServiceDiscoveryEntry& entry,
                              const ServiceDiscoveryOptionContainer& options, bool is_multicast);
  /**
   * \brief Destructor of ServiceDiscoveryClientOffer.
   */
  virtual ~ServiceDiscoveryClientOffer() = default;
  /**
   * \brief Indicates whether the offer is valid or not.
   *
   * \return true if the offer is valid and false otherwise.
   */
  bool IsValid() const;
  /**
   * \brief Indicates whether the offer has been received via multicast.
   *
   * \return true if the offer has been received via multicast and false otherwise.
   */
  bool IsMulticast() const;
  /**
   * \brief Invalidates the offer.
   */
  void Invalidate();
  /**
   * \brief Returns the IP address and the UDP port of a service discovery the offer came from.
   *
   * \return A pair of the IP address and UDP port of a remote service discovery.
   */
  EndpointType GetSdEndpoint() const;
  /**
   * \brief Returns the OfferService entry.
   *
   * \return An OfferService entry.
   */
  const ServiceDiscoveryEntry& GetEntry() const;
  /**
   * \brief Returns a container of options belonging to the offer.
   *
   * \return A container of options.
   */
  const ServiceDiscoveryOptionContainer& GetOptions() const;
  /**
   * \brief Indicates whether the offer is a StopOfferService.
   *
   * \return true if the offer is a StopOfferService and false otherwise.
   */
  bool IsStopOffer() const;
  /**
   * \brief Returns the IP address and the TCP port of the server offering the service.
   *
   * \return A pair of the IP address and TCP port of a remote server.
   */
  EndpointType GetTcpEndpoint() const;
  /**
   * \brief Returns the IP address and the UDP port of the server offering the service.
   *
   * \return A pair of the IP address and UDP port of a remote server.
   */
  EndpointType GetUdpEndpoint() const;

 private:
  /**
   * \brief Indicates whether the offer is valid or not.
   */
  bool is_valid_;
  /**
   * \brief The IP address of service discovery that sent the offer.
   */
  std::string sd_address_;
  /**
   * \brief The UDP port of service discovery that sent the offer.
   */
  std::uint16_t sd_port_;
  /**
   * \brief An OfferService entry belonging to the offer.
   */
  ServiceDiscoveryEntry entry_;
  /**
   * \brief A container of options belonging to the offer.
   */
  ServiceDiscoveryOptionContainer options_;
  /**
   * \brief Indicates whether the offer has been received via multicast.
   */
  bool is_multicast_;
  /**
   * \brief Points to the TCP option if present.
   */
  int tcp_option_;
  /**
   * \brief Points to the UDP option if present.
   */
  int udp_option_;
};

}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_SERVICE_DISCOVERY_CLIENT_OFFER_H_

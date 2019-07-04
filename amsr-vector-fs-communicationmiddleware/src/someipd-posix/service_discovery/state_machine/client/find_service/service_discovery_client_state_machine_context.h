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
/**        \file  service_discovery_client_state_machine_context.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_MACHINE_CONTEXT_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_MACHINE_CONTEXT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <cstdint>

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

/**
 * \brief A Service Discovery Client state machine context.
 */
class ServiceDiscoveryClientStateMachineContext {
 public:
  /**
   * \brief A timer delay.
   */
  using TimerDelay = std::chrono::high_resolution_clock::duration;
  /**
   * \brief Destructor of ServiceDiscoveryClientStateMachineContext.
   */
  virtual ~ServiceDiscoveryClientStateMachineContext() = default;
  /**
   * \brief Updates the availability of the service.
   *
   * \param is_available Indicates the availability of the service.
   */
  virtual void SetServiceAvailable(bool is_available) = 0;
  /**
   * \brief Returns the TTL value from the last matching OfferService entry.
   *
   * \return TTL value.
   */
  virtual TimerDelay GetOfferServiceTtl() const = 0;
  /**
   * \brief Returns the minimum time value to delay the transmission of a FindService entry.
   *
   * \return Minimum delay.
   */
  virtual TimerDelay GetFindServiceInitialDelayMin() const = 0;
  /**
   * \brief Returns the maximum time value to delay the transmission of a FindService entry.
   *
   * \return Maximum delay.
   */
  virtual TimerDelay GetFindServiceInitialDelayMax() const = 0;
  /**
   * \brief Returns the maximum number of transmissions of FindService entry.
   *
   * \return Maximum number of repetitions.
   */
  virtual std::size_t GetFindServiceRepetitionsMax() const = 0;
  /**
   * \brief Returns the base delay for the transmission of FindService entry.
   *
   * \return Base delay.
   */
  virtual TimerDelay GetFindServiceRepetitionsBaseDelay() const = 0;
  /**
   * \brief Transmits a FindService entry.
   */
  virtual void SendFindService() = 0;
};

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_MACHINE_CONTEXT_H_

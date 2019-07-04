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
/**        \file  service_discovery_client_state_owner.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_OWNER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_OWNER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state.h"
#include "someipd-posix/service_discovery/state_machine/client/find_service/service_discovery_client_state_pool.h"
#include "vac/statemachine/state_owner.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace find_service {

/**
 * \brief A Service Discovery Client state owner.
 */
class ServiceDiscoveryClientStateOwner : public vac::statemachine::StateOwner<ServiceDiscoveryClientState> {
 public:
  using vac::statemachine::StateOwner<ServiceDiscoveryClientState>::StateOwner;
  /**
   * \brief Constructor of ServiceDiscoveryClientStateOwner.
   *
   * \param state_pool A state pool.
   * \param context A state context.
   */
  ServiceDiscoveryClientStateOwner(ServiceDiscoveryClientStatePool& state_pool,
                                   ServiceDiscoveryClientStateContext& context)
      : StateOwner{state_pool, ServiceDiscoveryClientStateHandle::kDownPhase}, context_{context} {}
  /**
   * \brief Destructor of ServiceDiscoveryClientStateOwner.
   */
  virtual ~ServiceDiscoveryClientStateOwner() = default;
  /**
   * \brief Returns the context for states.
   *
   * \return The state context.
   */
  ServiceDiscoveryClientStateContext& GetContext() override { return context_; }

 private:
  /**
   * \brief A state context.
   */
  ServiceDiscoveryClientStateContext& context_;
};

}  // namespace find_service
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_FIND_SERVICE_SERVICE_DISCOVERY_CLIENT_STATE_OWNER_H_

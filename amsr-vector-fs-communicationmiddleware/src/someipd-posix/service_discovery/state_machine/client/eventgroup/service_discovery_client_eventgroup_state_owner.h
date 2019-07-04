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
/**        \file  service_discovery_client_eventgroup_state_owner.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_STATE_OWNER_H_
#define SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_STATE_OWNER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_state.h"
#include "someipd-posix/service_discovery/state_machine/client/eventgroup/service_discovery_client_eventgroup_state_pool.h"
#include "vac/statemachine/state_owner.h"

namespace someipd_posix {
namespace service_discovery {
namespace state_machine {
namespace client {
namespace eventgroup {

/**
 * \brief A Service Discovery Client eventgroup state owner.
 */
class ServiceDiscoveryClientEventgroupStateOwner
    : public vac::statemachine::StateOwner<ServiceDiscoveryClientEventgroupState> {
 public:
  using vac::statemachine::StateOwner<ServiceDiscoveryClientEventgroupState>::StateOwner;
  /**
   * \brief Constructor of ServiceDiscoveryClientEventgroupStateOwner.
   *
   * \param state_pool A state pool.
   * \param context A state context.
   */
  ServiceDiscoveryClientEventgroupStateOwner(ServiceDiscoveryClientEventgroupStatePool& state_pool,
                                             ServiceDiscoveryClientEventgroupStateContext& context)
      : StateOwner{state_pool, ServiceDiscoveryClientEventgroupStateHandle::kNotSubscribed}, context_{context} {}
  /**
   * \brief Destructor of ServiceDiscoveryEventgroupClientStateOwner.
   */
  virtual ~ServiceDiscoveryClientEventgroupStateOwner() = default;
  /**
   * \brief Returns the context for states.
   *
   * \return The state context.
   */
  ServiceDiscoveryClientEventgroupStateContext& GetContext() override { return context_; }

 private:
  /**
   * \brief A state context.
   */
  ServiceDiscoveryClientEventgroupStateContext& context_;
};

}  // namespace eventgroup
}  // namespace client
}  // namespace state_machine
}  // namespace service_discovery
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_SERVICE_DISCOVERY_STATE_MACHINE_CLIENT_EVENTGROUP_SERVICE_DISCOVERY_CLIENT_EVENTGROUP_STATE_OWNER_H_

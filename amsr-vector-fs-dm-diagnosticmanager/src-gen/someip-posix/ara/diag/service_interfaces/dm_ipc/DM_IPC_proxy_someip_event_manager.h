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
/**        \file  DM_IPC_proxy_someip_event_manager.h
 *        \brief  SOME/IP proxy event handling for events and field notifications of service 'DM_IPC'
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_DM_IPC_PROXY_SOMEIP_EVENT_MANAGER_H_
#define SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_DM_IPC_PROXY_SOMEIP_EVENT_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <mutex>
#include "ara-someip-posix/e2e_marshalling.h"
#include "ara-someip-posix/service_proxy_someip_event_interface.h"
#include "ara/com/internal/proxy_event.h"
#include "ara/com/internal/proxy_impl_event.h"
#include "ara/diag/service_interfaces/dm_ipc/DM_IPC.h"
#include "someip-posix-common/someip/marshalling.h"
#include "someip-posix/someip_posix.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace dm_ipc {

// Forward-declaration for back-reference
class DM_IPCProxySomeIpBinding;

}  // namespace dm_ipc
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_DM_IPC_PROXY_SOMEIP_EVENT_MANAGER_H_

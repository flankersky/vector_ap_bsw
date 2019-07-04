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
/**        \file  proxys_pool.h
 *        \brief  This file contains memory spaces for each used proxy.
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_PROXYS_PROXYS_POOL_H_
#define SRC_SERVER_PROXYS_PROXYS_POOL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/diag/service_interfaces/dm_ipc/DM_IPC_proxy.h"
#include "vac/memory/optional.h"

namespace amsr {
namespace diag {
namespace server {
namespace proxys {
/**
 * \brief Implementation of ProxysPool
 * This class contains memory space for used proxy. This is currently not thread safe but used as a workaround since
 * proxys are not movable.
 */
struct ProxysPool {
 public:
  /**
   * \brief Constructor.
   */
  ProxysPool() = default;

  // Remove copy/move constructor/operator.
  ProxysPool(const ProxysPool&) = delete;
  ProxysPool(const ProxysPool&&) = delete;
  ProxysPool& operator=(const ProxysPool&) = delete;
  ProxysPool& operator=(const ProxysPool&&) = delete;

  /**
   * \brief Optional containing the DM_IPCProxy proxy.
   */
  vac::memory::optional<ara::diag::service_interfaces::dm_ipc::DM_IPCProxy> ipc_proxy_;
};

}  // namespace proxys
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_PROXYS_PROXYS_POOL_H_

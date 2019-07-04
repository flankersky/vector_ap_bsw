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
/**        \file  DM_IPC_proxy_impl_interface.h
 *        \brief  Proxy implementation interface of service 'DM_IPC'.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_DM_IPC_PROXY_IMPL_INTERFACE_H_
#define SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_DM_IPC_PROXY_IMPL_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/future.h"
#include "ara/com/internal/proxy_impl_event.h"
#include "ara/diag/service_interfaces/dm_ipc/DM_IPC.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace dm_ipc {

/**
 * \brief Proxy implementation interface for the Service 'DM_IPC'
 * \remark generated
 */
class DM_IPCProxyImplInterface : public ara::com::internal::ProxyImplInterface {
 public:
  /**
   * \brief Default destructor
   */
  virtual ~DM_IPCProxyImplInterface() = default;

  /* ---- Methods -------------------------------------------------------------------------------------------------- */

  /**
   * \brief Pure virtual implementation interface of service method 'RequestData'.
   * \param handle IN parameter of type uint32
   * \param meta_info IN parameter of type MetaInfoType
   * \param request_data IN parameter of type DataArrayType
   * \return ara::com::Future with output struct arguments
   *         response_data OUT parameter of type DataArrayType
   */
  virtual ara::com::Future<proxy::methods::RequestData::Output> MethodRequestData(uint32& handle,
                                                                                  MetaInfoType& meta_info,
                                                                                  DataArrayType& request_data) = 0;

  /* ---- Events --------------------------------------------------------------------------------------------------- */

  /* ---- Fields --------------------------------------------------------------------------------------------------- */
};

}  // namespace dm_ipc
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_DM_IPC_PROXY_IMPL_INTERFACE_H_

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
/**        \file  GenericUDSService_proxy_impl_interface.h
 *        \brief  Proxy implementation interface of service 'GenericUDSService'.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_GENERICUDSSERVICE_PROXY_IMPL_INTERFACE_H_
#define SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_GENERICUDSSERVICE_PROXY_IMPL_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/future.h"
#include "ara/com/internal/proxy_impl_event.h"
#include "ara/diag/service_interfaces/generic_uds_service/GenericUDSService.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace generic_uds_service {

/**
 * \brief Proxy implementation interface for the Service 'GenericUDSService'
 * \remark generated
 */
class GenericUDSServiceProxyImplInterface : public ara::com::internal::ProxyImplInterface {
 public:
  /**
   * \brief Default destructor
   */
  virtual ~GenericUDSServiceProxyImplInterface() = default;

  /* ---- Methods -------------------------------------------------------------------------------------------------- */

  /**
   * \brief Pure virtual implementation interface of service method 'Service'.
   * \param SID IN parameter of type uint8
   * \param RequestData IN parameter of type DataArrayType
   * \param MetaInfo IN parameter of type MetaInfoType
   * \return ara::com::Future with output struct arguments
   *         ResponseData OUT parameter of type DataArrayType
   */
  virtual ara::com::Future<proxy::methods::Service::Output> MethodService(uint8& SID, DataArrayType& RequestData,
                                                                          MetaInfoType& MetaInfo) = 0;

  /* ---- Events --------------------------------------------------------------------------------------------------- */

  /* ---- Fields --------------------------------------------------------------------------------------------------- */
};

}  // namespace generic_uds_service
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_GENERICUDSSERVICE_PROXY_IMPL_INTERFACE_H_

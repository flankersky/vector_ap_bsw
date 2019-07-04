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
/**        \file  GenericUDSService.h
 *        \brief  Header for service 'GenericUDSService'.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTAPP_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_GENERICUDSSERVICE_H_
#define ADDON_DIAGTESTAPP_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_GENERICUDSSERVICE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/internal/types.h"
#include "ara/diag/service_interfaces/generic_uds_service/impl_type_DataArrayType.h"
#include "ara/diag/service_interfaces/generic_uds_service/impl_type_MetaInfoKeyType.h"
#include "ara/diag/service_interfaces/generic_uds_service/impl_type_MetaInfoType.h"
#include "ara/diag/service_interfaces/generic_uds_service/impl_type_MetaInfoValueType.h"
#include "ara/diag/service_interfaces/generic_uds_service/impl_type_UDSResponseCodeType.h"
#include "ara/diag/service_interfaces/generic_uds_service/impl_type_uint8.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace generic_uds_service {

namespace proxy {

namespace events {}  // namespace events

namespace fields {}  // namespace fields

namespace methods {

/**
 * \brief Data class for service method 'Service'.
 * \remark generated
 */
class Service {
 public:
  /**
   * \brief Return/output parameters of service method 'Service'
   */
  struct Output {
    ara::diag::service_interfaces::generic_uds_service::DataArrayType ResponseData;
  };
};

}  // namespace methods
}  // namespace proxy

namespace skeleton {

namespace events {}  // namespace events

namespace fields {}  // namespace fields

namespace methods {

/**
 * \brief Data class for service method 'Service'.
 * \remark generated
 */
class Service {
 public:
  /**
   * \brief Return/output parameters of service method 'Service'
   */
  struct Output {
    ara::diag::service_interfaces::generic_uds_service::DataArrayType ResponseData;
  };
};

}  // namespace methods
}  // namespace skeleton

}  // namespace generic_uds_service
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTAPP_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_GENERICUDSSERVICE_H_

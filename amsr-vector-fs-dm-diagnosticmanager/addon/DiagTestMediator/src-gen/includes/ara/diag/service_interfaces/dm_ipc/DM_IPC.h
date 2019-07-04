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
/**        \file  DM_IPC.h
 *        \brief  Header for service 'DM_IPC'.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_DM_IPC_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_DM_IPC_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/internal/types.h"
#include "ara/diag/service_interfaces/dm_ipc/impl_type_DataArrayType.h"
#include "ara/diag/service_interfaces/dm_ipc/impl_type_MetaInfoKeyType.h"
#include "ara/diag/service_interfaces/dm_ipc/impl_type_MetaInfoType.h"
#include "ara/diag/service_interfaces/dm_ipc/impl_type_MetaInfoValueType.h"
#include "ara/diag/service_interfaces/dm_ipc/impl_type_uint32.h"
#include "ara/diag/service_interfaces/dm_ipc/impl_type_uint8.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace dm_ipc {

namespace proxy {

namespace events {}  // namespace events

namespace fields {}  // namespace fields

namespace methods {

/**
 * \brief Data class for service method 'RequestData'.
 * \remark generated
 */
class RequestData {
 public:
  /**
   * \brief Return/output parameters of service method 'RequestData'
   */
  struct Output {
    ara::diag::service_interfaces::dm_ipc::DataArrayType response_data;
  };
};

}  // namespace methods
}  // namespace proxy

namespace skeleton {

namespace events {}  // namespace events

namespace fields {}  // namespace fields

namespace methods {

/**
 * \brief Data class for service method 'RequestData'.
 * \remark generated
 */
class RequestData {
 public:
  /**
   * \brief Return/output parameters of service method 'RequestData'
   */
  struct Output {
    ara::diag::service_interfaces::dm_ipc::DataArrayType response_data;
  };
};

}  // namespace methods
}  // namespace skeleton

}  // namespace dm_ipc
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_DM_IPC_H_

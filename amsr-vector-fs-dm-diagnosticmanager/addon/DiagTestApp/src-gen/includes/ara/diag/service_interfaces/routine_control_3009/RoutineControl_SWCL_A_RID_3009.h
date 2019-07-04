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
/**        \file  RoutineControl_SWCL_A_RID_3009.h
 *        \brief  Header for service 'RoutineControl_SWCL_A_RID_3009'.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTAPP_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_H_
#define ADDON_DIAGTESTAPP_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/internal/types.h"
#include "ara/diag/service_interfaces/routine_control_3009/impl_type_MetaInfoKeyType.h"
#include "ara/diag/service_interfaces/routine_control_3009/impl_type_MetaInfoType.h"
#include "ara/diag/service_interfaces/routine_control_3009/impl_type_MetaInfoValueType.h"
#include "ara/diag/service_interfaces/routine_control_3009/impl_type_UDSResponseCodeType.h"
#include "ara/diag/service_interfaces/routine_control_3009/impl_type_boolean.h"
#include "ara/diag/service_interfaces/routine_control_3009/impl_type_uint32.h"
#include "ara/diag/service_interfaces/routine_control_3009/impl_type_uint8.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace routine_control_3009 {

namespace proxy {

namespace events {}  // namespace events

namespace fields {}  // namespace fields

namespace methods {

/**
 * \brief Data class for service method 'Start'.
 * \remark generated
 */
class Start {
 public:
  /**
   * \brief Return/output parameters of service method 'Start'
   */
  struct Output {
    /* No output parameters */
  };
};

/**
 * \brief Data class for service method 'Stop'.
 * \remark generated
 */
class Stop {
 public:
  /**
   * \brief Return/output parameters of service method 'Stop'
   */
  struct Output {
    ara::diag::service_interfaces::routine_control_3009::uint32 Resp_RemainingCounts;
  };
};

/**
 * \brief Data class for service method 'RequestResults'.
 * \remark generated
 */
class RequestResults {
 public:
  /**
   * \brief Return/output parameters of service method 'RequestResults'
   */
  struct Output {
    ara::diag::service_interfaces::routine_control_3009::uint32 Resp_RemainingCounts;
  };
};

}  // namespace methods
}  // namespace proxy

namespace skeleton {

namespace events {}  // namespace events

namespace fields {}  // namespace fields

namespace methods {

/**
 * \brief Data class for service method 'Start'.
 * \remark generated
 */
class Start {
 public:
  /**
   * \brief Return/output parameters of service method 'Start'
   */
  struct Output {
    /* No output parameters */
  };
};

/**
 * \brief Data class for service method 'Stop'.
 * \remark generated
 */
class Stop {
 public:
  /**
   * \brief Return/output parameters of service method 'Stop'
   */
  struct Output {
    ara::diag::service_interfaces::routine_control_3009::uint32 Resp_RemainingCounts;
  };
};

/**
 * \brief Data class for service method 'RequestResults'.
 * \remark generated
 */
class RequestResults {
 public:
  /**
   * \brief Return/output parameters of service method 'RequestResults'
   */
  struct Output {
    ara::diag::service_interfaces::routine_control_3009::uint32 Resp_RemainingCounts;
  };
};

}  // namespace methods
}  // namespace skeleton

}  // namespace routine_control_3009
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTAPP_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_H_

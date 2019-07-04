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
/**        \file  RoutineControl_SWCL_A_RID_3009_proxy_impl_interface.h
 *        \brief  Proxy implementation interface of service 'RoutineControl_SWCL_A_RID_3009'.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_PROXY_IMPL_INTERFACE_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_PROXY_IMPL_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/future.h"
#include "ara/com/internal/proxy_impl_event.h"
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace routine_control_3009 {

/**
 * \brief Proxy implementation interface for the Service 'RoutineControl_SWCL_A_RID_3009'
 * \remark generated
 */
class RoutineControl_SWCL_A_RID_3009ProxyImplInterface : public ara::com::internal::ProxyImplInterface {
 public:
  /**
   * \brief Default destructor
   */
  virtual ~RoutineControl_SWCL_A_RID_3009ProxyImplInterface() = default;

  /* ---- Methods -------------------------------------------------------------------------------------------------- */

  /**
   * \brief Pure virtual implementation interface of service method 'Start'.
   * \param MetaInfo IN parameter of type MetaInfoType
   * \param Req_Reset IN parameter of type boolean
   * \param Req_CounterLimit IN parameter of type uint32
   */
  virtual ara::com::Future<proxy::methods::Start::Output> MethodStart(MetaInfoType& MetaInfo, boolean& Req_Reset,
                                                                      uint32& Req_CounterLimit) = 0;

  /**
   * \brief Pure virtual implementation interface of service method 'Stop'.
   * \param MetaInfo IN parameter of type MetaInfoType
   * \return ara::com::Future with output struct arguments
   *         Resp_RemainingCounts OUT parameter of type uint32
   */
  virtual ara::com::Future<proxy::methods::Stop::Output> MethodStop(MetaInfoType& MetaInfo) = 0;

  /**
   * \brief Pure virtual implementation interface of service method 'RequestResults'.
   * \param MetaInfo IN parameter of type MetaInfoType
   * \return ara::com::Future with output struct arguments
   *         Resp_RemainingCounts OUT parameter of type uint32
   */
  virtual ara::com::Future<proxy::methods::RequestResults::Output> MethodRequestResults(MetaInfoType& MetaInfo) = 0;

  /* ---- Events --------------------------------------------------------------------------------------------------- */

  /* ---- Fields --------------------------------------------------------------------------------------------------- */
};

}  // namespace routine_control_3009
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_PROXY_IMPL_INTERFACE_H_

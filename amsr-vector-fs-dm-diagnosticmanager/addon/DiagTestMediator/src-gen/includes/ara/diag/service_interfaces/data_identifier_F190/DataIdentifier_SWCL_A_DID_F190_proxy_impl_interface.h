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
/**        \file  DataIdentifier_SWCL_A_DID_F190_proxy_impl_interface.h
 *        \brief  Proxy implementation interface of service 'DataIdentifier_SWCL_A_DID_F190'.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_DATAIDENTIFIER_SWCL_A_DID_F190_PROXY_IMPL_INTERFACE_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_DATAIDENTIFIER_SWCL_A_DID_F190_PROXY_IMPL_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/future.h"
#include "ara/com/internal/proxy_impl_event.h"
#include "ara/diag/service_interfaces/data_identifier_F190/DataIdentifier_SWCL_A_DID_F190.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace data_identifier_F190 {

/**
 * \brief Proxy implementation interface for the Service 'DataIdentifier_SWCL_A_DID_F190'
 * \remark generated
 */
class DataIdentifier_SWCL_A_DID_F190ProxyImplInterface : public ara::com::internal::ProxyImplInterface {
 public:
  /**
   * \brief Default destructor
   */
  virtual ~DataIdentifier_SWCL_A_DID_F190ProxyImplInterface() = default;

  /* ---- Methods -------------------------------------------------------------------------------------------------- */

  /**
   * \brief Pure virtual implementation interface of service method 'Read'.
   * \param MetaInfo IN parameter of type MetaInfoType
   * \return ara::com::Future with output struct arguments
   *         dataRecord_vin OUT parameter of type fixed_size_array_with_17_uint8_items
   */
  virtual ara::com::Future<proxy::methods::Read::Output> MethodRead(MetaInfoType& MetaInfo) = 0;

  /**
   * \brief Pure virtual implementation interface of service method 'Write'.
   * \param MetaInfo IN parameter of type MetaInfoType
   * \param dataRecord_vin IN parameter of type fixed_size_array_with_17_uint8_items
   */
  virtual ara::com::Future<proxy::methods::Write::Output> MethodWrite(
      MetaInfoType& MetaInfo, fixed_size_array_with_17_uint8_items& dataRecord_vin) = 0;

  /**
   * \brief Pure virtual implementation interface of service method 'Cancel'.
   * \param MetaInfo IN parameter of type MetaInfoType
   */
  virtual ara::com::Future<proxy::methods::Cancel::Output> MethodCancel(MetaInfoType& MetaInfo) = 0;

  /* ---- Events --------------------------------------------------------------------------------------------------- */

  /* ---- Fields --------------------------------------------------------------------------------------------------- */
};

}  // namespace data_identifier_F190
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_DATAIDENTIFIER_SWCL_A_DID_F190_PROXY_IMPL_INTERFACE_H_

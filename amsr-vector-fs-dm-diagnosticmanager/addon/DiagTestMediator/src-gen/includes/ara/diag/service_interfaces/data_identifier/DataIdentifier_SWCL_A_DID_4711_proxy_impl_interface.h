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
/**        \file  DataIdentifier_SWCL_A_DID_4711_proxy_impl_interface.h
 *        \brief  Proxy implementation interface of service 'DataIdentifier_SWCL_A_DID_4711'.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_DATAIDENTIFIER_SWCL_A_DID_4711_PROXY_IMPL_INTERFACE_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_DATAIDENTIFIER_SWCL_A_DID_4711_PROXY_IMPL_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/future.h"
#include "ara/com/internal/proxy_impl_event.h"
#include "ara/diag/service_interfaces/data_identifier/DataIdentifier_SWCL_A_DID_4711.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace data_identifier {

/**
 * \brief Proxy implementation interface for the Service 'DataIdentifier_SWCL_A_DID_4711'
 * \remark generated
 */
class DataIdentifier_SWCL_A_DID_4711ProxyImplInterface : public ara::com::internal::ProxyImplInterface {
 public:
  /**
   * \brief Default destructor
   */
  virtual ~DataIdentifier_SWCL_A_DID_4711ProxyImplInterface() = default;

  /* ---- Methods -------------------------------------------------------------------------------------------------- */

  /**
   * \brief Pure virtual implementation interface of service method 'Read'.
   * \param MetaInfo IN parameter of type MetaInfoType
   * \return ara::com::Future with output struct arguments
   *         dataRecord_DataElement_one OUT parameter of type uint8
   *         dataRecord_DataElement_two OUT parameter of type uint16
   */
  virtual ara::com::Future<proxy::methods::Read::Output> MethodRead(MetaInfoType& MetaInfo) = 0;

  /**
   * \brief Pure virtual implementation interface of service method 'Write'.
   * \param MetaInfo IN parameter of type MetaInfoType
   * \param dataRecord_DataElement_one IN parameter of type uint8
   * \param dataRecord_DataElement_two IN parameter of type uint16
   */
  virtual ara::com::Future<proxy::methods::Write::Output> MethodWrite(MetaInfoType& MetaInfo,
                                                                      uint8& dataRecord_DataElement_one,
                                                                      uint16& dataRecord_DataElement_two) = 0;

  /**
   * \brief Pure virtual implementation interface of service method 'Cancel'.
   * \param MetaInfo IN parameter of type MetaInfoType
   */
  virtual ara::com::Future<proxy::methods::Cancel::Output> MethodCancel(MetaInfoType& MetaInfo) = 0;

  /* ---- Events --------------------------------------------------------------------------------------------------- */

  /* ---- Fields --------------------------------------------------------------------------------------------------- */
};

}  // namespace data_identifier
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_DATAIDENTIFIER_SWCL_A_DID_4711_PROXY_IMPL_INTERFACE_H_

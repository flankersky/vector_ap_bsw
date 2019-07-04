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
/**        \file  DataIdentifier_SWCL_A_DID_F190.h
 *        \brief  Header for service 'DataIdentifier_SWCL_A_DID_F190'.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_DATAIDENTIFIER_SWCL_A_DID_F190_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_DATAIDENTIFIER_SWCL_A_DID_F190_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/internal/types.h"
#include "ara/diag/service_interfaces/data_identifier_F190/impl_type_MetaInfoKeyType.h"
#include "ara/diag/service_interfaces/data_identifier_F190/impl_type_MetaInfoType.h"
#include "ara/diag/service_interfaces/data_identifier_F190/impl_type_MetaInfoValueType.h"
#include "ara/diag/service_interfaces/data_identifier_F190/impl_type_UDSResponseCodeType.h"
#include "ara/diag/service_interfaces/data_identifier_F190/impl_type_fixed_size_array_with_17_uint8_items.h"
#include "ara/diag/service_interfaces/data_identifier_F190/impl_type_uint8.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace data_identifier_F190 {

namespace proxy {

namespace events {}  // namespace events

namespace fields {}  // namespace fields

namespace methods {

/**
 * \brief Data class for service method 'Read'.
 * \remark generated
 */
class Read {
 public:
  /**
   * \brief Return/output parameters of service method 'Read'
   */
  struct Output {
    ara::diag::service_interfaces::data_identifier_F190::fixed_size_array_with_17_uint8_items dataRecord_vin;
  };
};

/**
 * \brief Data class for service method 'Write'.
 * \remark generated
 */
class Write {
 public:
  /**
   * \brief Return/output parameters of service method 'Write'
   */
  struct Output {
    /* No output parameters */
  };
};

/**
 * \brief Data class for service method 'Cancel'.
 * \remark generated
 */
class Cancel {
 public:
  /**
   * \brief Return/output parameters of service method 'Cancel'
   */
  struct Output {
    /* No output parameters */
  };
};

}  // namespace methods
}  // namespace proxy

namespace skeleton {

namespace events {}  // namespace events

namespace fields {}  // namespace fields

namespace methods {

/**
 * \brief Data class for service method 'Read'.
 * \remark generated
 */
class Read {
 public:
  /**
   * \brief Return/output parameters of service method 'Read'
   */
  struct Output {
    ara::diag::service_interfaces::data_identifier_F190::fixed_size_array_with_17_uint8_items dataRecord_vin;
  };
};

/**
 * \brief Data class for service method 'Write'.
 * \remark generated
 */
class Write {
 public:
  /**
   * \brief Return/output parameters of service method 'Write'
   */
  struct Output {
    /* No output parameters */
  };
};

/**
 * \brief Data class for service method 'Cancel'.
 * \remark generated
 */
class Cancel {
 public:
  /**
   * \brief Return/output parameters of service method 'Cancel'
   */
  struct Output {
    /* No output parameters */
  };
};

}  // namespace methods
}  // namespace skeleton

}  // namespace data_identifier_F190
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_DATAIDENTIFIER_SWCL_A_DID_F190_H_

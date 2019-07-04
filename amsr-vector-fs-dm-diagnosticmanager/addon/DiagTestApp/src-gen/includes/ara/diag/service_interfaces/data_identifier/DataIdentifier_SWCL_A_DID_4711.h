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
/**        \file  DataIdentifier_SWCL_A_DID_4711.h
 *        \brief  Header for service 'DataIdentifier_SWCL_A_DID_4711'.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTAPP_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_DATAIDENTIFIER_SWCL_A_DID_4711_H_
#define ADDON_DIAGTESTAPP_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_DATAIDENTIFIER_SWCL_A_DID_4711_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/internal/types.h"
#include "ara/diag/service_interfaces/data_identifier/impl_type_MetaInfoKeyType.h"
#include "ara/diag/service_interfaces/data_identifier/impl_type_MetaInfoType.h"
#include "ara/diag/service_interfaces/data_identifier/impl_type_MetaInfoValueType.h"
#include "ara/diag/service_interfaces/data_identifier/impl_type_UDSResponseCodeType.h"
#include "ara/diag/service_interfaces/data_identifier/impl_type_uint16.h"
#include "ara/diag/service_interfaces/data_identifier/impl_type_uint8.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace data_identifier {

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
    ara::diag::service_interfaces::data_identifier::uint8 dataRecord_DataElement_one;
    ara::diag::service_interfaces::data_identifier::uint16 dataRecord_DataElement_two;
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
    ara::diag::service_interfaces::data_identifier::uint8 dataRecord_DataElement_one;
    ara::diag::service_interfaces::data_identifier::uint16 dataRecord_DataElement_two;
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

}  // namespace data_identifier
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTAPP_SRC_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_DATAIDENTIFIER_SWCL_A_DID_4711_H_

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
/**        \file  calculatorInterface.h
 *        \brief  Header for service 'calculatorInterface'.
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADAPTIVECALCULATORSERVER_INCLUDES_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_H_
#define ADAPTIVECALCULATORSERVER_INCLUDES_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/internal/types.h"
#include "vector/calculatorService/impl_type_boolean.h"
#include "vector/calculatorService/impl_type_uint32.h"

namespace vector {
namespace calculatorService {

namespace proxy {

namespace events {}  // namespace events

namespace fields {

/**
 * \brief Data class for service field 'divideResult'.
 * \remark generated
 */
class DivideResult {
 public:
  /**
   * \brief Return/output parameters of service field 'divideResult'
   */
  using Output = vector::calculatorService::uint32;
};

}  // namespace fields

namespace methods {

/**
 * \brief Data class for service method 'subtract'.
 * \remark generated
 */
class Subtract {
 public:
  /**
   * \brief Return/output parameters of service method 'subtract'
   */
  struct Output {
    vector::calculatorService::uint32 result;
  };
};

/**
 * \brief Data class for service method 'add'.
 * \remark generated
 */
class Add {
 public:
  /**
   * \brief Return/output parameters of service method 'add'
   */
  struct Output {
    vector::calculatorService::uint32 result;
  };
};

/**
 * \brief Data class for service method 'divide'.
 * \remark generated
 */
class Divide {
 public:
  /**
   * \brief Return/output parameters of service method 'divide'
   */
  struct Output {
    vector::calculatorService::uint32 result;
  };
};

}  // namespace methods
}  // namespace proxy

namespace skeleton {

namespace events {}  // namespace events

namespace fields {

/**
 * \brief Data class for service field 'divideResult'.
 * \remark generated
 */
class DivideResult {
 public:
  /**
   * \brief Return/output parameters of service field 'divideResult'
   */
  using Output = vector::calculatorService::uint32;
};

}  // namespace fields

namespace methods {

/**
 * \brief Data class for service method 'subtract'.
 * \remark generated
 */
class Subtract {
 public:
  /**
   * \brief Return/output parameters of service method 'subtract'
   */
  struct Output {
    vector::calculatorService::uint32 result;
  };
};

/**
 * \brief Data class for service method 'add'.
 * \remark generated
 */
class Add {
 public:
  /**
   * \brief Return/output parameters of service method 'add'
   */
  struct Output {
    vector::calculatorService::uint32 result;
  };
};

/**
 * \brief Data class for service method 'divide'.
 * \remark generated
 */
class Divide {
 public:
  /**
   * \brief Return/output parameters of service method 'divide'
   */
  struct Output {
    vector::calculatorService::uint32 result;
  };
};

}  // namespace methods
}  // namespace skeleton

}  // namespace calculatorService
}  // namespace vector

#endif  // ADAPTIVECALCULATORSERVER_INCLUDES_VECTOR_CALCULATORSERVICE_CALCULATORINTERFACE_H_

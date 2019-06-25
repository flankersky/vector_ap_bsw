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
/**        \file  proxy_impl_interface.h
 *        \brief  TODO
 *
 *      \details  Proxy implementation interface
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_PROXY_IMPL_INTERFACE_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_PROXY_IMPL_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/internal/types.h"  // Do not include ara/com/types here. This will lead to cyclic includes.

namespace ara {
namespace com {
namespace internal {

/**
 * \brief Interface class for proxy implementations.
 */
class ProxyImplInterface {
 public:
  virtual ~ProxyImplInterface() = default;

  /**
   * \brief Get the ProxyId this ProxyImplInterface can handle.
   * \returns The ProxyId this ProxyImplInterface can handle.
   */
  virtual ara::com::internal::ProxyId GetProxyId() const = 0;
};

}  // namespace internal
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_PROXY_IMPL_INTERFACE_H_

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
/**        \file  service_proxy_factory_interface.h
 *        \brief  TODO
 *
 *      \details  Contains common components used by all user-defined interfaces (proxy)
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_SERVICE_PROXY_FACTORY_INTERFACE_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_SERVICE_PROXY_FACTORY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/com/internal/proxy_impl_interface.h"
#include "ara/com/internal/types.h"  // Do not include ara/com/types here. This will lead to cyclic includes.

#include "vac/container/static_list.h"
#include "vac/memory/intrusive_shared_ptr.h"

namespace ara {
namespace com {
namespace internal {

/**
 * \brief Factory interface for ProxyImplInterfaces
 *
 * This interface shall be implemented by bindings to produce instances of ProxyImplInterfaces associated with a given
 * handle.
 */
class ServiceProxyFactoryInterface : public vac::memory::IntrusiveShared<ServiceProxyFactoryInterface> {
 public:
  /**
   * \brief Destructor
   */
  virtual ~ServiceProxyFactoryInterface() = default;

  /**
   * \brief Factory method for a ProxyImplInterface object
   */
  virtual std::unique_ptr<ProxyImplInterface> create() const = 0;
};

}  // namespace internal
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_SERVICE_PROXY_FACTORY_INTERFACE_H_

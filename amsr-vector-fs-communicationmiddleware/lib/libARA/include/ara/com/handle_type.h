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
 *  ----------------------------------------------------------------------------------------------------------------*/
/**        \file  handle_type.h
 *        \brief
 *
 *      \details  ara::com::HandleType
 *          \see  SWS_CM_00312
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_HANDLE_TYPE_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_HANDLE_TYPE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <tuple>
#include <utility>

#include "ara/com/instance_identifier.h"
#include "ara/com/internal/service_proxy_factory_interface.h"
#include "ara/com/internal/types.h"

namespace ara {
namespace com {

/**
 * \brief Specifies handle type that specifies what a handle looks like
 * \see SWS_CM_00312
 */
class HandleType {
 public:
  /**
   * \brief The constructor sets a pointer to the interface, for creating the binding-specific part of a proxy on
   * instantiation.
   * \param instance_id The ara::com instance identifier assigned...
   * \param interface A smart pointer to the binding-related proxy factory.
   */
  HandleType(ara::com::InstanceIdentifier instance_id, internal::ServiceProxyFactoryInterface::shared_ptr& interface)
      : instance_id_{instance_id}, interface_{interface} {}
  /**
   * \brief Returns a reference to an instance identifier
   * SWS_CM_00312
   */
  const ara::com::InstanceIdentifier& GetInstanceId() const { return instance_id_; }
  /**
   * \brief Getter for the proxy factory interface.
   */
  internal::ServiceProxyFactoryInterface::shared_ptr GetInterface() const { return interface_; }

 private:
  /// The ara::com instance identifier assigned to this handle after a FindService / StartFindService
  ara::com::InstanceIdentifier instance_id_;

  /// A concrete object of HandleType is the owner of an interface to create the binding-related part
  /// of a proxy.
  internal::ServiceProxyFactoryInterface::shared_ptr interface_;
};

}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_HANDLE_TYPE_H_

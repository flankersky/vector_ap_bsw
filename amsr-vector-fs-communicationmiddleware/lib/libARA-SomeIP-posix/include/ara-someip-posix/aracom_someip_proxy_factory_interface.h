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
/**        \file  aracom_someip_proxy_factory_interface.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_ARACOM_SOMEIP_PROXY_FACTORY_INTERFACE_H_
#define LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_ARACOM_SOMEIP_PROXY_FACTORY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/com/instance_identifier.h"
#include "ara/com/internal/proxy_impl_interface.h"
#include "ara/com/internal/types.h"
#include "someip-posix-common/someip/someip_posix_types.h"
#include "vac/container/intrusive_list.h"

namespace ara {
namespace com {
namespace someip_posix {

/**
 * \brief Factory class for ara::com::internal::ProxyImplInterface for a specific service.
 *
 * Note that the Service ID is encoded in the actual Factory implementation and therefore does not show up in the
 * interface.
 */
class AraComSomeIpProxyFactoryInterface : public vac::container::IntrusiveListNode<AraComSomeIpProxyFactoryInterface> {
 public:
  /**
   * Constructor
   */
  AraComSomeIpProxyFactoryInterface() : vac::container::IntrusiveListNode<AraComSomeIpProxyFactoryInterface>() {}
  /**
   * Destructor
   */
  virtual ~AraComSomeIpProxyFactoryInterface() = default;

  /**
   * \brief Getter for the SOME/IP ServiceId for which this Factory will produce ProxyImplInterface instances.
   *
   * \return The SOME/IP Service ID of the produced ProxyImplInterface instances.
   */
  virtual someip_posix_common::someip::ServiceId GetSomeIpServiceId() const = 0;

  /**
   * \brief Getter for the ara::com ProxyId for which this Factory will produce ProxyImplInterface instances.
   *
   * \return The ara::com Service ID of the produced ProxyImplInterface instances.
   */
  virtual ara::com::internal::ProxyId GetProxyId() const = 0;

  /**
   * \brief Factory method for ara::com::internal::ProxyImplInterface instances.
   *
   * \param instance_id The SOME/IP Instance ID to use for the created ProxyImplInterface.
   *
   * \return A ProxyImplInterface for the given Service ID and Instance ID.
   */
  virtual std::unique_ptr<ara::com::internal::ProxyImplInterface> create(
      someip_posix_common::someip::InstanceId instance_id) const = 0;
};

}  // namespace someip_posix
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_ARACOM_SOMEIP_PROXY_FACTORY_INTERFACE_H_

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
/**        \file  aracom_someip_proxy_instance_factory.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_ARACOM_SOMEIP_PROXY_INSTANCE_FACTORY_H_
#define LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_ARACOM_SOMEIP_PROXY_INSTANCE_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <memory>
#include "ara-someip-posix/aracom_someip_proxy_factory_interface.h"
#include "ara/com/internal/service_proxy_factory_interface.h"
#include "ara/com/types.h"
#include "vac/container/static_list.h"

namespace ara {
namespace com {
namespace someip_posix {

/**
 * \brief SOME/IP proxy instance factory.
 *
 * Instances of this factory are used for the instantiation of instance id specific ServiceProxySomeIpBinding objects.
 * In case of InstanceIdentifier::Any one AraComSomeIpProxyInstanceFactory instance will be created for every available
 * instance id.
 */
class AraComSomeIpProxyInstanceFactory : public internal::ServiceProxyFactoryInterface {
 public:
  /// Pool to store object instances in
  using PoolType = vac::container::StaticList<AraComSomeIpProxyInstanceFactory>;

  /**
   * \brief Constructor
   * \param instance_id Instance identifier of the proxy to be used.
   * \param someip_proxy_factory Related AraComSomeIpProxyFactory to be used for creation of the ProxyImplInterface
   * \param pool The storage pool to delete this object from when calling CallDeleter().
   */
  explicit AraComSomeIpProxyInstanceFactory(someip_posix_common::someip::InstanceId instance_id,
                                            AraComSomeIpProxyFactoryInterface* someip_proxy_factory, PoolType& pool)
      : internal::ServiceProxyFactoryInterface(),
        instance_id_(instance_id),
        someip_proxy_factory_(someip_proxy_factory),
        pool_(pool) {}

  /**
   * \brief Factory method for a ProxyImplInterface object
   */
  std::unique_ptr<internal::ProxyImplInterface> create() const override;

  /**
   * \brief Compares the service interface and service instance this factory will create to the given parameters.
   *
   * \param instance_id The InstanceIdentifier to compare to
   * \param someip_proxy_factory The Proxy Factory to compare to
   *
   * \return True if the given parameters match the service instance and service interface this factory will create.
   * false otherwise.
   */
  bool compare(const someip_posix_common::someip::InstanceId instance_id,
               const AraComSomeIpProxyFactoryInterface* someip_proxy_factory) const {
    return instance_id_ == instance_id && someip_proxy_factory == someip_proxy_factory_;
  }

  /**
   * \brief Remove this object from the containing pool.
   *
   * Calls the deleter. The object is invalid after this call.
   */
  void CallDeleter() override {
    PoolType::iterator it = std::find_if(
        pool_.begin(), pool_.end(), [this](AraComSomeIpProxyInstanceFactory& factory) { return &factory == this; });
    pool_.erase(it);
  }

 private:
  /// Instance identifier which will be created by this factory
  someip_posix_common::someip::InstanceId instance_id_;

  /// Related AraComSomeIpProxyFactory used for creation of the ProxyImplInterface
  AraComSomeIpProxyFactoryInterface* someip_proxy_factory_;

  /// The PoolType that allocated this object
  PoolType& pool_;
};

}  // namespace someip_posix
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_ARACOM_SOMEIP_PROXY_INSTANCE_FACTORY_H_

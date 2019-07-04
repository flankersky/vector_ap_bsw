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
/**        \file  aracom_someip_skeleton_factory_interface.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_ARACOM_SOMEIP_SKELETON_FACTORY_INTERFACE_H_
#define LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_ARACOM_SOMEIP_SKELETON_FACTORY_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include "ara/com/internal/skeleton_impl_interface.h"
#include "ara/com/internal/skeleton_interface.h"
#include "someip-posix-common/someip/someip_posix_types.h"

namespace ara {
namespace com {
namespace someip_posix {

/**
 * \brief Factory class for ara::com::internal::SkeletonImplInterface for a specific service.
 */
class AraComSomeIpSkeletonFactoryInterface
    : public vac::container::IntrusiveListNode<AraComSomeIpSkeletonFactoryInterface> {
 public:
  AraComSomeIpSkeletonFactoryInterface() : vac::container::IntrusiveListNode<AraComSomeIpSkeletonFactoryInterface>() {}
  virtual ~AraComSomeIpSkeletonFactoryInterface() = default;

  /**
   * \brief Getter for the SOME/IP ServiceId for which this Factory will produce SkeletonImplInterface instances.
   *
   * \return The SOME/IP Service ID of the produced SkeletonImplInterface instances.
   */
  virtual someip_posix_common::someip::ServiceId GetSomeIpServiceId() const = 0;

  /**
   * \brief Getter for the ara::com SkeletonId for which this Factory will produce SkeletonImplInterface instances.
   *
   * \return The ara::com Service ID of the produced SkeletonImplInterface instances.
   */
  virtual ara::com::internal::SkeletonId GetSkeletonId() const = 0;

  /**
   * \brief Factory method for ara::com::internal::SkeletonImplInterface instances.
   *
   * \param instance_id The SOME/IP Instance ID to use for the created SkeletonImplInterface.
   * \param skeleton The related frontend skeleton implementation
   *
   * \return A SkeletonImplInterface for the given Service ID and Instance ID.
   */
  virtual std::unique_ptr<ara::com::internal::SkeletonImplInterface> create(
      InstanceId instance_id, ara::com::internal::SkeletonInterface& skeleton) const = 0;
};

}  // namespace someip_posix
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_ARACOM_SOMEIP_SKELETON_FACTORY_INTERFACE_H_

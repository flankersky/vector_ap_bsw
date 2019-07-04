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
/**        \file  aracom_someip_skeleton_interface.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_ARACOM_SOMEIP_SKELETON_INTERFACE_H_
#define LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_ARACOM_SOMEIP_SKELETON_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/types.h"
#include "someip-posix-common/someip/marshalling.h"
#include "someip-posix/someip_posix.h"

/**
 * \brief Type-alias for shortening.
 */
using RootDeserializerAlias = ::someip_posix_common::someip::serialization::Deserializer<
    ::someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>;

namespace ara {
namespace com {
namespace someip_posix {

/**
 * \brief Interface class for ara::com SOME/IP skeleton implementations.
 */
class AraComSomeIpSkeletonInterface {
 public:
  virtual ~AraComSomeIpSkeletonInterface() = default;

  /**
   * \brief Receive handler for method requests.
   *
   * \param header Deserialized SOME/IP header.
   * \param deserializer Root deserializer holding the serialized packet.
   */
  virtual void HandleMethodRequest(const ::someip_posix_common::someip::SomeIpMessageHeader& header,
                                   RootDeserializerAlias&& deserializer) = 0;
};

}  // namespace someip_posix
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_ARACOM_SOMEIP_SKELETON_INTERFACE_H_

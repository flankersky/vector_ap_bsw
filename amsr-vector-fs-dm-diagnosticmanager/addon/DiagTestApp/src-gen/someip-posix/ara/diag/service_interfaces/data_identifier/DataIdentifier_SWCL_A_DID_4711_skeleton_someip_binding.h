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
/**        \file  DataIdentifier_SWCL_A_DID_4711_skeleton_someip_binding.h
 *        \brief  SOME/IP binding of service skeleton for service 'DataIdentifier_SWCL_A_DID_4711'
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTAPP_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_DATAIDENTIFIER_SWCL_A_DID_4711_SKELETON_SOMEIP_BINDING_H_
#define ADDON_DIAGTESTAPP_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_DATAIDENTIFIER_SWCL_A_DID_4711_SKELETON_SOMEIP_BINDING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include <utility>
#include "ara-someip-posix/aracom_someip_binding_interface.h"
#include "ara-someip-posix/aracom_someip_skeleton_interface.h"
#include "ara/com/promise.h"
#include "ara/diag/service_interfaces/data_identifier/DataIdentifier_SWCL_A_DID_4711_skeleton.h"
#include "ara/diag/service_interfaces/data_identifier/DataIdentifier_SWCL_A_DID_4711_skeleton_impl_interface.h"
#include "ara/diag/service_interfaces/data_identifier/DataIdentifier_SWCL_A_DID_4711_skeleton_someip_event_manager.h"
#include "ara/diag/service_interfaces/data_identifier/DataIdentifier_SWCL_A_DID_4711_skeleton_someip_method_manager.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace data_identifier {

/**
 * \brief SOME/IP skeleton binding of Service 'DataIdentifier_SWCL_A_DID_4711'.
 * \details Handles serialization and deserialization of all method calls, events, etc.
 * \remark generated
 */
class DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpBinding
    : public DataIdentifier_SWCL_A_DID_4711SkeletonImplInterface,
      public ara::com::someip_posix::AraComSomeIpSkeletonInterface {
 public:
  /**
   * \brief Generated SOME/IP related service ID.
   */
  static constexpr ::someip_posix_common::someip::ServiceId kServiceId{0x840U};

  /**
   * \brief SOME/IP major version of this service interface deployment.
   */
  static constexpr ::someip_posix_common::someip::MajorVersion kMajorVersion{1U};

  /**
   * \brief Constructor
   * \param instance_id SOME/IP service instance ID offered by this skeleton
   * \param someip_binding Reference to the related SOME/IP transport binding
   * \param skeleton Reference to the related frontend skeleton
   */
  explicit DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpBinding(
      ::someip_posix_common::someip::InstanceId instance_id,
      ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding,
      DataIdentifier_SWCL_A_DID_4711Skeleton& skeleton);

  /**
   * \brief Default destructor must de-register the implementation from the AraComSomeIpBinding.
   */
  ~DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpBinding();

  /**
   * \brief Get the SkeletonId this SkeletonImplInterface can handle.
   * \returns The SkeletonId this SkeletonImplInterface can handle.
   */
  ara::com::internal::SkeletonId GetSkeletonId() const override {
    return DataIdentifier_SWCL_A_DID_4711Skeleton::GetSkeletonIdStatic();
  }

  /**
   * \brief Getter for accessing the related frontend skeleton.
   */
  DataIdentifier_SWCL_A_DID_4711Skeleton& GetSkeleton() const { return skeleton_; }

  /**
   * \brief Type-alias for shortening.
   */
  using RootDeserializerAlias = ::someip_posix_common::someip::serialization::Deserializer<
      ::someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>;

  /**
   * \brief Receive handler for method requests.
   *
   * \param header Deserialized SOME/IP header.
   * \param deserializer Root deserializer holding the packet buffer.
   */
  void HandleMethodRequest(const ::someip_posix_common::someip::SomeIpMessageHeader& header,
                           RootDeserializerAlias&& deserializer) override;

  /**
   * \brief Forward a prepared packet to the AraComSomeIpBinding.
   * \param packet the serialized response packet.
   */
  void SendMethodResponse(::someip_posix_common::someip::SomeIpPacket&& packet);

  /**
   * \brief Is needed for infrastructural faults such as:
   * Deserialization fails or the service/method isn't implemented at all.
   * \tparam ReturnCode The error return code as template parameter to make a
   * compile-time check.
   * \param request_header The SOME/IP header from the request, which is used
   * to build the error response.
   */
  template <typename ::someip_posix_common::someip::ReturnCode ReturnCode>
  void SendErrorResponseOnMethodRequest(const ::someip_posix_common::someip::SomeIpMessageHeader& request_header) {
    static_assert((ReturnCode >= ::someip_posix_common::someip::SomeIpReturnCode::kNotOk) &&
                      (ReturnCode <= ::someip_posix_common::someip::SomeIpReturnCode::kRangeServiceErrorsEnd),
                  "Return code for errors must be in the range of 0x01 .. 0x5E.");
    someip_binding_.SendErrorResponseOnMethodRequest(ReturnCode, instance_id_, request_header);
  }

  /**
   * \brief Send an error response if the SOME/IP request is malformed. This method is invoked
   * when the return code of the error response is only known during runtime.
   * \param return_code The return code to send.
   * \param request_header The header taken from the request.
   */
  void SendErrorResponseOnMethodRequest(const ::someip_posix_common::someip::ReturnCode return_code,
                                        const ::someip_posix_common::someip::SomeIpMessageHeader& request_header) {
    someip_binding_.SendErrorResponseOnMethodRequest(return_code, instance_id_, request_header);
  }

  /**
   * \brief Send an event notification
   * \param packet SOME/IP packet with header and payload.
   */
  void SendEventNotification(::someip_posix_common::someip::SomeIpPacket&& packet);

  /* ---- Events --------------------------------------------------------------------------------------------------- */

  /* ---- Fields --------------------------------------------------------------------------------------------------- */

 private:
  /** SOME/IP instance ID offered by this skeleton */
  ::someip_posix_common::someip::InstanceId instance_id_;

  /** Related SOME/IP binding */
  ara::com::someip_posix::AraComSomeIpBindingInterface& someip_binding_;

  /** Reference to the related frontend skeleton */
  DataIdentifier_SWCL_A_DID_4711Skeleton& skeleton_;

  /* ---- Methods manager ------------------------------------------------------------------------------------------ */
  /** Method request/response manager for skeleton method 'Read' */
  DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerRead method_manager_Read_;

  /** Method request/response manager for skeleton method 'Write' */
  DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerWrite method_manager_Write_;

  /** Method request/response manager for skeleton method 'Cancel' */
  DataIdentifier_SWCL_A_DID_4711SkeletonSomeIpMethodManagerCancel method_manager_Cancel_;

  /* ---- Event manager -------------------------------------------------------------------------------------------- */
  /* ---- Field manager -------------------------------------------------------------------------------------------- */
};

}  // namespace data_identifier
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTAPP_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_DATAIDENTIFIER_SWCL_A_DID_4711_SKELETON_SOMEIP_BINDING_H_

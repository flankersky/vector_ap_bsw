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
/**        \file  DM_IPC_skeleton_someip_method_manager.h
 *        \brief  SOME/IP skeleton method de- /serialization handling for methods and field methods of service 'DM_IPC'
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_DM_IPC_SKELETON_SOMEIP_METHOD_MANAGER_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_DM_IPC_SKELETON_SOMEIP_METHOD_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include "ara/com/promise.h"
#include "ara/diag/service_interfaces/dm_ipc/DM_IPC.h"
#include "ara/diag/service_interfaces/dm_ipc/DM_IPC_skeleton.h"
#include "someip-posix-common/someip/marshalling.h"
#include "someip-posix-common/someip/message.h"
#include "someip-posix/someip_posix.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace dm_ipc {

// Forward-declaration for back-reference
class DM_IPCSkeletonSomeIpBinding;

/**
 * \brief Context storage for the response path for method 'RequestData'.
 */
struct DM_IPCSomeIpResponsePathInfoMethodManagerRequestData {
  /// Header from the request with client ID and session ID
  const ::someip_posix_common::someip::SomeIpMessageHeader& header_;
  /// Response type
  const skeleton::methods::RequestData::Output& out_val_;
};

/**
 * \brief SOME/IP Skeleton method manager for method 'RequestData'.
 * \details Handles SOME/IP de-/serialization.
 * \remark generated
 */
class DM_IPCSkeletonSomeIpMethodManagerRequestData {
 public:
  /**
   * \brief SOME/IP method identifier for this method.
   */
  static constexpr ::someip_posix_common::someip::MethodId kMethodId{0x64U};

  /**
   * \brief Constructor sets a reference to DM_IPCProxySomeIpBinding, for sending out
   * the serialized packets.
   * \param skeleton_binding Related SOME/IP skeleton binding to be used
   */
  explicit DM_IPCSkeletonSomeIpMethodManagerRequestData(DM_IPCSkeletonSomeIpBinding& skeleton_binding);

  /**
   * \brief Type-alias for shortening.
   */
  using RootDeserializerAlias = ::someip_posix_common::someip::serialization::Deserializer<
      ::someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>;

  /**
   * \brief Return the expected SOME/IP header for this request.
   * \return The expected request header to check service id, method id etc.
   * \note The session ID can not be verified and is set to zero as the session id is
   * incremented for each method request.
   */
  const ::someip_posix_common::someip::SomeIpMessageHeader& GetExpectedRequestHeader() const;

  /**
   * \brief Deserialization of service method 'RequestData'.
   * \details Stores the request into the map and calls the frontend.
   *
   * \param header Deserialized SOME/IP header.
   * \param deserializer Root deserializer holding the serialized byte stream.
   */
  void HandleMethodRequest(const ::someip_posix_common::someip::SomeIpMessageHeader& header,
                           RootDeserializerAlias&& deserializer);

  /**
   * \brief Is called from the skeleton, when the method has been processed in the
   * frontend and the response value shall be forwarded to the client application
   * that made this method request.
   * \param response This structure contains the output value from the method invocation
   * and binding-related information from the request such as the SOME/IP header for SOME/IP.
   */
  void SendMethodResponse(const DM_IPCSomeIpResponsePathInfoMethodManagerRequestData& response);

  /**
   * \brief Is called from the skeleton, when the method has been processed in the
   * frontend and the application error 'Failed' was set by the application.
   * The method performs the application error specific argument serialization and sends the SOME/IP response message.
   * \param header Header from the request with client ID and session ID
   * possible error context arguments.
   */
  void SendMethodResponseFailed(const ::someip_posix_common::someip::SomeIpMessageHeader& header);

 private:
  /** To send method responses */
  DM_IPCSkeletonSomeIpBinding& skeleton_binding_;

  /** Mutex for method requests from multiple threads */
  std::mutex pending_lock_;
};

}  // namespace dm_ipc
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_DM_IPC_SKELETON_SOMEIP_METHOD_MANAGER_H_

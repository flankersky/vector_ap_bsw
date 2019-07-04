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
/**        \file  DM_IPC_proxy_someip_method_manager.h
 *        \brief  SOME/IP proxy method de- /serialization handling for methods and field methods of service 'DM_IPC'
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_DM_IPC_PROXY_SOMEIP_METHOD_MANAGER_H_
#define SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_DM_IPC_PROXY_SOMEIP_METHOD_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include "ara/com/promise.h"
#include "ara/diag/service_interfaces/dm_ipc/DM_IPC.h"
#include "someip-posix-common/someip/marshalling.h"
#include "someip-posix/someip_posix.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace dm_ipc {

// Forward-declaration for back-reference
class DM_IPCProxySomeIpBinding;

/**
 * \brief SOME/IP Proxy method manager for method 'RequestData'.
 * \details Handles SOME/IP de-/serialization.
 * \remark generated
 */
class DM_IPCProxySomeIpMethodManagerRequestData {
 public:
  /**
   * \brief Promise type
   */
  using Promise = ara::com::Promise<proxy::methods::RequestData::Output>;

  /**
   * \brief Holds all pending method request promises based on a unique session id.
   * Every time a proxy method call occurs, this map is extended.
   */
  using PendingRequests = std::map<::someip_posix_common::someip::SessionId, Promise>;

  /**
   * \brief SOME/IP method identifier for this method.
   */
  static constexpr ::someip_posix_common::someip::MethodId kMethodId{0x64U};

  /**
   * \brief Constructor sets a reference to DM_IPCProxySomeIpBinding, for sending out
   * the serialized packets.
   * \param proxy_binding Related SOME/IP proxy binding to be used
   */
  explicit DM_IPCProxySomeIpMethodManagerRequestData(DM_IPCProxySomeIpBinding& proxy_binding);

  /**
   * \brief Serialization of of service method 'RequestData'
   * \param handle Input argument of type ara::diag::service_interfaces::dm_ipc::uint32
   * \param meta_info Input argument of type ara::diag::service_interfaces::dm_ipc::MetaInfoType
   * \param request_data Input argument of type ara::diag::service_interfaces::dm_ipc::DataArrayType
   * \return ara::com::Future for return value of service method 'RequestData'
   **/
  ara::com::Future<proxy::methods::RequestData::Output> HandleMethodRequest(uint32& handle, MetaInfoType& meta_info,
                                                                            DataArrayType& request_data);

  /**
   * \brief Type-alias for shortening.
   */
  using RootDeserializerAlias = ::someip_posix_common::someip::serialization::Deserializer<
      ::someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>;

  /**
   * \brief Deserialization of service method 'RequestData'.
   *
   * \param header The deserialized SOME/IP header for direct access.
   * \param deserializer Root deserializer containing the bytestream to deserialize.
   */
  void HandleMethodResponse(const ::someip_posix_common::someip::SomeIpMessageHeader& header,
                            RootDeserializerAlias&& deserializer);

 private:
  /**
   * \brief Deserialize response and set promise values in case a valid response with return code OK was received.
   * \tparam RootDeserializer Type of used root deserializer
   * \param deserializer root deserializer with already consumed SOME/IP header
   * \param p Frontend method call promise
   */
  template <typename RootDeserializer>
  void HandleMethodResponseOk(RootDeserializer&& deserializer, Promise&& p);

  /*
   * \brief Deserialize response and set promise with application error 'Failed'
   * \param p Frontend method call promise
   */
  void HandleMethodResponseFailed(Promise&& p);

  /** Collection of pending requests for method 'RequestData' */
  PendingRequests pending_requests_;

  /** A proxy method has its own session id, which is incremented every method request */
  ::someip_posix_common::someip::SessionId next_session_id_;

  /** To send method requests */
  DM_IPCProxySomeIpBinding& proxy_binding_;

  /** Mutex for method requests from multiple threads */
  std::mutex pending_requests_lock_;
};

}  // namespace dm_ipc
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DM_IPC_DM_IPC_PROXY_SOMEIP_METHOD_MANAGER_H_

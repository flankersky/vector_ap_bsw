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
/**        \file  DataIdentifier_SWCL_A_DID_F190_skeleton_someip_method_manager.h
 *        \brief  SOME/IP skeleton method de- /serialization handling for methods and field methods of service
 *'DataIdentifier_SWCL_A_DID_F190'
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTAPP_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_DATAIDENTIFIER_SWCL_A_DID_F190_SKELETON_SOMEIP_METHOD_MANAGER_H_
#define ADDON_DIAGTESTAPP_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_DATAIDENTIFIER_SWCL_A_DID_F190_SKELETON_SOMEIP_METHOD_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include "ara/com/promise.h"
#include "ara/diag/service_interfaces/data_identifier_F190/DataIdentifier_SWCL_A_DID_F190.h"
#include "ara/diag/service_interfaces/data_identifier_F190/DataIdentifier_SWCL_A_DID_F190_skeleton.h"
#include "someip-posix-common/someip/marshalling.h"
#include "someip-posix-common/someip/message.h"
#include "someip-posix/someip_posix.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace data_identifier_F190 {

// Forward-declaration for back-reference
class DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpBinding;

/**
 * \brief Context storage for the response path for method 'Read'.
 */
struct DataIdentifier_SWCL_A_DID_F190SomeIpResponsePathInfoMethodManagerRead {
  /// Header from the request with client ID and session ID
  const ::someip_posix_common::someip::SomeIpMessageHeader& header_;
  /// Response type
  const skeleton::methods::Read::Output& out_val_;
};

/**
 * \brief SOME/IP Skeleton method manager for method 'Read'.
 * \details Handles SOME/IP de-/serialization.
 * \remark generated
 */
class DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpMethodManagerRead {
 public:
  /**
   * \brief SOME/IP method identifier for this method.
   */
  static constexpr ::someip_posix_common::someip::MethodId kMethodId{0x73U};

  /**
   * \brief Constructor sets a reference to DataIdentifier_SWCL_A_DID_F190ProxySomeIpBinding, for sending out
   * the serialized packets.
   * \param skeleton_binding Related SOME/IP skeleton binding to be used
   */
  explicit DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpMethodManagerRead(
      DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpBinding& skeleton_binding);

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
   * \brief Deserialization of service method 'Read'.
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
  void SendMethodResponse(const DataIdentifier_SWCL_A_DID_F190SomeIpResponsePathInfoMethodManagerRead& response);

  /**
   * \brief Is called from the skeleton, when the method has been processed in the
   * frontend and the application error 'UDSServiceFailedOnRead' was set by the application.
   * The method performs the application error specific argument serialization and sends the SOME/IP response message.
   * \param header Header from the request with client ID and session ID
   * \param application_error The application error set by the application. This object contains the error code and the
   * possible error context arguments.
   */
  void SendMethodResponseUDSServiceFailedOnRead(
      const ::someip_posix_common::someip::SomeIpMessageHeader& header,
      const skeleton::application_errors::UDSServiceFailedOnRead& application_error);

 private:
  /** To send method responses */
  DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpBinding& skeleton_binding_;

  /** Mutex for method requests from multiple threads */
  std::mutex pending_lock_;
};

/**
 * \brief Context storage for the response path for method 'Write'.
 */
struct DataIdentifier_SWCL_A_DID_F190SomeIpResponsePathInfoMethodManagerWrite {
  /// Header from the request with client ID and session ID
  const ::someip_posix_common::someip::SomeIpMessageHeader& header_;
};

/**
 * \brief SOME/IP Skeleton method manager for method 'Write'.
 * \details Handles SOME/IP de-/serialization.
 * \remark generated
 */
class DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpMethodManagerWrite {
 public:
  /**
   * \brief SOME/IP method identifier for this method.
   */
  static constexpr ::someip_posix_common::someip::MethodId kMethodId{0x74U};

  /**
   * \brief Constructor sets a reference to DataIdentifier_SWCL_A_DID_F190ProxySomeIpBinding, for sending out
   * the serialized packets.
   * \param skeleton_binding Related SOME/IP skeleton binding to be used
   */
  explicit DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpMethodManagerWrite(
      DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpBinding& skeleton_binding);

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
   * \brief Deserialization of service method 'Write'.
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
  void SendMethodResponse(const DataIdentifier_SWCL_A_DID_F190SomeIpResponsePathInfoMethodManagerWrite& response);

  /**
   * \brief Is called from the skeleton, when the method has been processed in the
   * frontend and the application error 'UDSServiceFailedOnWrite' was set by the application.
   * The method performs the application error specific argument serialization and sends the SOME/IP response message.
   * \param header Header from the request with client ID and session ID
   * \param application_error The application error set by the application. This object contains the error code and the
   * possible error context arguments.
   */
  void SendMethodResponseUDSServiceFailedOnWrite(
      const ::someip_posix_common::someip::SomeIpMessageHeader& header,
      const skeleton::application_errors::UDSServiceFailedOnWrite& application_error);

 private:
  /** To send method responses */
  DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpBinding& skeleton_binding_;

  /** Mutex for method requests from multiple threads */
  std::mutex pending_lock_;
};

/**
 * \brief Context storage for the response path for method 'Cancel'.
 */
struct DataIdentifier_SWCL_A_DID_F190SomeIpResponsePathInfoMethodManagerCancel {
  /// Header from the request with client ID and session ID
  const ::someip_posix_common::someip::SomeIpMessageHeader& header_;
};

/**
 * \brief SOME/IP Skeleton method manager for method 'Cancel'.
 * \details Handles SOME/IP de-/serialization.
 * \remark generated
 */
class DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpMethodManagerCancel {
 public:
  /**
   * \brief SOME/IP method identifier for this method.
   */
  static constexpr ::someip_posix_common::someip::MethodId kMethodId{0x75U};

  /**
   * \brief Constructor sets a reference to DataIdentifier_SWCL_A_DID_F190ProxySomeIpBinding, for sending out
   * the serialized packets.
   * \param skeleton_binding Related SOME/IP skeleton binding to be used
   */
  explicit DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpMethodManagerCancel(
      DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpBinding& skeleton_binding);

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
   * \brief Deserialization of service method 'Cancel'.
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
  void SendMethodResponse(const DataIdentifier_SWCL_A_DID_F190SomeIpResponsePathInfoMethodManagerCancel& response);

 private:
  /** To send method responses */
  DataIdentifier_SWCL_A_DID_F190SkeletonSomeIpBinding& skeleton_binding_;

  /** Mutex for method requests from multiple threads */
  std::mutex pending_lock_;
};

}  // namespace data_identifier_F190
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTAPP_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_DATAIDENTIFIER_SWCL_A_DID_F190_SKELETON_SOMEIP_METHOD_MANAGER_H_

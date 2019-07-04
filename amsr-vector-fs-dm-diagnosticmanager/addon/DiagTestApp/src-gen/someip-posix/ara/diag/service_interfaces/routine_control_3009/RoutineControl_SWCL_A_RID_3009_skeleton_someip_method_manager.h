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
/**        \file  RoutineControl_SWCL_A_RID_3009_skeleton_someip_method_manager.h
 *        \brief  SOME/IP skeleton method de- /serialization handling for methods and field methods of service
 *'RoutineControl_SWCL_A_RID_3009'
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTAPP_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_SKELETON_SOMEIP_METHOD_MANAGER_H_
#define ADDON_DIAGTESTAPP_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_SKELETON_SOMEIP_METHOD_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include "ara/com/promise.h"
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009.h"
#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009_skeleton.h"
#include "someip-posix-common/someip/marshalling.h"
#include "someip-posix-common/someip/message.h"
#include "someip-posix/someip_posix.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace routine_control_3009 {

// Forward-declaration for back-reference
class RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding;

/**
 * \brief Context storage for the response path for method 'Start'.
 */
struct RoutineControl_SWCL_A_RID_3009SomeIpResponsePathInfoMethodManagerStart {
  /// Header from the request with client ID and session ID
  const ::someip_posix_common::someip::SomeIpMessageHeader& header_;
};

/**
 * \brief SOME/IP Skeleton method manager for method 'Start'.
 * \details Handles SOME/IP de-/serialization.
 * \remark generated
 */
class RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStart {
 public:
  /**
   * \brief SOME/IP method identifier for this method.
   */
  static constexpr ::someip_posix_common::someip::MethodId kMethodId{0x76U};

  /**
   * \brief Constructor sets a reference to RoutineControl_SWCL_A_RID_3009ProxySomeIpBinding, for sending out
   * the serialized packets.
   * \param skeleton_binding Related SOME/IP skeleton binding to be used
   */
  explicit RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStart(
      RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding& skeleton_binding);

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
   * \brief Deserialization of service method 'Start'.
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
  void SendMethodResponse(const RoutineControl_SWCL_A_RID_3009SomeIpResponsePathInfoMethodManagerStart& response);

  /**
   * \brief Is called from the skeleton, when the method has been processed in the
   * frontend and the application error 'UDSServiceFailedOnStart' was set by the application.
   * The method performs the application error specific argument serialization and sends the SOME/IP response message.
   * \param header Header from the request with client ID and session ID
   * \param application_error The application error set by the application. This object contains the error code and the
   * possible error context arguments.
   */
  void SendMethodResponseUDSServiceFailedOnStart(
      const ::someip_posix_common::someip::SomeIpMessageHeader& header,
      const skeleton::application_errors::UDSServiceFailedOnStart& application_error);

 private:
  /** To send method responses */
  RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding& skeleton_binding_;

  /** Mutex for method requests from multiple threads */
  std::mutex pending_lock_;
};

/**
 * \brief Context storage for the response path for method 'Stop'.
 */
struct RoutineControl_SWCL_A_RID_3009SomeIpResponsePathInfoMethodManagerStop {
  /// Header from the request with client ID and session ID
  const ::someip_posix_common::someip::SomeIpMessageHeader& header_;
  /// Response type
  const skeleton::methods::Stop::Output& out_val_;
};

/**
 * \brief SOME/IP Skeleton method manager for method 'Stop'.
 * \details Handles SOME/IP de-/serialization.
 * \remark generated
 */
class RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStop {
 public:
  /**
   * \brief SOME/IP method identifier for this method.
   */
  static constexpr ::someip_posix_common::someip::MethodId kMethodId{0x77U};

  /**
   * \brief Constructor sets a reference to RoutineControl_SWCL_A_RID_3009ProxySomeIpBinding, for sending out
   * the serialized packets.
   * \param skeleton_binding Related SOME/IP skeleton binding to be used
   */
  explicit RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerStop(
      RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding& skeleton_binding);

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
   * \brief Deserialization of service method 'Stop'.
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
  void SendMethodResponse(const RoutineControl_SWCL_A_RID_3009SomeIpResponsePathInfoMethodManagerStop& response);

  /**
   * \brief Is called from the skeleton, when the method has been processed in the
   * frontend and the application error 'UDSServiceFailedOnStop' was set by the application.
   * The method performs the application error specific argument serialization and sends the SOME/IP response message.
   * \param header Header from the request with client ID and session ID
   * \param application_error The application error set by the application. This object contains the error code and the
   * possible error context arguments.
   */
  void SendMethodResponseUDSServiceFailedOnStop(
      const ::someip_posix_common::someip::SomeIpMessageHeader& header,
      const skeleton::application_errors::UDSServiceFailedOnStop& application_error);

 private:
  /** To send method responses */
  RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding& skeleton_binding_;

  /** Mutex for method requests from multiple threads */
  std::mutex pending_lock_;
};

/**
 * \brief Context storage for the response path for method 'RequestResults'.
 */
struct RoutineControl_SWCL_A_RID_3009SomeIpResponsePathInfoMethodManagerRequestResults {
  /// Header from the request with client ID and session ID
  const ::someip_posix_common::someip::SomeIpMessageHeader& header_;
  /// Response type
  const skeleton::methods::RequestResults::Output& out_val_;
};

/**
 * \brief SOME/IP Skeleton method manager for method 'RequestResults'.
 * \details Handles SOME/IP de-/serialization.
 * \remark generated
 */
class RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerRequestResults {
 public:
  /**
   * \brief SOME/IP method identifier for this method.
   */
  static constexpr ::someip_posix_common::someip::MethodId kMethodId{0x78U};

  /**
   * \brief Constructor sets a reference to RoutineControl_SWCL_A_RID_3009ProxySomeIpBinding, for sending out
   * the serialized packets.
   * \param skeleton_binding Related SOME/IP skeleton binding to be used
   */
  explicit RoutineControl_SWCL_A_RID_3009SkeletonSomeIpMethodManagerRequestResults(
      RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding& skeleton_binding);

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
   * \brief Deserialization of service method 'RequestResults'.
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
  void SendMethodResponse(
      const RoutineControl_SWCL_A_RID_3009SomeIpResponsePathInfoMethodManagerRequestResults& response);

  /**
   * \brief Is called from the skeleton, when the method has been processed in the
   * frontend and the application error 'UDSServiceFailedOnRequestResults' was set by the application.
   * The method performs the application error specific argument serialization and sends the SOME/IP response message.
   * \param header Header from the request with client ID and session ID
   * \param application_error The application error set by the application. This object contains the error code and the
   * possible error context arguments.
   */
  void SendMethodResponseUDSServiceFailedOnRequestResults(
      const ::someip_posix_common::someip::SomeIpMessageHeader& header,
      const skeleton::application_errors::UDSServiceFailedOnRequestResults& application_error);

 private:
  /** To send method responses */
  RoutineControl_SWCL_A_RID_3009SkeletonSomeIpBinding& skeleton_binding_;

  /** Mutex for method requests from multiple threads */
  std::mutex pending_lock_;
};

}  // namespace routine_control_3009
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTAPP_SRC_GEN_SOMEIP_POSIX_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_SKELETON_SOMEIP_METHOD_MANAGER_H_

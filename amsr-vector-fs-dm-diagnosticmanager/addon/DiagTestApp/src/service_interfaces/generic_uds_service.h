/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  generic_uds_service.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTAPP_SRC_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_H_
#define ADDON_DIAGTESTAPP_SRC_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ara/diag/service_interfaces/generic_uds_service/GenericUDSService_skeleton.h"

namespace diag_test_application {
namespace service_interfaces {

// shorthand for namespace of GenericUDSService skeleton
namespace guds = ara::diag::service_interfaces::generic_uds_service;

/**
 * \brief List of Negative Response Codes (NRC) used in this context.
 * See Table A.1 in ISO 14229-1 for more details and other NRCs.
 */
enum class NegativeResponseCodes : guds::uint8 {
  /** SFNS: sub-functionNotSupported
   * This NRC indicates that the requested action will not be taken because the server
   * does not support the service specific parameters of the request message.
   * The server shall send this NRC in case the client has sent a request message with
   * a known and supported service identifier but with "sub-function“ which is either
   * unknown or not supported.
   */
  kSNS = 0x11,
  /** IMLOIF: incorrectMessageLengthOrInvalidFormat
   * This NRC shall be sent if the length of the message is wrong.
   */
  kIMLOIF = 0x13,
  /** SNSIAS: serviceNotSupportedInActiveSession  SNSIAS
   * This NRC indicates that the requested action will not be taken because the server
   * does not support the requested service in the session currently active. This NRC
   * shall only be used when the requested service is known to be supported in another
   * session, otherwise response code SNS (serviceNotSupported) shall be used (e.g.,
   * servers executing the boot software generally do not know which services are
   * supported in the application (and vice versa) and therefore may need to respond
   * with NRC 0x11 instead).
   */
  kSNSIAS = 0x7F
};

/**
 * \brief Implementation class for GenericUDSService skeleton.
 * This implementation of the GenericUDSService skeleton is intended to show the concept
 * of writing Diagnostic Manager extensions. The current implementation is a demo of
 * the UDS SessionService ($10). The demo implementation returns  that
 */
class GenericUDSService : public guds::GenericUDSServiceSkeleton {
 public:
  /**
   * \brief Service Identifier for Response Pending tests (0xBA).
   * The service identifier 0xBA is intended for internal testing only.
   * \remarks The Service ID range from 0xBA to 0xBE can be used by system suppliers.
   */
  static constexpr guds::uint8 kResponsePendingTestServiceId = 0xBA;
  /**
   * Modifier that is used to create a Positive Response Service Identifier.
   */
  static constexpr guds::uint8 kPositiveResponseServiceIdModifier = 0x40;

  /**
   * \brief Constructor.
   * \param[in] instance_id instance identifier.
   */
  explicit GenericUDSService(const ara::com::InstanceIdentifier& instance_id)
      : GenericUDSServiceSkeleton(instance_id) {}

  virtual ~GenericUDSService() {}

  explicit GenericUDSService(const GenericUDSService&) = delete;

  /**
   * \brief Service method of generic uds service.
   */
  ara::com::Future<guds::skeleton::methods::Service::Output> Service(const guds::uint8& SID,
                                                                     const guds::DataArrayType& RequestData,
                                                                     const guds::MetaInfoType& MetaInfo) override;

 private:
  /**
   * \brief Creates a positive Response for the 0xBA request.
   * \param[in] request_data The request data after the service ID.
   * \param[out] response Reference to vector used for response.
   */
  static void Create0xBAPosResponse(const guds::DataArrayType& request_data, guds::DataArrayType& response);

  /**
   * \brief Creates a negative response with given NRC.
   * \param[in] promise The promise to set the future.
   * \param[in] nrc The nrc of the response.
   */
  static void CreateNRC(ara::com::Promise<guds::skeleton::methods::Service::Output>& promise,
                        NegativeResponseCodes nrc);

  FRIEND_TEST(GenericUdsServiceTest, CheckCreate0xBAPosResponse);
  FRIEND_TEST(GenericUdsServiceTest, CheckCreateNRCWithSubfunctionNotSupported);
  FRIEND_TEST(GenericUdsServiceTest, CheckCreateNRCWithIncorrectMessageLengthOrInvalidFormat);
  FRIEND_TEST(GenericUdsServiceTest, CheckCreateNRCWithServiceNotSupportedInActiveSession);
};

}  // namespace service_interfaces
}  // namespace diag_test_application

#endif  // ADDON_DIAGTESTAPP_SRC_SERVICE_INTERFACES_GENERIC_UDS_SERVICE_H_

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
/**        \file  generic_service_processor.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_PROCESSOR_GENERIC_SERVICE_PROCESSOR_H_
#define SRC_SERVER_SERVICE_PROCESSOR_GENERIC_SERVICE_PROCESSOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>
#include <memory>
#include <utility>

#include "ara/diag/service_interfaces/generic_uds_service/GenericUDSService.h"
#include "ara/diag/service_interfaces/generic_uds_service/GenericUDSService_proxy.h"
#include "server/conversation/conversation.h"
#include "server/service/processor/service_processor_base.h"
#include "vac/memory/optional.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace processor {
/**
 * \brief Size in byte of SID.
 */
constexpr std::size_t kSizeSID = 1;

/**
 * \brief Size in byte of SID.
 */
constexpr std::uint8_t kUdsResponseOffset = 0x40;

/**
 * \brief Implementation of ServiceProcessor interface.
 */
class GenericServiceProcessor : public ServiceProcessorBase {
 public:
  using ServiceProcessorBase::ServiceProcessorBase;

  GenericServiceProcessor(const GenericServiceProcessor& that) = delete;
  GenericServiceProcessor& operator=(const GenericServiceProcessor& that) = delete;
  /**
   * \brief Handle the uds_message.
   */
  ProcessingStatus HandleMessage() override;

  /**
   * \brief Cancel.
   */
  void Cancel() override {}

  /**
   * \brief On state change.
   */
  void OnStateChange() override {}

  /**
   * \brief Post handling.
   */
  void PostHandling() override {}

 private:
  /**
   * \brief This method is called if HandleMessage is called for the first time.
   */
  void HandleMessageFirstCall();

  /**
   * \brief Handle the case, when future<Service::Output> is ready.
   */
  void HandleFutureReady();

  /**
   * \brief Generic UDS service proxy.
   */
  vac::memory::optional<ara::diag::service_interfaces::generic_uds_service::GenericUDSServiceProxy>
      generic_uds_service_proxy_;

  /**
   * \brief Future containing NRC code of called service.
   */
  ara::com::Future<ara::diag::service_interfaces::generic_uds_service::proxy::methods::Service::Output>
      future_service_output_;
};
}  // namespace processor
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_PROCESSOR_GENERIC_SERVICE_PROCESSOR_H_

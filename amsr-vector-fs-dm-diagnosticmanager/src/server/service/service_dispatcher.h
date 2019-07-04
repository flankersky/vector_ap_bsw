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
/**        \file  service_dispatcher.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_SERVICE_DISPATCHER_H_
#define SRC_SERVER_SERVICE_SERVICE_DISPATCHER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <map>
#include <utility>

#include "diagpreconditions/diag_preconditions.h"
#include "server/conversation/conversation.h"
#include "server/service/handler/session_control_handler.h"
#include "server/service/processor/service_processor.h"
#include "server/service/service_processing_context.h"
#include "udstransport/uds_message.h"
#include "udstransport/uds_negative_response_code.h"
#include "vac/container/static_map.h"
#include "vac/memory/three_phase_allocator.h"

#include "server/diagnostic_server_configuration.h"
#include "server/service/handler/generic_service_handler.h"
#include "server/service/handler/read_did_service_handler.h"
#include "server/service/handler/routine/routine_control_handler.h"
#include "server/service/handler/tester_present_handler.h"
#include "server/service/handler/write_did_service_handler.h"
#include "server/service/service_table.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {

/**
 * \brief Implementation of service dispatcher.
 */
class ServiceDispatcher {
 public:
  /**
   * \brief Constructor.
   * \param server_config diagnostic server configuration
   * \param dext_config dext configuration
   */
  ServiceDispatcher(const DiagnosticServerConfiguration& server_config,
                    const amsr::diag::configuration::DextConfiguration& dext_config);
  ServiceDispatcher(const ServiceDispatcher& that) = delete;
  ServiceDispatcher& operator=(const ServiceDispatcher&) = delete;
  ServiceDispatcher(ServiceDispatcher&&) = delete;
  ServiceDispatcher& operator=(ServiceDispatcher&&) = delete;

  VIRTUALMOCK ~ServiceDispatcher() = default;

  /**
   * \brief Type definition of a pair of processor and negative response code.
   */
  using PairProcessorNRC =
      std::pair<processor::ServiceProcessor::Ptr, ara::diag::udstransport::UdsNegativeResponseCode>;

  /**
   * \brief Method called during shutdown phase.
   */
  void Shutdown();

  /**
    * \brief Creates a new ServiceProcessor for the given UdsMessage with the given processing context.
    * \param message UDS message
    * \param processing_context processing context for this message
    * \return pair of negative response code and service processor if successful, otherwise ServiceProcessor is
    * nullptr
    */
  VIRTUALMOCK PairProcessorNRC FindServiceProcessor(ara::diag::udstransport::UdsMessage::Ptr message,
                                                    ServiceProcessingContext& processing_context);

  /**
   * \brief Returns the service table.
   */
  ServiceTable& GetServiceTable() { return service_table_; }

 private:
  /**
   * \brief This method is returning true if the SID is supported in active sessions.
   * Otherwise false is returned.
   */
  bool IsSidSupportedActiveSession(ServiceProcessingContext& processing_context, std::uint8_t sid);
  /**
   * \brief Type definition for three phase allocator.
   */
  template <typename T>
  using ThreePhaseAllocator = vac::memory::ThreePhaseAllocator<T>;

  /**
   * \brief Type definition for Preconditions.
   */
  using Preconditions = diagpreconditions::DiagPreconditions;

  /**
   * \brief Type def for map associating the SID with the preconditions.
   */
  using ServicePreconditionsMap = vac::container::StaticMap<std::uint8_t, Preconditions>;
  /**
   * \brief Service table.
   */
  ServiceTable service_table_;

  /**
   * \brief Map associating the SID with the preconditions.
   */
  ServicePreconditionsMap service_preconditions_map_;
};

}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_SERVICE_DISPATCHER_H_

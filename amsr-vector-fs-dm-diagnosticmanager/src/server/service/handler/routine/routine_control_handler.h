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
/**        \file  routine_control_handler.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_HANDLER_ROUTINE_ROUTINE_CONTROL_HANDLER_H_
#define SRC_SERVER_SERVICE_HANDLER_ROUTINE_ROUTINE_CONTROL_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <server/service/handler/routine/routine_types.h>
#include "configuration/dext_configuration.h"
#include "ridmanager/rid_manager.h"
#include "server/service/handler/service_handler.h"
#include "server/service/processor/routine_control_processor.h"
#include "udstransport/uds_message.h"
#include "vac/memory/smart_base_type_object_pool.h"
#include "vac/testing/test_adapter.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {
namespace routine {
/**
 * \brief Diagnostic server handler for routine control service.
 */
class RoutineControlHandler : public ServiceHandler {
 public:
  /**
   * Configuration parameters for WriteDidServiceHandler.
   */
  struct Configuration final {
    /** Base configuration */
    ServiceHandler::Configuration base;

    /** Configuration from dext **/
    const configuration::DextConfiguration::Rids& rids_table;
  };

  /**
   * \brief Constructor.
   * \param config configuration parameter
   */
  explicit RoutineControlHandler(const Configuration config);

  /**
   *\brief Destructor.
   */
  virtual ~RoutineControlHandler() = default;

  RoutineControlHandler(const RoutineControlHandler& that) = delete;
  RoutineControlHandler& operator=(const RoutineControlHandler&) = delete;
  RoutineControlHandler(RoutineControlHandler&&) = delete;
  RoutineControlHandler& operator=(RoutineControlHandler&&) = delete;

  /**
     * \brief Creates a new RoutineControlProcessor for the given UdsMessage with the given processing context.
     * \remark Will return processor at each call until the maximum number of service processors is reached.
     * \param message UDS message. Must Not be nullptr. payload_size must be at least 1.
     * \param processing_context processing context for this message
     * \return Service processor if successful, otherwise nullptr
     */
  processor::ServiceProcessor::Ptr CreateServiceProcessor(ara::diag::udstransport::UdsMessage::Ptr message,
                                                          ServiceProcessingContext& processing_context) override;

 private:
  /**
   * \brief Factory for Service Processors.
   */
  vac::memory::SmartBaseTypeObjectPool<processor::RoutineControlProcessor> processor_factory_;

  /**
   * \brief RID Manager
   */
  ridmanager::RidManager rid_manager_;

 private:
  FRIEND_TEST(RoutineControlHandler, CallToConstructor);
};

}  // namespace routine
}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr
#endif  // SRC_SERVER_SERVICE_HANDLER_ROUTINE_ROUTINE_CONTROL_HANDLER_H_

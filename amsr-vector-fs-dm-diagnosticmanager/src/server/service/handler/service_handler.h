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
/**        \file  service_handler.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_HANDLER_SERVICE_HANDLER_H_
#define SRC_SERVER_SERVICE_HANDLER_SERVICE_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <memory>

#include "server/conversation/conversation.h"
#include "server/diagnostic_server_configuration.h"
#include "server/service/processor/service_processor.h"
#include "server/service/service_processing_context.h"
#include "udstransport/uds_message.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {

/**
 * \brief ServiceHandler interface.
 */
class ServiceHandler {
 public:
  /**
   * Configuration parameters for ServiceHandler.
   */
  struct Configuration final {
    /** Maximum number of service processors. */
    std::size_t max_number_of_service_processors;
  };

  /**
   * \brief Enum for Handler type.
   */
  enum class HandlerType : std::uint8_t { kInternal = 0x00, kExternal = 0x01 };

  ServiceHandler() = default;

  /**
   * \brief Destructor.
   */
  virtual ~ServiceHandler() = default;

  ServiceHandler(const ServiceHandler& that) = delete;
  ServiceHandler& operator=(const ServiceHandler&) = delete;
  ServiceHandler(ServiceHandler&&) = delete;
  ServiceHandler& operator=(ServiceHandler&&) = delete;

  /**
   * \brief Creates a new ServiceProcessor for the given UdsMessage with the given processing context.
   * \remark Will return processor at each call until the maximum number of service processors is reached.
   * \param message UDS message. Must Not be nullptr. payload_size must be at least 1.
   * \param processing_context processing context for this message
   * \return Service processor if successful, otherwise nullptr
   */
  virtual processor::ServiceProcessor::Ptr CreateServiceProcessor(ara::diag::udstransport::UdsMessage::Ptr message,
                                                                  ServiceProcessingContext& processing_context) = 0;
};

}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_HANDLER_SERVICE_HANDLER_H_

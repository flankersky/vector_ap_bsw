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
/**        \file  generic_service_handler.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_HANDLER_GENERIC_SERVICE_HANDLER_H_
#define SRC_SERVER_SERVICE_HANDLER_GENERIC_SERVICE_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "server/service/handler/service_handler.h"
#include "server/service/processor/generic_service_processor.h"
#include "udstransport/uds_message.h"
#include "vac/memory/smart_base_type_object_pool.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {

/**
 * \brief Diagnostic Service Handler for Generic UDS service.
 */
class GenericServiceHandler : public ServiceHandler {
 public:
  /**
   * Configuration parameters for GenericServiceHandler.
   */
  struct Configuration final {
    /** Base configuration */
    ServiceHandler::Configuration base;
  };

  /**
   * \brief Constructor.
   * \param config configuration parameter.
   */
  explicit GenericServiceHandler(const Configuration config);

  virtual ~GenericServiceHandler() = default;

  GenericServiceHandler(const GenericServiceHandler& that) = delete;
  GenericServiceHandler& operator=(const GenericServiceHandler&) = delete;
  GenericServiceHandler(GenericServiceHandler&&) = delete;
  GenericServiceHandler& operator=(GenericServiceHandler&&) = delete;

  /**
    * \brief Creates a new GenericServiceProcessor for the given UdsMessage with the given processing context.
    * \remark Will return processor at each call until the maximum number of service processors is reached (defined in
    * the constructor configuration).
    * \param message UDS message
    * \param processing_context processing context for this message
    * \return Service processor if successful, otherwise nullptr
    */
  processor::ServiceProcessor::Ptr CreateServiceProcessor(ara::diag::udstransport::UdsMessage::Ptr message,
                                                          ServiceProcessingContext& processing_context) override;

 private:
  /**
   * Factory for Service Processors.
   */
  vac::memory::SmartBaseTypeObjectPool<processor::GenericServiceProcessor> processor_factory_;
};

}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_HANDLER_GENERIC_SERVICE_HANDLER_H_

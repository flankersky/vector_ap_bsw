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
/**        \file  read_did_service_handler.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_HANDLER_READ_DID_SERVICE_HANDLER_H_
#define SRC_SERVER_SERVICE_HANDLER_READ_DID_SERVICE_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "server/service/handler/service_handler.h"
#include "server/service/processor/read_did_processor.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {

/**
 * \brief Diagnostic Service Handler for UDS service 0x22.
 */
class ReadDidServiceHandler : public ServiceHandler {
 public:
  /**
   * Configuration parameters for ReadDidServiceHandler.
   */
  struct Configuration final {
    /** Base configuration */
    ServiceHandler::Configuration base;
    /** Maximum number of requested DIDs in a single UDS message. */
    std::size_t maximum_number_of_dids;
  };

  /**
   * \brief Constructor.
   * \param config configuration parameter.
   */
  explicit ReadDidServiceHandler(const Configuration config);
  virtual ~ReadDidServiceHandler() = default;

  ReadDidServiceHandler(const ReadDidServiceHandler& that) = delete;
  ReadDidServiceHandler& operator=(const ReadDidServiceHandler&) = delete;
  ReadDidServiceHandler(ReadDidServiceHandler&&) = delete;
  ReadDidServiceHandler& operator=(ReadDidServiceHandler&&) = delete;

  /**
    * \brief Creates a new ReadDidServiceProcessor for the given UdsMessage with the given processing context.
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
   * Configuration parameter.
   */
  const Configuration config_;
  /**
   * Factory for Service Processors.
   */
  vac::memory::SmartBaseTypeObjectPool<processor::ReadDidProcessor> processor_factory_;
};

}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_HANDLER_READ_DID_SERVICE_HANDLER_H_

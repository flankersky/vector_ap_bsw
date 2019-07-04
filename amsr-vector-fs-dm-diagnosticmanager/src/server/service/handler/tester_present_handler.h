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
/**
 *     \file  tester_present_handler.h
 *     \brief  Contains the definition of the TesterPresentHandler
 *
 *     \details ServiceHandler implementation for Tester Present requests (0x3E)
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_HANDLER_TESTER_PRESENT_HANDLER_H_
#define SRC_SERVER_SERVICE_HANDLER_TESTER_PRESENT_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "server/service/handler/service_handler.h"
#include "server/service/processor/tester_present_processor.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace handler {

/**
 * \brief Diagnostic Service Handler for UDS service 0x3E.
 */
class TesterPresentHandler : public ServiceHandler {
 public:
  /**
   * Configuration parameters for TesterPresentHandler.
   */
  struct Configuration final {
    /** Base configuration */
    ServiceHandler::Configuration base;
  };

  /**
   * \brief Constructor.
   * \param config configuration parameter.
   */
  explicit TesterPresentHandler(const Configuration config);
  virtual ~TesterPresentHandler() = default;

  TesterPresentHandler(const TesterPresentHandler& that) = delete;
  TesterPresentHandler& operator=(const TesterPresentHandler&) = delete;
  TesterPresentHandler(TesterPresentHandler&&) = delete;
  TesterPresentHandler& operator=(TesterPresentHandler&&) = delete;

  /**
   * \brief Creates a new SessionServiceProcessor for the given UdsMessage with the given processing context.
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
  vac::memory::SmartBaseTypeObjectPool<processor::TesterPresentProcessor> processor_factory_;
};

}  // namespace handler
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_HANDLER_TESTER_PRESENT_HANDLER_H_

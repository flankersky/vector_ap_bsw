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
/**        \file  service_processor_base.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_PROCESSOR_SERVICE_PROCESSOR_BASE_H_
#define SRC_SERVER_SERVICE_PROCESSOR_SERVICE_PROCESSOR_BASE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <assert.h>
#include <utility>

#include "server/service/processor/service_processor.h"
#include "server/service/service_processing_context.h"
#include "udstransport/uds_negative_response_code.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace processor {

/**
 * \brief Abstract base class for service processors.
 */
class ServiceProcessorBase : public ServiceProcessor {
 public:
  /**
   * \brief Constructor.
   * Sets the DeleterContext to the provided.
   * \param uds_message UDS message, the ServiceProcessor takes ownership of.
   * must be at least 1.
   * \param processing_context reference to the corresponding conversation
   * \param deleter_context DeleterContext
   */
  ServiceProcessorBase(ara::diag::udstransport::UdsMessage::Ptr uds_message,
                       ServiceProcessingContext& processing_context,
                       vac::memory::SmartObjectPoolDeleterContext* deleter_context)
      : ServiceProcessor(deleter_context),
        uds_message_(std::move(uds_message)),
        processing_context_(processing_context) {
    assert(uds_message_ != nullptr);
    assert(uds_message_->GetPayload().size() >= 1);
  }

  ServiceProcessorBase(const ServiceProcessorBase& that) = delete;
  ServiceProcessorBase& operator=(const ServiceProcessorBase& that) = delete;

 protected:
  /**
   * \brief Finish the current processing by calling FinishProcessing of processing_context.
   * \param nrc negative response code.
   * \return the processing status.
   */
  ProcessingStatus FinishServiceProcessing(ara::diag::udstransport::UdsNegativeResponseCode nrc) {
    processing_context_.FinishProcessing(nrc);
    return ProcessingStatus::kDone;
  }

  /**
   * \brief Pointer to the UDS Message.
   */
  ara::diag::udstransport::UdsMessage::Ptr uds_message_;

  /**
   * \brief Reference to the ServiceProcessingContext.
   */
  ServiceProcessingContext& processing_context_;
};

}  // namespace processor
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_PROCESSOR_SERVICE_PROCESSOR_BASE_H_

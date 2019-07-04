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
/**        \file  service_processor.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_PROCESSOR_SERVICE_PROCESSOR_H_
#define SRC_SERVER_SERVICE_PROCESSOR_SERVICE_PROCESSOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "server/conversation/conversation.h"
#include "udstransport/uds_message.h"
#include "vac/memory/smart_base_type_object_pool.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
namespace processor {
/**
 * \brief Enum for processing Status/
 */
enum class ProcessingStatus : std::uint8_t { kDone = 0x00, kNotDone = 0x01 };

/**
 * \brief ServiceProcessor interface.
 */
class ServiceProcessor : public vac::memory::DeleteableSmartPoolObject {
 public:
  /**
   * \brief Unique pointer to ServiceProcessor.
   */
  using Ptr = vac::memory::SmartBaseTypeObjectPoolUniquePtr<ServiceProcessor>;

  /**
   * \brief Default Constructor.
   * Sets the DeleterContext to default.
   */
  ServiceProcessor() : DeleteableSmartPoolObject(nullptr) {}

  /**
   * Constructor with the provided DeleterContext.
   * \param context DeleterContext
   */
  explicit ServiceProcessor(vac::memory::SmartObjectPoolDeleterContext* context) : DeleteableSmartPoolObject(context) {}

  ServiceProcessor(const ServiceProcessor& that) = delete;
  ServiceProcessor& operator=(const ServiceProcessor& that) = delete;

  /**
   * \brief Destructor.
   */
  virtual ~ServiceProcessor() = default;

  /**
   * \brief Handle the uds_message.
   */
  virtual ProcessingStatus HandleMessage() = 0;

  /**
   * \brief Cancel.
   */
  virtual void Cancel() = 0;

  /**
   * \brief On state change.
   */
  virtual void OnStateChange() = 0;

  /**
   * \brief Post handling.
   */
  virtual void PostHandling() = 0;
};
}  // namespace processor
}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_PROCESSOR_SERVICE_PROCESSOR_H_

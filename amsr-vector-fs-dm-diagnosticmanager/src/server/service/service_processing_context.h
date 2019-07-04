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
/**        \file  service_processing_context.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_SERVICE_PROCESSING_CONTEXT_H_
#define SRC_SERVER_SERVICE_SERVICE_PROCESSING_CONTEXT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "server/proxys/proxys_pool.h"
#include "server/service/processor/context/conversation_state.h"
#include "server/service/processor/context/uds_response.h"

namespace amsr {
namespace diag {
namespace server {
namespace data {
class DidManager;  // forward declaration
}  // namespace data
namespace service {

/**
 * \brief Interface to the UDS message processing pipeline.
 */
class ServiceProcessingContext : public processor::context::UdsResponse, public processor::context::ConversationState {
 public:
  /**
   * \brief Destructor.
   */
  virtual ~ServiceProcessingContext() = default;

  /**
   *\brief Return a reference to the DiD Manager.
   */
  virtual data::DidManager& GetDidManager() const = 0;

  /**
   * \brief Return a reference to the proxys pool.
   */
  virtual proxys::ProxysPool& GetProxysPool() const = 0;
};

}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_SERVICE_PROCESSING_CONTEXT_H_

/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2018 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  did_data_element_writer.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "did_data_element_writer.h"
#include "ara/log/logging.hpp"

namespace amsr {
namespace diag {
namespace server {
namespace data {
namespace operationhandler {

void DidDataElementWriter::Select(server::service::handler::ServiceHandler::HandlerType handler_type) {
  // TODO(visbft): Should we throw exception here ? Should the delegate pointer be null if there is no available polling
  // task ?
  using HandlerType = server::service::handler::ServiceHandler::HandlerType;

  switch (handler_type) {
    case HandlerType::kInternal:
      throw std::runtime_error("DidDataElementWriter::Select: No polling task available for internal handler");
    case HandlerType::kExternal:
      ara::log::LogDebug() << "DidDataElementWriter::" << __func__ << ": polling task for mediator selected.";
      delegate_ = &mediator_polling_task_;
      break;
    default:
      throw std::runtime_error("DidDataElementWriter::Select: Handler type is unknown");
  }
}
}  // namespace operationhandler
}  // namespace data
}  // namespace server
}  // namespace diag
}  // namespace amsr

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
/**        \file  did_data_element_info.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>

#include "did_data_element_info.h"

namespace amsr {
namespace diag {
namespace server {
namespace data {

// Definition of the static member as well as the declaration have to be provided. The declaration and the initializer
// go inside the class definition, but the member definition has to be separate.
constexpr char DidDataElementInfo::kInternalStr[];

constexpr char DidDataElementInfo::kExternalStr[];

DidDataElementInfo::DidDataElementInfo(const configuration::DataElementConfiguration& data_elem_conf)
    : min_length_(data_elem_conf.min_length), max_length_(data_elem_conf.max_length) {
  handler_type_ = GetHandlerTypeFromString(data_elem_conf.handler);
}

service::handler::ServiceHandler::HandlerType DidDataElementInfo::GetHandlerTypeFromString(
    const amsr::diag::configuration::HandlerInfo& handler_info) {
  const std::string& handler_type = handler_info.handler_type;
  if (handler_type.compare(kInternalStr) == 0) {
    return service::handler::ServiceHandler::HandlerType::kInternal;
  }

  if (handler_type.compare(kExternalStr) == 0) {
    return service::handler::ServiceHandler::HandlerType::kExternal;
  }

  // Handler Type is unknown. Internal is used as default.
  ara::log::LogError() << "ServiceTable::" << __func__ << " : Handler type is unknown. Internal is used as default";
  return service::handler::ServiceHandler::HandlerType::kInternal;
}

}  // namespace data
}  // namespace server
}  // namespace diag
}  // namespace amsr

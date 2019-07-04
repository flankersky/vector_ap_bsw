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
/**        \file  service_table.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "service_table.h"

#include <algorithm>
#include <string>

namespace amsr {
namespace diag {
namespace server {
namespace service {

ServiceTable::ServiceTable(const DiagnosticServerConfiguration& server_configuration,
                           const amsr::diag::configuration::DextConfiguration& dext_config)
    : server_config(server_configuration) {
  // Allocate handlers specified in the configuration file.
  service_table_map.reserve(dext_config.services.size());
  for (amsr::diag::configuration::DextConfiguration::ServicesArray::const_reference service : dext_config.services) {
    this->AllocateHandler(GetHandlerType(service.handler_info), service.id, dext_config);
  }
}

void ServiceTable::Shutdown() {
  // deallocate all handlers by clearing the map since unique pointers are contained.
  service_table_map.clear();
}

handler::ServiceHandler* ServiceTable::GetHandlerIfAvailable(std::uint8_t sid) {
  // Find the handler.
  ServiceTableMap::iterator it = service_table_map.find(sid);

  // Return the pointer to the handler.
  if (it != service_table_map.end()) {
    return (it->second).get();
  }
  ara::log::LogWarn() << "ServiceTable::" << __func__ << " : service handler with sid : " << sid
                      << " is not available.";
  return nullptr;
}

void ServiceTable::AllocateHandler(handler::ServiceHandler::HandlerType handler_type, std::uint8_t sid,
                                   const amsr::diag::configuration::DextConfiguration& dext_config) {
  switch (handler_type) {
    case handler::ServiceHandler::HandlerType::kExternal: {
      // The handler type is external and it means that the handler associated with this sid is the generic service
      // hander.
      ara::log::LogDebug() << "ServiceTable::" << __func__ << " : Handler type is external.";
      //  Allocate GenericUDS handler for this sid.
      using GenericServiceHandler = server::service::handler::GenericServiceHandler;
      ThreePhaseAllocator<GenericServiceHandler> allocator;
      GenericServiceHandler* handler_raw_ptr = allocator.allocate(1);
      allocator.construct(handler_raw_ptr, generic_service_config);

      HandlerUniquePtr generic_service_handler_ptr(handler_raw_ptr, ServiceHandlerDeleter());

      // Add this handler to the map.
      service_table_map.emplace(sid, std::move(generic_service_handler_ptr));
      break;
    }
    case handler::ServiceHandler::HandlerType::kInternal: {
      // The handler type is internal service handler based on id is used.
      ara::log::LogDebug() << "ServiceTable::" << __func__ << " : Handler type is internal.";
      // Allocate internal handler for this sid.
      AllocateHandlerInternal(sid, dext_config);
      break;
    }
    default:
      ara::log::LogError() << "ServiceTable::" << __func__ << " : Unknown handler type.";
      break;
  }
}

void ServiceTable::AllocateHandlerInternal(std::uint8_t sid,
                                           const amsr::diag::configuration::DextConfiguration& dext_config) {
  switch (sid) {
    case 0x10: {
      ara::log::LogDebug() << "ServiceTable::" << __func__ << " : Session Service handler to be created.";
      //  Allocate SessionControlHandler for this sid.
      const handler::SessionControlHandler::Configuration session_service_config{base_service_config, server_config};
      using SessionControlHandler = server::service::handler::SessionControlHandler;
      ThreePhaseAllocator<SessionControlHandler> allocator;
      SessionControlHandler* handler_raw_ptr = allocator.allocate(1);
      allocator.construct(handler_raw_ptr, session_service_config);

      HandlerUniquePtr handler_ptr = HandlerUniquePtr(handler_raw_ptr, ServiceHandlerDeleter());

      // Add this handler to the map.
      service_table_map.emplace(sid, std::move(handler_ptr));
      break;
    }
    case 0x22: {
      ara::log::LogDebug() << "ServiceTable::" << __func__ << " : ReadDidServiceHandler to be created.";
      //  Allocate ReadDidServiceHandler for this sid.
      /** Read DID service config */
      const handler::ReadDidServiceHandler::Configuration read_did_service_config{
          base_service_config, dext_config.dids_table.max_number_dids_to_read};
      using ReadDidServiceHandler = server::service::handler::ReadDidServiceHandler;
      ThreePhaseAllocator<ReadDidServiceHandler> allocator;
      ReadDidServiceHandler* handler_raw_ptr = allocator.allocate(1);
      allocator.construct(handler_raw_ptr, read_did_service_config);

      HandlerUniquePtr handler_ptr = HandlerUniquePtr(handler_raw_ptr, ServiceHandlerDeleter());

      // Add this handler to the map.
      service_table_map.emplace(sid, std::move(handler_ptr));
      break;
    }
    case 0x2E: {
      ara::log::LogDebug() << "ServiceTable::" << __func__ << " : WriteDidServiceHandler to be created.";
      //  Allocate WriteDidServiceHandler for this sid.
      using WriteDidServiceHandler = server::service::handler::WriteDidServiceHandler;
      ThreePhaseAllocator<WriteDidServiceHandler> allocator;
      WriteDidServiceHandler* handler_raw_ptr = allocator.allocate(1);
      allocator.construct(handler_raw_ptr, write_did_service_config);

      HandlerUniquePtr handler_ptr = HandlerUniquePtr(handler_raw_ptr, ServiceHandlerDeleter());

      // Add this handler to the map.
      service_table_map.emplace(sid, std::move(handler_ptr));
      break;
    }
    case 0x3E: {
      ara::log::LogDebug() << "ServiceTable::" << __func__ << " : TesterPresentHandler to be created.";
      //  Allocate SessionControlHandler for this sid.
      using TesterPresentHandler = server::service::handler::TesterPresentHandler;
      ThreePhaseAllocator<TesterPresentHandler> allocator;
      TesterPresentHandler* handler_raw_ptr = allocator.allocate(1);
      allocator.construct(handler_raw_ptr, tester_present_service_config);

      HandlerUniquePtr handler_ptr = HandlerUniquePtr(handler_raw_ptr, ServiceHandlerDeleter());

      // Add this handler to the map.
      service_table_map.emplace(sid, std::move(handler_ptr));
      break;
    }
    case 0x31: {
      ara::log::LogDebug() << "ServiceTable::" << __func__ << " : RoutineControlHandler to be created.";
      //  Allocate SessionControlHandler for this sid.
      /** Routine controle service config */
      const handler::routine::RoutineControlHandler::Configuration routine_control_service_config{
          base_service_config, dext_config.rids_table};
      using RoutineControlHandler = server::service::handler::routine::RoutineControlHandler;
      ThreePhaseAllocator<RoutineControlHandler> allocator;
      RoutineControlHandler* handler_raw_ptr = allocator.allocate(1);
      allocator.construct(handler_raw_ptr, routine_control_service_config);

      HandlerUniquePtr handler_ptr = HandlerUniquePtr(handler_raw_ptr, ServiceHandlerDeleter());
      // Add this handler to the map.
      service_table_map.emplace(sid, std::move(handler_ptr));
      break;
    }
    default:
      ara::log::LogError() << "ServiceTable::" << __func__ << " : Unknown sid : " << std::to_string(sid);
      break;
  }
}

handler::ServiceHandler::HandlerType ServiceTable::GetHandlerType(
    const amsr::diag::configuration::HandlerInfo& handler_info) {
  const std::string& handler_type = handler_info.handler_type;
  if (handler_type == "Internal") {
    return handler::ServiceHandler::HandlerType::kInternal;
  }

  if (handler_type == "External") {
    return handler::ServiceHandler::HandlerType::kExternal;
  }

  // Handler Type is unknown. Internal is used as default.
  ara::log::LogError() << "ServiceTable::" << __func__ << " : Handler type is unknown. Internal is used as default";
  return handler::ServiceHandler::HandlerType::kInternal;
}

void ServiceTable::ServiceHandlerDeleter::operator()(handler::ServiceHandler* ptr) {
  if (ptr != nullptr) {
    ptr->~ServiceHandler();
    alloc_.deallocate(ptr, 1);
  }
}

}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

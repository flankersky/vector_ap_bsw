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
/**        \file  service_table.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SERVICE_SERVICE_TABLE_H_
#define SRC_SERVER_SERVICE_SERVICE_TABLE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <vac/memory/optional.h>
#include <vac/memory/three_phase_allocator.h>
#include <vac/testing/test_adapter.h>
#include <functional>
#include <map>
#include <memory>
#include <utility>

#include "ara/log/logging.hpp"
#include "configuration/dext_configuration.h"
#include "server/service/handler/service_handler.h"

#include "server/diagnostic_server_configuration.h"
#include "server/service/handler/generic_service_handler.h"
#include "server/service/handler/read_did_service_handler.h"
#include "server/service/handler/routine/routine_control_handler.h"
#include "server/service/handler/session_control_handler.h"
#include "server/service/handler/tester_present_handler.h"
#include "server/service/handler/write_did_service_handler.h"
#include "vac/container/static_map.h"

namespace amsr {
namespace diag {
namespace server {
namespace service {
/**
 * \brief Implementation of ServiceTable class.
 * This class is used to associate the SID with the handler specified in the configuration file.
 */
class ServiceTable {
  // TODO(PAASR-2156): Make maximum number of ServiceProcessor configurable in ServiceTable

 public:
  /** Maximum of conversations*/
  static constexpr std::size_t kMaxNumberOfConversations = 30;
  /** Base config */
  const handler::ServiceHandler::Configuration base_service_config{kMaxNumberOfConversations};
  /** Generic service config */
  const handler::GenericServiceHandler::Configuration generic_service_config{base_service_config};
  /** Write DID service config */
  const handler::WriteDidServiceHandler::Configuration write_did_service_config{base_service_config};
  /** Tester present service config */
  const handler::TesterPresentHandler::Configuration tester_present_service_config{base_service_config};

 public:
  /**
   * \brief Constructor.
   */
  ServiceTable(const DiagnosticServerConfiguration& server_configuration,
               const amsr::diag::configuration::DextConfiguration& dext_config);

  ServiceTable(const ServiceTable&) = delete;
  ServiceTable(const ServiceTable&&) = delete;
  ServiceTable& operator=(const ServiceTable&) = delete;
  ServiceTable& operator=(const ServiceTable&&) = delete;

  /**
   * \brief Method called during shutdown phase.
   */
  void Shutdown();

  /**
   * \brief Return the service handler if available otherwise nullptr.
   * As this method may not return a service handler, a pointer to the service handler which can be null is returned.
   * The pointed handler shall then not be deleted since the pointer is contained inside an unique pointer.
   * The service handlers are deconstructed only during Shutdown phase, therefore there is no risk at all that a non
   * null pointer is pointing to an already destructed object.
   * \param sid service id.
   * \return pointer to the service handler.
   */
  handler::ServiceHandler* GetHandlerIfAvailable(std::uint8_t sid);

 private:
  /**
   * \brief Type definition for three phase allocator.
   */
  template <typename T>
  using ThreePhaseAllocator = vac::memory::ThreePhaseAllocator<T>;

  /**
   * \brief Deleter for  Service Handlers.
   *
   * The deleter is used here only to make sure that handlers deallocation is done during the deallocation phase.
   * The type used with the allocator does not really matter since std::allocator is the allocator hiding behind the
   * ThreePhaseAllocator.
   */
  class ServiceHandlerDeleter {
   public:
    /**
     * \The actual deleter for Service handlers.
     */
    void operator()(handler::ServiceHandler* ptr);

   private:
    /**
     * \brief ThreePhaseAllocator used for deallocation.
     */
    ThreePhaseAllocator<handler::ServiceHandler> alloc_;
  };

  /**
   * \brief Type definition for unique pointer to Handler.
   */
  using HandlerUniquePtr = std::unique_ptr<handler::ServiceHandler, ServiceHandlerDeleter>;

  /**
   * \brief Method used to allocate handler.
   */
  void AllocateHandler(handler::ServiceHandler::HandlerType handler_type, std::uint8_t sid,
                       const amsr::diag::configuration::DextConfiguration& dext_config);

  /**
   * \brief Method used to allocate handlers when type is internal.
   */
  void AllocateHandlerInternal(std::uint8_t sid, const amsr::diag::configuration::DextConfiguration& dext_config);

  /**
   * \brief Method used to return the ServiceHandler::HandlerType contained in the HandlerInfo structure.
   */
  handler::ServiceHandler::HandlerType GetHandlerType(const configuration::HandlerInfo& handler_info);

  /**
   * \brief Type def for map associating the SID with the handler.
   */
  using ServiceTableMap = vac::container::StaticMap<std::uint8_t, HandlerUniquePtr>;

  /**
   * \brief Map associating the SID with the corresponding handler.
   */
  ServiceTableMap service_table_map;

  /**
   * \brief Reference to the diagnostic server configuration.
   */
  const DiagnosticServerConfiguration& server_config;

 private:
  FRIEND_TEST(ServiceTableTestFixture, Initialize);
  FRIEND_TEST(ServiceTableTestFixture, Shutdown);
  FRIEND_TEST(ServiceTableTestFixture, GetHandlerIfAvailable);
  FRIEND_TEST(ServiceTableTestFixture, InternalAllocateHandler);
  FRIEND_TEST(ServiceTableTestFixture, ExternalAllocateHandler);
  FRIEND_TEST(ServiceTableTestFixture, AllocateHandlerInternalSid0x10);
  FRIEND_TEST(ServiceTableTestFixture, AllocateHandlerInternalSid0x22);
  FRIEND_TEST(ServiceTableTestFixture, AllocateHandlerInternalSid0x2E);
  FRIEND_TEST(ServiceTableTestFixture, AllocateHandlerInternalSid0x3E);
  FRIEND_TEST(ServiceTableTestFixture, AllocateHandlerInternalSid0x31);
  FRIEND_TEST(ServiceTableTestFixture, AllocateHandlerInternalSidUnknown);
  FRIEND_TEST(ServiceTableTestFixture, GetHandlerType);
};

}  // namespace service
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SERVICE_SERVICE_TABLE_H_

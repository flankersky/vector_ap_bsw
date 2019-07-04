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
/**        \file  did_manager.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_DATA_DID_MANAGER_H_
#define SRC_SERVER_DATA_DID_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <map>
#include <utility>

#include "configuration/dext_configuration.h"
#include "did_info.h"
#include "operationhandler/read_did_operation_handler.h"
#include "operationhandler/write_did_operation_handler.h"
#include "server/data/did_operation.h"
#include "server/service/service_processing_context.h"
#include "vac/container/static_map.h"
#include "vac/memory/optional.h"
#include "vac/memory/three_phase_allocator.h"

namespace amsr {
namespace diag {
namespace server {
namespace data {

/**
 * \brief DID management
 */
class DidManager {
 public:
  /**
   * \brief Constructor initializes the instance.
   * \param dids_table_conf contains the configuration of the Did table.
   */
  explicit DidManager(const configuration::DidsTableConfiguration& dids_table_conf);
  virtual ~DidManager() = default;

  DidManager(const DidManager&) = delete;
  DidManager& operator=(const DidManager&) = delete;
  DidManager(DidManager&&) = delete;
  DidManager& operator=(DidManager&&) = delete;

  /**
   * \brief Returns an optional containing a ReadDidOperationHandler if available. Otherwise the optional is empty.
   * \param did the data identifier
   * \param processing_context the current service processing context
   * \return optional of ReadDidOperationHandler
   */
  vac::memory::optional<operationhandler::ReadDidOperationHandler> CreateOpHandlerIfDidReadable(
      const DidInfo::Did did, service::ServiceProcessingContext& processing_context);

  /**
   * \brief Returns an optional containing a WriteDidOperationHandler if available. Otherwise the optional is empty.
   * \param did the data identifier
   * \param processing_context the current service processing context
   * \return optional of WriteDidOperationHandler
   */
  vac::memory::optional<operationhandler::WriteDidOperationHandler> CreateOpHandlerIfDidWriteable(
      const DidInfo::Did did, service::ServiceProcessingContext& processing_context);

 private:
  /**
   * \brief Return a pointer to the DidInfo if the operation is supported in active session for specified did. Nullptr
   * is returned otherwise.
   * \param did the data identifier
   * \param processing_context the current service processing context
   * \param operation the requested operation
   * \return pointer to the DidInfo
   */
  const DidInfo* GetDidInfoIfSupported(const DidInfo::Did did, service::ServiceProcessingContext& processing_context,
                                       data::DidOperation::Type operation);

  /**
   * \brief Type definition for map associating a did with DidInfo.
   */
  using DidInfoMap = vac::container::StaticMap<DidInfo::Did, DidInfo>;

  /**
   * \brief Map associating a did with DidInfo.
   */
  DidInfoMap did_info_map_;
};

}  // namespace data
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_DATA_DID_MANAGER_H_

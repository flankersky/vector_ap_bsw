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
/**        \file  data_manager.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_DATA_DATA_MANAGER_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_DATA_DATA_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "data/data_operation_factory.h"
#include "vac/memory/optional.h"

namespace diag_test_mediator {
namespace data {

/**
 * \brief
 * The DataManager responsibility is the dispatching of request received by the Mediator
 */
class DataManager {
  // @TODO(viskth): I think DataManager is not a good name, my suggestion is HandleDispatcher.
 public:
  /**
   * Initializes a created instance
   * \param did_op_factory Factory to create DataPollingTask instances for DIDs
   * \param rid_op_factory Factory to create DataPollingTask instances for RIDs (start, stop routines, ...)
   */
  explicit DataManager(const DataOperationFactory& did_op_factory, const DataOperationFactory& rid_op_factory);
  virtual ~DataManager() = default;

  DataManager(const DataManager&) = delete;
  DataManager& operator=(const DataManager&) = delete;
  DataManager(DataManager&&) = delete;
  DataManager& operator=(DataManager&&) = delete;

  /**
   * \brief Checks if the provided handle is supported
   * \param handle The handle of the operation (read/write DID, start/stop/read results of routine)
   * \return true when the handle is known; otherwise false
   */
  bool IsHandleSupported(DataHandler::Handle handle) const;

  /**
   * \brief Creates a new DataIdentifier object if DID is available
   * \param handle The handle of the operation (read/write DID, start/stop/read results of routine)
   * \return optional containing new DataHandle if available, otherwise empty
   */
  vac::memory::optional<DataHandler> CreateDataHandlerIfAvailable(DataHandler::Handle handle);

 private:
  /**
   * \brief Factory for creating polling tasks for read/write operations on DIDs that use ara::com.
   */
  const DataOperationFactory& did_op_factory_;
  /**
   * \brief Factory for creating polling tasks for start/stop routine or fetch routine results
   * operations that use ara::com.
   */
  const DataOperationFactory& rid_op_factory_;
};

}  // namespace data
}  // namespace diag_test_mediator

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_DATA_DATA_MANAGER_H_

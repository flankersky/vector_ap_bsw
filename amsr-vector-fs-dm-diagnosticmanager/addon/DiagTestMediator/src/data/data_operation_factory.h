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
/**        \file  did_operation_factory.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_DATA_DATA_OPERATION_FACTORY_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_DATA_DATA_OPERATION_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "common/mediator_handles.h"
#include "data/data_handler.h"
#include "data/data_polling_task.h"

namespace diag_test_mediator {
namespace data {

/**
 * \brief Interface for DID operation factory.
 */
class DataOperationFactory {
 public:
  using Handle = amsr::diag::common::handles::Handle;

  /**
   * \brief Destructor.
   */
  virtual ~DataOperationFactory() = default;

  DataOperationFactory(const DataOperationFactory& other) = delete;
  DataOperationFactory(DataOperationFactory&& other) = delete;
  DataOperationFactory& operator=(const DataOperationFactory&) = delete;
  DataOperationFactory& operator=(DataOperationFactory&&) = delete;

  /**
   * Test whether a factory can construct a polling task for the given handle
   * \param handle The operation handle.
   * \return true when the factory can create a polling task for the given handle; otherwise false is returned.
   */
  virtual bool CanCreatePollingTask(Handle handle) const = 0;

  /**
   * Creates a polling task for performing the operation.
   * \param handle The operation handle.
   * \return Pointer to the requested polling task for performing the operation.
   */
  virtual DataPollingTask::Ptr CreatePollingTask(Handle handle) const = 0;

  /**
   * Returns the expected size of the output data.
   * \param handle The operation handle.
   */
  virtual std::size_t GetOutputDataSize(Handle handle) const = 0;

 protected:
  DataOperationFactory() = default;
};

}  // namespace data
}  // namespace diag_test_mediator

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_DATA_DATA_OPERATION_FACTORY_H_

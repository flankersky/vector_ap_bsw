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
/**        \file  did_operation_factory_impl.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_DATA_DID_SERVICE_DID_OPERATION_FACTORY_IMPL_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_DATA_DID_SERVICE_DID_OPERATION_FACTORY_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "data/data_operation_factory.h"

namespace diag_test_mediator {
namespace data {
namespace did_service {

/**
 * \brief Implementation of DataOperationFactory.
 *
 * This class is the intermediate solution for the problem of linking generated proxy files for read/write DID Interface
 * with the DM core.
 */
class DidOperationFactoryImpl : public DataOperationFactory {
 public:
  /**
   * \brief Constructor.
   */
  DidOperationFactoryImpl() = default;

  /**
   * \brief Destructor.
   */
  virtual ~DidOperationFactoryImpl() = default;

  /**
   * Test whether a factory can construct a polling task for the given handle
   * \param handle The operation handle.
   * \return true The factory can create a polling task for the given handle
   * \return false The factory cannot create a polling task for the given handle
   */
  bool CanCreatePollingTask(Handle handle) const override;

  DataPollingTask::Ptr CreatePollingTask(Handle handle) const override;

  std::size_t GetOutputDataSize(Handle handle) const override;

 private:
  DataPollingTask::Ptr CreateReadDidPollingTask(Handle handle) const;
  DataPollingTask::Ptr CreateWriteDidPollingTask(Handle handle) const;
};

}  // namespace did_service
}  // namespace data
}  // namespace diag_test_mediator

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_DATA_DID_SERVICE_DID_OPERATION_FACTORY_IMPL_H_

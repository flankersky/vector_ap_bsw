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
/**
 *      \file  routine_operation_factory_impl.h
 *      \brief  Implementation of DataOperationFactory for routines
 *
 *      \details  RoutineOperationFactoryImpl implements the DataOperationFactory interface for routines
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_DATA_ROUTINE_SERVICE_ROUTINE_OPERATION_FACTORY_IMPL_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_DATA_ROUTINE_SERVICE_ROUTINE_OPERATION_FACTORY_IMPL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "data/data_operation_factory.h"

namespace diag_test_mediator {
namespace data {
namespace routine_service {

/**
 * \brief Implementation of DataOperationFactory.
 *
 * This class is the intermediate solution for the problem of linking generated proxy files for routine Interface
 * with the DM core.
 */
class RoutineOperationFactoryImpl : public DataOperationFactory {
 public:
  /**
   * \brief Constructor.
   */
  RoutineOperationFactoryImpl() = default;

  /**
   * \brief Destructor.
   */
  virtual ~RoutineOperationFactoryImpl() = default;

  bool CanCreatePollingTask(Handle handle) const override;

  DataPollingTask::Ptr CreatePollingTask(Handle handle) const override;

  std::size_t GetOutputDataSize(Handle handle) const override;

 private:
  DataPollingTask::Ptr CreateStartRoutinePollingTask(Handle handle) const;
  DataPollingTask::Ptr CreateStopRoutinePollingTask(Handle handle) const;
  DataPollingTask::Ptr CreateRequestRoutineResultsPollingTask(Handle handle) const;
};

}  // namespace routine_service
}  // namespace data
}  // namespace diag_test_mediator

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_DATA_ROUTINE_SERVICE_ROUTINE_OPERATION_FACTORY_IMPL_H_

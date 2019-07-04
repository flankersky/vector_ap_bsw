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
/**
 *      \file  data_manager.cc
 *      \brief  Contains the implementation of the DataManager.
 *
 *      \details  The DataManager creates DataHandler instances for provided handles.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "data/data_manager.h"
#include "ara/log/logging.hpp"

namespace diag_test_mediator {
namespace data {

DataManager::DataManager(const DataOperationFactory& did_op_factory, const DataOperationFactory& rid_op_factory)
    : did_op_factory_(did_op_factory), rid_op_factory_(rid_op_factory) {}

bool DataManager::IsHandleSupported(DataHandler::Handle handle) const {
  // first check if handle addresses a DID
  if (did_op_factory_.CanCreatePollingTask(handle)) {
    return true;
  }

  // second check if handle addresses a routine
  if (rid_op_factory_.CanCreatePollingTask(handle)) {
    return true;
  }

  return false;
}

vac::memory::optional<DataHandler> DataManager::CreateDataHandlerIfAvailable(DataHandler::Handle handle) {
  vac::memory::optional<DataHandler> result;

  if (did_op_factory_.CanCreatePollingTask(handle)) {
    result.emplace(handle, did_op_factory_);
    return result;
  }

  if (rid_op_factory_.CanCreatePollingTask(handle)) {
    ara::log::LogDebug() << "DataManager::" << __func__ << ": Requested '" << handle << "' addresses  supported.";
    result.emplace(handle, rid_op_factory_);
    return result;
  }

  ara::log::LogWarn() << "DataManager::" << __func__ << ": Requested '" << handle << "' not supported.";
  return result;
}

}  // namespace data
}  // namespace diag_test_mediator

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
 *      \file  data_handle.cc
 *      \brief  DataHandle implementation
 *
 *      \details  Contains the implementation of the DataHandle class.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <data/data_handler.h>
#include <cassert>

#include "data/data_operation_factory.h"

namespace diag_test_mediator {
namespace data {

DataHandler::DataHandler(Handle handle, const DataOperationFactory& op_factory)
    : handle_(handle), op_factory_(op_factory) {
  if (false == op_factory_.CanCreatePollingTask(handle_)) {
    throw std::logic_error(
        "DataHandler: Couldn't create valid instance - polling task for provided handle not supported!");
  }
}

DataPollingTask::Ptr DataHandler::StartOperation(ara::diag::udstransport::UdsMessage::MetaInfoMap meta_info,
                                                 DataPollingTask::InBufferType in_buffer,
                                                 DataPollingTask::OutBufferType out_buffer) const {
  DataPollingTask::Ptr polling_task = op_factory_.CreatePollingTask(handle_);
  // CreatePollingTask should never fail, because a DataHandle instance is only generated
  // when op_factory_.CanCreatePollingTask() returns true
  assert(polling_task);
  polling_task->Init(in_buffer, out_buffer);
  polling_task->Start();
  return polling_task;
}

std::size_t DataHandler::GetOutputDataSize() const { return op_factory_.GetOutputDataSize(handle_); }

}  // namespace data
}  // namespace diag_test_mediator

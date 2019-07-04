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
 *      \file  data_handler.h
 *      \brief  DataHandler definition
 *
 *      \details  A helper class that can be used to start a data operation (fetch DID data, write DID, ...)
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_DATA_DATA_HANDLER_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_DATA_DATA_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "vac/container/array_view.h"

#include "common/mediator_handles.h"
#include "data/data_operation_factory.h"
#include "data/data_polling_task.h"
#include "udstransport/uds_message.h"

namespace diag_test_mediator {
namespace data {

class DataOperationFactory;

/**
 * \brief Helper class to start a data polling task
 * The instance combines a handle value and a reference to a DataOperationFactory.
 * Instances of this type can be used to start a data operation (e. g. read/wrtie a DID
 * or start/stop/read results of a routine)
 */
class DataHandler {
 public:
  /**
   * Handle value type 0 .. 2^32-1
   */
  using Handle = amsr::diag::common::handles::Handle;

  /**
   * Constructor
   * \param handle The handle that identifies the to be executed operation (read/write DID, ...)
   * \param op_factory Factory of polling tasks handling the read/write operation on DID.
   */
  DataHandler(Handle handle, const DataOperationFactory& op_factory);

  /**
   * Getter for the DID.
   * \return DID
   */
  Handle GetHandle() const { return handle_; }

  /**
   * \brief Starts a new PollingTask.
   * \param meta_info meta info provided with the UDS message
   * \param in_buffer buffer containing data to be deserialized and sent to the application
   * \param out_buffer output buffer to receive the data returned by the application
   * \return started ReadPollingTask
   */
  DataPollingTask::Ptr StartOperation(ara::diag::udstransport::UdsMessage::MetaInfoMap meta_info,
                                      DataPollingTask::InBufferType in_buffer,
                                      DataPollingTask::OutBufferType out_buffer) const;

  /**
   * Returns the expected size of the output data (e. g. the data size of a single DID).
   */
  std::size_t GetOutputDataSize() const;

 private:
  // can't be constant due to use of assignment operator in StaticList
  Handle handle_;

  const DataOperationFactory& op_factory_;
};

}  // namespace data
}  // namespace diag_test_mediator

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_DATA_DATA_HANDLER_H_

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
/**        \file  did_operation_factory_impl.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "data/did_service/did_operation_factory_impl.h"
#include "ara/diag/service_interfaces/data_identifier/DataIdentifier_SWCL_A_DID_4711_proxy.h"
#include "ara/diag/service_interfaces/data_identifier/DataIdentifier_SWCL_A_DID_4711_serializer.h"
#include "ara/diag/service_interfaces/data_identifier/impl_type_MetaInfoType.h"
#include "ara/diag/service_interfaces/data_identifier_F190/DataIdentifier_SWCL_A_DID_F190_proxy.h"
#include "ara/diag/service_interfaces/data_identifier_F190/DataIdentifier_SWCL_A_DID_F190_serializer.h"
#include "common/mediator_handles.h"
#include "data/did_service/read_did_polling_task_impl.h"
#include "data/did_service/write_did_polling_task_impl.h"
#include "vac/language/cpp14_backport.h"

namespace diag_test_mediator {
namespace data {
namespace did_service {

namespace handles = amsr::diag::common::handles;

bool DidOperationFactoryImpl::CanCreatePollingTask(Handle handle) const {
  switch (handle) {
    case handles::kReadDid0x4711:
    case handles::kWriteDid0x4711:
    case handles::kReadDid0xF190:
    case handles::kWriteDid0xF190:
      return true;
    default:
      return false;
  }
}

DataPollingTask::Ptr DidOperationFactoryImpl::CreatePollingTask(Handle handle) const {
  switch (handle) {
    case handles::kReadDid0x4711:
    case handles::kReadDid0xF190:
      return CreateReadDidPollingTask(handle);

    case handles::kWriteDid0x4711:
    case handles::kWriteDid0xF190:
      return CreateWriteDidPollingTask(handle);

    default:
      ara::log::LogError() << "DidOperationFactoryImpl::" << __func__ << ": DID '" << handle << "' not supported!";
      return DataPollingTask::Ptr();
  }
}

using DataIdentifier_SWCL_A_DID_4711Serializer =
    ara::diag::service_interfaces::data_identifier::serializer::DataIdentifier_SWCL_A_DID_4711Serializer;

using DataIdentifier_SWCL_A_DID_F190Serializer =
    ara::diag::service_interfaces::data_identifier_F190::serializer::DataIdentifier_SWCL_A_DID_F190Serializer;

DataPollingTask::Ptr DidOperationFactoryImpl::CreateReadDidPollingTask(Handle handle) const {
  switch (handle) {
    case handles::kReadDid0x4711:
      // TODO(PAASR-2308): PollingTasks should NOT be created on the heap
      return vac::language::make_unique<ReadDidPollingTaskImpl<
          ara::diag::service_interfaces::data_identifier::DataIdentifier_SWCL_A_DID_4711Proxy,
          DataIdentifier_SWCL_A_DID_4711Serializer, ara::diag::service_interfaces::data_identifier::MetaInfoType>>();
    case handles::kReadDid0xF190:
      // TODO(PAASR-2308): Creating PollingTasks on heap shall be replaced by elaborate memory concept.
      return vac::language::make_unique<ReadDidPollingTaskImpl<
          ara::diag::service_interfaces::data_identifier_F190::DataIdentifier_SWCL_A_DID_F190Proxy,
          DataIdentifier_SWCL_A_DID_F190Serializer,
          ara::diag::service_interfaces::data_identifier_F190::MetaInfoType>>();
    default:
      ara::log::LogError() << "DidOperationFactoryImpl::" << __func__ << ": handle '" << handle << "' not supported!";
      return DataPollingTask::Ptr();
  }
}

DataPollingTask::Ptr DidOperationFactoryImpl::CreateWriteDidPollingTask(Handle handle) const {
  switch (handle) {
    case handles::kWriteDid0x4711:
      // TODO(PAASR-2308): PollingTasks should NOT be created on the heap
      return vac::language::make_unique<WriteDidPollingTaskImpl<
          ara::diag::service_interfaces::data_identifier::DataIdentifier_SWCL_A_DID_4711Proxy,
          DataIdentifier_SWCL_A_DID_4711Serializer, ara::diag::service_interfaces::data_identifier::MetaInfoType>>();
    case handles::kWriteDid0xF190:
      // TODO(PAASR-2308): PollingTasks should NOT be created on the heap
      return vac::language::make_unique<WriteDidPollingTaskImpl<
          ara::diag::service_interfaces::data_identifier_F190::DataIdentifier_SWCL_A_DID_F190Proxy,
          DataIdentifier_SWCL_A_DID_F190Serializer,
          ara::diag::service_interfaces::data_identifier_F190::MetaInfoType>>();
    default:
      ara::log::LogError() << "DidOperationFactoryImpl::" << __func__ << ": handle '" << handle << "' not supported!";
      return DataPollingTask::Ptr();
  }
}

std::size_t DidOperationFactoryImpl::GetOutputDataSize(Handle handle) const {
  switch (handle) {
    case handles::kReadDid0x4711:
      return DataIdentifier_SWCL_A_DID_4711Serializer::GetDidDataSize();
    case handles::kReadDid0xF190:
      return DataIdentifier_SWCL_A_DID_F190Serializer::GetDidDataSize();
    default:
      ara::log::LogError() << "DidOperationFactoryImpl::" << __func__ << ": handle '" << handle << "' not supported!";
      return 0;
  }
}

}  // namespace did_service
}  // namespace data
}  // namespace diag_test_mediator

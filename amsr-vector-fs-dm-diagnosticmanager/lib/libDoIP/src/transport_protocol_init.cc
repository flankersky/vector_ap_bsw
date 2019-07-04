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
/**        \file  transport_protocol_init.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "configuration/diagnostic_configuration.h"
#include "doip_transport_protocol_handler.h"
#include "udstransport/uds_transport_protocol_manager_impl.h"
#include "vac/language/cpp14_backport.h"
#include "vac/memory/optional.h"

namespace amsr {
namespace diag {
namespace udstransport {

void UdsTransportProtocolMgrImpl::InitializeTransportProtocol(
    const ara::diag::udstransport::UdsTransportProtocolHandlerID handler_id, const std::string& path_tp_config) {
  // Load the DoIP configuration.
  configuration::DoIPChannelConfiguration do_ip_channel_configuration;
  do_ip_channel_configuration.LoadDoIPChannelConfiguration(path_tp_config);

  ara::log::LogInfo() << "UdsTransportProtocolMgr::" << __func__ << ": using vector DoIP layer";
  uds_transport_protocol_handlers_.emplace_back(vac::language::make_unique<doip::DoIPTransportProtocolHandler>(
      0, *this, reactor_, timer_manager_, do_ip_channel_configuration));
}

}  // namespace udstransport
}  // namespace diag
}  // namespace amsr

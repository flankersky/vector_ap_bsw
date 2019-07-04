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
/**        \file  client.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/rest/client.h"

#include <vac/language/cpp14_backport.h>
#include <system_error>
#include <utility>

#include "ara/rest/client_interface.h"
#include "ara/rest/endpoint.h"
#include "ara/rest/httpclient.h"
#include "ara/rest/ogm/object.h"

namespace ara {
namespace rest {

Client::Client(const String& conf_id, Allocator* alloc) : client_(vac::language::make_unique<HttpClient>(conf_id)) {
  (void)alloc;
}

Task<void> Client::Stop(ShutdownPolicy policy) { return client_->Stop(policy); }

Task<Pointer<Reply>> Client::Send(const Request& req) { return client_->Send(req); }

Task<Event> Client::Subscribe(const Uri& uri, EventPolicy policy, duration_t time,
                              const Function<client::NotificationHandlerType>& notify,
                              const Function<client::SubscriptionStateHandlerType>& state) {
  return client_->Subscribe(uri, policy, time, notify, state);
}

std::error_code Client::GetError() const noexcept { return client_->GetError(); }

void Client::ObserveError(const Function<void(std::error_code)>& hnd) { client_->ObserveError(hnd); }

}  // namespace rest
}  // namespace ara

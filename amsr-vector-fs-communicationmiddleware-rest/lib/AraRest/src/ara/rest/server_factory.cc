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
/**        \file  server_factory.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/rest/server_factory.h"
#include "ara/rest/httpserver.h"
#include "ara/rest/server_types.h"

namespace ara {
namespace rest {

std::unique_ptr<ServerInterface> ServerFactory::CreateServer(String protocol, config::ServerBinding binding,
                                                             const Function<RequestHandlerType> &hnd,
                                                             Allocator *alloc) {
  if (protocol.compare("HTTP/1.1")) {
    return vac::language::make_unique<HttpServer>(binding, hnd, alloc);
  }
  return nullptr;
}

}  // namespace rest
}  // namespace ara

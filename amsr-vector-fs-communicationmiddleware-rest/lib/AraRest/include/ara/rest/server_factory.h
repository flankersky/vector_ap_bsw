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
/**        \file  server_factory.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_SERVER_FACTORY_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_SERVER_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>

#include "ara/rest/config.h"
#include "ara/rest/server.h"
#include "ara/rest/server_interface.h"

namespace ara {
namespace rest {

/**
 * TODO
 */
class ServerFactory {
 public:
  /**
   * Destructor
   */
  ~ServerFactory() = default;

  /**
   * Singelton Get
   * @return Singelton instance of Server Factory
   */
  static ServerFactory *Get() {
    static ServerFactory instance;
    return &instance;
  }

  /**
   * TODO
   * @param protocol
   * @param binding
   * @param hnd
   * @param alloc
   * @return
   */
  std::unique_ptr<ServerInterface> CreateServer(String protocol, config::ServerBinding binding,
                                                const Function<RequestHandlerType> &hnd, Allocator *alloc);

 protected:
  ServerFactory() = default;  //< Default constructor protected

 private:
  ServerFactory(ServerFactory const &) = delete;             // Copy construct deleted
  ServerFactory(ServerFactory &&) = delete;                  // Move construct deleted
  ServerFactory &operator=(ServerFactory const &) = delete;  // Copy assign deleted
  ServerFactory &operator=(ServerFactory &&) = delete;       // Move assign deleted
};

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_SERVER_FACTORY_H_

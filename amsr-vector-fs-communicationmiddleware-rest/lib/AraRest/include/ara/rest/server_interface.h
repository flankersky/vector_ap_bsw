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
/**        \file  server_interface.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_SERVER_INTERFACE_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_SERVER_INTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/rest/server.h"
#include "ara/rest/server_types.h"

namespace ara {
namespace rest {

/**
 * Interface to make different kind of server bindings (e.g. HTTP) possible to be use by the server.h
 */
class ServerInterface {
 public:
  /**
   * \brief Default Constructor
   */
  ServerInterface() = default;

  explicit ServerInterface(const ServerInterface&) = delete;    ///< Server is non-copy-constructible
  ServerInterface& operator=(const ServerInterface&) = delete;  ///< Server is non-copy-assignable

  /**
   * \brief Destructor
   */
  virtual ~ServerInterface() = default;

  /**
   * \brief Start the Server Binding asynchronously
   * \param policy
   * \return
   */
  virtual Task<void> Start(StartupPolicy policy = StartupPolicy::kDetached) = 0;

  /**
   * \brief Stop the Server asynchronously
   * \param policy
   * \return
   */
  virtual Task<void> Stop(ShutdownPolicy policy = ShutdownPolicy::kGraceful) = 0;

  /**
   * \brief TODO
   * \param shnd
   * \param sshnd
   */
  virtual void ObserveSubscriptions(const Function<SubscriptionHandlerType>& shnd,
                                    const Function<SubscriptionStateHandlerType>& sshnd) = 0;

  /** \brief Obtain server status
   * \return a reference to the server Status
   */
  virtual std::error_code GetError() const noexcept = 0;

  /** \brief Observe status changes
   *
   *  \param hnd user-defined handler function to to called on status changes
   */
  virtual void ObserveError(const Function<void(std::error_code)>& hnd) = 0;
};

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_SERVER_INTERFACE_H_

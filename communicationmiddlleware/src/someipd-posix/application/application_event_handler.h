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
/**        \file  application_event_handler.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_APPLICATION_APPLICATION_EVENT_HANDLER_H_
#define SRC_SOMEIPD_POSIX_APPLICATION_APPLICATION_EVENT_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/io/event_handler.h"

namespace someipd_posix {
namespace application {

class ApplicationManager;

/**
 * \brief EventHandler that asks an ApplicationManager for the Application matching the given handle to be handled.
 */
class ApplicationEventHandler : public osabstraction::io::EventHandler {
 public:
  /**
   * \brief Constructor
   *
   * \param application_manager The application manager to be queried for applications.
   * \throws std::runtime_error in case a nullpointer was passed.
   */
  explicit ApplicationEventHandler(const ApplicationManager* const application_manager);

  virtual ~ApplicationEventHandler() = default;

  /**
   * \brief Read event handler.
   *
   * \param handle A file descriptor.
   * \return true in case the EventHandler can be called again, false otherwise
   *
   * This function is called by reactor whenever the file descriptor becomes readable.
   */
  bool HandleRead(int handle) override;

 private:
  const ApplicationManager* const application_manager_;
};

}  // namespace application
}  // namespace someipd_posix

#endif  // SRC_SOMEIPD_POSIX_APPLICATION_APPLICATION_EVENT_HANDLER_H_

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
/**        \file  application_event_handler.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/application/application_event_handler.h"

#include <memory>

#include "someipd-posix/application/application.h"
#include "someipd-posix/application/application_manager.h"

namespace someipd_posix {
namespace application {

ApplicationEventHandler::ApplicationEventHandler(const ApplicationManager* const application_manager)
    : application_manager_(application_manager) {
  if (!application_manager_) {
    throw std::runtime_error("ApplicationEventHandler::ctor: No ApplicationManager provided");
  }
}

bool ApplicationEventHandler::HandleRead(int handle) {
  // Find the proper application from the ApplicationManager
  std::shared_ptr<someipd_posix::application::Application> application = application_manager_->GetApplication(handle);
  if (application) {
    return application->HandleRead(handle);
  } else {
    return false;
  }
}

}  // namespace application
}  // namespace someipd_posix

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
/**       \file  /lib/libosabstraction-linux/src/osabstraction/oscontrol/oscontrol.cc
 *        \brief  This implementation provides some function to control the OS.
 *
 *        \details  This implementation provides the following functionality for Linux:
 *                  a function to restart the OS.
 *                  a function to shutdown the OS.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/reboot.h>
#include <unistd.h>

#include "osabstraction/oscontrol/oscontrol.h"

namespace osabstraction {
namespace oscontrol {

void OsController::Shutdown() {
  // Synchronizes the associated stream buffer with its controlled input sequence.
  // Note that if reboot is not preceded by a call to sync, data may be lost.
  sync();
  // Shutdown call
  reboot(RB_POWER_OFF);
}

void OsController::Restart() {
  // Synchronizes the associated stream buffer with its controlled input sequence.
  // Note that if reboot is not preceded by a call to sync, data may be lost.
  sync();
  // Shutdown call
  reboot(RB_AUTOBOOT);
}

OsController::OsController() {}
OsController::~OsController() {}

}  // namespace oscontrol
}  // namespace osabstraction

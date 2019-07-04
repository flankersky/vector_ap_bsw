/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2018 by Vector Informatik GmbH.                                              All rights reserved.
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
#include "osabstraction/oscontrol/oscontrol.h"

// Note: sys/shutdown.h is designed for c and uses class as a variable name which gives compilation error in c++ to
// avoid that we use #define for class to klass.
#define class klass
#include <sys/shutdown.h>
#define class class

namespace osabstraction {
namespace oscontrol {

void OsController::Shutdown() {
  // Synchronizes the associated stream buffer with its controlled input sequence.
  // Note that if reboot is not preceded by a call to sync, data may be lost.
  sync();
  // performing shutdown
  shutdown_system(SHUTDOWN_SYSTEM, FLAG_FAST);
}
void OsController::Restart() {
  // Synchronizes the associated stream buffer with its controlled input sequence.
  // Note that if reboot is not preceded by a call to sync, data may be lost.
  sync();
  // performing reboot
  shutdown_system(SHUTDOWN_REBOOT, FLAG_FAST);
}

}  // namespace oscontrol
}  // namespace osabstraction

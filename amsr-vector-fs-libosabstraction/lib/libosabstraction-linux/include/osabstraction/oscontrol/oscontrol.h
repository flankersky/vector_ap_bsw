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
/**        \file
 *        \brief  Linux OsController.
 *
 *      \details  This file defines the Linux variant of the OsControllerInterface.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_OSCONTROL_OSCONTROL_H_
#define LIB_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_OSCONTROL_OSCONTROL_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/oscontrol/oscontrollerinterface.h"

namespace osabstraction {
namespace oscontrol {

/**
 * \brief Oscontroller class for Linux.
 */
class OsController : public OsControllerInterface {
 public:
  /**
   * \brief Creates a Linux Oscontroller.
   */
  OsController();

  /**
   * \brief Performs an shutdown of the OS.
   */
  void Shutdown();

  /**
   * \brief Performs an restart of the OS.
   */
  void Restart();
  /**
   * \brief destroys a Linux Oscontroller.
   */
  ~OsController();
};

}  // namespace oscontrol
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_LINUX_INCLUDE_OSABSTRACTION_OSCONTROL_OSCONTROL_H_

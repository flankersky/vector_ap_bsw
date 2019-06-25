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
 *        \brief  Abstract interface of OsController.
 *
 *      \details  This file defines an OS independent interface to work with OsController.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_OSCONTROL_OSCONTROLLERINTERFACE_H_
#define LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_OSCONTROL_OSCONTROLLERINTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
namespace osabstraction {
namespace oscontrol {
/**
 * \brief Interface to control the OS
 */
class OsControllerInterface {
 public:
  /**
   * \brief Performs an shutdown of the OS.
   *
   */
  virtual void Shutdown() = 0;

  /**
   * \brief Performs an restart of the OS.
   *
   */
  virtual void Restart() = 0;

  /**
   * \brief Allocates all required OS resources and starts process execution.
   */
  OsControllerInterface() {}

  /**
   * \brief Terminates the process and releases all process related OS resources.
   */
  virtual ~OsControllerInterface() {}
};

}  // namespace oscontrol
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_OSCONTROL_OSCONTROLLERINTERFACE_H_

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
/**        \file  addon/amsr-vector-fs-em-executionmanager-restart-application/run.cc
 *        \brief  -
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <osabstraction/oscontrol/oscontrol.h>
/**
 * \brief The main function for the restart application.
 */
int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  osabstraction::oscontrol::OsController oscontroller = osabstraction::oscontrol::OsController();
  oscontroller.Restart();
  return (0);
}

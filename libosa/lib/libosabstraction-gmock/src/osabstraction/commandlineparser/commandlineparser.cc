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
/**        \file  libosabstraction-gmock/src/osabstraction/commandlineparser/commandlineparser.cc
 *        \brief  A Wrapper for an commandline parser
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <getopt.h>

#include "osabstraction/commandlineparser/commandlineparser.h"

int osabstraction::commandlineparser::CommandLineParser(int argc, char *const argv[], const char *optstring) {
  return getopt(argc, argv, optstring);
}

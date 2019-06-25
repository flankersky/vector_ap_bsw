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
/**  \file
 *   \brief  A Wrapper for an commandline parser
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_STUB_INCLUDE_OSABSTRACTION_COMMANDLINEPARSER_COMMANDLINEPARSER_H_
#define LIB_LIBOSABSTRACTION_STUB_INCLUDE_OSABSTRACTION_COMMANDLINEPARSER_COMMANDLINEPARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/**
 * \brief EM currently assumes, that this symbol exists.
 */
char *optarg;

namespace osabstraction {
namespace commandlineparser {

/**
 * \brief Stub implementation.
 */
int CommandLineParser(int argc, char *const argv[], const char *optstring) {
  (void)argc;
  (void)argv;
  (void)optstring;
  return 0;
}

}  // namespace commandlineparser
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_STUB_INCLUDE_OSABSTRACTION_COMMANDLINEPARSER_COMMANDLINEPARSER_H_

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
/**        \file  libosabstraction-pikeos/include/osabstraction/commandlineparser/commandlineparser.h
 *        \brief  A Wrapper for an commandline parser
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_COMMANDLINEPARSER_COMMANDLINEPARSER_H_
#define LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_COMMANDLINEPARSER_COMMANDLINEPARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace osabstraction {
namespace commandlineparser {

/**
 * \brief The CommandLineParser function is a command-line parser, that shall follow Utility Syntax Guidelines 3, 4, 5,
 * 6, 7, 9, and 10 in the Base Definitions volume of IEEE Std 1003.1-2001
 * \param argc The argument count.
 * \param argv The option elements
 * \param optstring A string containing the legitimate option characters
 * \returns If an option was successfully found, then CommandLineParser() returns the option character. If all
 * command-line options have been parsed, then CommandLineParser() returns -1. If CommandLineParser() encounters an
 * option character that was not in optstring, then '?' is returned. If CommandLineParser() encounters an option with a
 * missing argument, then the return value depends on the first character in optstring: if it is ':', then ':' is
 * returned; otherwise '?' is returned.
 */
int CommandLineParser(int argc, char *const argv[], const char *optstring);

}  // namespace commandlineparser
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_COMMANDLINEPARSER_COMMANDLINEPARSER_H_

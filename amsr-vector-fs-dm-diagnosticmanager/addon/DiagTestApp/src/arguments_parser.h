/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  amsr-vector-fs-dm-diagnosticmanager/addon/DiagTestApp/src/arguments_parser.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTAPP_SRC_ARGUMENTS_PARSER_H_
#define ADDON_DIAGTESTAPP_SRC_ARGUMENTS_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <osabstraction/commandlineparser/commandlineparser.h>
#include <string>
#include "ara/log/logging.hpp"

namespace diag_test_application {
/**
 * \brief The default path where the ara::com communication file is located.
 */
const char kPathToAraCom[] = "./etc/application.json";

/**
 * \brief Container for command line arguments.
 */
struct CommandLineArguments {
  /**
   * \brief Ctor
   */
  CommandLineArguments() : verbosity_level(ara::log::LogLevel::kWarn) {}

  /**
   * \brief The path where the file for ara::com communication is located.
   */
  std::string path_to_aracom_application_file;

  /**
   * \brief The verbosity level.
   */
  ara::log::LogLevel verbosity_level;
};

/**
 * \brief Implementation of ArgumentsProvider class.
 */
class ArgumentsParser {
 public:
  /**
   * \brief Parses command line arguments.
   *
   * \param argc Command line argument count.
   * \param argv Array of pointers to command line arguments.
   * \return Parsed arguments.
   */
  static CommandLineArguments ParseArguments(int argc, char* const argv[]);

 private:
  /**
   * \brief Prints the usage message.
   *
   * \param progname Program name.
   */
  static void Usage(const char* progname);
};

}  // namespace diag_test_application
#endif  // ADDON_DIAGTESTAPP_SRC_ARGUMENTS_PARSER_H_

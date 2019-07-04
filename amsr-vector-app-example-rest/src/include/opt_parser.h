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
/**        \file  opt_parser.h
 *        \brief  Parser for program execution options
 *
 *********************************************************************************************************************/

#ifndef SRC_INCLUDE_OPT_PARSER_H_
#define SRC_INCLUDE_OPT_PARSER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <getopt.h>
#include <string>

namespace appl {
namespace parse {
/**
 * \brief Container for command line arguments.
 */
struct CommandLineArguments {
  /**
   * \brief Path to configuration file.
   */
  std::string cfg_path_;
  /**
   * \brief Verbosity level.
   */
  std::size_t verbose_;
};

/**
 * \brief Prints the usage message.
 *
 * \param progname Program name.
 */
void Usage(const char* progname) {
  std::cerr << "usage: " << progname << " [-h] [-v] -c <config file path>\n"
                                        "-h                       Print this message and exit.\n"
                                        "-c <config file path>    Specify the location of the configuration file.\n";
}

/**
 * \brief Parses command line arguments.
 *
 * \param argc Command line argument count.
 * \param argv Array of pointers to command line arguments.
 * \return Parsed arguments.
 */
CommandLineArguments ParseArguments(int argc, char* argv[]) {
#ifdef ENABLE_EXEC_MANAGER_SUPPORT_AUTOSAR
  if (argc > 1 && argv[1][0] != '-') {
    // Remove first parameter when launched by AUTOSAR EM
    argv++;
  }
#endif  // ENABLE_EXEC_MANAGER_SUPPORT_AUTOSAR
  CommandLineArguments args;
  int c;
  args.cfg_path_ = "";
  args.verbose_ = 0;

  while ((c = getopt(argc, argv, "hc:")) != -1) {
    switch (c) {
      case 'h':
        Usage(argv[0]);
        exit(EXIT_SUCCESS);
        break;
      case 'c':
        args.cfg_path_ = optarg;
        break;
      case '?':
      default:
        Usage(argv[0]);
        exit(EXIT_FAILURE);
        break;
    }
  }
  if (args.cfg_path_ == "") {
    std::cerr << "No configuration file path provided\n";
    exit(EXIT_FAILURE);
  }

  return args;
}
}  // namespace parse
}  // namespace appl

#endif  // SRC_INCLUDE_OPT_PARSER_H_

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
/**        \file  amsr-vector-fs-dm-diagnosticmanager/src/arguments_parser.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/
#include "arguments_parser.h"

namespace amsr {
namespace diag {

void ArgumentsParser::Usage(const char* progname) {
  ara::log::Logger& logger = ara::log::CreateLogger("Usage", "");
  logger.LogInfo() << "usage: " << progname <<
      R"( [-h] [-v] [-a <ara::com communication file path>] [-m <meta configuration file path>] [-t <transport protocol configuration file path>]
         -h                                                   Print this message and exit.
         -v                                                   Specify the verbosity level.
         -a <ara::com communication file path>                Specify the location of the ara::com communication file.
         -m <meta configuration file path>                    Specify the location of the meta configuration file.
         -t <transport protocol configuration file path>      Specify the location of the transport protocol configuration file.
)";
}

CommandLineArguments ArgumentsParser::ParseArguments(int argc, char* const argv[]) {
  using LogLevelUnderlyingType = std::underlying_type<ara::log::LogLevel>::type;
  amsr::diag::CommandLineArguments args;
  LogLevelUnderlyingType verbose = static_cast<LogLevelUnderlyingType>(ara::log::LogLevel::kWarn);
  int c;
  ara::log::Logger& logger = ara::log::CreateLogger("Arguments parser", "");

  while ((c = osabstraction::commandlineparser::CommandLineParser(argc, argv, "ha:m:t:v")) != -1) {
    switch (c) {
      case 'h':
        Usage(argv[0]);
        exit(EXIT_SUCCESS);
        break;
      case 'a':
        args.path_to_aracom_application_file = optarg;
        break;
      case 'm':
        args.path_to_meta_configuration_file = optarg;
        break;
      case 't':
        args.path_to_transport_protocol_configuration_file = optarg;
        break;
      case 'v':
        // TODO(PAASR-2572) : Use ara::log exported method to increase logLevelvalue.
        if (verbose < static_cast<LogLevelUnderlyingType>(ara::log::LogLevel::kVerbose)) {
          verbose++;
        }
        break;
      case '?':
      default:
        Usage(argv[0]);
        exit(EXIT_FAILURE);
        break;
    }
    args.verbosity_level = static_cast<ara::log::LogLevel>(verbose);
  }

  if (args.path_to_aracom_application_file == "") {
    logger.LogWarn() << "No path_to_aracom_application provided. Using default: " << kPathToAraCom;
    args.path_to_aracom_application_file = kPathToAraCom;
  }
  if (args.path_to_meta_configuration_file == "") {
    logger.LogWarn() << "No path_to_meta_configuration provided. Using default: " << kPathToMetaConfiguration;
    args.path_to_meta_configuration_file = kPathToMetaConfiguration;
  }
  if (args.path_to_transport_protocol_configuration_file == "") {
    logger.LogWarn() << "No path_to_transport_protocol_configuration_file provided. Using default: "
                     << kPathToTPConfiguration;
    args.path_to_transport_protocol_configuration_file = kPathToTPConfiguration;
  }

  return args;
}

}  // namespace diag
}  // namespace amsr

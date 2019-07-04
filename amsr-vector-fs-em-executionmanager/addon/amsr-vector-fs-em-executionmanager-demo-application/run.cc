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
/**        \file  addon/amsr-vector-fs-em-executionmanager-demo-application/run.cc
 *        \brief  -
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <limits.h>
#include <unistd.h>
#include <ara/log/logging.hpp>

#include "application_demo.h"

/**
 * \brief The main function for the demo application.
 */
int main(int argc, char* argv[]) {
  char cwd[PATH_MAX];
  ara::log::InitLogging("DEMO-APP", "Logging context of the demo application", ara::log::LogLevel::kVerbose,
                        ara::log::LogMode::kConsole, "");

  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    ara::log::LogDebug() << __func__ << " ADDON APP: Current working dir: " << cwd;
  }
  bool is_msc = false;
  std::string msc_option("msc");
  ara::log::LogDebug() << __func__ << " ADDON APP: Have " << argc << " arguments:";
  for (int i = 0; i < argc; ++i) {
    ara::log::LogDebug() << __func__ << " ADDON APP: argument " << i << ": " << argv[i];
    if (argv[i] == msc_option) {
      is_msc = true;
    }
  }

  application_demo::AdaptiveApplication app(is_msc);
  app.Start();
}

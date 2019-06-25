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
 *        \brief  Stub implementation of process.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/process/process.h"

namespace osabstraction {
namespace process {

ProcessId GetProcessId() { return 0; }

Process::Process(ProgramImage image, Arguments args) {
  (void)image;
  (void)args;
}

Process::Process(Process&& other) { (void)other; }

Process& Process::operator=(Process&& other) {
  std::swap(other, *this);
  return *this;
}

Process::~Process() {}

bool Process::IsRunning() { return false; }

void Process::SendTerminationRequest() {}

ProcessId Process::GetId() const { return 0; }

Process ProcessBuilder::Build() { return Process(image_, args_); }

}  // namespace process
}  // namespace osabstraction

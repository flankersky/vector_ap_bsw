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
 *        \brief  Implementation of OS independent parts of the process abstraction.
 *
 *      \details  Setter functions for ProcessBuilderInterface.
 *
 *********************************************************************************************************************/

#include "osabstraction/process/processinterface.h"

namespace osabstraction {
namespace process {

ProcessBuilderInterface& ProcessBuilderInterface::AddArgument(Argument arg) {
  args_.push_back(arg);
  return *this;
}

ProcessBuilderInterface& ProcessBuilderInterface::SetProgramImage(ProgramImage image) {
  image_ = image;
  return *this;
}

ProcessBuilderInterface& ProcessBuilderInterface::SetSchedulingPriority(unsigned int scheduling_priority) {
  scheduling_priority_ = scheduling_priority;
  return *this;
}

ProcessBuilderInterface& ProcessBuilderInterface::SetSchedulingPolicy(const SchedulingPolicies& scheduling_policy) {
  scheduling_policy_ = scheduling_policy;
  return *this;
}

ProcessBuilderInterface& ProcessBuilderInterface::SetCpuCoreControl(CPUCoreControl* cpu_core_control) {
  if (cpu_core_control != nullptr) {
    cpu_core_control_ = cpu_core_control;
  }

  return *this;
}

ProcessBuilderInterface& ProcessBuilderInterface::SetProcessCreatedCallout(
    const std::function<void(osabstraction::process::ProcessId)>& callout) {
  process_created_callout_ = callout;
  return *this;
}

}  // namespace process
}  // namespace osabstraction

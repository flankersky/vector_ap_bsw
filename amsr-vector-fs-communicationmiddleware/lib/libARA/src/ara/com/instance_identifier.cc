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
/**        \file  instance_identifier.cc
 *        \brief
 *
 *      \details
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/instance_identifier.h"

#include <cassert>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

namespace ara {
namespace com {

const InstanceIdentifier InstanceIdentifier::Any{""};

std::string InstanceIdentifier::toString() const {
  std::ostringstream oss;
  oss << instance_id_;
  return oss.str();
}

ara::com::InstanceId InstanceIdentifier::ParseString(const std::string& label) {
  if (label.empty()) {
    return ara::com::kInstanceIdAny;
  }
  std::size_t pos{0};
  auto iid = std::stoul(label, &pos, 0);  // determine base automatically
  if (pos != label.length() || iid >= std::numeric_limits<ara::com::InstanceId>::max()) {
    throw std::invalid_argument("Instance identifier must represent a 16 bit wide integer value");
  }
  return static_cast<ara::com::InstanceId>(iid);
}

}  // namespace com
}  // namespace ara

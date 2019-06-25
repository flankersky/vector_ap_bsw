// --------------------------------------------------------------------------
// |              _    _ _______     .----.      _____         _____        |
// |         /\  | |  | |__   __|  .  ____ .    / ____|  /\   |  __ \       |
// |        /  \ | |  | |  | |    .  / __ \ .  | (___   /  \  | |__) |      |
// |       / /\ \| |  | |  | |   .  / / / / v   \___ \ / /\ \ |  _  /       |
// |      / /__\ \ |__| |  | |   . / /_/ /  .   ____) / /__\ \| | \ \       |
// |     /________\____/   |_|   ^ \____/  .   |_____/________\_|  \_\      |
// |                              . _ _  .                                  |
// --------------------------------------------------------------------------
//
// All Rights Reserved.
// Any use of this source code is subject to a license agreement with the
// AUTOSAR development cooperation.
// More information is available at www.autosar.org.
//
// Disclaimer
//
// This work (specification and/or software implementation) and the material
// contained in it, as released by AUTOSAR, is for the purpose of information
// only. AUTOSAR and the companies that have contributed to it shall not be
// liable for any use of the work.
//
// The material contained in this work is protected by copyright and other
// types of intellectual property rights. The commercial exploitation of the
// material contained in this work requires a license to such intellectual
// property rights.
//
// This work may be utilized or reproduced without any modification, in any
// form or by any means, for informational purposes only. For any other
// purpose, no part of the work may be utilized or reproduced, in any form
// or by any means, without permission in writing from the publisher.
//
// The work has been developed for automotive applications only. It has
// neither been developed, nor tested for non-automotive applications.
//
// The word AUTOSAR and the AUTOSAR logo are registered trademarks.
// --------------------------------------------------------------------------

/**
 * \file
 * \brief This file contains common types definitions for the logging API.
 *
 * \attention Due to limitation of the C++ language, some internal implementation
 * (e.g. the function bodies) are coded directly into this header file.
 * Such code is NOT meant to be used or referred to by a user directly,
 * and is subject to CHANGE WITHOUT NOTICE.
 * Therefore DO NOT DEPEND ON any implementation details!
 */

#ifndef LOGCOMMON_H
#define LOGCOMMON_H

#include <memory>

namespace ara {
namespace log {

/**
 * @defgroup common Common Types
 * @brief Common type definitions for the logging API.
 */

/// @addtogroup common
/// @{

/**
 * @brief Log levels, in decreasing order of severity.
 *
 * Maps one-by-one to the values from DltLogLevelType.
 * @sa <dlt/dlt_types.h>
 */
enum class LogLevel : uint8_t {
  kOff,     ///< No logging
  kFatal,   ///< Fatal error, not recoverable
  kError,   ///< Error with impact to correct functionality
  kWarn,    ///< Warning if correct behavior cannot be ensured
  kInfo,    ///< Informational, providing high level understanding
  kDebug,   ///< Detailed information for programmers
  kVerbose  ///< Extra-verbose debug messages (highest grade of information)
};

/**
 * @brief Log mode. Flags, used to configure the sink for log messages.
 * @note In order to combine flags, at least the OR and AND operators needs to be provided for this type.
 */
enum class LogMode : uint8_t {
  kRemote   = 0x01,  ///< Sent remotely
  kFile     = 0x02,  ///< Save to file
  kConsole  = 0x04   ///< Forward to console
};

inline LogMode operator|(LogMode lhs, LogMode rhs)
{
    return (static_cast<LogMode>(static_cast<typename std::underlying_type<LogMode>::type>(lhs) |
                                 static_cast<typename std::underlying_type<LogMode>::type>(rhs)));
}

inline typename std::underlying_type<LogMode>::type operator&(LogMode lhs, LogMode rhs)
{
    return (static_cast<typename std::underlying_type<LogMode>::type>(lhs) &
            static_cast<typename std::underlying_type<LogMode>::type>(rhs));
}

/// @}
} /* namespace log */
} /* namespace ara */

#endif  // LOGCOMMON_H

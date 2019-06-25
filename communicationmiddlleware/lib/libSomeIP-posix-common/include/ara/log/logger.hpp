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
 * \brief This file contains the DLT context abstraction for the logging API.
 *
 * \attention Due to limitation of the C++ language, some internal implementation
 * (e.g. the function bodies) are coded directly into this header file.
 * Such code is NOT meant to be used or referred to by a user directly,
 * and is subject to CHANGE WITHOUT NOTICE.
 * Therefore DO NOT DEPEND ON any implementation details!
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <dlt/dlt.h>

#include "logcommon.hpp"
#include "logstream.hpp"

#include <atomic>

namespace ara {
namespace log {
/**
 * @defgroup logger Log Context
 * @brief DLT context abstraction for the logging API.
 */

/**
 * @ingroup logger
 * @class Logger
 *
 * @brief Class representing a DLT logger context.
 *
 * DLT defines so called _contexts_ which can be seen as logger instances within one application
 * or process scope. Contexts have the same properties to be set up as for the application:
 *  -# ID (_up to four-character_)
 *  -# description text (opt.)
 *  -# default log severity level
 *
 * A context will be automatically registered against the DLT back-end during creation phase, as well
 * as automatically deregistered during process shutdown phase. So the end user does not care for the
 * objects life time.
 *
 * @warning Actually the user is not allowed to create a @c Logger object by himself. @c Logger contexts
 * needs to be created by the provided API call @c CreateLogger(). This is because the internal @c LogManager
 * is tracking all created log contexts and does checking for correct ID naming convention as well
 * as multiple instantiation attempts.
 */
class Logger final
{
  public:
    Logger() = delete;

    /**
     * @brief Creates a @c Logger object.
     *
     * Holds the DLT context which is registered in the DLT back-end.
     *
     * @warning This ctor __MUST__ not be used directly by an application, instead it is designated to
     * be called by the internal LogManager only, since it does track of the instances and cares for
     * the proper de-/registration in the DLT back-end. The only reason why this ctor is not private
     * is some technical issues with _c++11 std::forward_ implementation - it needs the ctor to be public.
     *
     * @param[in] ctxId             The _up to four-character_ ID
     * @param[in] ctxDescription    Some description
     * @param[in] ctxDefLogLevel    The context's default log reporting level
     */
    Logger(const std::string& ctxId, const std::string& ctxDescription, LogLevel ctxDefLogLevel) noexcept;

    /**
     * @brief Deregister the DLT context from the DLT back-end.
     *
     * If not already done explicitly by the LogManager.
     */
    ~Logger();

    /*
     * The big five.
     *
     * We don't want this class movable or copyable, but instead enforce a single ownership relation
     * between the LogManager that handles the object lifetime of the instances of this class.
     */
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    /**
     * @brief Check current configured log reporting level.
     *
     * Applications may want to check the actual configured reporting log level of certain loggers
     * before doing log data preparation that is runtime intensive.
     *
     * @param[in] logLevel  The to be checked log level.
     * @return              True if desired log level satisfies the configured reporting level.
     */
    bool IsLogEnabled(LogLevel logLevel) const noexcept;

    /**
     * @brief Creates a @c LogStream object.
     *
     * Returned object will accept arguments via the _insert stream operator_ "@c <<".
     *
     * @note In the normal usage scenario, the object's life time of the created @c LogStream is scoped
     * within one statement (ends with @c ; after last passed argument). If one wants to extend the
     * @c LogStream object's life time, the object might be assigned to a named variable.
     * @see Refer to the @c LogStream class documentation for further details.
     *
     * @return  @c LogStream object of Fatal severity.
     */
    LogStream LogFatal() noexcept { return LogStream{LogLevel::kFatal, *this}; }
    /**
     * @brief Same as @c Logger::LogFatal().
     * @return  @c LogStream object of Error severity.
     */
    LogStream LogError() noexcept { return LogStream{LogLevel::kError, *this}; }
    /**
     * @brief Same as @c Logger::LogFatal().
     * @return  @c LogStream object of Warn severity.
     */
    LogStream LogWarn() noexcept { return LogStream{LogLevel::kWarn, *this}; }
    /**
     * @brief Same as @c Logger::LogFatal().
     * @return  @c LogStream object of Info severity.
     */
    LogStream LogInfo() noexcept { return LogStream{LogLevel::kInfo, *this}; }
    /**
     * @brief Same as @c Logger::LogFatal().
     * @return  @c LogStream object of Debug severity.
     */
    LogStream LogDebug() noexcept { return LogStream{LogLevel::kDebug, *this}; }
    /**
     * @brief Same as @c Logger::LogFatal().
     * @return  @c LogStream object of Verbose severity.
     */
    LogStream LogVerbose() noexcept { return LogStream{LogLevel::kVerbose, *this}; }

  private:
    /* Deregister the DLT context from the DLT back-end. */
    void unregisterFromDlt() noexcept;

    /*
     * Returns the context ID.
     * @return  The _up to four-character_ ID
     */
    std::string getId() const noexcept;

    std::atomic<bool> isRegistered_{false};
    DltContext dltContext_{};

    friend class LogManager;
    friend LogStream::LogStream(LogLevel, Logger&) noexcept;
};

} /* namespace log */
} /* namespace ara */

#endif // LOGGER_H

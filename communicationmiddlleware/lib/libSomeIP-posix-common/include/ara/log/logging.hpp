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
 * \brief This file is the main public header of the logging API
 *
 * \details It contains methods, macros and utilities for doing DLT logging.
 *
 * \attention Due to limitation of the C++ language, some internal implementation
 * (e.g. the function bodies) are coded directly into this header file.
 * Such code is NOT meant to be used or referred to by a user directly,
 * and is subject to CHANGE WITHOUT NOTICE.
 * Therefore DO NOT DEPEND ON any implementation details!
 */

#ifndef LOGGING_H
#define LOGGING_H

#include <cstring>

#include "logcommon.hpp"
#include "logger.hpp"
#include "logstream.hpp"
#include "version.hpp"

namespace ara {
namespace log {
/**
 * @brief The high-level logging API for AUTOSAR adaptive applications.
 *
 * It is based on the _Genivi DLT back-end_ and provides simple to use log methods and additional
 * formatting utilities. It also abstracts the de-/registration phases as well as the DLT back-end
 * resource handling.
 *
 * This logging API supports the Genivi DLT version __2.16.0__ and newer.
 *
 * @sa <a href="https://github.com/GENIVI/dlt-daemon">DLT-Daemon at Github</a> and
 *     <a href="https://github.com/GENIVI/dlt-viewer">DLT-Viewer at Github</a>
 *
 * The logging API supports the concept of _logging contexts_ as defined by DLT. This means that each log
 * message is associated with a particular log context. The <a href="https://github.com/GENIVI/dlt-viewer">
 * DLT-Viewer</a> application allows for filtering of such contexts as well as adjusting log severity
 * per context during runtime.
 *
 * It is up to the application developer whether he want to use multiple contexts, categorized by
 * sub-functionality, classes/modules, threads or whatever, or whether he just want to use a single
 * default context for the whole application.
 *
 * The logging API offers ready-to-use stream based calls that already supports all primitive data types.
 * The advantage is that applications can extend the logging framework to understand new/custom
 * types simply by providing the relevant stream operators as overloads, without having to touch the
 * framework's code itself.
 *
 * @sa Read description of @c LogStream for how-to write custom type handlers.
 *
 * @par Initialization
 * In order to distinguish the logs from different applications, every application in the system
 * a particular ID (_up to four-character_) and a description has to be assigned.
 * @par
 * These values has to be provided to the internal logging manager in order to register with the
 * DLT back-end. The user also has to specify the application's default log level. Only log messages
 * that have a higher log severity level will be processed. The log level is adjustable during runtime
 * via some client (e.g. <a href="https://github.com/GENIVI/dlt-viewer">DLT-Viewer</a>).
 *
 * @note It is highly recommended to assign system-wide unique IDs per application and even per process,
 * in case same application is started multiple times. If multiple processes registers with the same ID,
 * the logs will be mixed up, and the log output will be hardly to filter and to read. It is advised to
 * keep and maintain a central list of _ID to application/instance_.
 *
 * @attention If some application is allowed to fork, which is normally forbidden in AUTOSAR adaptive,
 * except for a very few special applications, the init call must be done __AFTER__ forking, otherwise
 * we will end up in undefined behavior. Apart from that, the logging API and the DLT back-end is thread
 * safe. That means, logging can be done asynchronous from multiple threads, within one registered
 * application and within same logging contexts.
 *
 * @par Logging contexts
 * To be able to log at least one context needs to be available per application. Logging contexts are
 * represented by the class @c Logger. Hereafter _logger_ or _DLT context_ are used as synonyms
 * for _logging context_.
 * @par
 * If the application requires only trivial logging setup, meaning if there is no need to have more
 * than one context at all, it is not necessary to create a context manually. The logging API will
 * provide one default context, based on lazy-init at the very first attempt to do a message log via
 * the convenience methods _Log<LogLevel>()_.
 * @par
 * Contexts are identified similarly like the application by an _up to four-character_ ID and
 * a description. They can either be defined scoped (within class or function or compilation unit),
 * or just globally. The default context provided by the logging framework, will get the ID _DFLT_.
 *
 * @warning Creating loggers __MUST__ be done via the convenience method @c CreateLogger().
 * Don't call @c Logger's @link
 * Logger::Logger(const std::string& ctxId, const std::string& ctxDescription, LogLevel ctxDefLogLevel)
 * @c ctor @endlink by yourself unless you know what you do.
 *
 * @note For runtime critical applications, it might be of interest to prevent any side effects
 * caused by the lazy-init of the default logger (if used at all) when doing the very first message
 * log. To achieve this, just do a log of some informal nature within your application init-phase.
 * However, this is optional.
 *
 * This is all that needs to be done in order to be able to start logging. Everything else is setup
 * and handled automatically in background.
 *
 * @par Early logging
 * All messages logged before the initialization is done will be stored inside a ring-buffer
 * with a limited size. That means, oldest entries are lost if the buffer exceeds.
 *
 * @note In DLT every context has its own severity level. To make the usage of the API less complex
 * an application-wide log level (that one specified in the init-phase) applies to all contexts until
 * they are changed by some DLT client, remotely. Since the loggers can be created before the
 * initialization call, they are created per default with most verbose level, to enable early logging.
 * That means, until the logging framework is not initialized, it is possible to log already into the
 * globally defined loggers with most verbose severity. As soon as the the logging framework becomes
 * up and running, all contexts are settled to the application-wide log level. This ensures that no
 * messages are lost before the init-phase.
 *
 * @par Usage
 * Only the main public header needs to be included and the logging namespace to shortcut the usage
 * (optional but recommended):
 * @code
 * #include <ara/logging/logging.hpp>
 * using namespace ara::log;
 * @endcode
 * @par
 * Init the application against the logging framework and create one or more contexts (optionally).
 * Then simply create a message using one of the provided log methods _Log<LogLevel>()_ and then
 * use the insert stream operator to build up your log content:
 * @code
 * InitLogging("ABCD", "my awesome app.", LogLevel::kVerbose, LogMode::kRemoteConsole);
 *
 * Logger& g_loggerCtx0{CreateLogger(CTX0, "some context of application ABCD")}
 *
 * g_loggerCtx0.LogDebug() << "The answer is" << 42;
 * LogDebug() << "log into default context provided by the framework";
 * @endcode
 *
 * Behind the scenes, these methods creates a @c LogStream temporary object and prepares it for the
 * given log level. Every passed stream argument is getting copied to the internal message buffer
 * as payload with evaluated type information. There is no need to put spaces in between the single
 * arguments as client applications will represent the arguments anyway one-by-one.
 *
 * @par Message length
 * The maximum size of one single message that can be sent out depends on the underlying
 * _Genivi DLT back-end_ implementation, which is defined in @c DLT_USER_BUF_MAX_SIZE (inside
 * <dlt/dlt_user.h>). So, theoretically developers can add as much arguments as they want to the
 * message stream, if a message exceeds the buffer length, it will be simply cut-off by the back-end.
 * This needs to be kept in mind when dealing with big data that is to be logged, especially in
 * conjunction with the @c RawBuffer() stream handler.
 *
 * @warning Do not log in code triggered by the destruction of global/static objects. As the destruction
 * order of static objects is unspecified, your application may crash due to the fact that the logging
 * framework itself, or other relevant instances, having been already deregistered from DLT and killed
 * prior to your own instances. It is good practice to completely avoid logging in destructors unless
 * you can be absolutely sure that the object in question is destroyed during the runtime of the
 * application, rather than during the cleanup phase for static objects (after exiting @c main()).
 */

/**
 * @defgroup logging Logging API
 * @brief The high-level logging API.
 *
 * Accumulation of function and utilities for using the logging framework.
 */

/// @addtogroup logging
/// @{
/* -------- High-level API -------- */

/**
 * @brief Initialize the logging framework.
 *
 * Inits the logging framework for the application with given properties.
 * In case that in @c logMode the @c kFile flag is set, the directory path needs to be provided.
 * This directory must be writable from the corresponding process UID. The actual file name will be
 * generated by the logging framework.
 *
 * @note The call to @c InitLogging shall be done as early as possible inside the program runnable
 * (normally the @c main() function).
 *
 * @note The behavior of the LogMode flags is not fully supported in current status of implementation.
 * It is not possible to select _console only_ mode. Having a single _kConsole_ flag selected, always
 * will enable also _remote_ mode in addition. Having _file only_ mode however is possible.
 *
 * @code
 * int main(int argc, char* argv[])
 * {
 *   InitLogging("ABCD", "This is the application known as ABCD", LogLevel::kVerbose, LogMode::kRemote);
 * }
 * @endcode
 *
 * @param[in] appId             The _up to four-character_ ID
 * @param[in] appDescription    Some description
 * @param[in] appDefLogLevel    The application's default log reporting level
 * @param[in] logMode           The log mode(s) to be used
 * @param[in] directoryPath     The directory path for the file log mode
 */
void InitLogging(std::string appId,
                 std::string appDescription,
                 LogLevel appDefLogLevel,
                 LogMode logMode,
                 std::string directoryPath = "") noexcept;

/**
 * @brief Creates a Logger instance.
 *
 * Holds the DLT context which is registered in the DLT back-end.
 *
 * @code
 * Logger& g_loggerCtx0{CreateLogger(CTX0, "the default context of application ABCD")}
 * @endcode
 *
 * @param[in] ctxId             The _up to four-character_ ID
 * @param[in] ctxDescription    Some description
 * @return                      @c Logger object of type @c Logger&
 */
Logger& CreateLogger(std::string ctxId, std::string ctxDescription) noexcept;

/**
 * @copydoc Logger::LogFatal()
 * @note This method is a shortcut which applies on the default provided logger.
 *       Same method is also available on a @c Logger object itself.
 *
 * @attention This shortcut operation is based on an internal lazy-init call to @c CreateLogger().
 * Therefore it is advised to call this function __once__ outside safety critical situations.
 */
LogStream LogFatal() noexcept;
/**
 * @brief Same as @c LogFatal().
 * @return  @c LogStream object of Error severity
 */
LogStream LogError() noexcept;
/**
 * @brief Same as @c LogFatal().
 * @return  @c LogStream object of Warn severity
 */
LogStream LogWarn() noexcept;
/**
 * @brief Same as @c LogFatal().
 * @return  @c LogStream object of Info severity
 */
LogStream LogInfo() noexcept;
/**
 * @brief Same as @c LogFatal().
 * @return  @c LogStream object of Debug severity
 */
LogStream LogDebug() noexcept;
/**
 * @brief Same as @c LogFatal().
 * @return  @c LogStream object of Verbose severity
 */
LogStream LogVerbose() noexcept;

/* Additional logging formats */

/**
 * @brief Logs raw binary data by providing a buffer.
 *
 * @code
 * LogInfo() << "This is some raw data:" << RawBuffer(buffer);
 * @endcode
 *
 * @note The max. size of the provided data that can be processed depends on the underlying back-end implementation.
 *
 * @tparam    T     Any type
 * @param[in] value Reference to any buffer
 * @return          @c LogRawBuffer type that has a built-in stream handler
 */
template <typename T>
constexpr LogRawBuffer RawBuffer(const T& value) noexcept
{   // TODO: fine-tune this implementation to check passed size against back-end max. size, via SFINAE
    return LogRawBuffer{static_cast<const void*>(&value), sizeof(T)};
}

/**
 * @brief  Logs decimal numbers in hexadecimal format.
 *
 * Negatives are represented in 2's complement. The number of represented digits depends on the
 * overloaded parameter type length.
 *
 * @code
 * uint16_t num = 42;
 * LogInfo() << "This is some number in hex:" << HexFormat(num);
 * LogInfo() << "This is some negative number in hex:" << HexFormat(-num);
 * @endcode
 *
 * @param[in] value Decimal number to be represented in 8 digits
 * @return          @c LogHex8 type that has a built-in stream handler
 */
constexpr LogHex8  HexFormat(uint8_t value) noexcept { return LogHex8{value}; }

/**
 * @brief Same as @c HexFormat(uint8_t value).
 *
 * @param[in] value Decimal number to be represented in 8 digits
 * @return          @c LogHex16 type that has a built-in stream handler
 */
constexpr LogHex8  HexFormat(int8_t value) noexcept { return LogHex8{static_cast<uint8_t>(value)}; }

/**
 * @brief Same as @c HexFormat(uint8_t value).
 *
 * @param[in] value Decimal number to be represented in 16 digits
 * @return          @c LogHex16 type that has a built-in stream handler
 */
constexpr LogHex16 HexFormat(uint16_t value) noexcept { return LogHex16{value}; }

/**
 * @brief Same as @c HexFormat(uint8_t value).
 *
 * @param[in] value Decimal number to be represented in 16 digits
 * @return          @c LogHex16 type that has a built-in stream handler
 */
constexpr LogHex16  HexFormat(int16_t value) noexcept { return LogHex16{static_cast<uint16_t>(value)}; }

/**
 * @brief Same as @c HexFormat(uint8_t value).
 *
 * @param[in] value Decimal number to be represented in 32 digits
 * @return          @c LogHex32 type that has a built-in stream handler
 */
constexpr LogHex32 HexFormat(uint32_t value) noexcept { return LogHex32{value}; }

/**
 * @brief Same as @c HexFormat(uint8_t value).
 *
 * @param[in] value Decimal number to be represented in 32 digits
 * @return          @c LogHex32 type that has a built-in stream handler
 */
constexpr LogHex32  HexFormat(int32_t value) noexcept { return LogHex32{static_cast<uint32_t>(value)}; }

/**
 * @brief Same as @c HexFormat(uint8_t value).
 *
 * @param[in] value Decimal number to be represented in 64 digits
 * @return          @c LogHex64 type that has a built-in stream handler
 */
constexpr LogHex64 HexFormat(uint64_t value) noexcept { return LogHex64{value}; }

/**
 * @brief Same as @c HexFormat(uint8_t value).
 *
 * @param[in] value Decimal number to be represented in 64 digits
 * @return          @c LogHex64 type that has a built-in stream handler
 */
constexpr LogHex64  HexFormat(int64_t value) noexcept { return LogHex64{static_cast<uint64_t>(value)}; }

/**
 * @brief Logs decimal numbers in binary format.
 *
 * Negatives are represented in 2's complement. The number of represented digits depends on the
 * overloaded parameter type length.
 *
 * @code
 * LogInfo() << "This is some number in bin:" << BinFormat(42);
 * LogInfo() << "This is some negative number in bin:" << BinFormat(-42);
 * @endcode
 *
 * @param[in] value Decimal number to be represented in 8 digits
 * @return          @c LogBin8 type that has a built-in stream handler
 */
constexpr LogBin8  BinFormat(uint8_t value) noexcept { return LogBin8{value}; }

/**
 * @brief Same as @c BinFormat(uint8_t value).
 *
 * @param[in] value Decimal number to be represented in 8 digits
 * @return          @c LogBin8 type that has a built-in stream handler
 */
constexpr LogBin8  BinFormat(int8_t value) noexcept { return LogBin8{static_cast<uint8_t>(value)}; }

/**
 * @brief Same as @c BinFormat(uint8_t value).
 *
 * @param[in] value Decimal number to be represented in 16 digits
 * @return          @c LogBin16 type that has a built-in stream handler
 */
constexpr LogBin16 BinFormat(uint16_t value) noexcept { return LogBin16{value}; }

/**
 * @brief Same as @c BinFormat(uint8_t value).
 *
 * @param[in] value Decimal number to be represented in 16 digits
 * @return          @c LogBin16 type that has a built-in stream handler
 */
constexpr LogBin16  BinFormat(int16_t value) noexcept { return LogBin16{static_cast<uint16_t>(value)}; }

/**
 * @brief Same as @c BinFormat(uint8_t value).
 *
 * @param[in] value Decimal number to be represented in 32 digits
 * @return          @c LogBin32 type that has a built-in stream handler
 */
constexpr LogBin32 BinFormat(uint32_t value) noexcept { return LogBin32{value}; }

/**
 * @brief Same as @c BinFormat(uint8_t value).
 *
 * @param[in] value Decimal number to be represented in 32 digits
 * @return          @c LogBin32 type that has a built-in stream handler
 */
constexpr LogBin32 BinFormat(int32_t value) noexcept { return LogBin32{static_cast<uint32_t>(value)}; }

/**
 * @brief Same as @c BinFormat(uint8_t value).
 *
 * @param[in] value Decimal number to be represented in 64 digits
 * @return          @c LogBin64 type that has a built-in stream handler
 */
constexpr LogBin64 BinFormat(uint64_t value) noexcept { return LogBin64{value}; }

/**
 * @brief Same as @c BinFormat(uint8_t value).
 *
 * @param[in] value Decimal number to be represented in 64 digits
 * @return          @c LogBin64 type that has a built-in stream handler
 */
constexpr LogBin64 BinFormat(int64_t value) noexcept { return LogBin64{static_cast<uint64_t>(value)}; }

/* Some utilities */

/**
 * @copydoc Logger::IsLogEnabled()
 * @code
 * if (IsLogEnabled(LogLevel::Verbose)) {
 *   [...] // some log data preparation that costs runtime resources
 *   LogVerbose() << "This is some hard calculated log data...";
 * }
 * @endcode
 *
 * @note This method is a shortcut which applies on the default provided logger.
 *       Same method is also available on a Logger object itself.
 */
bool IsLogEnabled(LogLevel logLevel) noexcept;

/* -------- END of API -------- */
/// @}

} /* namespace log */
} /* namespace ara */

#endif // LOGGING_H

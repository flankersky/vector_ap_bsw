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
 * \brief This file contains the stream based message handler.
 *
 * \attention Due to limitation of the C++ language, some internal implementation
 * (e.g. the function bodies) are coded directly into this header file.
 * Such code is NOT meant to be used or referred to by a user directly,
 * and is subject to CHANGE WITHOUT NOTICE.
 * Therefore DO NOT DEPEND ON any implementation details!
 */

#ifndef LOGSTREAM_H
#define LOGSTREAM_H

#include <type_traits>
#include <string>

#include <dlt/dlt.h>

#include "logcommon.hpp"

namespace ara {
namespace log {
/**
 * @defgroup logstream Log Stream
 * @brief Stream based message handler.
 */

/**
 * @ingroup logstream
 * @class LogStream
 *
 * @brief Class representing a log message.
 *
 * Insert stream operators are to be used for appending data.
 *
 * @note Normally, you would not use this class directly, but use one of the log methods provided in
 * the main logging API or an instance of the Logger class instead. Those methods automatically setup
 * an object of this class with the given log severity level. The only reason to get in touch with this
 * class directly is if you want to hold a @c LogStream object longer than the default one-statement scope.
 * This is useful in order to create log messages that are distributed over multiple code lines.
 * See @c Flush() method for further information.
 *
 * Every call on the insert stream operator against this object will be processed as a call to the
 * underlying DLT back-end, which means that the message buffer is going to be filled with payload data.
 *
 * Once this object gets out of scope (in normal usage as soon as the statement ends), its @c dtor is
 * calling the message flush command on the DLT back-end which marks the message buffer as ready to be
 * sent out remotely.
 *
 * @c LogStream supports all of the primitive data types (PoDs) natively. However, it can be easily extended
 * for your own complex types by providing a stream operator that makes use of already supported types.
 *
 * @code
 * struct MyCustomType {
 *   int8_t foo;
 *   std::string bar;
 * };
 *
 * LogStream& operator<<(LogStream& out, const MyCustomType& value) {
 *     return (out << value.foo << value.bar);
 * }
 *
 * g_loggerCtx0.LogDebug() << MyCustomType{42, "The answer is"};
 * @endcode
 */

class Logger; // forward declare Logger class

/*
 * Helper struct that is utilized as custom type.
 * Holds a pointer to some data and the size which is to be logged as raw data.
 */
struct LogRawBuffer {
    const void* const buffer;
    uint16_t size; // max. allowed size of the back-end
};

/*
 * Helper struct that is utilized as custom type.
 * Holds an integer value that will be logged with a special format.
 */
struct LogHex8  { uint8_t  value; };
struct LogHex16 { uint16_t value; };
struct LogHex32 { uint32_t value; };
struct LogHex64 { uint64_t value; };
struct LogBin8  { uint8_t  value; };
struct LogBin16 { uint16_t value; };
struct LogBin32 { uint32_t value; };
struct LogBin64 { uint64_t value; };

class LogStream final
{
  public:
    LogStream() = delete;

    /**
     * @brief Creates the message stream object.
     *
     * Initiates it with the given log level directly on the back-end.
     *
     * @param[in] logLevel  The severity level of this message
     * @param[in] logger    The associated logger context
     */
    LogStream(LogLevel logLevel, Logger& logger) noexcept;

    /**
     * @brief Flushes the log message before destroying the stream object.
     *
     * Unless it was not explicitly flushed before, by calling @c Flush().
     */
    ~LogStream();

    /*
     * The big five.
     *
     * We actually don't want this class movable or copyable, but enforce RVO instead.
     * Since "Guaranteed copy elision" will be supported up from C++17, declaring the move ctor
     * without definition is a workaround and is necessary because we have a custom dtor.
     * With the missing move ctor implementation, explicit calls of std::move() on objects
     * of this class, will be prevented by a compile error.
     */
    LogStream(const LogStream&) = delete;
    LogStream& operator=(const LogStream&) = delete;
    LogStream(LogStream&&);
    LogStream& operator=(LogStream&&) = delete;

    /**
     * @brief Processes the current log buffer.
     *
     * And prepares the object for new message in one go. Further usage of the @c LogStream object
     * after calling this method will end up in a new message. Use this method in case you want to
     * build your message distributed over multiple code lines.
     *
     * @code
     * LogStream logInfoBuilder{g_loggerCtx0.LogInfo()};
     * logInfoBuilder << "message builder...";
     *
     * logInfoBuilder << 123;
     * logInfoBuilder << "one more param.";
     * logInfoBuilder << 4.56;
     * logInfoBuilder << "we're done...";
     * logInfoBuilder.Flush(); // sends out what is in the buffer as one single message
     *
     * logInfoBuilder << "new message initiated...";
     * @endcode
     *
     * @note Calling @c Flush() is only necessary if the @c LogStream object is going to be re-used within
     * the same scope. Otherwise, if the object goes out of scope (e.g. end of function block), than
     * flush operation will be anyway done internally by the @c dtor.
     */
    void Flush() noexcept;

    /// @name DLT primitive types
    /// Build-in support for DLT native types.
    /// @{
    /** @brief Writes boolean parameter into message. */
    LogStream& operator<<(bool value) noexcept;
    /** @brief Writes unsigned int 8 bit parameter into message. */
    LogStream& operator<<(uint8_t value) noexcept;
    /** @brief Writes unsigned int 16 bit parameter into message. */
    LogStream& operator<<(uint16_t value) noexcept;
    /** @brief Writes unsigned int 32 bit parameter into message. */
    LogStream& operator<<(uint32_t value) noexcept;
    /** @brief Writes unsigned int 64 bit parameter into message. */
    LogStream& operator<<(uint64_t value) noexcept;
    /** @brief Writes signed int 8 bit parameter into message. */
    LogStream& operator<<(int8_t value) noexcept;
    /** @brief Writes signed int 16 bit parameter into message. */
    LogStream& operator<<(int16_t value) noexcept;
    /** @brief Writes signed int 32 bit parameter into message. */
    LogStream& operator<<(int32_t value) noexcept;
    /** @brief Writes signed int 64 bit parameter into message. */
    LogStream& operator<<(int64_t value) noexcept;
    /** @brief Writes float 32 bit parameter into message. */
    LogStream& operator<<(float value) noexcept;
    /** @brief Writes float 64 bit parameter into message. */
    LogStream& operator<<(double value) noexcept;
    /** @brief Writes null terminated UTF8 string into message. */
    LogStream& operator<<(const char* const value) noexcept;
    /// @}

    /// @name DLT special types
    /// Build-in support for DLT special format types.
    /// @{
    /** @brief Writes plain binary data into message. */
    LogStream& operator<<(const LogRawBuffer& value) noexcept;
    /** @brief Writes unsigned int parameter into message, formatted as hexadecimal 8 digits. */
    LogStream& operator<<(const LogHex8& value) noexcept;
    /** @brief Writes unsigned int parameter into message, formatted as hexadecimal 16 digits. */
    LogStream& operator<<(const LogHex16& value) noexcept;
    /** @brief Writes unsigned int parameter into message, formatted as hexadecimal 32 digits. */
    LogStream& operator<<(const LogHex32& value) noexcept;
    /** @brief Writes unsigned int parameter into message, formatted as hexadecimal 64 digits. */
    LogStream& operator<<(const LogHex64& value) noexcept;
    /** @brief Writes unsigned int parameter into message, formatted as binary 8 digits. */
    LogStream& operator<<(const LogBin8& value) noexcept;
    /** @brief Writes unsigned int parameter into message, formatted as binary 16 digits. */
    LogStream& operator<<(const LogBin16& value) noexcept;
    /// @}

  private:
    DltReturnValue dltRet_{DLT_RETURN_OK};
    DltContextData dltLocalData_{};
};

/* Support for LogStream objects as rvalue reference. Enables custom type <<operator overload as first call */
template <typename T>
LogStream& operator<<(LogStream&& out, T&& value)  noexcept
{
    return (out << std::forward<T>(value));
}

/* Support for signed long long (transforms into int64_t on 64 bit platforms) */
template <typename T = void,
          typename std::enable_if<((!std::is_same<long long, int64_t>::value) &&
                                     (sizeof(long long) == sizeof(int64_t))), T>::type* = nullptr>
inline LogStream& operator<<(LogStream& out, long long value) noexcept
{
    return (out << static_cast<int64_t>(value));
}

/* Support for signed long (transforms into int32_t on 32 bit platforms) */
template <typename T = void,
          typename std::enable_if<((!std::is_same<long, int32_t>::value) &&
                                     (sizeof(long) == sizeof(int32_t))), T>::type* = nullptr>
inline LogStream& operator<<(LogStream& out, long value) noexcept
{
    return (out << static_cast<int32_t>(value));
}

/* Support for unsigned long long (transforms into uint64_t on 64 bit platforms) */
template <typename T = void,
          typename std::enable_if<((!std::is_same<unsigned long long, uint64_t>::value) &&
                                     (sizeof(unsigned long long) == sizeof(uint64_t))), T>::type* = nullptr>
inline LogStream& operator<<(LogStream& out, unsigned long long value) noexcept
{
    return (out << static_cast<uint64_t>(value));
}

/* Support for unsigned long (transforms into uint32_t on 32 bit platforms) */
template <typename T = void,
          typename std::enable_if<((!std::is_same<unsigned long, uint32_t>::value) &&
                                     (sizeof(unsigned long) == sizeof(uint32_t))), T>::type* = nullptr>
inline LogStream& operator<<(LogStream& out, unsigned long value) noexcept
{
    return (out << static_cast<uint32_t>(value));
}

constexpr LogHex32 loghex(uint32_t val) noexcept { return LogHex32{val}; }
constexpr LogHex64 loghex(uint64_t val) noexcept { return LogHex64{val}; }

/// @name Extra types
/// Support for extra types that transforms into DLT native types.
/// @addtogroup logstream
/// @{
/** @brief Writes unsigned int parameter into message, formatted as binary 32 digits. */
inline LogStream& operator<<(LogStream& out, const LogBin32& value) noexcept
{   /* Currently the Genivi DLT back-end does not natively support binary format >16 bit,
     * so we will split it up into multiple parts - as workaround */
    return out << LogBin16{static_cast<uint16_t>((value.value >> 16u) & 0x0000FFFFu)}
               << LogBin16{static_cast<uint16_t>(value.value & 0x0000FFFFu)};
}

/** @brief Writes unsigned int parameter into message, formatted as binary 64 digits. */
inline LogStream& operator<<(LogStream& out, const LogBin64& value) noexcept
{   /* Currently the Genivi DLT back-end does not natively support binary format >16 bit,
     * so we will split it up into multiple parts - as workaround */
    return out << LogBin16{static_cast<uint16_t>((value.value >> 48u) & 0x000000000000FFFFu)}
               << LogBin16{static_cast<uint16_t>((value.value >> 32u) & 0x000000000000FFFFu)}
               << LogBin16{static_cast<uint16_t>((value.value >> 16u) & 0x000000000000FFFFu)}
               << LogBin16{static_cast<uint16_t>(value.value & 0x000000000000FFFFu)};
}

/** @brief Writes LogLevel enum parameter as text into message. */
LogStream& operator<<(LogStream& out, LogLevel value) noexcept;

/** @brief Writes pointer address into message, formatted as hexadecimal. */
inline LogStream& operator<<(LogStream& out, const void* const value) noexcept
{
    return (out << loghex(reinterpret_cast<std::uintptr_t>(value)));
}

/** @brief Writes std::string into message. */
inline LogStream& operator<<(LogStream& out, const std::string& value) noexcept
{
    return (out << value.c_str());
}

/** @brief Writes std::string into message. */
template < class charT,
        class traits = std::char_traits<charT>,    // basic_string::traits_type
        class Alloc = std::allocator<charT>        // basic_string::allocator_type
> 
inline LogStream& operator<<(LogStream& out, const std::basic_string<charT, traits, Alloc>& value) noexcept
{
    return (out << value.c_str());
}

/// @}

} /* namespace log */
} /* namespace ara */

#endif // LOGSTREAM_H

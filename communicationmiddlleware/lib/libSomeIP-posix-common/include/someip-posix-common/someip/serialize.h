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
/**        \file  serialize.h
 *        \brief  TODO
 *
 *      \details  SOME/IP compliant (de)serializer components
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_SERIALIZE_H_
#define LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_SERIALIZE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <iterator>

namespace someip_posix_common {
namespace someip {

/** \brief Basic payload type
 *
 */
using Byte = std::uint8_t;

// Internal functionality of serializer
namespace serialization {

/******************************************************************************
 * Byte order support
 *
 */

// If there is no such macro, we fall back to a runtime check. This check is performed only once
// prior to main() and as such does cause only insignificant overhead at runtime. Ideally, we
// should not need it (__BYTE_ORDER__ is a implementation-definded system macro)
#ifndef __BYTE_ORDER__

/** \brief Portable runtime endianess test
 *  This type is carefully crafted to provide efficient, branch-free access to a read-only denoting
 *  endianess.
 */
extern struct IsMSBTest {
  /// Constructor (performs the actual test)
  IsMSBTest() noexcept;
  /** \brief Returns whether current host is a Big Endian machine
   *  \return true if Big Endian
   */
  operator bool() const noexcept { return isMsb_; }

 private:
  bool isMsb_;
} __isMSBTest;  ///< Internal use only

#endif

/** \brief Returns true if system is big-endian
 *
 */
static inline constexpr bool IsHostMSB() noexcept {
// Preprocessor macros not portable. If missing we switch to a one-time-evaluated online check
#ifdef __BYTE_ORDER__
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  return false;
#else
  return true;
#endif
#else
  return __isMSBTest;
#endif
}

/** \brief Swap 8bit value
 *  \param v value to byte swap
 *  \return swapped input
 */
inline constexpr std::uint8_t SwapByteOrder(std::uint8_t v) noexcept { return v; }

/** \brief Swap 8bit signed value
 *  \param v value to byte swap
 *  \return swapped input
 */
inline constexpr std::int8_t SwapByteOrder(std::int8_t v) noexcept {
  return SwapByteOrder(static_cast<std::uint8_t>(v));
}

/** \brief Swap 16bit value
 *  \param v value to byte swap
 *  \return swapped input
 */
static constexpr inline std::uint16_t SwapByteOrder(std::uint16_t v) noexcept {
  return static_cast<std::uint16_t>((v << 8) | ((v >> 8) & 0x00fful));
}

/** \brief Swap 16bit signed value
 *  \param v value to byte swap
 *  \return swapped input
 */
inline constexpr std::int16_t SwapByteOrder(std::int16_t v) noexcept {
  return SwapByteOrder(static_cast<std::uint16_t>(v));
}

/** \brief Swap 32bit value
 *  \param v value to byte swap
 *  \return swapped input
 */
static constexpr inline std::uint32_t SwapByteOrder(std::uint32_t v) noexcept {
  return static_cast<std::uint32_t>(((v >> 24)) | ((v >> 8) & 0xff00ul) | ((v << 8) & 0xff0000lu) | ((v << 24)));
}

/** \brief Swap 32bit signed value
 *  \param v value to byte swap
 *  \return swapped input
 */
inline constexpr std::int32_t SwapByteOrder(std::int32_t v) noexcept {
  return SwapByteOrder(static_cast<std::uint32_t>(v));
}

/** \brief Swap 64bit value
 *  \param v value to byte swap
 *  \return swapped input
 */
inline constexpr std::uint64_t SwapByteOrder(std::uint64_t v) noexcept {
  return ((v >> 56)) | ((v >> 40) & 0xff00ull) | ((v >> 24) & 0xff0000ull) | ((v >> 8) & 0xff000000ull) | ((v << 56)) |
         ((v << 40) & 0xff000000000000ull) | ((v << 24) & 0xff0000000000ull) | ((v << 8) & 0xff00000000ull);
}

/** \brief Swap 64bit signed value
 *  \param v value to byte swap
 *  \return swapped input
 */
inline constexpr std::int64_t SwapByteOrder(std::int64_t v) noexcept {
  return SwapByteOrder(static_cast<std::uint64_t>(v));
}

/** \brief Swap float value
 *  \param v value to byte swap
 *  \return swapped input
 */
inline float SwapByteOrder(float v) noexcept {
  // This implementation avoids type aliasing warnings
  float ret;
  std::uint8_t* in = reinterpret_cast<std::uint8_t*>(&v);
  std::uint8_t* out = reinterpret_cast<std::uint8_t*>(&ret);
  out[0] = in[3];
  out[1] = in[2];
  out[2] = in[1];
  out[3] = in[0];
  return (ret);
}

/** \brief Swap double value
 *  \param v value to byte swap
 *  \return swapped input
 */
inline double SwapByteOrder(double v) noexcept {
  // This implementation avoids type aliasing warnings
  double ret;
  std::uint8_t* in = reinterpret_cast<std::uint8_t*>(&v);
  std::uint8_t* out = reinterpret_cast<std::uint8_t*>(&ret);
  out[0] = in[7];
  out[1] = in[6];
  out[2] = in[5];
  out[3] = in[4];
  out[4] = in[3];
  out[5] = in[2];
  out[6] = in[1];
  out[7] = in[0];
  return (ret);
}

/** \brief Converts any integral type to its Big Endian form
 *  \param v value to convert
 *  \return converted value
 */
template <typename T>
inline constexpr T ToMSB(T v) {
  return IsHostMSB() ? v : SwapByteOrder(v);
}

/** \brief Converts any integral type to its Big Endian form
 *  \param v value to convert
 *  \return converted value
 */
template <typename T>
inline constexpr T FromMSB(T v) {
  return IsHostMSB() ? v : SwapByteOrder(v);
}

/** \brief Converts any integral type to its Little Endian form
 *  \param v value to convert
 *  \return converted value
 */
template <typename T>
inline constexpr T ToLSB(T v) {
  return IsHostMSB() ? SwapByteOrder(v) : v;
}

/** \brief Converts any integral type to its Little Endian form
 *  \param v value to convert
 *  \return converted value
 */
template <typename T>
inline constexpr T FromLSB(T v) {
  return IsHostMSB() ? SwapByteOrder(v) : v;
}

/******************************************************************************
 * Serialization traits
 *
 */

/** \brief Denotes the byte order to apply
 *
 */
enum class ByteOrder {
  kMostSignificantByteFirst,  ///< Big Endian
  kMostSignificantByteLast,   ///< Little Endian
  kOpaque                     ///< Runtime dynamic encoding
};

/** \brief Denotes string encoding
 *
 */
enum class StringEncoding {
  kUTF8,     ///< UTF8 encoding
  kUTF16LE,  ///< UTF16 Little Endian (LSB) encoding
  kUTF16BE,  ///< UTF16 Big Endian (MSB) encoding
};

/** \brief Default serialization policy
 *  Derive and override values as needed
 */
struct DefaultPolicy {
  static constexpr ByteOrder kByteOrder = ByteOrder::kMostSignificantByteFirst;  ///< default byte order
  static constexpr std::size_t kLengthFieldSize = 4;                             ///< default value for length fields
  static constexpr StringEncoding kStringEncoding = StringEncoding::kUTF8;       ///< default string encoding
  static constexpr bool kStringTerminal = false;  ///< denotes whether an explicit terminal char is appended
};

/** \brief Converts the size of a type to the fixed length integer type of the same length
 *
 */
template <int N>
struct TypeLengthToFixedLengthIntType;

/** Invalid type length
 *
 */
static constexpr std::size_t kInvalidTypeLength = 0;

/** \brief Specialization for 0 bytes
 *
 */
template <>
struct TypeLengthToFixedLengthIntType<kInvalidTypeLength> {
  using type = std::uint8_t;  ///< Fixed length integer type
};

/** \brief Specialization for 1 byte
 *
 */
template <>
struct TypeLengthToFixedLengthIntType<sizeof(std::uint8_t)> {
  using type = std::uint8_t;  ///< Fixed length integer type
};

/** \brief Specialization for 2 bytes
 *
 */
template <>
struct TypeLengthToFixedLengthIntType<sizeof(std::uint16_t)> {
  using type = std::uint16_t;  ///< Fixed length integer type
};

/** \brief Specialization for 4 bytes
 *
 */
template <>
struct TypeLengthToFixedLengthIntType<sizeof(std::uint32_t)> {
  using type = std::uint32_t;  ///< Fixed length integer type
};

/** \brief Specialization for 8 bytes
 *
 */
template <>
struct TypeLengthToFixedLengthIntType<sizeof(std::uint64_t)> {
  using type = std::uint64_t;  ///< Fixed length integer type
};

/** \brief SOME/IP message serialization policy
 *  Derive and override values as needed
 */
struct SomeIpMessagePolicy : DefaultPolicy {
  static constexpr ByteOrder kByteOrder =
      ByteOrder::kMostSignificantByteFirst;  ///< SOME/IP protocol requires big-endian byte order
  static constexpr std::size_t kLengthFieldSize = kInvalidTypeLength;  ///< No length field
};

/** \brief Determines whether enumerate is callable with the specified type.
 *
 * \tparam S A type for which it will be tested whether an enumerate overload exists
 * \tparam V A visitor which is passed to enumerate
 */
template <typename S, typename V>
struct IsEnumerateInvocable {
  /**
   * \brief Compiler selects this overload in case the enumerate call will succeed.
   *
   * \return true.
   */
  template <typename St, typename Vt>
  static constexpr decltype(enumerate(std::declval<St&>(), std::declval<Vt>()), bool{}) test(int) {
    return true;
  }
  /**
   * \brief Compiler selects this overload in case the enumerate call will not succeed.
   *
   * \return false.
   */
  template <typename St, typename Vt>
  static constexpr bool test(...) {
    return false;
  }
  /**
   * \brief true if the enumerate call can be made and false otherwise.
   */
  static constexpr bool value = test<S, V>(int{});
};

/**
 * \brief Iterator for serialization
 *
 * We require access to the protected member variable 'container' of std::back_insert_iterator.
 */
template <typename C>
struct serialize_iterator : std::back_insert_iterator<C> {
  /**
   * \brief Import all member methods of back_insert_itreator.
   */
  using std::back_insert_iterator<C>::back_insert_iterator;
  /**
   * \brief Make public the protected member variable.
   */
  using std::back_insert_iterator<C>::container;
};

/**
 * \brief Creates a serialization iterator from a container.
 *
 * \param c A container.
 * \return A serialization iterator.
 */
template <typename Container>
serialize_iterator<Container> make_serialize_iterator(Container& c) {
  return serialize_iterator<Container>(c);
}

/******************************************************************************
 * Read support
 *
 */

/** \brief Read a integral value of type T into a buffer (big endian)
 *  \param pos iterator that denotes the position to insert into
 *  \param v value to serialize
 *  \return iterator position after reading
 */
template <typename Policy = DefaultPolicy, typename IterType, typename DataType>
typename std::enable_if<((std::is_integral<DataType>::value || std::is_floating_point<DataType>::value) &&
                         Policy::kByteOrder == ByteOrder::kMostSignificantByteFirst),
                        IterType>::type
read(IterType pos, DataType v) {
  v = ToMSB(v);
  auto* first = reinterpret_cast<Byte*>(&v);
  auto* last = first + sizeof(DataType);
  return std::copy(first, last, pos);
}

/**
 *  \brief Read an enumeration value of type T into a buffer
 *  \tparam Policy serialization policy
 *  \tparam IterType serialization iterator type
 *  \tparam DataType data type of value to serialize
 *  \param pos iterator that denotes the position to insert into
 *  \param v value to serialize
 *  \return iterator position after reading
 */
template <typename Policy = DefaultPolicy, typename IterType, typename DataType>
typename std::enable_if<(std::is_enum<DataType>::value), IterType>::type read(IterType pos, DataType v) {
  using T = typename std::underlying_type<DataType>::type;
  return read<Policy>(pos, static_cast<T>(v));
}

/** \brief  Read a integral value of type T into a buffer (little endian)
 *  \param pos iterator that denotes the position to insert into
 *  \param v value to serialize
 *  \return iterator position after reading
 */
template <typename Policy = DefaultPolicy, typename IterType, typename DataType>
typename std::enable_if<((std::is_integral<DataType>::value || std::is_floating_point<DataType>::value) &&
                         Policy::kByteOrder == ByteOrder::kMostSignificantByteLast),
                        IterType>::type
read(IterType pos, DataType v) {
  v = ToLSB(v);
  auto* first = reinterpret_cast<Byte*>(&v);
  auto* last = first + sizeof(DataType);
  return std::copy(first, last, pos);
}

/******************************************************************************
 * Write support
 *
 */

/** \brief  Write a value of integral type from a buffer (big endian)
 *  \param v value to deserialize into
 *  \param pos current position in buffer
 *  \return new position in buffer
 */
template <typename Policy = DefaultPolicy, typename DataType, typename IterType>
typename std::enable_if<
    (std::is_integral<DataType>::value && Policy::kByteOrder == ByteOrder::kMostSignificantByteFirst), IterType>::type
write(DataType& v, IterType pos) {
  v = FromMSB(*reinterpret_cast<DataType*>(&*pos));
  return pos + sizeof(DataType);
}

/** Write a value of integral type from a buffer (little endian)
 *  \param v value to deserialize into
 *  \param pos current position in buffer
 *  \return new position in buffer
 */
template <typename Policy = DefaultPolicy, typename DataType, typename IterType>
typename std::enable_if<
    (std::is_integral<DataType>::value && Policy::kByteOrder == ByteOrder::kMostSignificantByteLast), IterType>::type
write(DataType& v, IterType pos) {
  v = FromLSB(*reinterpret_cast<DataType*>(&*pos));
  return pos + sizeof(DataType);
}

/** \brief  Write a value of float type from a buffer (big endian)
 *  \param v value to deserialize into
 *  \param pos current position in buffer
 *  \return new position in buffer
 */
template <typename Policy = DefaultPolicy, typename DataType, typename IterType>
typename std::enable_if<(std::is_floating_point<DataType>::value &&
                         Policy::kByteOrder == ByteOrder::kMostSignificantByteFirst),
                        IterType>::type
write(DataType& v, IterType pos) {
  typename std::aligned_storage<sizeof(Byte), alignof(DataType)>::type data[sizeof(DataType)];
  std::copy_n(pos, sizeof(DataType), reinterpret_cast<Byte*>(data));
  v = FromMSB(*reinterpret_cast<DataType*>(data));
  return pos + sizeof(DataType);
}

/** Write a value of float type from a buffer (little endian)
 *  \param v value to deserialize into
 *  \param pos current position in buffer
 *  \return new position in buffer
 */
template <typename Policy = DefaultPolicy, typename DataType, typename IterType>
typename std::enable_if<(std::is_floating_point<DataType>::value &&
                         Policy::kByteOrder == ByteOrder::kMostSignificantByteLast),
                        IterType>::type
write(DataType& v, IterType pos) {
  typename std::aligned_storage<sizeof(Byte), alignof(DataType)>::type data[sizeof(DataType)];
  std::copy_n(pos, sizeof(DataType), reinterpret_cast<Byte*>(data));
  v = FromLSB(*reinterpret_cast<DataType*>(data));
  return pos + sizeof(DataType);
}

}  // namespace serialization

}  // namespace someip
}  // namespace someip_posix_common

#endif  // LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_SERIALIZE_H_

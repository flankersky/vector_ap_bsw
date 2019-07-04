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
/**        \file  marshalling.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_MARSHALLING_H_
#define LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_MARSHALLING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <cassert>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

#include "someip-posix-common/someip/message.h"
#include "someip-posix-common/someip/serialize.h"

namespace someip_posix_common {
namespace someip {
namespace serialization {

/// Easy-to-read type for Little-Endian
struct LittleEndian {
  /// Define a constant expression for the conventional de/serializer.
  static constexpr ByteOrder value{ByteOrder::kMostSignificantByteLast};
};

/// Easy-to-read type for Big-Endian
struct BigEndian {
  /// Define a constant expression for the conventional de/serializer.
  static constexpr ByteOrder value{ByteOrder::kMostSignificantByteFirst};
};

/// Easy-to-read type for Network-Byte-Order, which is Big-Endian
using NetworkByteOrder = BigEndian;

/**
 * \brief For TLV and dynamic length arrays AUTOSAR specifies a length-field.
 * The length field could be either 0, 1, 2 or 4 bytes.
 * This template-class helps to get the size on compile-time and also checks for incorrect configuration.
 *
 * \uptrace PRS_SOMEIP_00376
 * \uptrace PRS_SOMEIP_00107
 *
 * \tparam LengthFieldType Could be either void (which stands for 0 bits), uint8, uint16 or uint32
 * in conformance with the AUTOSAR SWS.
 */
template <typename LengthFieldType>
struct LengthField;

/**
 * \brief This is the template specialization for the case
 * LengthField<void>, which is equal to "no length field".
 * \uptrace PRS_SOMEIP_00376
 * \uptrace PRS_SOMEIP_00107
 */
template <>
struct LengthField<void> {
  /**
   * \brief Method to return the length in bytes of the length field.
   *
   * \return zero for this length field specialization.
   */
  static constexpr std::size_t GetLengthFieldSize() { return 0U; }
};

/**
 * \brief Template-specialization for the size of 1 byte.
 */
template <>
struct LengthField<std::uint8_t> {
  /**
   * \brief Method to return the length in bytes of the configured length field
   * from the data definition.
   */
  static constexpr std::size_t GetLengthFieldSize() { return 1U; }
};

/**
 * \brief Template-specialization for the size of 2 bytes.
 */
template <>
struct LengthField<std::uint16_t> {
  /**
   * \brief Method to return the length in bytes of the configured length field
   * from the data definition.
   */
  static constexpr std::size_t GetLengthFieldSize() { return 2U; }
};

/**
 * \brief Template-specialization for the size of 2 bytes.
 */
template <>
struct LengthField<std::uint32_t> {
  /**
   * \brief Method to return the length in bytes of the configured length field
   * from the data definition.
   */
  static constexpr std::size_t GetLengthFieldSize() { return 4U; }
};

namespace tlv {

/**
 * \brief Specifies the available wire types from the AUTOSAR specification.
 * \uptrace PRS_SOMEIP_00205
 */
enum class WireTypeEnum : std::uint8_t {
  /// 8 Bit Data Base data type
  FundamentalType8Bit = 0x00U,
  /// 16 Bit Data Base data type
  FundamentalType16Bit = 0x01U,
  /// 32 Bit Data Base data type
  FundamentalType32Bit = 0x02U,
  /// 64 Bit Data Base data type
  FundamentalType64Bit = 0x03U,
  /// Complex Data Type: Array, Struct, String, Union with length field of
  /// static size(configured in data definition).
  ComplexDataTypeFromDataDefinition = 0x04U,
  /// Complex Data Type: Array, Struct,
  /// String, Union with length field size 1
  /// byte (ignore static definition).
  ComplexDataType8BitLengthField = 0x05U,
  /// Complex Data Type: Array, Struct,
  /// String, Union with length field size 2
  /// byte (ignore static definition).
  ComplexDataType16BitLengthField = 0x06U,
  /// Complex Data Type: Array, Struct,
  /// String, Union with length field size 4
  /// byte (ignore static definition).
  ComplexDataType32BitLengthField = 0x07U,
};

/**
 * \brief Class for the wire-type of TLV to transfer the wire-type from the stream
 * into an internal data model.
 *
 * \note This wrapper is used for deserialization / extracting the wire type from the
 * stream.
 * \uptrace PRS_SOMEIP_00206
 */
class WireTypeFromStream {
 public:
  /**
   * \brief The constructor transfers the wire-type from the stream into
   * an internal data model.
   *
   * \param data An array to the beginning of the stream. Can be const, because
   * we do a read-only on the stream.
   */
  explicit WireTypeFromStream(const std::uint8_t data) : value_{WireTypeEnum::FundamentalType8Bit} {
    const std::uint8_t wire_field = static_cast<std::uint8_t>((data & 0x70U) >> 4U);
    value_ = static_cast<WireTypeEnum>(wire_field);
  }

  /**
   * \brief Default constructor for the case, that Wire Type is not directly set
   * during construction.
   */
  WireTypeFromStream() : value_{WireTypeEnum::FundamentalType8Bit} {}

  /// Holds the current wire-type extracted from the packet.
  WireTypeEnum value_;
};

/// Type-alias for the Data ID for TLV. Actually, the Data ID has a size of
/// 12 bits, thus fits into one uint16.
using DataIdType = std::uint16_t;

/**
 * \brief Transfer the data id for storage from the stream into the internal
 * data representation.
 *
 * \note This wrapper is used for deserialization and extraction of the Data ID
 * from the stream.
 */
class DataIdFromStream {
 public:
  /**
   * \brief The constructor extracts the DataId from the stream with TLV and
   * stores it as an attribute.
   *
   * \param tlv_tag The TLV tag from the serialized byte stream.
   */
  explicit DataIdFromStream(const std::array<std::uint8_t, 2U> tlv_tag) : value_{0U} {
    value_ = static_cast<DataIdType>(((tlv_tag[0] << 8U) & 0x0F00U) | (tlv_tag[1] & 0xFFU));
  }

  /// Holds the extracted DataId
  DataIdType value_;
};

}  // namespace tlv

/**
 * \brief This template class shall be used to create a Data Id that is configured
 * in the data definition for a data element using TLV.
 * This class shall be provide a readable interface to generate a DataId, instead
 * of passing a raw number to a class.
 *
 * Usage:
 *  DataId<5U>
 */
template <tlv::DataIdType DataIdValue>
struct DataId {
  /// The Data Id has only 12 bits, so a Data ID must be in the range of
  /// 0 .. 2^12 - 1 (4095)
  static constexpr std::size_t kDataIdValueMax{4095U};
  /// Check if the Data ID is valid at compile-time.
  static_assert(DataIdValue <= kDataIdValueMax, "The Data ID must be smaller or equal to 4095.");
  /// Store the unique Data Id other objects can access.
  static constexpr tlv::DataIdType value{DataIdValue};
};

/**
 * \brief The PolicyBase template class is used to determine some things
 * at compile-time depending on the data definition from the model.
 *
 * \tparam ConcretePolicy This is the concrete policy generated from the
 * model and the data definition.
 */
template <typename ConcretePolicy>
struct PolicyBase {
  /**
   * \brief Determine the length field size from the concrete policy at compile-time.
   */
  constexpr static std::size_t LengthFieldSize{
      LengthField<typename ConcretePolicy::LengthFieldType>::GetLengthFieldSize()};

  /**
   * \brief Based on the LengthFieldSize attribute, it's possible to determine if
   * there is a length field active or not.
   */
  constexpr static bool LengthFieldActive{LengthFieldSize > 0U};

  /**
   * To stay compatible with the current implementation, a struct is automatically filled
   * based on the template-parameters given.
   */
  struct Policy : public DefaultPolicy {
    static constexpr ByteOrder kByteOrder =
        ConcretePolicy::PayloadEndianness::value;  ///< Endianness of the payload determined by the template config
    static constexpr std::size_t kLengthFieldSize =
        LengthFieldSize;  ///< Length of the length field determined based on the config
  };
};

/**
 * \brief Small helper template for storing the configuration within a type
 * and calculating the length field in the base class.
 *
 * \tparam PayloadEndiannessCfg The payload endianness for a specific member of
 * the stream. Is configured via the serialization properties in the model
 * and generated as C++ code accordingly.
 * \tparam LengthFieldTypeCfg Default length field from the data definition.
 *
 * \note MachineEndianess could be eliminated in the near future.
 */
template <typename PayloadEndiannessCfg, typename LengthFieldTypeCfg = std::uint32_t>
struct PolicyHelper : public PolicyBase<PolicyHelper<PayloadEndiannessCfg, LengthFieldTypeCfg>> {
  /// Configures the endianness of the payload. Could be configured for every
  /// member individually.
  using PayloadEndianness = PayloadEndiannessCfg;

  /// Store the length field configuration from the data definition.
  using LengthFieldType = LengthFieldTypeCfg;
};

/// Payload is in little-endian
/// No length-field is sent out
using LEPayloadNoLengthFieldPolicy = PolicyHelper<LittleEndian, void>;

/// Payload is little-endian
/// Length-field consists of 8 bits and stands in the front of the payload.
using LEPayloadUint8LengthFieldPolicy = PolicyHelper<LittleEndian, std::uint8_t>;

/// Payload is little-endian
/// Length-field consists of 16 bits and stands in the front of the payload.
using LEPayloadUint16LengthFieldPolicy = PolicyHelper<LittleEndian, std::uint16_t>;

/// Payload is little-endian
/// Length-field consists of 32 bits and stands in the front of the payload.
using LEPayloadUint32LengthFieldPolicy = PolicyHelper<LittleEndian, std::uint32_t>;

/// Payload is big-endian / network-byte-order
/// No length-field in payload
using BEPayloadNoLengthFieldPolicy = PolicyHelper<BigEndian, void>;

/// Payload is big-endian / network-byte-order
/// Length-field consists of 8 bits and stands in the front of the payload.
using BEPayloadUint8LengthFieldPolicy = PolicyHelper<BigEndian, std::uint8_t>;

/// Payload is big-endian / network-byte-order
/// Length-field consists of 16 bits and stands in the front of the payload.
using BEPayloadUint16LengthFieldPolicy = PolicyHelper<BigEndian, std::uint16_t>;

/// Payload is big-endian / network-byte-order
/// Length-field consists of 32 bits and stands in the front of the payload.
using BEPayloadUint32LengthFieldPolicy = PolicyHelper<BigEndian, std::uint32_t>;

/// TODO(visdtr) Shift this to the config / policy template to make this configurable.
/// The packet buffer could be of any container type, but out of compatibility with the current implementation and
/// for a rapid prototype this shall be a vector.
using PacketBuffer = std::vector<std::uint8_t>;

/**
 * \brief The idea is here to have one root serializer, which is the owner of the serialized
 * byte stream (buffer) and may have multiple nested serializers that may also write their data into.
 * The advantage of having this root serializer is that we're in full control
 * over the maximum size of the buffer (also exchanging it with a static sized array is possible). In addition,
 * the pointer arithmetics etc. is hidden as best as possible.
 *
 * \tparam Config This template parameter contains information about the default length field size from the data
 * definition
 * the byte ordering etc. which is known to the model.
 */
template <typename Config>
class RootSerializer {
 public:
  /**
   * \brief The constructor of a root serializer creates a new packet himself and all
   * nested serializers can push data into by calling PushBack.
   */
  RootSerializer() : buffer_{new PacketBuffer} {}

  /**
   * \brief The constructor of a root serializer gets an already allocated buffer from outside.
   *
   * \param buffer An allocated buffer.
   */
  explicit RootSerializer(std::unique_ptr<PacketBuffer> buffer) : buffer_{std::move(buffer)}, size_{buffer_->size()} {}

  /**
   * \brief Push data to the end of the stream.
   * Specialize for all fundamental types to have full control, which is going to be serialized
   * into the byte stream and in which correct size.
   * With the following approach, we avoid any
   * sizeof(). It's also possible to do a static_assert in the private method Serialize,
   * which is only called from one specialization of PushBack.
   *
   * One advantage is the error of the linker, if one specialization is missing (e.g. bool is missing):
   * undefined reference to `void someip_posix_common::someip::serialization::RootSerializer<...>::PushBack<bool>(bool)'
   *
   * It is also straightforward to provide additional template method specializations for
   * strings or use other serialization methods then.
   *
   * \tparam NestedConfig Specifies the policy to apply for the data to push back into the buffer.
   * \tparam DataType Deduced data type.
   * \param data The value to serialize and push into the stream.
   *
   */
  template <typename NestedConfig = Config, typename DataType>
  void PushBack(DataType data);

  /**
   * \brief Template method specialization for a bool.
   *
   * \tparam NestedConfig Specifies the policy to apply for the data to push back into the buffer.
   * \param data The value to serialize and push into the stream.
   */
  template <typename NestedConfig = Config>
  void PushBack(bool data) {
    Serialize<NestedConfig>(data);
    size_ += 1U;  // add manually without using sizeof to maximize portability.
  }

  /**
   * \brief Template method specialization for a uint8.
   *
   * \tparam NestedConfig Specifies the policy to apply for the data to push back into the buffer.
   * \param data The value to serialize and push into the stream.
   */
  template <typename NestedConfig = Config>
  void PushBack(std::uint8_t data) {
    Serialize<NestedConfig>(data);
    size_ += 1U;  // add manually without using sizeof to maximize portability.
  }

  /**
   * \brief Template method specialization for a sint8.
   *
   * \tparam NestedConfig Specifies the policy to apply for the data to push back into the buffer.
   * \param data The value to serialize and push into the stream.
   */
  template <typename NestedConfig = Config>
  void PushBack(std::int8_t data) {
    Serialize<NestedConfig>(data);
    size_ += 1U;  // add manually without using sizeof to maximize portability.
  }

  /**
   * \brief Template method specialization for a sint16.
   *
   * \tparam NestedConfig Specifies the policy to apply for the data to push back into the buffer.
   * \param data The value to serialize and push into the stream.
   */
  template <typename NestedConfig = Config>
  void PushBack(std::int16_t data) {
    Serialize<NestedConfig>(data);
    size_ += 2U;  // add manually without using sizeof to maximize portability.
  }

  /**
   * \brief Template method specialization for a uint16.
   *
   * \tparam NestedConfig Specifies the policy to apply for the data to push back into the buffer.
   * \param data The value to serialize and push into the stream.
   */
  template <typename NestedConfig = Config>
  void PushBack(std::uint16_t data) {
    Serialize<NestedConfig>(data);
    size_ += 2U;  // add manually without using sizeof to maximize portability.
  }

  /**
   * \brief Template method specialization for a sint32.
   *
   * \tparam NestedConfig Specifies the policy to apply for the data to push back into the buffer.
   * \param data The value to serialize and push into the stream.
   */
  template <typename NestedConfig = Config>
  void PushBack(std::int32_t data) {
    Serialize<NestedConfig>(data);
    size_ += 4U;  // add manually without using sizeof to maximize portability.
  }

  /**
   * \brief Template method specialization for a uint32.
   *
   * \tparam NestedConfig Specifies the policy to apply for the data to push back into the buffer.
   * \param data The value to serialize and push into the stream.
   */
  template <typename NestedConfig = Config>
  void PushBack(std::uint32_t data) {
    Serialize<NestedConfig>(data);
    size_ += 4U;  // add manually without using sizeof to maximize portability.
  }

  /**
   * \brief Template method specialization for a sint64.
   *
   * \tparam NestedConfig Specifies the policy to apply for the data to push back into the buffer.
   * \param data The value to serialize and push into the stream.
   */
  template <typename NestedConfig = Config>
  void PushBack(std::int64_t data) {
    Serialize<NestedConfig>(data);
    size_ += 8U;  // add manually without using sizeof to maximize portability.
  }

  /**
   * \brief Template method specialization for a uint64.
   *
   * \tparam NestedConfig Specifies the policy to apply for the data to push back into the buffer.
   * \param data The value to serialize and push into the stream.
   */
  template <typename NestedConfig = Config>
  void PushBack(std::uint64_t data) {
    Serialize<NestedConfig>(data);
    size_ += 8U;  // add manually without using sizeof to maximize portability.
  }

  /**
   * \brief Template method specialization for a floating point value.
   *
   * \tparam NestedConfig Specifies the policy to apply for the data to push back into the buffer.
   * \param data The value to serialize and push into the stream.
   */
  template <typename NestedConfig = Config>
  void PushBack(float data) {
    Serialize<NestedConfig>(data);
    size_ += 4U;  // add manually without using sizeof to maximize portability.
  }

  /**
   * \brief Template method specialization for a floating point value with double precision.
   *
   * \tparam NestedConfig Specifies the policy to apply for the data to push back into the buffer.
   * \param data The value to serialize and push into the stream.
   */
  template <typename NestedConfig = Config>
  void PushBack(double data) {
    Serialize<NestedConfig>(data);
    size_ += 8U;  // add manually without using sizeof to maximize portability.
  }

  /**
   * \brief Extend the byte stream with amount of bytes. New bytes will be filled with value zero.
   * \param bytes Amount of bytes to allocate in the byte stream.
   */
  template <typename NestedConfig = Config>
  void Extend(const std::size_t bytes) {
    size_ += bytes;
    buffer_->resize(size_, 0U);
  }

  /**
   * \brief Push data into the stream at a specific position, which is already
   * "allocated" or within the range.
   *
   * \tparam NestedConfig Specifies the policy to apply for this data.
   * \tparam DataType Deduced data type
   * \param data Data to serialize and write to the specific position.
   * \param abs_pos The beginning position to write data to.
   */
  template <typename NestedConfig = Config, typename DataType>
  void Push(DataType data, std::size_t abs_pos);

  /**
   * \brief Specialized method to push a uint8 into the serialized byte stream
   * at a specific position.
   */
  template <typename NestedConfig = Config>
  void Push(std::uint8_t data, std::size_t abs_pos) {
    const std::size_t current_size = size_;
    assert(abs_pos <= current_size - 1U);
    Serialize<NestedConfig>(data, abs_pos);
  }

  /**
   * \brief Specialized method to push a sint8 into the serialized byte stream
   * at a specific position.
   */
  template <typename NestedConfig = Config>
  void Push(std::int8_t data, std::size_t abs_pos) {
    const std::size_t current_size = size_;
    assert(abs_pos <= current_size - 1U);
    Serialize<NestedConfig>(data, abs_pos);
  }

  /**
   * \brief Specialized method to push a uint16 into the serialized byte stream
   * at a specific position.
   */
  template <typename NestedConfig = Config>
  void Push(std::uint16_t data, std::size_t abs_pos) {
    const std::size_t current_size = size_;
    assert(abs_pos <= current_size - 2U);
    Serialize<NestedConfig>(data, abs_pos);
  }

  /**
   * \brief Specialized method to push a sint16 into the serialized byte stream
   * at a specific position.
   */
  template <typename NestedConfig = Config>
  void Push(std::int16_t data, std::size_t abs_pos) {
    const std::size_t current_size = size_;
    assert(abs_pos <= current_size - 2U);
    Serialize<NestedConfig>(data, abs_pos);
  }

  /**
   * \brief Specialized method to push a int32 into the serialized byte stream
   * at a specific position.
   */
  template <typename NestedConfig = Config>
  void Push(std::int32_t data, std::size_t abs_pos) {
    const std::size_t current_size = size_;
    assert(abs_pos <= current_size - 4U);
    Serialize<NestedConfig>(data, abs_pos);
  }

  /**
   * \brief Specialized method to push a uint32 into the serialized byte stream
   * at a specific position.
   */
  template <typename NestedConfig = Config>
  void Push(std::uint32_t data, std::size_t abs_pos) {
    const std::size_t current_size = size_;
    assert(abs_pos <= current_size - 4U);
    Serialize<NestedConfig>(data, abs_pos);
  }

  /**
   * \brief Specialized method to push a uint64 into the serialized byte stream
   * at a specific position.
   */
  template <typename NestedConfig = Config>
  void Push(std::uint64_t data, std::size_t abs_pos) {
    const std::size_t current_size = size_;
    assert(abs_pos <= current_size - 8U);
    Serialize<NestedConfig>(data, abs_pos);
  }

  /**
   * \brief Specialized method to push a sint64 into the serialized byte stream
   * at a specific position.
   */
  template <typename NestedConfig = Config>
  void Push(std::int64_t data, std::size_t abs_pos) {
    const std::size_t current_size = size_;
    assert(abs_pos <= current_size - 8U);
    Serialize<NestedConfig>(data, abs_pos);
  }

  /**
   * \brief Specialized method to push a float into the serialized byte stream
   * at a specific position.
   */
  template <typename NestedConfig = Config>
  void Push(float data, std::size_t abs_pos) {
    const std::size_t current_size = size_;
    assert(abs_pos <= current_size - 4U);
    Serialize<NestedConfig>(data, abs_pos);
  }

  /**
   * \brief Specialized method to push a double into the serialized byte stream
   * at a specific position.
   */
  template <typename NestedConfig = Config>
  void Push(double data, std::size_t abs_pos) {
    const std::size_t current_size = size_;
    assert(abs_pos <= current_size - 8U);
    Serialize<NestedConfig>(data, abs_pos);
  }

  /**
   * \brief Get the current length of the serialized byte stream. This is always
   * incremented by a PushBack call.
   *
   * \return The current length in bytes of the byte stream.
   */
  std::size_t GetLength() const noexcept { return size_; }

  /**
   * \brief Get reference to the managed packet buffer.
   * \return Reference to the manager packet buffer.
   */
  PacketBuffer& GetBuffer() { return *buffer_; }

  /**
   * \brief Check if this serializer is already closed.
   *
   * \return true if Close() was called, otherwise false.
   */
  bool IsClosed() const noexcept { return closed_; }

  /**
   * \brief When the serialization is finished, this method will be called, which *moves* the ownership of
   * the buffer back to the user for transmission.
   *
   * \return The complete serialized buffer.
   */
  std::unique_ptr<PacketBuffer> Close() {
    closed_ = true;
    return std::move(buffer_);
  }

 private:
  /**
   * \brief This method can be called for all fundamental types.
   * If someone wants to use the current serializer for fundamental types and
   * strings only, but not for std::arrays or std::vector and other complex data
   * types he has full control and only has to create specialization for std::string.
   *
   * \tparam NestedConfig Contains the policy to apply for the serialization.
   * \tparam DataType Deduced data type.
   * \param data The value to push back.
   */
  template <typename NestedConfig = Config, typename DataType>
  void Serialize(DataType data) {
    static_assert(std::is_fundamental<DataType>::value,
                  "Must be a fundamental type or provide a serialization method for your specific complex data type.");
    someip_posix_common::someip::serialization::read<typename NestedConfig::Policy>(
        someip_posix_common::someip::serialization::make_serialize_iterator(*buffer_), data);
  }

  /**
   * \brief This method is called on pushing and serializing data to a specific position in the buffer.
   *
   * \tparam NestedConfig Specifies the policy to apply for serialization.
   * \tparam Deduced data type of parameter data.
   * \param data Value to push into the buffer.
   * \param abs_pos Absolute position in the buffer to push data to.
   */
  template <typename NestedConfig = Config, typename DataType>
  void Serialize(DataType data, std::size_t abs_pos) {
    static_assert(std::is_fundamental<DataType>::value,
                  "Must be a fundamental type or provide a serialization method for your specific complex data type.");
    someip_posix_common::someip::serialization::read<typename NestedConfig::Policy>(buffer_->begin() + abs_pos, data);
    // DO NOT increment size_ attribute here, because we push to an existing
    // position in the byte stream.
  }

  /// The root always has access.
  std::unique_ptr<PacketBuffer> buffer_;

  /// Overall size of the serialized data.
  std::size_t size_{};

  /// Flag if this serializer is closed to make sure, that a nested serializer
  /// is not closed before its root.
  bool closed_{false};
};

/**
 * \brief The idea is that the nested serializer does not contain any info about the buffer "management" etc., but acts
 * more like a forwarder for the root serializer.
 * \tparam Config This template parameter contains information about the default length field size from the data
 * definition
 * the byte ordering etc. which is known to the model.
 * \tparam Root The type of the root serializer to get a pointer to it, for accessing the methods PushBack, Push and
 * GetLength..
 */
template <typename Config, typename Root>
class NestedSerializer {
 public:
  /**
   * This constructor sets the root pointer to be able to access the methods from the root serializer.
   *
   * \param root A pointer to the RootSerializer instance.
   */
  explicit NestedSerializer(Root* root) : root_{root}, closed_{false} {}

  /**
   * \brief Extend the byte stream with amount of bytes. New bytes will be filled with value zero.
   * \param bytes Amount of bytes to allocate in the byte stream.
   */
  template <typename NestedConfig = Config>
  void Extend(const std::size_t bytes) {
    root_->template Extend(bytes);
  }

  /**
   * \brief Use the PushBack from the RootSerializer template class.
   *
   * \tparam NestedConfig Policy to apply for the serialization.
   * \tparam DataType Deduced data type.
   * \param data The data to push pack into the byte stream.
   */
  template <typename NestedConfig = Config, typename DataType>
  void PushBack(DataType data) {
    root_->template PushBack<NestedConfig>(data);
  }

  /**
   * \brief Uses the Push method from the RootSerializer template class.
   *
   * \tparam NestedConfig Policy to apply for the serialization.
   * \tparam DataType Deduced data type.
   * \param data The data value to serialize and put into the buffer.
   * \param abs_pos The absolute position to put the serialized data to.
   */
  template <typename NestedConfig = Config, typename DataType>
  void Push(DataType data, std::size_t abs_pos) {
    root_->template Push<NestedConfig>(data, abs_pos);
  }

  /**
   * \brief Uses the GetLength method from the RootSerializer template class.
   *
   * \return the current length of the byte stream of the RootSerializer.
   */
  std::size_t GetLength() const noexcept { return root_->GetLength(); }

  /**
   * \brief Get reference to the managed packet buffer.
   * \return Reference to the manager packet buffer.
   */
  PacketBuffer& GetBuffer() { return root_->GetBuffer(); }

  /**
   * \brief Check if this serializer is already closed.
   * This is normally used from a nested serializer to check if the root is already closed.
   * If the root is closed before the nested one, an error should occur, to prevent from
   * faults.
   */
  bool IsClosed() const noexcept { return closed_; }

  /**
   * \brief For nested serializers this is empty, otherwise it's overridden in
   * the specialized serializer.
   */
  void Close() noexcept {
    assert(!root_->IsClosed());
    closed_ = true;
  }

 private:
  /// A pointer to the root serializer.
  Root* root_;

  /// Flag if this serializer is closed to make sure, that a nested serializer is not closed before its root.
  bool closed_;
};

/**
 * \brief Template for a nested serializer, where the root is explicitly given.
 *
 * \tparam Config Length field, endianness config from the data definition.
 * \tparam Root This will default to void, if not given explicitly, which means
 * that this is the root serializer. If a type for Root is given, then it assumes,
 * that this is a child serializer of a root.
 */
template <typename Config, typename Root = void>
class Serializer : public NestedSerializer<Config, Root> {
 public:
  /// Type-alias for the base template class.
  using Base = NestedSerializer<Config, Root>;

  /**
   * \brief Constructor to init a nested serializer.
   * This constructor sets a pointer to its root serializer to
   * be able to push data into the byte stream of the root.
   */
  explicit Serializer(Root* root) : Base{root} {}
};

/**
 * \brief Template specialization for a RootSerializer, where no root has to
 * be given. (template parameter Root = void).
 */
template <typename Config>
class Serializer<Config, void> : public RootSerializer<Config> {
 public:
  /// Type-alias for the base template class.
  using Base = RootSerializer<Config>;

  /**
   * \brief Constructor to init a RootSerializer.
   */
  Serializer() : Base() {}

  /**
   * \brief Constructor to init a root serializer with a buffer given from outside.
   */
  explicit Serializer(std::unique_ptr<PacketBuffer> buffer) : Base(std::move(buffer)) {}
};

/**
 * \brief Concrete serializer for complex data types with a possible length field.
 * Structs, arrays can have an optional length field according to AUTOSAR.
 * This template class acts as a helper for this case, which reserves and sets
 * the length field according to the length field size, that is configurable in the
 * data definition.
 */
template <typename Config, typename Root = void>
class ComplexDataTypeSerializer : public Serializer<Config, Root> {
 public:
  /// Type-alias for the base template class of this specialized serializer.
  using Base = Serializer<Config, Root>;

  /**
   * \brief The constructor of ComplexDataTypeSerializer will serialize the wire type, data id.
   *
   * \param root Could be a nested serializer; defaults to nullptr which stands for root.
   */
  explicit ComplexDataTypeSerializer(Root* root) : Base{root} { ReserveLengthField(); }

  /**
   * \brief Close if this for the nested serializer.
   */
  void Close() { CloseHandler(); }

 private:
  /**
   * \brief Reserves a possible length field of the configured size from the data definition.
   *
   * Example:
   * If the data definition says the length field is 8 bits, then the stream will be extended for
   * 8 bits.
   */
  void ReserveLengthField() {
    length_pos_ = Base::GetLength();  // Store the length position for later insertion of the length value.
    // We need this conditional here, if there is no length type set (type void) in the data definition.
    // The conditional makes sure, that the source is compile-able, when no length field is set.
    using T = typename std::conditional<std::is_same<typename Config::LengthFieldType, void>::value, int,
                                        typename Config::LengthFieldType>::type;
    if (Config::LengthFieldActive) {
      T field{};
      Base::PushBack(field);
    }
  }

  /**
   * \brief Is called whenever Close method is called from outside. This handler does serializer-specific logic.
   */
  void CloseHandler() {
    std::size_t len{Base::GetLength() - length_pos_};

    if (Config::LengthFieldActive) {
      // This conditional is used to avoid a compile-error, when the length field in the data definition is zero
      // (which is modeled as void here).
      // If the length field is configured to zero (void), we default to int with the conditional, but do not
      // enter this if-condition anyway, because Config::LengthFieldActive is false.
      using T = typename std::conditional<std::is_same<typename Config::LengthFieldType, void>::value, int,
                                          typename Config::LengthFieldType>::type;
      const T field{static_cast<T>(len - Config::LengthFieldSize)};
      assert((len - Config::LengthFieldSize) <= std::numeric_limits<T>::max());
      Base::Push(field, length_pos_);
    }
  }

  /// Store the position of the length field.
  std::size_t length_pos_;
};

/**
 * \brief Partial template specialization for ComplexDataTypeSerializer to declare
 * a RootSerializer.
 */
template <typename Config>
class ComplexDataTypeSerializer<Config, void> : public Serializer<Config, void> {
 public:
  /// Type-alias for the base template class of this specialized serializer.
  using Base = Serializer<Config, void>;

  /**
   * \brief The constructor of ComplexDataTypeSerializer will serialize the wire type, data id.
   */
  ComplexDataTypeSerializer() : Base() { ReserveLengthField(); }

  /**
   * \brief The constructor of ComplexDataTypeSerializer will serialize the wire type, data id.
   */
  explicit ComplexDataTypeSerializer(std::unique_ptr<PacketBuffer> buffer) : Base{std::move(buffer)} {
    ReserveLengthField();
  }

  /**
   * \brief Close for the RootSerializer.
   */
  std::unique_ptr<PacketBuffer> Close() {
    CloseHandler();
    return std::move(Base::Close());
  }

 private:
  /**
   * \brief Reserves a possible length field of the configured size from the data definition.
   *
   * Example:
   * If the data definition says the length field is 8 bits, then the stream will be extended for
   * 8 bits.
   */
  void ReserveLengthField() {
    length_pos_ = Base::GetLength();  // Store the length position for later insertion of the length value.
    // We need this conditional here, if there is no length type set (type void) in the data definition.
    // The conditional makes sure, that the source is compile-able, when no length field is set.
    using T = typename std::conditional<std::is_same<typename Config::LengthFieldType, void>::value, int,
                                        typename Config::LengthFieldType>::type;
    if (Config::LengthFieldActive) {
      T field{};
      Base::template PushBack(field);
    }
  }

  /**
   * \brief Is called whenever Close method is called from outside. This handler does serializer-specific logic.
   */
  void CloseHandler() {
    std::size_t len{Base::GetLength() - length_pos_};

    if (Config::LengthFieldActive) {
      // This conditional is used to avoid a compile-error, when the length field in the data definition is zero
      // (which is modeled as void here).
      // If the length field is configured to zero (void), we default to int with the conditional, but do not
      // enter this if-condition anyway, because Config::LengthFieldActive is false.
      using T = typename std::conditional<std::is_same<typename Config::LengthFieldType, void>::value, int,
                                          typename Config::LengthFieldType>::type;
      const T field{static_cast<T>(len - Config::LengthFieldSize)};
      assert((len - Config::LengthFieldSize) <= std::numeric_limits<T>::max());
      Base::Push(field, length_pos_);
    }
  }

  /// Store the position of the length field.
  std::size_t length_pos_;
};

/**
 * \brief Concrete serializer for the SOME/IP header.
 */
template <typename Config, typename Root = void>
class SomeIpHeaderSerializer : public Serializer<Config, Root> {
 public:
  /// Type-alias for the base template class of this specialized serializer.
  using Base = Serializer<Config, Root>;

  /**
   * \brief The constructor of SomeIpHeaderSerializer will serialize
   * the header info given immediately.
   *
   * \param root Could be a nested serializer; defaults to nullptr which stands for root.
   * \param header The SOME/IP header information to serialize.
   */
  explicit SomeIpHeaderSerializer(Root* root, const ::someip_posix_common::someip::SomeIpMessageHeader& header)
      : Base{root}, header_begin_pos_{Base::GetLength()} {
    WriteHeaderField(header);
  }

  /**
   * \brief Close if this for the nested serializer.
   */
  void Close() { CloseHandler(); }

 private:
  /**
   * \brief Will push back the given data into the buffer to reserve the size of a SOME/IP header.
   *
   * \param header The SOME/IP header information to serialize and push into the buffer.
   */
  void WriteHeaderField(const ::someip_posix_common::someip::SomeIpMessageHeader& header) {
    Base::template PushBack<BEPayloadNoLengthFieldPolicy>(header.service_id_);
    Base::template PushBack<BEPayloadNoLengthFieldPolicy>(header.method_id_);

    // Store the position of the length field right before the length field is pushed into the buffer.
    // Getting the position here is used for insertion of the length value after all the payload data has been
    // serialized and pushed into the buffer.
    length_pos_ = Base::GetLength();

    Base::template PushBack<BEPayloadNoLengthFieldPolicy>(header.length_);
    Base::template PushBack<BEPayloadNoLengthFieldPolicy>(header.client_id_);
    Base::template PushBack<BEPayloadNoLengthFieldPolicy>(header.session_id_);
    Base::template PushBack<BEPayloadNoLengthFieldPolicy>(header.protocol_version_);
    Base::template PushBack<BEPayloadNoLengthFieldPolicy>(header.interface_version_);
    Base::template PushBack<BEPayloadNoLengthFieldPolicy>(header.message_type_);
    Base::template PushBack<BEPayloadNoLengthFieldPolicy>(header.return_code_);
  }

  /**
   * \brief Is called whenever Close method is called from outside.
   * This handler sets the value of the length field in the SOME/IP header.
   */
  void CloseHandler() {
    const std::size_t len{Base::GetLength() - ::someip_posix_common::someip::kHeaderLength - header_begin_pos_};
    assert(len < std::numeric_limits<std::uint32_t>::max());
    const std::uint32_t temp = static_cast<std::uint32_t>(len);
    // Overwrite the policy here, because SOME/IP header is always in network-byte-order.
    Base::template Push<BEPayloadNoLengthFieldPolicy>(temp, length_pos_);
  }

  /// Store the position of the header field.
  const std::size_t header_begin_pos_;

  /// Store the position of the length field.
  std::size_t length_pos_;
};

/**
 * \brief Partial template specialization for ComplexDataTypeSerializer to declare
 * a RootSerializer.
 */
template <typename Config>
class SomeIpHeaderSerializer<Config, void> : public Serializer<Config, void> {
 public:
  /// Type-alias for the base template class of this specialized serializer.
  using Base = Serializer<Config, void>;

  /**
   * \brief The constructor of SomeIpHeaderSerializer will push the SOME/IP header and serialize it in
   * Big-Endian format.
   *
   * \param header The SOME/IP header to serialize and push into the buffer.
   */
  explicit SomeIpHeaderSerializer(const ::someip_posix_common::someip::SomeIpMessageHeader& header)
      : Base(), header_begin_pos_{Base::GetLength()} {
    WriteHeaderField(header);
  }

  /**
   * \brief The constructor of SomeIpHeaderSerializer will push the SOME/IP header and serialize it in
   * Big-Endian format.
   *
   * \param buffer The buffer to move into this root serializer.
   * \param header The SOME/IP header data to serialize and push into the buffer.
   */
  explicit SomeIpHeaderSerializer(std::unique_ptr<PacketBuffer> buffer,
                                  const ::someip_posix_common::someip::SomeIpMessageHeader& header)
      : Base{std::move(buffer)}, header_begin_pos_{Base::GetLength()} {
    WriteHeaderField(header);
  }

  /**
   * \brief Close for the RootSerializer.
   *
   * \return The buffer and the ownership of it.
   */
  std::unique_ptr<PacketBuffer> Close() {
    CloseHandler();
    return std::move(Base::Close());
  }

 private:
  /**
   * \brief Will push back the given data into the buffer to reserve the size of a SOME/IP header.
   *
   * \param header The SOME/IP header information to serialize and push into the buffer.
   */
  void WriteHeaderField(const ::someip_posix_common::someip::SomeIpMessageHeader& header) {
    Base::template PushBack<BEPayloadNoLengthFieldPolicy>(header.service_id_);
    Base::template PushBack<BEPayloadNoLengthFieldPolicy>(header.method_id_);

    // Store the position of the length field right before the length field is pushed into the buffer.
    // Getting the position here is used for insertion of the length value after all the payload data has been
    // serialized and pushed into the buffer.
    length_pos_ = Base::GetLength();

    Base::template PushBack<BEPayloadNoLengthFieldPolicy>(header.length_);
    Base::template PushBack<BEPayloadNoLengthFieldPolicy>(header.client_id_);
    Base::template PushBack<BEPayloadNoLengthFieldPolicy>(header.session_id_);
    Base::template PushBack<BEPayloadNoLengthFieldPolicy>(header.protocol_version_);
    Base::template PushBack<BEPayloadNoLengthFieldPolicy>(header.interface_version_);
    Base::template PushBack<BEPayloadNoLengthFieldPolicy>(header.message_type_);
    Base::template PushBack<BEPayloadNoLengthFieldPolicy>(header.return_code_);
  }

  /**
   * \brief Is called whenever Close method is called from outside.
   * This handler sets the value of the length field in the SOME/IP header.
   */
  void CloseHandler() {
    const std::uint32_t len{Base::GetLength() - ::someip_posix_common::someip::kHeaderLength - header_begin_pos_};
    // Overwrite the policy here, because SOME/IP header is always in network-byte-order.
    Base::template Push<BEPayloadNoLengthFieldPolicy>(len, length_pos_);
  }

  /// Store the position of the header field.
  const std::size_t header_begin_pos_;

  /// Store the position of the length field.
  std::size_t length_pos_;
};

/**
 * \brief Special variant of the serializer for TLV.
 * \tparam Config Data definition configuration, which consists of endianness, default length field settings.
 * \tparam DataId The Data Id from the configuration, which is statically known.
 * \tparam Root This deserializer specialization may have or may have not a root
 * serializer. If this is void, the TlvSerializer itself is the root, which is determined by
 * the template base class Serializer<Config, Parent>.
 */
template <typename Config, typename DataId, typename Root = void>
class TlvSerializer : public Serializer<Config, Root> {
 public:
  /// Type-alias for the base template class of this specialized serializer.
  using Base = Serializer<Config, Root>;

  /**
   * \brief The constructor of TlvSerializer will serialize the wire type, data id.
   *
   * \param wire_type The wire type of the TLV packet.
   */
  template <typename T = Root, typename std::enable_if<std::is_same<T, void>::value, int>::type = 0>
  explicit TlvSerializer(tlv::WireTypeEnum wire_type) : Base(), wire_type_{wire_type} {
    InitTag(wire_type);
  }

  /**
   * \brief The constructor of TlvSerializer will serialize the wire type, data id.
   *
   * \param wire_type The wire type of the TLV packet.
   * \param root Could be a nested serializer; defaults to nullptr which stands for root.
   */
  template <typename T = Root, typename std::enable_if<!std::is_same<T, void>::value, int>::type = 0>
  TlvSerializer(tlv::WireTypeEnum wire_type, Root* root) : Base{root}, wire_type_{wire_type} {
    InitTag(wire_type);
  }

  /**
   * \brief Close method if this is the root serializer.
   */
  template <typename T = Root>
  typename std::enable_if<std::is_same<T, void>::value, std::unique_ptr<PacketBuffer>>::type Close() {
    CloseHandler();
    return std::move(Base::Close());
  }

  /**
   * \brief Close method if this is a nested serializer.
   */
  template <typename T = Root>
  typename std::enable_if<!std::is_same<T, void>::value, void>::type Close() {
    CloseHandler();
  }

 private:
  /**
   * \brief Initializes a tag in the front of a payload.
   */
  void InitTag(tlv::WireTypeEnum wire_type) noexcept {
    std::uint8_t write_byte0 = static_cast<std::uint8_t>((static_cast<std::uint8_t>(wire_type) & 0x07U) << 0x04U);
    write_byte0 |= static_cast<std::uint8_t>((DataId::value & 0x0F00U) >> 0x08U);
    const std::uint8_t write_byte1 = static_cast<std::uint8_t>(DataId::value & 0xFFU);

    // The following pushes are done manually - independent of any byte order, because the layout of TLV
    // stays always the same.
    Base::template PushBack(write_byte0);  // Wire Type + Data ID (4 bits): [6 .. 4 | 3 .. 0]
    Base::template PushBack(write_byte1);  // Data ID (8 bits)
    length_pos_ = Base::GetLength();       // Store the length position mark for later insertion of the length value.

    switch (wire_type_) {
      case tlv::WireTypeEnum::ComplexDataTypeFromDataDefinition: {
        // We need this conditional here, if there is no length type set (type void) in the data definition.
        // The conditional makes sure, that the source is compile-able, when no length field is set.
        using T = typename std::conditional<std::is_same<typename Config::LengthFieldType, void>::value, int,
                                            typename Config::LengthFieldType>::type;
        if (Config::LengthFieldActive) {
          T field{};
          Base::template PushBack(field);
        }
        break;
      }

      case tlv::WireTypeEnum::ComplexDataType8BitLengthField: {
        std::uint8_t field{};
        Base::template PushBack(field);
        break;
      }

      case tlv::WireTypeEnum::ComplexDataType16BitLengthField: {
        std::uint16_t field{};
        Base::template PushBack(field);
        break;
      }

      case tlv::WireTypeEnum::ComplexDataType32BitLengthField: {
        std::uint32_t field{};
        Base::template PushBack(field);
        break;
      }

      // For any other fundamental data type no length field shall be pushed back.
      default: { break; }
    }
  }

  /**
   * \brief The close handler is used to write the length field, when all
   * the data is pushed back.
   */
  void CloseHandler() {
    std::size_t len{Base::GetLength() - length_pos_};

    switch (wire_type_) {
      case tlv::WireTypeEnum::ComplexDataTypeFromDataDefinition: {
        if (Config::LengthFieldActive) {
          // This conditional is used to avoid a compile-error, when the length field in the data definition is zero
          // (which is modeled as void here).
          // If the length field is configured to zero (void), we default to int with the conditional, but do not
          // enter this if-condition anyway, because Config::LengthFieldActive is false.
          using T = typename std::conditional<std::is_same<typename Config::LengthFieldType, void>::value, int,
                                              typename Config::LengthFieldType>::type;
          const T field{static_cast<T>(len - Config::LengthFieldSize)};
          assert((len - Config::LengthFieldSize) <= std::numeric_limits<T>::max());
          Base::Push(field, length_pos_);
        }
        break;
      }

      case tlv::WireTypeEnum::ComplexDataType8BitLengthField: {
        const std::uint8_t field{static_cast<std::uint8_t>(len - 1U)};
        assert((len - 1U) <= std::numeric_limits<std::uint8_t>::max());
        Base::Push(field, length_pos_);
        break;
      }

      case tlv::WireTypeEnum::ComplexDataType16BitLengthField: {
        const std::uint16_t field{static_cast<std::uint16_t>(len - 2U)};
        assert((len - 2U) <= std::numeric_limits<std::uint16_t>::max());
        Base::Push(field, length_pos_);
        break;
      }

      case tlv::WireTypeEnum::ComplexDataType32BitLengthField: {
        const std::uint32_t field{static_cast<std::uint32_t>(len - 4U)};
        assert((len - 4U) <= std::numeric_limits<std::uint32_t>::max());
        Base::Push(field, length_pos_);
        break;
      }

      // For any other fundamental data type, no length field shall be calculated & pushed back.
      default: { break; }
    }
  }

  /// Store the position of the length field.
  std::size_t length_pos_{};

  /// Store the wire type.. on finishing, the length field is written based on
  /// the wire type value
  tlv::WireTypeEnum wire_type_;
};

/**
 * \brief The idea is here to have one root deserializer, that is owner of the serialized
 * byte stream (buffer) and may have multiple nested deserializers, that may access the byte stream.
 * But they can only access it through the root.
 * The advantage of having this root serializer is that we're in full control
 * over the maximum size of the buffer (also exchanging it with a static sized array is possible). In addition,
 * the pointer arithmetics etc. is hidden as best as possible.
 * Also, specialized deserializers do not implement their own versions of PopFront etc., but use the given API from
 * the root, which reduces complexity.
 *
 * \tparam Config This template parameter contains information about the default length field size from the data
 * definition the byte ordering etc. which is known to the model.
 */
template <typename Config>
class RootDeserializer {
 public:
  /**
   * \brief The constructor moves a buffer with serialized data to the object.
   *
   * \param buffer Contains the serialized data to deserialize.
   */
  explicit RootDeserializer(std::unique_ptr<PacketBuffer> buffer)
      : buffer_{std::move(buffer)}, pos_{buffer_->data()}, length_{buffer_->size()} {}

  /**
   * \brief Pop data from the current position.
   *
   * Usage:
   *
   * Deserializer<BEPayloadUint32LengthFieldPolicy<LittleEndian>> root{std::move(buffer)};
   * std::uint16_t a;
   * root.PopFront(a);
   *
   * \tparam DataType Deduced data type.
   * \tparam NestedConfig If it's a nested deserializer, a configuration is given.
   * This is mandatory, because the endianness configuration may differ per member.
   * \param data The data reference to write the value to.
   *
   * \return sizeof the data popped; if the size of the data type is greater than the
   * buffer, then it will return 0.
   */
  template <typename SelectedConfig = Config, typename DataType>
  std::size_t PopFront(DataType& data) {
    return Deserialize<SelectedConfig>(data);
  }

  /**
   * \brief Will return the remaining length of a serialized buffer.
   */
  std::size_t GetRemainingLength() const noexcept { return static_cast<std::size_t>(&(*buffer_->end()) - pos_); }

  /**
   * \brief A derived deserializer can retreive the current position in the byte stream.
   */
  std::uint8_t* GetPos() const noexcept { return pos_; }

  /**
   * \brief A derived deserializer can set the position in the byte stream.
   */
  void SetPos(std::uint8_t* pos) noexcept { pos_ = pos; }

  /**
   * \brief Consume amount of bytes without deserialization.
   * \param bytes Amount of bytes to consume
   */
  void Skip(const std::size_t bytes) {
    if ((bytes_read_ + bytes) <= length_) {
      bytes_read_ += bytes;
      pos_ += bytes;
    }
  }

  /**
   * \brief Get reference to the managed packet buffer.
   * \return Reference to the managed packet buffer.
   */
  PacketBuffer& GetBuffer() { return *buffer_; }

  /**
   * \brief Transfers ownership of the buffer from the deserializer to the caller.
   *
   * \return the moved packet buffer.
   */
  std::unique_ptr<PacketBuffer> ReleaseBuffer() { return std::move(buffer_); }

 private:
  template <typename SelectedConfig = Config, typename DataType>
  std::size_t Deserialize(DataType& data) {
    static_assert(std::is_fundamental<DataType>::value, "Must be a fundamental type or specialize the deserializer.");

    std::size_t nbytes{};

    if ((bytes_read_ + sizeof(DataType)) <= length_) {
      pos_ = someip_posix_common::someip::serialization::write<typename SelectedConfig::Policy>(data, pos_);
      bytes_read_ += sizeof(DataType);
      nbytes = sizeof(DataType);
    }

    return nbytes;
  }

  /**
   * \brief Partial specialization for bool, because the size of bool is implementation-specific.
   */
  template <typename SelectedConfig = Config>
  std::size_t Deserialize(bool& data) {
    std::size_t nbytes{};

    if ((bytes_read_ + 1U) <= length_) {
      pos_ = someip_posix_common::someip::serialization::write<typename SelectedConfig::Policy>(data, pos_);
      bytes_read_ += 1U;
      nbytes = 1U;
    }

    return nbytes;
  }

  /**
   * \brief Partial specialization for float, because float may be implementation-specific.
   *
   * \tparam SelectedConfig
   */
  template <typename SelectedConfig = Config>
  std::size_t Deserialize(float& data) {
    std::size_t nbytes{};

    if ((bytes_read_ + 4U) <= length_) {
      pos_ = someip_posix_common::someip::serialization::write<typename SelectedConfig::Policy>(data, pos_);
      bytes_read_ += 4U;
      nbytes = 4U;
    }

    return nbytes;
  }

  /**
   * \brief Partial specialization for double, because a float of double-precision may be implementation-specific.
   *
   * \tparam SelectedConfig
   */
  template <typename SelectedConfig = Config>
  std::size_t Deserialize(double& data) {
    std::size_t nbytes{};

    if ((bytes_read_ + 8U) <= length_) {
      pos_ = someip_posix_common::someip::serialization::write<typename SelectedConfig::Policy>(data, pos_);
      bytes_read_ += 8U;
      nbytes = 8U;
    }

    return nbytes;
  }

  /// The root is owner of the buffer.
  std::unique_ptr<PacketBuffer> buffer_;

  /// Current position for calling the write method to deserialize data from.
  std::uint8_t* pos_;

  /// Complete length of the serialized buffer.
  const std::size_t length_;

  /// Bytes that have been read with PopFront for this deserializer.
  std::size_t bytes_read_{};
};

/**
 * \brief The nested serializer itself has no logic, but uses the root deserializer
 * to extract data from the byte stream. This is useful to have no additional logic
 * here, because we always have one entry point in the root.
 *
 * \tparam Config The configured policy to use.
 * \tparam Root The type of the root deserializer.
 */
template <typename Config, typename Root>
class NestedDeserializer {
 public:
  /**
   * \brief The constructor initializes the pointer to the root deserializer.
   */
  explicit NestedDeserializer(Root* root) : root_{root} {}

  /**
   * \brief This method uses the PopFront method from the root.
   *
   * \tparam NestedConfig Will override the policy of the root on a PopFront.
   * \tparam DataType Deduced data type
   * \param data The object to write a value to.
   * \return The size in bytes which is read from the buffer of the root deserializer.
   */
  template <typename NestedConfig = Config, typename DataType>
  std::size_t PopFront(DataType& data) {
    // Make sure that the config of this nested deserializer is used instead of the config from
    // the root deserializer, by giving the Config as a template parameter to PopFront.
    return root_->template PopFront<NestedConfig>(data);
  }

  /**
   * \brief This method uses the GetPosition method from the root.
   */
  std::size_t GetPosition() noexcept { return root_->GetPosition(); };

  /**
   * \brief This method uses the GetRemainingLength method from the root.
   */
  std::size_t GetRemainingLength() const { return root_->GetRemainingLength(); }

  /**
   * \brief A derived deserializer can retreive the current position in the byte stream.
   */
  std::uint8_t* GetPos() const noexcept { return root_->GetPos(); }

  /**
   * \brief A derived deserializer can set the position in the byte stream.
   */
  void SetPos(std::uint8_t* pos) noexcept { root_->SetPos(pos); }

  /**
   * \brief Consume amount of bytes without deserialization.
   * \param bytes Amount of bytes to consume
   */
  void Skip(const std::size_t bytes) { root_->Skip(bytes); }

  /**
   * \brief Get reference to the managed packet buffer.
   * \return Reference to the manager packet buffer.
   */
  PacketBuffer& GetBuffer() { return root_->GetBuffer(); }

 private:
  /// Pointer to the root deserializer to use.
  Root* root_;
};

/**
 * \brief The deserializer template class acts as a gateway
 * for specialized deserializers.
 */
template <typename Config, typename Root = void>
class Deserializer : public NestedDeserializer<Config, Root> {
 public:
  /// Type-alias for the base template class Deserializer.
  using Base = NestedDeserializer<Config, Root>;

  /**
   * \brief Constructor to init a nested serializer. This constructor sets a pointer to its root serializer to
   * be able to push data into the byte stream of the root.
   */
  explicit Deserializer(Root* root) : Base{root} {}
};

/**
 * \brief Partial template specialization for the RootDeserializer.
 */
template <typename Config>
class Deserializer<Config, void> : public RootDeserializer<Config> {
 public:
  /// Type-alias for the base template class Deserializer.
  using Base = RootDeserializer<Config>;

  /**
   * \brief Constructor to initialize a root serializer.
   */
  explicit Deserializer(std::unique_ptr<PacketBuffer> buffer) : Base{std::move(buffer)} {}
};

/**
 * \brief This struct is used for the extraction of data id and data length
 * from a TLV conformant packet. These two informations must be provided to the user.
 */
struct TlvTagInfo {
  /// Extracted Data ID from the TLV packet
  tlv::DataIdType data_id_;

  /// Extracted length info.. If the length field is zero, because a fundamental type is given,
  /// or the no length field is configured in case of Wire Type = 4, this will default to zero.
  std::size_t data_length_;
};

/**
 * \brief The TLV packet template class is used to extract all relevant
 * informations based on the configuration from the model.
 *
 * \tparam Config Configuration based on the data definition from the model
 * that defines the LengthFieldType, the endianness etc.
 * \tparam Root Configurable root deserializer. This will default to void, which
 * means that this is the root.
 */
template <typename Config, typename Root = void>
class TlvDeserializer : public Deserializer<Config, Root> {
 public:
  /// Type-alias for the base class
  using Base = Deserializer<Config, Root>;

  /**
   * \brief This constructor will set up a root serializer.
   */
  template <typename T = Root, typename std::enable_if<std::is_same<T, void>::value, int>::type = 0>
  explicit TlvDeserializer(std::unique_ptr<PacketBuffer> buffer) : Base(std::move(buffer)) {}

  /**
   * \brief This constructor will set up a nested serializer.
   */
  template <typename T = Root, typename std::enable_if<!std::is_same<T, void>::value, int>::type = 0>
  explicit TlvDeserializer(Root* root) : Base{root} {}

  /**
   * \brief This is the entry-point whenever a new TLV packet shall be processed.
   * The following method does the following:
   * - Consume the wire type and store it as attribute.
   * - Consume the data id and give it back to the user for further processing.
   * - Consume a possible length field in dependence of the wire type given and
   * store it.
   *
   * After this method is called we stand exactly at the start of the member data (marked with ->):
   *
   * | Wire Type | Data Id | [Length Field] | -> Data Member Payload
   *
   * \return A structure of extracted data id and possible length, if a length
   * field is active for the wire type.
   */
  TlvTagInfo ConsumeNextTag() noexcept {
    std::array<std::uint8_t, 2U> tlv_tag{};
    const bool tag0_av = Base::PopFront(tlv_tag[0]) > 0U;  // Consume wire type and 3 bits of data id
    const bool tag1_av = Base::PopFront(tlv_tag[1]) > 0U;  // Consume the remaining bits of data id
    const tlv::WireTypeFromStream wire_type(tlv_tag[0]);
    wire_type_ = wire_type;
    tlv::DataIdFromStream data_id(tlv_tag);
    bytes_read_ = 0U;

    if (tag0_av && tag1_av) {
      current_len_ = ConsumeMemberDataLength();
    } else {
      current_len_ = 0U;
    }

    return {data_id.value_, current_len_};
  }

  /**
   * \brief The method PopFront is overridden in this specialized deserializer
   * to check for overflows. We can not read more data from the stream, than it is defined by the
   * length field.
   */
  template <typename NestedConfig = Config, typename DataType>
  std::size_t PopFront(DataType& data) {
    std::size_t nbytes{};
    if ((bytes_read_ + sizeof(DataType)) <= current_len_) {
      nbytes = Base::template PopFront<NestedConfig>(data);
      bytes_read_ += nbytes;
    }

    return nbytes;
  }

  /**
   * \brief Method specialization for extracting a bool from the byte stream.
   * \note sizeof(bool) depends on the implementation (normally one byte),
   * but shall be only one byte according to the AUTOSAR specification.
   *
   * \param data value to get from the buffer and to deserialize.
   * \return One byte if the value was popped out from the buffer successfully, 0
   * if not.
   */
  template <typename NestedConfig = Config>
  std::size_t PopFront(bool& data) {
    std::size_t nbytes{};
    if ((bytes_read_ + 1U) <= current_len_) {
      nbytes = Base::template PopFront<NestedConfig>(data);
      bytes_read_ += nbytes;
    }

    return nbytes;
  }

  /**
   * \brief Method specialization for extracting a float from the byte stream.
   */
  template <typename NestedConfig = Config>
  std::size_t PopFront(float& data) {
    std::size_t nbytes{};
    if ((bytes_read_ + 4U) <= current_len_) {
      nbytes = Base::template PopFront<NestedConfig>(data);
      bytes_read_ += nbytes;
    }

    return nbytes;
  }

  /**
   * \brief Method specialization for extracting a double from the byte stream.
   */
  template <typename NestedConfig = Config>
  std::size_t PopFront(double& data) {
    std::size_t nbytes{};
    if ((bytes_read_ + 8U) <= current_len_) {
      nbytes = Base::template PopFront<NestedConfig>(data);
      bytes_read_ += nbytes;
    }

    return nbytes;
  }

  /**
   * \brief Skips a current data member, if the Data ID is unknown.
   *
   * Example use-case: If we have two TLV packets after one another and the first shall
   * be skipped, because the Data Id is unknown, a call to Skip() will
   */
  void Skip() {
    std::uint8_t c{};

    for (std::size_t b = 0U; b < current_len_; ++b) {
      // We use PopFront here to not bypass specialized root deserializers
      // such as ComplexDataTypeDeserializer, which do not have Skip().
      PopFront(c);
    }
  }

  /**
   * \brief Getter for the length field value extracted in the constructor.
   */
  std::size_t GetLengthFieldValue() const noexcept { return current_len_; }

  /**
   * \brief Check if the buffer contains more data to pop out.
   */
  bool HasDataRemaining() const noexcept { return bytes_read_ < current_len_; }

 private:
  /**
   * \brief If there is a length field depending on the wire type,
   * this method extracts the length of the actual data.
   * If the wire type is either 0, 1, 2 or 3, it will return the length of the
   * respective fundamental type.
   *
   * \return the length in bytes of the next data member WITHOUT wire type, data id
   * and possible length field.
   */
  std::size_t ConsumeMemberDataLength() noexcept {
    std::size_t len{};

    switch (wire_type_.value_) {
      case tlv::WireTypeEnum::ComplexDataTypeFromDataDefinition: {
        typename Config::LengthFieldType field{};
        if (Base::PopFront(field) > 0U) {
          len = field;
        }
        break;
      }

      case tlv::WireTypeEnum::ComplexDataType8BitLengthField: {
        std::uint8_t field{};
        if (Base::PopFront(field) > 0U) {
          len = field;
        }
        break;
      }

      case tlv::WireTypeEnum::ComplexDataType16BitLengthField: {
        std::uint16_t field{};
        if (Base::PopFront(field) > 0U) {
          len = field;
        }
        break;
      }

      case tlv::WireTypeEnum::ComplexDataType32BitLengthField: {
        std::uint32_t field{};
        if (Base::PopFront(field) > 0U) {
          len = field;
        }
        break;
      }

      // For the following cases of fundamental data types, no length field is
      // part of the current TLV packet.
      case tlv::WireTypeEnum::FundamentalType8Bit: {
        len = 1U;
        break;
      }

      case tlv::WireTypeEnum::FundamentalType16Bit: {
        len = 2U;
        break;
      }

      case tlv::WireTypeEnum::FundamentalType32Bit: {
        len = 4U;
        break;
      }

      case tlv::WireTypeEnum::FundamentalType64Bit: {
        len = 8U;
        break;
      }

      default: {
        assert(false);  // Defensive here, this should not happen.
        // TODO(visdtr) Replace this assert in production code.
        break;
      }
    }

    return len;
  }

  /// Store the wire type for processing the length field.
  tlv::WireTypeFromStream wire_type_{};

  /// Length written after a call of ConsumeNextTag.
  std::size_t current_len_{};

  /// Bytes read for the current specific element
  std::size_t bytes_read_{};
};

/**
 * \brief Special deserializer for the SOME/IP header data.
 * This deserializer is used to prevent the use of htons / htonl for specific header data members
 * of a SOME/IP header and also be able to have an integral solution for every deserialization task.
 */
template <typename Config, typename Root = void>
class SomeIpHeaderDeserializer : public Deserializer<Config, Root> {
 public:
  /// Type-alias for the base class
  using Base = Deserializer<Config, Root>;

  /**
   * \brief This constructor will set up a root deserializer.
   */
  template <typename T = Root, typename C = Config,
            typename std::enable_if<std::is_same<T, void>::value, int>::type = 0>
  explicit SomeIpHeaderDeserializer(std::unique_ptr<PacketBuffer> buffer) : Base(std::move(buffer)) {
    ConsumeHeader();
  }

  /**
   * \brief This constructor will set up a nested deserializer.
   */
  template <typename T = Root, typename C = Config,
            typename std::enable_if<!std::is_same<T, void>::value, int>::type = 0>
  explicit SomeIpHeaderDeserializer(Root* root) : Base{root} {
    ConsumeHeader();
  }

  /**
   * \brief After creating an instance of SomeIpHeaderDeserializer, the header is going to be deserialized from the
   * byte
   * stream. The user can retreive the data with a call to this getter method.
   */
  const ::someip_posix_common::someip::SomeIpMessageHeader& GetDeserializedHeader() const {
    return deserialized_header_;
  }

 private:
  /**
   * \brief Extract the complete serialized SOME/IP header into an internal
   * data model.
   */
  void ConsumeHeader() {
    Base::template PopFront<BEPayloadNoLengthFieldPolicy>(deserialized_header_.service_id_);
    Base::template PopFront<BEPayloadNoLengthFieldPolicy>(deserialized_header_.method_id_);
    Base::template PopFront<BEPayloadNoLengthFieldPolicy>(deserialized_header_.length_);
    Base::template PopFront<BEPayloadNoLengthFieldPolicy>(deserialized_header_.client_id_);
    Base::template PopFront<BEPayloadNoLengthFieldPolicy>(deserialized_header_.session_id_);
    Base::template PopFront<BEPayloadNoLengthFieldPolicy>(deserialized_header_.protocol_version_);
    Base::template PopFront<BEPayloadNoLengthFieldPolicy>(deserialized_header_.interface_version_);
    Base::template PopFront<BEPayloadNoLengthFieldPolicy>(deserialized_header_.message_type_);
    Base::template PopFront<BEPayloadNoLengthFieldPolicy>(deserialized_header_.return_code_);
  }

  /// Contains the deserialized extracted data.
  ::someip_posix_common::someip::SomeIpMessageHeader deserialized_header_;
};

/**
 * \brief This special deserializer acts as a helper for dynamic length arrays with a
 * length field that is configurable in its size.
 */
template <typename Config, typename Root = void>
class ComplexDataTypeDeserializer : public Deserializer<Config, Root> {
 public:
  /// Type-alias for the base class
  using Base = Deserializer<Config, Root>;

  /**
   * \brief This constructor will set up a root deserializer.
   */
  template <typename T = Root, typename C = Config,
            typename std::enable_if<std::is_same<T, void>::value, int>::type = 0>
  explicit ComplexDataTypeDeserializer(std::unique_ptr<PacketBuffer> buffer) : Base(std::move(buffer)) {
    if (Config::LengthFieldActive) {
      length_ = ConsumeLengthField();
    }
  }

  /**
   * \brief This constructor will set up a nested deserializer.
   */
  template <typename T = Root, typename C = Config,
            typename std::enable_if<!std::is_same<T, void>::value, int>::type = 0>
  explicit ComplexDataTypeDeserializer(Root* root) : Base{root} {
    if (Config::LengthFieldActive) {
      length_ = ConsumeLengthField();
    }
  }

  /**
   * \brief Used to set a length if there is no length field specified.
   * If the length of the length field is set to 0 Bits, the number of elements in the array has
   * to be fixed; thus, being an array with fixed length.
   */
  void SetFixedLength(std::size_t fixed_length) {
    assert(!Config::LengthFieldActive);
    length_ = fixed_length;
  }

  /**
   * \brief The method PopFront is overridden in this specialized deserializer
   * to check for overflows. We can not read more data from the stream, than it is defined by the
   * length field.
   */
  template <typename NestedConfig = Config, typename DataType>
  std::size_t PopFront(DataType& data) {
    std::size_t nbytes{};

    // Only do range-checking, when a length field is active
    if (Config::LengthFieldActive) {
      if ((bytes_read_ + sizeof(DataType)) <= length_) {
        nbytes = Base::template PopFront<NestedConfig>(data);
        bytes_read_ += nbytes;
      }
    } else {
      // If there is no length field active (for instance for structs)
      // we try to pop data for any case.
      nbytes = Base::template PopFront<NestedConfig>(data);
      bytes_read_ += nbytes;
    }

    return nbytes;
  }

  /**
   * \brief Method specialization for extracting a bool from the byte stream.
   * \note sizeof(bool) depends on the implementation (normally one byte),
   * but shall be only one byte according to the AUTOSAR specification.
   *
   * \param data value to get from the buffer and to deserialize.
   * \return One byte if the value was popped out from the buffer successfully, 0
   * if not.
   */
  template <typename NestedConfig = Config>
  std::size_t PopFront(bool& data) {
    std::size_t nbytes{};

    // Only do range-checking, when a length field is active
    if (Config::LengthFieldActive) {
      if ((bytes_read_ + 1U) <= length_) {
        nbytes = Base::template PopFront<NestedConfig>(data);
        bytes_read_ += nbytes;
      }
    } else {
      nbytes = Base::template PopFront<NestedConfig>(data);
      bytes_read_ += nbytes;
    }

    return nbytes;
  }

  /**
   * \brief Method specialization for extracting a float from the byte stream.
   */
  template <typename NestedConfig = Config>
  std::size_t PopFront(float& data) {
    std::size_t nbytes{};

    // Only do range-checking, when a length field is active
    if (Config::LengthFieldActive) {
      if ((bytes_read_ + 4U) <= length_) {
        nbytes = Base::template PopFront<NestedConfig>(data);
        bytes_read_ += nbytes;
      }
    } else {
      nbytes = Base::template PopFront<NestedConfig>(data);
      bytes_read_ += nbytes;
    }

    return nbytes;
  }

  /**
   * \brief Method specialization for extracting a double from the byte stream.
   */
  template <typename NestedConfig = Config>
  std::size_t PopFront(double& data) {
    std::size_t nbytes{};

    // Only do range-checking, when a length field is active
    if (Config::LengthFieldActive) {
      if ((bytes_read_ + 8U) <= length_) {
        nbytes = Base::template PopFront<NestedConfig>(data);
        bytes_read_ += nbytes;
      }
    } else {
      nbytes = Base::template PopFront<NestedConfig>(data);
      bytes_read_ += nbytes;
    }

    return nbytes;
  }

  /**
   * \brief Getter for the length field value extracted in the constructor.
   */
  std::size_t GetLengthFieldValue() const noexcept { return length_; }

  /**
   * \brief Check if the complex data type contains more data to pop.
   */
  bool HasDataRemaining() const noexcept { return bytes_read_ < length_; }

 private:
  /**
   * \brief Consumes the length field of configured size.
   *
   * \return The value of the length field.
   */
  std::size_t ConsumeLengthField() {
    using T = typename std::conditional<std::is_same<typename Config::LengthFieldType, void>::value, int,
                                        typename Config::LengthFieldType>::type;
    T length_field{};
    const std::size_t nbytes = Base::PopFront(length_field);

    if (nbytes == 0U) {
      length_field = 0U;
    }

    return length_field;
  }

  /// Value of the length field is stored in this attribute.
  std::size_t length_{};

  /// Bytes that have been read with PopFront for this complex data type deserializer.
  std::size_t bytes_read_{};
};

}  // namespace serialization
}  // namespace someip
}  // namespace someip_posix_common

#endif  // LIB_LIBSOMEIP_POSIX_COMMON_INCLUDE_SOMEIP_POSIX_COMMON_SOMEIP_MARSHALLING_H_

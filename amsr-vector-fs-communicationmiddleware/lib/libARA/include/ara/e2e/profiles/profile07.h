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
/**        \file  profile07.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_E2E_PROFILES_PROFILE07_H_
#define LIB_LIBARA_INCLUDE_ARA_E2E_PROFILES_PROFILE07_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <limits>
#include "ara/crc/crc.h"
#include "ara/e2e/e2e_types.h"
#include "ara/e2e/e2exf/e2e_result.h"
#include "vac/container/array_view.h"

namespace ara {
namespace e2e {
namespace profiles {

/**
 * \brief CRC calculation utilization for E2E profile 7.
 */
class CRCProfile07 {
 public:
  /**
   * \brief E2E profile 7 has a 64 bits CRC.
   */
  using CRCType = std::uint64_t;

  /**
   * \brief Polynomial for the CRC of profile 4.
   *
   * \uptrace SWS_Crc_00062
   */
  static constexpr CRCType kPolynomial{0x42F0E1EBA9EA3693ULL};

  /**
   * \brief This service makes a CRC32 calculation on Crc_Length data bytes, using the polynomial 0x42F0E1EBA9EA3693.
   *
   * \uptrace SWS_Crc_00058
   * \param data_ptr Pointer to start address of data block to be calculated.
   * \param length Length of data block to be calculated in bytes.
   * \param start_value Start value when the algorithm starts.
   * \param is_first_call true: First call in a sequence or individual CRC calculation; start from initial value and
   * ignore Crc_StartValue32.
   * false: Subsequent call in a call sequence; The start value is interpreted to be the return value of the previous
   * function call. Default is true (first call).
   * \return Calculated CRC over the data.
   */
  static CRCType CalculateCRC64P07(const std::uint8_t* data_ptr, const std::uint32_t length, const CRCType start_value,
                                   const bool is_first_call = true) noexcept {
    return ara::crc::Crc::CalculateCRC64P07(ara::crc::Crc::BufferView(data_ptr, length), start_value, is_first_call);
  }
};

/**
 * \brief Common structure of E2E profile 7, derived classes may use.
 */
class Profile07 : public ProfileFrame {
 public:
  /**
   * \brief The length field has four bytes for E2E profile 7.
   *
   * \uptrace PRS_E2EProtocol_00480
   */
  using LengthFieldType = std::uint32_t;

  /**
   * \brief The offset type shall be two bytes for E2E profile 4.
   *
   * \uptrace PRS_E2EProtocol_00488
   */
  using OffsetType = std::uint32_t;

  /**
   * \brief The counter shall be four bytes long for E2E profile 7.
   *
   * \uptrace PRS_E2EProtocol_00480
   */
  using CounterType = std::uint32_t;

  /**
   * \brief DataId type for E2E profile 7 shall be four bytes.
   *
   * \uptrace PRS_E2EProtocol_00480
   */
  using DataIdType = std::uint32_t;

  /**
   * Size of the CRC for E2E profile 7.
   */
  using CRCType = typename CRCProfile07::CRCType;

  /**
   * \brief Get the header size to pre-allocate the correct buffer size.
   *
   * \return The header size from the specification.
   */
  static constexpr std::uint_fast8_t GetHeaderSize() noexcept { return kHeaderSize; }

  /**
   * \brief Compute the byte offset from the bit offset defined in the configuration.
   *
   * \uptrace SWS_E2E_00488
   * \param bit_offset The offset in bits from the configuration.
   * \return the computed offset in bytes.
   */
  OffsetType ComputeOffset(const OffsetType bit_offset) const noexcept {
    const OffsetType offset{static_cast<OffsetType>(bit_offset / static_cast<OffsetType>(8U))};
    return offset;
  }

  /**
   * \brief Compute the CRC over parts of the E2E header and the complete user data.
   *
   * \uptrace SWS_E2E_00367
   * \tparam StreamType Deduced type of the stream
   * \param buffer The buffer the CRC is computed for.
   * \param length The length in bytes.
   * \param offset in bytes the E2E header starts.
   * \return computed CRC-32 over parts of the header excluding the CRC field and
   * the user data.
   */
  template <typename StreamType>
  CRCType ComputeCRC(const StreamType& buffer, const LengthFieldType length, const OffsetType offset) const noexcept {
    CRCType computed_crc{0xFFFFFFFFFFFFFFFFULL};

    // Calculate CRC over data in the front of the E2E header.
    if (offset > 0U) {
      computed_crc = CRCProfile07::CalculateCRC64P07(&buffer[0U], offset, 0xFFFFFFFFFFFFFFFFULL, true);
    }

    // Calculate CRC over user data
    computed_crc =
        CRCProfile07::CalculateCRC64P07(&buffer[offset + kCRCSize], length - offset - kCRCSize, computed_crc, false);

    return computed_crc;
  }

  /**
   * \brief Static size of the E2E header for profile 7.
   * The layout of the header for profile looks like this:
   * | CRC (8 bytes) | Length (4 bytes) | Counter (4 bytes) | DataId (4 bytes) |
   */
  static constexpr std::uint_fast8_t kHeaderSize{20U};

  /**
   * \brief CRC field size for profile 7.
   */
  static constexpr std::uint_fast8_t kCRCSize{8U};

 protected:
  /**
   * \brief Counter for E2E profile 4 to detect missing packets.
   * Incremented after each Protect() call and set after each Check() call within derived classes.
   */
  CounterType counter_value_{};
};

/**
 * \brief Implementation for protector of E2E profile 7.
 */
class Profile07Protector : public Profile07 {
 public:
  /**
   * \brief Allocates the size of the header at the back of the given buffer.
   * This method will not shrink the buffer, but extend it.
   *
   * \param buffer The buffer to extend by the E2E header from the profile.
   * \return gives back the size the given buffer extended by the size of the E2E header.
   */
  template <typename StreamType>
  std::size_t ExtendE2EHeader(StreamType& buffer) const {
    // Current size + E2E header size
    const std::size_t extended_e2e_header_size{buffer.size() + this->GetHeaderSize()};
    buffer.resize(extended_e2e_header_size, 0U);
    return buffer.size();
  }

  /**
   * \brief Protect routine for E2E profile 7.
   *
   * \uptrace SWS_E2E_00486
   * \tparam E2EConfig Deduced type of the configuration.
   * \tparam StreamType Deduced type of the buffer (could be a vector or static array later on).
   * \param config Reference to the configuration that holds the DataId for example, the min and the
   * maximal length.
   * \param buffer Holds the bytestream that includes the pre-allocated E2E header filled with dummy values..
   * As known from the STL containers, the buffer shall provide a size() method to receive the length of the
   * data.
   * \return kInputOk if the protection was successful, kWrongInput if on verification fault.
   */
  template <typename E2EConfig, typename StreamType>
  ProtectReturnType Protect(const E2EConfig& config, StreamType& buffer) noexcept;

 protected:
  /**
   * \brief Writes the length field of the E2E header for profile 7.
   *
   * \uptrace SWS_E2E_00489
   * \tparam StreamType Deduced type of the stream.
   * \param buffer the serialized byte stream (E2E header + user data)
   * \param length to store into the buffer from the parameter before in Big Endian format.
   * \param offset in bytes.
   * \return the serialized length of the stream in BE format for pedantic checks in safety-relevant
   * SW and in case of false writing to stay robust.
   */
  template <typename StreamType>
  void WriteLength(StreamType& buffer, const LengthFieldType length, const OffsetType offset) const noexcept {
    buffer[offset + 8U] = static_cast<std::uint8_t>(length >> 24U);
    buffer[offset + 9U] = static_cast<std::uint8_t>(length >> 16U);
    buffer[offset + 10U] = static_cast<std::uint8_t>(length >> 8U);
    buffer[offset + 11U] = static_cast<std::uint8_t>(length);
  }

  /**
   * \brief Writes the counter into serialized E2E header.
   *
   * \uptrace SWS_E2E_00490
   * \tparam Deduced type of the buffer (std::vector / std::array)
   * \param buffer The buffer to write the counter to.
   * \param offset The offset of the E2E header in bytes.
   */
  template <typename StreamType>
  void WriteCounter(StreamType& buffer, const OffsetType offset) const noexcept {
    buffer[offset + 12U] = static_cast<std::uint8_t>(counter_value_ >> 24U);
    buffer[offset + 13U] = static_cast<std::uint8_t>(counter_value_ >> 16U);
    buffer[offset + 14U] = static_cast<std::uint8_t>(counter_value_ >> 8U);
    buffer[offset + 15U] = static_cast<std::uint8_t>(counter_value_);
  }

  /**
   * \brief Write the DataID of four bytes.
   *
   * \uptrace SWS_E2E_00491
   * \tparam StreamType Deduced type of the buffer.
   * \param buffer The stream that contains E2E header and user data.
   * \param data_id The E2E DataId to store.
   * \param offset The offset of the E2E in bytes.
   */
  template <typename StreamType>
  void WriteDataID(StreamType& buffer, DataIdType data_id, const OffsetType offset) const noexcept {
    buffer[offset + 16U] = static_cast<std::uint8_t>(data_id >> 24U);
    buffer[offset + 17U] = static_cast<std::uint8_t>(data_id >> 16U);
    buffer[offset + 18U] = static_cast<std::uint8_t>(data_id >> 8U);
    buffer[offset + 19U] = static_cast<std::uint8_t>(data_id);
  }

  /**
   * \brief Write the computed CRC into the E2E header.
   *
   * \uptrace SWS_E2E_00493
   * \param buffer The buffer the computed CRC to write to.
   * \param crc Computed crc to write into the E2E header.
   * \param offset in bytes for the E2E header begin.
   */
  template <typename StreamType>
  void WriteCRC(StreamType& buffer, const CRCType crc, const OffsetType offset) const noexcept {
    buffer[offset] = static_cast<std::uint8_t>(crc >> 56U);
    buffer[offset + 1U] = static_cast<std::uint8_t>(crc >> 48U);
    buffer[offset + 2U] = static_cast<std::uint8_t>(crc >> 40U);
    buffer[offset + 3U] = static_cast<std::uint8_t>(crc >> 32U);
    buffer[offset + 4U] = static_cast<std::uint8_t>(crc >> 24U);
    buffer[offset + 5U] = static_cast<std::uint8_t>(crc >> 16U);
    buffer[offset + 6U] = static_cast<std::uint8_t>(crc >> 8U);
    buffer[offset + 7U] = static_cast<std::uint8_t>(crc);
  }

  /**
   * \brief Increments the counter of the profile 7 object. Does a wrap-up on 0xFFFFFFFF.
   *
   * \uptrace SWS_E2E_00494
   */
  void IncrementCounter() noexcept { ++counter_value_; }
};

template <typename E2EConfig, typename StreamType>
Profile07Protector::ProtectReturnType Profile07Protector::Protect(const E2EConfig& config,
                                                                  StreamType& buffer) noexcept {
  ProtectReturnType protect_ret{ProtectReturnType::kInputOk};

  // Compile-time check to verify the DataId type.
  static_assert(std::is_same<typename std::decay<decltype(config.kDataId)>::type, DataIdType>::value,
                "DataId type from configuration does not match the profile's specification.");

  const auto buffer_size{buffer.size()};

  if ((buffer_size == 0U) || (buffer_size < GetHeaderSize())) {
    protect_ret = ProtectReturnType::kWrongInput;
  }

  if (buffer_size > std::numeric_limits<LengthFieldType>::max()) {
    protect_ret = ProtectReturnType::kWrongInput;
  }

  if (protect_ret == ProtectReturnType::kInputOk) {
    const OffsetType offset = ComputeOffset(config.kBitOffset);
    const LengthFieldType length = static_cast<LengthFieldType>(buffer_size);
    WriteLength(buffer, length, offset);
    WriteCounter(buffer, offset);
    WriteDataID(buffer, config.kDataId, offset);
    const auto crc = ComputeCRC(buffer, length, offset);
    WriteCRC(buffer, crc, offset);
    IncrementCounter();
  }

  return protect_ret;
}

/**
 * \brief The checker for E2E profile 7.
 */
class Profile07Checker : public Profile07 {
 public:
  /**
   * \brief Specific check state type for E2E profile 4 according to the specification.
   * Status of the reception on one single Data in one cycle, protected with E2E Profile 4.
   *
   * \uptrace SWS_E2E_00543
   */
  enum class CheckStatusType {
    /**
     * \brief the checks of the Data in this cycle  were successful (including counter  check, which was incremented by
     * 1).
     */
    kOk = 0x00U,
    /**
     * \brief the Check function has been invoked but no new Data is not available since the last call, according to
     * communication medium (e.g. RTE, COM). As a result, no E2E checks of Data have been consequently executed.
     */
    kNoNewData = 0x01U,
    /**
     * \brief  error not related to counters occurred (e.g. wrong crc, wrong length, wrong options, wrong Data ID).
     */
    kStatusError = 0x02U,
    /**
     * \brief the checks of the Data in this cycle  were successful, with the exception of the repetition.
     */
    kStatusRepeated = 0x08U,
    /**
     * \brief the checks of the Data in this cycle were successful (including counter check, which was incremented
     * within the allowed configured delta).
     */
    kOkSomeLost = 0x20U,
    /**
     * \brief the checks of the Data in this cycle were successful, with the exception of counter jump, which changed
     * more than the allowed delta
     */
    kWrongSequence = 0x40U
  };

  /**
   * \brief Check an incoming buffer on reception.
   *
   * \tparam E2EConfig
   * \tparam StreamType
   * \param config The configuration to use.
   * \param buffer the buffer to check.
   * \return The check status according to the profile 4 specification.
   */
  template <typename E2EConfig, typename StreamType>
  CheckStatusType Check(const E2EConfig& config, StreamType& buffer) noexcept {
    CheckStatusType check_status{};
    const auto buffer_size{buffer.size()};

    if (buffer_size > 0U) {
      const OffsetType offset = ComputeOffset(config.kBitOffset);
      const bool offset_in_range = OffsetRangeCheck(buffer_size, kHeaderSize, offset);

      // We must check if there is enough space to access the buffer for reading out
      // length, counter, CRC and data id. Otherwise it will lead to a segmentation fault on accessing memory beyond
      // the limits.
      if (offset_in_range) {
        const LengthFieldType length = static_cast<LengthFieldType>(buffer_size);
        const LengthFieldType received_length = ReadLength(buffer, offset);
        const CounterType received_counter = ReadCounter(buffer, offset);
        const DataIdType received_data_id = ReadDataID(buffer, offset);
        const CRCType received_crc = ReadCRC(buffer, offset);
        const CRCType computed_crc = ComputeCRC(buffer, length, offset);
        check_status = DoChecks(config, length, received_length, counter_value_, received_counter, config.kDataId,
                                received_data_id, computed_crc, received_crc);
      } else {
        check_status = CheckStatusType::kStatusError;
      }
    } else {
      check_status = CheckStatusType::kNoNewData;
    }

    return check_status;
  }

 protected:
  /**
   * \brief Reads the length field of the E2E header for profile 7 and returns it.
   *
   * \uptrace SWS_E2E_00497
   * \tparam StreamType Deduced type of the stream.
   * \param buffer the serialized byte stream (E2E header + user data)
   * \param offset in bytes. The E2E header starts at this offset.
   * \return the deserialized length from the buffer.
   */
  template <typename StreamType>
  LengthFieldType ReadLength(const StreamType& buffer, const OffsetType offset) const noexcept {
    LengthFieldType received_length{};
    received_length = static_cast<LengthFieldType>(static_cast<LengthFieldType>(buffer[offset + 8U]) << 24U |
                                                   static_cast<LengthFieldType>(buffer[offset + 9U]) << 16U |
                                                   static_cast<LengthFieldType>(buffer[offset + 10U]) << 8U |
                                                   static_cast<LengthFieldType>(buffer[offset + 11U]));
    return received_length;
  }

  /**
   * \brief Read the counter field from the serialized E2E header and return the deserialized value.
   *
   * \uptrace SWS_E2E_00498
   * \tparam StreamType Deduced type of the stream.
   * \param buffer the serialized byte stream (E2E header + user data).
   * \param offset in bytes to the beginning of the E2E header.
   * \return the deserialized counter value from the buffer.
   */
  template <typename StreamType>
  CounterType ReadCounter(const StreamType& buffer, const OffsetType offset) const noexcept {
    CounterType received_counter{};
    received_counter = static_cast<LengthFieldType>(
        static_cast<CounterType>(buffer[offset + 12U]) << 24U | static_cast<CounterType>(buffer[offset + 13U]) << 16U |
        static_cast<CounterType>(buffer[offset + 14U]) << 8U | static_cast<CounterType>(buffer[offset + 15U]));
    return received_counter;
  }

  /**
   * \brief Read the DataID field from the serialized E2E header and return the deserialized value.
   *
   * \uptrace SWS_E2E_00499
   * \param buffer
   * \param offset
   * \return the deserialized data id from the buffer.
   */
  template <typename StreamType>
  DataIdType ReadDataID(const StreamType& buffer, const OffsetType offset) const noexcept {
    DataIdType received_data_id{};
    received_data_id = static_cast<DataIdType>(buffer[offset + 16U]) << 24U;
    received_data_id += static_cast<DataIdType>(buffer[offset + 17U]) << 16U;
    received_data_id += static_cast<DataIdType>(buffer[offset + 18U]) << 8U;
    received_data_id += static_cast<DataIdType>(buffer[offset + 19U]);
    return received_data_id;
  }

  /**
   * \brief Read the CRC field from the serialized E2E header and return the deserialized value.
   *
   * \uptrace SWS_E2E_00500
   * \param buffer
   * \param offset
   * \return the deserialized CRC from the buffer.
   */
  template <typename StreamType>
  CRCType ReadCRC(const StreamType& buffer, const OffsetType offset) const noexcept {
    CRCType received_crc{};
    received_crc = static_cast<CRCType>(buffer[offset]) << 56U | static_cast<CRCType>(buffer[offset + 1U]) << 48U |
                   static_cast<CRCType>(buffer[offset + 2U]) << 40U | static_cast<CRCType>(buffer[offset + 3U]) << 32U |
                   static_cast<CRCType>(buffer[offset + 4U]) << 24U | static_cast<CRCType>(buffer[offset + 5U]) << 16U |
                   static_cast<CRCType>(buffer[offset + 6U]) << 8U | static_cast<CRCType>(buffer[offset + 7U]);
    return received_crc;
  }

 private:
  /**
   * \brief The necessary checks are done within this method. The method compares the lengths, counters, the data id
   * and the CRC.
   *
   * \uptrace SWS_E2E_00501
   */
  template <typename E2EConfig>
  CheckStatusType DoChecks(const E2EConfig& config, const LengthFieldType actual_length,
                           const LengthFieldType received_length, const CounterType actual_counter,
                           const CounterType received_counter, const DataIdType cfg_data_id,
                           const DataIdType received_data_id, const CRCType computed_crc,
                           const CRCType received_crc) noexcept {
    CheckStatusType status{CheckStatusType::kOk};

    if ((actual_length == received_length) && (cfg_data_id == received_data_id) && (computed_crc == received_crc)) {
      const CounterType delta_counter = static_cast<CounterType>(received_counter - actual_counter);
      status = CheckCounter(config, delta_counter);
      counter_value_ = received_counter;
    } else {
      status = CheckStatusType::kStatusError;
    }

    return status;
  }

  /**
   * \brief Checks the delta between received counter and the actual counter value in detail.
   *
   * \uptrace SWS_E2E_00501
   *
   * \return the E2E profile 7 check status.
   */
  template <typename E2EConfig>
  CheckStatusType CheckCounter(const E2EConfig& config, const CounterType delta_counter) const noexcept {
    CheckStatusType status{CheckStatusType::kOk};

    if ((delta_counter <= config.kMaxDeltaCounter) && (delta_counter >= 0U)) {
      if (delta_counter > 0U) {
        if (delta_counter == 1U) {
          status = CheckStatusType::kOk;
        } else {
          status = CheckStatusType::kOkSomeLost;
        }
      } else {
        status = CheckStatusType::kStatusRepeated;
      }
    } else {
      status = CheckStatusType::kWrongSequence;
    }

    return status;
  }
};

}  // namespace profiles
}  // namespace e2e
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_E2E_PROFILES_PROFILE07_H_

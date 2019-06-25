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
/**        \file  e2e_types.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_E2E_E2E_TYPES_H_
#define LIB_LIBARA_INCLUDE_ARA_E2E_E2E_TYPES_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>

namespace ara {
namespace e2e {

/**
 * \brief Supported inclusion modes to include the implicit two-byte Data ID in the one-byte CRC.
 */
enum class DataIdModeEnum : std::uint8_t {
  /**
   * \brief Two bytes are included in the CRC (double ID configuration).
   */
  kAll16Bit = 0U,

  /**
   * \brief The low byte is included in the implicit CRC calculation, the low nibble of the high byte is transmitted
   * along with the data (i.e. it is explicitly included), the high nibble of the high byte is not used. This is
   * applicable for the IDs up to 12 bits.
   */
  kLower12Bit = 2U
};

/**
 * \brief Blueprint of necessary configuration parameters to define per event.
 *
 * \uptrace PRS_E2EProtocol_00324
 * \tparam DataId The E2E Data ID for this event sample
 * \tparam Offset offset in bits; at this position the E2E header starts
 * \tparam MinLen Minimal length to expect.
 * \tparam MaxLen Maxmimal length expected.
 */
template <std::uint32_t DataId = 0U, std::uint16_t Offset = 0U, std::uint16_t MinLen = 0U, std::uint16_t MaxLen = 0U>
struct End2EndEventProtectionProps {
  /**
   * \brief Plausability check
   *
   * \uptrace SWS_E2E_00334
   */
  static_assert(MinLen <= MaxLen, "Minimal buffer length shall be smaller than or equal to MaxLen parameter.");

  /**
   * \brief This represents a unique numerical identifier for the referenced event or field notifier that is included in
   * the CRC calculation.
   */
  const std::uint32_t kDataId{DataId};

  /**
   * \brief Offset of the E2E header in the Data[] array in bits.
   */
  const std::uint16_t kBitOffset{Offset};

  /**
   * \brief For variable size data: Maximum length of Data in bits.
   */
  const std::uint16_t kMaxDataLength{MaxLen};

  /**
   * \brief For variable size data: Minimum length of Data in bits.
   */
  const std::uint16_t kMinDataLength{MinLen};
};

/**
 * \brief Blueprint of an E2EProfileConfiguration parameterized by the generator.
 * \tparam DataIdMode Configuration value for kDataIdMode
 * \tparam DeltaCounter Configuration value for kMaxDeltaCounter
 * \tparam MinOkStateInit Configuration value for kMinOkStateInit
 * \tparam MaxErrorStateInit Configuration value for kMaxErrorStateInit
 * \tparam MinOkStateValid Configuration value for kMinOkStateValid
 * \tparam MaxErrorStateValid Configuration value for kMaxErrorStateValid
 * \tparam MinOkStateInvalid Configuration value for kMinOkStateInvalid
 * \tparam MaxErrorStateInvalid Configuration value for MaxErrorStateInvalid
 * \tparam WindowSize Configuration value for kWindowSize
 * \uptrace SWS_E2E_00342
 */
template <DataIdModeEnum DataIdMode, std::uint32_t DeltaCounter, std::uint32_t MinOkStateInit,
          std::uint32_t MaxErrorStateInit, std::uint32_t MinOkStateValid, std::uint32_t MaxErrorStateValid,
          std::uint32_t MinOkStateInvalid, std::uint32_t MaxErrorStateInvalid, std::uint32_t WindowSize>
struct E2EProfileConfiguration {
  /**
   * \brief This attribute describes the inclusion mode that is used to include the implicit two-byte Data ID in the
   * one-byte CRC.
   */
  const DataIdModeEnum kDataIdMode{DataIdMode};

  /**
   * \brief Maximum allowed difference between two counter values of two consecutively received valid messages. For
   * example, if the receiver gets data with counter 1 and MaxDeltaCounter is 3, then at the next reception the receiver
   * can accept Counters with values 2, 3 or 4.
   */
  const std::uint8_t kMaxDeltaCounter{DeltaCounter};

  /**
   * \brief Minimal number of checks in which ProfileStatus equal to E2E_P_OK was determined, within the last WindowSize
   * checks, for the state E2E_SM_INIT.
   */
  const std::uint8_t kMinOkStateInit{MinOkStateInit};

  /**
   * \brief Maximal number of checks in which ProfileStatus equal to E2E_P_ERROR was determined, within the last
   * WindowSize checks, for the state E2E_SM_INIT.
   */
  const std::uint8_t kMaxErrorStateInit{MaxErrorStateInit};

  /**
   * \brief Minimal number of checks in which ProfileStatus equal to E2E_P_OK was determined, within the last WindowSize
   * checks, for the state E2E_SM_VALID.
   */
  const std::uint8_t kMinOkStateValid{MinOkStateValid};

  /**
   * \brief Maximal number of checks in which ProfileStatus equal to E2E_P_ERROR was determined, within the last
   * WindowSize checks, for the state E2E_SM_VALID.
   */
  const std::uint8_t kMaxErrorStateValid{MaxErrorStateValid};

  /**
   * \brief Minimal number of checks in which ProfileStatus equal to E2E_P_OK was determined, within the last WindowSize
   * checks, for the state E2E_SM_INVALID.
   */
  const std::uint8_t kMinOkStateInvalid{MinOkStateInvalid};

  /**
   * \brief Maximal number of checks in which ProfileStatus equal to E2E_P_ERROR was determined, within the last
   * WindowSize checks, for the state E2E_SM_INVALID.
   */
  const std::uint8_t kMaxErrorStateInvalid{MaxErrorStateInvalid};

  /**
   * \brief Size of the monitoring window for the E2E state machine.
   */
  const std::uint8_t kWindowSize{WindowSize};
};

/**
 * \brief Basic frame that concrete profiles can derive from.
 */
class ProfileFrame {
 public:
  /**
   * \brief Taken from the classic E2E protection library.
   */
  enum class ProtectReturnType { kInputOk, kNullInput, kWrongInput };

 protected:
  /**
   * \brief Full generic check for all profiles if the offset in bytes exceeds the size of the buffer.
   * There should be enough space left in the buffer to read out the E2E header.
   *
   * \tparam ContainerSize Deduced type of the container size.
   * \tparam HeaderSizeType Deduced type of the E2E header.
   * \param buffer_size The size of the given buffer.
   * \param header_size The size of the E2E header from the specific profile.
   * \param offset The offset in bytes.
   * \return true if the offset is within the range of the buffer, false if the offset is larger than the remaining
   * space for the complete E2E header to read out.
   */
  template <typename ContainerSize, typename HeaderSizeType, typename OffsetType>
  bool OffsetRangeCheck(const ContainerSize buffer_size, const HeaderSizeType header_size,
                        const OffsetType offset) const noexcept {
    const std::int32_t size_left = static_cast<std::int32_t>(buffer_size) - static_cast<std::int32_t>(offset);
    return (size_left >= static_cast<std::int32_t>(header_size));
  }
};

namespace state_machine {

/**
 * \brief Enumeration representing the results of the check of a single sample.
 * \uptrace SWS_CM_90421
 */
enum class CheckStatus : std::uint8_t {
  /// the checks of the sample in this cycle were successful (including counter check).
  Ok,
  /// sample has a repeated counter.
  Repeated,
  /// The checks of the sample in this cycle were successful, with the exception of counter jump, which changed more
  /// than the allowed delta.
  WrongSequence,
  /// Error not related to counters occurred (e.g. wrong crc, wrong length, wrong Data ID).
  Error,
  /// No value has been received yet (e.g. during initialization). This is used as the initialization value for the
  /// buffer.
  NotAvailable,
  /// No new data is available (assuming a sample has already been received since the initialization).
  NoNewData
};

/**
 * \brief Type-alias for compatibility due to consistency issues in the COM SWS.
 */
using E2ECheckStatus = CheckStatus;

/**
 * \brief Represents the state if samples can be used or not.
 * \uptrace SWS_CM_90422
 */
enum class State : std::uint8_t {
  /// Communication of the samples of this event functioning properly according to e2e checks, sample(s) can be used.
  Valid,
  /// State before ara::e2e is initialized, sample cannot be used.
  NoData,
  /// No data from the publisher is available since the initialization, sample(s) cannot be used.
  Init,
  /// Communication of the sample of this event not functioning properly, sample(s) cannot be used.
  Invalid
};

/**
 * \brief Type-alias for compatibility due to consistency issues in the COM SWS.
 */
using E2EState = State;

}  // namespace state_machine
}  // namespace e2e
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_E2E_E2E_TYPES_H_

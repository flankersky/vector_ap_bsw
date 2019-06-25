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
/**        \file  e2e_marshalling.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/
#ifndef LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_E2E_MARSHALLING_H_
#define LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_E2E_MARSHALLING_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/e2e/e2exf/transformer.h"
#include "someip-posix-common/someip/marshalling.h"

namespace ara {
namespace com {
namespace someip_posix {

/**
 * \brief Bit offset of the E2E header inside a SOME/IP packet relative to the protected/checked parts of the E2E.
 * \details The E2E header is located after the SOME/IP header, but only the trailing 8 bytes of the SOME/IP header
 * (client ID, session ID, protocol version, ...) are involved in the E2E CRC calculation. The service ID, method ID and
 * length are not involved in the E2E CRC calculation.
 * before the SOME/IP payload.
 */
constexpr std::size_t kE2EHeaderSomeIpBitOffset =
    ((::someip_posix_common::someip::kHeaderSize - ::someip_posix_common::someip::kHeaderLength) * 8);

/* ---- E2E protection serializer -------------------------------------------------------------- */

/**
 * \brief Serializer for the E2E protection header.
 * \tparam TransformerType Type of used E2E transformer
 * \tparam Config Serialization properties not required for E2E header serializer because E2E format defined by
 * protocol
 * and implemented by E2E transformer.
 * \tparam Root This serializer specialization may have or may have not a root
 */
template <typename TransformerType, typename Root = void,
          typename Config = ::someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>
class E2EHeaderSerializer : public ::someip_posix_common::someip::serialization::Serializer<Config, Root> {
 public:
  /// Type-alias for the base template class of this specialized serializer.
  using Base = ::someip_posix_common::someip::serialization::Serializer<Config, Root>;

  /**
   * \brief The constructor of E2EHeaderSerializer
   *
   * \param root Could be a nested serializer; defaults to nullptr which stands for root.
   * \param transformer E2E transformer, protects the payload and writes the E2E header
   * \param non_protected_offset Byte offset in the packet buffer to the protected parts
   */
  explicit E2EHeaderSerializer(Root* root, TransformerType& transformer, const std::uint8_t non_protected_offset = 0U)
      : Base{root}, transformer_{transformer}, non_protected_offset_{non_protected_offset} {
    /* Allocate the E2E header in packet. Header will be written in context of CloseHandler() */
    Base::template Extend(transformer_.GetHeaderSize());
  }

  /**
   * \brief Close if this for the nested serializer.
   */
  void Close() { CloseHandler(); }

  /**
   * \brief Is called whenever Close method is called from outside.
   * This handler protects the payload and updates the E2E header with CRC, length, etc. information
   */
  void CloseHandler() {
    /* Protect the payload byte stream and update the E2E header */
    transformer_.Protect(Base::GetBuffer(), non_protected_offset_);
  }

  /// Reference to used transformer object which handles E2E protection and update of E2E header
  TransformerType& transformer_;

  /// Byte offset to the packet buffer part which shall be protected, previous bytes will not be protected.
  const std::uint8_t non_protected_offset_;
};

/* ---- E2E checker deserializer --------------------------------------------------------------- */

/**
 * \brief Deserializer and checker for the E2E protection header.
 * \tparam TransformerType Type of used E2E transformer
 * \tparam Config Serialization properties not required for E2E header deserializer because E2E format defined by
 * protocol and implemented by E2E transformer.
 * \tparam Root This deserializer specialization may have or may have not a root
 */
template <typename TransformerType, typename Root = void,
          typename Config = ::someip_posix_common::someip::serialization::BEPayloadNoLengthFieldPolicy>
class E2EHeaderDeserializer : public ::someip_posix_common::someip::serialization::Deserializer<Config, Root> {
 public:
  /// Type-alias for the base template class of this specialized deserializer.
  using Base = ::someip_posix_common::someip::serialization::Deserializer<Config, Root>;

  /**
   * \brief The constructor of E2EHeaderDeserializer. Constructor initially consume the E2E header. Check if the E2E
   * header against the payload must be done separately using the Check() API.
   *
   *
   * \param root Could be a nested deserializer; defaults to nullptr which stands for root.
   * \param transformer E2E transformer used for check of payload vs E2E header.
   * \param non_checked_offset Byte offset in the packet buffer to the checked parts
   */
  explicit E2EHeaderDeserializer(Root* root, TransformerType& transformer, const std::uint8_t non_checked_offset = 0U)
      : Base{root}, transformer_{transformer}, non_checked_offset_{non_checked_offset} {
    /* Consume the E2E header */
    Base::Skip(transformer_.GetHeaderSize());
  }

  /**
   * \brief Check the E2E header against the complete packet payload.
   * \return E2E transformer check result.
   */
  ara::e2e::e2exf::Result Check() {
    /* Check the E2E header against package byte stream */
    return transformer_.Check(Base::GetBuffer(), non_checked_offset_);
  }

  /// Reference to used transformer object which handles E2E protection and update of E2E header
  TransformerType& transformer_;

  /// Byte offset to the packet buffer part which shall be checked, previous bytes will not be checked.
  const std::uint8_t non_checked_offset_;
};

}  // namespace someip_posix
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_SOMEIP_POSIX_INCLUDE_ARA_SOMEIP_POSIX_E2E_MARSHALLING_H_

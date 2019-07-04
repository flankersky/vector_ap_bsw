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
/**        \file  rest/uuid.h
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_UUID_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_UUID_H_

#include <cstdint>
#include <ostream>
#include <tuple>

#include "ara/rest/allocator.h"
#include "ara/rest/exception.h"
#include "ara/rest/string.h"

namespace ara {
namespace rest {

/**
 * \brief UUID representation class, according to RFC 4122
 * \see https://tools.ietf.org/html/rfc4122
 */
class Uuid {
 public:
  /**
   * \brief default ctor
   */
  Uuid() noexcept : time_low_(0), time_mid_(0), time_high_and_version_(0), clock_seq_(0), node_(0) {}
  /** \brief Constructs a Uuid from a string representation
   *
   *  Throws an IllformedUuidException if parsing fails.
   *
   *  \param id a UUID in RFC4122 format
   */
  explicit Uuid(StringView id) : Uuid() {
    if (!FromString(id)) {
      throw IllformedUuidException("UUID string is invalid.");
    }
  }

  /**
   * \brief constructor taking value parts
   *
   * \param time_low
   * \param time_mid
   * \param time_high_and_version
   * \param clock_seq
   * \param node
   */
  Uuid(std::uint32_t time_low, std::uint16_t time_mid, std::uint16_t time_high_and_version, std::uint16_t clock_seq,
       std::uint64_t node) noexcept : time_low_(time_low),
                                      time_mid_(time_mid),
                                      time_high_and_version_(time_high_and_version),
                                      clock_seq_(clock_seq),
                                      node_(node) {}

  /**
   * \brief get time low
   * \return time low
   */
  std::uint32_t GetTimeLow() const noexcept { return time_low_; }

  /**
   * \brief get time mid
   * \return time mid
   */
  std::uint16_t GetTimeMid() const noexcept { return time_mid_; }

  /**
   * \brief get time high and version
   * \return time high and version
   */
  std::uint16_t GetTimeHighAndVersion() const noexcept { return time_high_and_version_; }

  /**
   * \brief get clock seq
   * \return clock seq
   */
  std::uint16_t GetClockSeq() const noexcept { return clock_seq_; }

  /**
   * \brief get node
   * \return node
   */
  std::uint64_t GetNode() const noexcept { return node_; }

  /**
   * \brief equal operator
   * \param a uuid left side operator
   * \param b uuid right side operator
   * \return true if both uuids are the same, false otherwise
   */
  friend bool operator==(const Uuid& a, const Uuid& b) noexcept {
    return (a.time_low_ == b.time_low_ && a.time_mid_ == b.time_mid_ &&
            a.time_high_and_version_ == b.time_high_and_version_ && a.clock_seq_ == b.clock_seq_ && a.node_ == b.node_);
  }

  /**
   * \brief not equal operator
   * \param a uuid left side operator
   * \param b uuid right side operator
   * \return true if both uuids are different, false otherwise
   */
  friend bool operator!=(const Uuid& a, const Uuid& b) noexcept { return !(a == b); }

  /**
   * \brief smaller than operator
   * \param a uuid left side operator
   * \param b uuid right side operator
   * \return true if first uuids is lexically smaller than second, false otherwise
   */
  friend bool operator<(const Uuid& a, const Uuid& b) noexcept {
    auto aa = std::make_tuple(a.time_low_, a.time_mid_, a.time_high_and_version_, a.clock_seq_, a.node_);
    auto bb = std::make_tuple(b.time_low_, b.time_mid_, b.time_high_and_version_, b.clock_seq_, b.node_);
    return aa < bb;
  }

 private:
  /**
   * \brief convert string to uuid
   * \param str_uuid
   * \return true if conversion was successful, false otherwise
   */
  bool FromString(const StringView& str_uuid) noexcept;

  std::uint32_t time_low_;
  std::uint16_t time_mid_;
  std::uint16_t time_high_and_version_;
  std::uint16_t clock_seq_;
  std::uint64_t node_;
};

/** \brief Converts Uuid into its canonical textual representation
 *  \param uuid a UUID
 *  \param alloc an allocator
 *  \return its canonic textual representation
 */
String ToString(const Uuid& uuid, Allocator* alloc = GetDefaultAllocator());

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_UUID_H_

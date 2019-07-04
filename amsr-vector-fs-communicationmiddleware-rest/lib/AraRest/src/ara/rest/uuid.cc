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
/**        \file  uuid.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/rest/uuid.h"

#include <cstring>
#include <string>

namespace ara {
namespace rest {

bool Uuid::FromString(const StringView& str_uuid) noexcept {
  size_t last{0}, next{0};
  uint8_t tokenIndex{0};

  // get parts of the UUID separated by "-"
  while ((next = str_uuid.find('-', last)) != String::npos) {
    String token(str_uuid.cbegin() + last, std::distance(str_uuid.cbegin() + last, str_uuid.cbegin() + next));
    last = next + 1;
    switch (tokenIndex) {
      case 0:
        time_low_ = static_cast<std::uint32_t>(std::stol(token, nullptr, 16));
        break;
      case 1:
        time_mid_ = static_cast<std::uint16_t>(std::stol(token, nullptr, 16));
        break;
      case 2:
        time_high_and_version_ = static_cast<std::uint16_t>(std::stol(token, nullptr, 16));
        break;
      case 3:
        clock_seq_ = static_cast<std::uint16_t>(std::stol(token, nullptr, 16));
        break;
      default:
        return false;
    }
    tokenIndex++;
  }

  // get the last remaining part of the UUID
  if (tokenIndex == 4) {
    String token(str_uuid.cbegin() + last, std::distance(str_uuid.cbegin() + last, str_uuid.cend()));
    node_ = static_cast<std::uint64_t>(std::stoll(token, nullptr, 16));
    return true;
  }

  return false;
}

/**
 * \brief Append fragment as string.
 */
String ToString(const Uuid& uuid, Allocator* alloc) {
  // 37 = 32 hex chars + 4 separators + string terminator
  char buf[37];
  snprintf(buf, sizeof(buf), "%8.8x-%4.4x-%4.4x-%4.4x-%8.8x%4.4x", uuid.GetTimeLow(), uuid.GetTimeMid(),
           uuid.GetTimeHighAndVersion(), uuid.GetClockSeq(),
           static_cast<std::uint32_t>((uuid.GetNode() & 0xFFFFFFFF0000) >> 16),
           static_cast<std::uint32_t>(uuid.GetNode() & 0xFFFF));

  /* TODO(X): use allocator for String uuid; */
  (void)alloc;
  String uuid_str;
  uuid_str.resize(sizeof(buf) - 1);
  memcpy(&uuid_str[0], buf, sizeof(buf) - 1);
  return buf;
}

}  // namespace rest
}  // namespace ara

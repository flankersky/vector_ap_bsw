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
/**        \file  someip_message.cc
 *        \brief  This class represents a single complete SOME/IP message.
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someip-posix-common/someip/someip_message.h"
#include <cassert>
#include "vac/language/cpp14_backport.h"

namespace someip_posix_common {
namespace someip {

SomeIpMessage::SomeIpMessage(DataBuffer&& data)
    : data_{std::move(data)},
      from_address_{{}, false},
      deserializer_{vac::language::make_unique<DataBuffer>(data_.begin(), data_.begin() + kHeaderSize)},
      io_vector_array_{{data_.data(), data_.size()}} {
  assert((GetHeader().length_ + kHeaderLength) == data_.size());
}

SomeIpMessage::SomeIpMessage(DataBuffer&& data, const SocketAddress& from_address)
    : data_{std::move(data)},
      from_address_{from_address, true},
      deserializer_{vac::language::make_unique<DataBuffer>(data_.begin(), data_.begin() + kHeaderSize)},
      io_vector_array_{{data_.data(), data_.size()}} {
  assert((GetHeader().length_ + kHeaderLength) == data_.size());
}

}  // namespace someip
}  // namespace someip_posix_common

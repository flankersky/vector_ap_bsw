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
/**        \file  serialize.cc
 *        \brief  TODO
 *
 *      \details  SOME/IP compliant (de)serializer components
 *
 *********************************************************************************************************************/
#include "someip-posix-common/someip/serialize.h"

namespace someip_posix_common {
namespace someip {

namespace serialization {

#ifndef __BYTE_ORDER__

IsMSBTest::IsMSBTest() noexcept : isMsb_(false) {
  // Write a value to the int, check where it ends up in the overlaying array
  union Test {
    Test(std::uint32_t v) : _0(v) {}
    std::uint32_t _0;
    std::uint8_t _1[4];
  } test = {0x01};  // set first octet
  isMsb_ = test._1[3];
}

// Ctor invoked once upon initialization
IsMSBTest __isMSBTest;

#endif

}  // namespace serialization

}  // namespace someip
}  // namespace someip_posix_common

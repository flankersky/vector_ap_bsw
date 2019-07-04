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
/**        \file  rest/string.h
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_STRING_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_STRING_H_
#include <vac/container/string_view.h>
#include <cstdint>
#include <memory>
#include <string>
#if __cplusplus > 201103L
#include <experimental/string_view>
#endif

#include "ara/rest/allocator.h"

namespace ara {
namespace rest {

/**
 * \brief maximum number of characters to examine for CSTRING_TO_STRINGVIEW
 */
static constexpr std::size_t MAX_LEN = 2048;  // To accommodate uri max length

/**
 * \brief macro for getting string
 */
#define STRINGVIEW_TO_STRING(Y, X) Y.append(X.data(), X.size())

/**
 * \brief macro for getting string view from cstring
 */
#define CSTRING_TO_STRINGVIEW(X) StringView(X, strnlen(X, MAX_LEN))

/**
 * \brief macro for getting string view from String
 */
#define STRING_TO_STRINGVIEW(X) X.c_str(), X.size()

/** \brief Equivalent of std::basic_string but with a custom ara::rest::Allocator adaptor
 *
 *  Detailed description
 *
 *  \param param
 *  \return return type
 */
template <typename CharT, typename TraitsT = std::char_traits<CharT>>
using BasicString =
    std::basic_string<CharT, TraitsT,
                      std::allocator<CharT>>;  // TODO(x): use ara::rest::StdAllocator<CharT> when its implemented.

/**
 * \brief typedef for string.
 */
using String = BasicString<char>;

/**
 * \brief typedef for StringView.
 */
#if __cplusplus <= 201103L
/**
 * \brief typedef for StringView.
 */
template <typename CharT, typename TraitsT = std::char_traits<CharT>>
using BasicStringView = BasicString<CharT, TraitsT>;

#elif defined __QNXNTO__
/**
 * \brief typedef for StringView.
 */
template <typename CharT, typename TraitsT = std::char_traits<CharT>>
using BasicStringView = BasicString<CharT, TraitsT>;

#else
/**
 * \brief typedef for StringView.
 */
template <typename CharT, typename TraitsT = std::char_traits<CharT>>
using BasicStringView = std::experimental::basic_string_view<CharT, TraitsT>;
#endif

/**
 * \brief typedef for StringView.
 */
using StringView = vac::container::string_view;

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_STRING_H_

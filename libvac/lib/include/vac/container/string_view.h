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
/**        \file  string_view.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_CONTAINER_STRING_VIEW_H_
#define LIB_INCLUDE_VAC_CONTAINER_STRING_VIEW_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <algorithm>
#include <array>
#include <cassert>
#include <cstring>
#include <iterator>
#include <limits>
#include <string>
#include <utility>

#include "vac/container/array_view.h"
#include "vac/testing/test_adapter.h"

namespace vac {
namespace container {

/**
 * \brief end of view indicator by the functions that expect a view index
 */
static constexpr std::size_t npos = std::numeric_limits<std::size_t>::max();

/**
 * \brief basic_string_view describes an object that can refer to a constant contiguous sequence of char-like objects
 * with the first element of the sequence at position zero.
 *
 * basic_string_view  holds only two members: a const pointer T and a size. .
 *
 */
template <typename T>
class basic_string_view final : public const_array_view<T> {
 public:
  /// Type of contained element
  using value_type = typename const_array_view<T>::value_type;

  /// Type of the size field
  using size_type = typename const_array_view<T>::size_type;

  /// Reference type for contained elements
  using reference = typename const_array_view<T>::reference;

  /// Const reference type for contained elements
  using const_reference = typename const_array_view<T>::const_reference;

  /// Pointer type for contained elements
  using pointer = typename const_array_view<T>::pointer;

  /// Const pointer type for contained elements
  using const_pointer = typename const_array_view<T>::const_pointer;

  /// Iterator for the basic_string_view
  using iterator = typename const_array_view<T>::iterator;

  /// Const iterator for the basic_string_view
  using const_iterator = typename const_array_view<T>::const_iterator;

  /// Default constructor for an empty basic_string_view
  constexpr basic_string_view() : const_array_view<T>() {}

  /**
   * \brief Constructor with a const pointer and size
   *
   * When given a nullptr, the size is reset to 0
   *
   * \param data_val Pointer to the raw array containing the data
   * \param data_size Length of data in number of elements
   */
  constexpr basic_string_view(const_pointer data_val, size_type data_size) noexcept
      : const_array_view<T>(data_val, data_size) {
    // Inline-If in constructor to support constexpr in C++11
  }

  /**
   * \brief Constructor with a const pointer
   * \param data_val Pointer to the raw array containing the data
   */
  constexpr explicit basic_string_view(const_pointer data_val) noexcept
      : basic_string_view(data_val, (data_val == nullptr ? 0 : std::char_traits<T>::length(data_val))) {}

  /**
   * \brief Templated constructor to convert from arbitrary containers
   *
   * When given a nullptr, the size is reset to 0
   *
   * \param t A container providing contiguous data, ContainerType::data() and ContainerType::size().
   */
  template <typename ContainerType>
  constexpr explicit basic_string_view(const ContainerType& t) : const_array_view<T>(t) {}

  /// Copy constructor
  basic_string_view(const basic_string_view&) = default;

  /**
   * \brief Copy assignment
   * \returns a reference to the assigned-to basic_string_view
   */
  basic_string_view& operator=(const basic_string_view&) = default;

  /// Move constructor
  basic_string_view(basic_string_view&&) = default;

  /**
   * \brief Move assignment
   * \returns a reference to the assigned-to basic_string_view
   */
  basic_string_view& operator=(basic_string_view&&) = default;

  virtual ~basic_string_view() = default;

  /**
   * \brief Get the length of the contained string
   * \returns the size of the string
   */
  size_type length() const noexcept { return this->size(); }

  /**
   * \brief comparision
   *
   * \param other Index of the element to get
   * \return bool
   */
  bool operator==(const basic_string_view& other) const noexcept { return (0 == compare(other)); }

  /**
   * \brief comparision
   *
   * \param other Index of the element to get
   * \return bool
   */
  bool operator==(const char* other) const noexcept {
    basic_string_view other_view(other);
    return (0 == compare(other_view));
  }

  /**
   * \brief compare the contents of two string_view objects
   *
   * \param other Index of the element to get
   * \return int : negative value if this view is less than the other character sequence, zero if the both character
   *               sequences are equal, positive value if this view is greater than the other character sequence.
   */
  int compare(const basic_string_view& other) const noexcept {
    size_type counter = 0;
    size_type len = std::min(this->size(), other.size());
    while ((counter < len) && ((*this)[counter] == other[counter])) {
      ++counter;
    }

    int result = 0;
    if (counter == len) {
      // we checked all common characters. Difference is now the difference in the length of strings
      if (this->size() < other.size()) {
        result = -1;
      } else if (this->size() > other.size()) {
        result = 1;
      } else {
        result = 0;
      }
    } else {
      // there are remaining characters in both strings that were not compared
      result = (*this)[counter] - other[counter];

      if ((result < 0) || ((result == 0) && (this->size() < other.size()))) {
        result = -1;
      } else if ((result > 0) || ((result == 0) && (this->size() > other.size()))) {
        result = 1;
      }
    }

    return result;
  }

  /**
   * \brief returns a view of the substring
   *
   * \param pos position of the first character
   * \param count requested length
   * \return View of the substring
   */
  basic_string_view substr(size_type pos = 0, size_type count = npos) const noexcept {
    basic_string_view str;
    if (pos <= this->size()) {
      if (count == npos) {
        count = this->size() - pos;
      } else {
        // count is the smaller of count and size() - pos
        count = std::min(count, this->size() - pos);
      }

      str = basic_string_view((this->data() + pos), count);
    }
    return str;
  }

  /**
   * \brief returns the position of found character
   *
   * \param ch character to search for
   * \param pos position of the first character
   * \return Position of the first character of the found substring, or npos if no such substring is found
   */
  size_type find(value_type ch, size_type pos = 0) const noexcept {
    size_type index{npos};

    // npos will be returned in case pos == size_.
    for (index = pos; index < this->size(); index++) {
      if ((*this)[index] == ch) {
        break;
      }
    }
    if (index >= this->size()) {
      index = npos;
    }
    return index;
  }

  /**
   * \brief returns the position of found string
   *
   * \param str pointer to a character string to search for
   * \param pos position of the first character
   * \param len length of substring to search for
   * \return Position of the first character of the found substring, or npos if no such substring is found.
   */
  size_type find(const_pointer str, size_type pos, size_type len) const noexcept {
    size_type index{npos};

    // npos will be returned in case pos == size_.
    if (this->size() < len || len == 0 || str == nullptr) {
      index = npos;
    } else {
      // search for string.
      for (index = pos; index < this->size(); index++) {
        const_pointer str_view = &(this->at(index));
        const_pointer sub_view = str;

        if ((this->size() - index) < len) {
          index = npos;
          break;
        }

        size_type sub_index = 0;
        while (*str_view++ == *sub_view++) {
          // TODO(PAASR-2604) Add range check here.
          sub_index++;
          if (sub_index == len) {
            return index;
          }
        }
      }
    }

    if (index >= this->size()) {
      index = npos;
    }
    return index;
  }

  /**
   * \brief returns the position of found character
   *
   * \param sw view to search for.
   * \param pos position at which to start the search.
   * \return Position of the first character of the found substring, or npos if no such substring is found.
   */
  size_type find(basic_string_view sw, size_type pos = 0) const noexcept { return find(sw.data(), pos, sw.length()); }

  /**
   * \brief Finds the last substring equal to the given character sequence.
   * \param sw view to search for.
   * \param pos position at which to start the search.
   * \return Position of the first character of the found substring or npos if no such substring is found.
   */
  size_type rfind(basic_string_view sw, size_type pos = npos) const noexcept {
    size_type found_position(npos);
    if (sw.size() == this->size()) {
      if (compare(sw)) {
        found_position = 0;
      }
    } else if (sw.size() < this->size()) {
      // Move pos towards the start of the string by sw.size() characters
      pos = std::min(pos, this->size() - sw.size() + 1);
      // now move from pos towards beginning
      for (size_type loop_counter = 0; loop_counter <= pos && found_position == npos; ++loop_counter) {
        size_type it = pos - loop_counter;
        basic_string_view check_view = this->substr(it, sw.size());
        if (check_view.compare(sw) == 0) {
          found_position = it;
        }
      }
    }
    return found_position;
  }

  /**
   * \brief Finds the last substring equal to the given character sequence.
   * \param ch pointer to a character string to search for.
   * \param pos position at which to start the search.
   * \param count length of substring to search for.
   * \return Position of the first character of the found substring or npos if no such substring is found.
   */
  size_type rfind(const_pointer ch, size_type pos, size_type count) const noexcept {
    return this->rfind(basic_string_view(ch, count), pos);
  }

  /**
   * \brief Finds the first character equal to any of the characters in the given character sequence.
   *
   * \param sw view to search for.
   * \param pos position at which to start the search.
   * \return Position of the first occurrence of any character of the substring, or npos if no such character is found.
   */
  size_type find_first_of(basic_string_view sw, size_type pos = 0) const noexcept {
    return find_first_of(sw.data_, pos, sw.size_);
  }

  /**
   * \brief Finds the first character equal to any of the characters in the given character sequence.
   *
   * \param ch character to search for.
   * \param pos position at which to start the search.
   * \return Position of the first occurrence of any character of the substring, or npos if no such character is found.
   */
  size_type find_first_of(value_type ch, size_type pos = 0) const noexcept { return find(ch, pos); }

  /**
   * \brief Finds the first character equal to any of the characters in the given character sequence.
   *
   * \param ch character to search for.
   * \param pos position at which to start the search.
   * \param count length of the string of characters to search for.
   * \return Position of the first occurrence of any character of the substring, or npos if no such character is found.
   */
  size_type find_first_of(const_pointer ch, size_type pos, size_type count) const noexcept {
    size_type retval{npos};
    if (count > 0) {
      for (; pos < this->size(); ++pos) {
        size_type p = find(ch, pos, count);
        if (p != npos) {
          retval = p;
          break;
        }
      }
    }
    return retval;
  }

  /**
   * \brief Finds the first character not equal to any of the characters in the given character sequence.
   *
   * \param sw view to search for.
   * \param pos position at which to start the search.
   * \return Position of the first occurrence of any character not equal to any character of the substring, or npos if
   * no such character is found.
   */
  size_type find_first_not_of(basic_string_view sw, size_type pos = 0) const noexcept {
    size_type retval{npos};
    for (; pos < this->size(); ++pos) {
      size_type p = sw.find((*this)[pos]);
      if (p == npos) {
        retval = pos;
        break;
      }
    }
    return retval;
  }

  /**
   * \brief Finds the first character not equal to the given character.
   *
   * \param ch character to search for.
   * \param pos position at which to start the search.
   * \return Position of the first occurrence not equal to the given character, or npos if no such character is found.
   */
  size_type find_first_not_of(value_type ch, size_type pos = 0) const noexcept {
    size_type retval{npos};
    for (; pos < this->size(); ++pos) {
      if ((*this)[pos] != ch) {
        retval = pos;
        break;
      }
    }
    return retval;
  }

  /**
   * \brief Finds the first character not equal to any of the characters in the given character sequence.
   *
   * \param ch character to search for.
   * \param pos position at which to start the search.
   * \param count length of the string of characters to search for.
   * \return Position of the first occurrence of any character not equal to any character of the substring, or npos if
   * no such character is found.
   */
  size_type find_first_not_of(const_pointer ch, size_type pos, size_type count) const noexcept {
    return find_first_not_of(basic_string_view{ch, count}, pos);
  }

  /**
   * \brief Finds the last character equal to any of the characters in the given character sequence.
   *
   * \param ch pointer to a string of characters to search for.
   * \param pos position in this string_view at which to start the search.
   * \param count length of the string of characters to search for.
   * \return Position of the last occurrence of any character of the substring, or npos if no such character is found.
   */
  size_type find_last_of(const_pointer ch, size_type pos, size_type count) const noexcept {
    if (this->size_ > 0) {
      pos = std::min(pos, this->size_ - 1);  // pos must point to a valid character

      // iterate this data
      for (size_type i = 0; i <= pos; ++i) {
        size_type self_check_position = pos - i;

        // iterate input data
        for (size_type input_check_position = 0; input_check_position < count; ++input_check_position) {
          if ((*this)[self_check_position] == ch[input_check_position]) {
            return self_check_position;  // Multi-return to abort any nesting level of search loops
          }
        }
      }
    }
    return npos;
  }

  /**
   * \brief Finds the last character equal to any of the characters in the given character sequence.
   *
   * \param sw view to search for.
   * \param pos position this string_view at which to start the search.
   * \return Position of the last occurrence of any character of the substring, or npos if no such character is found.
   */
  size_type find_last_of(basic_string_view sw, size_type pos = npos) const noexcept {
    return find_last_of(sw.data(), pos, sw.length());
  }

  /**
   * \brief Finds the last character equal to any of the characters in the given character sequence.
   *
   * \param ch character to search for.
   * \param pos position this string_view at which to start the search.
   * \return Position of the last occurrence of any character of the substring, or npos if no such character is found.
   */
  size_type find_last_of(value_type ch, size_type pos = npos) const noexcept { return find_last_of(&ch, pos, 1); }

  /**
   * \brief Finds the last character equal to any of the characters in the given character sequence.
   *
   * \param ch null-terminated sequence of characters to search for.
   * \param pos position this string_view at which to start the search.
   * \return Position of the last occurrence of any character of the substring, or npos if no such character is found.
   */
  size_type find_last_of(const_pointer ch, size_type pos = npos) const noexcept {
    return find_last_of(ch, pos, strlen(ch));
  }

  /**
   * \brief Functions check if the string_view containts a null terminating symbol
   * \return Returns true if a null character is contained, false if not
   */
  bool IsNullTerminated() const noexcept { return find_last_of('\0') != npos; }

  /**
   * \brief Moves the end of the view back by n characters.
   * \param n number of characters to remove from the end of the view.
   */
  void remove_suffix(size_type n) noexcept {
    if (n < this->size_) {
      this->size_ -= n;
    } else {
      this->size_ = 0;
    }
  }

  /**
   * \brief Moves the start of the view forward by n characters.
   * \param n number of characters to remove from the start of the view.
   */
  void remove_prefix(size_type n) noexcept {
    if (this->data_ != nullptr) {
      n = std::min(n, this->size_);
      this->data_ += n;
      this->size_ -= n;
    }
  }

 private:
  /// Friend declaration for testing.
  FRIEND_TEST(StringViewFixture, at);
};
/**
 * \brief string_view is a constant assortment of characters
 */
using string_view = basic_string_view<char>;

}  // namespace container
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_CONTAINER_STRING_VIEW_H_

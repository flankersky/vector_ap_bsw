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
/**        \file  static_string_stream.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_INCLUDE_VAC_CONTAINER_STATIC_STRING_STREAM_H_
#define LIB_INCLUDE_VAC_CONTAINER_STATIC_STRING_STREAM_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <utility>

#include "vac/container/basic_string.h"
#include "vac/container/string_view.h"

namespace vac {
namespace container {

/**
 * \brief A static stringstream implementation.
 */
class StaticStringStream {
 private:
  /**
   * \brief Type of the underlying data.
   */
  using base_type = SafeString;

 public:
  /**
   * \brief The size_type.
   */
  using size_type = base_type::size_type;

  /**
   * \brief Constructor which reserves a specified amount of memory.
   * \param size The amount of memory to be reserved.
   */
  explicit StaticStringStream(size_type size) : data_() { data_.reserve(size); }

  /**
   * \brief Constructor.
   * \param data pointer to the character string to store.
   */
  explicit StaticStringStream(const char* data) : data_(data) {}

  /**
   * \brief Destructor
   */
  virtual ~StaticStringStream() = default;

  /**
   * \brief Replaces the contents of the underlying string.
   * \param input pointer to the character string which shall be set.
   * \throws std::length_error When input length exceeds remaining capacity.
   */
  void str(const char* input) {
    reset();
    append(input);
  }

  /**
   * \brief Returns a StringView to the underlying data.
   * \returns a string view to the underlying data
   */
  vac::container::string_view str() const noexcept { return vac::container::string_view(data_.data()); }

  /**
   * \brief Returns a pointer to the underlying array serving as character storage.
   * \returns a pointer to the underlying data
   */
  const char* data() const noexcept { return data_.data(); }

  /**
   * \brief Append data to the StaticStringStream object.
   * \param input pointer to the character string to append.
   * \throws std::length_error When input length exceeds remaining capacity.
   * \returns *this
   */
  StaticStringStream& append(const char* input) {
    const size_t input_size{strlen(input)};
    if (input_size < capacity_remaining()) {
      // append data
      data_.append(input, input_size);
    } else {
      throw std::length_error("Input length exceeds remaining capacity");
    }
    return *this;
  }

  /**
   * \brief Append data to the StaticStringStream object.
   * \param input data to be appended.
   * \throws std::length_error When input exceeds remaining capacity.
   * \returns *this
   */
  StaticStringStream& operator<<(StaticStringStream& input) {
    append(input.data());
    return *this;
  }

  /**
   * \brief Returns the number of characters that can be held in currently allocated storage.
   * \returns the number of characters that can be stored
   */
  size_type capacity() const noexcept { return data_.capacity(); }

  /**
   * \brief Returns the remaining number of characters that can be held in currently allocated storage without a reset.
   * \returns the remaining number of characters that can be stored
   */
  size_type capacity_remaining() const noexcept { return data_.capacity() - data_.size(); }

  /**
   * \brief Returns the number of characters currently stored.
   * \returns the number of stored characters.
   */
  size_type size() const noexcept { return data_.size(); }

  /**
   * \brief Resets the data position to the beginning, the remaining capacity is then at maximum.
   */
  void reset() noexcept { data_.clear(); }

  /**
   * \brief Accessor operator.
   * \param i The index to be accessed.
   * \return The character which is placed at the given index.
   */
  char& operator[](size_type i) { return data_[i]; }

  /**
   * \brief Accessor operator for const.
   * \param i The index to be accessed.
   * \return The character which is placed at the given index.
   */
  const char& operator[](size_type i) const { return data_[i]; }

  /**
   * \brief Erases the content of an StaticStringStream positioned at first argument
   * \param pos Iterator pointing to the cell which should be erased
   */
  void erase(base_type::iterator pos) { data_.erase(pos); }

  /**
   * \brief Erases the content of an StaticStringStream from first argument to second argument
   * \param from The index to start from
   * \param to The index to end
   */
  void erase(size_type from, size_type to) { data_.erase(from, to); }

  /**
   * \brief Erases the content of an StaticStringStream from first argument to second argument
   * \param first Iterator pointing to the start pos
   * \param last Iterator pointing to the end pos
   */
  void erase(base_type::iterator first, base_type::iterator last) { data_.erase(first, last); }

  /**
   * \brief Returns an iterator to the first character of the string.
   * \return Iterator to the first character of the string.
   */
  base_type::iterator begin() { return data_.begin(); }

  /**
   * \brief Returns an iterator to the character following the last character of the string. This character acts as a
   * placeholder, attempting to access it results in undefined behavior.
   * \return Iterator to the character following the last character of the string
   */
  base_type::iterator end() { return data_.end(); }

 private:
  /**
   * \brief contains data.
   */
  base_type data_;
};

}  // namespace container
}  // namespace vac

#endif  // LIB_INCLUDE_VAC_CONTAINER_STATIC_STRING_STREAM_H_

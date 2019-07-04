/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  category_mask.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_CONVERSATION_ACCESS_CATEGORY_MASK_H_
#define SRC_SERVER_CONVERSATION_ACCESS_CATEGORY_MASK_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <assert.h>
#include <initializer_list>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "ara/log/logging.hpp"

namespace amsr {
namespace diag {
namespace server {
namespace conversation {
namespace access {

/**
 * \brief Enables to create a bit mask masking multiple categories. The given template parameter must be an Enum and its
 * the bit mask of the values must not overlap: (value1 & value2)==0.
 */
template <typename EnumType>
class CategoryMask final {
  static_assert(std::is_enum<EnumType>::value, "Must be an enum type");

 public:
  /**
   * Underlying type of the EnumType
   */
  using UnderlyingType = typename std::underlying_type<EnumType>::type;

  /**
   * \brief All categories are initialized as unmasked
   */
  CategoryMask() = default;

  /**
   * \brief Only the given category is masked.
   * \param category
   */
  explicit CategoryMask(EnumType category) : mask_(static_cast<UnderlyingType>(category)) {}

  /**
   * \brief Initializes the mask with a given list of categories.
   * \remark If an overlap between the bit masks of the values given in the list exists, a logic_error will be thrown.
   * \param category_list
   */
  explicit CategoryMask(std::initializer_list<EnumType> category_list) {
    for (typename std::initializer_list<EnumType>::value_type category : category_list) {
      const UnderlyingType value = static_cast<UnderlyingType>(category);
      if (OverlapsWithMask(value)) {
        ara::log::LogError() << "Bit mask of category list elements must not overlap!" << std::to_string(mask_)
                             << " (mask) vs. " << std::to_string(value) << " (category)";
        throw std::logic_error("Bit mask of category list elements must not overlap!");
      }
      mask_ |= value;
    }
  }

  /**
   * \brief Mask the given category
   * \remark If at least one bit set in the category's underlying value is masked already, a logic error will be thrown.
   * Use IsMasked if you are unsure. If IsMasked returns false and a call of this method results in an exception, an
   * overlap between the bit masks of EnumType exists which violates the intended use of this class.
   * \param category category to be masked
   */
  void Mask(EnumType category) {
    const UnderlyingType value = static_cast<UnderlyingType>(category);
    if (OverlapsWithMask(value)) {
      ara::log::LogError() << "Bit mask of category must not overlap with mask: " << std::to_string(mask_)
                           << " (mask) vs. " << std::to_string(value) << " (category)";
      throw std::logic_error("Bit mask of category must not overlap with mask!");

    } else {
      mask_ |= value;
    }
  }

  /**
   * \brief Checks if a category is masked already.
   * \param category category to check
   * \return true (masked), if all set bits of the underlying value of the category are also set in the mask, else false
   * (unmasked)
   */
  bool IsMasked(EnumType category) const {
    const UnderlyingType value = static_cast<UnderlyingType>(category);
    return !(~mask_ & value);
  }

 private:
  UnderlyingType mask_{0};

  bool OverlapsWithMask(UnderlyingType value) const { return mask_ & value; }
};

}  // namespace access
}  // namespace conversation
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_CONVERSATION_ACCESS_CATEGORY_MASK_H_

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
/**        \file  pattern.h
 *        \brief  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_ROUTING_PATTERN_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_ROUTING_PATTERN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

#include "ara/rest/string.h"

namespace ara {
namespace rest {
namespace test {
class PatternTestFixture_Test_Test; /**> forward declaration for testing */
}

/**
 * \brief The wildchard character for one segment.
 */
static constexpr char WILDCARD = '*';

/**
 * Order criteria:
 *   - lexicographical order with segments as digits (in left-to-right order)
 *     'car' < 'car/window'
 *   - then order: '**' < '*' < stuff
 */
class Pattern {
 public:
  friend class test::PatternTestFixture_Test_Test; /**> for testing */

  /**
   * \brief Constructor
   * \param pattern The pattern.
   */
  explicit Pattern(StringView pattern) : pattern_() {
    pattern_.reserve(10);  // Reserve space for 10 segments

    String temp_str;
    STRINGVIEW_TO_STRING(temp_str, pattern);

    std::stringstream buf;
    buf.str(temp_str);
    String segment;
    while (std::getline(buf, segment, '/')) {
      pattern_.push_back(std::move(segment));
    }
  }
  /**
   * \brief Lexicographical 'is equal' compare
   * \param left
   * \param right
   */
  friend bool operator==(const Pattern& left, const Pattern& right) noexcept {
    bool equal{true};
    std::size_t i = 0;
    std::size_t j = 0;
    for (; i < left.pattern_.size(); i++) {
      // check if segments are equal
      if (i < left.pattern_.size() && j < right.pattern_.size()) {
        if ((left.pattern_.at(i).compare(right.pattern_.at(j))) != 0) {
          if (left.pattern_.at(i).size() == 1 && left.pattern_.at(i).at(0) == WILDCARD) {
            // single Wildcard left
            // check if one of the patterns is at the end while another continues
            if ((i + 1 >= left.pattern_.size() && j + 1 < right.pattern_.size()) ||
                (j + 1 >= right.pattern_.size() && i + 1 < left.pattern_.size())) {
              equal = false;
              break;
            }
          } else if (left.pattern_.at(i).size() == 2 && left.pattern_.at(i).at(0) == WILDCARD &&
                     left.pattern_.at(i).at(1) == WILDCARD) {
            // double wildcard left
            // check if left or right reached the last segment -> this leads to equal patterns
            if ((!(j + 1 < right.pattern_.size())) || (!(i + 1 < left.pattern_.size()))) {
              break;
            } else {
              // search next segment which is not equal to the double wildcard
              while (i < left.pattern_.size() && left.pattern_.at(i).size() == 2 &&
                     left.pattern_.at(i).at(0) == WILDCARD && left.pattern_.at(i).at(1) == WILDCARD) {
                i++;
              }
              // if the last segment of left is equal to the double wildcard left and right are equal
              if (i >= left.pattern_.size()) {
                i--;
                break;
              }
              // serch next segment which isn't the double wildcard in the right pattern
              while ((left.pattern_.at(i).compare(right.pattern_.at(j))) != 0) {
                j++;
                if (j >= right.pattern_.size()) {
                  return false;
                }
              }
            }
          } else if (right.pattern_.at(i).size() == 1 && right.pattern_.at(i).at(0) == WILDCARD) {
            // single Wildcard right
            // check if one of left or right is at the end while another continues
            if ((i + 1 >= left.pattern_.size() && j + 1 < right.pattern_.size()) ||
                (j + 1 >= right.pattern_.size() && i + 1 < left.pattern_.size())) {
              equal = false;
              break;
            }
          } else if (right.pattern_.at(j).size() == 2 && right.pattern_.at(j).at(0) == WILDCARD &&
                     right.pattern_.at(j).at(1) == WILDCARD) {
            // double wildcard right
            // check if left or right reached the last segment -> this leads to equal patterns
            if ((!(j + 1 < right.pattern_.size())) || (!(i + 1 < left.pattern_.size()))) {
              break;
            } else {
              // search next segment which is not equal to the double wildcard
              while (j < right.pattern_.size() && right.pattern_.at(j).size() == 2 &&
                     right.pattern_.at(j).at(0) == WILDCARD && right.pattern_.at(j).at(1) == WILDCARD) {
                j++;
              }
              // if the last segment of right is equal to the double wildcard left and right are equal
              if (j >= right.pattern_.size()) {
                j--;
                break;
              }
              // search next segment which isn't the double wildcard in the left pattern
              while ((left.pattern_.at(i).compare(right.pattern_.at(j))) != 0) {
                i++;
                if (i >= left.pattern_.size()) {
                  return false;
                }
              }
            }

          } else {
            // no wildcard leads to not equal
            equal = false;
            break;
          }
        } else if ((left.pattern_.at(i).size() == 2 && left.pattern_.at(i).at(0) == WILDCARD &&
                    left.pattern_.at(i).at(1) == WILDCARD)) {
          // both contains an double wildcard at the same position
          // check if both patterns are not at the last segment
          //-> if one of them is at the end it leads to the result equal
          if (left.pattern_.size() > i + 1 && right.pattern_.size() > j + 1) {
            // check which pattern has more segments left -> if left loop through left and the other way
            if (left.pattern_.size() - i > right.pattern_.size() - j) {
              // search next segment which is not equal to the double wildcard
              while (j < right.pattern_.size() && right.pattern_.at(j).size() == 2 &&
                     right.pattern_.at(j).at(0) == WILDCARD && right.pattern_.at(j).at(1) == WILDCARD) {
                j++;
              }
              // if the last segment of right is equal to the double wildcard left and right are equal
              if (j >= right.pattern_.size()) {
                j--;
                break;
              }
              // loop through the left pattern and search for the next equal segment
              std::size_t x = i + 1;
              for (; x < left.pattern_.size(); x++) {
                if (right.pattern_.at(j).compare(left.pattern_.at(x)) == 0) {
                  i = x;
                  equal = true;
                  break;
                }
                equal = false;
              }
            } else if (left.pattern_.size() - i < right.pattern_.size() - j) {
              // search next segment which is not equal to the double wildcard
              while (i < left.pattern_.size() && left.pattern_.at(i).size() == 2 &&
                     left.pattern_.at(i).at(0) == WILDCARD && left.pattern_.at(i).at(1) == WILDCARD) {
                i++;
              }
              // if the last segment of left is equal to the double wildcard left and right are equal
              if (i >= left.pattern_.size()) {
                i--;
                break;
              }
              std::size_t x = j + 1;
              for (; x < right.pattern_.size(); x++) {
                if (right.pattern_.at(x).compare(left.pattern_.at(i)) == 0) {
                  j = x;
                  equal = true;
                  break;
                }
                equal = false;
              }
            }
          } else {
            return true;
          }
        }
        j++;
      } else if (left.pattern_.size() > i && left.pattern_.at(i).compare(right.pattern_.at(j - 1)) != 0 &&
                 (left.pattern_.at(i).size() < 2 || left.pattern_.at(i).at(0) != WILDCARD ||
                  left.pattern_.at(i).at(1) != WILDCARD)) {
        equal = false;
        break;
      }
    }
    if (left.pattern_.size() <= i &&
        (right.pattern_.size() > j && (right.pattern_.at(j).size() < 2 || right.pattern_.at(j).at(0) != WILDCARD ||
                                       right.pattern_.at(j).at(1) != WILDCARD))) {
      equal = false;
    }
    return equal;
  }

  /**
     * \brief Lexicographical 'is not equal' compare
     * \param left
     * \param right
     * \return
     */
  friend bool operator!=(const Pattern& left, const Pattern& right) noexcept { return !(left == right); }

  /**
   * \brief Lexicographical 'is equal' compare
   * \param left
   * \param right
   * \return
   */
  friend bool operator<(const Pattern& left, const Pattern& right) noexcept {
    std::size_t i = 0;
    std::size_t j = 0;
    for (; i < left.pattern_.size(); i++) {
      if (j >= right.pattern_.size()) {
        return false;
      }
      if (left.pattern_.at(i).compare(right.pattern_.at(j)) != 0) {
        if (left.pattern_.at(i).size() == 2 && left.pattern_.at(i).at(0) == WILDCARD &&
            left.pattern_.at(i).at(1) == WILDCARD) {
          return true;
        } else if (left.pattern_.at(i).size() == 1 && left.pattern_.at(i).at(0) == WILDCARD) {
          if (right.pattern_.at(j).size() == 2 && right.pattern_.at(j).at(0) == WILDCARD &&
              right.pattern_.at(j).at(1) == WILDCARD) {
            return false;

          } else {
            return true;
          }
        } else {
          if (left.pattern_.at(i).compare(right.pattern_.at(j)) < 0) {
            return true;
          } else {
            return false;
          }
        }
      }
      j++;
    }
    if (left.pattern_.size() < right.pattern_.size()) {
      return true;
    } else {
      return false;
    }
  }

  /**
   * \brief Function to get pattern
   *
   * \return pattern_string
   */
  const std::vector<String>& GetPatternString() const noexcept { return pattern_; }

 private:
  /**
   * \brief Data of Pattern.
   */
  std::vector<String> pattern_;
};
}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_ROUTING_PATTERN_H_

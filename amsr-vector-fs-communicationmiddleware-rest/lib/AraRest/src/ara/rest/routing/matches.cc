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
/**        \file  matches.cc
 *        \brief  Represents the result of a pattern matching on a URI
 *
 *      \details  Contains the method implementations for URI matches
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "ara/rest/routing/matches.h"
#include "ara/rest/routing/router.h"

namespace ara {
namespace rest {

/**
 * \brief Empty string view.
 */
StringView EMPTY;

/**
 * \brief Empty Match object.
 */
const Match default_match(EMPTY);

Matches::Matches(Route& route, Pattern& uri_pattern) {
  route_ = &route;
  Pattern route_pattern = route.GetPattern();
  route_pattern_ = route_pattern.GetPatternString();
  uri_pattern_ = uri_pattern.GetPatternString();
  CreateWildCardList();
}

void Matches::CreateWildCardList() {  // TODO(visnep) : Need to clarify the exact behavior with visjak.
  std::size_t route_index = 1;

  if ((uri_pattern_.size() == 1 && (uri_pattern_[0] == "**")) ||
      ((uri_pattern_.size() == 1 && (uri_pattern_[0] == "*")) && route_pattern_.size() == 2)) {
    for (size_t index = 1; index < route_pattern_.size(); index++) {  // Push characters in list
      if ((route_pattern_[index] != "*") && (route_pattern_[index] != "**")) {
        wildcard_matches_.push_back(Match(StringView(STRING_TO_STRINGVIEW(route_pattern_[index]))));
      }
    }
  } else if (route_pattern_.size() > 1) {
    for (std::size_t index = 1; index < uri_pattern_.size(); index++) {
      if (uri_pattern_[index] == "*") {  // * in uri_pattern
        if (route_index < route_pattern_.size() && (route_pattern_[route_index] != "*") &&
            (route_pattern_[route_index] != "**")) {
          wildcard_matches_.push_back(Match(StringView(STRING_TO_STRINGVIEW(route_pattern_[route_index]))));
          route_index++;
        }
      } else if (uri_pattern_[index] == "**") {  // ** in uri_pattern
        std::vector<String>::iterator itr_beg = route_pattern_.begin() + index;
        std::vector<String>::iterator itr_after;
        std::vector<String>::iterator itr_uri;
        if (uri_pattern_.size() == 1 || index == uri_pattern_.size()) {
          itr_beg = itr_beg + 1;
          itr_after = route_pattern_.end();
        } else {
          itr_after = find(route_pattern_.begin() + index, route_pattern_.end(), uri_pattern_[index + 1]);
          itr_uri = uri_pattern_.begin() + index + 1;
          bool is_itr_found = true;
          for (; itr_after != route_pattern_.end();) {
            is_itr_found = true;
            std::vector<String>::iterator itr_temp = itr_after;
            for (; itr_temp != route_pattern_.end() && itr_uri != uri_pattern_.end(); itr_temp++, itr_uri++) {
              if (itr_uri + 1 != uri_pattern_.end() && itr_temp + 1 != route_pattern_.end()) {
                if ((*(itr_uri + 1)) == "**") {  // Let it get handled in next cycle
                  break;
                } else if ((((*(itr_uri + 1)) != (*(itr_temp + 1))) && ((*(itr_uri + 1)) != "*"))) {
                  is_itr_found = false;  // Change the itr_after position
                  break;
                }
              } else {
                break;
              }
            }

            if (is_itr_found == false) {  // Change the itr_after position
              std::vector<String>::iterator itr_new = itr_after + 1;
              itr_after = find(itr_new, route_pattern_.end(), uri_pattern_[index + 1]);
              itr_uri = uri_pattern_.begin() + index + 1;
            } else {
              break;
            }
          }
        }
        for (; itr_beg != itr_after; itr_beg++) {  // Push characters in list
          if (((*itr_beg) != "*") && ((*itr_beg) != "**")) {
            wildcard_matches_.push_back(Match(StringView(STRING_TO_STRINGVIEW((*itr_beg)))));
          }
          route_index++;
        }
      } else {
        route_index++;  // point to next element in route_pattern
      }
    }
  }
}

std::size_t Matches::Count() const noexcept { return wildcard_matches_.size(); }

const Match& Matches::Get(std::size_t i) const noexcept {
  if ((wildcard_matches_.size() != 0) && (i <= wildcard_matches_.size()))
    return wildcard_matches_[i];
  else
    return default_match;
}

Matches::MatchRange Matches::Get() noexcept { return MatchRange{wildcard_matches_.begin(), wildcard_matches_.end()}; }

Matches::ConstMatchRange Matches::Get() const noexcept {
  return ConstMatchRange{wildcard_matches_.begin(), wildcard_matches_.end()};
}

const Route& Matches::GetRoute() const noexcept { return *route_; }
}  // namespace rest
}  // namespace ara

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
/**        \file  meta_info_map_conversion.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDM_INCLUDE_UDSTRANSPORT_META_INFO_MAP_CONVERSION_H_
#define LIB_LIBDM_INCLUDE_UDSTRANSPORT_META_INFO_MAP_CONVERSION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstdlib>
#include <limits>
#include <string>
#include <type_traits>
#include <utility>

#include "udstransport/uds_message.h"
#include "vac/container/container_aggregator.h"

namespace ara {
namespace diag {
namespace udstransport {

/**
 * \brief Converts the MetaInfoMaps to a map that can by processed by ara::com
 *
 * If the key string cannot be converted to the target (integer) key type due to an overflow, the corresponding
 * key-value-pair is not converted.
 *
 * \remark This is temporary workaround until ara::com interfaces can be adapted.
 *
 * \param container container for all the meta info maps
 * \return the new meta info map
 */
template <typename AraComMetaInfoMap, typename MapContainerType>
AraComMetaInfoMap ConvertToAraComMetaInfo(const MapContainerType &container) {
  using KeyType = typename AraComMetaInfoMap::key_type;
  using UnderlyingKeyType = typename std::underlying_type<KeyType>::type;  // must be enum

  AraComMetaInfoMap meta_info{};

  auto iter = container.begin();
  while (iter != container.end()) {
    const std::pair<std::string, std::string> &key_value_pair = *iter;
    char *end = nullptr;
    std::uint64_t value = std::strtoul(key_value_pair.second.c_str(), &end, 10);
    if (value <= std::numeric_limits<UnderlyingKeyType>::max()) {
      meta_info.emplace(static_cast<KeyType>(value), key_value_pair.second);
    }
    ++iter;
  }
  return meta_info;
}

} /* namespace udstransport */
} /* namespace diag */
} /* namespace ara */

#endif  // LIB_LIBDM_INCLUDE_UDSTRANSPORT_META_INFO_MAP_CONVERSION_H_

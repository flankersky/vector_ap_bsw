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
/**        \file  packet_cache.h
 *        \brief  Packet Cache
 *
 *      \details  Packet Cache
 *
 *********************************************************************************************************************/

#ifndef SRC_SOMEIPD_POSIX_PACKET_ROUTER_PACKET_CACHE_H_
#define SRC_SOMEIPD_POSIX_PACKET_ROUTER_PACKET_CACHE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <map>
#include "someipd-posix/packet_router/packet_router_interface.h"

namespace someipd_posix {
namespace packet_router {

/**
 * \brief Cache for SOME/IP Packets.
 */
template <class CacheKey>
class PacketCache {
 private:
  /**
   * \brief Single element of the cache.
   */
  struct CacheElement {
    /**
     * \brief Constructor. Once the Cached element is created, it is marked as not stale.
     *
     * \param cached_value The cached packet
     */
    explicit CacheElement(const Packet cached_value) : cached_value_(cached_value) {}

    Packet cached_value_;  ///< Cached value
    bool stale_{false};    ///< Value in cache is out of date
  };

 public:
  /**
   * \brief Type for cache map
   */
  using CacheMap = std::map<CacheKey, CacheElement>;

  /**
   * \brief Constructor
   */
  PacketCache() = default;

  /**
   * \brief Destructor
   */
  virtual ~PacketCache() = default;

  /**
   * \brief If the key is found in the cache, update its value. Otherwise, insert the key/value pair.
   *
   * \param key Key to find the cached element.
   * \param cached_value The cached packet.
   */
  void InsertOrAssign(const CacheKey key, const Packet cached_value) {
    const auto it = cache_list_.find(key);
    if (it == cache_list_.end()) {
      cache_list_.emplace(key, CacheElement{cached_value});
    } else {
      it->second.cached_value_ = cached_value;
    }
  }

  /**
   * \brief Gets the cached value for this field.
   *
   * \param key Key of the cached element.
   *
   * \return The cached packet, if exists AND not stale; otherwise nullptr.
   */
  Packet GetValue(const CacheKey key) const {
    Packet packet{nullptr};

    const auto it = cache_list_.find(key);
    if (it != cache_list_.end()) {
      if (!(it->second.stale_)) {
        packet = it->second.cached_value_;
      }
    }
    return packet;
  }

  /**
   * \brief Mark the given cache value as stale (out-of-date).
   *
   * \param key Key of the cached element.
   */
  void MarkStale(const CacheKey key) {
    const auto it = cache_list_.find(key);
    if (it != cache_list_.end()) {
      it->second.stale_ = true;
    }
  }

  /**
   * \brief Checks if the given cache value is stale (out-of-date).
   *
   * \param key Key of the cached element.
   *
   * \return True if value is found and stale; false otherwise.
   */
  bool IsStale(const CacheKey key) const {
    const auto it = cache_list_.find(key);
    return (it == cache_list_.end()) || (it->second.stale_);
  }

 private:
  CacheMap cache_list_;  ///< Cache list
};

} /* namespace packet_router */
} /* namespace someipd_posix */

#endif  // SRC_SOMEIPD_POSIX_PACKET_ROUTER_PACKET_CACHE_H_

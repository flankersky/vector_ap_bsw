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
/**        \file  uds_message.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDM_INCLUDE_UDSTRANSPORT_UDS_MESSAGE_H_
#define LIB_LIBDM_INCLUDE_UDSTRANSPORT_UDS_MESSAGE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "udstransport/uds_transport_protocol_types.h"
#include "vac/container/array_view.h"
#include "vac/container/container_aggregator.h"
#include "vac/memory/buffer_provider.h"
#include "vac/memory/leaky_array_allocator.h"
#include "vac/memory/object_pool.h"

namespace ara {
namespace diag {
namespace udstransport {
/**
 * \brief Class representing a UDS message
 */
class UdsMessage {
 public:
  /**
   * \brief Typedef for uds adresses.
   */
  using Address = uint16_t;

  /**
   * \brief Type for the meta information of a UdsMessage.   *
   */
  using MetaInfoMap = std::map<std::string, std::string, std::less<std::string>,
                               vac::memory::LeakyArrayAllocator<std::pair<std::string, std::string>>>;
  /** \brief Pointer to meta info maps */
  using MetaInfoMapPointer = std::shared_ptr<MetaInfoMap>;

  /**
   * \brief Enumerator for application layer target address type.
   */
  enum class TargetAddressType : uint8_t { PHYSICAL = 0x00, FUNCTIONAL = 0x01 };

  /**
   * \brief Type of tuple to pack UdsTransportProtocolHandlerID and ChannelID together, to form a global unique (among
   * all used UdsTransportProtocolHandlers within DM) identifier of a UdsTransportProtocol channel.
   */
  using GlobalChannelIdentifier = std::tuple<UdsTransportProtocolHandlerID, ChannelID>;

  /**
   * \brief Define pointer to UDS Message.
   */
  using Ptr = vac::memory::SmartObjectPoolUniquePtr<UdsMessage>;

  /**
   * \brief Define pointer to const UDS Message
   */
  using ConstPtr = vac::memory::SmartObjectPoolUniquePtrToConst<UdsMessage>;

  // TODO(PAASR-3036): Create header for compile time config parameters
  /// Maximum number of AddMetaInfo calls
  static constexpr std::size_t kMaxNumberOfMetaInfoMaps = 3;

  /**
   * \brief Overloaded constructor.
   */
  UdsMessage(Address sa, Address ta, TargetAddressType ta_type, uint32_t bytevector_length,
             vac::memory::SmartBufferProvider<uint8_t>::UniqueBufferPtr buffer_ptr,
             GlobalChannelIdentifier global_channel_identifier);

  UdsMessage(const UdsMessage&) = delete;
  UdsMessage operator=(const UdsMessage&) = delete;
  UdsMessage(UdsMessage&&) = delete;
  UdsMessage operator=(UdsMessage&&) = delete;

  /**
   * \brief Get the source address of the uds message.
   * \return The source address of the uds message.
   */
  Address GetSa() const;

  /**
   * \brief Get the target address of the uds message.
   * \return The target address of the uds message.
   */
  Address GetTa() const;

  /**
   * \brief Get the target address type (phys/func) of the uds message.
   * \return The target address type of the uds message.
   */
  TargetAddressType GetTaType() const;

  /**
   * \brief Get the UDS message data starting with the SID (A_Data as per ISO)
   * \return The entire payload (A_Data)
   */
  const ByteVector& GetPayload() const;

  /**
   * \brief return the underlying buffer for write access.
   *
   * \note needed by Plugin implementation to fill the created UdsMessage with data. I.e. plugin impl. will typically
   * create UdsMessage via createUdsMessage() and then call this method on it and write into returned
   * udstransport::ByteVector.
   * \return payload of the UDSMessage starting from SID.
   */
  ByteVector& GetPayload();

  /**
   * \brief Returns the meta information.
   *
   * \return meta info map container.
   */
  const vac::container::ContainerAggregator<MetaInfoMapPointer>& GetMetaInfo() const { return meta_info_; }

  /**
   * \brief add new metaInfo to this message.
   *
   * \note typically called by the transport plugin to add channel specific meta-info.
   * (see SWS - there are already predefined meta-info keys for DoIP....)
   * If the meta info map storage is full already, the call is ignored.
   *
   * \param metaInfo meta information relevant for UdsMessage
   */
  void AddMetaInfo(std::shared_ptr<MetaInfoMap> metaInfo);

  /**
   * \brief Return the global channel identifier
   */
  GlobalChannelIdentifier GetGlobalChannelIdentifier() const { return global_channel_identifier_; }

 private:
  /**
   * \brief The application layer target address type.
   */
  const TargetAddressType target_type_;

  /**
   * \brief The source address.
   */
  const Address source_address_;

  /**
   * \brief The target address.
   */
  const Address target_address_;

  /**
   * \brief The data length.
   */
  uint32_t data_length_;

  /**
   * \brief Unique pointer to the buffer.
   */
  vac::memory::SmartBufferProvider<uint8_t>::UniqueBufferPtr buffer_unique_ptr_;

  /**
   * \brief The Data
   */
  ByteVector data_;

  /**
   * \brief Global channel identifier.
   */
  const GlobalChannelIdentifier global_channel_identifier_;

  /** backend storage for Meta info maps*/
  std::array<MetaInfoMapPointer, kMaxNumberOfMetaInfoMaps> meta_info_storage_;

  /** front-end object for accessing stored meta info maps */
  vac::container::ContainerAggregator<MetaInfoMapPointer> meta_info_{
      vac::container::array_view<MetaInfoMapPointer>(meta_info_storage_)};
};

} /* namespace udstransport */
} /* namespace diag */
} /* namespace ara */

#endif  // LIB_LIBDM_INCLUDE_UDSTRANSPORT_UDS_MESSAGE_H_

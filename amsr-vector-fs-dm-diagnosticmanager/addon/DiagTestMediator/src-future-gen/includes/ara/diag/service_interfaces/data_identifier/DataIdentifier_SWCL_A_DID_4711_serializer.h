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
/**        \file  DataIdentifier_SWCL_A_DID_4711_serializer.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_FUTURE_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_DATAIDENTIFIER_SWCL_A_DID_4711_SERIALIZER_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_FUTURE_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_DATAIDENTIFIER_SWCL_A_DID_4711_SERIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <tuple>

#include "ara/diag/service_interfaces/data_identifier/DataIdentifier_SWCL_A_DID_4711.h"
#include "common/multi_byte_type.h"

#include "ara/log/logging.hpp"
#include "vac/container/array_view.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace data_identifier {
namespace serializer {

/**
 * \brief Serializer class for Proxy DataIdentifier_SWCL_A_DID_4711
 */
class DataIdentifier_SWCL_A_DID_4711Serializer {
 public:
  /**
   * \brief Type of DID in output of Read method.
   */
  using ReadOutput = proxy::methods::Read::Output;

  /**
   * \brief Type of DID in output of Read method.
   */
  using WriteOutput = proxy::methods::Write::Output;

  /**
   * \brief Type of DID as input for Write method, stored in a tuple.
   */
  using TupleDidData = std::tuple<ara::diag::service_interfaces::data_identifier::uint8,
                                  ara::diag::service_interfaces::data_identifier::uint16>;

  /**
   * \brief Buffer type
   */
  using OutBufferType = vac::container::array_view<std::uint8_t>;

  /**
   * \brief Buffer type
   */
  using InBufferType = vac::container::const_array_view<std::uint8_t>;

  static constexpr std::size_t kDidSize = 3;

  /**
   * \brief Returns the size of the DID data.
   * \returns the size of the DID data (3 bytes).
   */
  static std::size_t GetDidDataSize() { return kDidSize; }

  /**
   * \brief Serializes typed DID data to UDS byte view.
   *
   * \param in_data_ref The typed data to be serialized.
   * \param out_data_view The view onto the buffer the serialization is written to.
   */
  static void Serialize(const ReadOutput& in_data_ref, OutBufferType out_data_view) {
    ara::log::LogDebug() << __func__;
    // check precondition on size
    if (out_data_view.size() != kDidSize) {
      throw std::length_error("UdsByteView of size " + std::to_string(out_data_view.size()) +
                              " does not match required size (=" + std::to_string(kDidSize) + ").");
    }
    // Serialization of dataRecord_DataELement_one (uint8)
    out_data_view.at(0) = in_data_ref.dataRecord_DataElement_one;
    // Serialization of dataRecord_DataELement_two (uint16)
    out_data_view.at(1) = amsr::diag::common::GetByte(in_data_ref.dataRecord_DataElement_two, 1);
    out_data_view.at(2) = amsr::diag::common::GetByte(in_data_ref.dataRecord_DataElement_two, 0);
  }

  /**
   * \brief Deserializes UDS byte view to typed DID data.
   *
   * \param in_data_view The UDS serialized data to be deserialized.
   * \return The tuple of the deserialized data.
   */
  static TupleDidData Deserialize(InBufferType in_data_view) {
    ara::log::LogDebug() << __func__;
    // check precondition on size
    if (in_data_view.size() != kDidSize) {
      throw std::length_error("UdsByteView of size " + std::to_string(in_data_view.size()) +
                              " does not match required size (=" + std::to_string(kDidSize) + ").");
    }
    // Deserialization of dataRecord_DataELement_one (uint8)
    uint8 dataRecord_DataElement_one = in_data_view.at(0);
    // Deserialization of dataRecord_DataELement_two (uint16)
    uint16 dataRecord_DataElement_two = 0;
    amsr::diag::common::SetByte(dataRecord_DataElement_two, in_data_view.at(2), 0);
    amsr::diag::common::SetByte(dataRecord_DataElement_two, in_data_view.at(1), 1);
    return std::make_tuple(dataRecord_DataElement_one, dataRecord_DataElement_two);
  }
};

}  // namespace serializer
}  // namespace data_identifier
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_FUTURE_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_DATAIDENTIFIER_SWCL_A_DID_4711_SERIALIZER_H_

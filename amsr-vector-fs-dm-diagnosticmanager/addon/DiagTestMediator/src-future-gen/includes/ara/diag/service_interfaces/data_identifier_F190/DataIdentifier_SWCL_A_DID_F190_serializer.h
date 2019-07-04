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
/**        \file  DataIdentifier_SWCL_A_DID_F190_serializer.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_FUTURE_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_DATAIDENTIFIER_SWCL_A_DID_F190_SERIALIZER_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_FUTURE_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_DATAIDENTIFIER_SWCL_A_DID_F190_SERIALIZER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <tuple>

#include "ara/diag/service_interfaces/data_identifier_F190/DataIdentifier_SWCL_A_DID_F190.h"
#include "common/multi_byte_type.h"

#include "ara/log/logging.hpp"
#include "vac/container/array_view.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace data_identifier_F190 {
namespace serializer {

/**
 * \brief Serializer class for Proxy DataIdentifier_SWCL_A_DID_F190
 */
class DataIdentifier_SWCL_A_DID_F190Serializer {
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
  using TupleDidData =
      std::tuple<ara::diag::service_interfaces::data_identifier_F190::fixed_size_array_with_17_uint8_items>;

  /**
   * \brief Buffer type
   */
  using OutBufferType = vac::container::array_view<std::uint8_t>;

  /**
   * \brief Buffer type
   */
  using InBufferType = vac::container::const_array_view<std::uint8_t>;

  static constexpr std::size_t kDidDataSize = 17;

  /**
   * \brief Returns the size of the DID data.
   * \returns the size of the DID data (17 bytes).
   */
  static std::size_t GetDidDataSize() { return kDidDataSize; }

  /**
   * \brief Serializes typed DID data to UDS byte view.
   *
   * \param in_data_ref The typed data to be serialized.
   * \param out_data_view The view onto the buffer the serialization is written to.
   */
  static void Serialize(const ReadOutput& in_data_ref, OutBufferType out_data_view) {
    ara::log::LogDebug() << "DataIdentifier_SWCL_A_DID_F190Serializer::" << __func__ << " >> input size: '"
                         << std::to_string(in_data_ref.dataRecord_vin.size()) << "', output size: '"
                         << std::to_string(out_data_view.size()) << "'";
    // check precondition on size
    if (out_data_view.size() < kDidDataSize) {
      throw std::length_error("UdsByteView of size " + std::to_string(out_data_view.size()) +
                              " can not be used to store the received data (=" + std::to_string(kDidDataSize) + ").");
    }

    static_assert(in_data_ref.dataRecord_vin.size() == kDidDataSize, "in_data_ref.dataRecord_vin has unexpected size!");

    // Serialization of dataRecord_vin (17 bytes)
    std::copy_n(in_data_ref.dataRecord_vin.begin(), kDidDataSize, out_data_view.begin());
  }

  /**
   * \brief Deserializes UDS byte view to typed DID data.
   *
   * \param in_data_view The UDS serialized data to be deserialized.
   * \return The tuple of the deserialized data.
   */
  static TupleDidData Deserialize(InBufferType in_data_view) {
    ara::log::LogDebug() << "DataIdentifier_SWCL_A_DID_F190Serializer::" << __func__ << ">> input size: '"
                         << std::to_string(in_data_view.size()) << "'";

    // check precondition on size
    if (in_data_view.size() != kDidDataSize) {
      throw std::length_error("UdsByteView of size " + std::to_string(in_data_view.size()) +
                              " does not match required size (=" + std::to_string(kDidDataSize) + ").");
    }

    ara::diag::service_interfaces::data_identifier_F190::fixed_size_array_with_17_uint8_items result;
    static_assert(result.size() == kDidDataSize, "Size of result has to be equal to kDidDataSize!");

    // Deserialization of dataRecord_vin (17 bytes)
    std::copy_n(in_data_view.begin(), kDidDataSize, result.begin());

    return std::make_tuple(result);
  }
};

}  // namespace serializer
}  // namespace data_identifier_F190
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara

#endif  // ADDON_DIAGTESTMEDIATOR_SRC_FUTURE_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_DATA_IDENTIFIER_F190_DATAIDENTIFIER_SWCL_A_DID_F190_SERIALIZER_H_

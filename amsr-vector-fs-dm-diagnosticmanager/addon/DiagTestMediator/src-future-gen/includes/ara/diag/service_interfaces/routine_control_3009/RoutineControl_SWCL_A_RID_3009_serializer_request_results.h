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
/**        \file  RoutineControl_SWCL_A_RID_3003_serializer_request_results.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_FUTURE_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_SERIALIZER_REQUEST_RESULTS_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_FUTURE_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_SERIALIZER_REQUEST_RESULTS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <tuple>

#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009.h"
#include "ara/log/logging.hpp"
#include "common/multi_byte_type.h"
#include "vac/container/array_view.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace routine_control_3009 {
namespace serializer {

/**
 * \brief Class for de/serialization of routine control request_results method.
 */
class RoutineControl_SWCL_A_RID_3009SerializerRequestResults {
 public:
  /**
   * \brief View on serializes RID data.
   */
  using UdsByteView = vac::container::array_view<std::uint8_t>;

  /**
   * \brief View on const serializes RID data.
   */
  using UdsConstByteView = vac::container::const_array_view<std::uint8_t>;

  /**
   * \brief Type of RequestResults output.
   */
  using RequestResultsOutput = proxy::methods::RequestResults::Output;

  /**
   * \brief Type of Input for RequestResults method, stored in a tuple
   */
  using RequestResultsTupleRidData = std::tuple<>;

  /**
   * \brief Size of RequestResults input.
   */
  static constexpr std::size_t kRidSizeRequestResultsInput = 0;

  /**
   * \brief Size of RequestResults output.
   */
  static constexpr std::size_t kRidSizeRequestResultsOutput = 4;

  /**
   * \brief Returns the size of the input data.
   * \returns the size of the input data.
   */
  static std::size_t GetInputDataSize() { return kRidSizeRequestResultsInput; }

  /**
   * \brief Returns the size of the output data.
   * \returns the size of the output data.
   */
  static std::size_t GetOutputDataSize() { return kRidSizeRequestResultsOutput; }

  /**
   * \brief Serializes typed RID data to UDS byte view.
   *
   * \param in_data_ref The typed data to be serialized.
   * \param out_data_view The view onto the buffer the serialization is written to.
   */
  static void Serialize(const RequestResultsOutput& in_data_ref, UdsByteView out_data_view) {
    ara::log::LogDebug() << "RoutineControl_SWCL_A_RID_3009SerializerRequestResults::" << __func__;
    // check precondition on size
    if (out_data_view.size() != kRidSizeRequestResultsOutput) {
      throw std::length_error(
          "RoutineControl_SWCL_A_RID_3009SerializerRequestResults::Serialize: UdsByteView of size " +
          std::to_string(out_data_view.size()) + " does not match required size (=" +
          std::to_string(kRidSizeRequestResultsOutput) + ").");
    }

    // Serialize counter value.
    out_data_view[0] = amsr::diag::common::GetByte(in_data_ref.Resp_RemainingCounts, 3);
    out_data_view[1] = amsr::diag::common::GetByte(in_data_ref.Resp_RemainingCounts, 2);
    out_data_view[2] = amsr::diag::common::GetByte(in_data_ref.Resp_RemainingCounts, 1);
    out_data_view[3] = amsr::diag::common::GetByte(in_data_ref.Resp_RemainingCounts, 0);
  }

  /**
   * \brief Deserializes UDS byte view to typed RID data.
   *
   * \param in_data_view The UDS serialized data to be deserialized.
   * \return The tuple of the deserialized data.
   */
  static RequestResultsTupleRidData Deserialize(UdsConstByteView in_data_view) {
    ara::log::LogDebug() << "RoutineControl_SWCL_A_RID_3009SerializerRequestResults::" << __func__;
    // check precondition on size
    if (in_data_view.size() != kRidSizeRequestResultsInput) {
      throw std::length_error(
          "RoutineControl_SWCL_A_RID_3009SerializerRequestResults::Deserialize: UdsByteView of size " +
          std::to_string(in_data_view.size()) + " does not match required size (=" +
          std::to_string(kRidSizeRequestResultsInput) + ").");
    }

    // nothing to be deserialized.
    return std::make_tuple();
  }
};

}  // namespace serializer
}  // namespace routine_control_3009
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
#endif  // ADDON_DIAGTESTMEDIATOR_SRC_FUTURE_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_SERIALIZER_REQUEST_RESULTS_H_

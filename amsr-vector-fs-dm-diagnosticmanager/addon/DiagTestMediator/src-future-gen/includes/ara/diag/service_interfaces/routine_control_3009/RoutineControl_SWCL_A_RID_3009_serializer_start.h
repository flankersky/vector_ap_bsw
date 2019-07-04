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
/**        \file  RoutineControl_SWCL_A_RID_3003_serializer_start.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef ADDON_DIAGTESTMEDIATOR_SRC_FUTURE_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_SERIALIZER_START_H_
#define ADDON_DIAGTESTMEDIATOR_SRC_FUTURE_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_SERIALIZER_START_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <tuple>

#include "ara/diag/service_interfaces/routine_control_3009/RoutineControl_SWCL_A_RID_3009.h"
#include "common/multi_byte_type.h"

#include "ara/log/logging.hpp"
#include "vac/container/array_view.h"

namespace ara {
namespace diag {
namespace service_interfaces {
namespace routine_control_3009 {
namespace serializer {

/**
 * \brief Class for de/serialization of routine control start method.
 */
class RoutineControl_SWCL_A_RID_3009SerializerStart {
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
   * \brief Type of Start output.
   */
  using StartOutput = proxy::methods::Start::Output;

  /**
   * \brief Type of Input for Start method, stored in a tuple
   */
  using StartTupleRidData = std::tuple<routine_control_3009::boolean, routine_control_3009::uint32>;

  /**
   * \brief Size of Start input.
   */
  static constexpr std::size_t kRidSizeStartInput = 5;

  /**
   * \brief Size of Start output.
   */
  static constexpr std::size_t kRidSizeStartOutput = 0;

  /**
   * \brief Returns the size of the input data.
   * \returns the size of the input data.
   */
  static std::size_t GetInputDataSize() { return kRidSizeStartInput; }

  /**
   * \brief Returns the size of the output data.
   * \returns the size of the output data.
   */
  static std::size_t GetOutputDataSize() { return kRidSizeStartOutput; }

  /**
   * \brief Serializes typed RID data to UDS byte view.
   *
   * \param in_data_ref The typed data to be serialized.
   * \param out_data_view The view onto the buffer the serialization is written to.
   */
  static void Serialize(const StartOutput& in_data_ref, UdsByteView out_data_view) {
    ara::log::LogDebug() << "RoutineControl_SWCL_A_RID_3009SerializerStart::" << __func__;
    // check precondition on size
    if (out_data_view.size() != kRidSizeStartOutput) {
      throw std::length_error("RoutineControl_SWCL_A_RID_3009SerializerStart::Serialize: UdsByteView of size " +
                              std::to_string(out_data_view.size()) + " does not match required size (=" +
                              std::to_string(kRidSizeStartOutput) + ").");
    }

    // Nothing has to be serialized.
  }

  /**
   * \brief Deserializes UDS byte view to typed RID data.
   *
   * \param in_data_view The UDS serialized data to be deserialized.
   * \return The tuple of the deserialized data.
   */
  static StartTupleRidData Deserialize(UdsConstByteView in_data_view) {
    ara::log::LogDebug() << "RoutineControl_SWCL_A_RID_3009SerializerStart::" << __func__;
    // check precondition on size
    if (in_data_view.size() != kRidSizeStartInput) {
      throw std::length_error("RoutineControl_SWCL_A_RID_3009SerializerStart::Deserialize: UdsByteView of size " +
                              std::to_string(in_data_view.size()) + " does not match required size (=" +
                              std::to_string(kRidSizeStartInput) + ").");
    }

    // Deserialization of reset parameter.
    routine_control_3009::boolean req_reset = (in_data_view[0] != 0x00);

    // Deserialization of counter limit parameter.
    routine_control_3009::uint32 req_counter_limit;
    amsr::diag::common::SetByte(req_counter_limit, in_data_view.at(4), 0);
    amsr::diag::common::SetByte(req_counter_limit, in_data_view.at(3), 1);
    amsr::diag::common::SetByte(req_counter_limit, in_data_view.at(2), 2);
    amsr::diag::common::SetByte(req_counter_limit, in_data_view.at(1), 3);

    return std::make_tuple(req_reset, req_counter_limit);
  }
};

}  // namespace serializer
}  // namespace routine_control_3009
}  // namespace service_interfaces
}  // namespace diag
}  // namespace ara
#endif  // ADDON_DIAGTESTMEDIATOR_SRC_FUTURE_GEN_INCLUDES_ARA_DIAG_SERVICE_INTERFACES_ROUTINE_CONTROL_3009_ROUTINECONTROL_SWCL_A_RID_3009_SERIALIZER_START_H_

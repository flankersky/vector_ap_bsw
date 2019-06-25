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
/**        \file  transformer.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_E2E_E2EXF_TRANSFORMER_H_
#define LIB_LIBARA_INCLUDE_ARA_E2E_E2EXF_TRANSFORMER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <vector>
#include "ara/e2e/e2e_types.h"
#include "ara/e2e/e2exf/e2e_result.h"
#include "ara/e2e/e2exf/state_machine.h"
#include "ara/e2e/profiles/profiles.h"

namespace ara {
namespace e2e {
namespace e2exf {

/**
 * \brief Blueprint for the default configuration of the used profile.
 */
template <typename ProfileCfg, typename E2EProtectionPropsCfg = End2EndEventProtectionProps<>,
          typename E2EProfileCfg = void>
struct E2ExfConfiguration {
  /**
   * \brief Profile to be configured and used by the transformer.
   */
  using Profile = ProfileCfg;

  /**
   * \brief E2EProtectionProperties given to the profile.
   */
  using End2EndEventProtectionProps = E2EProtectionPropsCfg;

  /**
   * \brief E2EProfileProps for the state machine and the check.
   */
  using E2EProfileProps = E2EProfileCfg;
};

/**
 * \brief Class for the E2E transformation process of a given byte stream. This will derive
 * from the given profile parametrized in E2EXfConfiguration to be able to access the Protect
 * and Check sequence from the profile.
 *
 * \tparam E2EXfConfiguration Holds all statically configured parameters for a given E2E profile
 * and the profile in use.
 */
template <typename E2EXfConfiguration, typename StreamType = std::vector<std::uint8_t>>
class Transformer : public E2EXfConfiguration::Profile {
 public:
  /**
   * \brief Profile configured as type-alias
   */
  using TransformerProfile = typename E2EXfConfiguration::Profile;

  /**
   * \brief The base is the profile to call on a Protect / Check call.
   */
  using Base = TransformerProfile;

  /**
   * \brief Protect a certain buffer based on the configured profile.
   *
   * \param buffer Holds the byte stream to verify. This includes the pre-allocated E2EHeader
   * for the given profile and the user-data.
   * \param non_protected_offset The offset in the buffer to not protect.
   * \return Based on the profile given a profile might give back additional information to its caller.
   */
  typename TransformerProfile::ProtectReturnType Protect(StreamType& buffer,
                                                         const std::uint8_t non_protected_offset = 0U) {
    using BufferView = vac::container::array_view<std::uint8_t>;

    // First check if the non-protected offset does not exceed the overall buffer size.
    if (non_protected_offset <= buffer.size()) {
      std::uint8_t* protect_begin = &buffer[0] + non_protected_offset;
      const auto protected_size = buffer.size() - static_cast<decltype(buffer.size())>(non_protected_offset);
      BufferView buffer_view(protect_begin, protected_size);
      return this->Base::Protect(typename E2EXfConfiguration::End2EndEventProtectionProps{}, buffer_view);
    } else {
      return Base::ProtectReturnType::kWrongInput;
    }
  }

  /**
   * \brief Executes an E2E check on a given stream, including the
   * E2E header and the protected serialized payload. This will only be called for checkers.
   * If the transformer is configured for a protector and this method gets called, it will
   * lead to a compile-error.
   *
   * \uptrace SWS_E2E_00355
   * \param buffer Holds the byte stream to do an E2E check for. This buffer includes the pre-allocated E2EHeader
   * for the given profile and the user-data.
   * \param non_checked_offset The offset in bytes before the check.
   * \return The result of this E2E check.
   */
  ara::e2e::e2exf::Result Check(const StreamType& buffer, const std::uint8_t non_checked_offset = 0U) noexcept {
    struct MergedConfig : public E2EXfConfiguration::End2EndEventProtectionProps,
                          public E2EXfConfiguration::E2EProfileProps {};

    using BufferView = vac::container::const_array_view<const std::uint8_t>;

    // First check if the non-checked offset does not exceed the overall buffer size.
    if (non_checked_offset <= buffer.size()) {
      const std::uint8_t* check_begin = &buffer[0] + non_checked_offset;
      const auto checked_size = buffer.size() - static_cast<decltype(buffer.size())>(non_checked_offset);
      BufferView buffer_view(check_begin, checked_size);
      const auto profile_specific_check_status = this->Base::Check(MergedConfig{}, buffer_view);
      ara::e2e::state_machine::CheckStatus check_status = MapToAraCheckStatus(profile_specific_check_status);
      ara::e2e::state_machine::E2EState state = e2e_state_machine_.Check(check_status);
      return ara::e2e::e2exf::Result{state, check_status};
    } else {
      return ara::e2e::e2exf::Result{ara::e2e::state_machine::E2EState::Invalid,
                                     ara::e2e::state_machine::CheckStatus::Error};
    }
  }

 private:
  /**
   * \brief Maps the profile-specific check status from the AUTOSAR classic SWS
   * into the AUTOSAR adaptive one.
   *
   * \tparam ProfileCheckStatus
   * \param profile_check_status Check status from the profile to convert.
   * \return The check status in ara::e2e representation.
   */
  template <typename ProfileCheckStatus>
  ara::e2e::state_machine::CheckStatus MapToAraCheckStatus(const ProfileCheckStatus& profile_check_status) const
      noexcept {
    ara::e2e::state_machine::CheckStatus check_status{};

    switch (profile_check_status) {
      // Fall-through
      case ProfileCheckStatus::kOk:
      case ProfileCheckStatus::kOkSomeLost: {
        check_status = ara::e2e::state_machine::CheckStatus::Ok;
        break;
      }
      case ProfileCheckStatus::kStatusError: {
        check_status = ara::e2e::state_machine::CheckStatus::Error;
        break;
      }
      case ProfileCheckStatus::kStatusRepeated: {
        check_status = ara::e2e::state_machine::CheckStatus::Repeated;
        break;
      }
      case ProfileCheckStatus::kWrongSequence: {
        check_status = ara::e2e::state_machine::CheckStatus::WrongSequence;
        break;
      }
      case ProfileCheckStatus::kNoNewData: {
        check_status = ara::e2e::state_machine::CheckStatus::NoNewData;
        break;
      }
      default: {
        assert(false && "Unspecified ProfileCheckStatus!");
        break;
      }
    }

    return check_status;
  }

  /**
   * \brief E2E state machine
   */
  StateMachine<E2EXfConfiguration> e2e_state_machine_;
};

}  // namespace e2exf
}  // namespace e2e
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_E2E_E2EXF_TRANSFORMER_H_

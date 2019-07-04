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
/**
 *      \file  session_info.h
 *      \brief  Contains the SessionInfo type
 *
 *      \details  This file contains the definition of the SessionInfo type.
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_SESSION_INFO_H_
#define SRC_SERVER_SESSION_INFO_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace amsr {
namespace diag {
namespace server {

/**
 * \brief Contains configuration information of a Session.
 */
struct SessionInfo {
  /**
   * \brief Overloaded Constructor.
   * \param sessionId The session identifier (0x01: defaultSession, 0x02: programmingSession, ...)
   * \param p2Time P2 timeout in milliseconds
   * \param p2StarTime P2* timeout in milliseconds
   */
  SessionInfo(std::uint8_t sessionId, std::chrono::milliseconds p2Time, std::chrono::milliseconds p2StarTime)
      : session_id(sessionId), p2_time(p2Time), p2_star_time(p2StarTime) {}

  SessionInfo(const SessionInfo& other) = default;       ///< Default copy Constructor
  SessionInfo(SessionInfo&& other) = default;            ///< Default move  Constructor
  SessionInfo& operator=(const SessionInfo&) = default;  ///< Default copy assignment operator
  SessionInfo& operator=(SessionInfo&&) = default;       ///< Default move assignment operator

  /**
   * \brief Session id.
   */
  std::uint8_t session_id;

  /**
   * \brief Duration of P2 in ms.
   */
  std::chrono::milliseconds p2_time;

  /**
   * \brief Duration of P2 star in ms.
   */
  std::chrono::milliseconds p2_star_time;

  /**
   * \brief The P2* (P2star) default value.
   * 50 milliseconds is the default value used, when no SessionInfo was configured.
   */
  static constexpr std::uint16_t kP2DefaultTime{50};

  /**
   * \brief The P2* (P2star) default value.
   * 5000 milliseconds is the default value used, when no SessionInfo was configured.
   * This corresponds to the value 0x014F in the $10 response, which uses a resolution of 10 milliseconds.
   */
  static constexpr std::uint16_t kP2StartDefaultTime{5000};

  /**
   * \brief The S3 value.
   * 5000 milliseconds is the value used as per the ISO 14229-2 v2013 (section 7.5, table 5).
   */
  static constexpr std::uint16_t kS3Time{5000};

  /**
   * \brief Creates a SessionInfo by using timeout provided as std::uint16_t values in the corrrect resolution.
   * \param sessionId The session identifier (0x01: defaultSession, 0x02: programmingSession, ...)
   * \param p2Time P2 timeout in milliseconds
   * \param p2StarTime P2* timeout with a resolution of 10 milliseconds
   * \return A SessionInfo initialized with the provided values
   */
  static SessionInfo Create(std::uint8_t sessionId, std::uint16_t p2Time, std::uint16_t p2StarTime) {
    std::chrono::milliseconds p2_time_in_ms{p2Time};
    std::chrono::milliseconds p2_star_time_in_ms{p2StarTime * 10};
    return SessionInfo{sessionId, p2_time_in_ms, p2_star_time_in_ms};
  }
};

}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_SESSION_INFO_H_

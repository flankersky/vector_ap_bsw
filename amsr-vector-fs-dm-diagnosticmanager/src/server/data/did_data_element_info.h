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
/**        \file  did_data_element_info.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_DATA_DID_DATA_ELEMENT_INFO_H_
#define SRC_SERVER_DATA_DID_DATA_ELEMENT_INFO_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>

#include "configuration/dext_configuration.h"
#include "server/service/handler/service_handler.h"

namespace amsr {
namespace diag {
namespace server {
namespace data {
/**
 * \brief Implementation of DidDataElementInfo Class.
 */
class DidDataElementInfo {
 public:
  /**
   * \brief Definition of kInternalStr constant to "Internal".
   */
  static constexpr char kInternalStr[] = "Internal";

  /**
   * \brief Definition of kExternalStr constant to "External".
   */
  static constexpr char kExternalStr[] = "External";

  /**
   * \brief Ctor.
   */
  explicit DidDataElementInfo(const configuration::DataElementConfiguration& data_elem_conf);

  /**
   * \brief Return the handler type.
   */
  service::handler::ServiceHandler::HandlerType GetHandlerType() const { return handler_type_; }

  // Remove copy/move ctor/assignment.
  DidDataElementInfo(const DidDataElementInfo&) = delete;
  DidDataElementInfo(const DidDataElementInfo&&) = delete;
  DidDataElementInfo& operator=(const DidDataElementInfo&) = delete;
  DidDataElementInfo& operator=(const DidDataElementInfo&&) = delete;

  /**
   * \brief Return the maximal size of the data elements.
   * \return std:size_t max size of all data elements
   */
  std::size_t GetMaxSize() const { return max_length_; }

  /**
   * \brief Return the minimal size of the data elements.
   * \return std:size_t min size of all data elements
   */
  std::size_t GetMinSize() const { return min_length_; }

 private:
  /**
   * \brief Get the handler type from string.
   * \param handler_info handler configuration.
   */
  service::handler::ServiceHandler::HandlerType GetHandlerTypeFromString(
      const amsr::diag::configuration::HandlerInfo& handler_info);

  /**
   * \brief min length
   */
  std::uint8_t min_length_;

  /**
   * \brief max length
   */
  std::uint8_t max_length_;

  /**
   * \brief handler type.
   */
  service::handler::ServiceHandler::HandlerType handler_type_;
};
}  // namespace data
}  // namespace server
}  // namespace diag
}  // namespace amsr

#endif  // SRC_SERVER_DATA_DID_DATA_ELEMENT_INFO_H_

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
/**        \file  instance_identifier.h
 *        \brief  Identifier of a certain instance of a service.
 *
 *      \details  The ara::com::InstanceIdentifier is needed to distinguish different instances of the same service.
 *          \see  SWS_CM_00302
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INSTANCE_IDENTIFIER_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INSTANCE_IDENTIFIER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <string>
#include <utility>

#include "ara/com/internal/types.h"

namespace ara {
namespace com {

/**
 * \brief InstanceIdentifier
 */
class InstanceIdentifier {
 public:
  /**
   * \brief InstanceIdentifier that represents all instances
   */
  static const InstanceIdentifier Any;
  /**
   * \brief Constructor
   * \param v Value that is used as instance id
   */
  explicit InstanceIdentifier(const std::string& v) : instance_id_(ParseString(v)) {}
  /**
   * \brief Constructor
   * \param iid numeric instance id
   */
  explicit InstanceIdentifier(ara::com::InstanceId iid) : instance_id_(iid) {}
  /**
   * \brief Get string representation of InstanceIdentifier
   * \return String representation of InstanceIdentifier
   */
  std::string toString() const;
  /**
   *  \brief Returns a numeric representation of the instance id
   */
  ara::com::InstanceId GetInstanceId() const { return instance_id_; }
  /**
   * \brief Overloads == operator
   * \param other InstanceIdentifier to compare
   * \return True if equal else false
   */
  bool operator==(const InstanceIdentifier& other) const { return instance_id_ == other.instance_id_; }
  /**
   * \brief Overloads < operator
   * \param other InstanceIdentifier to compare
   * \return True if *this < other else false
   */
  bool operator<(const InstanceIdentifier& other) const { return instance_id_ < other.instance_id_; }

 private:
  // Parses a string representing a 16 bit integer value. If empty, returns ANY.
  static ara::com::InstanceId ParseString(const std::string& label);
  /// Numeric representation of an instance
  ara::com::InstanceId instance_id_;
};

}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INSTANCE_IDENTIFIER_H_

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
/**        \file  thread_pool_assignment_config.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_CONFIGURATION_THREAD_POOL_ASSIGNMENT_CONFIG_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_CONFIGURATION_THREAD_POOL_ASSIGNMENT_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>
#include "ara/com/instance_identifier.h"

namespace ara {
namespace com {
namespace configuration {

/**
 * \brief Thread pool assignment configuration. This maps concrete service interface instances to the related thread
 * pool.
 */
class ThreadPoolAssignmentConfig {
 public:
  /**
   * \brief Type alias for assignment name
   */
  using AssignmentName = std::string;

  /**
   * \brief Type alias for instance identifier
   */
  using InstanceIdentifierValue = std::string;

  /**
   * \brief Constructor of ThreadPoolAssignmentConfig.
   * \param name Thread pool assignment name, the service interface class name
   * \param instance_identifier Thread pool assignment instance identifier
   */
  ThreadPoolAssignmentConfig(const AssignmentName& name, const InstanceIdentifierValue& instance_identifier)
      : name_(name), instance_identifier_(InstanceIdentifier{instance_identifier}) {}

  /**
   * \brief Destructor
   */
  virtual ~ThreadPoolAssignmentConfig() = default;

  /**
   * \brief Get thread pool assignment name
   * \return Assignment name,.the service interface class name
   */
  const AssignmentName& GetName() const noexcept { return name_; }

  /**
   * \brief Get thread pool assignment instance identifier
   * \return Assignment instance identifier
   */
  const InstanceIdentifier& GetInstanceIdentifier() const noexcept { return instance_identifier_; }

 private:
  /**
   * \brief Assignment name (service interface class name)
   */
  AssignmentName name_;

  /**
   * \brief Assignment instance identifier (service interface instance)
   */
  InstanceIdentifier instance_identifier_;
};

}  // namespace configuration
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_CONFIGURATION_THREAD_POOL_ASSIGNMENT_CONFIG_H_

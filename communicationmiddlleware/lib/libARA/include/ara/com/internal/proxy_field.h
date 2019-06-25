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
/**        \file  proxy_field.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_PROXY_FIELD_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_PROXY_FIELD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/com/internal/field_config.h"
#include "ara/com/internal/method.h"
#include "ara/com/internal/proxy_event.h"

namespace ara {
namespace com {
namespace internal {

/**
 * \brief Generic template class which can be used to create fields for a proxy.
 *
 * \tparam FieldType Represents the data type of the field.
 * \tparam FieldConfig Is a model of the configuration in the arxml with info about
 * hasSetter, hasGetter and hasNotifier.
 *
 * Example usage:
 *
 * ```
 * using MyFieldConfig = FieldParameters<ProxyImplInterface, NotifierConfig, GetterConfig, SetterConfig>;
 * ```
 */
template <typename FieldType, typename FieldConfig>
class ProxyField : public FieldConfig::NotificationType {
 public:
  /**
   * \brief Take the interface from the configuration for this field.
   */
  using ProxyImplInterfacePtr = typename FieldConfig::ImplInterfacePtr;

  /**
   * \brief The configuration template-class will take care of this and either
   * inserts a concrete event or a null-type, if hasNotifier is set to false.
   */
  using Notification = typename FieldConfig::NotificationType;

  /**
   * \brief Default constructor sets the pointer to the interface.
   *
   * \param interface A pointer to the interface of a proxy instance.
   */
  explicit ProxyField(ProxyImplInterfacePtr interface) : Notification{interface}, get_{interface}, set_{interface} {}

  /**
   * \brief The getter allows to request the actual value of the service provider.
   *
   * \return the current field value wrapped in a future object.
   */
  template <typename Config = FieldConfig>
  typename std::enable_if<Config::HasGetter, ara::com::Future<FieldType>>::type Get() {
    return get_();
  }

  /**
   * \brief Request to modify the field value. It's up to the service provider to accept the modification request.
   *
   * \param value the value for a property.
   *
   * \return the current value of the field.
   */
  template <typename Config = FieldConfig>
  typename std::enable_if<Config::HasSetter, ara::com::Future<FieldType>>::type Set(FieldType& value) {
    return set_(value);
  }

 private:
  /**
   * \brief Getter method of this field as an object.
   */
  typename FieldConfig::GetterMethodType get_;

  /**
   * \brief Setter method of this field as an object.
   */
  typename FieldConfig::SetterMethodType set_;
};

}  // namespace internal
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_PROXY_FIELD_H_

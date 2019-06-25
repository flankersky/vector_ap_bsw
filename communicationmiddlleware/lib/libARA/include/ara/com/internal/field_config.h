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
/**        \file  field_config.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_FIELD_CONFIG_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_FIELD_CONFIG_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace ara {
namespace com {
namespace internal {

/**
 * \brief Parametrize hasNotifier option of a field.
 * \tparam Option Set this template-parameter to true, if this value is set to true
 * in the arxml - false if not.
 * \tparam Event If the notifier is set to true, provide an Event structure. If
 * no notification is active, it defaults to a default template-parameter of void.
 *
 * \note This is a handy way to extend the template-parameter-list.
 * If Option is set to true, the user must ensure to provide a struct like this:
 *
 * ```
 * HasNotifier<true, Event<ProxyImplInterface, std::uint32_t>>
 * ```
 *
 * If no notifier is necessary, the user is possible to set the type like this:
 *
 * ```
 * HasNotifier<false>
 * ```
 *
 * without the need to give a second template-parameter, because
 * it defaults to void.
 */
template <bool Option, typename Event = typename std::enable_if<!Option, void>::type>
struct HasNotifier {
  /**
   * \brief This enum is used to store parameters set in the template parameters.
   * One parameter is active which contains the value of hasNotifier from the generator model.
   */
  enum { active = Option };
  /**
   * \brief Either this is the concrete Event type or void, if hasNotifier is set to false.
   */
  using type = Event;
};

/**
 * \brief Parametrize hasGetter option of a field.
 *
 * \tparam Option Set this template-parameter to true, if this value is set to true
 * in the arxml - false if not.
 * \tparam GetMethod If the getter is set to true, provide a method structure. If
 * getter isn't active, it defaults to a default template-parameter of `void`.
 *
 * \note This is a handy way to extend the template-parameter-list.
 * If Option is set to true, the user must ensure to provide a struct like this:
 *
 * ```
 * HasGetter<true, ProxyMethod<ProxyImplInterface, ...>
 * ```
 *
 * If no notifier is necessary, it is sufficient to use:
 *
 * ```
 * HasGetter<false>
 * ```
 *
 * without the need to give a second template-parameter, because
 * it defaults to void.
 */
template <bool Option, typename GetMethod = typename std::enable_if<!Option, void>::type>
struct HasGetter {
  /**
   * \brief This enum is used to store parameters set in the template parameters.
   * One parameter is active, which contains the value of hasGetter from the generator model.
   */
  enum { active = Option };

  /**
   * \brief Type-alias to forward the generated type for the get method.
   * Either this is a concrete Method type definition or void, if hasGetter is set to false in
   * the generator model.
   */
  using type = GetMethod;
};

/**
 * \brief Parametrize hasSetter option of a field.
 *
 * \tparam Option Set this template-parameter to true, if this value is set to true
 * in the arxml - false if not.
 * \tparam SetMethod If the setter is set to true, provide a method structure. If
 * getter isn't active, it defaults to a default template-parameter of `void`.
 *
 * \note This is a handy way to extend the template-parameter-list.
 * If Option is set to true, the user must ensure to provide a struct like this:
 *
 * ```
 * HasSetter<true, ProxyMethod<ProxyImplInterface, ...>
 * ```
 *
 * If no notifier is necessary, it is sufficient to parametrize in the following
 * readable way:
 *
 * ```
 * HasSetter<false>
 * ```
 *
 * without the need to give a second template-parameter, because
 * it defaults to void.
 */
template <bool Option, typename SetMethod = typename std::enable_if<!Option, void>::type>
struct HasSetter {
  /**
   * \brief This enum is used to store parameters set in the template parameters.
   */
  enum { active = Option };

  /**
   * \brief Type-alias to forward the generated type for the set method.
   * Either this is a concrete Method type definition or void, if hasSetter is set to false in
   * the generator model.
   */
  using type = SetMethod;
};

/**
 * \brief Parameter-set for one field
 *
 * \tparam ImplInterfaceType The implementation interface the frontend will talk to.
 * \tparam Notification The configurated notification type (contains info if hasNotifier is set to
 * true / false)
 * \tparam Getter The configured getter type (contains info if hasGetter is set to
 * true / false)
 * \tparam Setter The configured setter type (contains info if hasSetter is set to
 * true / false)
 *
 * Example usage:
 *
 * ```
 * using NotifierConfig = HasNotifier<true, Event<ProxyImplInterface, std::uint32_t>>;
 * using GetterConfig = HasGetter<false>;
 * using SetterConfig = HasSetter<false>;
 * using MyFieldConfig = FieldParameters<ProxyImplInterface, NotifierConfig, GetterConfig, SetterConfig>;
 * ```
 */
template <typename ImplInterfaceType, typename Notification, typename Getter, typename Setter>
struct FieldParameters {
  /**
   * \brief Type-alias for a pointer to the implementation interface , which is
   * needed for notifier, getter and setter respectively.
   */
  using ImplInterfacePtr = ImplInterfaceType*;

  /**
   * \brief Store the switches from the template parameterization to be able to
   * use it in other structures.
   */
  enum { HasNotification = Notification::active, HasGetter = Getter::active, HasSetter = Setter::active };

  /**
   * \brief This is used if an option (hasGetter, hasSetter, ...) is set to false.
   * Provides a way to initialize the members with some empty structure in the field template class,
   * because we can not enable_if member initialization in the constructor.
   */
  class OptionalInitializer {
   public:
    /**
     * \brief Constructor is used to call it in the constructor of the field itself for bypassing
     * if an option is not active.
     */
    explicit OptionalInitializer(ImplInterfacePtr interface) {
      // The ptr isn't used here, because no event notification.
      (void)interface;
    }
  };

  /**
   * \brief With the std::conditional we choose between a concrete Event or a "null" type for initialization.
   */
  using NotificationType =
      typename std::conditional<HasNotification, typename Notification::type, OptionalInitializer>::type;

  /**
   * \brief With the std::conditional we choose between a concrete getter method type or a "null" type for
   * initialization.
   */
  using GetterMethodType = typename std::conditional<HasGetter, typename Getter::type, OptionalInitializer>::type;

  /**
   * \brief With the std::conditional we choose between a concrete setter method or a "null" type for initialization.
   */
  using SetterMethodType = typename std::conditional<HasSetter, typename Setter::type, OptionalInitializer>::type;
};

}  // namespace internal
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_FIELD_CONFIG_H_

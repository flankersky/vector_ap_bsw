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
/**        \file  skeleton_field.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_SKELETON_FIELD_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_SKELETON_FIELD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cassert>
#include "ara/com/internal/field_config.h"
#include "ara/com/promise.h"

namespace ara {
namespace com {
namespace internal {

/**
 * \brief Generic template class for skeleton fields, that unifies common behavior.
 * This is more or less a copy of an event on skeleton-side. Fields have an additional
 * Update method on skeleton-side, which is an alias for the Send() method of events.
 *
 * \tparam FieldType Represents the data type of the field.
 * \tparam FieldConfig Is a model of the configuration from the arxml with info about
 * hasSetter, hasGetter and hasNotifier.
 */
template <typename FieldDataType, typename FieldConfig>
class SkeletonField : public FieldConfig::NotificationType {
 public:
  /**
   * \brief Type-alias for the data type of this field.
   */
  using FieldType = FieldDataType;

  /**
   * \brief Take the interface from the configuration for this field.
   */
  using SkeletonImplInterfacePtr = typename FieldConfig::ImplInterfacePtr;

  /**
   * \brief Short for base-class.
   */
  using Base = typename FieldConfig::NotificationType;

  // Inherit constructor
  using Base::Base;

  /**
   * \brief Because init_value could have 0..1 multiplicity according to the AR meta-model, we
   * provide a second constructor to set the init value.
   */
  SkeletonField(SkeletonImplInterfacePtr interface, const FieldType& init_value) : Base::Base(interface) {
    Update(init_value);
  }

  /**
   * \brief Apply a new value to this field.
   * Acts as the send method of the event and triggers the transmission of the notify
   * to the subscribed clients.
   * The field data has been allocated somewhere by the service application
   * developer and is given via reference to the binding implementation of Update.
   * After the call to Update returns, the data might be removed/altered on the caller side.
   * \uptrace SWS_CM_00119
   * \note In case of a configured Getter, this has to be called at least once to set the initial value.
   *
   * \param data A reference to the data allocated by the service implementer.
   */
  template <typename Config = FieldConfig>
  typename std::enable_if<Config::HasNotification, void>::type Update(const FieldType& data) {
    // The following call_once is necessary in case no GetHandler is set and we need to check if the user has called
    // update once for a possible getter call from the client.
    std::call_once(once_flag_, [&]() { field_update_once_ = true; });
    {
      // Protect concurrent read/write to field_data_ by Get() and Update() API
      std::lock_guard<std::mutex> guard(field_data_lock_);
      field_data_ = data;
    }
    Base::Send(data);
  }

  /**
   * \brief This is used if the field has no notifier.
   */
  template <typename Config = FieldConfig>
  typename std::enable_if<!Config::HasNotification, void>::type Update(const FieldType& data) {}

  /**
   * \brief Type-alias for the get handler callback. To the user it shall be a std::function
   * template, as it is defined in the ara::com SWS.
   * \uptrace SWS_CM_00114
   * \note GetHandler is fully optional.
   */
  using GetHandlerInterface = std::function<ara::com::Future<FieldType>()>;

  /**
   * \brief If registered the function is called whenever a get request is
   * received.
   * If no Getter is registered ara::com is responsible for responding to the request using the last value set
   * by update.
   * This implicitly requires at least one call to update after initialization of the Service, before the
   * service is offered.
   * \uptrace SWS_CM_00128
   * \note Registering a GetHandler is optional.
   *
   * \param get_handler the handler function which is called on a get from the client.
   */
  void RegisterGetHandler(GetHandlerInterface get_handler) {
    /* Protect concurrent read/write of get_handler_ which can occur reactor thread calls Get() method while application
     * calls RegisterGetHandler() */
    std::lock_guard<std::recursive_mutex> guard(get_handler_lock_);
    if (get_handler != nullptr) {
      get_handler_ = get_handler;
    }
  }

  /**
   * \brief Get the value of the field via get_handler_ interface. To the user it shall be a std::function
   * template, as it is defined in the ara::com SWS.
   * \uptrace SWS_CM_00128
   *
   * \tparam Config this parameter is used for enable_if
   *
   * \return A future object that contains the field value.
   */
  template <typename Config = FieldConfig>
  typename std::enable_if<Config::HasGetter, ara::com::Future<FieldType>>::type Get() {
    /* Protect concurrent read/write of get_handler_ which can occur reactor thread calls Get() method while
     * application calls RegisterGetHandler() */
    std::lock_guard<std::recursive_mutex> get_handler_guard(get_handler_lock_);

    // The get handler is optional, so we must check if the there is a getter registered.
    if (get_handler_) {
      // Do not use get_handler_ after call of handler. Due to recursive mutex the handler could replace itself.
      return get_handler_();
    } else {
      // If there is no getter registered, ara::com is responsible for responding to the
      // request using the last value set by update. Check if the update was called once before.
      if (field_update_once_) {
        // Protect concurrent read/write to field_data_ by Get() and Update() API
        std::lock_guard<std::mutex> guard(field_data_lock_);
        ara::com::Promise<FieldType> promise;
        promise.set_value(field_data_);
        return promise.get_future();
      } else {
        throw std::runtime_error(
            "No GetHandler registered and no initial value at the time of a get call for a field.");
      }
    }
  }

  /**
   * \brief Set the value of the field via set_handler_ interface.
   *
   * \tparam Config this parameter is used for enable_if
   * \param value The value to set for this field.
   *
   * \return a future object containing the mirrored value.
   */
  template <typename Config = FieldConfig>
  typename std::enable_if<Config::HasSetter, ara::com::Future<FieldType>>::type Set(const FieldType& value) {
    /* Protect concurrent read/write of set_handler_ which can occur reactor thread calls Set() method while
     * application calls RegisterSetHandler() */
    std::lock_guard<std::recursive_mutex> set_handler_guard(set_handler_lock_);

    // The set handler is optional, so we must check if a set handler is registered
    if (set_handler_) {
      // Do not use get_handler_ after call of handler. Due to recursive mutex the handler could replace itself.
      return set_handler_(value);
    } else {
      throw std::runtime_error(
          "SetHandler is mandatory, but no SetHandler is registered for this field on the time of a set request.");
    }
  }

  /**
   * \brief Type-alias for the set handler callback.
   * \uptrace SWS_CM_00116
   */
  using SetHandlerInterface = std::function<ara::com::Future<FieldType>(const FieldType& data)>;

  /**
   * \brief Registering a SetHandler is mandatory, if the field supports it.
   * \uptrace SWS_CM_00129
   *
   * \param set_handler The handler which is called on a set call from the client.
   *
   * \return The returned value is sent to the requester and is sent via notification to all subscribed entities.
   */
  void RegisterSetHandler(SetHandlerInterface set_handler) {
    /* Protect concurrent read/write of set_handler_ which can occur reactor thread calls Set() method while
     * application calls RegisterSetHandler() */
    std::lock_guard<std::recursive_mutex> guard(set_handler_lock_);
    if (set_handler != nullptr) {
      set_handler_ = set_handler;
    }
  }

 private:
  /**
   * Callback for a getter call.
   */
  GetHandlerInterface get_handler_{nullptr};

  /**
   * Mutex to protect concurrent modification of get_handler_.
   * Violation of 'HIC++ rule 18.3.3 Do not use std::recursive_mutex' currently accepted due to ara::com design.
   */
  mutable std::recursive_mutex get_handler_lock_;

  /**
   * Callback for a setter call.
   */
  SetHandlerInterface set_handler_{nullptr};

  /**
   * Mutex to protect concurrent modification of set_handler_
   * Violation of 'HIC++ rule 18.3.3 Do not use std::recursive_mutex' currently accepted due to ara::com design.
   */
  mutable std::recursive_mutex set_handler_lock_;

  /**
   * Used to set the field update flag only once.
   */
  std::once_flag once_flag_;

  /**
   * This flag is set to true, when the user called Update once.
   */
  bool field_update_once_{false};

  /**
   * The field on skeleton-side shall always have access to the latest value, which has been set via Update. This is
   * necessary, in case no GetHandler is registered.
   */
  FieldType field_data_{};

  /**
   * Mutex to protect concurrent modification of field_data_;
   */
  std::mutex field_data_lock_;
};

}  // namespace internal
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_SKELETON_FIELD_H_

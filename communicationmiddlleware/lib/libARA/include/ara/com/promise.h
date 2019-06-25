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
/**        \file  promise.h
 *        \brief  Specific implementation of a promise for ara::com
 *
 *      \details  ara::com::Promise is the corresponding promise that returns an ara::com::Future.
 *          \see  SWS_CM_00340
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_PROMISE_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_PROMISE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <functional>
#include <future>

#include "ara/com/future.h"

namespace ara {
namespace com {

/**
 * \brief ara::com specific Promise
 */
template <class T>
class Promise {
 public:
  /** \brief The promised type
   *
   */
  using ValueType = T;
  /**
   * \brief Default constructor
   */
  Promise() = default;
  /**
   * \brief Default copy constructor deleted
   */
  Promise(const Promise&) = delete;
  /**
   * \brief Move constructor
   */
  Promise(Promise&&) noexcept = default;
  /**
   * \brief Destructor
   */
  ~Promise() = default;
  /**
   * \brief Default copy assignment operator deleted
   */
  Promise& operator=(const Promise&) = delete;
  /**
   * \brief Move assignment operator
   */
  Promise& operator=(Promise&&) noexcept = default;
  /**
   * \brief Return a Future with the same shared state.
   * \return Future with same shared state
   */
  Future<ValueType> get_future() { return ara::com::Future<ValueType>(delegate_promise_.get_future()); }
  /**
   * \brief Store an exception in the shared state.
   * \param p pointer to an exception pointer
   * \return p Exception to store
   */
  void set_exception(std::exception_ptr p) { delegate_promise_.set_exception(p); }
  /**
   * \brief Store a value in the shared state.
   * \param value value to be set
   * \return value Value to store
   */
  void set_value(const ValueType& value) { delegate_promise_.set_value(value); }
  /**
   * \brief Set a value in the shared state.
   * \param value value to be set
   * \return value Value to store
   */
  void set_value(ValueType&& value) { delegate_promise_.set_value(value); }
  /**
   * \brief Set a handler to be called, upon future destruction.
   * \param handler user-defined deleter
   * \return handler Handler to be called upon future destruction
   */
  void set_future_dtor_handler(std::function<void> handler);

 private:
  std::promise<ValueType> delegate_promise_;
};

}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_PROMISE_H_

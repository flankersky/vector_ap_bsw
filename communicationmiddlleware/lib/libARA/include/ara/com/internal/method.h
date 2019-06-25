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
/**        \file  method.h
 *        \brief  TODO
 *
 *      \details  Contains common components used by all user-defined methods
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_METHOD_H_
#define LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_METHOD_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <utility>
#include "ara/com/future.h"

namespace ara {
namespace com {
namespace internal {

/**
 * \brief Parameter list of a method.
 * \tparam ArgsT The parameters of this method call
 */
template <typename... ArgsT>
struct MethodParameters {
  /**
   * \brief Generic template class for a method called by a proxy.
   * \tparam Interface To get a reference to the interface to call on an operator-call.
   * \tparam ConcreteMethod To identify the method at server-side.
   * \tparam Method The method signature of the method, that is defined in the interface,
   * belonging to that proxy.
   */
  template <typename Interface, typename ConcreteMethod,
            ara::com::Future<typename ConcreteMethod::Output> (Interface::*Method)(ArgsT...)>
  class ProxyMethod {
   public:
    /**
     * \brief Type-alias which is used by other classes such as the field template-class.
     */
    using ImplInterface = Interface;

    /**
     * \brief Default constructor of this method sets the proxy to
     * access the concrete ServiceProxyImplInterface on a method call.
     *
     * \param i A pointer to the interface of a proxy instance.
     */
    explicit ProxyMethod(Interface* i) noexcept : i_{i} {}

    /**
     * \brief Return structure for this method, the proxy expects.
     */
    using Output = typename ConcreteMethod::Output;

    /**
     * \brief The return value of this method is a future object
     * including the output structure for asynchronous operation.
     */
    using Return = ara::com::Future<Output>;

    /**
     * \brief Operation will call the method of the ServiceProxyImplInterface.
     *
     * \param args The parameters of this method call.
     * \return a future object for the application user, waiting for the return values of the method.
     */
    Return operator()(ArgsT... args) { return (i_->*Method)(std::forward<ArgsT>(args)...); }

   private:
    /// An interface of a proxy.
    Interface* i_;
  };
};

/**
 * \brief Specialization for no input parameters.
 */
template <>
struct MethodParameters<void> {
  /**
   * \brief Generic template class for a method called by a proxy.
   * \tparam Interface To get a reference to the interface to call on an operator-call.
   * \tparam ConcreteMethod To identify the method at server-side.
   * \tparam Method The method signature of the method, that is defined in the interface,
   * belonging to that proxy.
   */
  template <typename Interface, typename ConcreteMethod,
            ara::com::Future<typename ConcreteMethod::Output> (Interface::*Method)()>
  class ProxyMethod {
   public:
    /**
     * \brief Type-alias which is used by other classes such as the field template-class.
     */
    using ImplInterface = Interface;

    /**
     * \brief Default constructor of this method sets the proxy to
     * access the concrete ServiceProxyImplInterface on a method call.
     *
     * \param i A pointer to the interface of a proxy instance.
     */
    explicit ProxyMethod(Interface* i) noexcept : i_{i} {}

    /**
     * \brief Return structure for this method, the proxy expects.
     */
    using Output = typename ConcreteMethod::Output;

    /**
     * \brief The return value of this method is a future object
     * including the output structure for asynchronous operation.
     */
    using Return = ara::com::Future<Output>;

    /**
     * \brief Operation will call the method of the ServiceProxyImplInterface.
     *
     * \return a future object for the application user, waiting for the return values of the method.
     */
    Return operator()() { return (i_->*Method)(); }

   private:
    /// An interface of a proxy.
    Interface* i_;
  };
};

}  // namespace internal
}  // namespace com
}  // namespace ara

#endif  // LIB_LIBARA_INCLUDE_ARA_COM_INTERNAL_METHOD_H_

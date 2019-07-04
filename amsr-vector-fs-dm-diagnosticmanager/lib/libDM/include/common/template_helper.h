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
/**        \file  template_helper.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDM_INCLUDE_COMMON_TEMPLATE_HELPER_H_
#define LIB_LIBDM_INCLUDE_COMMON_TEMPLATE_HELPER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <utility>

namespace amsr {
namespace diag {
namespace common {

namespace helper {
/**
 * \brief Helper templated struct for resolving call with multiple parameters.
 */
template <typename F, typename Return, typename Tuple, bool Done, int Total, int... N>
struct call_impl {
  /**
   * \brief Template method that is recursively called.
   */
  static void call(F f, Tuple&& t, Return& ret) {
    call_impl<F, Return, Tuple, Total == 1 + sizeof...(N), Total, N..., sizeof...(N)>::call(f, std::forward<Tuple>(t),
                                                                                            ret);
  }
};
/**
 * \brief Partial specialization of templated struct call_impl.
 */
template <typename F, typename Return, typename Tuple, int Total, int... N>
struct call_impl<F, Return, Tuple, true, Total, N...> {
  /**
   * \brief Method called on final resolution of template iterations.
   * By construction, this method gets called with template parameters:
   * F: function to be called,
   * Return: return type,
   * Tuple: Tuple of parameters used for the method call,
   * Total = number of tuple elements,
   * N... = 0,1,2,3,...,(Total-1).
   */
  static void call(F f, Tuple&& t, Return& ret) { ret = f(std::get<N>(std::forward<Tuple>(t))...); }
};
}  // namespace helper

/**
 * \brief Method for calling a method with parameters given as a tuple.
 * \param f The method to be called.
 * \param t The tuple of parameters for the method call.
 * \param ret The return of the method provided by reference.
 */
template <typename F, typename Return, typename Tuple>
void call(F f, Tuple&& t, Return& ret) {
  using ttype = typename std::decay<Tuple>::type;
  helper::call_impl<F, Return, Tuple, 0 == std::tuple_size<ttype>::value, std::tuple_size<ttype>::value>::call(
      f, std::forward<Tuple>(t), ret);
}

} /* namespace common */
} /* namespace diag */
} /* namespace amsr */

#endif  // LIB_LIBDM_INCLUDE_COMMON_TEMPLATE_HELPER_H_

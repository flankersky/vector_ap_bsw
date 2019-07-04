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
/**        \file
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_ADDRESS_IFADDRS_PTR_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_ADDRESS_IFADDRS_PTR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ifaddrs.h>
#include <sys/types.h>

#include <memory>

namespace osabstraction {
namespace io {
namespace network {
namespace address {

/**
 * \brief A type for ifaddrs struct pointer.
 */
using ifaddrs_ptr = std::unique_ptr<struct ifaddrs, decltype(&freeifaddrs)>;

/**
 * \brief Wraps an ifaddrs_ptr around a ifaddrs struct.
 *
 * \param ifa A ifaddrs struct.
 * \return An ifaddrs_ptr value.
 */
inline ifaddrs_ptr make_ifaddrs_ptr(struct ifaddrs* ifa) { return ifaddrs_ptr{ifa, &freeifaddrs}; }

}  // namespace address
}  // namespace network
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NETWORK_ADDRESS_IFADDRS_PTR_H_

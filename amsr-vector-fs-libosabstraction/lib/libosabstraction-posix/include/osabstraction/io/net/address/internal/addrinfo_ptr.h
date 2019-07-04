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

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_INTERNAL_ADDRINFO_PTR_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_INTERNAL_ADDRINFO_PTR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <memory>

namespace osabstraction {
namespace io {
namespace net {
namespace address {
namespace internal {

/**
 * \brief A type for addrinfo struct pointer.
 */
using addrinfo_ptr = std::unique_ptr<struct addrinfo, decltype(&freeaddrinfo)>;

/**
 * \brief Wraps an addrinfo_ptr around an addrinfo struct.
 *
 * \param   ai  An addrinfo struct.
 *
 * \return An addrinfo_ptr value.
 */
inline addrinfo_ptr make_addrinfo_ptr(struct addrinfo* ai) { return addrinfo_ptr(ai, &freeaddrinfo); }

}  // namespace internal
}  // namespace address
}  // namespace net
}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_IO_NET_ADDRESS_INTERNAL_ADDRINFO_PTR_H_

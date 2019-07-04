#ifndef LIB_ARAREST_INCLUDE_ARA_REST_OGM_COPY_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_OGM_COPY_H_

#include <cassert>
#include <type_traits>

#include "ara/rest/allocator.h"
#include "ara/rest/ogm/node.h"
#include "ara/rest/pointer.h"

namespace ara {

namespace rest {

namespace ogm {

namespace details {

// see CC file
ara::rest::Pointer<Node> CopyImpl(const Node*);

}  // namespace details

/** \brief Copies an object graph
 *
 *  Performs a deep copy of the argument
 *
 *  \param u OGM graph to copy
 *  \param alloc allocator to use for the copy
 *  \return a copy
 */

template <typename T>
ara::rest::Pointer<T> Copy(const T& u, Allocator* alloc = GetDefaultAllocator()) {
  (void)alloc;
  static_assert(std::is_base_of<ara::rest::ogm::Node, T>::value, "Unsupported type for Visit()");

  return Pointer<T>(details::cast<T>(details::CopyImpl(&u).release()));
}

/** \brief Copies an object graph
 *
 *  Performs a deep copy of the argument
 *
 *  \param u OGM graph to copy
 *  \param alloc allocator to use for the copy
 *  \return a copy
 */
template <typename T>
ara::rest::Pointer<T> Copy(T* u, Allocator* alloc = GetDefaultAllocator()) {
  (void)alloc;
  return Copy(*u, alloc);
}

/** \brief Copies an object graph
 *
 *  Performs a deep copy of the arguments
 *
 *  \param u OGM graph to copy
 *  \param alloc allocator to use for the copy
 *  \return a copy
 */
template <typename T>
ara::rest::Pointer<T> Copy(const Pointer<T>& p, Allocator* alloc = GetDefaultAllocator()) {
  (void)alloc;
  return Copy((p.get()));
}

}  // namespace ogm

}  // namespace rest

}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_OGM_COPY_H_

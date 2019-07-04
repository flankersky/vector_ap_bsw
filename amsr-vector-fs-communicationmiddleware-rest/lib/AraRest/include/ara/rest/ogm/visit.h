#ifndef LIB_ARAREST_INCLUDE_ARA_REST_OGM_VISIT_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_OGM_VISIT_H_

#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>

#include "ara/rest/allocator.h"
#include "ara/rest/pointer.h"

#include "ara/rest/ogm/base.h"
// needed for visitation; inheritance relation must be known.
#include "ara/rest/ogm/array.h"
#include "ara/rest/ogm/field.h"
#include "ara/rest/ogm/int.h"
#include "ara/rest/ogm/object.h"
#include "ara/rest/ogm/real.h"
#include "ara/rest/ogm/string.h"
#include "ara/rest/ogm/uri.h"
#include "ara/rest/ogm/uuid.h"

namespace ara {

namespace rest {

namespace ogm {

namespace details {

/**
 * Overload
 * \brief inherit (linearly) from functors F
 */
template <typename F, typename... Fs>
struct overload /** @cond */ : F, overload<Fs...> /** @endcond */ {
  using F::operator();
  using overload<Fs...>::operator();
  /**
   * overload
   * \brief TODO
   * \param f TODO
   * \param fs TODO
   */
  constexpr overload(F&& f, Fs&&... fs) noexcept : F(std::forward<F>(f)), overload<Fs...>(std::forward<Fs>(fs)...) {}
};

/**
 * Overload
 * \brief TODO
 */
template <typename F>
struct overload<F> : F {
  using F::operator();
  /**
   * overload
   * \brief TODO
   * \param f TODO
   */
  constexpr explicit overload(F&& f) noexcept : F(std::forward<F>(f)) {}
};

/**
 * make_overload
 * \brief TODO
 */
template <typename... Fs>
static constexpr auto make_overload(Fs&&... fs) noexcept -> overload<Fs...> {
  return overload<Fs...>(std::forward<Fs>(fs)...);
}

/** \brief function description
 *
 *  Detailed description
 *
 *  \param param
 *  \return return type
 */
template <typename T, typename... Fs>
void VisitImpl(T* u, Fs&&... fs) {
  auto visitor = details::make_overload(std::forward<Fs>(fs)..., [](...) {});

  switch (u->type()) {
    case NodeType::Field:
      visitor(details::cast<Field>(u));
      break;
    case NodeType::Int:
      visitor(details::cast<Int>(u));
      break;
    case NodeType::String:
      visitor(details::cast<String>(u));
      break;
    case NodeType::Real:
      visitor(details::cast<Real>(u));
      break;
    case NodeType::Array:
      visitor(details::cast<Array>(u));
      break;
    case NodeType::Uuid:
      visitor(details::cast<Uuid>(u));
      break;
    case NodeType::Uri:
      visitor(details::cast<Uri>(u));
      break;

    default:
      if (details::isa<Object>(u)) {
        visitor(details::cast<Object>(u));
      } else {
        assert(0 && "Unknown NodeType");
      }
  }
}

}  // namespace details

/** \brief See documentation of void Visit(const Node&, Visitors&&);
 *
 *  \param u OGM node to resolve
 *  \param fs a set of functors
 *  \return a copy
 */
template <typename... Fs>
void Visit(Node& u, Fs&&... fs) {
  details::VisitImpl(&u, std::forward<Fs>(fs)...);
}

// (visjak): not sure this is needed. please provide a test case here
//
template <typename... Fs>
void Visit(const Node* u, Fs&&... fs) {
  details::VisitImpl(u, std::forward<Fs>(fs)...);
}

/** \brief See documentation of void Visit(const Node&, Visitors&&);
 *
 *  \param u OGM node to resolve
 *  \param fs a set of functors
 *  \return a copy
 */
template <typename T, typename... Fs>
void Visit(const ara::rest::Pointer<T>& u, Fs&&... fs) {
  Visit(u.get(), std::forward<Fs>(fs)...);
}

/** \brief See documentation of void Visit(const Node&, Visitors&&);
 *
 *  \param u OGM node to resolve
 *  \param vis a set of functors
 *  \return a copy
 */
template <typename NodeT, typename... Fs>
void Visit(Pointer<NodeT>& u, Fs&&... fs) {
  Visit(u.get(), std::forward<Fs>(fs)...);
}

}  // namespace ogm

}  // namespace rest

}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_OGM_VISIT_H_

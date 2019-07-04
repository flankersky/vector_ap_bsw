#ifndef LIB_ARAREST_INCLUDE_ARA_REST_OGM_BASE_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_OGM_BASE_H_

#include <memory>
#include <utility>

#include "ara/rest/allocator.h"
#include "ara/rest/pointer.h"

namespace ara {

namespace rest {

namespace ogm {

namespace details {

/**
 * Type-system support
 */

/** \brief function description
 *
 *  Detailed description
 *
 *  \param param
 *  \return return type
 */
template <typename To, typename From>
constexpr bool isa(From* u) noexcept {
  return To::classof(u);
}

/** \brief function description
 *
 *  unchecked cast; non-const
 *
 *  \param param
 *  \return return type
 */ template <typename To, typename From>
constexpr To* cast(From* u) noexcept {
  return static_cast<To*>(u);
}

/** \brief function description
 *
 *  unchecked cast, const
 *
 *  \param param
 *  \return return type
 */
template <typename To, typename From>
constexpr const To* cast(const From* u) noexcept {
  return static_cast<const To*>(u);
}

}  // namespace details

/**
 * node type enumeration
 */
enum class NodeType : std::uint_least8_t {  // (visjak) why 8?
  // Special types
  Undefined,  //!< Undefined
  Field,      //!< Field

  // Value types
  ValueTypesStart,  //!< ValueTypesStart

  Int,     //!< Int
  Real,    //!< Real
  String,  //!< String
  Uri,     //!< Uri
  Uuid,    //!< Uuid
  Array,   //!< Array
  Object,  //!< Object

  ValueTypesEnd,  //!< ValueTypesEnd
};

/**
 * \brief CommonBase class
 */
class CommonBase {
 public:
  /**
   * \brief get allocator
   *
   * \return TODO
   */
  Allocator* GetAllocator() { return alloc_; }

  /**
   * \brief get allocator
   *
   * \return TODO
   */
  const Allocator* GetAllocator() const { return alloc_; }

 protected:
  /**
   * \brief allocator constructor
   *
   * \param alloc TODO
   */
  explicit CommonBase(Allocator* alloc) : alloc_(alloc) {}

  /**
   * \brief destructor
   */
  virtual ~CommonBase() {}

 private:
  /**
   * \brief pointer to allocator
   */
  Allocator* alloc_;
};

template <typename NodeT, typename ParentT, bool is_constructible = true>
class Base;

/**
 * \brief base class
 */
template <typename NodeT, typename ParentT>
class Base<NodeT, ParentT, true> {
 public:
  /**
   * needs documentation
   */
  using SelfType = NodeT;

  /**
   * needs documentation
   */
  using ParentType = ParentT;

  /**
   * \brief create object of SelfType
   */
  template <typename... Ts>
  static Pointer<SelfType> Make(Ts&&... ts) {
    return Pointer<SelfType>{new SelfType(std::forward<Ts>(ts)...)};
  }

  /**
   * \brief create object of SelfType
   */
  template <typename... Ts>
  static Pointer<SelfType> Make(Allocator* alloc, Ts&&... ts) {
    return Pointer<SelfType>{new SelfType(alloc, std::forward<Ts>(ts)...)};
  }

  /**
   * \brief get parent
   *
   * \return TODO
   */
  ParentType* GetParent() { return parent_; }

  /**
   * \brief get parent
   */
  const ParentType* GetParent() const { return parent_; }

  /**
   * \brief has parent
   */
  bool HasParent() const { return parent_ != nullptr; }

 protected:
  /**
 * \brief pointer to parent node
 */
  Base() = default;

  /**
   * \brief pointer to parent node
   */
  ParentType* parent_ = nullptr;
};

/**
 * \brief base class
 */
template <typename NodeT, typename ParentT>
class Base<NodeT, ParentT, false> {
 public:
  /**
   * needs documentation
   */
  using SelfType = NodeT;

  /**
   * needs documentation
   */
  using ParentType = ParentT;

  /**
   * \brief get parent
   *
   * \return TODO
   */
  ParentType* GetParent() { return nullptr; }

  /**
   * \brief get parent
   */
  const ParentType* GetParent() const { return nullptr; }

  /**
   * \brief has parent
   */
  bool HasParent() const { return false; }
};

}  // namespace ogm

}  // namespace rest

}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_OGM_BASE_H_

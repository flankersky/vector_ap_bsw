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
/**        \file  rest/uri.h
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_URI_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_URI_H_

#include <array>
#include <cassert>
#include <cstddef>
#include <cstring>
#include <limits>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "ara/rest/allocator.h"
#include "ara/rest/exception.h"
#include "ara/rest/iterator.h"
#include "ara/rest/string.h"
#include "ara/rest/types.h"

namespace ara {

namespace rest {

// TODO(visjak): noexcept; abhängig vom StringView typ

// (visjak): url max length see
// https://stackoverflow.com/questions/417142/what-is-the-maximum-length-of-a-url-in-different-browsers

/** \brief Uri is a generic URI container compliant to RFC 3986
 *
 * Uri is immutable and does not throw (unless StringView throws). A new Uri can be constructed via
 * Uri::Builder from an empty Uri or an existing one. No member access to Uri causes dynamic memory
 * allocation. This includes the copy- and copy-assignment constructors too.
 *
 * Uri is not allowed to perform any kind of dynamic memory allocation.
 *
 * For conversion into textual representations of URI components, accessor functions accept a
 * template argument T that specified the type conversion. Type T must must be "InputStreamable"
 * which means that an operator>>() must be defined that takes a std::istream object on the left
 * hand side and an instance of the result type on the right.
 *
 * A URI is always non-percent-encoded. When parsed percent-encoded charaters are replaced.
 * Accordingly, Uri comparison is also only performed in non-encoded representations.
 * Percent-encoding applied by ToString() only. All member functions that perform string
 * conversion  return non-percent-encoded representations.
 *
 * Uri must be efficiently movable.
 *
 */
class Uri {
 public:
  /**
   * \brief The maximum length of a URI.
   *
   * The suggested length maximum is around 2000 characters which roughly conforms to the
   * typical limit that mainstream webbrowsers support. A bound is specified to enable
   * optimization potential in the internal encoding.
   */
  static constexpr std::size_t LENGTH_MAX = 2048;  // 10 bits will do

  /**
   * \brief Return value of Uri::GetPort() when no port is found
   */
  static constexpr char PORT_NOT_FOUND = -1;

  /** \brief Used to specify a subset of a URI.
   *
   * Part defines components of a Uri. This is typically used to convert Uri into a textual
   * representation.
   */
  enum class Part : std::uint32_t {
    Scheme = 1 << 1,                                      ///< Denotes URI scheme according to RFC 3986
    UserInfo = 1 << 2,                                    ///< Denotes URI user info according to RFC 3986
    Host = 1 << 3,                                        ///< Denotes URI authority host according to RFC 3986
    Port = 1 << 4,                                        ///< Denotes URI authority port according to RFC 3986
    Path = 1 << 5,                                        ///< Denotes URI path according to RFC 3986
    Query = 1 << 6,                                       ///< Denotes URI query according to RFC 3986
    Fragment = 1 << 7,                                    ///< Denotes URI fragment according to RFC 3986
    PathAndQuery = Part::Path | Part::Query,              ///< Denotes URI path and query
    PathEtc = Part::Path | Part::Query | Part::Fragment,  ///< Dentes URI path, query and fragment
    All = ~std::underlying_type<Part>::type{0}            ///< Denotes all known URI components
  };

  /** \brief  Computes a set of Part enumerators
   * \param a (set of) Part enumerator(s)
   * \param b (set of) Part enumerator(s)
   * \return a set of Part enumerators
   */
  friend constexpr Part operator|(Part a, Part b) noexcept {
    using T = std::underlying_type<Part>::type;
    return static_cast<Part>(static_cast<T>(a) | static_cast<T>(b));
  }

  /** \brief Composes Part enumerators
   *  \param a
   *  \param b
   */
  friend constexpr Part operator&(Part a, Part b) noexcept {
    using T = std::underlying_type<Part>::type;
    return static_cast<Part>(static_cast<T>(a) & static_cast<T>(b));
  }

  class Builder;

  /** \brief Constructs a default URI.
   *
   * A default-constructed URI is empty. To populate an existing URI, Uri::Builder must be used.
   * Uri member functions must not throw unless in those cases where StringView is
   * used and StringView is not 'nothrow_constructible'.
   *
   */
  Uri() noexcept = default;

  /**
   * \brief Has scheme.
   */
  bool HasScheme() const noexcept;

  /**
   * \brief gets scheme.
   */
  StringView GetScheme() const noexcept(std::is_nothrow_constructible<StringView>::value);

  /**
   * \brief has user info.
   */
  bool HasUserInfo() const noexcept;

  /**
   * \brief get user info.
   */
  StringView GetUserinfo() const noexcept(std::is_nothrow_constructible<StringView>::value);

  /**
   * \brief has host.
   */
  bool HasHost() const noexcept;

  /**
   * \brief get host.
   */
  StringView GetHost() const noexcept(std::is_nothrow_constructible<StringView>::value);

  /**
   * \brief has host.
   */
  bool HasPort() const noexcept;

  /**
   * \brief get port.
   */
  int GetPort() const noexcept;

  class Path;

  /**
   * \brief has path.
   */
  bool HasPath() const noexcept;

  /**
   * \brief get path.
   */
  const Path& GetPath() const noexcept;

  class Query;

  /**
   * \brief has query.
   */
  bool HasQuery() const noexcept;

  /**
   * \brief get query.
   */
  const Query& GetQuery() const noexcept;

  /**
   * \brief Has Fragment.
   */
  bool HasFragment() const noexcept;
  /**
   * \brief Get Fragment.
   */
  StringView GetFragment() const noexcept(std::is_nothrow_constructible<StringView>::value);
  /** \brief Converts a URI fragment part to a given type
   *
   *  The conversion result is assigned to the function argument which is subsequently returned.  If
   *  conversion fails the function argument is returned unchanged. So either form is valid:
   *  GetFragmentAs<string>(), GetFragmentAs(string{my_allocator}), GetFragmentAs<string>("conversion failed")
   *
   *  \tparam T type to convert to. T must be InputStreamable.
   *  \param def a default value
   *  \return an instance of type T that represents this URI component
   */
  template <typename T>
  T GetFragmentAs(T&& def = T{}) const;

  /**
   * \brief Is URI empty.
   * \return true if empty
   */
  bool IsEmpty() const noexcept;
  /**
   * \brief Is URI relative. An URI is relative if it does not starts with a scheme.
   * \return true if relative
   */
  bool IsRelative() const noexcept;

  /** \brief Denotes whether the URI is opaque
   *
   *  An opaque URI is an absolute URI whose scheme-specific part does not begin with a slash
   *  character ('/').
   *
   *  \return true if this URI is opaque
   */
  bool IsOpaque() const noexcept;

  /** \brief Denotes whether this URI is hierarchical
   *
   * A hierarchical URI is either an absolute URI whose scheme-specific part begins with a slash
   * character, or a relative URI, that is, a URI that does not specify a scheme.
   *
   *  \return true if this URI is hierarchical
   */
  bool IsHierarchical() const noexcept;

 protected:
  /**
   * Separated data of uri
   */
  class Impl;

  /**
   * Pointer to uri data
   */
  std::shared_ptr<const Impl> data_;

  /**
   * \brief Check if path is absolute according to rfc3986: Appendix A.  Collected ABNF for URI
   * \return true if path is absolute
   */
  bool IsPathAbsolute() const noexcept;

  /**
   * \brief Check if path is absolute or empty according to rfc3986: Appendix A.  Collected ABNF for URI
   * \return true either if path is empty or it is an absolute path
   */
  bool IsPathAbEmpty() const noexcept;

  /**
   * \brief Check if path is no schem path according to rfc3986: Appendix A.  Collected ABNF for URI
   * \return true if path has no scheme
   */
  bool IsPathNoScheme() const noexcept;

  /**
   * \brief Check if path is rootless according to rfc3986: Appendix A.  Collected ABNF for URI
   * \return true if path is rootless
   */
  bool IsPathRootless() const noexcept;
};

/** \brief Returns a string representation of a Uri
 *
 *  \param uri URI to encode
 *  \param part denotes which components of a URI should be encoded
 *  \param encode if true, then the string will be percent-encoded. If false, the string must not be string encoded.
 *  \param alloc a user-defined allocator passed to the string object being returned
 *  \return the encoded URI
 */
String ToString(const Uri& uri, Uri::Part part, bool encode, Allocator* alloc = GetDefaultAllocator());

/**
 *  \brief Returns a string representation of a Uri
 *  \param uri URI to encode
 *  \param part denotes which components of a URI should be encoded
 *  \param alloc a user-defined allocator passed to the string object being returned
 *  \return the encoded URI
 */
String ToString(const Uri& uri, Uri::Part part, Allocator* alloc = GetDefaultAllocator());

/**
 *  \brief Returns a string representation of a Uri
 *  \param uri URI to encode
 *  \param alloc a user-defined allocator passed to the string object being returned
 *  \return the encoded URI
 */
String ToString(const Uri& uri, Allocator* alloc = GetDefaultAllocator());

/**
 *  \brief Returns a string representation of a Uri
 *  \param uri URI to encode
 *  \param part denotes which components of a URI should be encoded
 *  \param encode if true, then the string will be percent-encoded. If false, the string must not be string encoded.
 *  \param alloc a user-defined allocator passed to the string object being returned
 *  \return the encoded URI
 */
String ToString(Uri&& uri, Uri::Part part, bool encode, Allocator* alloc = GetDefaultAllocator());

/**
 *  \brief Returns a string representation of a Uri
 *  \param uri URI to encode
 *  \param part denotes which components of a URI should be encoded
 *  \param alloc a user-defined allocator passed to the string object being returned
 *  \return the encoded URI
 */
String ToString(Uri&& uri, Uri::Part part, Allocator* alloc = GetDefaultAllocator());

/**
 *  \brief Returns a string representation of a Uri
 *  \param uri URI to encode
 *  \param alloc a user-defined allocator passed to the string object being returned
 *  \return the encoded URI
 */
String ToString(Uri&& uri, Allocator* alloc = GetDefaultAllocator());

/** \brief Represents the path component of a URI
 *
 *
 *
 */
class Uri::Path {
 public:
  /** \brief Represents a path segment
   *
   */
  class Segment;

  /**
   * Iterator of path segment
   */
  using Iterator = std::vector<Segment>::const_iterator;

  /**
   * \brief Iterator range of path segments
   */
  using IteratorRange = ara::rest::IteratorRange<Iterator>;

  /** \brief Returns the number of path segments
   *  \return a number of path segments
   */
  std::size_t NumSegments() const noexcept;

  /** \brief Returns a range of path segments
   *  \return an iterator range of path segments
   */
  IteratorRange GetSegments() const noexcept { return IteratorRange{segments_.cbegin(), segments_.cend()}; }

  /** \brief Tests two paths for equality
   * \param a object to compare
   * \param b object to compare
   *  \return true if equal
   */
  friend bool operator==(const Path& a, const Path& b) noexcept {
    return (a.NumSegments() == b.NumSegments() &&
            std::equal(a.GetSegments().begin(), a.GetSegments().end(), b.GetSegments().begin()));
  }

  /** \brief Tests two paths for inequality
   * \param a object to compare
   * \param b object to compare
   *  \return true if not equal
   */
  friend bool operator!=(const Path& a, const Path& b) noexcept { return !(a == b); }

  /** \brief Relates two paths according to their lexicographical order
   * \param a object to compare
   * \param b object to compare
   *  \return true if a compares less-than b
   */
  friend bool operator<(const Path& a, const Path& b) noexcept {
    return (std::lexicographical_compare(a.GetSegments().begin(), a.GetSegments().end(), b.GetSegments().begin(),
                                         b.GetSegments().end()));
  }
  /**
   * \brief Check if path has leading slash
   * \return boolean
   */
  bool HasLeadingSlash() const noexcept { return leading_slash_; }
  /**
   * \brief Check if path has trailing slash
   * \return boolean
   */
  bool HasTrailingSlash() const noexcept { return trailing_slash_; }

  /** \brief Returns a string representation of a Uri
   *
   *  \param uri URI to encode
   *  \param part denotes which components of a URI should be encoded
   *  \param encode if true, then the string will be percent-encoded. If false, the string must not be string encoded.
   *  \param alloc a user-defined allocator passed to the string object being returned
   *  \return the encoded URI
   */
  friend String ToString(const Uri& uri, Uri::Part part, bool encode, Allocator* alloc);

  /**
   *  \brief Returns a string representation of a Uri
   *  \param uri URI to encode
   *  \param part denotes which components of a URI should be encoded
   *  \param alloc a user-defined allocator passed to the string object being returned
   *  \return the encoded URI
   */
  friend String ToString(const Uri& uri, Uri::Part part, Allocator* alloc);

  /**
   *  \brief Returns a string representation of a Uri
   *  \param uri URI to encode
   *  \param alloc a user-defined allocator passed to the string object being returned
   *  \return the encoded URI
   */
  friend String ToString(const Uri& uri, Allocator* alloc);

  /**
   *  \brief Returns a string representation of a Uri
   *  \param uri URI to encode
   *  \param part denotes which components of a URI should be encoded
   *  \param encode if true, then the string will be percent-encoded. If false, the string must not be string encoded.
   *  \param alloc a user-defined allocator passed to the string object being returned
   *  \return the encoded URI
   */
  friend String ToString(Uri&& uri, Uri::Part part, bool encode, Allocator* alloc);

  /**
   *  \brief Returns a string representation of a Uri
   *  \param uri URI to encode
   *  \param part denotes which components of a URI should be encoded
   *  \param alloc a user-defined allocator passed to the string object being returned
   *  \return the encoded URI
   */
  friend String ToString(Uri&& uri, Uri::Part part, Allocator* alloc);

  /**
   *  \brief Returns a string representation of a Uri
   *  \param uri URI to encode
   *  \param alloc a user-defined allocator passed to the string object being returned
   *  \return the encoded URI
   */
  friend String ToString(Uri&& uri, Allocator* alloc);

 protected:
  friend class Uri::Builder;

  /**
   * \brief Vector with the segments of path.
   */
  std::vector<Segment> segments_;

  /**
   * \brief Indicates presence of leading slash
   */
  bool leading_slash_;

  /**
   * \brief Indicates presence of trailing slash
   */
  bool trailing_slash_;
};

/** \brief Need documentation
 *
 */
class Uri::Path::Segment {
 public:
  /**
   * Default constructor
   */
  Segment() noexcept = default;
  /** \brief Returns a string representation of this path segment
   *
   *  The representation is non-percent-encoded
   *
   *  \return a string representation of this path segment
   */
  StringView Get() const noexcept(std::is_nothrow_constructible<StringView>::value);

  /** \brief Returns this segment converted to a user-defined type
   *
   *  The conversion result is assigned to the function argument which is subsequently returned.  If
   *  conversion fails the function argument is returned unchanged. So either form is valid:
   *  GetAs<string>(), GetAs(string{my_allocator}), GetAs<string>("conversion failed")
   *
   *  \tparam T type to convert to. T must be InputStreamable.
   *  \param def a default value
   *  \return an instance of type T that represents this URI component.
   */
  template <typename T>
  T GetAs(T&& def = T{}) const {
    std::stringstream ss_seg;
    ss_seg.str(segment_);
    typename std::remove_reference<T>::type tmp = def;
    ss_seg >> def;
    if (ss_seg.fail()) {
      def = tmp;
    }
    return std::forward<T>(def);
  }

  /** \brief Tests two segments for equality
   * \param a object to compare
   * \param b object to compare
   *  \return true if segments compare equal
   */
  friend bool operator==(const Segment& a, const Segment& b) noexcept {
    return (a.segment_.size() == b.segment_.size() && a.segment_ == b.segment_);
  }
  /** \brief Tests two segments for inequality
   * \param a object to compare
   * \param b object to compare
   *  \return true if segments compare unequal
   */
  friend bool operator!=(const Segment& a, const Segment& b) noexcept { return !(a == b); }

  /** \brief Compares two path segments according to their lexicographical order
   *
   * Order is determined on the non-percent-encoded representation of a segment
   * \param a object to compare
   * \param b object to compare
   * \return true if a compares less-than b
   */
  friend bool operator<(const Segment& a, const Segment& b) noexcept {
    return (a.segment_.size() < b.segment_.size() || a.segment_ < b.segment_);
  }

 private:
  friend class Uri::Builder;
  /**
   * \brief Constructor
   */
  explicit Segment(const StringView& segment) { STRINGVIEW_TO_STRING(segment_, segment); }
  /**
   * \brief The segment content.
   */
  String segment_;
};

/**
 * \brief Query of an Uri.
 */
class Uri::Query {
 public:
  class Parameter;
  /**
   * \brief Needs documentation.
   */
  using IteratorRange = ara::rest::IteratorRange<std::vector<Parameter>::const_iterator>;

  /** \brief Returns the number of query parameters
   *  \return the number of query parameters
   */
  std::size_t NumParameters() const noexcept;
  /** \brief Returns the range of all query parameters
   * \return an iterator range of query parameters
   */
  IteratorRange GetParameters() const noexcept { return IteratorRange{parameters_.begin(), parameters_.end()}; }
  /** \brief Returns a specific query parameter by index
   *
   *  If the index is out-of-bound, the result is undefined.
   *  \param i an index
   *  \return a reference to the query parameter
   */
  const Parameter& GetParameter(std::size_t i) const noexcept;
  /** \brief Searches for a query parameter by key
   *
   *  If no such key is found, returns Query::GetParameters.end()
   *  \param key a key
   *  \return an iterator to the respective query parameter
   */
  IteratorRange::Iterator Find(StringView key) const noexcept;
  /** \brief Tests whether a query parameter of a given key exists
   *  \param key a key
   * \return true of key exists
   */
  bool HasKey(StringView key) noexcept;

 protected:
  friend class Uri::Builder;

  /**
   * \brief The parameters of the query.
   */
  std::vector<Parameter> parameters_;
};

/** \brief Represents a parameter of URI::Query.
 *
 *  A query parameter is either a pair ("key=value") or a singleton ("key").
 *
 */
class Uri::Query::Parameter {
 public:
  /** \brief Returns a string representation of the parameter key
   *  The representation is non-percent-encoded.
   *  \return a string representation
   */
  StringView GetKey() const noexcept(std::is_nothrow_constructible<StringView>::value);
  /** \brief Converts a query parameter key to the specified type
   *
   *
   *  The conversion result is assigned to the function argument which is subsequently returned.  If
   *  conversion fails the function argument is returned unchanged. So either form is valid:
   *  GetKeyAs<string>(), GetKeyAs(string{my_allocator}), GetKeyAs<string>("conversion failed")
   *
   *  \tparam T type to convert to. T must be InputStreamable.
   *  \param def a default value
   *  \return an instance of type T that represents this URI component.
   */
  template <typename T>
  T GetKeyAs(T&& def = T{}) const {
    std::stringstream ss_key;
    ss_key.str(key_);
    typename std::remove_reference<T>::type tmp = def;
    ss_key >> def;
    if (ss_key.fail()) {
      def = tmp;
    }
    return std::forward<T>(def);
  }

  /**
   * \brief Needs documentation.
   * \return true if this query paramater has a value component
   */
  bool HasValue() const noexcept;

  /**
   * \brief Obtains the value of a query parameter
   * If none exists the result is undefined.
   * \return a string representation of the value
   */
  StringView GetValue() const noexcept(std::is_nothrow_constructible<StringView>::value);

  /** \brief Converts a query parameter value to the specified type
   *
   *  The conversion result is assigned to the function argument which is subsequently returned.  If
   *  conversion fails the function argument is returned unchanged. So either form is valid:
   *  GetValueAs<string>(), GetValueAs(string{my_allocator}), GetValueAs<string>("conversion failed")
   *
   *  \tparam T type to convert to. T must be InputStreamable.
   *  \param def a default value
   *  \return an instance of type T that represents this URI component.
   */
  template <typename T>
  T GetValueAs(T&& def = T{}) const {
    std::stringstream ss_value;
    ss_value.str(value_);
    typename std::remove_reference<T>::type tmp = def;
    ss_value >> def;
    if (ss_value.fail()) {
      def = tmp;
    }
    return std::forward<T>(def);
  }

 protected:
  friend class Uri::Builder;
  /**
  * Constructor
  */
  Parameter() = default;
  /**
   * \brief The key of the parameter.
   */
  String key_;
  /**
   * \brief The value of the parameter.
   */
  String value_;
};

/**
 * Class contains implementation data of Uri
 */
class Uri::Impl {
 public:
  /**
   * Constructor
   */
  Impl();

  /**
   * \brief Pointer to scheme of Uri.
   */
  String scheme_;

  /**
   * \brief Pointer to user info of Uri.
   */
  String user_info_;

  /**
   * \brief Pointer to host of Uri.
   */
  String host_;

  /**
   * \brief Pointer to port of Uri.
   */
  int port_;

  /**
   * \brief Pointer to path of Uri.
   */
  Uri::Path path_;

  /**
   * \brief Pointer to query of Uri.
   */
  Uri::Query query_;

  /**
   * \brief Pointer to fragment of Uri.
   */
  String fragment_;

  /**
   * Complete string of Uri
   */
  String uri_str_;
};

// NOTE(!): "T&&" is a universal reference; neither rvalue nor lvalue. treat accordingly.

/** \brief Builder to modify an Uri.
 *
 *  Uri is an immutable data structure for reasons of efficiency and resource control.
 *  For URI construction Uri::Builder must be used. Builder is allowed to perform dynamic
 *  memory allocation and its member functions may throw. Builder::ToUri() can be used
 *  to derive an (optimized) URI representation from the builder.
 *
 *  The member functions for URI composition accept arbitrary types which must be
 *  "OutputStreamable".  It means that an operator<<() must be defined that takes a std::ostream
 *  object on the left hand side and an instance of the argument type on the right.
 *
 *  To erase elements call constructor functions with an empty argument.
 *
 */
class Uri::Builder {
 public:
  /** \brief Default-constructs a builder
   * \param alloc an allocator
   */
  explicit Builder(Allocator* alloc = GetDefaultAllocator());
  /**
   * \brief Parses a URI in string format
   *
   * Throws std::invalid_argument of URI parsing fails.
   * \param uri an URI to initiazlize from
   * \param alloc an allocator
   */
  explicit Builder(StringView uri, Allocator* alloc = GetDefaultAllocator());

  /** \brief Initializes this builder with an existing Uri
   *
   * \param uri an URI to initiazlize from
   * \param alloc an allocator
   */
  explicit Builder(const Uri& uri, Allocator* alloc = GetDefaultAllocator());

  /** \brief Initializes this builder with an existing Uri
   *
   * \param uri an URI to initiazlize from
   * \param alloc an allocator
   */
  explicit Builder(Uri&& uri, Allocator* alloc = GetDefaultAllocator());

  /**
   * \brief Set scheme by parsing the given argument
   * Throws std::invalid_argument if parsing fails
   * \param value a value of an output-streamable type
   * \return a reference to this builder
   */
  template <typename T>
  Builder& Scheme(T&& value) {
    uri_data_.scheme_ = GetUriDataMemberString(std::forward<T>(value), "Cannot convert scheme to string!");
    return *this;
  }

  /**
   * \brief Set user info by parsing the given argument
   * Throws std::invalid_argument if parsing fails
   * \param value a value of an output-streamable type
   * \return a reference to this builder
   */
  template <typename T>
  Builder& UserInfo(T&& value) {
    uri_data_.user_info_ = GetUriDataMemberString(std::forward<T>(value), "Cannot convert user info to string!");
    return *this;
  }

  /** \brief Sets host by parsing the given argument
   * Throws std::invalid_argument if parsing fails
   * \param value a value of an output-streamable type
   * \return a reference to this builder
   */
  template <typename T>
  Builder& Host(T&& value) {
    uri_data_.host_ = GetUriDataMemberString(std::forward<T>(value), "Cannot convert host to string!");
    return *this;
  }

  /** \brief Sets the the Uri port from the given argument
   *  Throws std::invalid_argument if parsing fails
   * \param value
   * \return a reference to this builder
   */
  template <typename T>
  Builder& Port(T&& value) {
    uri_data_.port_ = std::stoi(GetUriDataMemberString(std::forward<T>(value), "Cannot convert port to string!"));
    return *this;
  }

  /** \brief Sets the URI path by parsing the given argument
   *  Throws std::invalid_argument, if parsing fails.
   * \param value a value of an output-streamable type
   * \return a reference to this builder
   */
  Builder& Path(StringView value) {
    uri_data_.path_ = BuildPath(value);
    return *this;
  }

  /** \brief Sets a path from an existing Uri::Path components
   * Throws std::invalid_argument if parsing fails
   * \param value a value of an output-streamable type
   * \return a reference to this builder
   */
  Builder& Path(const Uri::Path& value) {
    uri_data_.path_ = value;
    return *this;
  }

  /** \brief Constructs a path from the given function arguments
   * Throws std::invalid_argument if parsing fails
   * \param values values of output-streamable types
   * \return a reference to this builder
   */
  template <typename... Ts>
  Builder& PathSegments(Ts&&... values) {
    std::vector<Uri::Path::Segment> tmp;
    Uri::Path::Segment seg;

    for (auto&& val : {values...}) {
      seg.segment_ =
          GetUriDataMemberString(std::forward<decltype(val)>(val), "Conversion of passed segment type failed");
      tmp.push_back(seg);
    }
    uri_data_.path_.segments_.swap(tmp);
    return *this;
  }
  /**
   * \brief Constructs a path from the given function arguments starting from the n'th path segment
   *
   * Existing path segments starting from position pos will be destroyed.
   * Throws std::invalid_argument if parsing fails.
   * \param pos index to start from
   * \param values values of output-streamable type
   * \return a reference to this builder
   */
  template <typename... Ts>
  Builder& PathSegmentsFrom(std::size_t pos, Ts&&... values) {
    std::vector<Uri::Path::Segment> tmp;
    Uri::Path::Segment seg;

    if (pos + 1 < uri_data_.path_.NumSegments()) {
      uri_data_.path_.segments_.erase(uri_data_.path_.segments_.begin() + pos, uri_data_.path_.segments_.end());
    }

    for (auto&& val : {values...}) {
      seg.segment_ =
          GetUriDataMemberString(std::forward<decltype(val)>(val), "Conversion of passed segment type failed");
      uri_data_.path_.segments_.push_back(seg);
    }
    return *this;
  }

  /** \brief Needs documentation.
   *
   * Throws std::invalid_argument if parsing fails
   * \param q query
   * \return a reference to this builder
   *
   */
  Builder& Query(const Uri::Query& q) {
    uri_data_.query_ = q;
    return *this;
  }

  /** \brief Needs documentation.
   *
   * Throws std::invalid_argument if parsing fails
   * \param q query
   * \return a reference to this builder
   *
   */
  Builder& Query(StringView q) {
    uri_data_.query_ = BuildQuery(q);
    return *this;
  }

  /** \brief Inserts a query parameter (key only)
   *
   * If the given key already exists, no action is performed.
   *
   * Throws std::invalid_argument if parsing fails
   * \param key of a query parameter
   * \return a reference to this builder
   */
  template <typename T>
  Builder& QueryParameter(T&& key) {
    std::stringstream ss_key;
    ss_key << std::forward<T>(key);
    if (ss_key.fail()) {
      throw std::invalid_argument("Not able to convert key to string!");
    }
    StringView str(STRING_TO_STRINGVIEW(ss_key.str()));
    if (uri_data_.query_.Find(str) == uri_data_.query_.parameters_.end()) {
      uri_data_.query_.parameters_.emplace_back(BuildParameter(str));
    }
    return *this;
  }
  /** \brief Inserts a query parameter (key and value)
   *
   * If the given key already exists, no action is performed.
   *
   * Throws std::invalid_argument if parsing fails
   * \param key a key
   * \param value a value
   * \return a reference to this builder
   */
  template <typename T, typename U>
  Builder& QueryParameter(T&& key, U&& value) {
    std::stringstream ss_key, ss_value;
    ss_key << std::forward<T>(key);
    if (ss_key.fail()) {
      throw std::invalid_argument("Not able to convert key to string!");
    }
    ss_value << std::forward<U>(value);
    if (ss_value.fail()) {
      throw std::invalid_argument("Not able to convert value to string!");
    }

    StringView str(STRING_TO_STRINGVIEW(ss_key.str()));
    if (uri_data_.query_.Find(str) == uri_data_.query_.parameters_.end()) {
      uri_data_.query_.parameters_.emplace_back(CreateParameter(ss_key.str(), ss_value.str()));
    }
    return *this;
  }

  /** \brief Replaces an existing paramater (key only)
   *
   *  If old exists, then it is replaced by new. Otherwise no action is performed.
   *  If the existing key is part of a key/value pair, the entire pair is replaced.
   *
   * Throws std::invalid_argument if parsing fails
   * \param oldkey a key
   * \param newkey a value
   * \return a reference to this builder
   */
  template <typename T, typename U>
  Builder& QueryParameterAt(T&& oldkey, U&& newkey) {
    std::stringstream ss_oldkey, ss_newkey;

    ss_oldkey << std::forward<T>(oldkey);
    if (ss_oldkey.fail()) {
      throw std::invalid_argument("Not able to convert old key to string!");
    }

    ss_newkey << std::forward<U>(newkey);
    if (ss_newkey.fail()) {
      throw std::invalid_argument("Not able to convert new key to string!");
    }

    for (auto& param : uri_data_.query_.parameters_) {
      if (param.key_.compare(ss_oldkey.str()) == 0) {
        param.key_ = ss_newkey.str();
        param.value_ = "";
      }
    }

    return *this;
  }

  /** \brief Removes a query parameter (by key)
   * If key exists, removes it. Otherwise no action is performed.
   * If key belong to a key/value pair, the pair is removed.
   * Throws std::invalid_argument if parsing fails
   * \param key a key
   * \return a reference to this builder
   */
  template <typename T>
  Builder& QueryParameterAt(T&& key) {
    std::stringstream ss_key;
    ss_key << std::forward<T>(key);
    if (ss_key.fail()) {
      throw std::invalid_argument("Not able to convert key to string!");
    }

    StringView str(STRING_TO_STRINGVIEW(ss_key.str()));
    auto param_it = uri_data_.query_.Find(str);
    // replace key
    if (param_it != uri_data_.query_.parameters_.end()) {
      uri_data_.query_.parameters_.erase(param_it);
    }
    return *this;
  }
  /** \brief Replaces an existing paramater (key + value)
   *
   *  If oldkey exists, then it is replaced (including its value) by newkey and newvalue. Otherwise
   *  no action is performed. If no old value exists, it is set.
   *
   * Throws std::invalid_argument if parsing fails
   * \param oldkey a key
   * \param newkey a value
   * \param newvalue a value
   * \return a reference to this builder
   */
  template <typename T, typename U, typename V>
  Builder& QueryParameterAt(T&& oldkey, U&& newkey, V&& newvalue) {
    std::stringstream ss_oldkey, ss_newkey, ss_newvalue;
    ss_oldkey << std::forward<T>(oldkey);
    if (ss_oldkey.fail()) {
      throw std::invalid_argument("Not able to convert old key to string!");
    }

    ss_newkey << std::forward<U>(newkey);
    if (ss_newkey.fail()) {
      throw std::invalid_argument("Not able to convert new key to string!");
    }

    ss_newvalue << std::forward<V>(newvalue);
    if (ss_newvalue.fail()) {
      throw std::invalid_argument("Not able to convert new value to string!");
    }

    for (auto& param : uri_data_.query_.parameters_) {
      if (param.key_.compare(ss_oldkey.str()) == 0) {
        param.key_ = ss_newkey.str();
        param.value_ = ss_newvalue.str();
      }
    }
    return *this;
  }

  /** \brief Sets the fragment component of a URI
   * Throws std::invalid_argument if parsing fails
   * \param value a value of an output-streamable type
   * \return a reference to this builder
   */
  template <typename T>
  Builder& Fragment(T&& value) {
    std::stringstream ss_value;
    ss_value << std::forward<T>(value);
    if (ss_value.fail()) {
      throw std::invalid_argument("Cannot convert fragment value to string stream");
    }
    uri_data_.fragment_ = ss_value.str();
    return *this;
  }

  /** \brief Clears the fragment component
   * \return a reference to this builder
   */
  Builder& Fragment() {
    uri_data_.fragment_ = "";
    return *this;
  }

  /** \brief Returns a Uri
   *  \return Uri
   */
  Uri ToUri() const;

 protected:
  /**
   * \brief auxiliary template for setting uri data members
   * \param value
   * \param exception
   *\return
   */
  template <typename T>
  String GetUriDataMemberString(T&& value, String&& exception) {
    std::stringstream ss;
    ss << std::forward<T>(value);
    if (ss.fail()) {
      throw std::invalid_argument(std::move(exception));
    }
    return ss.str();
  }

  /**
   * enum to index parts of Uri within vector
   */
  enum UriSeg : std::uint8_t {
    SchemeIndex,     ///< Denotes URI scheme according to RFC 3986
    AuthorityIndex,  ///< Denotes URI authority host according to RFC 3986
    UserInfoIndex,   ///< Denotes URI user info according to RFC 3986
    HostIndex,       ///< Denotes URI authority host according to RFC 3986
    PortIndex,       ///< Denotes URI authority port according to RFC 3986
    PathIndex,       ///< Denotes URI path according to RFC 3986
    QueryIndex,      ///< Denotes URI query according to RFC 3986
    FragmentIndex,   ///< Denotes URI fragment according to RFC 3986
    UriPartSize
  };

  /**
   * Type for position of Uri part
   */
  using PartPos = std::array<std::size_t, UriSeg::UriPartSize>;

  /**
   * Defines existence of Uri part
   */
  using PartExist = std::array<bool, UriSeg::UriPartSize>;

  /**
   *
   * \param uri
   *\return
   */
  Uri::Impl ParseUri(StringView uri);

  /**
   *
   * \param uristr
   * \param part_exists
   * \param begin_pos
   * \param end_pos
   *\return
   */
  Uri::Impl BuildUri(StringView uristr, PartExist& part_exists, const PartPos& begin_pos, const PartPos& end_pos);

  /**
   *
   * \param querystr
   *\return
   */
  Uri::Query BuildQuery(StringView querystr);

  /**
   *
   * \param paramstr
   *\return
   */
  Uri::Query::Parameter BuildParameter(StringView paramstr);

  /**
   *
   * \param key
   * \param value
   *\return
   */
  Uri::Query::Parameter CreateParameter(const String& key, const String& value = "");

  /**
   *
   * \param pathstr
   * \return path
   */
  Uri::Path BuildPath(StringView pathstr);

  /**
   * \brief the uri_ object constructed by the builder.
   */
  Uri::Impl uri_data_;
};

/** \brief Resolves a relative URI against a base URI
 *
 * See section 5.2 of RFC 3986 for the algorithm used.
 *
 *  \param base the URI to resolve against
 *  \param rel a relative URI
 *  \param alloc an allocator
 *  \return a resolved URI
 */
Uri Resolve(const Uri& base, const Uri& rel, Allocator* alloc = GetDefaultAllocator());

/** \brief Normalizes a given URI
 *
 *  \param uri URI to normalize
 *  \param alloc an allocator
 *  \return a noralized URI
 */
Uri Normalize(const Uri& uri, Allocator* alloc = GetDefaultAllocator());

/** \brief Relativizes a URI with respect to a given base URI
 *
 * The relativization of the given URI against this URI is computed as follows:
 *
 * If either the base URI or the given URI are opaque, or if the scheme and authority components of the
 * two URIs are not identical, or if the path of the base URI is not a prefix of the path of the given
 * URI, then the given URI is returned.
 *
 * Otherwise a new relative hierarchical URI is constructed with query and fragment components taken
 * from the given URI and with a path component computed by removing this URI's path from the
 * beginning of the given URI's path.
 *
 *  \param base a base URI as reference
 *  \param uri a URI to relativize
 *  \param alloc an allocator
 *  \return a relative URI
 */
Uri Relativize(const Uri& base, const Uri& uri, Allocator* alloc = GetDefaultAllocator());

/**
 * \brief Decode string
 * \param value The string to decode
 * \return The decode string
 */
String Decode(StringView value);

/**
 * \brief Encode string
 * \param value The string to encode
 * \return The encoded string
 */
String Encode(StringView value);

/**
 *
 * \param def
 * \return
 */
template <typename T>
T Uri::GetFragmentAs(T&& def) const {
  if (this->HasFragment()) {
    typename std::remove_reference<T>::type tmp = def;
    std::stringstream str;
    str.str(this->data_->fragment_);
    str >> def;
    if (str.fail()) {
      def = tmp;
    }
  }
  return std::forward<T>(def);
}

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_URI_H_

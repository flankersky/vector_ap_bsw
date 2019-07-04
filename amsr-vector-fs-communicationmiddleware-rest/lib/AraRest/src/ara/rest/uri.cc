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
/**        \file  uri.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/rest/uri.h"

#include <vac/language/cpp14_backport.h>

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>

#include "ara/rest/string.h"

namespace ara {
namespace rest {

/**
 * \brief Empty string.
 */
static StringView EMPTY;

Uri::Impl::Impl()
    : scheme_(), user_info_(), host_(), port_(PORT_NOT_FOUND), path_(), query_(), fragment_(), uri_str_() {}

bool Uri::HasScheme() const noexcept {
  bool ret_val = (data_) ? (!data_->scheme_.empty()) : false;
  return ret_val;
}

StringView Uri::GetScheme() const noexcept(std::is_nothrow_constructible<StringView>::value) {
  if (data_) {
    StringView str_view(STRING_TO_STRINGVIEW(data_->scheme_));
    return str_view;
  } else {
    return EMPTY;
  }
}

bool Uri::HasUserInfo() const noexcept {
  bool ret_val = (data_) ? (!data_->user_info_.empty()) : false;
  return ret_val;
}

StringView Uri::GetUserinfo() const noexcept(std::is_nothrow_constructible<StringView>::value) {
  if (data_) {
    StringView str_view(STRING_TO_STRINGVIEW(data_->user_info_));
    return str_view;
  } else {
    return EMPTY;
  }
}

bool Uri::HasHost() const noexcept {
  bool ret_val = (data_) ? (!data_->host_.empty()) : false;
  return ret_val;
}

StringView Uri::GetHost() const noexcept(std::is_nothrow_constructible<StringView>::value) {
  if (data_) {
    StringView str_view(STRING_TO_STRINGVIEW(data_->host_));
    return str_view;
  } else {
    return EMPTY;
  }
}

bool Uri::HasPort() const noexcept {
  bool ret_val = false;
  if (data_) {
    ret_val = (data_->port_ != PORT_NOT_FOUND);
  }
  return ret_val;
}

int Uri::GetPort() const noexcept {
  assert(data_);
  return data_->port_;
}

bool Uri::HasPath() const noexcept {
  bool ret_val = (data_) ? (data_->path_.NumSegments()) : false;
  return ret_val;
}

const Uri::Path& Uri::GetPath() const noexcept {
  assert(data_);
  return data_->path_;
}

bool Uri::HasQuery() const noexcept {
  bool ret_val = (data_) ? (data_->query_.NumParameters()) : false;
  return ret_val;
}

const Uri::Query& Uri::GetQuery() const noexcept {
  assert(data_);
  return data_->query_;
}

bool Uri::HasFragment() const noexcept { return (data_ ? (!data_->fragment_.empty()) : false); }

StringView Uri::GetFragment() const noexcept {
  if (data_) {
    StringView str_view(STRING_TO_STRINGVIEW(data_->fragment_));
    return str_view;
  } else {
    return EMPTY;
  }
}

bool Uri::IsEmpty() const noexcept {
  bool ret_val;
  if (data_) {
    ret_val = !(HasFragment() || HasHost() || HasPath() || HasPort() || HasScheme() || HasUserInfo() || HasQuery());
  } else {
    ret_val = true;
  }
  return ret_val;
}

bool Uri::IsRelative() const noexcept { return !HasScheme(); }

bool Uri::IsOpaque() const noexcept {
  /* Description:
   * URI scheme specifications can define opaque identifiers by
   disallowing use of slash characters, question mark characters, and
   the URIs "scheme:." and "scheme:..".
   */
  assert(data_);
  bool has_dots = false;
  if (HasPath()) {
    has_dots = ((((*data_->path_.GetSegments().begin()).Get().compare(CSTRING_TO_STRINGVIEW(".")) == 0)) ||
                (((*data_->path_.GetSegments().begin()).Get().compare(CSTRING_TO_STRINGVIEW("..")) == 0)));
  }
  return (Uri::IsPathRootless() && !has_dots && (data_->path_.NumSegments() == 1) && !HasQuery());
}

bool Uri::IsHierarchical() const noexcept {
  return ((HasHost() && IsPathAbEmpty()) || IsPathAbsolute() || IsPathRootless() || HasPath());
}

bool Uri::IsPathAbsolute() const noexcept {
  assert(data_);
  // TODO(visjop): Check second condition of / path-absolute   ; begins with "/" but not "//"
  return (data_->path_.HasLeadingSlash());
}

bool Uri::IsPathNoScheme() const noexcept {
  assert(data_);
  return (!data_->path_.HasLeadingSlash() && !HasScheme());
}

bool Uri::IsPathAbEmpty() const noexcept { return (Uri::IsPathAbsolute() || !Uri::HasPath()); }

bool Uri::IsPathRootless() const noexcept {
  assert(data_);
  return (!data_->path_.HasLeadingSlash() && HasScheme());
}

/************************************************/
/****************URI::PATH***********************/
/************************************************/
std::size_t Uri::Path::NumSegments() const noexcept { return segments_.size(); }

/************************************************/
/****************URI::PATH::SEGMENT**************/
/************************************************/

StringView Uri::Path::Segment::Get() const noexcept(std::is_nothrow_constructible<StringView>::value) {
  if (!segment_.empty()) {
    StringView str_view(STRING_TO_STRINGVIEW(segment_));
    return str_view;
  } else {
    return StringView(EMPTY);
  }
}

/************************************************/
/****************URI::QUERY**********************/
/************************************************/

Uri::Query::IteratorRange::Iterator Uri::Query::Find(StringView key) const noexcept {
  Uri::Query::IteratorRange::Iterator res_it = parameters_.end();
  for (Uri::Query::IteratorRange::Iterator param = parameters_.begin(); param != parameters_.end(); param++) {
    if (key.compare(param->GetKey()) == 0) {
      res_it = param;
      break;
    }
  }
  return res_it;
}

std::size_t Uri::Query::NumParameters() const noexcept { return parameters_.size(); }

const Uri::Query::Parameter& Uri::Query::GetParameter(std::size_t parameter_number) const noexcept {
  return parameters_.at(parameter_number);
}

bool Uri::Query::HasKey(StringView requested_key) noexcept {
  for (auto const& param : parameters_) {
    if (param.GetKey().compare(requested_key) == 0) {
      return true;
    }
  }
  return false;
}

/************************************************/
/****************URI::QUERY::PARAMETER***********/
/************************************************/

StringView Uri::Query::Parameter::GetKey() const noexcept {
  if (!key_.empty()) {
    StringView str_view(STRING_TO_STRINGVIEW(key_));
    return str_view;
  } else {
    return StringView(EMPTY);
  }
}

bool Uri::Query::Parameter::HasValue() const noexcept { return (!value_.empty()); }

StringView Uri::Query::Parameter::GetValue() const noexcept {
  if (!value_.empty()) {
    StringView str_view(STRING_TO_STRINGVIEW(value_));
    return str_view;
  } else {
    return StringView(EMPTY);
  }
}

/************************************************/
/****************URI::Builder********************/
/************************************************/

Uri::Builder::Builder(Allocator* allocator) : uri_data_() { (void)allocator; }

Uri::Builder::Builder(const Uri& uri, Allocator* allocator) : uri_data_(*uri.data_) { (void)allocator; }

Uri::Builder::Builder(Uri&& uri, Allocator* allocator) : uri_data_(*uri.data_) { (void)allocator; }

Uri::Builder::Builder(StringView uri, Allocator* alloc) : uri_data_() {
  (void)alloc;
  uri_data_ = ParseUri(uri);
}

Uri Uri::Builder::ToUri() const {
  Uri uri;
  uri.data_ = std::shared_ptr<Uri::Impl>(new Uri::Impl(uri_data_));
  return uri;
}

Uri::Query Uri::Builder::BuildQuery(StringView querystr) {
  Uri::Query query;
  String temp_str;
  STRINGVIEW_TO_STRING(temp_str, querystr);
  std::stringstream buf(temp_str);
  String token_str;
  while (std::getline(buf, token_str, '&')) {
    query.parameters_.emplace_back(BuildParameter(StringView(STRING_TO_STRINGVIEW(token_str))));
  }
  return query;
}

Uri::Query::Parameter Uri::Builder::BuildParameter(StringView paramstr) {
  // Split parameters
  Uri::Query::Parameter param;

  if (paramstr.size() > 0) {
    auto value_begin = std::find(paramstr.cbegin(), paramstr.cend(), '=');

    if (paramstr.at(0) == '?') {
      // remove the leading '?' from key

      String str;
      STRINGVIEW_TO_STRING(str, paramstr);
      param.key_ = String(++(std::begin(str), value_begin));
      param.key_ = Decode(StringView(STRING_TO_STRINGVIEW(param.key_)));
    } else {
      param.key_ = String(paramstr.cbegin(), value_begin);
      param.key_ = Decode(StringView(STRING_TO_STRINGVIEW(param.key_)));
    }
    if (value_begin != paramstr.cend()) {
      param.value_ = String(++value_begin, paramstr.cend());
      param.value_ = Decode(StringView(STRING_TO_STRINGVIEW(param.value_)));
    }
  }
  return param;
}

Uri::Query::Parameter Uri::Builder::CreateParameter(const String& key, const String& value) {
  // Split parameters
  Uri::Query::Parameter param;
  param.key_ = key;
  param.value_ = value;
  return param;
}

Uri::Path Uri::Builder::BuildPath(StringView pathstr) {
  // Divide into Segments
  // erase the leading '/'
  Uri::Path path;

  String temp_str;
  STRINGVIEW_TO_STRING(temp_str, pathstr);

  path.trailing_slash_ = (pathstr.back() == '/');
  if (path.trailing_slash_) {
    temp_str.pop_back();
  }
  // erase the leading '/'
  path.leading_slash_ = (pathstr.at(0) == '/');
  if (path.leading_slash_) {
    path.leading_slash_ = true;

    temp_str.erase(0, 1);
  }
  std::stringstream buf;
  buf.str(temp_str);

  // StringView token;
  String token_str;
  while (std::getline(buf, token_str, '/')) {
    path.segments_.push_back(Uri::Path::Segment(StringView(STRING_TO_STRINGVIEW(token_str))));
  }
  return path;
}

Uri::Impl Uri::Builder::BuildUri(StringView uristr, PartExist& part_exists, const PartPos& begin_pos,
                                 const PartPos& end_pos) {
  Uri::Impl data;

  // Scheme
  if (part_exists[SchemeIndex]) {
    StringView scheme_view(uristr.substr(begin_pos[SchemeIndex], end_pos[SchemeIndex] - begin_pos[SchemeIndex]));
    STRINGVIEW_TO_STRING(data.scheme_, scheme_view);
  }

  // Authority
  if (part_exists[HostIndex]) {
    // User Info
    StringView info_view(uristr.substr(begin_pos[UserInfoIndex], end_pos[UserInfoIndex] - begin_pos[UserInfoIndex]));
    if (part_exists[UserInfoIndex]) {
      STRINGVIEW_TO_STRING(data.user_info_, info_view);
    }

    // Host
    StringView host_view(uristr.substr(begin_pos[HostIndex], end_pos[HostIndex] - begin_pos[HostIndex]));
    STRINGVIEW_TO_STRING(data.host_, host_view);

    // Port
    if (part_exists[PortIndex]) {
      StringView port_view(uristr.substr(begin_pos[PortIndex], end_pos[PortIndex] - begin_pos[PortIndex]));
      String temp;
      STRINGVIEW_TO_STRING(temp, port_view);
      data.port_ = std::stoi(temp);
    }
  }

  // Path
  if (part_exists[PathIndex]) {
    data.path_ = BuildPath(uristr.substr(begin_pos[PathIndex], end_pos[PathIndex] - begin_pos[PathIndex]));
  }

  // Query
  if (part_exists[QueryIndex]) {
    data.query_ = BuildQuery(uristr.substr(begin_pos[QueryIndex], end_pos[QueryIndex] - begin_pos[QueryIndex]));
  }

  // Fragment
  if (part_exists[FragmentIndex]) {
    data.fragment_ = Decode(uristr.substr(begin_pos[FragmentIndex], end_pos[FragmentIndex] - begin_pos[FragmentIndex]));
  }

  return data;
}

Uri::Impl Uri::Builder::ParseUri(StringView uri) {
  PartExist part_exists;
  part_exists.fill(false);

  PartPos begin_pos, end_pos;
  begin_pos.fill(0);
  end_pos.fill(uri.size());

  // Search for delimiters
  begin_pos[AuthorityIndex] = uri.find("//", 0, strlen("//"));
  end_pos[SchemeIndex] = uri.find(":", 0, strlen(":"));
  begin_pos[FragmentIndex] = uri.find("#", 0, strlen("#"));
  begin_pos[QueryIndex] = uri.find("?", 0, strlen("?"));
  if (begin_pos[QueryIndex] > begin_pos[FragmentIndex]) {
    begin_pos[QueryIndex] = std::string::npos;
  }

  // Determine end position of authority/host, when existing
  if (begin_pos[AuthorityIndex] != std::string::npos) {
    begin_pos[AuthorityIndex] += 2;  // offset of delimiter
    part_exists[AuthorityIndex] = true;
    if (end_pos[SchemeIndex] < begin_pos[AuthorityIndex]) {
      part_exists[SchemeIndex] = true;
    }
    // Search for path delimiter
    end_pos[AuthorityIndex] = uri.substr(begin_pos[AuthorityIndex]).find("/", 0, strlen("/"));
    if (end_pos[AuthorityIndex] != std::string::npos) {
      end_pos[AuthorityIndex] += begin_pos[AuthorityIndex];
    }
  } else if (end_pos[SchemeIndex] != std::string::npos) {
    part_exists[SchemeIndex] = true;
    end_pos[AuthorityIndex] = end_pos[SchemeIndex] + 1;
  } else {
    end_pos[AuthorityIndex] = 0;
  }

  // start position of path
  begin_pos[PathIndex] = end_pos[AuthorityIndex];

  // Fragment
  if (begin_pos[FragmentIndex] != std::string::npos) {
    part_exists[FragmentIndex] = true;
    end_pos[QueryIndex] = begin_pos[FragmentIndex];
    end_pos[PathIndex] = end_pos[QueryIndex];
    begin_pos[FragmentIndex]++;  // offset of delimiter
  }

  // Query
  if (begin_pos[QueryIndex] != std::string::npos) {
    part_exists[QueryIndex] = true;
    end_pos[PathIndex] = begin_pos[QueryIndex];
    begin_pos[QueryIndex]++;  // offset of delimiter
  }

  // end position of path
  if (begin_pos[PathIndex] < end_pos[PathIndex]) {
    part_exists[PathIndex] = true;
  }

  // Parse authority
  if (part_exists[AuthorityIndex]) {
    StringView authority = uri.substr(begin_pos[AuthorityIndex], end_pos[AuthorityIndex] - begin_pos[AuthorityIndex]);
    // Search for user and port delimiter
    end_pos[UserInfoIndex] = authority.find("@", 0, strlen("@"));

    // Initialze Host position
    part_exists[HostIndex] = true;
    begin_pos[HostIndex] = begin_pos[AuthorityIndex];
    end_pos[HostIndex] = end_pos[AuthorityIndex];

    // User Info
    if (end_pos[UserInfoIndex] != std::string::npos) {
      part_exists[UserInfoIndex] = true;
      end_pos[UserInfoIndex] += begin_pos[AuthorityIndex];
      begin_pos[UserInfoIndex] = begin_pos[AuthorityIndex];
      begin_pos[HostIndex] = end_pos[UserInfoIndex] + 1;
    }

    begin_pos[PortIndex] =
        uri.substr(begin_pos[HostIndex], end_pos[AuthorityIndex] - begin_pos[HostIndex]).find(":", 0, strlen(":"));
    // Port
    if (begin_pos[PortIndex] != std::string::npos) {
      part_exists[PortIndex] = true;
      end_pos[PortIndex] = end_pos[AuthorityIndex];
      end_pos[HostIndex] = begin_pos[PortIndex] + begin_pos[HostIndex];
      begin_pos[PortIndex] = end_pos[HostIndex] + 1;
    }
  }

  return BuildUri(uri, part_exists, begin_pos, end_pos);
}

/************************************************/
/****************URI Utilities*******************/
/************************************************/

/**
 * \brief Encode string
 */
String Encode(StringView to_encode) {
  String out;
  out.reserve(256);  // reserve a buffer up front to avoid re-allocations.
  std::ostringstream encoded(out);
  encoded.fill('0');    // fill character used to pad the output conversion.
  encoded << std::hex;  // set numeric base of encoded to hex.

  for (auto i = to_encode.cbegin(); i != to_encode.cend(); i++) {
    // Keep alphanumeric and other non reserved characters intact
    if (std::isalnum(*i) || *i == '-' || *i == '_' || *i == '.' || *i == '~') {
      encoded << *i;
      continue;
    }
    // percent-encoded characters
    encoded << std::uppercase;
    encoded << '%' << std::setw(2) << static_cast<unsigned int>(*i);
    encoded << std::nouppercase;
  }
  return encoded.str();
}

/**
 * \brief Decode String
 */
String Decode(StringView to_decode) {
  std::ostringstream decoded;
  decoded.fill('0');    // fill character used to pad the output conversion.
  decoded << std::hex;  // set numeric base of decoded to hex.
  for (auto i = to_decode.cbegin(); i != to_decode.cend(); i++) {
    // Keep alphanumeric and other non reserved characters intact
    if (std::isalnum(*i) || *i == '-' || *i == '_' || *i == '.' || *i == '~') {
      decoded << *i;
      continue;
    }
    // percent-decoded characters
    i++;  // skip the '%' character
    if (i == to_decode.cend()) {
      // an unexpected character occurred
      // e.g. * at the end
      throw IllegalUriFormatException("Illegal uri format: unexpected character");  // TODO(?) ERRORHANDLING
    }
    String tmp{*i};
    i++;
    tmp += *i;  // string representation of the value e.g. "20" for %20
    char* end;
    std::int64_t result = std::strtol(tmp.c_str(), &end, 16);
    if (*end == *tmp.end()) {
      // conversion complete
      decoded << static_cast<char>(result);
    }
  }
  return decoded.str();
}

namespace {

/**
 * \brief Append query as string
 */
static void AppendQueryAsString(const Uri& in, String* out, bool encode = false) {
  assert(out);
  if (in.HasQuery()) {
    *out += "?";
    for (unsigned char i = 0; i < in.GetQuery().NumParameters(); i++) {
      if (i != 0) {
        *out += "&";
      }
      if (encode) {
        *out += Encode(in.GetQuery().GetParameter(i).GetKey());
        *out += "=";
        *out += Encode(in.GetQuery().GetParameter(i).GetValue());
      } else {
        STRINGVIEW_TO_STRING((*out), in.GetQuery().GetParameter(i).GetKey());
        *out += "=";
        STRINGVIEW_TO_STRING((*out), in.GetQuery().GetParameter(i).GetValue());
      }
    }
  }
}

/**
 *
 * \param seg
 * \return
 */
StringView GetSegmentString(const String& seg) { return StringView(STRING_TO_STRINGVIEW(seg)); }

/**
 *
 * \param seg
 * \return
 */
StringView GetSegmentString(const Uri::Path::Segment& seg) { return seg.Get(); }

/**
 * \brief Append path to string.
 */
template <typename T>
static void AppendPathAsString(const IteratorRange<T>& in, String* out, bool encode = false, bool leading_slash = true,
                               bool trailing_slash = false) {
  assert(out);
  bool first = true;
  for (auto& segment : in) {
    if (first == true) {
      if (leading_slash) {
        *out += "/";
      }
      first = false;
    } else {
      *out += "/";
    }
    if (encode) {
      *out += Encode(GetSegmentString(segment));
    } else {
      StringView tmp = GetSegmentString(segment);
      STRINGVIEW_TO_STRING((*out), tmp);
    }
  }
  if (trailing_slash) {
    *out += "/";
  }
}

/**
 * \brief Append scheme to string.
 */
static void AppendSchemeAsString(const Uri& in, String* out, bool encode = false) {
  assert(out);
  if (in.HasScheme()) {
    if (encode) {
      *out += Encode(in.GetScheme());
    } else {
      STRINGVIEW_TO_STRING((*out), in.GetScheme());
    }
    *out += ":";
  }
}

/**
 * \brief Append user info to string.
 */
static void AppendUserInfoAsString(const Uri& in, String* out, bool encode = false) {
  assert(out);
  if (in.HasUserInfo()) {
    if (encode) {
      *out += Encode(in.GetUserinfo());
    } else {
      STRINGVIEW_TO_STRING((*out), in.GetUserinfo());
    }
    *out += "@";
  }
}

/**
 * \brief Append host to string.
 */
static void AppendHostAsString(const Uri& in, String* out, bool encode = false) {
  assert(out);
  if (in.HasHost()) {
    if (encode) {
      *out += Encode(in.GetHost());
    } else {
      STRINGVIEW_TO_STRING((*out), in.GetHost());
    }
  }
}

/**
 * \brief Append port to string.
 */
static void AppendPortAsString(const Uri& in, String* out) {
  assert(out);
  if (in.HasPort()) {
    *out += ":";
    *out += std::to_string(in.GetPort());
  }
}

/**
 * \brief Append fragment as string.
 */
static void AppendFragmentAsString(const Uri& in, String* out, bool encode = false) {
  assert(out);
  if (in.HasFragment()) {
    *out += "#";
    if (encode) {
      *out += Encode(in.GetFragment());
    } else {
      STRINGVIEW_TO_STRING((*out), in.GetFragment());
    }
  }
}

}  // namespace

String ToString(const Uri& uri, Uri::Part part, Allocator* alloc) { return ToString(uri, part, false, alloc); }

String ToString(const Uri& uri, Allocator* alloc) {
  (void)alloc;
  return ToString(uri, Uri::Part::All);
}

String ToString(const Uri& uri, Uri::Part part, bool encode, Allocator* alloc) {
  (void)alloc;
  String uri_string;
  uri_string.reserve(Uri::LENGTH_MAX);  // reserve a buffer up front to avoid re-allocations.

  if ((part & Uri::Part::Scheme) == Uri::Part::Scheme) {
    AppendSchemeAsString(uri, &uri_string, encode);
  }
  if ((part & Uri::Part::Host) == Uri::Part::Host) {
    if (uri.HasHost()) {
      uri_string += "//";
    }
  }
  if ((part & Uri::Part::UserInfo) == Uri::Part::UserInfo) {
    AppendUserInfoAsString(uri, &uri_string, encode);
  }
  if ((part & Uri::Part::Host) == Uri::Part::Host) {
    AppendHostAsString(uri, &uri_string, encode);
  }
  if ((part & Uri::Part::Port) == Uri::Part::Port) {
    AppendPortAsString(uri, &uri_string);
  }
  if ((part & Uri::Part::Path) == Uri::Part::Path) {
    if (uri.HasPath()) {
      AppendPathAsString(uri.GetPath().GetSegments(), &uri_string, encode, uri.GetPath().leading_slash_,
                         uri.GetPath().trailing_slash_);
    }
  }
  if ((part & Uri::Part::Query) == Uri::Part::Query) {
    AppendQueryAsString(uri, &uri_string, encode);
  }
  if ((part & Uri::Part::Fragment) == Uri::Part::Fragment) {
    AppendFragmentAsString(uri, &uri_string, encode);
  }
  return uri_string;
}

String ToString(Uri&& uri, Uri::Part part, Allocator* alloc) { return ToString(uri, part, alloc); }

String ToString(Uri&& uri, Allocator* alloc) { return ToString(uri, alloc); }

static void RemoveDotSegments(Uri::Path path, Uri::Builder& builder, bool leading_dots = false) {
  std::vector<String> dotless_strpath;
  for (auto& segment : path.GetSegments()) {
    if (leading_dots && ((segment.Get().compare(CSTRING_TO_STRINGVIEW("..")) == 0) ||
                         (segment.Get().compare(CSTRING_TO_STRINGVIEW("."))) == 0)) {
      String temp;
      STRINGVIEW_TO_STRING(temp, segment.Get());
      dotless_strpath.push_back(temp);
    } else {
      if (leading_dots) {
        leading_dots = false;
      }
      if (segment.Get().compare(CSTRING_TO_STRINGVIEW("..")) == 0) {
        if (!dotless_strpath.empty()) {
          dotless_strpath.pop_back();
        }
      } else if (segment.Get().compare(CSTRING_TO_STRINGVIEW(".")) == 0) {
        // Do nothing
      } else {
        String temp;
        STRINGVIEW_TO_STRING(temp, segment.Get());
        dotless_strpath.push_back(temp);
      }
    }
  }

  String strpath;
  AppendPathAsString(IteratorRange<std::vector<String>::const_iterator>{dotless_strpath.begin(), dotless_strpath.end()},
                     &strpath, false, path.HasLeadingSlash(), path.HasTrailingSlash());

  builder.Path(StringView(STRING_TO_STRINGVIEW(strpath)));
}

static void MergePaths(Uri::Path abspath, Uri::Path relpath, bool has_host, Uri::Builder& builder) {
  String merge_str;
  AppendPathAsString(abspath.GetSegments(), &merge_str, false, abspath.HasLeadingSlash(), abspath.HasTrailingSlash());

  std::size_t pos;

  if (!has_host) {
    merge_str = '/';
  }
  if (merge_str.back() != '/') {
    pos = merge_str.rfind('/') + 1;
    merge_str.erase(pos, merge_str.length() - pos);
  }
  AppendPathAsString(relpath.GetSegments(), &merge_str, false, relpath.HasLeadingSlash(), relpath.HasTrailingSlash());
  builder.Path(StringView(STRING_TO_STRINGVIEW(merge_str)));
}

Uri Resolve(const Uri& base, const Uri& rel, Allocator* alloc) {
  (void)alloc;
  Uri::Builder builder;

  String temp_scheme;
  String temp_userinfo;
  String temp_host;
  String temp_fragment;

  STRINGVIEW_TO_STRING(temp_scheme, rel.GetScheme());
  STRINGVIEW_TO_STRING(temp_userinfo, rel.GetUserinfo());
  STRINGVIEW_TO_STRING(temp_host, rel.GetHost());
  STRINGVIEW_TO_STRING(temp_fragment, rel.GetFragment());

  if (rel.HasScheme()) {
    builder.Scheme(temp_scheme).UserInfo(temp_userinfo).Host(temp_host).Port(rel.GetPort()).Query(rel.GetQuery());
    RemoveDotSegments(rel.GetPath(), builder);
  } else {
    if (rel.HasHost()) {
      builder.Scheme(temp_scheme).UserInfo(temp_userinfo).Host(temp_host).Port(rel.GetPort()).Query(rel.GetQuery());
      RemoveDotSegments(rel.GetPath(), builder);
    } else {
      if (rel.HasPath()) {
        if (ToString(rel, Uri::Part::Path).at(0) == '/') {
          RemoveDotSegments(rel.GetPath(), builder);
        } else {
          MergePaths(base.GetPath(), rel.GetPath(), base.HasHost(), builder);
          RemoveDotSegments(builder.ToUri().GetPath(), builder);
        }
        builder.Query(rel.GetQuery());
      } else {
        builder.Path(base.GetPath());
        if (rel.HasQuery()) {
          builder.Query(rel.GetQuery());
        } else if (base.HasQuery()) {
          builder.Query(base.GetQuery());
        } else {
          /* Do nothing */
        }
      }

      temp_userinfo.clear();
      temp_host.clear();
      STRINGVIEW_TO_STRING(temp_userinfo, base.GetUserinfo());
      STRINGVIEW_TO_STRING(temp_host, base.GetHost());
      builder.Port(base.GetPort());
      builder.UserInfo(temp_userinfo);
      builder.Host(temp_host);
    }
    temp_scheme.clear();
    STRINGVIEW_TO_STRING(temp_scheme, base.GetScheme());
    builder.Scheme(temp_scheme);
  }
  builder.Fragment(temp_fragment);
  return builder.ToUri();
}
/**
 * Remove all . and .. segments except leading to normalize Uri
 */
Uri Normalize(const Uri& uri, Allocator* alloc) {
  (void)alloc;
  Uri::Builder builder;

  String temp_scheme;
  String temp_userinfo;
  String temp_host;
  String temp_fragment;

  STRINGVIEW_TO_STRING(temp_scheme, uri.GetScheme());
  STRINGVIEW_TO_STRING(temp_userinfo, uri.GetUserinfo());
  STRINGVIEW_TO_STRING(temp_host, uri.GetHost());
  STRINGVIEW_TO_STRING(temp_fragment, uri.GetFragment());

  // Initialize builder
  builder.Scheme(temp_scheme)
      .UserInfo(temp_userinfo)
      .Host(temp_host)
      .Port(uri.GetPort())
      .Query(uri.GetQuery())
      .Fragment(temp_fragment);

  // Determine if there is leading . or ..
  bool leading_dots = false;
  if (!uri.HasHost() && uri.HasPath()) {
    String temp_segments;
    STRINGVIEW_TO_STRING(temp_userinfo, uri.GetPath().GetSegments().begin()->Get());
    if (strcmp(temp_userinfo.c_str(), "..") == 0 || strcmp(temp_userinfo.c_str(), ".") == 0) {
      leading_dots = true;
    }
  }
  // Remove all but leading dots
  RemoveDotSegments(uri.GetPath(), builder, leading_dots);

  return builder.ToUri();
}

/**
 *
 * \param base_depth
 * \param uri_depth
 * \param path
 */
static inline void AppendRelativePathPosition(std::size_t base_depth, std::size_t uri_depth, String& path,
                                              bool trailing_slash = false) {
  path += "./";
  if (!trailing_slash) {
    base_depth--;
  }
  // Resolve upper ladder path
  for (std::size_t i = uri_depth; i < base_depth; ++i) {
    path += "../";
  }
}
/**
 *
 * \param begin
 * \param end
 * \param path
 */
static inline void AppendRemainingPath(Uri::Path::Iterator begin, Uri::Path::Iterator end, String& path) {
  // Append remaining path
  for (auto it = begin; it != end; ++it) {
    STRINGVIEW_TO_STRING(path, it->Get());
    path += "/";
  }
}

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
 */
Uri Relativize(const Uri& base, const Uri& uri, Allocator* alloc) {
  (void)alloc;
  Uri ret_uri(uri);

  // Check if relative path can be constructed Uri
  if ((!base.IsOpaque() && !uri.IsOpaque()) && (base.GetScheme().compare(uri.GetScheme()) == 0) &&
      (base.GetUserinfo().compare(uri.GetUserinfo()) == 0) && (base.GetHost().compare(uri.GetHost()) == 0) &&
      (base.GetPort() == uri.GetPort()) && (base.HasPath() && uri.HasPath())) {
    auto base_it = base.GetPath().GetSegments().begin();
    auto uri_it = uri.GetPath().GetSegments().begin();
    if (uri_it->Get().compare(base_it->Get()) == 0) {
      Uri::Builder builder;
      String path_str;
      std::size_t depth = 0u;

      for (; uri_it != uri.GetPath().GetSegments().end(); ++uri_it) {
        if (base_it != base.GetPath().GetSegments().end()) {
          if (uri_it->Get().compare(base_it->Get()) != 0) {
            // Resolve upper ladder path
            AppendRelativePathPosition(depth, base.GetPath().NumSegments(), path_str,
                                       base.GetPath().HasTrailingSlash());

            // Append remaining path
            AppendRemainingPath(uri_it, uri.GetPath().GetSegments().end(), path_str);
            break;
          } else {
            // Increment path level/depth
            depth++;
            base_it++;
          }
        } else {
          // Append remaining path
          AppendRemainingPath(uri_it, uri.GetPath().GetSegments().end(), path_str);
          break;
        }
      }

      if (uri_it == uri.GetPath().GetSegments().end()) {
        // Uri path relative to upper of base
        AppendRelativePathPosition(base.GetPath().NumSegments(), depth, path_str, base.GetPath().HasTrailingSlash());
      }
      if (!uri.GetPath().HasTrailingSlash()) {
        // Remove trailing slash when necessary
        path_str.erase(path_str.size() - 1);
      }
      ret_uri = builder.Path(StringView(STRING_TO_STRINGVIEW(path_str))).ToUri();
    }
  }
  return ret_uri;
}

}  // namespace rest
}  // namespace ara

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
/**        \file  header.h
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_HEADER_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_HEADER_H_

#include "ara/rest/endpoint.h"
#include "ara/rest/types.h"
#include "ara/rest/uri.h"

namespace ara {
namespace rest {

/** \brief Represents the message header of a request
 *
 *  Request header is used in both client and server messages.
 *
 */
class RequestHeader {
 public:
  /**
   * Default CTOR
   */
  RequestHeader() = default;
  /**
   * Constructor
   * \param request_method
   * \param uri
   */
  RequestHeader(RequestMethod request_method, Uri uri);

  /** \brief Returns the request method
   *
   *  \return a request method
   */
  RequestMethod GetMethod() const noexcept;
  /** \brief Allows to set the request method
   *
   *  \param met a RequestMethod
   */
  void SetMethod(RequestMethod met);
  /** \brief Returns a Uri
   *
   *  It is binding-specific how Uri map to the transport protocol format.
   *  \return a Uri
   */
  const Uri& GetUri() const noexcept;
  /** \brief Allows to set a Uri
   *
   *  It is binding-specific how Uri map to the transport protocol format.
   *  \param uri a Uri
   */
  void SetUri(const Uri& uri);
  /** \brief Allows to set a Uri
   *
   *  It is binding-specific how Uri map to the transport protocol format.
   *  Same as before, just with move semantics
   *
   *  \param uri a Uri
   */
  void SetUri(Uri&& uri);

 private:
  /**
   * Member to save the RequestMethod
   */
  RequestMethod request_method_;

  /**
   * Member to save the Uri
   */
  Uri uri_;
};

/** \brief Represents the message header of a reply
 *
 *  Reply header is used in both client and server messages.
 *
 */
class ReplyHeader {
 public:
  /**
   * Default CTOR
   */
  ReplyHeader() = default;
  /**
   * Constructor
   * \param status
   * \param uri
   */
  ReplyHeader(int status, Uri uri);

  /** \brief Returns the current message status code
   *
   *  Status codes are binding-specific
   *
   *  \return a status code
   */
  int GetStatus() const noexcept;
  /** \brief Sets a message status code
   *
   *  Status codes are binding-specific
   *
   *  \param code an integral status code
   */
  void SetStatus(int code) noexcept;
  /** \brief Returns a Uri
   *
   *  It is binding-specific how Uri map to the transport protocol format.
   *
   *  \return a Uri
   */
  const Uri& GetUri() const noexcept;
  /** \brief Allows to set a Uri
   *
   *  It is binding-specific how Uri map to the transport protocol format.
   *
   *  \param uri a Uri
   */
  void SetUri(const Uri& uri);
  /** \brief Allows to set a Uri
   *
   *  It is binding-specific how Uri map to the transport protocol format.
   *  Same as before, just with move semantics
   *
   *  \param uri a Uri
   */
  void SetUri(Uri&& uri);

 private:
  /**
   * StatusCode for the ServerReply
   */
  int status_;

  /**
   * Uri for the ServerReply
   */
  Uri uri_;
};

}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_HEADER_H_

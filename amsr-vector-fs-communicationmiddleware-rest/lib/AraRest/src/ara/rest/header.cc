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
/**        \file  header.cc
 *        \brief  Header implementation for ara::rest::ServerReply and ara::rest:ServerRequest
 *
 *      \details  Header implementation for ara::rest::ServerReply and ara::rest:ServerRequest
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/rest/header.h"

namespace ara {
namespace rest {

RequestHeader::RequestHeader(RequestMethod request_method, Uri uri) : request_method_(request_method), uri_(uri) {}

RequestMethod RequestHeader::GetMethod() const noexcept { return request_method_; }

void RequestHeader::SetMethod(RequestMethod met) { request_method_ = met; }

const Uri& RequestHeader::GetUri() const noexcept { return uri_; }

void RequestHeader::SetUri(const Uri& uri) { uri_ = Uri(uri); }

void RequestHeader::SetUri(Uri&& uri) { uri_ = std::move(uri); }

ReplyHeader::ReplyHeader(int status, Uri uri) : status_(status), uri_(uri) {}

int ReplyHeader::GetStatus() const noexcept { return status_; }

void ReplyHeader::SetStatus(int code) noexcept { status_ = code; }

const Uri& ReplyHeader::GetUri() const noexcept { return uri_; }

void ReplyHeader::SetUri(const Uri& uri) { uri_ = Uri(uri); }

void ReplyHeader::SetUri(Uri&& uri) { uri_ = std::move(uri); }

}  // namespace rest
}  // namespace ara

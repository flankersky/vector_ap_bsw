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
/**        \file  json_writer.cc
 *        \brief  Only includes the header
 *
 *      \details  This file only includes the header, where all functionality is placed
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <iostream>

#include "ara/per/internal/exception/configuration_exceptions.h"
#include "ara/per/internal/json_writer.h"

namespace ara {
namespace per {
namespace internal {

namespace json {

void WriteFile(const JsonDocument& document, const std::string& path) {
  if (document.HasParseError()) {
    throw exception::ParserError();
  }

  StringBuffer strbuf;
  Writer<StringBuffer> writer(strbuf);
  document.Accept(writer);
  std::ofstream out(path);

  if (out.is_open()) {
    out << strbuf.GetString();
    out.close();
  } else {
    throw exception::FileNotFound(path);
  }
}

}  // namespace json
}  // namespace internal
}  // namespace per
}  // namespace ara

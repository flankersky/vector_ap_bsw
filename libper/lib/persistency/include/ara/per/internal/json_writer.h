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
/**        \file  json_writer.h
 *        \brief  Wrapps rapidjson and renames some rapidjson typdefs
 *
 *      \details  Wrapps the rapidjson library to rename the namespace and also rename some typedefs
 *                * rapidjson::Document is now ara::per::internal::JsonDocument
 *                * rapidjson::Value is now ara::per::internal::JsonValue
 *
 *********************************************************************************************************************/

#ifndef LIB_PERSISTENCY_INCLUDE_ARA_PER_INTERNAL_JSON_WRITER_H_
#define LIB_PERSISTENCY_INCLUDE_ARA_PER_INTERNAL_JSON_WRITER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/pointer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/schema.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <cinttypes>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <typeinfo>

namespace ara {
namespace per {
namespace internal {

namespace json {
using namespace rapidjson; /* NOLINT[Do not use using-directives] Used for unitary namespace naming when using
                            rapidjson objects */
/**
 * \brief rename typedef of rapidjson::Value to JsonValue
 */
typedef Value JsonValue;

/**
 * \brief rename typedef of rapidjson::Document to JsonDocument
 */
typedef Document JsonDocument;

/**
 * \brief Typedef for the rapidjson object type
 */
typedef JsonValue::Object JsonObject;

/**
 * \brief Typedef for the rapidjson const object type
 */
typedef JsonValue::ConstObject JsonConstObject;

/**
 * \brief Writes a JSON file
 *
 * \param document JSON document that should be written to file
 * \param path File path of JSON file
 * \return error code
 */
void WriteFile(const JsonDocument& document, const std::string& path);

}  // namespace json
}  // namespace internal
}  // namespace per
}  // namespace ara

#endif  // LIB_PERSISTENCY_INCLUDE_ARA_PER_INTERNAL_JSON_WRITER_H_

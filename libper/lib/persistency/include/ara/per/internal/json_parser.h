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
/**        \file  json_parser.h
 *        \brief  Wrapps rapidjson and renames some rapidjson typdefs
 *
 *      \details  Wrapps the rapidjson library to rename the namespace and also rename some typedefs
 *                * rapidjson::Document is now ara::per::internal::JsonDocument
 *                * rapidjson::Value is now ara::per::internal::JsonValue
 *
 *********************************************************************************************************************/

#ifndef LIB_PERSISTENCY_INCLUDE_ARA_PER_INTERNAL_JSON_PARSER_H_
#define LIB_PERSISTENCY_INCLUDE_ARA_PER_INTERNAL_JSON_PARSER_H_

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

#include "ara/per/internal/exception/configuration_exceptions.h"

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
 * \brief Load a JSON file from the given path.
 *
 * \param path The path to the JSON file to be read.
 * \return The JSON Object representing the root of the file.
 */
template <typename T>
JsonDocument LoadFile(const T& path) {
  std::ifstream configuration_file_stream(path);
  JsonDocument configuration;

  if (configuration_file_stream.is_open()) {
    IStreamWrapper configuration_file_stream_wrapper(configuration_file_stream);
    configuration.ParseStream(configuration_file_stream_wrapper);

    if (configuration.HasParseError()) {
      throw exception::ParserError();
    }
  } else {
    throw exception::FileNotFound(path);
  }

  return configuration;
}

/**
 * \brief Load a JSON file from the given path and validate it against the JSON schema file from the given schema path.
 *
 * \param path The path to the JSON file to be read.
 * \param schema_path The path to the JSON schema file to be used for validation.
 * \return The JSON Object representing the root of the file.
 */
template <typename T>
JsonDocument LoadFile(const T& path, const T& schema_path) {
  JsonDocument configuration = LoadFile(path);
  JsonDocument schema = LoadFile(schema_path);

  SchemaDocument sd(schema);
  SchemaValidator validator(sd);

  if (!configuration.Accept(validator)) {
    throw exception::ValidationError();
  }

  return configuration;
}

/**
 * \brief Parse a unsigned integer from a string representation
 *        Template parameters:
 *        INT_TYPE: unsigned integer type to be converted to, e.g. std::uint16_t
 *        BASE: Number base used for parsing. Default is base auto-detection (=0)
 * \param value The hex string to be converted
 * \return INTEGER_TYPE Parsed unsigned integer
 */
template <typename UINT_TYPE, int BASE = 0>
typename std::enable_if<std::is_unsigned<UINT_TYPE>::value && !std::is_floating_point<UINT_TYPE>::value,
                        UINT_TYPE>::type
GetNumber(const rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>>&
              value) {
  std::string str_value = value.GetString();
  std::size_t pos = 0;
  auto int_value = std::stoull(str_value, &pos, BASE);
  if (pos != str_value.size()) {
    std::string error_string = "String value '" + str_value + "' could not be parsed as number.";
    throw std::runtime_error(error_string);
  }
  if (int_value > std::numeric_limits<UINT_TYPE>::max()) {
    std::string error_string = "String value '" + str_value + "' is out of bounds of requested type.";
    throw std::runtime_error(error_string);
  }
  return static_cast<UINT_TYPE>(int_value);
}

/**
 * \brief Parse a signed integer from a string representation.
 *        Template parameters:
 *        INT_TYPE: signed integer type to be converted to, e.g. std::int16_t
 *        BASE: Number base used for parsing. Default is base auto-detection (=0)
 * \param value The hex string to be converted
 * \return INTEGER_TYPE Parsed unsigned integer
 */
template <typename INT_TYPE, int BASE = 0>
typename std::enable_if<std::is_signed<INT_TYPE>::value && !std::is_floating_point<INT_TYPE>::value, INT_TYPE>::type
GetNumber(const rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>>&
              value) {
  std::string str_value = value.GetString();
  std::size_t pos = 0;
  auto int_value = std::stoll(str_value, &pos, BASE);
  if (pos != str_value.size()) {
    std::string error_string = "String value '" + str_value + "' could not be parsed as number.";
    throw std::runtime_error(error_string);
  }
  if (int_value < std::numeric_limits<INT_TYPE>::min() || int_value > std::numeric_limits<INT_TYPE>::max()) {
    std::string error_string = "String value '" + str_value + "' is out of bounds of requested type.";
    throw std::runtime_error(error_string);
  }
  return static_cast<INT_TYPE>(int_value);
}

}  // namespace json
}  // namespace internal
}  // namespace per
}  // namespace ara

#endif  // LIB_PERSISTENCY_INCLUDE_ARA_PER_INTERNAL_JSON_PARSER_H_

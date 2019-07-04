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
/**        \file  serialize.h
 *        \brief  Header for json to OGM serializer and deserializer
 *
 *      \details  Contains the prototypes for serializer and deserializer
 *
 *********************************************************************************************************************/

#ifndef LIB_ARAREST_INCLUDE_ARA_REST_SERIALIZE_SERIALIZE_H_
#define LIB_ARAREST_INCLUDE_ARA_REST_SERIALIZE_SERIALIZE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <rapidjson/document.h>
#include <iostream>
#include <vector>

#include "ara/rest/ogm/array.h"
#include "ara/rest/ogm/copy.h"
#include "ara/rest/ogm/field.h"
#include "ara/rest/ogm/int.h"
#include "ara/rest/ogm/object.h"
#include "ara/rest/ogm/real.h"
#include "ara/rest/ogm/string.h"
#include "ara/rest/ogm/uri.h"
#include "ara/rest/ogm/uuid.h"
#include "ara/rest/ogm/visit.h"
#include "ara/rest/pointer.h"
#include "ara/rest/string.h"
#include "ara/rest/uuid.h"

namespace ara {
namespace rest {
namespace serialize {
/**
 * \brief This class for serializer functionality
 */
class Serializer {
 public:
  /**
   * \brief parse JSON string.
   *
   * \return ogm::Object pointer.
   * \param json_string
   */
  static Pointer<ogm::Object> JsonToOgm(String json_string);

  /**
   * \brief creates json document from ogm::Object.
   *
   * \param ogm_object .
   * \return json string.
   */
  static String OgmToJson(const Pointer<ogm::Object>& ogm_object);

 private:
  /**
   * \brief constructor. (added to avoid class instantiation)
   *
   */
  Serializer();

  /**
   * \brief creates ogm array from  json Object.
   *
   * \param array .
   * \param obj_name .
   * \return ogm::Array pointer.
   */
  static Pointer<ogm::Array> CreateOgmArray(const rapidjson::Value& array, String obj_name);

  /**
   * \brief creates ogm::Object from parsed json string.
   *
   * \param array_obj The rapidjson object to json string.
   * \return ogm::Object pointer.
   */
  static Pointer<ogm::Object> CreateOgmNode(const rapidjson::Value& array_obj);

  /**
     * \brief creates json document from ogm::Object.
     *
     * \param ogm_object .
     * \param document The rapidjson object
     */
  static void createJsonObject(const Pointer<ogm::Object>& ogm_object, rapidjson::Document& document);
  /**
   * \brief creates json string from json document.
   *
   * \param document The rapidjson object
   * \return json string.
   */
  static String GetJsonString(rapidjson::Document& document);
};
}  // namespace serialize
}  // namespace rest
}  // namespace ara

#endif  // LIB_ARAREST_INCLUDE_ARA_REST_SERIALIZE_SERIALIZE_H_

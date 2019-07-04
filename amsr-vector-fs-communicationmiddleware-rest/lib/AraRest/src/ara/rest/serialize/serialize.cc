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
/**        \file  serialize.cc
 *        \brief  source for json to OGM serializer and deserializer
 *
 *      \details  Contains the method implementations for serializer and deserializer
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/rest/serialize/serialize.h"

#include <ara/per/internal/json_parser.h>
#include <vac/language/cpp14_backport.h>
#include <iostream>

namespace ara {
namespace rest {
namespace serialize {

Pointer<ogm::Object> Serializer::JsonToOgm(String json_string) {
  rapidjson::Document document;
  document.Parse(json_string.c_str());

  Pointer<ogm::Object> obj_node = CreateOgmNode(document);

  return obj_node;
}

Pointer<ogm::Object> Serializer::CreateOgmNode(const rapidjson::Value& document) {
  Pointer<ogm::Object> obj_node = ogm::Object::Make();
  String name;
  for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); ++itr) {
    name = itr->name.GetString();

    if (document[itr->name].IsInt()) {
      obj_node->Insert(ogm::Field::Make(name, ogm::Int::Make(document[itr->name].GetInt())));

    } else if (document[itr->name].IsDouble()) {
      obj_node->Insert(ogm::Field::Make(name, ogm::Real::Make(document[itr->name].GetDouble())));

    } else if (document[itr->name].IsString() && name == "uri") {
      obj_node->Insert(
          ogm::Field::Make(name, ogm::Uri::Make(rest::Uri::Builder{
                                     StringView(document[itr->name].GetString(), document[itr->name].GetStringLength())}
                                                    .ToUri())));

    } else if (document[itr->name].IsString() && name == "uuid") {
      obj_node->Insert(ogm::Field::Make(
          name, ogm::Uuid::Make(
                    rest::Uuid{StringView(document[itr->name].GetString(), document[itr->name].GetStringLength())})));

    } else if (document[itr->name].IsString()) {
      obj_node->Insert(ogm::Field::Make(
          name, ogm::String::Make(StringView(document[itr->name].GetString(), document[itr->name].GetStringLength()))));

    } else if (document[itr->name].IsArray()) {
      auto arr_node = CreateOgmArray(document[itr->name], name);
      obj_node->Insert(ogm::Field::Make(name, std::move(arr_node)));

    } else if (document[itr->name].IsObject()) {
      obj_node->Insert(ogm::Field::Make(name, CreateOgmNode(document[itr->name])));
    }
  }

  return obj_node;
}

Pointer<ogm::Array> Serializer::CreateOgmArray(const rapidjson::Value& array, String obj_name) {
  auto arr_node = ogm::Array::Make();
  for (rapidjson::SizeType i = 0; i < array.Size(); i++) {
    if (array[i].IsInt()) {  // if array value integer
      arr_node->Append(ogm::Int::Make(array[i].GetInt()));
    } else if (array[i].IsDouble()) {  // if array value double
      arr_node->Append(ogm::Real::Make(array[i].GetDouble()));
    } else if (array[i].IsString() && obj_name == "uuid") {  // if array value uuid
      arr_node->Append(ogm::Uuid::Make(rest::Uuid{StringView(array[i].GetString(), array[i].GetStringLength())}));
    } else if (array[i].IsString() && obj_name == "uri") {  // if array value uri
      arr_node->Append(
          ogm::Uri::Make(rest::Uri::Builder{StringView(array[i].GetString(), array[i].GetStringLength())}.ToUri()));
    } else if (array[i].IsString()) {  // if array value string
      arr_node->Append(ogm::String::Make(StringView(array[i].GetString(), array[i].GetStringLength())));
    } else if (array[i].IsObject()) {  // if array value object
      const rapidjson::Value& array_obj = array[i];
      arr_node->Append(CreateOgmNode(array_obj));
    } else if (array[i].IsArray()) {  // if array value array
      const rapidjson::Value& subArray = array[i];
      arr_node->Append(CreateOgmArray(subArray, obj_name));
    }
  }

  return arr_node;
}

String Serializer::OgmToJson(const Pointer<ogm::Object>& ogm_object) {
  rapidjson::Document document;
  createJsonObject(ogm_object, document);
  String json_string = GetJsonString(document);
  return json_string;
}

void Serializer::createJsonObject(const Pointer<ogm::Object>& ogm_object, rapidjson::Document& document) {
  document.SetObject();

  rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

  ogm::Object::Iterator itr_begin = ogm_object->GetFields().begin();
  ogm::Object::Iterator itr_end = ogm_object->GetFields().end();

  for (itr_begin = ogm_object->GetFields().begin(); itr_begin != itr_end; itr_begin++) {
    StringView name = (*itr_begin).GetName();
    rapidjson::Value str_name(name.data(), static_cast<rapidjson::SizeType>(name.size()), allocator);
    ogm::Visit((*itr_begin).GetValue(),
               [&str_name, &document, &allocator](ogm::Int* int_field) {  // if value type int
                 auto int_value = int_field->GetValue();
                 document.AddMember(str_name, int_value, allocator);
               },
               [&str_name, &document, &allocator](ogm::Real* double_field) {  // if value type real
                 rapidjson::Value double_value(static_cast<double>(double_field->GetValue()));
                 document.AddMember(str_name, double_value, allocator);
               },
               [&str_name, &document, &allocator](ogm::String* string_field) {  // if value type string
                 rapidjson::Value str_value(string_field->GetValue().data(),
                                            static_cast<rapidjson::SizeType>(string_field->GetValue().size()),
                                            document.GetAllocator());
                 document.AddMember(str_name, str_value, allocator);
               },
               [&str_name, &document, &allocator](ogm::Uuid* uuid_field) {  // if value type uuid
                 rapidjson::Value uuid_value(ToString(uuid_field->GetValue()).c_str(),
                                             static_cast<rapidjson::SizeType>(ToString(uuid_field->GetValue()).size()),
                                             document.GetAllocator());
                 document.AddMember(str_name, uuid_value, allocator);
               },
               [&str_name, &document, &allocator](ogm::Uri* uri_field) {  // if value type uri
                 rapidjson::Value uri_value(ToString(uri_field->GetValue()).c_str(),
                                            static_cast<rapidjson::SizeType>(ToString(uri_field->GetValue()).size()),
                                            document.GetAllocator());
                 document.AddMember(str_name, uri_value, allocator);
               },
               [&str_name, &document, &allocator](ogm::Array* array_field) {  // if its an array
                 rapidjson::Value json_array(rapidjson::kArrayType);
                 rapidjson::Value obj(rapidjson::kObjectType);

                 std::size_t array_size = array_field->GetSize();

                 for (std::size_t index = 0; index < array_size; index++) {
                   auto& arrayElement = array_field->GetValue(index);
                   ogm::Visit(arrayElement,
                              [&json_array, &document,
                               &allocator](ogm::Int* int_array_value) {  // if value type is int within array
                                json_array.PushBack(int_array_value->GetValue(), allocator);
                              },
                              [&json_array, &document,
                               &allocator](ogm::Real* real_array_value) {  // if value type is real within array
                                rapidjson::Value double_value(static_cast<double>(real_array_value->GetValue()));
                                json_array.PushBack(double_value, allocator);
                              },
                              [&json_array, &document,
                               &allocator](ogm::String* string_array_value) {  // if value type is string within array
                                rapidjson::Value str_value(
                                    string_array_value->GetValue().data(),
                                    static_cast<rapidjson::SizeType>(string_array_value->GetValue().size()),
                                    document.GetAllocator());
                                json_array.PushBack(str_value, allocator);
                              },
                              [&json_array, &document,
                               &allocator](ogm::Uuid* uuid_array_value) {  // if type is uuid within array
                                rapidjson::Value uuid_value(
                                    ToString(uuid_array_value->GetValue()).c_str(),
                                    static_cast<rapidjson::SizeType>(ToString(uuid_array_value->GetValue()).size()),
                                    document.GetAllocator());
                                json_array.PushBack(uuid_value, allocator);
                              },
                              [&json_array, &document,
                               &allocator](ogm::Uri* uri_array_value) {  // if value type is uri within array
                                rapidjson::Value uri_value(
                                    ToString(uri_array_value->GetValue()).c_str(),
                                    static_cast<rapidjson::SizeType>(ToString(uri_array_value->GetValue()).size()),
                                    document.GetAllocator());
                                json_array.PushBack(uri_value, allocator);
                              },
                              [&json_array, &document,
                               &allocator](ogm::Object* object_array_value) {  // if its an object within array
                                auto child_obj_ptr = Copy(object_array_value);
                                rapidjson::Document sub_document;
                                createJsonObject(child_obj_ptr, sub_document);
                                rapidjson::Value sub_value(sub_document, allocator);
                                json_array.PushBack(sub_value, allocator);
                              });
                 }
                 document.AddMember(str_name, json_array, allocator);
               },
               [&str_name, &document,
                &allocator](ogm::Object* object_with_in_field) {  // an object within object
                 auto child_obj_ptr = Copy(object_with_in_field);
                 rapidjson::Document sub_document;
                 createJsonObject(child_obj_ptr, sub_document);
                 rapidjson::Value sub_value(sub_document, allocator);
                 document.AddMember(str_name, sub_value, allocator);
               });
  }
}

String Serializer::GetJsonString(rapidjson::Document& document) {
  String json_string;

  // Convert JSON document to string
  rapidjson::StringBuffer strbuf;
  rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
  document.Accept(writer);
  json_string = strbuf.GetString();

  return json_string;
}
}  // namespace serialize
}  // namespace rest
}  // namespace ara

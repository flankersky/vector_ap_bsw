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
/**        \file  keyvaluestorage.cc
 *        \brief  Implementation of KeyValueStorage
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <sys/stat.h>
#include <iostream>

#include "ara/per/basicoperations.h"
#include "ara/per/keyvaluestorage.h"
#include "ara/per/perexceptions.h"

namespace ara {
namespace per {

KeyValueStorage::KeyValueStorage(const std::string& database)
    : database_(database),
      key_value_storage_(),
      file_proxy_(database),
      rw_access_(file_proxy_.CreateRWAccess("key_value_storage", ara::per::BasicOperations::OpenMode::in |
                                                                     ara::per::BasicOperations::OpenMode::out |
                                                                     ara::per::BasicOperations::OpenMode::trunc)) {
  /* check if given database is a folder which can be used for key value storage */
  struct stat buffer;
  if (stat(database.c_str(), &buffer) == 0) {
    if (!(buffer.st_mode & S_IFDIR)) {
      throw ExceptionStorageLocationNotFound(
          "KeyValueStorage::KeyValueStorage() Given database folder is not existing.");
    }
  }
  /* check if the file stream was successful created so key value pairs can be stored in the file  */
  if (rw_access_->fail() || rw_access_->bad()) {
    throw ExceptionPhysicalStorageError(
        "KeyValueStorage::KeyValueStorage() File could not be opened for read/write operations.");
  }

  // TODO(PAASR-2778): Get rid of magic number
  key_value_storage_.reserve(100);
}

KeyValueStorage::~KeyValueStorage() {}

std::vector<std::string> KeyValueStorage::GetAllKeys() {
  /* check if the underlying filestream is working  */
  if (rw_access_->fail() || rw_access_->bad()) {
    throw ExceptionPhysicalStorageError(
        "KeyValueStorage::GetAllKeys() Kvs-File could not be opened for read/write operations.");
  }

  std::vector<std::string> result_vector;

  for (auto it = key_value_storage_.begin(); it != key_value_storage_.end(); ++it) {
    result_vector.push_back(it->first);
  }

  return result_vector;
}

bool KeyValueStorage::HasKey(const std::string& key) noexcept {
  bool result = false;
  if (key_value_storage_.end() != key_value_storage_.find(key)) {
    result = true;
  }
  return result;
}

KvsType KeyValueStorage::GetValue(const std::string& key) noexcept { return key_value_storage_.find(key)->second; }

void KeyValueStorage::SetValue(const std::string& key, const KvsType& value) {
  auto result =
      key_value_storage_.emplace(std::piecewise_construct, std::forward_as_tuple(key), std::forward_as_tuple(value));
  if (!result.second) {
    throw ExceptionLogicError("KeyValueStorage::SetValue() Theres already an element with key: " + key);
  }
}

void KeyValueStorage::RemoveKey(const std::string& key) noexcept { key_value_storage_.erase(key); }

void KeyValueStorage::RemoveAllKeys(void) noexcept {
  std::vector<std::string> keys_to_erase = GetAllKeys();

  for (std::string key : keys_to_erase) {
    key_value_storage_.erase(key);
  }
}

/**
 * \brief Makes json human readable
 * \param json
 */
void PrettifyJsonOutput(std::string* json) {
  bool dq_opened = false;
  bool array_opened = false;

  const char kSpace = ' ';
  const char kNl = '\n';
  char current;
  const int kIndentation = 3;
  std::size_t object_level = 0;

  for (std::size_t i = 0; i < json->length(); i++) {
    current = json->operator[](i);
    if (current == '{') {
      if (object_level >= 1) {
        json->insert(i, object_level * kIndentation, kSpace);
        json->insert(i, 1, kNl);
        i = i + object_level * kIndentation + 1;
      }
      object_level++;
      json->insert(i + 1, 1, kNl);
      i = i + 1;
    } else if (current == '}') {
      json->insert(i, 1, kNl);
      i = i + 1;
      object_level--;
      if (object_level >= 1) {
        json->insert(i, object_level * kIndentation, kSpace);
        i = i + object_level * kIndentation;
      }
    } else if (current == '"') {
      if (!dq_opened) {
        dq_opened = true;
        if (json->operator[](i - 2) != ':') {
          json->insert(i, object_level * kIndentation, kSpace);
          i = i + object_level * kIndentation;
        }
      } else {
        dq_opened = false;
      }
    } else if (current == '[') {
      object_level++;
      array_opened = true;
    } else if (current == ']') {
      object_level--;
      if (json->operator[](i - 1) == '}') {
        json->insert(i, 1, kNl);
        i = i + 1;
        if (object_level >= 1) {
          json->insert(i, object_level * kIndentation, kSpace);
          i = i + object_level * kIndentation;
        }
      }
      array_opened = false;
    } else if (current == ',') {
      if (!array_opened && json->operator[](i + 1) != '{') {
        json->insert(i + 1, 1, kNl);
        i = i + 1;
      }
    }
  }
}

void KeyValueStorage::SyncToStorage() {
  /* check if the underlying filestream is working  */
  if (rw_access_->fail() || rw_access_->bad()) {
    throw ExceptionPhysicalStorageError(
        "KeyValueStorage::GetAllKeys() Kvs-File could not be opened for read/write operations.");
  }

  std::vector<std::string> keys_to_sync = GetAllKeys();
  std::string json_output = "{";

  for (auto it = key_value_storage_.begin(); it != key_value_storage_.end(); ++it) {
    json_output = json_output + it->second.ToString();
  }

  json_output = json_output + "}";
  /* clean json output to make it valid json */
  for (std::size_t i = 0; i < json_output.length(); i++) {
    if (json_output[i] == ']' || json_output[i] == '}') {
      if (json_output[i - 1] == ',') {
        json_output.erase(i - 1, 1);
        i = i - 1;
      } else if (json_output[i + 1] == '{') {
        json_output.insert(i + 1, 1, ',');
        i = i + 1;
      }
    }
  }
  PrettifyJsonOutput(&json_output);
  rw_access_->operator<<(json_output);
}

}  // namespace per
}  // namespace ara

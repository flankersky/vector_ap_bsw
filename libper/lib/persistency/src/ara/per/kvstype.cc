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
/**        \file  kvstype.cc
 *        \brief  Implementation of KvsType
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <algorithm>
#include <iostream>
#include <sstream>

#include "ara/per/kvstype.h"
#include "ara/per/perexceptions.h"

namespace ara {
namespace per {

KvsType::KvsType() : type_(KvsType::Type::kNotSet), status_(KvsType::Status::kNotFound), key_(""), object_array_() {}

KvsType::KvsType(bool value)
    : bool_(value), type_(KvsType::Type::kBoolean), status_(KvsType::Status::kSuccess), key_(""), object_array_() {}

KvsType::KvsType(int8_t value)
    : integer_(static_cast<int64_t>(value)),
      type_(KvsType::Type::kSInt8),
      status_(KvsType::Status::kSuccess),
      key_(""),
      object_array_() {}

KvsType::KvsType(int16_t value)
    : integer_(static_cast<int64_t>(value)),
      type_(KvsType::Type::kSInt16),
      status_(KvsType::Status::kSuccess),
      key_(""),
      object_array_() {}

KvsType::KvsType(int32_t value)
    : integer_(static_cast<int64_t>(value)),
      type_(KvsType::Type::kSInt32),
      status_(KvsType::Status::kSuccess),
      key_(""),
      object_array_() {}

KvsType::KvsType(int64_t value)
    : integer_(value), type_(KvsType::Type::kSInt64), status_(KvsType::Status::kSuccess), key_(""), object_array_() {}

KvsType::KvsType(uint8_t value)
    : unsigned_integer_(static_cast<uint64_t>(value)),
      type_(KvsType::Type::kUInt8),
      status_(KvsType::Status::kSuccess),
      key_(""),
      object_array_() {}

KvsType::KvsType(uint16_t value)
    : unsigned_integer_(static_cast<uint64_t>(value)),
      type_(KvsType::Type::kUInt16),
      status_(KvsType::Status::kSuccess),
      key_(""),
      object_array_() {}

KvsType::KvsType(uint32_t value)
    : unsigned_integer_(static_cast<uint64_t>(value)),
      type_(KvsType::Type::kUInt32),
      status_(KvsType::Status::kSuccess),
      key_(""),
      object_array_() {}

KvsType::KvsType(uint64_t value)
    : unsigned_integer_(value),
      type_(KvsType::Type::kUInt64),
      status_(KvsType::Status::kSuccess),
      key_(""),
      object_array_() {}

KvsType::KvsType(float value)
    : double_(static_cast<double>(value)),
      type_(KvsType::Type::kFloat),
      status_(KvsType::Status::kSuccess),
      key_(""),
      object_array_() {}

KvsType::KvsType(double value)
    : double_(value), type_(KvsType::Type::kDouble), status_(KvsType::Status::kSuccess), key_(""), object_array_() {}

KvsType::KvsType(const std::string& value)
    : string_(value), type_(KvsType::Type::kString), status_(KvsType::Status::kSuccess), key_(""), object_array_() {}

KvsType::KvsType(const char* value)
    : string_(value), type_(KvsType::Type::kString), status_(KvsType::Status::kSuccess), key_(""), object_array_() {}

KvsType::KvsType(void* data, std::size_t len)
    : byte_memory_(), type_(KvsType::Type::kBinary), status_(KvsType::Status::kSuccess), key_(""), object_array_() {
  unsigned char* data_as_char = static_cast<unsigned char*>(data);
  byte_memory_.reserve(len);
  for (size_t i = 0; i < len; ++i) {
    byte_memory_.push_back(data_as_char[i]);
  }
}

KvsType::KvsType(const KvsType& other)
    : type_(other.type_), status_(other.status_), key_(other.key_), object_array_(other.object_array_) {
  switch (type_) {
    case KvsType::Type::kSInt8:
    case KvsType::Type::kSInt16:
    case KvsType::Type::kSInt32:
    case KvsType::Type::kSInt64:
      integer_ = other.integer_;
      break;
    case KvsType::Type::kUInt8:
    case KvsType::Type::kUInt16:
    case KvsType::Type::kUInt32:
    case KvsType::Type::kUInt64:
      unsigned_integer_ = other.unsigned_integer_;
      break;
    case KvsType::Type::kBoolean:
      bool_ = other.bool_;
      break;
    case KvsType::Type::kDouble:
    case KvsType::Type::kFloat:
      double_ = other.double_;
      break;
    case KvsType::Type::kString:
      new (&string_) std::string(other.string_);
      break;
    case KvsType::Type::kBinary:
      new (&byte_memory_) std::vector<unsigned char>(other.byte_memory_);
      break;
    default:
      /* shouldn't do anything */
      break;
  }
}

KvsType::KvsType(KvsType&& other)
    : type_(std::move(other.type_)),
      status_(std::move(other.status_)),
      key_(std::move(other.key_)),
      object_array_(std::move(other.object_array_)) {
  switch (type_) {
    case KvsType::Type::kSInt8:
    case KvsType::Type::kSInt16:
    case KvsType::Type::kSInt32:
    case KvsType::Type::kSInt64:
      integer_ = std::move(other.integer_);
      break;
    case KvsType::Type::kUInt8:
    case KvsType::Type::kUInt16:
    case KvsType::Type::kUInt32:
    case KvsType::Type::kUInt64:
      unsigned_integer_ = std::move(other.unsigned_integer_);
      break;
    case KvsType::Type::kBoolean:
      bool_ = std::move(other.bool_);
      break;
    case KvsType::Type::kDouble:
    case KvsType::Type::kFloat:
      double_ = std::move(other.double_);
      break;
    case KvsType::Type::kString:
      new (&string_) std::string(std::move(other.string_));
      break;
    case KvsType::Type::kBinary:
      new (&byte_memory_) std::vector<unsigned char>(std::move(other.byte_memory_));
      break;
    default:
      /* shouldn't do anything */
      break;
  }
}

KvsType& KvsType::operator=(KvsType&& other) {
  type_ = std::move(other.type_);
  status_ = std::move(other.status_);
  key_ = std::move(other.key_);
  object_array_ = std::move(other.object_array_);

  switch (type_) {
    case KvsType::Type::kSInt8:
    case KvsType::Type::kSInt16:
    case KvsType::Type::kSInt32:
    case KvsType::Type::kSInt64:
      integer_ = std::move(other.integer_);
      break;
    case KvsType::Type::kUInt8:
    case KvsType::Type::kUInt16:
    case KvsType::Type::kUInt32:
    case KvsType::Type::kUInt64:
      unsigned_integer_ = std::move(other.unsigned_integer_);
      break;
    case KvsType::Type::kBoolean:
      bool_ = std::move(other.bool_);
      break;
    case KvsType::Type::kDouble:
    case KvsType::Type::kFloat:
      double_ = std::move(other.double_);
      break;
    case KvsType::Type::kString:
      string_ = std::move(other.string_);
      break;
    case KvsType::Type::kBinary:
      byte_memory_ = std::move(other.byte_memory_);
      break;
    default:
      /* shouldn't do anything */
      break;
  }
  return *this;
}

KvsType::Type KvsType::GetType() const noexcept { return type_; }

KvsType::Status KvsType::GetStatus() const noexcept { return status_; }

int32_t KvsType::GetSInt() const {
  if (type_ == KvsType::Type::kSInt8 || type_ == KvsType::Type::kSInt16 || type_ == KvsType::Type::kSInt32) {
    return static_cast<int32_t>(integer_);
  } else {
    throw ExceptionLogicError("KvsType::GetUInt() Internal type is not a signed integer.");
  }
}

uint32_t KvsType::GetUInt() const {
  if (type_ == KvsType::Type::kUInt8 || type_ == KvsType::Type::kUInt16 || type_ == KvsType::Type::kUInt32) {
    return static_cast<uint32_t>(unsigned_integer_);
  } else {
    throw ExceptionLogicError("KvsType::GetUInt() Internal type is not a unsigned integer.");
  }
}

int64_t KvsType::GetSInt64() const {
  if (type_ == KvsType::Type::kSInt8 || type_ == KvsType::Type::kSInt16 || type_ == KvsType::Type::kSInt32 ||
      type_ == KvsType::Type::kSInt64) {
    return static_cast<int64_t>(unsigned_integer_);
  } else {
    throw ExceptionLogicError("KvsType::GetSInt64() Internal type is not a 64 bit signed integer.");
  }
}

uint64_t KvsType::GetUInt64() const {
  if (type_ == KvsType::Type::kUInt8 || type_ == KvsType::Type::kUInt16 || type_ == KvsType::Type::kUInt32 ||
      type_ == KvsType::Type::kUInt64) {
    return static_cast<uint64_t>(unsigned_integer_);
  } else {
    throw ExceptionLogicError("KvsType::GetUInt64() Internal type is not a 64 bit unsigned integer.");
  }
}

float KvsType::GetFloat() const {
  if (type_ == KvsType::Type::kDouble || type_ == KvsType::Type::kFloat) {
    return static_cast<float>(double_);
  } else {
    throw ExceptionLogicError("KvsType::GetFloat() Internal type is not a float.");
  }
}

double KvsType::GetDouble() const {
  if (type_ == KvsType::Type::kDouble || type_ == KvsType::Type::kFloat) {
    return double_;
  } else {
    throw ExceptionLogicError("KvsType::GetDouble() Internal type is not a double.");
  }
}

bool KvsType::GetBool() const {
  if (type_ == KvsType::Type::kBoolean) {
    return bool_;
  } else {
    throw ExceptionLogicError("KvsType::GetBool() Internal type is not a boolean.");
  }
}

void KvsType::GetBinary(void* data, std::size_t len) {
  if (type_ == KvsType::Type::kBinary) {
    if (data != nullptr) {
      if (len != 0) {
        unsigned char* char_data = static_cast<unsigned char*>(data);
        for (size_t i = 0; i < std::max(len, byte_memory_.size()); ++i) {
          char_data[i] = byte_memory_[i];
        }
      } else {
        throw ExceptionLogicError("KvsType::GetBinary() Requested size equal to 0.");
      }
    } else {
      throw ExceptionLogicError("KvsType::GetBinary() Given data pointer is null.");
    }
  } else {
    throw ExceptionLogicError("KvsType::GetBinary() Internal type is not binary.");
  }
}

std::string KvsType::GetString() const {
  if (type_ == KvsType::Type::kString) {
    return string_;
  } else {
    throw ExceptionLogicError("KvsType::GetString() Internal type is not a string.");
  }
}

std::string KvsType::GetKey() const noexcept { return key_; }

void KvsType::SetKey(const std::string& name) noexcept { key_ = name; }

bool KvsType::IsSignedInteger() const noexcept {
  bool result = false;
  if (type_ == KvsType::Type::kSInt8 || type_ == KvsType::Type::kSInt16 || type_ == KvsType::Type::kSInt32 ||
      type_ == KvsType::Type::kSInt64) {
    result = true;
  }
  return result;
}

bool KvsType::IsUnsignedInteger() const noexcept {
  bool result = false;
  if (type_ == KvsType::Type::kUInt8 || type_ == KvsType::Type::kUInt16 || type_ == KvsType::Type::kUInt32 ||
      type_ == KvsType::Type::kUInt64) {
    result = true;
  }
  return result;
}

void KvsType::AddArrayItem(const KvsType& kvs) { object_array_.push_back(kvs); }

std::string KvsType::ToString() const {
  const std::string dq = "\"";
  const std::string comma = ",";
  const std::string colon = " : ";
  const std::string key_delimiter = dq + key_ + dq + colon;

  std::ostringstream strs;
  switch (type_) {
    case KvsType::Type::kSInt8:
    case KvsType::Type::kSInt16:
    case KvsType::Type::kSInt32:
    case KvsType::Type::kSInt64:
      strs << key_delimiter << integer_ << comma;
      break;
    case KvsType::Type::kUInt8:
    case KvsType::Type::kUInt16:
    case KvsType::Type::kUInt32:
    case KvsType::Type::kUInt64:
      strs << key_delimiter << unsigned_integer_ << comma;
      break;
    case KvsType::Type::kBoolean:
      if (bool_) {
        strs << key_delimiter << "true" << comma;
      } else {
        strs << key_delimiter << "false" << comma;
      }
      break;
    case KvsType::Type::kDouble:
    case KvsType::Type::kFloat:
      strs << key_delimiter << double_ << comma;
      break;
    case KvsType::Type::kString:
      strs << key_delimiter << dq << string_ << dq << comma;
      break;
    case KvsType::Type::kBinary:
      strs << key_delimiter << dq;
      for (auto byte : byte_memory_) {
        strs << byte;
      }
      strs << dq << comma;
      break;
    case KvsType::Type::kObject:
      strs << key_delimiter << "[";
      for (auto object : object_array_) {
        strs << object.ToStringLevelTwo();
      }
      strs << "]" << comma;
      break;
    default:
      /* shouldn't do anything */
      break;
  }
  std::string result(strs.str());
  return result;
}

std::string KvsType::ToStringLevelTwo() const {
  const std::string dq = "\"";
  const std::string comma = ",";
  const std::string nl = "\n";
  const std::string colon = " : ";
  const std::string key_delimiter = dq + key_ + dq + colon;

  std::ostringstream strs;
  switch (type_) {
    case KvsType::Type::kSInt8:
    case KvsType::Type::kSInt16:
    case KvsType::Type::kSInt32:
    case KvsType::Type::kSInt64:
      strs << integer_ << comma;
      break;
    case KvsType::Type::kUInt8:
    case KvsType::Type::kUInt16:
    case KvsType::Type::kUInt32:
    case KvsType::Type::kUInt64:
      strs << unsigned_integer_ << comma;
      break;
    case KvsType::Type::kBoolean:
      if (bool_) {
        strs << "true" << comma;
      } else {
        strs << "false" << comma;
      }
      break;
    case KvsType::Type::kDouble:
    case KvsType::Type::kFloat:
      strs << double_ << comma;
      break;
    case KvsType::Type::kString:
      strs << dq << string_ << dq;
      break;
    case KvsType::Type::kBinary:
      strs << dq;
      for (auto byte : byte_memory_) {
        strs << byte;
      }
      strs << dq << comma << nl;
      break;
    case KvsType::Type::kObject:
      strs << "{" << key_delimiter << "[";
      for (auto object : object_array_) {
        strs << object.ToStringLevelTwo();
      }
      strs << "]"
           << "}";
      break;
    default:
      /* shouldn't do anything */
      break;
  }
  std::string result(strs.str());
  return result;
}

}  // namespace per
}  // namespace ara

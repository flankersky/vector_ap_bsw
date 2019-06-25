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
/**        \file  kvstype.h
 *        \brief  Implementation of KvsType
 *
 *      \details  -
 *
 *********************************************************************************************************************/

#ifndef LIB_PERSISTENCY_INCLUDE_ARA_PER_KVSTYPE_H_
#define LIB_PERSISTENCY_INCLUDE_ARA_PER_KVSTYPE_H_

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/
#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "vac/container/static_vector.h"
#include "vac/testing/test_adapter.h"

#include "ara/per/perexceptions.h"

namespace ara {
namespace per {

class KvsTypeFactory;  //< fwd

/**
 * \brief Type class for every possible datatype in the KvsType::Type enumeration
 */
class KvsType {
 public:
  /**
   * \brief Defines the state of a KvsType::Status
   *
   * \trace SWS_PER_00004
   */
  enum class Status : uint8_t {
    kSuccess,        //!< Indicates that the value was successfully restored from the KVS-storage
    kNotFound,       //!< Requested key was not found
    kCheckSumError,  //!< The key-value pair was found, but the checksum of it is incorrect
    kGeneralError    //!< Any other failure
  };

  /**
   * \brief Defines the states of a KvsType::Type
   *
   * \trace SWS_PER_00003
   */
  enum class Type : uint8_t {
    kNotSupported = 0,
    kFloat,
    kDouble,
    kSInt8,
    kSInt16,
    kSInt32,
    kSInt64,
    kUInt8,
    kUInt16,
    kUInt32,
    kUInt64,
    kString,
    kBinary,
    kBoolean,
    kObject,
    kNotSet
  };

  /**
   * \brief Default Constructor
   * Creates an empty instance of KvsType with the status kNotFound
   *
   * \trace SWS_PER_00006
   */
  KvsType();

  /**
   * \brief Creates an instance of KvsType which can be used to store bools.
   * \param value The value to be stored as boolean
   *
   * \trace SWS_PER_00005
   */
  explicit KvsType(bool value);

  /**
   * \brief Creates an instance of KvsType which can be used to store int8_t.
   * \param value The value to be stored as int8_t
   *
   * \trace SWS_PER_00005
   */
  explicit KvsType(int8_t value);

  /**
   * \brief Creates an instance of KvsType which can be used to store int16_t.
   * \param value The value to be stored as int16_t
   *
   * \trace SWS_PER_00005
   */
  explicit KvsType(int16_t value);

  /**
   * \brief Creates an instance of KvsType which can be used to store int32_t.
   * \param value The value to be stored as int32_t
   *
   * \trace SWS_PER_00005
   */
  explicit KvsType(int32_t value);

  /**
   * \brief Creates an instance of KvsType which can be used to store int64_t.
   * \param value The value to be stored as int64_t
   *
   * \trace SWS_PER_00005
   */
  explicit KvsType(int64_t value);

  /**
   * \brief Creates an instance of KvsType which can be used to store uint8_t.
   * \param value The value to be stored as uint8_t
   *
   * \trace SWS_PER_00005
   */
  explicit KvsType(uint8_t value);

  /**
   * \brief Creates an instance of KvsType which can be used to store uint16_t.
   * \param value The value to be stored as uint16_t
   *
   * \trace SWS_PER_00005
   */
  explicit KvsType(uint16_t value);

  /**
   * \brief Creates an instance of KvsType which can be used to store uint32_t.
   * \param value The value to be stored as uint32_t
   *
   * \trace SWS_PER_00005
   */
  explicit KvsType(uint32_t value);

  /**
   * \brief Creates an instance of KvsType which can be used to store uint64_t.
   * \param value The value to be stored as uint64_t
   *
   * \trace SWS_PER_00005
   */
  explicit KvsType(uint64_t value);

  /**
   * \brief Creates an instance of KvsType which can be used to store flat.
   * \param value The value to be stored as float
   *
   * \trace SWS_PER_00005
   */
  explicit KvsType(float value);

  /**
   * \brief Creates an instance of KvsType which can be used to store double.
   * \param value The value to be stored as double
   *
   * \trace SWS_PER_00005
   */
  explicit KvsType(double value);

  /**
   * \brief Creates an instance of KvsType which can be used to store std::string.
   * \param value The value to be stored as std::string
   *
   * \trace SWS_PER_00005
   */
  explicit KvsType(const std::string& value);

  /**
   * \brief Creates an instance of KvsType which can be used to store char pointer.
   * \param value The value to be stored as char pointer
   *
   * \trace SWS_PER_00005
   */
  explicit KvsType(const char* value);

  /**
   * \brief Creates an instance of KvsType which can be used to store value(s) of plain-old-datatypes.
   * \param data Pointer to data to be stored
   * \param len Length of the data to be stored.
   *
   * \trace SWS_PER_00007
   */
  explicit KvsType(void* data, std::size_t len);

  /**
   * \brief Copy Constructor
   * \param other The KvsType to be copied
   */
  KvsType(const KvsType& other);

  /**
   * \brief Move Constructor
   * \param other The KvsType to be moved
   */
  KvsType(KvsType&& other);

  KvsType& operator=(const KvsType&) = delete;  //< Deleted copy assignment operator for KvsType

  /**
   * \brief Move assignment oeprator
   * \param other
   * \return
   */
  KvsType& operator=(KvsType&& other);

  /**
   * \brief Destructor
   *
   * \trace SWS_PER_00008
   */
  ~KvsType() {}

  /**
   * \brief Returns the type of the stored value
   * \return Returns the type of the stored value
   */
  KvsType::Type GetType() const noexcept;

  /**
   * \brief Returns the stored value
   * \return Stored value
   * \throws ExceptionLogicError if requested value is not of type signed integer
   *
   * \trace SWS_PER_00150
   */
  int32_t GetSInt() const;

  /**
   * \brief Returns the stored value
   * \return Stored value
   * \throws ExceptionLogicError if requested value is not of type unsigned integer
   *
   * \trace SWS_PER_00151
   */
  uint32_t GetUInt() const;

  /**
   * \brief Returns the stored value
   * \return Stored value
   * \throws ExceptionLogicError if requested value is not of type 64 bit signed integer
   *
   * \trace SWS_PER_00152
   */
  int64_t GetSInt64() const;

  /**
   * \brief Returns the stored value
   * \return Stored value
   * \throws ExceptionLogicError if requested value is not of type 64 bit unsigned integer
   *
   * \trace SWS_PER_00153
   */
  uint64_t GetUInt64() const;

  /**
   * \brief Returns the stored value
   * \return Stored value
   * \throws ExceptionLogicError if requested value is not of type float
   *
   * \trace SWS_PER_00154
   */
  float GetFloat() const;

  /**
   * \brief Returns the stored value
   * \return Stored value
   * \throws ExceptionLogicError if requested value is not of type double
   *
   * \trace SWS_PER_00155
   */
  double GetDouble() const;

  /**
   * \brief Returns the stored value
   * \return Stored value
   * \throws ExceptionLogicError if requested value is not of type string
   *
   * \trace SWS_PER_00156
   */
  std::string GetString() const;

  /**
   * \brief Returns the stored value
   * \return Stored value
   * \throws ExceptionLogicError if requested value is not of type boolean
   *
   * \trace SWS_PER_00157
   */
  bool GetBool() const;

  /**
   * \brief Returns the stored value
   * \param data Pointer where the data should be copied
   * \param len Size of the data to be retrieved
   * \throws ExceptionLogicError
   *
   * \trace SWS_PER_00012
   */
  void GetBinary(void* data, std::size_t len);

  /**
   * \brief The KvsType class shall provide a method to get the status of the KVS-Pair. The status shall be checked by
   * the client before trying to use the stored KVS-pair
   * \return Status of the KvsType.
   *
   * \trace SWS_PER_00013
   */
  Status GetStatus() const noexcept;

  /**
   * \brief The KvsType class shall provide a method to get the key of the KVS-Pair
   * \return Key of the KvsType.
   *
   * \trace SWS_PER_00014
   */
  std::string GetKey() const noexcept;

  /**
   * \brief The KvsType class shall provide a method to set the key of the KVS-Pair.
   * \param name Value of the key to be stored.
   *
   * \trace SWS_PER_00015
   */
  void SetKey(const std::string& name) noexcept;

  /**
   * \brief The KvsType class shall provide a method to check if the internal type is any of the signed integer types
   * (8,16,32,64bit)
   * \return True if type is signed integer, false if not
   *
   * \trace SWS_PER_00016
   */
  bool IsSignedInteger() const noexcept;

  /**
   * \brief The KvsType class shall provide a method to check if the internal type is any of the unsigned integer
   * types
   * (8,16,32,64bit)
   * \return True if type is unsigned integer, false if not
   *
   * \trace SWS_PER_00017
   */
  bool IsUnsignedInteger() const noexcept;

  /**
   * \brief The KvsType class shall provide a template method to store an array of [integer, floating point, or
   * KvsType]. The container shall be iterable and the elements are stored in the iteration order to the KVS internal
   * data structure.
   * \param array Array to be stored
   * \throws ExceptionStorageLocationNotFound
   *
   * \trace SWS_PER_00018
   */
  template <class Array>
  void StoreArray(const Array& array) {
    static_assert((std::is_integral<typename Array::value_type>::value) ||
                      (std::is_floating_point<typename Array::value_type>::value) ||
                      (std::is_same<typename Array::value_type, KvsType>::value),
                  "Storing of array template supports only iterable containers, which contain  "
                  "one of fundamental types (int,float, etc.) or KvsType-objects");
    if (type_ == Type::kNotSet) {
      for (auto current = array.cbegin(); current != array.cend(); current++) {
        object_array_.emplace_back(*current);
      }
      type_ = Type::kObject;
    } else {
      throw ExceptionStorageLocationNotFound(
          "KvsType::StoreArray() Object is of another basic type and cannot store an array.");
    }
  }

  /**
   * \brief The KvsType class shall provide a template method to store an array of [integer, floating point, or
   * KvsType]. The container shall be iterable and the elements are stored in the iteration order to the KVS internal
   * data structure.
   * \param array Array to be stored
   * \throws ExceptionStorageLocationNotFound
   *
   * \trace SWS_PER_00018
   */
  template <class Array, typename std::enable_if<std::is_same<Array, KvsType>::value, Array>::type* = nullptr>
  void StoreArray(const Array& array) {
    static_assert((std::is_integral<typename Array::value_type>::value) ||
                      (std::is_floating_point<typename Array::value_type>::value) ||
                      (std::is_same<typename Array::value_type, KvsType>::value),
                  "Storing of array template supports only iterable containers, which contain  "
                  "one of fundamental types (int,float, etc.) or KvsType-objects");
    if (type_ == Type::kNotSet) {
      for (auto current = array.cbegin(); current != array.cend(); current++) {
        object_array_.push_back(*current);
      }
      type_ = Type::kObject;
    } else {
      throw ExceptionStorageLocationNotFound(
          "KvsType::StoreArray() Object is of another basic type and cannot store an array.");
    }
  }

  /**
   * \brief Specialised for KvsType. The KvsType class shall provide a method to get an array. Returns a vector of
   * requested type. Supported
   * types are [integer, floating point and KVSType]. Restores the items in same order as they were saved with
   * StoreArray.
   * \return Template vector
   * \throws ExceptionStorageLocationNotFound
   *
   * \trace SWS_PER_00019
   */
  template <class T, typename std::enable_if<std::is_same<T, KvsType>::value, T>::type* = nullptr>
  std::vector<T> GetArray() const {
    if (type_ == Type::kObject) {
      std::vector<T> return_vector;
      for (const T& object : object_array_) {
        return_vector.push_back(object);
      }
      return return_vector;
    } else {
      throw ExceptionStorageLocationNotFound("KvsType::GetArray() KvsType is not of type Object.");
    }
  }

  /**
   * \brief Specialised for integer types and booleans. The KvsType class shall provide a method to get an array.
   * Returns a vector of requested type. Supported
   * types are [integer, floating point and KVSType]. Restores the items in same order as they were saved with
   * StoreArray.
   * \return Template vector
   * \throws ExceptionStorageLocationNotFound
   *
   * \trace SWS_PER_00019
   */
  template <class T, typename std::enable_if<std::is_integral<T>::value, int>::type* = nullptr>
  std::vector<T> GetArray() const {
    if (type_ == Type::kObject) {
      std::vector<T> return_vector;
      for (const KvsType& ref_kvstype : object_array_) {
        if (ref_kvstype.IsSignedInteger()) {
          return_vector.push_back(ref_kvstype.GetSInt64());
        } else if (ref_kvstype.IsUnsignedInteger()) {
          return_vector.push_back(ref_kvstype.GetUInt64());
        } else if (ref_kvstype.GetType() == Type::kBoolean) {
          return_vector.push_back(ref_kvstype.GetBool());
        }
      }
      return return_vector;
    } else {
      throw ExceptionStorageLocationNotFound("KvsType::GetArray() KvsType is not of type Object.");
    }
  }

  /**
   * \brief Specialised for floating point types. The KvsType class shall provide a method to get an array. Returns a
   * vector of requested type. Supported
   * types are [integer, floating point and KVSType]. Restores the items in same order as they were saved with
   * StoreArray.
   * \return Template vector
   * \throws ExceptionStorageLocationNotFound
   *
   * \trace SWS_PER_00019
   */
  template <class T, typename std::enable_if<std::is_floating_point<T>::value, int>::type* = nullptr>
  std::vector<T> GetArray() const {
    if (type_ == Type::kObject) {
      std::vector<T> return_vector;
      for (const KvsType& ref_kvstype : object_array_) {
        if (ref_kvstype.GetType() == Type::kFloat) {
          return_vector.push_back(ref_kvstype.GetFloat());
        } else if (ref_kvstype.GetType() == Type::kDouble) {
          return_vector.push_back(ref_kvstype.GetDouble());
        }
      }
      return return_vector;
    } else {
      throw ExceptionStorageLocationNotFound("KvsType::GetArray() KvsType is not of type Object.");
    }
  }
  /**
   * \brief The KvsType class shall provide a method to add an element to the internal array container. Note: This
   * function is used only if it’s of type kObject and the underlying types are not relevant.
   * \param kvs
   * \throws ExceptionStorageLocationNotFound
   *
   * \trace SWS_PER_00020
   */
  void AddArrayItem(const KvsType& kvs);

  /**
   * \brief Converts the KvsType in an string to print it out
   * \return A string which represents the KvsType's data
   */
  std::string ToString() const;

 private:
  /**
   * \brief Helper function to print valid json
   * \return String with valid json
   */
  std::string ToStringLevelTwo() const;

  /**
   * \brief Anonymus union for all numeric types and boolean
   *
   * 8, 16, 32, 64 bits: As the union always holds enough memory to save the biggest member of the union only 64 bits
   * integers are saved, because it is save to convert lower precision types like int_8t to int_64t (same for
   * unsigned).
   *
   * float, double: Double is guaranteed to be at least the size of a float. So it is safe to convert a float to a
   * double and only a double needs to be saved.
   */
  union {
    bool bool_;                               ///< Boolean member
    double double_;                           ///< Numeric member double
    int64_t integer_;                         ///< Numeric member signed int 64 bits
    uint64_t unsigned_integer_;               ///< Numeric member unsigned int 64 bits
    std::string string_;                      ///< Member to save std::strings
    std::vector<unsigned char> byte_memory_;  ///< Member to save pod bytes
  };

  /**
   * \brief Type of the created instance of KvsType
   */
  KvsType::Type type_;

  /**
   * \brief Status of the type
   */
  KvsType::Status status_;

  /**
   * \brief Key of the key value pair
   */
  std::string key_;

  /**
   * \brief Memory for array of some Type
   */
  std::vector<KvsType> object_array_;

  FRIEND_TEST(KvsTypeTest, StoreArray);         ///< Friend test decleration
  FRIEND_TEST(KvsTypeTest, StoreArrayKvsType);  ///< Friend test decleration
  FRIEND_TEST(KvsTypeTest, StoreArrayBool);     ///< Friend test decleration
};

}  // namespace per
}  // namespace ara

#endif  // LIB_PERSISTENCY_INCLUDE_ARA_PER_KVSTYPE_H_

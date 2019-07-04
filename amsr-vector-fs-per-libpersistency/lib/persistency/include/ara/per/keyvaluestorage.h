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
/**        \file  keyvaluestorage.h
 *        \brief  KeyValueStorage class definition
 *
 *      \details
 *
 *********************************************************************************************************************/

#ifndef LIB_PERSISTENCY_INCLUDE_ARA_PER_KEYVALUESTORAGE_H_
#define LIB_PERSISTENCY_INCLUDE_ARA_PER_KEYVALUESTORAGE_H_

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/

#include <memory>
#include <string>
#include <vector>

#include "vac/container/static_map.h"
#include "vac/container/string_view.h"
#include "vac/testing/test_adapter.h"

#include "ara/per/file_proxy_accessor_factory_impl.h"
#include "ara/per/kvstype.h"
#include "ara/per/readwriteaccessor.h"

namespace ara {
namespace per {

/**
 * \brief The KeyValueStorage class defined in the keyvaluestorage.h header file shall define an interface to the common
 * keyvalue store functions.
 *
 * \trace SWS_PER_00051
 */
class KeyValueStorage final {
 public:
  /**
   * Default Constructor
   */
  KeyValueStorage() = delete;

  /**
   * \brief Constructor: Creates an instance of KeyValueStorage which configures the storage location.
   * \param database shortName of PortPrototype typed by a PersistencyKeyValueDatabaseInterface
   * \throws ExceptionStorageLocationNotFound or ExceptionPhysicalStorageError
   *
   * \trace SWS_PER_00052
   */
  explicit KeyValueStorage(const std::string& database);

  KeyValueStorage(const KeyValueStorage& other) = delete;  //< Deleted copy Constructor /trace SWS_PER_00041
  KeyValueStorage(KeyValueStorage&& other) = delete;       //< Deleted move Constructor for KeyValueStorage
  KeyValueStorage& operator=(const KeyValueStorage&) =
      delete;  //< Deleted copy assignment operator for KeyValueStorage /trace SWS_PER_00041
  KeyValueStorage& operator=(KeyValueStorage&&) = delete;  //< Deleted move assignment operator for KeyValueStorage

  /**
   * Destructor: Deletes the KeyValueStorage instance.
   *
   * \trace SWS_PER_00050
   */
  ~KeyValueStorage();

  /**
   * \brief The KeyValueStorage class shall provide a method to get a list of all keys explicitly set in the dataset. It
   * shall return the list of available keys.
   * \return Vector of type std::string.
   * \throws ExceptionPhysicalStorageError if underlying file stream chokes
   *
   * \note According to persistency SWS this function may throws an ExpectionLogicError which will not happen in this
   * implementation.
   *
   * \trace SWS_PER_00042
   */
  std::vector<std::string> GetAllKeys();

  /**
   * \brief The KeyValueStorage class shall provide a method to determine whether the key exists in the dataset. It
   * shall return true if the key exists in the dataset otherwise false.
   * \param key Value of the key that shall be checked.
   * \return Returns if the key could be located.
   *
   * \trace SWS_PER_00043
   */
  bool HasKey(const std::string& key) noexcept;

  /**
   * \brief The KeyValueStorage class shall provide a method to get the value assigned to the key.
   * \param key Value of the key associated with the value.
   * \return Returns an object of type KvsType. If the key could not have been located, the function shall return a
   * KvsType with the status kNotFound.
   *
   * \trace SWS_PER_00044
   */
  KvsType GetValue(const std::string& key) noexcept;

  /**
   * \brief The KeyValueStorage class shall provide a method to assign the value to the key. It will be explicitly
   * stored it in the dataset.
   * \param key Value of the key associated with the value.
   * \param value KvsType containing the data to be stored.
   * \throws ExceptionLogicError if key is already present in key value storage
   *
   * \note According to persistency SWS this function may throws an ExpectionPhysicalStorageError which will not happen
   * in this implementation.
   *
   * \trace SWS_PER_00046
   */
  void SetValue(const std::string& key, const KvsType& value);

  /**
   * \brief The KeyValueStorage class shall provide a method that removes the key and associated value.
   * \param key Value of the key associated with the value.
   *
   * \trace SWS_PER_00047
   */
  void RemoveKey(const std::string& key) noexcept;

  /**
   * \brief The KeyValueStorage class shall provide a method that removes all keys and associated values.
   *
   * \trace SWS_PER_00048
   */
  void RemoveAllKeys(void) noexcept;

  /**
   * \brief The KeyValueStorage class shall provide a method to trigger flushing of key-value pairs to the physical
   * storage.
   * \throws ExceptionPhysicalStorageError if underlying file stream chokes
   *
   * \note According to persistency SWS this function may throws an ExpectionLogicError which will not happen
   * in this implementation.
   *
   * \trace SWS_PER_00049
   */
  void SyncToStorage(void);

 private:
  /**
   * \brief Descriptiv name for the kay-value storage
   */
  vac::container::string_view database_;

  /**
   * \brief Static map which holds the key-value pairs
   */
  vac::container::StaticMap<std::string, KvsType> key_value_storage_;

  /**
   * \brief File access
   */
  FileProxyAccessorFactoryImpl file_proxy_;

  /**
   * \brief ReadWrite access
   */
  FileProxyAccessUniquePtr<ReadWriteAccessor> rw_access_;

  FRIEND_TEST(KeyValueStorageFixture, RemoveAllKeys);                  ///< Friend test decleration
  FRIEND_TEST(KeyValueStorageFixture, SyncToStorage);                  ///< Friend test decleration
  FRIEND_TEST(KeyValueStorageFixture, SyncToStorageObjectsOfObjects);  ///< Friend test decleration
  FRIEND_TEST(KeyValueStorageFixture, SyncToStorageBeautifulJson);     ///< Friend test decleration
};

}  // namespace per
}  // namespace ara

#endif  // LIB_PERSISTENCY_INCLUDE_ARA_PER_KEYVALUESTORAGE_H_

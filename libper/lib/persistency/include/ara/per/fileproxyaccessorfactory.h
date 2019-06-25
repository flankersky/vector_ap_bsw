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
/**        \file  fileproxyaccessorfactory.h
 *        \brief  Abstract class FileProxyAccessorFactory which creates objects to read or write memory blocks.
 *
 *      \details  Abstract class FileProxyAccessorFactory which creates objects to read or write memory blocks.
 *
 *********************************************************************************************************************/

#ifndef LIB_PERSISTENCY_INCLUDE_ARA_PER_FILEPROXYACCESSORFACTORY_H_
#define LIB_PERSISTENCY_INCLUDE_ARA_PER_FILEPROXYACCESSORFACTORY_H_

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/
#include <memory>
#include <string>
#include <vector>

#include "vac/memory/smart_base_type_object_pool.h"

#include "ara/per/basicoperations.h"
#include "ara/per/readaccessor.h"
#include "ara/per/readwriteaccessor.h"
#include "ara/per/writeaccessor.h"

namespace ara {
namespace per {

/**
 * \brief Simpler definition for difficult type
 */
template <class T>
using FileProxyAccessUniquePtr = vac::memory::SmartBaseTypeObjectPoolUniquePtr<T>;

/**
 * \brief  Abstract class FileProxyAccessorFactory which creates objects to read or write memory blocks.
 *
 * \trace SWS_PER_00109
 */
class FileProxyAccessorFactory : public vac::memory::DeleteableSmartPoolObject {
 public:
  /**
   * \brief Constructor
   */
  FileProxyAccessorFactory() : vac::memory::DeleteableSmartPoolObject(nullptr) {}

  /**
   * \brief Constructor
   * \param dspo_ptr
   */
  explicit FileProxyAccessorFactory(vac::memory::SmartObjectPoolDeleterContext* dspo_ptr)
      : vac::memory::DeleteableSmartPoolObject(dspo_ptr) {}

  /**
   * \brief Virtual Destructor
   */
  virtual ~FileProxyAccessorFactory() = default;

  /**
   * \brief The function returns a vector of available (file) identifiers within this proxy class.
   * \return Returns a vector of identifiers.
   *
   * \trace SWS_PER_00110
   */
  virtual std::vector<std::string> GetAllKeys() = 0;

  /**
   * \brief  The function deletes a file with the given identifier from this accessor.
   * \param key Identifier of the file.
   *
   * \trace SWS_PER_00111
   */
  virtual void DeleteKey(const std::string& key) = 0;

  /**
   * \brief Query if a file with the given identifier is available in the proxy.
   * \param key Identifier of the file.
   * \return Returns true if the file exists, false otherwise.
   *
   * \trace SWS_PER_00112
   */
  virtual bool HasKey(const std::string& key) = 0;

  /**
   * \brief The function creates an accessor for reading and writing. For keys that are not yet defined in the context
   * of the File-Proxy the same error handling (setting of failbit) shall apply as defined in the C++ specification
   * for std::fstream. If an accessor with a new key would be created the method shall check the attribute
   * PersistencyFileProxyInterface.maxNumberOfFiles and set the failbit for the returned accessor if the defined
   * number is already reached.
   * \param key Identifier of the file.
   * \param mode Mode with which the file shall be opened.
   * \return ReadWriteAccessor associated to the file.
   *
   * \trace SWS_PER_00113
   */
  virtual FileProxyAccessUniquePtr<ReadWriteAccessor> CreateRWAccess(
      std::string const& key,
      BasicOperations::OpenMode const mode = BasicOperations::OpenMode::out | BasicOperations::OpenMode::in) = 0;

  /**
   * \brief The function creates an accessor for reading. For keys that are not yet defined in the context of the
   * File-Proxy the same error handling (setting of failbit) shall apply as defined in the C++ specification for
   * std::fstream.
   * \param key Identifier of the file.
   * \param mode Mode with which the file shall be opened.
   * \return ReadAccessor associated to the file.
   *
   * \trace SWS_PER_00114
   */
  virtual FileProxyAccessUniquePtr<ReadAccessor> CreateReadAccess(
      std::string const& key, BasicOperations::OpenMode const mode = BasicOperations::OpenMode::in) = 0;

  /**
   * \brief The function creates an accessor for writing. For keys that are not yet defined in the context of the
   * File-Proxy the same error handling (setting of failbit) shall apply as defined in the C++ specification for
   * std::fstream. If an accessor with a new key would be created the method shall check the attribute
   * PersistencyFileProxyInterface.maxNumberOfFiles and set the failbit for the returned accessor if the defined number
   * is already reached.
   * \param key Identifier of the file.
   * \param mode Mode with which the file shall be opened.
   * \return WriteAccessor associated to the file.
   *
   * \trace SWS_PER_00115
   */
  virtual FileProxyAccessUniquePtr<WriteAccessor> CreateWriteAccess(
      std::string const& key, BasicOperations::OpenMode const mode = BasicOperations::OpenMode::out) = 0;

 private:
};

/**
 * \brief Short naming for vac::memory::SmartBaseTypeObjectPoolUniquePtr of ara::per::FileProxyAccessorFactory
 */
using FileProxyFactoryUnqiuePtr = vac::memory::SmartBaseTypeObjectPoolUniquePtr<FileProxyAccessorFactory>;

/**
 * \brief The FileProxyAccessorFactory function shall be used as factory to create objects to read and write
 * persistent memory. Please note: This operator is not a member of the FileProxyAccessorFactory class.
 * \param proxy shortName of PortPrototype typed by a PersistencyFileProxyInterface
 * \return Creates an instance of FileProxyAccessorFactory.
 *
 * \trace SWS_PER_00116
 */
FileProxyFactoryUnqiuePtr CreateFileAccessorFactory(std::string proxy);

}  // namespace per
}  // namespace ara

#endif  // LIB_PERSISTENCY_INCLUDE_ARA_PER_FILEPROXYACCESSORFACTORY_H_

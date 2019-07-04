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
/**        \file  file_proxy_accessor_factory_impl.h
 *        \brief  Implementation of the FileProxyAccessorFactory class
 *
 *      \details  Implementation of the FileProxyAccessorFactory class
 *
 *********************************************************************************************************************/

#ifndef LIB_PERSISTENCY_INCLUDE_ARA_PER_FILE_PROXY_ACCESSOR_FACTORY_IMPL_H_
#define LIB_PERSISTENCY_INCLUDE_ARA_PER_FILE_PROXY_ACCESSOR_FACTORY_IMPL_H_

/**********************************************************************************************************************
*  INCLUDES
*********************************************************************************************************************/
#include <array>
#include <climits>
#include <memory>
#include <string>
#include <vector>

#include "vac/container/static_string_stream.h"
#include "vac/container/string_view.h"
#include "vac/memory/smart_base_type_object_pool.h"

#include "ara/per/fileproxyaccessorfactory.h"
#include "ara/per/readaccessor.h"
#include "ara/per/readwriteaccessor.h"
#include "ara/per/writeaccessor.h"

namespace ara {
namespace per {

/**
 * \brief Implementation of the FileProxyAccessorFactory class
 */
class FileProxyAccessorFactoryImpl final : public FileProxyAccessorFactory {
 public:
  /**
   * \brief Constructor
   * \param fileproxy
   * \param ptr
   */
  explicit FileProxyAccessorFactoryImpl(const std::string& fileproxy,
                                        vac::memory::SmartObjectPoolDeleterContext* ptr = nullptr);

  /**
   * \brief Constructor
   * \param fileproxy
   * \param ptr
   */
  explicit FileProxyAccessorFactoryImpl(vac::container::string_view fileproxy,
                                        vac::memory::SmartObjectPoolDeleterContext* ptr = nullptr);

  FileProxyAccessorFactoryImpl(const FileProxyAccessorFactoryImpl& other) = delete;       //< Deleted copy constructor
  FileProxyAccessorFactoryImpl(FileProxyAccessorFactoryImpl&& other) = delete;            //< Deleted move constructor
  FileProxyAccessorFactoryImpl& operator=(const FileProxyAccessorFactoryImpl&) = delete;  //< Deleted copy assignment
  FileProxyAccessorFactoryImpl& operator=(FileProxyAccessorFactoryImpl&&) = delete;       //< Deleted move assignment

  /**
   * Destructor
   */
  ~FileProxyAccessorFactoryImpl() = default;

  /**
     * \brief The function returns a vector of available (file) identifiers within this proxy class.
     * \return Returns a vector of identifiers.
     */
  std::vector<std::string> GetAllKeys() override;

  /**
   * \brief  The function deletes a file with the given identifier from this accessor.
   * \param key Identifier of the file.
   */
  void DeleteKey(const std::string& key) override;

  /**
   * \brief Query if a file with the given identifier is available in the proxy.
   * \param key Identifier of the file.
   * \return Returns true if the file exists, false otherwise.
   */
  bool HasKey(const std::string& key) override;

  /**
   * \brief The function creates an accessor for reading and writing. For keys that are not yet defined in the context
   * of the File-Proxy the same error handling (setting of failbit) shall apply as defined in the C++ specification
   * for std::fstream. If an accessor with a new key would be created the method shall check the attribute
   * PersistencyFileProxyInterface.maxNumberOfFiles and set the failbit for the returned accessor if the defined
   * number is already reached.
   * \param key Identifier of the file.
   * \param mode Mode with which the file shall be opened.
   * \return ReadWriteAccessor associated to the file.
   */
  FileProxyAccessUniquePtr<ReadWriteAccessor> CreateRWAccess(
      std::string const& key,
      BasicOperations::OpenMode const mode = BasicOperations::OpenMode::out | BasicOperations::OpenMode::in) override;

  /**
   * \brief The function creates an accessor for reading. For keys that are not yet defined in the context of the
   * File-Proxy the same error handling (setting of failbit) shall apply as defined in the C++ specification for
   * std::fstream.
   * \param key Identifier of the file.
   * \param mode Mode with which the file shall be opened.
   * \return ReadAccessor associated to the file.
   */
  FileProxyAccessUniquePtr<ReadAccessor> CreateReadAccess(
      std::string const& key, BasicOperations::OpenMode const mode = BasicOperations::OpenMode::in) override;

  /**
   * \brief The function creates an accessor for writing. For keys that are not yet defined in the context of the
   * File-Proxy the same error handling (setting of failbit) shall apply as defined in the C++ specification for
   * std::fstream. If an accessor with a new key would be created the method shall check the attribute
   * PersistencyFileProxyInterface.maxNumberOfFiles and set the failbit for the returned accessor if the defined number
   * is already reached.
   * \param key Identifier of the file.
   * \param mode Mode with which the file shall be opened.
   * \return WriteAccessor associated to the file.
   */
  FileProxyAccessUniquePtr<WriteAccessor> CreateWriteAccess(
      std::string const& key, BasicOperations::OpenMode const mode = BasicOperations::OpenMode::out) override;

 private:
  /**
   * \brief Initialize the path of the file_proxy_name_
   * \param proxy_name
   */
  void InitializeFileProxyName(vac::container::string_view proxy_name);

  /**
   * \brief Determines the amount of ReadWriteAccessor objects in the object pool
   */
  constexpr static const std::uint8_t kNumberReadWriteAccessorObjects{5};

  /**
   * \brief Appends a slash and the given key to the file_proxy_name_
   * \param key The key to be append
   * \return A string_view for the extended file_proxy_name_
   */
  vac::container::StaticStringStream& AppendKeyToFileProxyName(const std::string& key);

  /**
   * \brief Resets the FileProxyName to the given state at construction
   */
  void ResetFileProxyName();

  /**
   * \brief Name of the file proxy
   */
  vac::container::StaticStringStream file_proxy_name_;

  /**
   * \brief Actual position in array file_proxy_name_
   */
  std::size_t pos_;

  /**
   * \brief Holds always the size of the given file_proxy_name at construction
   */
  const std::size_t start_pos_;

  /**
   * \brief Smart object pool for ReadWriteAccessor which will be given out also for ReadAccessor and WriteAccessor
   */
  vac::memory::SmartBaseTypeObjectPool<ReadWriteAccessor> rw_access_pool_;

  FRIEND_TEST(FileProxyFactoryFixture, Construction);              ///< Friend test declaration
  FRIEND_TEST(FileProxyFactoryFixture, ResetFileProxyName);        ///< Friend test declaration
  FRIEND_TEST(FileProxyFactoryFixture, AppendKeyToFileProxyName);  ///< Friend test declaration
};

}  // namespace per
}  // namespace ara

#endif  // LIB_PERSISTENCY_INCLUDE_ARA_PER_FILE_PROXY_ACCESSOR_FACTORY_IMPL_H_

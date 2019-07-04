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
/**        \file  file_proxy_accessor_factory_impl.cc
 *        \brief  Implementation of FileProxyAccessorFactoryImpl
 *
 *      \details  -
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <dirent.h>
#include <algorithm>
#include <cerrno>
#include <cstdio>
#include <iostream>
#include <string>

#include "vac/language/cpp14_backport.h"

#include "ara/per/file_proxy_accessor_factory_impl.h"

namespace ara {
namespace per {

FileProxyAccessorFactoryImpl::FileProxyAccessorFactoryImpl(const std::string& fileproxy,
                                                           vac::memory::SmartObjectPoolDeleterContext* ptr)
    : FileProxyAccessorFactory(ptr), file_proxy_name_(PATH_MAX), pos_(0), start_pos_(fileproxy.size()) {
  InitializeFileProxyName(vac::container::string_view(fileproxy));
  /* reserve space for ReadWriteAccessor smart object pool*/
  // TODO(PAASR-3032): get rid of magic number
  rw_access_pool_.reserve(5);
}

FileProxyAccessorFactoryImpl::FileProxyAccessorFactoryImpl(vac::container::string_view fileproxy,
                                                           vac::memory::SmartObjectPoolDeleterContext* ptr)
    : FileProxyAccessorFactory(ptr), file_proxy_name_(PATH_MAX), pos_(0), start_pos_(fileproxy.size()) {
  InitializeFileProxyName(fileproxy);
  /* reserve space for ReadWriteAccessor smart object pool*/
  // TODO(PAASR-3032): get rid of magic number
  rw_access_pool_.reserve(5);
}

std::vector<std::string> FileProxyAccessorFactoryImpl::GetAllKeys() {
  ResetFileProxyName();
  std::vector<std::string> list_of_files;
  DIR* dir;
  struct dirent* dir_entry;
  dir = opendir(file_proxy_name_.data());
  if (!dir) {
    std::cout << "ERRNO: " << strerror(errno) << " , folder is: " << file_proxy_name_.data() << std::endl;
  }
  while (dir) {
    dir_entry = readdir(dir);
    if (!dir_entry) {
      break;
    }
    // List only regular files
    if (dir_entry->d_type == DT_REG) {
      list_of_files.push_back(dir_entry->d_name);
    }
  }

  return list_of_files;
}

void FileProxyAccessorFactoryImpl::DeleteKey(const std::string& key) {
  AppendKeyToFileProxyName(key);
  remove(file_proxy_name_.data());
  ResetFileProxyName();
}

bool FileProxyAccessorFactoryImpl::HasKey(const std::string& key) {
  auto keys = GetAllKeys();
  return std::find(keys.begin(), keys.end(), key) != keys.end();
}

FileProxyAccessUniquePtr<ReadWriteAccessor> FileProxyAccessorFactoryImpl::CreateRWAccess(
    std::string const& key, BasicOperations::OpenMode const mode) {
  return rw_access_pool_.create(AppendKeyToFileProxyName(key).str(), mode);
}

FileProxyAccessUniquePtr<ReadAccessor> FileProxyAccessorFactoryImpl::CreateReadAccess(
    std::string const& key, BasicOperations::OpenMode const mode) {
  return rw_access_pool_.create(AppendKeyToFileProxyName(key).str(), mode);
}

FileProxyAccessUniquePtr<WriteAccessor> FileProxyAccessorFactoryImpl::CreateWriteAccess(
    std::string const& key, BasicOperations::OpenMode const mode) {
  return rw_access_pool_.create(AppendKeyToFileProxyName(key).str(), mode);
}

vac::container::StaticStringStream& FileProxyAccessorFactoryImpl::AppendKeyToFileProxyName(const std::string& key) {
  ResetFileProxyName();
  const char* slash = "/";
  file_proxy_name_.append(slash);
  file_proxy_name_.append(key.c_str());
  pos_ = start_pos_;
  return file_proxy_name_;
}

void FileProxyAccessorFactoryImpl::ResetFileProxyName() {
  file_proxy_name_.erase(start_pos_, file_proxy_name_.capacity());
}

void FileProxyAccessorFactoryImpl::InitializeFileProxyName(vac::container::string_view proxy_name) {
  if (proxy_name.size() < file_proxy_name_.capacity()) {
    file_proxy_name_.append(proxy_name.data());
    pos_ = start_pos_;
    /* ensure null termination */
    char null_termination = '\0';
    file_proxy_name_.append(&null_termination);
  } else {
    throw std::runtime_error("Filename exceeds size of PATH_MAX.");
  }
}

}  // namespace per
}  // namespace ara

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
/**        \file  fileproxyaccessorfactory.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/per/fileproxyaccessorfactory.h"
#include "ara/per/file_proxy_accessor_factory_impl.h"

namespace ara {
namespace per {

/**
 * \brief Smart pool for file proxy instances
 */
static vac::memory::SmartBaseTypeObjectPool<FileProxyAccessorFactoryImpl> file_proxy_pool_;

FileProxyFactoryUnqiuePtr CreateFileAccessorFactory(std::string proxy) {
  // TODO(PAASR-3032): get rid of magic number
  file_proxy_pool_.reserve(2);
  return file_proxy_pool_.create(proxy);
}

}  // namespace per
}  // namespace ara

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
/**        \file  readaccessor.h
 *        \brief  The Persistency cluster shall provide an abstract class for read accessor operations.
 *
 *      \details  The Persistency cluster shall provide an abstract class for read accessor operations. This class shall
 *                be called ReadAccessor and shall be defined in the readaccessor.h header.
 *
 *********************************************************************************************************************/

#ifndef LIB_PERSISTENCY_INCLUDE_ARA_PER_READACCESSOR_H_
#define LIB_PERSISTENCY_INCLUDE_ARA_PER_READACCESSOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <string>

#include "ara/per/basicoperations.h"

namespace ara {
namespace per {

/**
 * \brief Abstract class for read accessor operations
 *
 * \trace SWS_PER_00117
 * \trace SWS_PER_00131
 */
class ReadAccessor : public BasicOperations {
 public:
  /**
   * \brief Constructor
   */
  ReadAccessor() : BasicOperations(nullptr) {}

  /**
   * \brief Constructor
   * \param ptr
   */
  explicit ReadAccessor(vac::memory::SmartObjectPoolDeleterContext* ptr) : BasicOperations(ptr) {}

  ReadAccessor(const ReadAccessor& other) = delete;       //< Deleted copy Constructor
  ReadAccessor(ReadAccessor&& other) = delete;            //< Deleted move Constructor
  ReadAccessor& operator=(const ReadAccessor&) = delete;  //< Deleted copy assignment operator
  ReadAccessor& operator=(ReadAccessor&&) = delete;       //< Deleted move assignment operator

  /**
   * \brief Virtual destructor
   */
  virtual ~ReadAccessor() = default;

  /**
   * \brief The function reads n bytes into char pointer and returns actual number that were read.
   * \param s Destination pointer for read bytes.
   * \param n Number of bytes that should be read.
   * \return Returns the actual number of read bytes.
   *
   * \trace SWS_PER_00118
   */
  virtual std::size_t read(char* const s, std::size_t const n) = 0;

  /**
   * \brief The function reads the next string and stores it in string.
   * \param string Destination for next string that will be read.
   * \param delim Char that is used as delimiter.
   * \return Returns the ReadAccessor object.
   *
   * \trace SWS_PER_00119
   */
  virtual ReadAccessor& getline(std::string& string, char const delim) = 0;  // NOLINT

  /**
   * \brief The operator reads the next string and stores it in string.
   * \param string Destination for next string that will be read.
   * \return Returns the ReadAccessor object.
   *
   * \trace SWS_PER_00160
   */
  virtual ReadAccessor& operator>>(std::string& string) = 0;

 private:
};

/**
 * \brief The operator reads the next string from ifstream and stores it in string.
 *        Please note: This function is not a member of the ReadAccessor class.
 * \param ifstream ReadAccessor to be read from.
 * \param string Destination for next string that will be read.
 * \param delim Char that is used as delimiter.
 * \return Returns the ReadAccessor object.
 *
 * \trace SWS_PER_00161
 */
inline ReadAccessor& getline(ReadAccessor& ifstream, std::string& string, char const delim) {  // NOLINT
  return ifstream.getline(string, delim);
}

}  // namespace per
}  // namespace ara

#endif  // LIB_PERSISTENCY_INCLUDE_ARA_PER_READACCESSOR_H_

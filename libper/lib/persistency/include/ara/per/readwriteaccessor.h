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
/**        \file  readwriteaccessor.h
 *        \brief  The Persistency cluster shall provide an abstract class for read/write accessor operations.
 *
 *      \details  The Persistency cluster shall provide an abstract class for read/write accessor operations. This class
 *                shall be called ReadWriteAccessor and shall be defined in the readwriteaccessor.h header.
 *
 *********************************************************************************************************************/

#ifndef LIB_PERSISTENCY_INCLUDE_ARA_PER_READWRITEACCESSOR_H_
#define LIB_PERSISTENCY_INCLUDE_ARA_PER_READWRITEACCESSOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <climits>
#include <string>

#include "vac/container/string_view.h"
#include "vac/testing/test_adapter.h"

#include "ara/per/readaccessor.h"

namespace ara {
namespace per {

/**
 * \brief Abstract class for read/write accessor operations
 *
 * \trace SWS_PER_00120
 * \trace SWS_PER_00129
 */
class ReadWriteAccessor final : public ReadAccessor {
 public:
  /**
   * \brief Constructor
   * \param filename
   * \param openmode
   * \param ptr
   */
  ReadWriteAccessor(std::string const& filename, BasicOperations::OpenMode const& openmode,
                    vac::memory::SmartObjectPoolDeleterContext* ptr = nullptr);

  /**
   * \brief Constructor
   * \param filename
   * \param openmode
   * \param ptr
   */
  ReadWriteAccessor(vac::container::string_view filename, BasicOperations::OpenMode const& openmode,
                    vac::memory::SmartObjectPoolDeleterContext* ptr = nullptr);

  /**
   * \brief Destructor
   */
  ~ReadWriteAccessor() = default;

  /**
   * \brief The function flushes and force the OS to write its data to persistent storage.
   *
   * \trace SWS_PER_00122
   */
  void fsync();

  /**
   * \brief The function writes n bytes from s and returns actual number of written bytes.
   * \param s Source of bytes to write.
   * \param n Number of bytes to write.
   * \return Number of actual written bytes.
   *
   * \trace SWS_PER_00123
   */
  std::size_t write(char const* const s, std::size_t const n);

  /**
   * \brief The function passes write buffer to OS.
   *
   * \trace SWS_PER_00124
   */
  void flush();

  /**
   * \brief The operator writes string to the WriteAccessor.
   * \param string The string to be put into the WriteAccessor.
   * \return Returns the WriteAccessor object.
   *
   * \trace SWS_PER_00125
   */
  ReadWriteAccessor& operator<<(std::string const& string);

  /**
   * \brief Operator overload for endl and flush (see below).
   * \param op
   * \return Returns the WriteAccessor object.
   *
   * \trace SWS_PER_00126
   */
  ReadWriteAccessor& operator<<(ReadWriteAccessor& (*op)(ReadWriteAccessor&));

  /**
   * \brief The function reads n bytes into char pointer and returns actual number that were read.
   * \param s Destination pointer for read bytes.
   * \param n Number of bytes that should be read.
   * \return Returns the actual number of read bytes.
   *
   * \trace SWS_PER_00118
   */
  std::size_t read(char* const s, std::size_t const n) override;

  /**
   * \brief The function reads the next string and stores it in string.
   * \param string Destination for next string that will be read.
   * \param delim Char that is used as delimiter.
   * \return Returns the ReadAccessor object.
   *
   * \trace SWS_PER_00119
   */
  ReadWriteAccessor& getline(std::string& string, char const delim) override;  // NOLINT

  /**
   * \brief The operator reads the next string and stores it in string.
   * \param string Destination for next string that will be read.
   * \return Returns the ReadAccessor object.
   *
   * \trace SWS_PER_00160
   */
  ReadWriteAccessor& operator>>(std::string& string) override;

  /**
   * \brief The function returns the current position in byte in the file from start.
   * \return Get the current position in byte in the file from start.
   *
   * \trace SWS_PER_00103
   */
  std::size_t tell() override;

  /**
   * \brief The function sets the position in byte in the file from start.
   * \param pos Sets offset in bytes for seek start.
   *
   * \trace SWS_PER_00104
   */
  void seek(std::size_t const pos) override;

  /**
   * \brief The function sets the position in byte in the file in relation to the current position.
   * \param off Sets offset in bytes for seek start.
   * \param dir Seek direction.
   *
   * \trace SWS_PER_00105
   */
  void seek(std::size_t const off, SeekDirection const dir) override;

  /**
   * \brief Returns true if no error occured.
   * \return Returns true if no error occured.
   *
   * \trace SWS_PER_00106
   */
  bool good() override;

  /**
   * \brief Returns true if end of file was reached.
   * \return Returns true if end of file was reached.
   *
   * \trace SWS_PER_00107
   */
  bool eof() override;

  /**
   * \brief Returns true if an error occurred during operation.
   * \return Returns true if an error occurred during operation.
   *
   * \trace SWS_PER_00108
   */
  bool fail() override;

  /**
   * \brief Returns true if an error occurred during operation with a loss of integrity of the stream.
   * \return Returns true if an error occurred during operation with a loss of integrity of the stream.
   *
   * \trace SWS_PER_00140
   */
  bool bad() override;

  /**
   * \brief Synonym of fail(). Returns true if an error occurred during operation.
   * \return Returns true if an error occurred during operation.
   *
   * \trace SWS_PER_00142
   */
  bool operator!() override;

  /**
   * \brief Synonym of !fail(). Returns false if an error occurred during operation.
   * \return Returns false if an error occurred during operation.
   *
   * \trace SWS_PER_00143
   */
  explicit operator bool() override;

  /**
   * \brief Clears error flags.
   *
   * \trace SWS_PER_00141
   */
  void clear() override;

 private:
  /**
   * \brief Copies the filename into the member
   * \param filename
   */
  void CopyFileName(vac::container::string_view filename);

  /**
   * \brief Name of the file to write to
   */
  std::array<char, PATH_MAX + 1> file_name_;

  /**
   * \brief the input/output file stream used by this class
   */
  std::fstream io_file_stream_;

  FRIEND_TEST(WriteAccessorTest, Construction);   ///< Friend test decleration
  FRIEND_TEST(WriteAccessorTest, Construction2);  ///< Friend test decleration
  FRIEND_TEST(WriteAccessorTest, Endl);           ///< Friend test decleration
  FRIEND_TEST(ReadAccessorTest, EofTest);         ///< Friend test decleration
};

}  // namespace per
}  // namespace ara

#endif  // LIB_PERSISTENCY_INCLUDE_ARA_PER_READWRITEACCESSOR_H_

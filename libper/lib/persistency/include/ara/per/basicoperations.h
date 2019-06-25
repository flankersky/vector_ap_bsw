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
/**        \file  basicoperations.h
 *        \brief  The Persistency cluster shall provide an abstract class for basic file accessor operations.
 *
 *      \details  The Persistency cluster shall provide an abstract class for basic file accessor operations. This class
 *                shall be called BasicOperations and shall be defined in the basicOperations.h header.
 *
 *********************************************************************************************************************/

#ifndef LIB_PERSISTENCY_INCLUDE_ARA_PER_BASICOPERATIONS_H_
#define LIB_PERSISTENCY_INCLUDE_ARA_PER_BASICOPERATIONS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <cstddef>
#include <cstdint>
#include <fstream>

#include "vac/memory/smart_base_type_object_pool.h"

namespace ara {
namespace per {

/**
 * \brief Abstract class for basic file accessor operations
 *
 * \trace SWS_PER_00102
 */
class BasicOperations : public vac::memory::DeleteableSmartPoolObject {
 public:
  /**
   * \brief Specification of seek direction.
   *
   * \trace SWS_PER_00100
   */
  enum class SeekDirection {
    beg = 0,  //< beginning of the stream
    end = 1,  //< end of the stream
    cur = 2   //< current position in the stream
  };

  /**
   * \brief How the file shall be opened.
   *
   * \trace SWS_PER_00101
   */
  enum class OpenMode : uint8_t {
    app = 1 << 0,  //< append - All output operations happen at the end of the file, appending to its existing contents.
    binary = 1 << 1,  //< binary - Operations are performed in binary mode rather than text.
    in = 1 << 2,      //< input - File open for reading
    out = 1 << 3,     //< output - File open for writing
    trunc = 1 << 4,   //< truncate - Any contents that existed in the file before it is open are discarded.
    ate = 1 << 5      //< at end -  The output position starts at the end of the file.
  };

  /**
   * \brief Constructor
   */
  BasicOperations() : vac::memory::DeleteableSmartPoolObject(nullptr) {}

  /**
   * \brief Constructor
   * \param ptr
   */
  explicit BasicOperations(vac::memory::SmartObjectPoolDeleterContext* ptr)
      : vac::memory::DeleteableSmartPoolObject(ptr) {}

  BasicOperations(const BasicOperations& other) = delete;       //< Deleted copy Constructor
  BasicOperations(BasicOperations&& other) = delete;            //< Deleted move Constructor
  BasicOperations& operator=(const BasicOperations&) = delete;  //< Deleted copy assignment operator
  BasicOperations& operator=(BasicOperations&&) = delete;       //< Deleted move assignment operator

  /**
   * \brief Virtual destructor
   */
  virtual ~BasicOperations() = default;

  /**
   * \brief The function returns the current position in byte in the file from start.
   * \return Get the current position in byte in the file from start.
   *
   * \trace SWS_PER_00103
   */
  virtual std::size_t tell() = 0;

  /**
   * \brief The function sets the position in byte in the file from start.
   * \param pos Sets offset in bytes for seek start.
   *
   * \trace SWS_PER_00104
   */
  virtual void seek(std::size_t const pos) = 0;

  /**
   * \brief The function sets the position in byte in the file in relation to the current position.
   * \param off Sets offset in bytes for seek start.
   * \param dir Seek direction.
   *
   * \trace SWS_PER_00105
   */
  virtual void seek(std::size_t const off, SeekDirection const dir) = 0;

  /**
   * \brief Returns true if no error occured.
   * \return Returns true if no error occured.
   *
   * \trace SWS_PER_00106
   */
  virtual bool good() = 0;

  /**
   * \brief Returns true if end of file was reached.
   * \return Returns true if end of file was reached.
   *
   * \trace SWS_PER_00107
   */
  virtual bool eof() = 0;

  /**
   * \brief Returns true if an error occurred during operation.
   * \return Returns true if an error occurred during operation.
   *
   * \trace SWS_PER_00108
   */
  virtual bool fail() = 0;

  /**
   * \brief Returns true if an error occurred during operation with a loss of integrity of the stream.
   * \return Returns true if an error occurred during operation with a loss of integrity of the stream.
   *
   * \trace SWS_PER_00140
   */
  virtual bool bad() = 0;

  /**
   * \brief Synonym of fail(). Returns true if an error occurred during operation.
   * \return Returns true if an error occurred during operation.
   *
   * \trace SWS_PER_00142
   */
  virtual bool operator!() = 0;

  /**
   * \brief Synonym of !fail(). Returns false if an error occurred during operation.
   * \return Returns false if an error occurred during operation.
   *
   * \trace SWS_PER_00143
   */
  virtual explicit operator bool() = 0;

  /**
   * \brief Clears error flags.
   *
   * \trace SWS_PER_00141
   */
  virtual void clear() = 0;

 private:
};

/**
 * \brief Merge all OpenMode modifiers into one OpenMode object.
 * Please note: This operator is not a member of the BasicOperations class.
 * \param left Left OpenMode modifiers.
 * \param right Right OpenMode modifiers.
 * \return Returns merged OpenMode modifiers.
 *
 * \trace SWS_PER_00144
 */
BasicOperations::OpenMode operator|(BasicOperations::OpenMode const& left, BasicOperations::OpenMode const& right);

/**
 * \brief Intersect all OpenMode modifiers into one OpenMode object.
 * Please note: This operator is not a member of the BasicOperations class.
 * \param left Left OpenMode modifiers.
 * \param right Right OpenMode modifiers.
 * \return Returns intersection of OpenMode modifiers.
 *
 * \trace SWS_PER_00145
 */
BasicOperations::OpenMode operator&(BasicOperations::OpenMode const& left, BasicOperations::OpenMode const& right);

/**
 * \brief Iterable list of all OpenModes
 */
static constexpr const std::array<BasicOperations::OpenMode, 6> kOpenModes_{
    {BasicOperations::OpenMode::app, BasicOperations::OpenMode::binary, BasicOperations::OpenMode::in,
     BasicOperations::OpenMode::out, BasicOperations::OpenMode::trunc, BasicOperations::OpenMode::ate}};

}  // namespace per
}  // namespace ara

#endif  // LIB_PERSISTENCY_INCLUDE_ARA_PER_BASICOPERATIONS_H_

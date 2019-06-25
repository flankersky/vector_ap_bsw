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
/**        \file  libosabstraction-interface/include/osabstraction/io/file_descriptor.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_FILE_DESCRIPTOR_H_
#define LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_FILE_DESCRIPTOR_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
namespace osabstraction {
namespace io {

/**
 * \brief Represents a file descriptor obtained from the OS
 */
class FileDescriptor {
 public:
  /**
   * \brief Constructor of FileDescriptor.
   */
  FileDescriptor() : handle_(kInvalidFileDescriptor) {}

  /**
   * \brief Constructor of FileDescriptor.
   *
   * \param handle A socket file descriptor.
   */
  explicit FileDescriptor(int handle) : handle_(handle) {}

  FileDescriptor(const FileDescriptor& other) = delete;             ///< FileDescriptor is not copy-constructable.
  FileDescriptor& operator=(const FileDescriptor& other) = delete;  ///< FileDescriptor is not copy-assignable.

  /**
   * \brief Move Constructor
   */
  FileDescriptor(FileDescriptor&& other) : handle_(other.handle_) { other.handle_ = kInvalidFileDescriptor; }

  /**
   * \brief Move Assignment
   */
  FileDescriptor& operator=(FileDescriptor&& other) {
    this->handle_ = other.handle_;
    other.handle_ = kInvalidFileDescriptor;
    return *this;
  }

  virtual ~FileDescriptor() { Close(); }

  /**
   * \brief Closes the file descriptor.
   */
  virtual void Close();

  /**
   * \brief Returns the raw file descriptor.
   *
   * \return The raw file descriptor.
   */
  int GetHandle() const { return handle_; }

  /**
   * \brief Sets blocking mode of a file descriptor.
   *
   * \param blocking Enables blocking mode if true and disables it otherwise.
   */
  void SetBlockingMode(bool blocking);

  /**
   * \brief Represents an invalid file descriptor.
   */
  static constexpr int kInvalidFileDescriptor = -1;

 protected:
  /**
   * \brief The raw file descriptor.
   */
  int handle_;
};

}  // namespace io
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_IO_FILE_DESCRIPTOR_H_

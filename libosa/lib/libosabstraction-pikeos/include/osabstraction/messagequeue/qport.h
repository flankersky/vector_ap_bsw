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
/**        \file
 *        \brief  PikeOS Queuing Port interface.
 *
 *      \details  Message queues on PikeOS are based on PikeOS Queuing Ports. This has the following impact on the
 *                usage of message queues.
 *
 *                - Message queues cannot be created dynamically on PikeOS. A Queuing Port with the given ID
 *                  has to be existing. If a message queue is opened with an unknown ID, an exception is thrown.
 *
 *                - Message queue attributes are static on PikeOS and cannot be changed during runtime. If attributes
 *                  are set which are not compatible with the statically configured attributes, an exception is thrown.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_QPORT_H_
#define LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_QPORT_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <chrono>   // nanoseconds
#include <cstddef>  // size_t

namespace osabstraction {
namespace messagequeue {

/**
 * \brief Settings to create a message queue.
 */
struct QPortSettings {
  /**
   * \brief Path to the qport.
   */
  const char* id;

  /**
   * \brief True if the message queue shall be checked.
   */
  bool perform_checks;

  /**
   * \brief Access mode flags.
   */
  int flags;

  /**
   * \brief Queue length.
   */
  int length;

  /**
   * \brief Mesage size.
   */
  std::size_t msgsize;
};

/**
 * \brief Manages qport descriptor handling.
 */
class QPort {
 public:
  /**
   * \brief Creates an invalid qport object.
   */
  QPort();

  /**
   * \brief Opens a PikeOS qport.
   *
   * \param[in] settings  QPort settings which shall be used to open the qport.
   *
   * \throws std::runtime_error   Opening qport failed.
   */
  explicit QPort(const QPortSettings& settings);

  /**
   * \brief Closes the qport.
   */
  virtual ~QPort();

  /**
   * \brief Move constructor.
   */
  QPort(QPort&& other);

  /**
   * \brief Move assignment operator.
   */
  QPort& operator=(QPort&& other);

  /**
   * \brief Returns whether the qport is open and can be used for communication.
   *
   * \retval true   QPort object is open.
   * \retval false  QPort object is not open.
   */
  bool IsOpen() const;

  /**
   * \brief Returns the qport handle.
   *
   * \return  QPort handle.
   */
  int GetHandle() const;

  /**
   * \brief Sets the timeout to the given value.
   *
   * \param[in] timeout The timeout which shall be set.
   */
  void SetTimeout(std::chrono::nanoseconds timeout);

  /**
   * \brief Clears the timeout.
   */
  void ClearTimeout();

 private:
  QPort(const QPort& other) = delete;
  QPort& operator=(const QPort& other) = delete;

  /**
   * \brief QPort descriptor.
   */
  int qport_descriptor_;

  /**
   * \brief Tells whether a timeout value is set.
   */
  bool timeout_set_;

  /**
   * \brief Current timeout value.
   */
  std::chrono::nanoseconds timeout_value_;
};

}  // namespace messagequeue
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_MESSAGEQUEUE_QPORT_H_

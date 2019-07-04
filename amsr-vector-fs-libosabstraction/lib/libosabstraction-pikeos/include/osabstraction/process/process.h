/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2018 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file
 *        \brief  Definition of the process interface for PikeOS.
 *
 *      \details  In this implementation PikeOS resource partitions are used as containers for process execution.
 *                The configured resource partitions are managed in a partition pool. Each pool partition is
 *                configured to load the program image from a PikeOS shared memory object. The "load" memory.
 *                The partition's start mode has to be IDLE mode.
 *
 *                If a process is created, a partition is requested from the pool and the required program image is
 *                copied into the shared memory object. Afterwards PikeOS is requested to start the partition.
 *
 *                The POSIX SIGTERM signal is send via a PikeOS Queuing Port. The Application State Client is
 *                listening on the Queuing Port on receiver side and translates it into a regular POSIX signal.
 *
 *                                                                                 Partition Pool
 *                                                                               +------------------+
 *                     +------------------+                                    +------------------+ |
 *                     |    Execution     |               SIGTERM              |    Adaptive      | |
 *                     |    Management    ------------------------------------->    Application   | |
 *                     |                  |     EM copies elf file             |                  | |
 *                     |                  |     from file system               |                  | |
 *                     |                  |     into shared memory             |                  | |
 *                     |                  -------------------+                 |                  | |
 *                     |                  |                  |                 |                  | |
 *                     |                  |                  |                 |                  | |
 *                     |                  |                  |                 |                  | |
 *                     |                  |                  |                 |                  | |
 *                     |         ^        |                  |                 |         ^        |-+
 *                     +--------/|\-------+                  |                 +--------/|\-------+
 *                               |                           |                           |
 *                               |                           |                           |
 *                               |                           |                           |
 *                               |                          \|/                          | PikeOS ELF loader
 *                    +----------|----------+     +----------V---------+                 | loads elf file
 *                    |      File System    |     |    Shared Memory   |                 | into partition
 *                    |                     |     |                    |                 |
 *                    |   +---------------+ |     | +----------------+ ------------------+
 *                    |   |   App.elf     | |     | |    App.elf     | |
 *                    |   +---------------+ |     | +----------------+ |
 *                    +---------------------+     +--------------------+
 *
 *                    +----------------------------------------------------------------------------+
 *                    |                                                                            |
 *                    |                                   PikeOS                                   |
 *                    |                                                                            |
 *                    +----------------------------------------------------------------------------+
 *
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_PROCESS_PROCESS_H_
#define LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_PROCESS_PROCESS_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <mutex>

#include "osabstraction/internal/partition.h"
#include "osabstraction/internal/sharedmemory.h"
#include "osabstraction/messagequeue/senderqueue.h"
#include "osabstraction/process/processinterface.h"

namespace osabstraction {
namespace process {

/**
 * \brief Process class.
 */
class Process : public ProcessInterface {
 public:
  /**
   * \brief Creates and starts a process.
   *
   * Acquires a resource partition, loads the given program image into the shared memory and starts
   * it in the partition.
   *
   * \param[in]   image       Program image that contains the executed machine code.
   * \param[in]   args        Command line argument which shall be passed to the process.
   */
  Process(ProgramImage image, Arguments args);

  /**
   * \brief Kills the process.
   *
   * Performs a partition shutdown and puts the partition back into the partition pool.
   */
  virtual ~Process() {}

  /**
   * \brief Move constructor.
   */
  Process(Process&& other);

  /**
   * \brief Move assignment operator.
   */
  Process& operator=(Process&& other);

  /**
   * \brief Returns whether the process is still running.
   *
   * \retval true   Process is running.
   * \retval false  Process is not running.
   */
  bool IsRunning() override;

  /**
   * \brief Sends a termination request to the process.
   */
  void SendTerminationRequest() override;

  /**
   * \brief Returns the process ID.
   *
   * Process IDs may be recycled, i.e. if a process has been killed, a new process may get its ID.
   *
   * \return Process ID
   */
  ProcessId GetId() const override;

 private:
  Process(const Process& other) = delete;
  Process& operator=(const Process& other) = delete;

  /**
   * \brief The associated PikeOS resource partition.
   */
  internal::Partition partition_;

  /**
   * \brief Currently assumed running state.
   */
  bool is_running_;

  /**
   * \brief Queue which is used for sending termination requests.
   */
  osabstraction::messagequeue::SenderQueue signal_queue_;

  /**
   * \brief Load memory.
   *
   * The load memory is a PikeOS shared memory object, which is used to load application binaries into partitions.
   * The load memory is a singleton.
   */
  class LoadMemory : public internal::SharedMemory {
   public:
    /**
     * \brief Get instance of the load memory.
     */
    static LoadMemory& GetInstance();

    /**
     * \brief Lock for operating on the Load memory.
     */
    std::mutex mutex_;

   private:
    /**
     * \brief Path to shared memory which is used for loading binaries into partitions.
     */
    constexpr static const char* kPathToSharedMemory = "load";

    /**
     * \brief Private constructor.
     */
    LoadMemory() : internal::SharedMemory(kPathToSharedMemory), mutex_() {}
  };
};

/**
 * \brief Process builder for processes.
 */
class ProcessBuilder : public ProcessBuilderInterface {
 public:
  /**
   * \brief Creates a process object with given program image and arguments.
   */
  Process Build();
};

}  // namespace process
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_PROCESS_PROCESS_H_

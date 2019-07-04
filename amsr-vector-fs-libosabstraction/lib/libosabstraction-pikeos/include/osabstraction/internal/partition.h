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
 *        \brief  Interface to work with PikeOS resource partitions (a.k.a. partitions).
 *
 *      \details  PikeOS resource partitions can be thought of as containers for program execution.
 *                Partitions define system resources which are available to the program which is executed within.
 *                The program is not able to access system resources which are not part of its partition.
 *                Access to other partitions or system resources is restricted by the PikeOS microkernel.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_INTERNAL_PARTITION_H_
#define LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_INTERNAL_PARTITION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <mutex>
#include <utility>
#include <vector>

namespace osabstraction {
namespace internal {

/**
 * \brief Returns the ID of the calling partition.
 */
int GetPartitionId();

class PartitionPool;

/**
 * \brief PikeOS resource partition.
 *
 * The number of PikeOS partitions is statically configured in the PikeOS configuration.
 */
class Partition {
 public:
  /**
   * \brief Constructor.
   *
   * Do not use the constructor directly. Always use the PartitionPool instead.
   *
   * \param[in] id  The id if the new partition. Default is invalid partition ID.
   */
  explicit Partition(int id);

  /**
   * \brief Destructor.
   *
   * Shutdown and release partition.
   */
  ~Partition();

  /**
   * \brief Move constructor.
   */
  Partition(Partition&& other);

  /**
   * \brief Move assignment operator.
   */
  Partition& operator=(Partition&& other);

  /**
   * \brief Loads program image into given partition and starts execution.
   */
  void Start();

  /**
   * \brief Shutdown the partition.
   */
  void Shutdown();

  /**
   * \brief Returns the partition identifier.
   */
  int GetId() const { return id_; }

  /**
   * \brief Returns whether the partition is running.
   *
   * \retval true   Partition is running.
   * \retval false  Partition is not running.
   */
  bool IsRunning();

 private:
  Partition(const Partition& other) = delete;
  Partition& operator=(const Partition& other) = delete;

  /**
   * \brief Partition identifier.
   */
  int id_;
};

/**
 * \brief Pool of partitions.
 *
 * The partition pool is a singleton, because partitions are a system resource.
 */
class PartitionPool {
 public:
  /**
   * \brief Returns a free resource partition from the pool.
   */
  Partition GetPartition();

  /**
   * \brief Puts a resource partition back into the pool.
   *
   * \param[in] partition Partition which shall be put into the pool.
   */
  void ReleasePartition(Partition&& partition);

  /**
   * \brief Get the partition pool singleton instance.
   */
  static PartitionPool& GetInstance();

 private:
  PartitionPool(PartitionPool const&) = delete;
  void operator=(PartitionPool const&) = delete;

  /**
   * \brief Creates a partition pool.
   *
   * The pool contains all other partitions which are configured in the PikeOS system,
   * The own partition is not part of the pool.
   */
  PartitionPool();

  /**
   * \brief List of partition which are free to use.
   */
  std::vector<Partition> partitions_;

  /**
   * \brief Lock for operating on the partition pool.
   */
  std::mutex mutex_;
};

}  // namespace internal
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_PIKEOS_INCLUDE_OSABSTRACTION_INTERNAL_PARTITION_H_

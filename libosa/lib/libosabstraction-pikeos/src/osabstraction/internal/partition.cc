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
 *        \brief  Implementation of the PikeOS resource partition interface.
 *
 *      \details  Implements interface to work with PikeOS resource partitions.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ddapi/ddapi.h>  // dd_*()
#include <p4.h>           // P4_NUM_RESPART

#include <unistd.h>

#include <stdexcept>
#include <utility>

#include "osabstraction/internal/partition.h"

namespace osabstraction {
namespace internal {

/**
 * \brief Maximum number of PikeOS resource partitions.
 */
constexpr int kMaxNumOfPartitions = P4_NUM_RESPART;

/**
 * \brief Smallest PikeOS resource partition ID which shall be used in the partition pool.
 *
 * Partition ID 1 is used system internally.
 */
constexpr int kMinPartitionId = 2;

/**
 * \brief Highest PikeOS resource partition ID which shall be used in the partition pool.
 */
constexpr int kMaxPartitionId = kMaxNumOfPartitions;

/**
 * \brief Invalid partition ID.
 */
constexpr int kInvalidPartitionId = -1;

int GetPartitionId() {
  dd_os_part_info_t pinfo;
  pinfo.part = -1;  // -1 refers to the calling partition.
  if (dd_os_control(DD_OS_PART_INFO, &pinfo) != 0) {
    throw std::runtime_error("Could not fetch ID of the calling partition.");
  }
  return pinfo.part;
}

/**
 * \brief Helper function to change partition mode.
 *
 * \param[in] partition_id  Partition ID which shall be modified.
 * \param[in] mode          PikeOS partition mode which shall be set.
 */
static void SetPartitionMode(int partition_id, dd_os_part_mode_t mode) {
  dd_os_set_part_mode_t pmode;

  pmode.part = partition_id;
  pmode.mode = mode;

  /* #00 Tell PikeOS to change partition mode. */
  if (dd_os_control(DD_OS_SET_PART_MODE, &pmode) != 0) {
    throw std::runtime_error(std::string("Failed to change mode of Partition ID: ") + std::to_string(partition_id));
  }
}

Partition::Partition(int id = kInvalidPartitionId) : id_(id) {}

Partition::~Partition() {
  /* #00 If partition is valid: */
  if (id_ != kInvalidPartitionId) {
    /* #00 Tell PikeOS to shutdown the partition. */
    Shutdown();

    /* #00 Put the partition back into the pool. */
    internal::PartitionPool::GetInstance().ReleasePartition(std::move(*this));
  }
}

Partition::Partition(Partition&& other) : id_(std::move(other.id_)) {
  /* #00 Mark other partition as invalid. */
  other.id_ = kInvalidPartitionId;
}

Partition& Partition::operator=(Partition&& other) {
  std::swap(id_, other.id_);
  return *this;
}

void Partition::Start() {
  /* #00 Tell PikeOS to start the partition. */
  SetPartitionMode(id_, DD_OS_MODE_COLD_START);
}

void Partition::Shutdown() {
  /* #00 Tell PikeOS to shutdown the partition. */
  SetPartitionMode(id_, DD_OS_MODE_IDLE);
}

bool Partition::IsRunning() {
  dd_os_part_info_t pinfo;

  pinfo.part = id_;

  if (dd_os_control(DD_OS_PART_INFO, &pinfo) != 0) {
    throw std::runtime_error(std::string("Could not fetch partition information. Partition ID: ") +
                             std::to_string(id_));
  }

  return (pinfo.mode != DD_OS_MODE_IDLE);
}

PartitionPool& PartitionPool::GetInstance() {
  static PartitionPool instance;
  return instance;
}

PartitionPool::PartitionPool() : partitions_(kMaxNumOfPartitions), mutex_() {
  int my_partition_id = GetPartitionId();

  /* #00 Critical section: */
  {
    std::unique_lock<std::mutex> lock{mutex_};

    /* #00 Fetch information from all resource partitions. */
    for (int id = kMinPartitionId; id < kMaxPartitionId; ++id) {
      dd_os_part_info_t pinfo;
      pinfo.part = id;

      /* #00 If there is a resource partition configured for this ID: */
      if (dd_os_control(DD_OS_PART_INFO, &pinfo) == 0) {
        /* #00 Skip myself. */
        if (pinfo.part == my_partition_id) {
          continue;
        } else {
          Partition partition(id);

          /* #00 If partition is not in idle mode, stop it. */
          if (pinfo.mode != DD_OS_MODE_IDLE) {
            partition.Shutdown();
          }

          /* #00 Create partition object and put it into the pool. */
          partitions_.push_back(std::move(partition));
        }
      }
    }
  }
}

Partition PartitionPool::GetPartition() {
  /* #00 Check that there are partitions available */
  if (partitions_.size() <= 0) {
    throw std::runtime_error("No partitions in pool. All partitions already in use.");
  }

  /* #00 Critical section: */
  {
    std::unique_lock<std::mutex> lock{mutex_};

    Partition partition = std::move(partitions_.back());
    partitions_.pop_back();
    return partition;
  }
}

void PartitionPool::ReleasePartition(Partition&& partition) {
  /* #00 Critical section: */
  {
    std::unique_lock<std::mutex> lock{mutex_};

    partitions_.push_back(std::move(partition));
  }
}

}  // namespace internal
}  // namespace osabstraction

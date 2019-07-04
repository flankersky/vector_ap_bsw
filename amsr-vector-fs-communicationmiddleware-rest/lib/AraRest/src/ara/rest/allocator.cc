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
/**        \file  allocator.cc
 *
 *********************************************************************************************************************/

#include "ara/rest/allocator.h"

namespace ara {
namespace rest {

Allocator::~Allocator() {}

void* Allocator::allocate(std::size_t bytes, std::size_t alignment) {
  /* TODO(x) implement me */
  (void)bytes;
  (void)alignment;
  return 0;
}

void Allocator::deallocate(void* p, std::size_t bytes, std::size_t alignment) {
  /* TODO(x) implement me */
  (void)p;
  (void)bytes;
  (void)alignment;
}

/* TODO(x) need to clarify with visjak */
inline bool Allocator::is_equal(const Allocator& other) const noexcept { return (*this == other); }

Allocator* NewDeleteAllocator() noexcept {
  static allocator::NewDelete x{};
  return &x;
}

/* TODO(x) need to clarify with visjak */
bool operator==(const Allocator& a, const Allocator& b) { return a.is_equal(b); }

/* TODO(x) need to clarify with visjak */
bool operator!=(const Allocator& a, const Allocator& b) { return !a.is_equal(b); }

namespace allocator {

std::atomic<Allocator*> DefaultAllocator{};

}  // namespace allocator
}  // namespace rest
}  // namespace ara

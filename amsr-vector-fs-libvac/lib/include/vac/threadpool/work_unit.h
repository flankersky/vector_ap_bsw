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
/**        \file  work_unit.h
 *        \brief
 *
 *      \details  -
 *
 *********************************************************************************************************************/
#ifndef LIB_INCLUDE_VAC_THREADPOOL_WORK_UNIT_H_
#define LIB_INCLUDE_VAC_THREADPOOL_WORK_UNIT_H_

#include <memory>
#include <ostream>

namespace vac {
namespace threadpool {

/**
 * \brief This class represents a unit of work that can be submitted to the ThreadPool.
 */
class WorkUnit {
 public:
  /**
   * \brief Destructor.
   */
  virtual ~WorkUnit() = default;

  /**
   * \brief Method to actually execute the WorkUnit.
   */
  virtual void Run() noexcept = 0;

  /**
   * \brief Print identifying information about this WorkUnit.
   *
   * \param stream output stream.
   *
   * This method is used when, e.g., Run() throws an exception of any kind.
   */
  virtual void PrintToStream(std::ostream& stream) const = 0;
};

/**
 * \brief Printing information of workunit in a stream.
 *
 * \param stream output stream.
 * \param work_unit Unit of work.
 *
 * \return reference to the output stream.
 *
 */
inline std::ostream& operator<<(std::ostream& stream, const WorkUnit& work_unit) {
  work_unit.PrintToStream(stream);
  return stream;
}

} /* namespace threadpool */
} /* namespace vac */

#endif  // LIB_INCLUDE_VAC_THREADPOOL_WORK_UNIT_H_

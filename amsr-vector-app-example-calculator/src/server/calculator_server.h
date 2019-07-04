/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  calculator_server.cc
 *        \brief  Class contains the calculator server service implementation
 *
 *      \details  -
 *
 *
 *********************************************************************************************************************/

#ifndef SRC_SERVER_CALCULATOR_SERVER_H_
#define SRC_SERVER_CALCULATOR_SERVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <ara/log/logging.hpp>
#include <chrono>
#include <thread>

#include "vector/calculatorService/calculatorInterface.h"
#include "vector/calculatorService/calculatorInterface_skeleton.h"

namespace vector {
namespace calculator {

class calculatorServer : public vector::calculatorService::CalculatorInterfaceSkeleton {
 public:
  /**
    * \brief  Constructor of the class calculatorServer that provides the calculatorService
    * \param  instance_id Contains instance id
    */
  explicit calculatorServer(ara::com::InstanceIdentifier instance_id);
  ~calculatorServer() {}

 private:
  /**
    * \brief  Holds the logging context for the server
    */
  ara::log::Logger& logger_ctx;
  /**
    * \brief  Method adds the two given arguments. Notice: There are no overflow protection checks
    * \param  arg2  1st Parameter
    * \param  arg1  2st Parameter
    * \return  Returns a future for async function call handling
    */
  ara::com::Future<vector::calculatorService::skeleton::methods::Add::Output> add(
      const vector::calculatorService::uint32& arg1, const vector::calculatorService::uint32& arg2) override;
  /**
    * \brief  Method divides the two given arguments. Notice: A division by zero will cause an divisionByZero event
    * \param  divisor Divisor
    * \param  dividend Dividend
    * \return  Returns a future for async function call handling
    */
  ara::com::Future<vector::calculatorService::skeleton::methods::Divide::Output> divide(
      const vector::calculatorService::uint32& divident, const vector::calculatorService::uint32& divisor) override;

  /**
    * \brief  Method subtracts the two given arguments. Notice: No bound checks
    * \param  arg2
    * \param  arg1
    * \return  Returns a future for async function call handling
    */
  ara::com::Future<vector::calculatorService::skeleton::methods::Subtract::Output> subtract(
      const vector::calculatorService::uint32& arg1, const vector::calculatorService::uint32& arg2) override;
};

}  // namespace calculator
}  // namespace vector

#endif  // SRC_SERVER_CALCULATOR_SERVER_H_

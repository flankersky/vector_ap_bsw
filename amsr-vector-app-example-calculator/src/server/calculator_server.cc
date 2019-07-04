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

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "calculator_server.h"
#include <functional>
#include <iostream>
#include <utility>

using namespace ara::log;  // NOLINT 'using' for ara::log should be OK.

namespace vector {
namespace calculator {

calculatorServer::calculatorServer(ara::com::InstanceIdentifier instance_id)
    : vector::calculatorService::CalculatorInterfaceSkeleton(instance_id),
      logger_ctx{CreateLogger("CSRV", "Context for calculator method implementation")} {
  LogInfo() << "Offer calculator server service";
  // Initialize field value before offering service
  divideResult.Send(0);

  // Offer service
  vector::calculatorService::CalculatorInterfaceSkeleton::OfferService();
}

ara::com::Future<vector::calculatorService::skeleton::methods::Subtract::Output> calculatorServer::subtract(
    const vector::calculatorService::uint32& arg1, const vector::calculatorService::uint32& arg2) {
  logger_ctx.LogInfo() << "Subtract method get's called with arguments: " << arg1 << " and " << arg2;
  calculatorService::skeleton::methods::Subtract::Output ret;
  // Simulates heavy processing load
  std::this_thread::sleep_for(std::chrono::seconds(5));
  ret.result = arg1 - arg2;
  ara::com::Promise<vector::calculatorService::skeleton::methods::Subtract::Output> promise;
  promise.set_value(std::move(ret));
  return promise.get_future();
}

ara::com::Future<vector::calculatorService::skeleton::methods::Add::Output> calculatorServer::add(
    const vector::calculatorService::uint32& arg1, const vector::calculatorService::uint32& arg2) {
  logger_ctx.LogInfo() << "Add method get's called with arguments: " << arg1 << " and " << arg2;
  calculatorService::skeleton::methods::Add::Output ret;
  ret.result = arg1 + arg2;
  ara::com::Promise<vector::calculatorService::skeleton::methods::Add::Output> promise;
  promise.set_value(std::move(ret));
  return promise.get_future();
}

ara::com::Future<vector::calculatorService::skeleton::methods::Divide::Output> calculatorServer::divide(
    const vector::calculatorService::uint32& dividend, const vector::calculatorService::uint32& divisor) {
  logger_ctx.LogInfo() << "Divide method get's called with dividend: " << dividend << " and divisor: " << divisor;
  calculatorService::skeleton::methods::Divide::Output ret;
  if (divisor != 0) {
    ret.result = dividend / divisor;
    divideResult.Send(ret.result);
  } else {
    ret.result = 0;
    // Send divisionByZero event
    divisionByZero.Send(static_cast<vector::calculatorService::boolean>(true));
  }

  ara::com::Promise<calculatorService::skeleton::methods::Divide::Output> promise;
  promise.set_value(std::move(ret));
  return promise.get_future();
}

}  // namespace calculator
}  // namespace vector

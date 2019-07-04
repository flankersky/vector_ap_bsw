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
 *        \brief  Signal receiver implementation for POSIX OSs.
 *
 *      \details  Nothing to do for POSIX OSs.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_SIGNALRECEIVER_H_
#define LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_SIGNALRECEIVER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "osabstraction/process/signalreceiverinterface.h"

namespace osabstraction {
namespace process {

/**
 * \brief Empty implementation for POSIX OSs.
 * POSIX compliant OS provides MULTI_PROCESS POSIX interface and allows usage of sending signals to other processes.
 * Nothing to implement here.
 */
class SignalReceiver final {};

}  // namespace process
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_POSIX_INCLUDE_OSABSTRACTION_PROCESS_SIGNALRECEIVER_H_

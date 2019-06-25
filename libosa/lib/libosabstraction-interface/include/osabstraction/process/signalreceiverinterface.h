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
 *        \brief  Signal receiver interface.
 *
 *      \details  AUTOSAR Adaptive requires sending SIGTERM signals from the EM to its Adaptive Applications.
 *
 *                Sending signals to other processes requires POSIX_MULTI_PROCESS functionality which is beyond
 *                POSIX PSE51. Some operating systems do not support this for safety and security reasons.
 *                A signal receiver is introduced which hides OS specific details on signal reception to bypass this
 *                restriction.
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_PROCESS_SIGNALRECEIVERINTERFACE_H_
#define LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_PROCESS_SIGNALRECEIVERINTERFACE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

namespace osabstraction {
namespace process {

/**
 * \brief Handles signal reception.
 *
 * Signal receiver is a singleton. There should be only one instance per process.
 */
class SignalReceiver;

}  // namespace process
}  // namespace osabstraction

#endif  // LIB_LIBOSABSTRACTION_INTERFACE_INCLUDE_OSABSTRACTION_PROCESS_SIGNALRECEIVERINTERFACE_H_

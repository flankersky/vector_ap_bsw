/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  uds_transport_protocol_manager.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDM_INCLUDE_UDSTRANSPORT_UDS_TRANSPORT_PROTOCOL_MANAGER_H_
#define LIB_LIBDM_INCLUDE_UDSTRANSPORT_UDS_TRANSPORT_PROTOCOL_MANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <tuple>
#include <utility>
#include "udstransport/uds_message.h"
#include "udstransport/uds_transport_protocol_types.h"

namespace ara {
namespace diag {
namespace udstransport {
class UdsTransportProtocolHandler;  // forward declaration

/**
 * \brief Interface for UDS transport protocol manager.
 */
class UdsTransportProtocolMgr {
 public:
  /**
   * \brief Destructor
   */
  virtual ~UdsTransportProtocolMgr() = default;
  /**
   * \brief type of target address in UdsMessage
   */
  enum class TransmissionResult : std::uint8_t { kTransmitOk = 0, kTransmitFailed = 1 };

  /**
   * \brief type of result of UdsMessge indication.
   */
  enum class IndicationResult : std::uint8_t { kIndicationOk = 0, kIndicationBusy = 1, kIndicationOverflow = 2 };

  /**
   * \brief type definition of std::pair of IndicationResult and UdsMessage pointer.
   */
  using IndicationPair = std::pair<IndicationResult, UdsMessage::Ptr>;

  /**
   * \brief Type of tuple to pack UdsTransportProtocolHandlerID and ChannelID together, to form a global unique (among
   * all used
   * UdsTransportProtocolHandlers within DM) identifier of a UdsTransportProtocol channel.
   */
  using GlobalChannelIdentifier = std::tuple<UdsTransportProtocolHandlerID, ChannelID>;

  /**
   * \brief Indicates a message start.
   * This is an interface, which is just served/called by UdsTransportProtocolHandlers, which return true from
   * UdsTransportProtocolHandlers::isStartOfMessageIndicationSupported().
   *
   * \param source_addr UDS source address of message
   * \param target_addr UDS target address of message
   * \param type indication whether its is phys/func request
   * \param global_channel_id transport protocol channel on which message start happened
   * \param size size in bytes of the UdsMessage starting from SID.
   * \param meta_info meta information for the UDS message.
   *
   * \return A pair of IndicationResult to be filled with Indication result and a pointer to UdsMessage owned/created by
   * DM core and returned to the handler to get filled.
   */
  virtual IndicationPair IndicateMessage(UdsMessage::Address source_addr, UdsMessage::Address target_addr,
                                         UdsMessage::TargetAddressType type, GlobalChannelIdentifier global_channel_id,
                                         std::size_t size, std::shared_ptr<UdsMessage::MetaInfoMap> meta_info) = 0;

  /**
   * \brief Indicates, that the message indicated via IndicateMessage() has failure and will not lead to a final
   * HandleMessage() call.
   *
   * \param message unique pointer to the current UDS message
   */
  virtual void NotifyMessageFailure(UdsMessage::ConstPtr message) = 0;

  /**
   * \brief Hands over a valid received Uds message (currently this is only a request type) from transport layer to
   * session layer.
   * It corresponds to T_Data.ind of Figure 2 from ISO 14229-2. The behavior is asynchronously. I.e. the UdsMessage is
   * handed over to Session Layer and it is expected, that it "instantly" returns, which means, that real processing
   * of the message shall be done asynchronously!
   *
   * \param message The Uds message ptr (unique_ptr semantics) with the request. Ownership of the UdsMessage is given
   * back to the generic DM core here.
   *
   */
  virtual void HandleMessage(UdsMessage::Ptr message) = 0;

  /**
   * \brief notification about the outcome of a transmit request called by core DM at the handler via
   * UdsTransportProtocolHandler::Transmit
   *
   * This transmit API covers T_Data.con of ISO 14229-2 Figure 2.
   *
   * \param message for which message (created in IndicateMessage()) this is the confirmation.
   * \param result Result of transmission. In case UDS message could be transmitted on network layer: kTransmitOk),
   *        kTransmitFailed else.
   */
  virtual void TransmitConfirmation(UdsMessage::ConstPtr message, TransmissionResult result) = 0;

  /**
   * \brief notification call from the given transport channel, that it has been reestablished since the last (Re)Start
   * from
   * the UdsTransportProtocolHandler to which this channel belongs.
   * To activate this notification a previous call to UdsTransportProtocolHandler::NotifyReestablishment() has to be
   * done.
   * See further documentation at UdsTransportProtocolHandler::NotifyReestablishment().
   *
   * \param global_channel_id transport protocol channel, which is available again.
   */
  virtual void ChannelReestablished(GlobalChannelIdentifier global_channel_id) = 0;

  /**
   * \brief notification from handler, that it has stopped now (e.g. closed down network connections, freed resources,
   * etc...)
   *
   * This callback is expected as a reaction from handler to a call to UdsTransportProtocolHandler::Stop.
   *
   * \param handler_id indication, which plugin stopped.
   */
  virtual void HandlerStopped(UdsTransportProtocolHandlerID handler_id) = 0;

  /**
   * \brief return the uds transport protocol handler with the corresponding ID
   *
   * \param  handler_id of the handle.
   * \return reference to the uds transport protocol handler.
   */
  virtual UdsTransportProtocolHandler* GetUdsTransportProtocolHandler(UdsTransportProtocolHandlerID handler_id) = 0;
};

}  // namespace udstransport
}  // namespace diag
}  // namespace ara

#endif  // LIB_LIBDM_INCLUDE_UDSTRANSPORT_UDS_TRANSPORT_PROTOCOL_MANAGER_H_

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
/**        \file  uds_transport_protocol_handler.h
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

#ifndef LIB_LIBDM_INCLUDE_UDSTRANSPORT_UDS_TRANSPORT_PROTOCOL_HANDLER_H_
#define LIB_LIBDM_INCLUDE_UDSTRANSPORT_UDS_TRANSPORT_PROTOCOL_HANDLER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <memory>
#include <string>
#include "udstransport/uds_transport_protocol_manager.h"
#include "udstransport/uds_transport_protocol_types.h"

namespace ara {
namespace diag {
namespace udstransport {

/**
 * \brief Exception used in UdsTransportProtocolHandler.
 */
class UdsTransportProtocolHandlerException : public std::runtime_error {
 public:
  /**
   * \brief Using constructor of base class
   */
  using std::runtime_error::runtime_error;
};

/**
 * \brief Interface for UDS transport protocol.
 */
class UdsTransportProtocolHandler {
 protected:
  /**
   * \brief The UdsTransportProtocolMgr used by the DM/DCM.
   */
  UdsTransportProtocolMgr& transportprotocol_manager_;

 public:
  /**
   * \brief Constructor of UdsTransportProtocolHandler.
   *
   * \param handler_id the handler ID used by DM to identify this handler. This is just a number/identification
   * given by the DM core when instantiating a UdsTransportProtocolHandler instance to be able to distinguish it from
   * other handler-plugins or built-in UdsTransportProtocolHandler implementations.
   * \param transport_protocol_mgr reference to UdsTransportProtocolMgr owned by this DM, with which
   * UdsTransportProtocolHandler
   * instance shall interact.
   */
  explicit UdsTransportProtocolHandler(const UdsTransportProtocolHandlerID handler_id,
                                       UdsTransportProtocolMgr& transport_protocol_mgr)
      : transportprotocol_manager_(transport_protocol_mgr), handler_id_(handler_id) {}
  virtual ~UdsTransportProtocolHandler() {}

  /**
   * \brief Unique pointer to transport protocol handler.
   */
  using Ptr = std::unique_ptr<UdsTransportProtocolHandler>;

  /**
   * \brief Initializes handler.
   *
   * Must be called before Start(). The idea is to have "initialization" of handler-plugin separated from its ctor.
   *
   * \throws  ara::diag::udstransport::UdsTransportProtocolHandlerException if initialization fails.
   */
  virtual void Initialize() = 0;

  /**
   * \brief Start processing the implemented Uds Transport Protocol.
   *
   * The implementation shall call its superclass Start() method as there might be some stack specific implementation.
   * Implementation shall be asynchronous as DM might start many/different UdsTransportProtocolHandler in parallel and
   * strong serialization of all those starts just unnecessarily slows down DM startup.
   */
  virtual void Start() = 0;

  /**
   * \brief Method to indicate that this UdsTransportProtocolHandler should terminate.
   *
   * If UdsTransportProtocolHandler has stopped, it shall call
   * UdsTransportProtocolMgr::HandlerStopped(UdsTransportProtocolHandlerID)
   *
   * After return from Stop(), the handler-plugin shall NOT call to UdsTransportProtocolMgr with any other method but
   * UdsTransportProtocolMgr::HandlerStopped()
   *
   */
  virtual void Stop() = 0;

  /**
   * \brief Return the UdsTransportProtocolPluginID, which was given to the implementation during construction (ctor
   * call)
   */
  virtual UdsTransportProtocolHandlerID GetHandlerID() const { return handler_id_; }

  /**
   * \brief Tells the UdsTransportProtocolHandler, that it shall notify the DM core
   * via UdsTransportProtocolMgr::ChannelReestablished())
   * if the given channel has been re-established after next UdsTransportProtocolHandler::Start().
   *
   * The main purpose of this method is to allow DM to provide an ECU-Reset (0x11 service), with configuration option
   * "Pos. response AFTER reset". In this scenario the request for 0x11 will be received on a certain channel with
   * identifying tuple <p_x, c_y> (GlobalChannelIdentifier). Then the ECU-Reset takes place and after ECU-Restart all
   * UdsProtocolHandlers/plugins get restarted via call to UdsTransportProtocolHandler::Start().
   * Now there are two expectations, when this method has been called before and returned "true":
   * - IF the same remote client connects to the UdsProtocolHandler, it shall get a channel identification with the
   * same identifying tuple <p_x, c_y> as last time.
   * - it shall call UdsTransportProtocolMgr::ChannelReestablished(GlobalChannelIdentifier<p_x, c_y>)
   *
   * Note1: IF the underlying network layer of the UdsTransportProtocolHandler isn't really connection based (e.g. a
   * UDP based protocol), then the UdsTransportProtocolHandler shall call
   * UdsTransportProtocolMgr::ChannelReestablished()
   * after UdsTransportProtocolHandler::Start() as soon as it detects/assumes that the remote client/tester will be
   * reachable again.
   *
   * Note2: The detection/decision, whether the "same" client reconnects as before is an UdsProtocolHandler
   * implementation specific decision. The general expectation is: If the channel is set up from exactly the same
   * remote network-endpoint, it typically shall be given the same channelID (c_y part of the tuple).
   * To support this functionality the implementation at least has to store non-volatile, that this notification has to
   * be done.
   * Further it might be needed to store some additional connection specific info non-volatile to make sure, that the
   * same channelID (c_y part of the tuple) can be reassigned. This is the case if the mapping of protocol specific
   * channel info -> channelID isn't a stable bijective mapping!
   * Small example:
   * The underlying network protocol, which UdsProtocolHandler implements is based on TCP.
   * At the point in time, where the 0x11 SI request is received on channel identified by <p_x, c_y> the DM calls
   * NotifyReestablishment() on this channelID.
   * Now the implementation of UdsProtocolHandler stores non-volatile in the context of this call:
   * - the NetworkEndpoint (IP-address and port number) of the channel
   * - the NetworkEndpoint (IP-address and port number) of the local port (because in this example, the
   * UdsTransportProtocolHandler listens on/supports different ports)
   * - the channelID (c_y part) it has currently assigned.
   * After restart this channelID only shall be reused for a channel with exactly the same NetworkEndpoint addresses
   * as stored non-volatile. If this channelID then gets reassigned, then
   * UdsTransportProtocolMgr::ChannelReestablished() has to be called.
   *
   * \param channel_id channelID, whose re-establishment shall be notified to UdsTransportProtocolMgr
   *
   * \returns true if notification request is accepted and can be fulfilled.
   */
  virtual bool NotifyReestablishment(ChannelID channel_id) = 0;

  /**
   * \brief Transmit a Uds message via the underlying Uds Transport Protocol channel.
   *
   * This transmit API covers T_Data.req of ISO 14229-2 Figure 2.
   *
   * \param message The message to be transmitted as a UdsMessage::Ptr (unique_ptr style). UdsTransportProtocolHandler
   * has to give back this UdsMessage::Ptr via UdsTransportProtocolMgr::TransmitConfirmation() to signal, that it is
   * done with this message.
   *
   * \param channel_id identification of channel on which to transmit.
   *
   */
  virtual void Transmit(UdsMessage::Ptr message, ChannelID channel_id) = 0;

 private:
  /**
   * shall be set by initializer list of ctor.
   */
  const UdsTransportProtocolHandlerID handler_id_;
};

}  // namespace udstransport
}  // namespace diag
}  // namespace ara
#endif  // LIB_LIBDM_INCLUDE_UDSTRANSPORT_UDS_TRANSPORT_PROTOCOL_HANDLER_H_

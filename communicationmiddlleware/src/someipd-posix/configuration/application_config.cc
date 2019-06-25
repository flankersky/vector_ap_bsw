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
/**        \file  application_config.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "someipd-posix/configuration/application_config.h"
#include <limits>

namespace someipd_posix {
namespace configuration {

const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";

ConfigurationTypesAndDefs::ServiceContainer ApplicationConfig::ReadServices() const {
  printf("ReadServices 1\n");  
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  const ara::per::internal::json::JsonDocument& config{app_gen_};
  printf("ReadServices 2\n");
  //assert(config.HasMember("services") && config["services"].IsArray());
  printf("ReadServices 3\n");
  ServiceContainer services;
  services.reserve(config["services"].Size());

  printf("ReadServices 4\n");
  for (const auto& sc : config["services"].GetArray()) {
    Service service{};
    printf("ReadServices 5\n");

    assert(sc.HasMember("service_id") && sc["service_id"].IsUint());
    service.id_ = static_cast<std::uint16_t>(sc["service_id"].GetUint());
    assert(sc.HasMember("major_version") && sc["major_version"].IsUint());
    service.major_version_ = static_cast<std::uint8_t>(sc["major_version"].GetUint());
    assert(sc.HasMember("minor_version") && sc["minor_version"].IsUint());
    service.minor_version_ = static_cast<std::uint32_t>(sc["minor_version"].GetUint());

    /* Parse methods */
    assert(sc.HasMember("methods") && sc["methods"].IsArray());

    for (const auto& mc : sc["methods"].GetArray()) {
      Method method{};

      assert(mc.HasMember("id") && mc["id"].IsUint());
      method.id_ = static_cast<std::uint16_t>(mc["id"].GetUint());
      assert(mc.HasMember("proto") && mc["proto"].IsString() && (mc["proto"] == "udp" || mc["proto"] == "tcp"));
      if (mc["proto"] == "tcp") {
        method.proto_ = Protocol::kTCP;
      } else {
        method.proto_ = Protocol::kUDP;
      }

      if (mc.HasMember("tx_trigger")) {
        assert(method.proto_ == Protocol::kUDP);
        assert(mc["tx_trigger"].IsObject());
        const auto& ttc = mc["tx_trigger"];
        assert(ttc.HasMember("mode") && ttc["mode"].IsString() && (ttc["mode"] == "always" || ttc["mode"] == "never"));
        method.tx_trigger_.is_valid_ = true;
        if (ttc["mode"] == "always") {
          method.tx_trigger_.mode_ = TransmissionTriggerMode::kTransmissionTriggerModeAlways;
          method.tx_trigger_.timeout_ = std::chrono::nanoseconds{0};
        } else {
          assert(ttc.HasMember("timeout_ns") && ttc["timeout_ns"].IsUint64());
          method.tx_trigger_.mode_ = TransmissionTriggerMode::kTransmissionTriggerModeNever;
          method.tx_trigger_.timeout_ = std::chrono::nanoseconds{ttc["timeout_ns"].GetUint64()};
        }
      } else {
        method.tx_trigger_.is_valid_ = false;
      }

      service.methods_.push_back(method);
    }

    /* Parse events */
    assert(sc.HasMember("events") && sc["events"].IsArray());
    for (const auto& ec : sc["events"].GetArray()) {
      Event event;
      assert(ec.HasMember("id") && ec["id"].IsUint());
      event.id_ = static_cast<std::uint16_t>(ec["id"].GetUint());
      assert(ec.HasMember("field") && ec["field"].IsBool());
      event.is_field_ = ec["field"].GetBool();

      assert(ec.HasMember("proto") && ec["proto"].IsString() && (ec["proto"] == "udp" || ec["proto"] == "tcp"));
      if (ec["proto"] == "tcp") {
        event.proto_ = Protocol::kTCP;
      } else {
        event.proto_ = Protocol::kUDP;
      }

      if (ec.HasMember("tx_trigger")) {
        assert(event.proto_ == Protocol::kUDP);
        assert(ec["tx_trigger"].IsObject());
        const auto& ttc = ec["tx_trigger"];
        assert(ttc.HasMember("mode") && ttc["mode"].IsString() && (ttc["mode"] == "always" || ttc["mode"] == "never"));
        event.tx_trigger_.is_valid_ = true;
        if (ttc["mode"] == "always") {
          event.tx_trigger_.mode_ = TransmissionTriggerMode::kTransmissionTriggerModeAlways;
          event.tx_trigger_.timeout_ = std::chrono::nanoseconds{0};
        } else {
          assert(ttc.HasMember("timeout_ns") && ttc["timeout_ns"].IsUint64());
          event.tx_trigger_.mode_ = TransmissionTriggerMode::kTransmissionTriggerModeNever;
          event.tx_trigger_.timeout_ = std::chrono::nanoseconds{ttc["timeout_ns"].GetUint64()};
        }
      } else {
        event.tx_trigger_.is_valid_ = false;
      }

      service.events_.push_back(event);
    }

    /* Parse eventgroups */
    assert(sc.HasMember("eventgroups") && sc["eventgroups"].IsArray());
    for (const auto& egc : sc["eventgroups"].GetArray()) {
      Eventgroup eventgroup{};
      assert(egc.HasMember("id") && egc["id"].IsUint());
      eventgroup.id_ = static_cast<std::uint16_t>(egc["id"].GetUint());
      /* Parse event ids */
      assert(egc.HasMember("events") && egc["events"].IsArray());
      for (const auto& eidc : egc["events"].GetArray()) {
        assert(eidc.IsUint());
        auto eid = static_cast<std::uint16_t>(eidc.GetUint());
        eventgroup.events_.push_back(eid);
      }

      service.eventgroups_.push_back(eventgroup);
    }

    services.push_back(service);
  }

  return services;
}

ConfigurationTypesAndDefs::RequiredServiceInstanceContainer ApplicationConfig::ReadRequiredServiceInstances() const {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  const ara::per::internal::json::JsonDocument& config{app_gen_};
  assert(config.HasMember("required_service_instances") && config["required_service_instances"].IsArray());

  RequiredServiceInstanceContainer required_service_instances;
  required_service_instances.reserve(config["required_service_instances"].Size());

  for (const auto& rsic : config["required_service_instances"].GetArray()) {
    RequiredServiceInstance required_service_instance{};

    assert(rsic.HasMember("service_id") && rsic["service_id"].IsUint());
    required_service_instance.service_id_ = static_cast<std::uint16_t>(rsic["service_id"].GetUint());
    assert(rsic.HasMember("instance_id") && rsic["instance_id"].IsUint());
    required_service_instance.instance_id_ = static_cast<std::uint16_t>(rsic["instance_id"].GetUint());
    assert(rsic.HasMember("major_version") && rsic["major_version"].IsUint());
    required_service_instance.major_version_ = static_cast<std::uint8_t>(rsic["major_version"].GetUint());
    assert(rsic.HasMember("major_version") && rsic["major_version"].IsUint());
    required_service_instance.minor_version_ = static_cast<std::uint32_t>(rsic["minor_version"].GetUint());
    /* Parse service discovery parameters */
    assert(rsic.HasMember("service_discovery") && rsic["service_discovery"].IsObject());
    const auto& sdc = rsic["service_discovery"];
    assert(sdc.HasMember("ttl") && sdc["ttl"].IsUint());
    required_service_instance.service_discovery_.ttl_ = sdc["ttl"].GetUint();
    assert(required_service_instance.service_discovery_.ttl_ > 0);
    assert(sdc.HasMember("initial_delay_min_ns") && sdc["initial_delay_min_ns"].IsUint64());
    required_service_instance.service_discovery_.initial_delay_min_ns_ = sdc["initial_delay_min_ns"].GetUint64();
    assert(sdc.HasMember("initial_delay_max_ns") && sdc["initial_delay_max_ns"].IsUint64());
    required_service_instance.service_discovery_.initial_delay_max_ns_ = sdc["initial_delay_max_ns"].GetUint64();
    assert(sdc.HasMember("initial_repetitions_base_delay_ns") && sdc["initial_repetitions_base_delay_ns"].IsUint64());
    required_service_instance.service_discovery_.initial_repetitions_base_delay_ns_ =
        sdc["initial_repetitions_base_delay_ns"].GetUint64();
    assert(sdc.HasMember("initial_repetitions_max") && sdc["initial_repetitions_max"].IsUint());
    required_service_instance.service_discovery_.initial_repetitions_max_ = sdc["initial_repetitions_max"].GetUint();
    /* Parse SD eventgroups */
    assert(sdc.HasMember("eventgroups") && sdc["eventgroups"].IsArray());
    for (const auto& egc : sdc["eventgroups"].GetArray()) {
      RequiredServiceInstanceSdEventgroup eventgroup;
      assert(egc.HasMember("id") && egc["id"].IsUint());
      eventgroup.id_ = static_cast<std::uint16_t>(egc["id"].GetUint());
      assert(egc.HasMember("ttl") && egc["ttl"].IsUint());
      eventgroup.ttl_ = egc["ttl"].GetUint();
      assert(eventgroup.ttl_ > 0);
      if (egc.HasMember("request_response_delay_min_ns")) {
        assert(egc["request_response_delay_min_ns"].IsUint64());
        eventgroup.request_response_delay_min_ns_ = egc["request_response_delay_min_ns"].GetUint64();
      } else {
        eventgroup.request_response_delay_min_ns_ = 0;
      }
      if (egc.HasMember("request_response_delay_max_ns")) {
        assert(egc["request_response_delay_max_ns"].IsUint64());
        eventgroup.request_response_delay_max_ns_ = egc["request_response_delay_max_ns"].GetUint64();
      } else {
        eventgroup.request_response_delay_max_ns_ = 0;
      }
      assert(eventgroup.request_response_delay_max_ns_ >= eventgroup.request_response_delay_min_ns_);
      required_service_instance.service_discovery_.eventgroups_.push_back(eventgroup);
    }
    /* Parse port mapping */
    assert(rsic.HasMember("port_mapping") && rsic["port_mapping"].IsObject());
    const auto& pmc = rsic["port_mapping"];
    assert(pmc.HasMember("address") && pmc["address"].IsString());
    required_service_instance.port_mapping_.address_ = pmc["address"].GetString();
    if (pmc.HasMember("tcp_port")) {
      assert(pmc["tcp_port"].IsUint());
      required_service_instance.port_mapping_.tcp_port_ = static_cast<std::uint16_t>(pmc["tcp_port"].GetUint());
      assert(required_service_instance.port_mapping_.tcp_port_ != kInvalidPort);
    } else {
      required_service_instance.port_mapping_.tcp_port_ = kInvalidPort;
    }
    if (pmc.HasMember("udp_port")) {
      assert(pmc["udp_port"].IsUint());
      required_service_instance.port_mapping_.udp_port_ = static_cast<std::uint16_t>(pmc["udp_port"].GetUint());
      assert(required_service_instance.port_mapping_.udp_port_ != kInvalidPort);
    } else {
      required_service_instance.port_mapping_.udp_port_ = kInvalidPort;
    }
    assert(!pmc.HasMember("event_multicast_address") && !pmc.HasMember("event_multicast_port"));
    required_service_instance.port_mapping_.event_multicast_address_ = kInvalidAddress;
    required_service_instance.port_mapping_.event_multicast_port_ = kInvalidPort;

    required_service_instances.push_back(required_service_instance);
  }

  return required_service_instances;
}

ConfigurationTypesAndDefs::ProvidedServiceInstanceContainer ApplicationConfig::ReadProvidedServiceInstances() const {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  const ara::per::internal::json::JsonDocument& config{app_gen_};
  assert(config.HasMember("provided_service_instances") && config["provided_service_instances"].IsArray());
  ProvidedServiceInstanceContainer provided_service_instances;

  for (const auto& psic : config["provided_service_instances"].GetArray()) {
    ProvidedServiceInstance provided_service_instance;
    assert(psic.HasMember("service_id") && psic["service_id"].IsUint());
    provided_service_instance.service_id_ = static_cast<std::uint16_t>(psic["service_id"].GetUint());
    assert(psic.HasMember("instance_id") && psic["instance_id"].IsUint());
    provided_service_instance.instance_id_ = static_cast<std::uint16_t>(psic["instance_id"].GetUint());
    assert(psic.HasMember("major_version") && psic["major_version"].IsUint());
    provided_service_instance.major_version_ = static_cast<std::uint8_t>(psic["major_version"].GetUint());
    assert(psic.HasMember("minor_version") && psic["minor_version"].IsUint());
    provided_service_instance.minor_version_ = static_cast<std::uint32_t>(psic["minor_version"].GetUint());
    /* Parse service discovery parameters */
    logger_.LogDebug() << __func__ << ":" << __LINE__;
    assert(psic.HasMember("service_discovery") && psic["service_discovery"].IsObject());
    const auto& sdc = psic["service_discovery"];
    assert(sdc.HasMember("ttl") && sdc["ttl"].IsUint());
    provided_service_instance.service_discovery_.ttl_ = sdc["ttl"].GetUint();
    assert(provided_service_instance.service_discovery_.ttl_ > 0);
    assert(sdc.HasMember("initial_delay_min_ns") && sdc["initial_delay_min_ns"].IsUint64());
    provided_service_instance.service_discovery_.initial_delay_min_ns_ = sdc["initial_delay_min_ns"].GetUint64();
    assert(sdc.HasMember("initial_delay_max_ns") && sdc["initial_delay_max_ns"].IsUint64());
    provided_service_instance.service_discovery_.initial_delay_max_ns_ = sdc["initial_delay_max_ns"].GetUint64();
    assert(sdc.HasMember("initial_repetitions_base_delay_ns") && sdc["initial_repetitions_base_delay_ns"].IsUint64());
    provided_service_instance.service_discovery_.initial_repetitions_base_delay_ns_ =
        sdc["initial_repetitions_base_delay_ns"].GetUint64();
    assert(sdc.HasMember("initial_repetitions_max") && sdc["initial_repetitions_max"].IsUint());
    provided_service_instance.service_discovery_.initial_repetitions_max_ = sdc["initial_repetitions_max"].GetUint();
    assert(sdc.HasMember("cyclic_offer_delay_ns") && sdc["cyclic_offer_delay_ns"].IsUint64());
    provided_service_instance.service_discovery_.cyclic_offer_delay_ns_ = sdc["cyclic_offer_delay_ns"].GetUint64();
    assert(sdc.HasMember("request_response_delay_min_ns") && sdc["request_response_delay_min_ns"].IsUint64());
    provided_service_instance.service_discovery_.request_response_delay_min_ns_ =
        sdc["request_response_delay_min_ns"].GetUint64();
    assert(sdc.HasMember("request_response_delay_max_ns") && sdc["request_response_delay_max_ns"].IsUint64());
    provided_service_instance.service_discovery_.request_response_delay_max_ns_ =
        sdc["request_response_delay_max_ns"].GetUint64();
    /* Parse SD eventgroups */
    logger_.LogDebug() << __func__ << ":" << __LINE__;
    assert(sdc.HasMember("eventgroups") && sdc["eventgroups"].IsArray());
    for (const auto& egc : sdc["eventgroups"].GetArray()) {
      ProvidedServiceInstanceSdEventgroup eventgroup;
      assert(egc.HasMember("id") && egc["id"].IsUint());
      eventgroup.id_ = static_cast<std::uint16_t>(egc["id"].GetUint());
      assert(egc.HasMember("ttl") && egc["ttl"].IsUint());
      eventgroup.ttl_ = egc["ttl"].GetUint();
      assert(eventgroup.ttl_ > 0);
      if (egc.HasMember("event_multicast_threshold")) {
        assert(egc["event_multicast_threshold"].IsUint());
        eventgroup.event_multicast_threshold_ = egc["event_multicast_threshold"].GetUint();
      } else {
        eventgroup.event_multicast_threshold_ = kEventMulticastThresholdUseOnlyUnicast;
      }
      if (egc.HasMember("request_response_delay_min_ns")) {
        assert(egc["request_response_delay_min_ns"].IsUint64());
        eventgroup.request_response_delay_min_ns_ = egc["request_response_delay_min_ns"].GetUint64();
      } else {
        eventgroup.request_response_delay_min_ns_ = 0;
      }
      if (egc.HasMember("request_response_delay_max_ns")) {
        assert(egc["request_response_delay_max_ns"].IsUint64());
        eventgroup.request_response_delay_max_ns_ = egc["request_response_delay_max_ns"].GetUint64();
      } else {
        eventgroup.request_response_delay_max_ns_ = 0;
      }
      assert(eventgroup.request_response_delay_max_ns_ >= eventgroup.request_response_delay_min_ns_);
      provided_service_instance.service_discovery_.eventgroups_.push_back(eventgroup);
    }
    /* Parse port mappings */
    logger_.LogDebug() << __func__ << ":" << __LINE__;
    assert(psic.HasMember("port_mappings") && psic["port_mappings"].IsArray());
    for (const auto& pmc : psic["port_mappings"].GetArray()) {
      NetworkEndpointPortMapping port_mapping;
      assert(pmc.HasMember("address") && pmc["address"].IsString());
      port_mapping.address_ = pmc["address"].GetString();
      if (pmc.HasMember("tcp_port")) {
        assert(pmc["tcp_port"].IsUint());
        port_mapping.tcp_port_ = static_cast<std::uint16_t>(pmc["tcp_port"].GetUint());
        assert(port_mapping.tcp_port_ != kInvalidPort);
      } else {
        port_mapping.tcp_port_ = kInvalidPort;
      }
      if (pmc.HasMember("udp_port")) {
        assert(pmc["udp_port"].IsUint());
        port_mapping.udp_port_ = static_cast<std::uint16_t>(pmc["udp_port"].GetUint());
        assert(port_mapping.udp_port_ != kInvalidPort);
      } else {
        port_mapping.udp_port_ = kInvalidPort;
      }
      if (pmc.HasMember("event_multicast_address")) {
        assert(pmc["event_multicast_address"].IsString());
        port_mapping.event_multicast_address_ = pmc["event_multicast_address"].GetString();
      } else {
        port_mapping.event_multicast_address_ = kInvalidAddress;
      }
      if (pmc.HasMember("event_multicast_port")) {
        assert(pmc["event_multicast_port"].IsUint());
        port_mapping.event_multicast_port_ = static_cast<std::uint16_t>(pmc["event_multicast_port"].GetUint());
        assert(port_mapping.event_multicast_port_ != kInvalidPort);
      } else {
        port_mapping.event_multicast_port_ = kInvalidPort;
      }
      provided_service_instance.port_mappings_.push_back(port_mapping);
    }
    provided_service_instances.push_back(provided_service_instance);
  }

  return provided_service_instances;
}

ConfigurationTypesAndDefs::NetworkEndpointContainer ApplicationConfig::ReadNetworkEndpoints() const {
  logger_.LogDebug() << __func__ << ":" << __LINE__;
  const ara::per::internal::json::JsonDocument& config{app_gen_};
  assert(config.HasMember("network_endpoints") && config["network_endpoints"].IsArray());
  NetworkEndpointContainer network_endpoints{};

  for (const auto& nec : config["network_endpoints"].GetArray()) {
    NetworkEndpoint network_endpoint{};
    assert(nec.HasMember("address") && nec["address"].IsString());
    network_endpoint.address_ = nec["address"].GetString();
    assert(nec.HasMember("mtu") && nec["mtu"].IsUint());
    network_endpoint.mtu_ = nec["mtu"].GetUint();
    assert(nec.HasMember("service_discovery") && nec["service_discovery"].IsObject());
    const auto& sdc = nec["service_discovery"];
    assert(sdc.HasMember("multicast_address") && sdc["multicast_address"].IsString());
    network_endpoint.service_discovery_.multicast_address_ = sdc["multicast_address"].GetString();
    assert(sdc.HasMember("port") && sdc["port"].IsUint());
    network_endpoint.service_discovery_.port_ = static_cast<std::uint16_t>(sdc["port"].GetUint());
    assert(nec.HasMember("ports") && nec["ports"].IsArray());
    for (const auto& pc : nec["ports"].GetArray()) {
      logger_.LogDebug() << __func__ << ":" << __LINE__;
      NetworkEndpointPort port{};
      assert(pc.HasMember("port") && pc["port"].IsUint());
      port.port_ = static_cast<std::uint16_t>(pc["port"].GetUint());
      assert(port.port_ != kInvalidPort);
      assert(pc.HasMember("proto") && pc["proto"].IsString() && (pc["proto"] == "udp" || pc["proto"] == "tcp"));

      if (pc["proto"] == "tcp") {
        port.proto_ = Protocol::kTCP;

        if (pc.HasMember("keep_alive") && pc["keep_alive"].IsObject()) {
          const auto& keep_alive = pc["keep_alive"];
          assert(keep_alive.HasMember("idle_time_s") && keep_alive["idle_time_s"].IsUint());
          assert(keep_alive.HasMember("alive_interval_s") && keep_alive["alive_interval_s"].IsUint());
          assert(keep_alive.HasMember("retry_count") && keep_alive["retry_count"].IsInt());
          port.options_.keep_alive_.first = true;
          const std::chrono::seconds idle_time{keep_alive["idle_time_s"].GetUint()};
          const std::chrono::seconds alive_interval{keep_alive["alive_interval_s"].GetUint()};
          const int retry_count{keep_alive["retry_count"].GetInt()};
          assert(retry_count >= 0);
          port.options_.keep_alive_.second =
              someip_posix_common::someip::KeepAliveParameters{idle_time, alive_interval, retry_count};
        }

      } else {
        port.proto_ = Protocol::kUDP;
      }

      if (pc.HasMember("qos") && pc["qos"].IsInt()) {
        port.options_.qos_.first = true;
        port.options_.qos_.second = pc["qos"].GetInt();
      } else {
        port.options_.qos_.first = false;
      }

      if (pc.HasMember("tx_trigger")) {
        assert(port.proto_ == Protocol::kUDP);
        assert(pc["tx_trigger"].IsObject());
        const auto& ttc = nec["tx_trigger"];
        assert(ttc.HasMember("buffer_size") && ttc["buffer_size"].IsUint());
        port.tx_trigger_.is_valid_ = true;
        port.tx_trigger_.buffer_size_ = static_cast<std::uint32_t>(ttc["buffer_size"].GetUint());
      } else {
        port.tx_trigger_.is_valid_ = false;
        port.tx_trigger_.buffer_size_ = 0;
      }

      network_endpoint.ports_.push_back(port);
    }

    network_endpoints.push_back(network_endpoint);
  }

  return network_endpoints;
}

}  // namespace configuration
}  // namespace someipd_posix

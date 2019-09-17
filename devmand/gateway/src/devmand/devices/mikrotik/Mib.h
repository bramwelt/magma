// Copyright (c) Facebook, Inc. and its affiliates. All Rights Reserved.

#pragma once

#include <folly/futures/Future.h>

#include <devmand/channels/snmp/Channel.h>

namespace devmand {
namespace devices {
namespace mikrotik {

class Mib {
 public:
  Mib() = delete;
  ~Mib() = delete;
  Mib(const Mib&) = delete;
  Mib& operator=(const Mib&) = delete;
  Mib(Mib&&) = delete;
  Mib& operator=(Mib&&) = delete;

 public:
  static folly::Future<std::string> getBaseMac(
      channels::snmp::Channel& channel);

  static folly::Future<std::string> getSerialNumber(
      channels::snmp::Channel& channel);

  static folly::Future<std::string> getFirmwareVersion(
      channels::snmp::Channel& channel);

  static folly::Future<std::string> getModel(channels::snmp::Channel& channel);

  // TODO move into DISMAN-EVENT-MIB
  static folly::Future<std::string> getUpTime(channels::snmp::Channel& channel);

  static folly::Future<std::string> getLongtitude(
      channels::snmp::Channel& channel);

  static folly::Future<std::string> getLatitude(
      channels::snmp::Channel& channel);

  static folly::Future<std::string> getAltitude(
      channels::snmp::Channel& channel);

  static folly::Future<std::string> getIpv4Address(
      channels::snmp::Channel& channel);

  static folly::Future<std::string> getIpv6Address(
      channels::snmp::Channel& channel);
};

} // namespace mikrotik
} // namespace devices
} // namespace devmand

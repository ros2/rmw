// Copyright 2020 Ericsson AB
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "gmock/gmock.h"
#include "osrf_testing_tools_cpp/scope_exit.hpp"
#include "rcutils/allocator.h"

#include "rmw/error_handling.h"
#include "rmw/network_flow.h"
#include "rmw/types.h"

TEST(test_network_flow, zero_init) {
  rmw_network_flow_t network_flow = rmw_get_zero_initialized_network_flow();
  EXPECT_EQ(network_flow.transport_protocol, 0) << "Non-zero transport protocol";
  EXPECT_EQ(network_flow.internet_protocol, 0) << "Non-zero internet protocol";
  EXPECT_EQ(network_flow.transport_port, 0u) << "Non-zero transport_port";
  EXPECT_EQ(network_flow.flow_label, 0u) << "Non-zero flow label";
  for (uint8_t i = 0; i < RMW_INET_ADDRSTRLEN; i++) {
    EXPECT_EQ(network_flow.internet_address[i], 0) << "Non-zero internet address";
  }
}

TEST(test_network_flow, set_transport_protocol) {
  rmw_network_flow_t network_flow = rmw_get_zero_initialized_network_flow();
  rmw_transport_protocol_t transport_protocol = RMW_TRANSPORT_PROTOCOL_UDP;

  rmw_ret_t ret = rmw_network_flow_set_transport_protocol(nullptr, transport_protocol);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for null network_flow";
  rmw_reset_error();

  ret = rmw_network_flow_set_transport_protocol(&network_flow, transport_protocol);
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid arguments";
  EXPECT_EQ(
    network_flow.transport_protocol,
    transport_protocol) << "transport_protocol value is not as expected";
}

TEST(test_network_flow, get_transport_protocol_string) {
  const char * transport_protocol_udp_str = rmw_network_flow_get_transport_protocol_string(
    RMW_TRANSPORT_PROTOCOL_UDP);
  EXPECT_STREQ(transport_protocol_udp_str, "UDP") <<
    "Expected \"UDP\" as string variant of RMW_TRANSPORT_PROTOCOL_UDP";
  rmw_reset_error();

  const char * transport_protocol_tcp_str = rmw_network_flow_get_transport_protocol_string(
    RMW_TRANSPORT_PROTOCOL_TCP);
  EXPECT_STREQ(transport_protocol_tcp_str, "TCP") <<
    "Expected \"TCP\" as string variant of RMW_TRANSPORT_PROTOCOL_TCP";
  rmw_reset_error();

  const char * transport_protocol_unknown_str = rmw_network_flow_get_transport_protocol_string(
    RMW_TRANSPORT_PROTOCOL_UNKNOWN);
  EXPECT_STREQ(transport_protocol_unknown_str, "Unknown") <<
    "Expected \"Unknown\" as string variant of RMW_TRANSPORT_PROTOCOL_UNKNOWN";
  rmw_reset_error();

  const char * transport_protocol_count_str = rmw_network_flow_get_transport_protocol_string(
    RMW_TRANSPORT_PROTOCOL_COUNT);
  EXPECT_STREQ(transport_protocol_count_str, "Unknown") <<
    "Expected \"Unknown\" as string variant of RMW_TRANSPORT_PROTOCOL_COUNT";
  rmw_reset_error();

  const char * transport_protocol_invalid_str =
    rmw_network_flow_get_transport_protocol_string(
    (rmw_transport_protocol_t)(
      RMW_TRANSPORT_PROTOCOL_COUNT + 1));
  EXPECT_STREQ(transport_protocol_invalid_str, "Unknown") <<
    "Expected \"Unknown\" if transport protocol argument is invalid";
}

TEST(test_network_flow, set_internet_protocol) {
  rmw_network_flow_t network_flow = rmw_get_zero_initialized_network_flow();
  rmw_internet_protocol_t internet_protocol = RMW_INTERNET_PROTOCOL_IPV4;

  rmw_ret_t ret = rmw_network_flow_set_internet_protocol(nullptr, internet_protocol);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for null network_flow";
  rmw_reset_error();

  ret = rmw_network_flow_set_internet_protocol(&network_flow, internet_protocol);
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid arguments";
  EXPECT_EQ(
    network_flow.internet_protocol,
    internet_protocol) << "internet_protocol value is not as expected";
}

TEST(test_network_flow, get_internet_protocol_string) {
  const char * internet_protocol_ipv4_str = rmw_network_flow_get_internet_protocol_string(
    RMW_INTERNET_PROTOCOL_IPV4);
  EXPECT_STREQ(internet_protocol_ipv4_str, "IPv4") <<
    "Expected \"IPv4\" as string variant of RMW_INTERNET_PROTOCOL_IPV4";
  rmw_reset_error();

  const char * internet_protocol_ipv6_str = rmw_network_flow_get_internet_protocol_string(
    RMW_INTERNET_PROTOCOL_IPV6);
  EXPECT_STREQ(internet_protocol_ipv6_str, "IPv6") <<
    "Expected \"IPv6\" as string variant of RMW_INTERNET_PROTOCOL_IPV6";
  rmw_reset_error();

  const char * internet_protocol_unknown_str = rmw_network_flow_get_internet_protocol_string(
    RMW_INTERNET_PROTOCOL_UNKNOWN);
  EXPECT_STREQ(internet_protocol_unknown_str, "Unknown") <<
    "Expected \"Unknown\" as string variant of RMW_INTERNET_PROTOCOL_UNKNOWN";
  rmw_reset_error();

  const char * internet_protocol_count_str = rmw_network_flow_get_internet_protocol_string(
    RMW_INTERNET_PROTOCOL_COUNT);
  EXPECT_STREQ(internet_protocol_count_str, "Unknown") <<
    "Expected \"Unknown\" as string variant of RMW_INTERNET_PROTOCOL_COUNT";
  rmw_reset_error();

  const char * internet_protocol_invalid_str =
    rmw_network_flow_get_internet_protocol_string(
    (rmw_internet_protocol_t)(
      RMW_INTERNET_PROTOCOL_COUNT + 1));
  EXPECT_STREQ(internet_protocol_invalid_str, "Unknown") <<
    "Expected \"Unknown\" if internet protocol argument is invalid";
}

TEST(test_network_flow, set_transport_port) {
  rmw_network_flow_t network_flow = rmw_get_zero_initialized_network_flow();
  uint16_t transport_port = 45678;

  rmw_ret_t ret = rmw_network_flow_set_transport_port(nullptr, transport_port);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for null network_flow";
  rmw_reset_error();

  ret = rmw_network_flow_set_transport_port(&network_flow, transport_port);
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid arguments";
  EXPECT_EQ(
    network_flow.transport_port,
    transport_port) << "transport_port value is not as expected";
}

TEST(test_network_flow, set_flow_label) {
  rmw_network_flow_t network_flow = rmw_get_zero_initialized_network_flow();
  uint32_t flow_label = 0xabcde;

  rmw_ret_t ret = rmw_network_flow_set_flow_label(nullptr, flow_label);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for null network_flow";
  rmw_reset_error();

  ret = rmw_network_flow_set_flow_label(&network_flow, flow_label);
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid arguments";
  EXPECT_EQ(network_flow.flow_label, flow_label) << "flow_label value is not as expected";
}

TEST(test_network_flow, set_internet_address) {
  rmw_network_flow_t network_flow = rmw_get_zero_initialized_network_flow();
  char internet_address[RMW_INET_ADDRSTRLEN];
  strncpy(internet_address, "1898:23ac:b1ae:44df:6170:9f3f:887d:d2a7", RMW_INET_ADDRSTRLEN);
  int size = strlen(internet_address);

  rmw_ret_t ret = rmw_network_flow_set_internet_address(nullptr, internet_address, size);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for null network_flow";
  rmw_reset_error();

  ret = rmw_network_flow_set_internet_address(
    &network_flow, internet_address,
    RMW_INET_ADDRSTRLEN + 1);
  EXPECT_EQ(ret, RMW_RET_INVALID_ARGUMENT) <<
    "Expected invalid argument for size greater than RMW_INET_ADDRSTRLEN";
  rmw_reset_error();

  ret = rmw_network_flow_set_internet_address(&network_flow, internet_address, size);
  EXPECT_EQ(ret, RMW_RET_OK) << "Expected OK for valid arguments";
  int strcmp_ret = strcmp(network_flow.internet_address, internet_address);
  EXPECT_EQ(strcmp_ret, 0) << "internet_address value is not as expected";
}

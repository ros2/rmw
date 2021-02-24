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

#include <string.h>

#include "rmw/error_handling.h"
#include "rmw/network_flow.h"

rmw_network_flow_t
rmw_get_zero_initialized_network_flow(void)
{
  rmw_network_flow_t network_flow = {0};
  return network_flow;
}

rmw_ret_t
rmw_network_flow_set_transport_protocol(
  rmw_network_flow_t * network_flow,
  const rmw_transport_protocol_t transport_protocol)
{
  if (!network_flow) {
    RMW_SET_ERROR_MSG("network_flow is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  network_flow->transport_protocol = transport_protocol;
  return RMW_RET_OK;
}

const char * rmw_network_flow_get_transport_protocol_string(
  const rmw_transport_protocol_t transport_protocol)
{
  const char * transport_protocol_str[RMW_TRANSPORT_PROTOCOL_COUNT] =
  {
    [RMW_TRANSPORT_PROTOCOL_UNKNOWN] = "Unknown",
    [RMW_TRANSPORT_PROTOCOL_UDP] = "UDP",
    [RMW_TRANSPORT_PROTOCOL_TCP] = "TCP",
  };
  if (transport_protocol >= RMW_TRANSPORT_PROTOCOL_COUNT) {
    return transport_protocol_str[RMW_TRANSPORT_PROTOCOL_UNKNOWN];
  }
  return transport_protocol_str[transport_protocol];
}

rmw_ret_t
rmw_network_flow_set_internet_protocol(
  rmw_network_flow_t * network_flow,
  const rmw_internet_protocol_t internet_protocol)
{
  if (!network_flow) {
    RMW_SET_ERROR_MSG("network_flow is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  network_flow->internet_protocol = internet_protocol;
  return RMW_RET_OK;
}

const char * rmw_network_flow_get_internet_protocol_string(
  const rmw_internet_protocol_t internet_protocol)
{
  const char * internet_protocol_str[RMW_INTERNET_PROTOCOL_COUNT] =
  {
    [RMW_INTERNET_PROTOCOL_UNKNOWN] = "Unknown",
    [RMW_INTERNET_PROTOCOL_IPV4] = "IPv4",
    [RMW_INTERNET_PROTOCOL_IPV6] = "IPv6",
  };
  if (internet_protocol >= RMW_INTERNET_PROTOCOL_COUNT) {
    return internet_protocol_str[RMW_INTERNET_PROTOCOL_UNKNOWN];
  }
  return internet_protocol_str[internet_protocol];
}

rmw_ret_t
rmw_network_flow_set_transport_port(
  rmw_network_flow_t * network_flow,
  const uint16_t transport_port)
{
  if (!network_flow) {
    RMW_SET_ERROR_MSG("network_flow is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  network_flow->transport_port = transport_port;
  return RMW_RET_OK;
}

rmw_ret_t
rmw_network_flow_set_flow_label(
  rmw_network_flow_t * network_flow,
  const uint32_t flow_label)
{
  if (!network_flow) {
    RMW_SET_ERROR_MSG("network_flow is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  network_flow->flow_label = flow_label;
  return RMW_RET_OK;
}

rmw_ret_t
rmw_network_flow_set_internet_address(
  rmw_network_flow_t * network_flow,
  const char * internet_address,
  size_t size)
{
  if (!network_flow) {
    RMW_SET_ERROR_MSG("network_flow is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (!internet_address) {
    RMW_SET_ERROR_MSG("internet_address is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (size > RMW_INET_ADDRSTRLEN) {
    RMW_SET_ERROR_MSG("size is greater than RMW_INET_ADDRSTRLEN");
    return RMW_RET_INVALID_ARGUMENT;
  }
  strncpy(network_flow->internet_address, internet_address, size);
  return RMW_RET_OK;
}

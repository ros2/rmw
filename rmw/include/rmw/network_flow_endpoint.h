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

#ifndef RMW__NETWORK_FLOW_H_
#define RMW__NETWORK_FLOW_H_

#if __cplusplus
extern "C"
{
#endif

#include "rmw/types.h"
#include "rmw/visibility_control.h"

/// Transport protocol types
typedef enum rmw_transport_protocol_t
{
  RMW_TRANSPORT_PROTOCOL_UNKNOWN = 0,
  RMW_TRANSPORT_PROTOCOL_UDP,
  RMW_TRANSPORT_PROTOCOL_TCP,
  RMW_TRANSPORT_PROTOCOL_COUNT
} rmw_transport_protocol_t;

/// Internet protocol types
typedef enum rmw_internet_protocol_t
{
  RMW_INTERNET_PROTOCOL_UNKNOWN = 0,
  RMW_INTERNET_PROTOCOL_IPV4,
  RMW_INTERNET_PROTOCOL_IPV6,
  RMW_INTERNET_PROTOCOL_COUNT
} rmw_internet_protocol_t;

/// Maximum length of internet address string
/// Inspired from linux/inet.h
#define RMW_INET_ADDRSTRLEN 48

/// Structure that describes network flow of a publisher or subscriber
// TODO(anamud): Consider renaming structure to network_flow_endpoint_t
typedef struct RMW_PUBLIC_TYPE rmw_network_flow_t
{
  // Transport protocol
  rmw_transport_protocol_t transport_protocol;
  // Internet protocol
  rmw_internet_protocol_t internet_protocol;
  // Port
  uint16_t transport_port;
  // Flow label
  // TODO(anamud): Consider specializing since flow_label is set only at publisher
  // ... side.
  uint32_t flow_label;
  // Internet address
  char internet_address[RMW_INET_ADDRSTRLEN];
} rmw_network_flow_t;

/// Return a rmw_network_flow_t struct with zero-initialized members
RMW_PUBLIC
rmw_network_flow_t
rmw_get_zero_initialized_network_flow(void);

/// Set transport protocol in given rmw_network_flow_t instance
/**
 * \param[in] network_flow network_flow_t to be initialized
 * \param[in] transport_protocol the tranport protocol
 * \returns `RMW_RET_OK` on successfull initilization, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if network_flow is NULL, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 */
RMW_PUBLIC
rmw_ret_t
rmw_network_flow_set_transport_protocol(
  rmw_network_flow_t * network_flow,
  const rmw_transport_protocol_t transport_protocol);

/// Convert to string variant of transport protocol
/**
 * \param[in] transport_protocol transport_protocol_t to be converted
 */
RMW_PUBLIC
const char * rmw_network_flow_get_transport_protocol_string(
  const rmw_transport_protocol_t transport_protocol);

/// Set internet protocol in given rmw_network_flow_t instance
/**
 * \param[in] network_flow network_flow_t to be initialized
 * \param[in] internet_protocol the internet protocolcol
 * \returns `RMW_RET_OK` on successfull initilization, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if network_flow is NULL, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 */
RMW_PUBLIC
rmw_ret_t
rmw_network_flow_set_internet_protocol(
  rmw_network_flow_t * network_flow,
  const rmw_internet_protocol_t internet_protocol);

/// Convert to string variant of internet protocol
/**
 * \param[in] internet_protocol internet_protocol_t to be converted
 */
RMW_PUBLIC
const char * rmw_network_flow_get_internet_protocol_string(
  const rmw_internet_protocol_t internet_protocol);

/// Set transport port in given rmw_network_flow_t instance
/**
 * \param[in] network_flow network_flow_t to be initialized
 * \param[in] transport_port the transport port
 * \returns `RMW_RET_OK` on successfull initilization, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `network_flow` is NULL, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 */
RMW_PUBLIC
rmw_ret_t
rmw_network_flow_set_transport_port(
  rmw_network_flow_t * network_flow,
  const uint16_t transport_port);

/// Set flow label in given rmw_network_flow_t instance
/**
 * \param[in] network_flow network_flow_t to be initialized
 * \param[in] flow_label the flow label
 * \returns `RMW_RET_OK` on successfull initilization, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `network_flow` is NULL, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 */
RMW_PUBLIC
rmw_ret_t
rmw_network_flow_set_flow_label(
  rmw_network_flow_t * network_flow,
  const uint32_t flow_label);

/// Set internet address in the rmw_network_flow_t struct
/**
 * \param[in] network_flow network_flow_t to be initialized
 * \param[in] internet_address the internet address as C-style string
 * \param[in] size size of internet_address
 * \returns `RMW_RET_OK` on successfull initilization, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `internet_address is NULL, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `size` exceeds RMW_internet_ADDRSTRLEN, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 */
RMW_PUBLIC
rmw_ret_t
rmw_network_flow_set_internet_address(
  rmw_network_flow_t * network_flow,
  const char * internet_address,
  size_t size);

#if __cplusplus
}
#endif

#endif  // RMW__NETWORK_FLOW_H_

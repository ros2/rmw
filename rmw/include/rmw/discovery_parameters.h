// Copyright 2022 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__DISCOVERY_PARAMETERS_H_
#define RMW__DISCOVERY_PARAMETERS_H_

#include "rcutils/allocator.h"

#include "rmw/macros.h"
#include "rmw/ret_types.h"
#include "rmw/visibility_control.h"

#ifdef __cplusplus
extern "C"
{
#endif

/// Used to enable or disable the use of multicast for node discovery
typedef enum RMW_PUBLIC_TYPE rmw_automatic_discovery_range_e
{
  /// Uses ROS_LOCALHOST_ONLY environment variable.
  RMW_AUTOMATIC_DISCOVERY_RANGE_DEFAULT = 0,
  /// Forces multicast discovery off
  RMW_AUTOMATIC_DISCOVERY_RANGE_OFF = 1,
  /// Allows multicast but only on the localhost
  RMW_AUTOMATIC_DISCOVERY_RANGE_LOCALHOST = 2,
  /// Allows multicast on the reachable network
  RMW_AUTOMATIC_DISCOVERY_RANGE_SUBNET = 3,
} rmw_automatic_discovery_range_t;

/// Maximum length of a peer hostname or IP address
#define RMW_DISCOVERY_PARAMETERS_PEER_MAX_LENGTH 256

/// Struct to typedef some of the peer addresses
typedef struct rmw_peer_address_s {
  char peer_address[RMW_DISCOVERY_PARAMETERS_PEER_MAX_LENGTH];
} rmw_peer_address_t;

/// Used to specify the parameters that control how discovery is performed
typedef struct RMW_PUBLIC_TYPE rmw_discovery_parameters_s
{
  /// How far to allow multicast to be used
  rmw_automatic_discovery_range_t automatic_discovery_range;

  /// The list of manually-specified peers to perform static discovery with
  /**
   * Each peer is specified as a hostname or an IP address (IPv4 and IPv6 are both acceptable), or
   * a subnet, e.g. 192.168.0.0/24.
   */
  rmw_peer_address_t* static_peers;

  /// The number of manually-specified peers
  size_t static_peers_count;
} rmw_discovery_parameters_t;

/// Return a zero-initialized discovery parameters structure.
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_discovery_parameters_t
rmw_get_zero_initialized_discovery_parameters(void);

/// Compare two discovery parameter instances for equality. Equality means the
/// automatic_discovery_range values are equal, they have the same
/// static_peers_count value, and each entry in static_peers is evaluated as
/// equal using strncmp.
///
/// NOTE: If the two parameter structs list the static peers in different orders
/// then this will evaulate as NOT equal.
///
/// RMW_RET_OK will be returned when the input arguments are valid.
/// RMW_RET_INVALID_ARGUMENT will be returned when any input is a nullptr,
/// or if something in either struct was malformed, such as static_peers being
/// a nullptr while static_peers_count is non-zero.
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_discovery_parameters_equal(
  const rmw_discovery_parameters_t * const left,
  const rmw_discovery_parameters_t * const right,
  bool * result);

/// Perform a deep copy of the discovery parameters from src into dst using the
/// given allocator. The dst will be left with an owned copy of the static peers
/// array whose string values match the src. If successful, src and dst will
/// evaluate as equal using rmw_discovery_parameters_equal.
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_discovery_parameters_copy(
  const rmw_discovery_parameters_t * src,
  const rcutils_allocator_t * allocator,
  rmw_discovery_parameters_t * dst);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_discovery_parameters_fini(
  rmw_discovery_parameters_t * discovery_parameters,
  const rcutils_allocator_t * allocator);
#ifdef __cplusplus
}
#endif

#endif  // RMW__DISCOVERY_PARAMETERS_H_

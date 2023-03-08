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

#ifndef RMW__DISCOVERY_PARAMS_H_
#define RMW__DISCOVERY_PARAMS_H_

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
#define RMW_DISCOVERY_PARAMS_PEER_MAX_LENGTH 256

/// Struct to typedef some of the peer addresses
typedef struct peer_address_s {
  char peer_address[RMW_DISCOVERY_PARAMS_PEER_MAX_LENGTH];
} peer_address_t;

/// Used to specify the parameters that control how discovery is performed
typedef struct RMW_PUBLIC_TYPE rmw_discovery_params_s
{
  /// How far to allow multicast to be used
  rmw_automatic_discovery_range_t automatic_discovery_range;

  /// The list of manually-specified peers to perform static discovery with
  /**
   * Each peer is specified as a hostname or an IP address (IPv4 and IPv6 are both acceptable), or
   * a subnet, e.g. 192.168.0.0/24.
   */
  peer_address_t* static_peers;

  /// The number of manually-specified peers
  size_t static_peers_count;
} rmw_discovery_params_t;

/// Return a zero-initialized discovery parameters structure.
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_discovery_params_t
rmw_get_zero_initialized_discovery_params(void);

/// Compare two discovery parameter instances for equality.
RMW_PUBLIC
RMW_WARN_UNUSED
bool
rmw_discovery_params_equal(
  const rmw_discovery_params_t * const left,
  const rmw_discovery_params_t * const right);

/// Copy a discovery parameter
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_discovery_params_copy(
  const rmw_discovery_params_t * src,
  const rcutils_allocator_t * allocator,
  rmw_discovery_params_t * dst);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_discovery_params_fini(
  rmw_discovery_params_t * discovery_params,
  const rcutils_allocator_t * allocator);
#ifdef __cplusplus
}
#endif

#endif  // RMW__DISCOVERY_PARAMS_H_

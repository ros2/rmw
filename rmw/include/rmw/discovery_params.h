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

#include "rmw/macros.h"
#include "rmw/visibility_control.h"

#ifdef __cplusplus
extern "C"
{
#endif

/// Used to enable or disable the use of multicast for node discovery
typedef enum RMW_PUBLIC_TYPE rmw_multicast_discovery_e
{
  /// Uses ROS_LOCALHOST_ONLY environment variable.
  RMW_MULTICAST_DISCOVERY_DEFAULT = 0,
  /// Forces using only localhost.
  RMW_MULTICAST_DISCOVERY_ENABLED = 1,
  /// Forces disabling localhost only.
  RMW_MULTICAST_DISCOVERY_DISABLED = 2,
} rmw_multicast_discovery_t;

/// Maximum number of peers that can be manually specified
#define RMW_DISCOVERY_PARAMS_MAX_PEERS 32

/// Maximum length of a peer hostname or IP address
#define RMW_DISCOVERY_PARAMS_PEER_MAX_LENGTH 256

/// Used to specify the parameters that control how discovery is performed
typedef struct RMW_PUBLIC_TYPE rmw_discovery_params_s
{
  /// Whether to use multicast discovery or not
  rmw_multicast_discovery_t use_multicast;

  /// The list of manually-specified peers to directly communicate with
  /**
   * Each peer is specified as a hostname or an IP address (IPv4 and IPv6 are both acceptable), or
   * a subnet, e.g. 192.168.0.0/24.
   */
  char peers[RMW_DISCOVERY_PARAMS_MAX_PEERS][RMW_DISCOVERY_PARAMS_PEER_MAX_LENGTH];

  /// The number of manually-specified peer hosts
  size_t peers_count;
} rmw_discovery_params_t;

/// Return a zero-initialized discovery parameters structure.
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_discovery_params_t
rmw_get_zero_initialized_discovery_params(void);

#ifdef __cplusplus
}
#endif

#endif  // RMW__DISCOVERY_PARAMS_H_

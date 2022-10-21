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

#include <stddef.h>
#include <string.h>

#include "rmw/discovery_params.h"

rmw_discovery_params_t
rmw_get_zero_initialized_discovery_params(void)
{
  rmw_discovery_params_t result = (rmw_discovery_params_t) {
    .automatic_discovery_range = RMW_AUTOMATIC_DISCOVERY_RANGE_DEFAULT,
    .static_peers_count = 0,
  };  // NOLINT(readability/braces): false positive
  for (size_t ii = 0; ii < RMW_DISCOVERY_PARAMS_MAX_PEERS; ++ii) {
    result.static_peers[ii][0] = '\0';
  }
  return result;
}

bool
rmw_discovery_params_equal(rmw_discovery_params_t * left, rmw_discovery_params_t * right)
{
  if (left->automatic_discovery_range != right->automatic_discovery_range) {
    return false;
  }

  if (left->static_peers_count != right->static_peers_count) {
    return false;
  }

  for (size_t ii = 0; ii < left->static_peers_count; ++ii) {
    if (strncmp(
        left->static_peers[ii],
        right->static_peers[ii],
        RMW_DISCOVERY_PARAMS_PEER_MAX_LENGTH) != 0)
    {
      return false;
    }
  }

  return true;
}

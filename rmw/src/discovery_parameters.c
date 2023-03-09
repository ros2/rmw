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

#include "rmw/discovery_parameters.h"
#include "rmw/error_handling.h"

rmw_discovery_parameters_t
rmw_get_zero_initialized_discovery_params(void)
{
  rmw_discovery_parameters_t result = (rmw_discovery_parameters_t) {
    .automatic_discovery_range = RMW_AUTOMATIC_DISCOVERY_RANGE_DEFAULT,
    .static_peers_count = 0,
  };  // NOLINT(readability/braces): false positive
  return result;
}

rmw_ret_t
rmw_discovery_params_equal(
  const rmw_discovery_parameters_t * const left,
  const rmw_discovery_parameters_t * const right,
  bool * result)
{
  if (!left || !right || !result) {
    return RMW_RET_INVALID_ARGUMENT;
  }

  if (left->automatic_discovery_range != right->automatic_discovery_range) {
    *result = false;
    return RMW_RET_OK;
  }

  if (left->static_peers_count != right->static_peers_count) {
    *result = false;
    return RMW_RET_OK;
  }

  if (left->static_peers_count == 0) {
    /* No need to examine the static_peers arrays if the count is zero */
    *result = true;
    return RMW_RET_OK;
  }

  if (!left->static_peers || !right->static_peers) {
    return RMW_RET_INVALID_ARGUMENT;
  }

  for (size_t ii = 0; ii < left->static_peers_count; ++ii) {
    if (strncmp(
        left->static_peers[ii].peer_address,
        right->static_peers[ii].peer_address,
        RMW_DISCOVERY_PARAMS_PEER_MAX_LENGTH) != 0)
    {
      *result = false;
      return RMW_RET_OK;
    }
  }

  *result = true;
  return RMW_RET_OK;
}

rmw_ret_t
rmw_discovery_params_copy(
  const rmw_discovery_parameters_t * src,
  const rcutils_allocator_t * allocator,
  rmw_discovery_parameters_t * dst)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(src, RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CHECK_ALLOCATOR(allocator, return RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(dst, RMW_RET_INVALID_ARGUMENT);

  dst->automatic_discovery_range = src->automatic_discovery_range;
  dst->static_peers =
      allocator->zero_allocate(
        src->static_peers_count,
        sizeof(rmw_peer_address_t),
        allocator->state);
  for (size_t i = 0; i < src->static_peers_count; i++)
  {
    strncpy(
      dst->static_peers[i].peer_address,
      src->static_peers[i].peer_address,
      RMW_DISCOVERY_PARAMS_PEER_MAX_LENGTH);
    dst->static_peers[i].peer_address[
        RMW_DISCOVERY_PARAMS_PEER_MAX_LENGTH - 1] = '\0';
  }
  dst->static_peers_count = src->static_peers_count;

  return RMW_RET_OK;
}

rmw_ret_t
rmw_discovery_params_fini(
  rmw_discovery_parameters_t * discovery_params,
  const rcutils_allocator_t * allocator)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(discovery_params, RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CHECK_ALLOCATOR(allocator, return RMW_RET_INVALID_ARGUMENT);

  if (discovery_params->static_peers_count > 0)
    allocator->deallocate(discovery_params->static_peers, allocator->state);
  *discovery_params = rmw_get_zero_initialized_discovery_params();

  return RMW_RET_OK;
}

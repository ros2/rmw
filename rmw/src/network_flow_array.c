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

#include "rmw/error_handling.h"
#include "rmw/network_flow_array.h"

rmw_network_flow_array_t
rmw_get_zero_initialized_network_flow_array(void)
{
  rmw_network_flow_array_t network_flow_array = {0};
  return network_flow_array;
}

rmw_ret_t
rmw_network_flow_array_check_zero(
  rmw_network_flow_array_t * network_flow_array)
{
  if (!network_flow_array) {
    RMW_SET_ERROR_MSG("network_flow_array is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (network_flow_array->size != 0 || network_flow_array->network_flow != NULL) {
    RMW_SET_ERROR_MSG("network_flow_array is not zeroed");
    return RMW_RET_ERROR;
  }
  return RMW_RET_OK;
}

rmw_ret_t
rmw_network_flow_array_init(
  rmw_network_flow_array_t * network_flow_array,
  size_t size,
  rcutils_allocator_t * allocator)
{
  if (!allocator) {
    RMW_SET_ERROR_MSG("allocator is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (!network_flow_array) {
    RMW_SET_ERROR_MSG("network_flow_array is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  network_flow_array->network_flow =
    allocator->allocate(sizeof(rmw_network_flow_t) * size, allocator->state);
  if (!network_flow_array->network_flow) {
    RMW_SET_ERROR_MSG("failed to allocate memory for network_flow_array");
    return RMW_RET_BAD_ALLOC;
  }
  network_flow_array->size = size;
  for (size_t i = 0; i < size; i++) {
    network_flow_array->network_flow[i] = rmw_get_zero_initialized_network_flow();
  }
  return RMW_RET_OK;
}

rmw_ret_t
rmw_network_flow_array_fini(
  rmw_network_flow_array_t * network_flow_array,
  rcutils_allocator_t * allocator)
{
  if (!allocator) {
    RMW_SET_ERROR_MSG("allocator is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  if (!network_flow_array) {
    RMW_SET_ERROR_MSG("network_flow_array is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  allocator->deallocate(network_flow_array->network_flow, allocator->state);
  network_flow_array->network_flow = NULL;
  network_flow_array->size = 0;
  return RMW_RET_OK;
}

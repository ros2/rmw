// Copyright 2025 Minju Lee (이민주).
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

#include "rmw/service_endpoint_info_array.h"
#include "rmw/error_handling.h"
#include "rmw/types.h"

rmw_service_endpoint_info_array_t
rmw_get_zero_initialized_service_endpoint_info_array(void)
{
  // All members are initialized to 0 or NULL by C99 6.7.8/10.
  static const rmw_service_endpoint_info_array_t zero;
  return zero;
}

rmw_ret_t
rmw_service_endpoint_info_array_check_zero(
  rmw_service_endpoint_info_array_t * service_endpoint_info_array)
{
  if (!service_endpoint_info_array) {
    RMW_SET_ERROR_MSG("service_endpoint_info_array is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (service_endpoint_info_array->size != 0 || service_endpoint_info_array->info_array != NULL) {
    RMW_SET_ERROR_MSG("service_endpoint_info_array is not zeroed");
    return RMW_RET_ERROR;
  }
  return RMW_RET_OK;
}

rmw_ret_t
rmw_service_endpoint_info_array_init_with_size(
  rmw_service_endpoint_info_array_t * service_endpoint_info_array,
  size_t size,
  rcutils_allocator_t * allocator)
{
  if (!allocator) {
    RMW_SET_ERROR_MSG("allocator is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (!service_endpoint_info_array) {
    RMW_SET_ERROR_MSG("service_endpoint_info_array is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  service_endpoint_info_array->info_array =
    allocator->allocate(sizeof(*service_endpoint_info_array->info_array) * size, allocator->state);
  if (!service_endpoint_info_array->info_array) {
    RMW_SET_ERROR_MSG("failed to allocate memory for info_array");
    return RMW_RET_BAD_ALLOC;
  }
  service_endpoint_info_array->size = size;
  for (size_t i = 0; i < size; i++) {
    service_endpoint_info_array->info_array[i] = rmw_get_zero_initialized_service_endpoint_info();
  }
  return RMW_RET_OK;
}

rmw_ret_t
rmw_service_endpoint_info_array_fini(
  rmw_service_endpoint_info_array_t * service_endpoint_info_array,
  rcutils_allocator_t * allocator)
{
  if (!allocator) {
    RMW_SET_ERROR_MSG("allocator is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  if (!service_endpoint_info_array) {
    RMW_SET_ERROR_MSG("service_endpoint_info_array is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  rmw_ret_t ret;
  // free all const char * inside the service_endpoint_info_t
  for (size_t i = 0u; i < service_endpoint_info_array->size; i++) {
    ret = rmw_service_endpoint_info_fini(&service_endpoint_info_array->info_array[i], allocator);
    if (ret != RMW_RET_OK) {
      return ret;
    }
  }

  allocator->deallocate(service_endpoint_info_array->info_array, allocator->state);
  service_endpoint_info_array->info_array = NULL;
  service_endpoint_info_array->size = 0;
  return RMW_RET_OK;
}

// Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

#include "rmw/topic_info_array.h"
#include "rmw/error_handling.h"
#include "rmw/types.h"

rmw_topic_info_array_t
rmw_get_zero_initialized_topic_info_array(void)
{
  rmw_topic_info_array_t zero = {0};
  return zero;
}

rmw_ret_t
rmw_topic_info_array_check_zero(rmw_topic_info_array_t * topic_info_array)
{
  if (!topic_info_array) {
    RMW_SET_ERROR_MSG("topic_info_array is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (topic_info_array->count != 0 || topic_info_array->info_array != NULL) {
    RMW_SET_ERROR_MSG("topic_info_array is not zeroed");
    return RMW_RET_ERROR;
  }
  return RMW_RET_OK;
}

rmw_ret_t
rmw_topic_info_array_init_with_size(
  rmw_topic_info_array_t * topic_info_array,
  size_t size,
  rcutils_allocator_t * allocator)
{
  if (!allocator) {
    RMW_SET_ERROR_MSG("allocator is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (!topic_info_array) {
    RMW_SET_ERROR_MSG("topic_info_array is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  topic_info_array->info_array = allocator->allocate(sizeof(*topic_info_array->info_array) * size,
      allocator->state);
  if (!topic_info_array->info_array) {
    RMW_SET_ERROR_MSG("failed to allocate memory for info_array");
    return RMW_RET_BAD_ALLOC;
  }
  return RMW_RET_OK;
}

rmw_ret_t
rmw_topic_info_array_fini(
  rmw_topic_info_array_t * topic_info_array,
  rcutils_allocator_t * allocator)
{
  if (!allocator) {
    RMW_SET_ERROR_MSG("allocator is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  if (!topic_info_array) {
    RMW_SET_ERROR_MSG("topic_info_array is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  rmw_ret_t ret;
  // free all const char * inside the topic_info_t
  for (size_t i = 0u; i < topic_info_array->count; i++) {
    ret = rmw_topic_info_fini_node_name(&topic_info_array->info_array[i], allocator);
    if (ret != RMW_RET_OK) {
      return ret;
    }
    ret = rmw_topic_info_fini_node_namespace(&topic_info_array->info_array[i], allocator);
    if (ret != RMW_RET_OK) {
      return ret;
    }
    ret = rmw_topic_info_fini_topic_type(&topic_info_array->info_array[i], allocator);
    if (ret != RMW_RET_OK) {
      return ret;
    }
  }

  allocator->deallocate(topic_info_array->info_array, allocator->state);
  topic_info_array->info_array = NULL;
  topic_info_array->count = 0;
  return RMW_RET_OK;
}

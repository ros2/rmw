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

#include "rcutils/logging_macros.h"
#include "rmw/error_handling.h"
#include "rmw/convert_rcutils_ret_to_rmw_ret.h"
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
    return RMW_RET_INVALID_ARGUMENT;
  }
  return RMW_RET_OK;
}

rmw_ret_t
rmw_topic_info_array_init_with_size(
  rcutils_allocator_t * allocator,
  size_t size,
  rmw_topic_info_array_t * topic_info_array)
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
  rcutils_allocator_t * allocator,
  rmw_topic_info_array_t * topic_info_array)
{
  if (!allocator) {
    RMW_SET_ERROR_MSG("allocator is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  if (!topic_info_array) {
    RMW_SET_ERROR_MSG("topic_info_array is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  // free all const char * inside the topic_info_t
  for (size_t i = 0u; i < topic_info_array->count; i++) {
    allocator->deallocate((char *) topic_info_array->info_array[i].gid, allocator->state);
    allocator->deallocate((char *) topic_info_array->info_array[i].topic_type, allocator->state);
    allocator->deallocate((char *) topic_info_array->info_array[i].node_name, allocator->state);
    allocator->deallocate((char *) topic_info_array->info_array[i].node_namespace, allocator->state);
  }

  allocator->deallocate(topic_info_array->info_array, allocator->state);
  topic_info_array->info_array = NULL;
  topic_info_array->count = 0;
  return RMW_RET_OK;
}


rmw_ret_t
_rmw_topic_info_copy_str(
  rcutils_allocator_t * allocator,
  const char * str,
  const char ** topic_info_str)
{
  if (!str) {
    RMW_SET_ERROR_MSG("str is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  if (!topic_info_str) {
    RMW_SET_ERROR_MSG("topic_info_str is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  char * temp_str = allocator->allocate(strlen(str) + 1, allocator->state);
  strcpy(temp_str, str);
  *topic_info_str = temp_str;
  return RMW_RET_OK;
}

rmw_ret_t
rmw_topic_info_set_gid(
  rcutils_allocator_t * allocator,
  const char * gid,
  rmw_topic_info_t * topic_info)
{
  if (!topic_info) {
    RMW_SET_ERROR_MSG("topic_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  return _rmw_topic_info_copy_str(allocator, gid, &topic_info->gid);
}

rmw_ret_t
rmw_topic_info_set_topic_type(
  rcutils_allocator_t * allocator,
  const char * topic_type,
  rmw_topic_info_t * topic_info)
{
  if (!topic_info) {
    RMW_SET_ERROR_MSG("topic_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  return _rmw_topic_info_copy_str(allocator, topic_type, &topic_info->topic_type);
}

rmw_ret_t
rmw_topic_info_set_node_name(
  rcutils_allocator_t * allocator,
  const char * node_name,
  rmw_topic_info_t * topic_info)
{
  if (!topic_info) {
    RMW_SET_ERROR_MSG("topic_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  return _rmw_topic_info_copy_str(allocator, node_name, &topic_info->node_name);
}

rmw_ret_t
rmw_topic_info_set_node_namespace(
  rcutils_allocator_t * allocator,
  const char * node_namespace,
  rmw_topic_info_t * topic_info)
{
  if (!topic_info) {
    RMW_SET_ERROR_MSG("topic_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  return _rmw_topic_info_copy_str(allocator, node_namespace, &topic_info->node_namespace);
}

rmw_ret_t
rmw_topic_info_set_qos_profile(
  rmw_qos_profile_t * qos_profile,
  rmw_topic_info_t * topic_info)
{
  if (!topic_info) {
    RMW_SET_ERROR_MSG("topic_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  topic_info->qos_profile = qos_profile;
  return RMW_RET_OK;
}

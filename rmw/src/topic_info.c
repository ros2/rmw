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

#include "rmw/topic_info.h"

#include "rmw/error_handling.h"
#include "rmw/types.h"

rmw_ret_t
_rmw_topic_info_copy_str(
  const char ** topic_info_str,
  const char * str,
  rcutils_allocator_t * allocator)
{
  if (!str) {
    RMW_SET_ERROR_MSG("str is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  if (!topic_info_str) {
    RMW_SET_ERROR_MSG("topic_info_str is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  if (!allocator) {
    RMW_SET_ERROR_MSG("allocator is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  size_t size = strlen(str) + 1;
  char * temp_str = allocator->allocate(size, allocator->state);
  memcpy(temp_str, str, size);
  *topic_info_str = temp_str;
  return RMW_RET_OK;
}

rmw_ret_t
rmw_topic_info_set_topic_type(
  rmw_topic_info_t * topic_info,
  const char * topic_type,
  rcutils_allocator_t * allocator)
{
  if (!topic_info) {
    RMW_SET_ERROR_MSG("topic_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  return _rmw_topic_info_copy_str(&topic_info->topic_type, topic_type, allocator);
}

rmw_ret_t
rmw_topic_info_set_node_name(
  rmw_topic_info_t * topic_info,
  const char * node_name,
  rcutils_allocator_t * allocator)
{
  if (!topic_info) {
    RMW_SET_ERROR_MSG("topic_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  return _rmw_topic_info_copy_str(&topic_info->node_name, node_name, allocator);
}

rmw_ret_t
rmw_topic_info_set_node_namespace(
  rmw_topic_info_t * topic_info,
  const char * node_namespace,
  rcutils_allocator_t * allocator)
{
  if (!topic_info) {
    RMW_SET_ERROR_MSG("topic_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  return _rmw_topic_info_copy_str(&topic_info->node_namespace, node_namespace, allocator);
}

rmw_ret_t
rmw_topic_info_set_qos_profile(
  rmw_topic_info_t * topic_info,
  const rmw_qos_profile_t * qos_profile)
{
  if (!topic_info) {
    RMW_SET_ERROR_MSG("topic_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  if (!qos_profile) {
    RMW_SET_ERROR_MSG("qos_profile is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  topic_info->qos_profile = *qos_profile;
  return RMW_RET_OK;
}

rmw_ret_t
rmw_topic_info_set_gid(
  rmw_topic_info_t * topic_info,
  const uint8_t gid[],
  size_t size)
{
  if (!topic_info) {
    RMW_SET_ERROR_MSG("topic_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (size > RMW_GID_STORAGE_SIZE) {
    RMW_SET_ERROR_MSG("size is more than RMW_GID_STORAGE_SIZE");
    return RMW_RET_INVALID_ARGUMENT;
  }
  memset(&topic_info->gid, 0, RMW_GID_STORAGE_SIZE);
  for (uint i = 0u; i < size; i++) {
    topic_info->gid[i] = gid[i];
  }
  return RMW_RET_OK;
}

rmw_ret_t
_rmw_topic_info_fini_str(
  const char ** topic_info_str,
  rcutils_allocator_t * allocator)
{
  if (!topic_info_str) {
    RMW_SET_ERROR_MSG("topic_info_str is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  if (!allocator) {
    RMW_SET_ERROR_MSG("allocator is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  allocator->deallocate((char *) *topic_info_str, allocator->state);
  return RMW_RET_OK;
}

rmw_ret_t
rmw_topic_info_fini_node_name(
  rmw_topic_info_t * topic_info,
  rcutils_allocator_t * allocator)
{
  if (!topic_info) {
    RMW_SET_ERROR_MSG("topic_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  return _rmw_topic_info_fini_str(&topic_info->node_name, allocator);
}

rmw_ret_t
rmw_topic_info_fini_node_namespace(
  rmw_topic_info_t * topic_info,
  rcutils_allocator_t * allocator)
{
  if (!topic_info) {
    RMW_SET_ERROR_MSG("topic_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  return _rmw_topic_info_fini_str(&topic_info->node_namespace, allocator);
}

rmw_ret_t
rmw_topic_info_fini_topic_type(
  rmw_topic_info_t * topic_info,
  rcutils_allocator_t * allocator)
{
  if (!topic_info) {
    RMW_SET_ERROR_MSG("topic_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  return _rmw_topic_info_fini_str(&topic_info->topic_type, allocator);
}

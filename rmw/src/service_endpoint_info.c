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

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "rcutils/macros.h"
#include "rcutils/strdup.h"

#include "rmw/service_endpoint_info.h"
#include "rmw/error_handling.h"
#include "rmw/types.h"

rmw_service_endpoint_info_t
rmw_get_zero_initialized_service_endpoint_info(void)
{
  // All members are initialized to 0 or NULL by C99 6.7.8/10.
  static const rmw_service_endpoint_info_t zero;
  return zero;
}

rmw_ret_t
_rmw_service_endpoint_info_fini_str(
  const char ** service_endpoint_info_str,
  rcutils_allocator_t * allocator)
{
  allocator->deallocate((char *) *service_endpoint_info_str, allocator->state);
  *service_endpoint_info_str = NULL;
  return RMW_RET_OK;
}

rmw_ret_t
_rmw_service_endpoint_info_fini_node_name(
  rmw_service_endpoint_info_t * service_endpoint_info,
  rcutils_allocator_t * allocator)
{
  return _rmw_service_endpoint_info_fini_str(&service_endpoint_info->node_name, allocator);
}

rmw_ret_t
_rmw_service_endpoint_info_fini_node_namespace(
  rmw_service_endpoint_info_t * service_endpoint_info,
  rcutils_allocator_t * allocator)
{
  return _rmw_service_endpoint_info_fini_str(&service_endpoint_info->node_namespace, allocator);
}

rmw_ret_t
_rmw_service_endpoint_info_fini_service_type(
  rmw_service_endpoint_info_t * service_endpoint_info,
  rcutils_allocator_t * allocator)
{
  return _rmw_service_endpoint_info_fini_str(&service_endpoint_info->service_type, allocator);
}

rmw_ret_t
_rmw_service_endpoint_info_fini_gids(
  rmw_service_endpoint_info_t * service_endpoint_info,
  rcutils_allocator_t * allocator)
{
  allocator->deallocate(service_endpoint_info->endpoint_gids, allocator->state);
  service_endpoint_info->endpoint_gids = NULL;
  return RMW_RET_OK;
}

rmw_ret_t
_rmw_service_endpoint_info_fini_qos_profiles(
  rmw_service_endpoint_info_t * service_endpoint_info,
  rcutils_allocator_t * allocator)
{
  allocator->deallocate(service_endpoint_info->qos_profiles, allocator->state);
  service_endpoint_info->qos_profiles = NULL;
  return RMW_RET_OK;
}

rmw_ret_t
rmw_service_endpoint_info_fini(
  rmw_service_endpoint_info_t * service_endpoint_info,
  rcutils_allocator_t * allocator)
{
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);

  if (!service_endpoint_info) {
    RMW_SET_ERROR_MSG("service_endpoint_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (!allocator) {
    RMW_SET_ERROR_MSG("allocator is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  rmw_ret_t ret;
  ret = _rmw_service_endpoint_info_fini_node_name(service_endpoint_info, allocator);
  if (ret != RMW_RET_OK) {
    return ret;
  }
  ret = _rmw_service_endpoint_info_fini_node_namespace(service_endpoint_info, allocator);
  if (ret != RMW_RET_OK) {
    return ret;
  }
  ret = _rmw_service_endpoint_info_fini_service_type(service_endpoint_info, allocator);
  if (ret != RMW_RET_OK) {
    return ret;
  }
  ret = _rmw_service_endpoint_info_fini_gids(service_endpoint_info, allocator);
  if (ret != RMW_RET_OK) {
    return ret;
  }
  ret = _rmw_service_endpoint_info_fini_qos_profiles(service_endpoint_info, allocator);
  if (ret != RMW_RET_OK) {
    return ret;
  }

  *service_endpoint_info = rmw_get_zero_initialized_service_endpoint_info();

  return RMW_RET_OK;
}

rmw_ret_t
_rmw_service_endpoint_info_copy_str(
  const char ** service_endpoint_info_str,
  const char * str,
  rcutils_allocator_t * allocator)
{
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_BAD_ALLOC);

  if (!str) {
    RMW_SET_ERROR_MSG("str is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  if (!service_endpoint_info_str) {
    RMW_SET_ERROR_MSG("service_endpoint_info_str is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  if (!allocator) {
    RMW_SET_ERROR_MSG("allocator is null");
    return RMW_RET_INVALID_ARGUMENT;
  }

  *service_endpoint_info_str = rcutils_strdup(str, *allocator);
  if (NULL == *service_endpoint_info_str) {
    return RMW_RET_BAD_ALLOC;
  }

  return RMW_RET_OK;
}

rmw_ret_t
rmw_service_endpoint_info_set_service_type(
  rmw_service_endpoint_info_t * service_endpoint_info,
  const char * service_type,
  rcutils_allocator_t * allocator)
{
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);

  if (!service_endpoint_info) {
    RMW_SET_ERROR_MSG("service_endpoint_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  return _rmw_service_endpoint_info_copy_str(
    &service_endpoint_info->service_type, service_type, allocator);
}

rmw_ret_t
rmw_service_endpoint_info_set_service_type_hash(
  rmw_service_endpoint_info_t * service_endpoint_info,
  const rosidl_type_hash_t * type_hash)
{
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CHECK_ARGUMENT_FOR_NULL(service_endpoint_info, RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CHECK_ARGUMENT_FOR_NULL(type_hash, RMW_RET_INVALID_ARGUMENT);
  service_endpoint_info->service_type_hash = *type_hash;
  return RMW_RET_OK;
}

rmw_ret_t
rmw_service_endpoint_info_set_node_name(
  rmw_service_endpoint_info_t * service_endpoint_info,
  const char * node_name,
  rcutils_allocator_t * allocator)
{
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);

  if (!service_endpoint_info) {
    RMW_SET_ERROR_MSG("service_endpoint_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  return _rmw_service_endpoint_info_copy_str(
    &service_endpoint_info->node_name, node_name, allocator);
}

rmw_ret_t
rmw_service_endpoint_info_set_node_namespace(
  rmw_service_endpoint_info_t * service_endpoint_info,
  const char * node_namespace,
  rcutils_allocator_t * allocator)
{
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);

  if (!service_endpoint_info) {
    RMW_SET_ERROR_MSG("service_endpoint_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  return _rmw_service_endpoint_info_copy_str(
    &service_endpoint_info->node_namespace,
    node_namespace,
    allocator);
}

rmw_ret_t
rmw_service_endpoint_info_set_endpoint_type(
  rmw_service_endpoint_info_t * service_endpoint_info,
  rmw_endpoint_type_t type)
{
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);

  if (!service_endpoint_info) {
    RMW_SET_ERROR_MSG("service_endpoint_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  service_endpoint_info->endpoint_type = type;
  return RMW_RET_OK;
}

rmw_ret_t
rmw_service_endpoint_info_set_endpoint_count(
  rmw_service_endpoint_info_t * service_endpoint_info,
  size_t endpoint_count)
{
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);

  if (!service_endpoint_info) {
    RMW_SET_ERROR_MSG("service_endpoint_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (!(endpoint_count == 1 || endpoint_count == 2)) {
    RMW_SET_ERROR_MSG("endpoint_count must be either 1 or 2");
    return RMW_RET_INVALID_ARGUMENT;
  }
  service_endpoint_info->endpoint_count = endpoint_count;
  return RMW_RET_OK;
}

rmw_ret_t
rmw_service_endpoint_info_set_gids(
  rmw_service_endpoint_info_t * service_endpoint_info,
  const uint8_t * gids,
  size_t endpoint_count,
  size_t size,
  rcutils_allocator_t * allocator)
{
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);

  if (!service_endpoint_info) {
    RMW_SET_ERROR_MSG("service_endpoint_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (size > RMW_GID_STORAGE_SIZE) {
    RMW_SET_ERROR_MSG("size is more than RMW_GID_STORAGE_SIZE");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (!(endpoint_count == 1 || endpoint_count == 2)) {
    RMW_SET_ERROR_MSG("endpoint_count must be either 1 or 2");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (!allocator) {
    RMW_SET_ERROR_MSG("allocator is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  service_endpoint_info->endpoint_gids = \
    (uint8_t (*)[RMW_GID_STORAGE_SIZE]) allocator->allocate(
      endpoint_count * RMW_GID_STORAGE_SIZE, allocator->state);

  if (!service_endpoint_info->endpoint_gids) {
    RMW_SET_ERROR_MSG("failed to allocate memory for endpoint_gids");
    return RMW_RET_BAD_ALLOC;
  }
  memset(service_endpoint_info->endpoint_gids, 0, RMW_GID_STORAGE_SIZE * endpoint_count);
  memcpy(service_endpoint_info->endpoint_gids, gids, RMW_GID_STORAGE_SIZE * endpoint_count);

  return RMW_RET_OK;
}

rmw_ret_t
rmw_service_endpoint_info_set_qos_profiles(
  rmw_service_endpoint_info_t * service_endpoint_info,
  const rmw_qos_profile_t * qos_profiles,
  size_t endpoint_count,
  rcutils_allocator_t * allocator)
{
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);

  if (!service_endpoint_info) {
    RMW_SET_ERROR_MSG("service_endpoint_info is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (!qos_profiles) {
    RMW_SET_ERROR_MSG("qos_profile is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (!(endpoint_count == 1 || endpoint_count == 2)) {
    RMW_SET_ERROR_MSG("endpoint_count must be either 1 or 2");
    return RMW_RET_INVALID_ARGUMENT;
  }
  service_endpoint_info->qos_profiles = (rmw_qos_profile_t *) allocator->allocate(
    endpoint_count * sizeof(rmw_qos_profile_t), allocator->state);
  if (!service_endpoint_info->qos_profiles) {
    RMW_SET_ERROR_MSG("failed to allocate memory for qos_profiles");
    return RMW_RET_BAD_ALLOC;
  }
  memcpy(service_endpoint_info->qos_profiles, qos_profiles,
    endpoint_count * sizeof(rmw_qos_profile_t));
  return RMW_RET_OK;
}

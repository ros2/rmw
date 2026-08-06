// Copyright 2017 Open Source Robotics Foundation, Inc.
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

#include "rmw/names_and_types.h"

#include "rcutils/logging_macros.h"
#include "rcutils/macros.h"
#include "rcutils/types/string_array.h"
#include "rmw/error_handling.h"
#include "rmw/convert_rcutils_ret_to_rmw_ret.h"
#include "rmw/types.h"

rmw_names_and_types_t
rmw_get_zero_initialized_names_and_types(void)
{
  // All members are initialized to 0 or NULL by C99 6.7.8/10.
  static const rmw_names_and_types_t zero;
  return zero;
}

rmw_ret_t
rmw_names_and_types_check_zero(rmw_names_and_types_t * names_and_types)
{
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);

  if (!names_and_types) {
    RMW_SET_ERROR_MSG("names_and_types is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (names_and_types->names.data != 0 || names_and_types->names.size != 0) {
    RMW_SET_ERROR_MSG("names array is not zeroed");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (names_and_types->types != 0) {
    RMW_SET_ERROR_MSG("types array is not NULL");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (names_and_types->type_hashes != 0) {
    RMW_SET_ERROR_MSG("type_hashes array is not NULL");
    return RMW_RET_INVALID_ARGUMENT;
  }
  return RMW_RET_OK;
}

rmw_ret_t
rmw_names_and_types_init(
  rmw_names_and_types_t * names_and_types,
  size_t size,
  rcutils_allocator_t * allocator)
{
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CAN_RETURN_WITH_ERROR_OF(RMW_RET_BAD_ALLOC);

  if (!rcutils_allocator_is_valid(allocator)) {
    RMW_SET_ERROR_MSG("allocator is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (!names_and_types) {
    RMW_SET_ERROR_MSG("names_and_types is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (rmw_names_and_types_check_zero(names_and_types) != RMW_RET_OK) {
    // rmw_names_and_types_check_zero already sets the error message
    return RMW_RET_INVALID_ARGUMENT;
  }
  rcutils_ret_t rcutils_ret = rcutils_string_array_init(&names_and_types->names, size, allocator);
  if (rcutils_ret != RCUTILS_RET_OK) {
    RMW_SET_ERROR_MSG(rcutils_get_error_string().str);
    // rcutils_string_array_init may leave the array in a partial state on failure;
    // restore the zero-initialized post-condition so the caller can re-init.
    names_and_types->names = rcutils_get_zero_initialized_string_array();
    return rmw_convert_rcutils_ret_to_rmw_ret(rcutils_ret);
  }
  names_and_types->types =
    allocator->zero_allocate(size, sizeof(rcutils_string_array_t), allocator->state);
  if (!names_and_types->types && size != 0) {
    rcutils_ret = rcutils_string_array_fini(&names_and_types->names);
    if (rcutils_ret != RCUTILS_RET_OK) {
      RCUTILS_LOG_ERROR("error while reporting error: %s", rcutils_get_error_string().str);
    }
    RMW_SET_ERROR_MSG("failed to allocate memory for types");
    return RMW_RET_BAD_ALLOC;
  }
  names_and_types->type_hashes =
    allocator->zero_allocate(size, sizeof(rosidl_type_hash_t *), allocator->state);
  if (!names_and_types->type_hashes && size != 0) {
    allocator->deallocate(names_and_types->types, allocator->state);
    names_and_types->types = NULL;
    rcutils_ret = rcutils_string_array_fini(&names_and_types->names);
    if (rcutils_ret != RCUTILS_RET_OK) {
      RCUTILS_LOG_ERROR("error while reporting error: %s", rcutils_get_error_string().str);
    }
    RMW_SET_ERROR_MSG("failed to allocate type_hashes array");
    return RMW_RET_BAD_ALLOC;
  }
  return RMW_RET_OK;
}

rmw_ret_t
rmw_names_and_types_fini(rmw_names_and_types_t * names_and_types)
{
  if (!names_and_types) {
    RMW_SET_ERROR_MSG("names_and_types is null");
    return RMW_RET_INVALID_ARGUMENT;
  }
  rcutils_ret_t rcutils_ret;
  if (names_and_types->types || names_and_types->type_hashes || names_and_types->names.data) {
    RCUTILS_CHECK_ALLOCATOR_WITH_MSG(
      &names_and_types->names.allocator,
      "allocator is invalid",
      return RMW_RET_INVALID_ARGUMENT);
  }
  if (names_and_types->types) {
    // Cleanup string arrays for types first
    size_t i;
    for (i = 0; i < names_and_types->names.size; ++i) {
      rcutils_ret = rcutils_string_array_fini(&names_and_types->types[i]);
      if (rcutils_ret != RCUTILS_RET_OK) {
        RMW_SET_ERROR_MSG(rcutils_get_error_string().str);
        return rmw_convert_rcutils_ret_to_rmw_ret(rcutils_ret);
      }
    }
    // Use the allocator in the names string array
    // (prevents this data structure from having to also store it)
    names_and_types->names.allocator.deallocate(
      names_and_types->types, names_and_types->names.allocator.state);
    names_and_types->types = NULL;
  }

  // Cleanup type hashes array
  if (names_and_types->type_hashes) {
    size_t i;
    for (i = 0; i < names_and_types->names.size; ++i) {
      if (names_and_types->type_hashes[i]) {
        names_and_types->names.allocator.deallocate(
          names_and_types->type_hashes[i], names_and_types->names.allocator.state);
        names_and_types->type_hashes[i] = NULL;
      }
    }
    names_and_types->names.allocator.deallocate(
      names_and_types->type_hashes, names_and_types->names.allocator.state);
    names_and_types->type_hashes = NULL;
  }
  // Cleanup names string array.
  // Safe on a zero-initialized names array; rcutils_string_array_fini is a
  // no-op when data is NULL and does not touch the embedded allocator.
  rcutils_ret = rcutils_string_array_fini(&names_and_types->names);
  if (rcutils_ret != RCUTILS_RET_OK) {
    RMW_SET_ERROR_MSG(rcutils_get_error_string().str);
    return rmw_convert_rcutils_ret_to_rmw_ret(rcutils_ret);
  }
  return RMW_RET_OK;
}

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

#ifdef __cplusplus
extern "C"
{
#endif

#include <rcutils/allocator.h>
#include <rcutils/logging_macros.h>
#include <rosidl_runtime_c/message_type_support_struct.h>
#include <rosidl_runtime_c/type_description/type_description__struct.h>
#include <rosidl_runtime_c/type_description/type_description__functions.h>

#include "rmw/convert_rcutils_ret_to_rmw_ret.h"
#include "rmw/dynamic_message_typesupport.h"
#include "rmw/error_handling.h"


const char * rmw_dynamic_typesupport_c__identifier = "rmw_dynamic_typesupport_c";

// NOTE(methylDragon): How do we test this? It depends on specific serialization support. Do I just
//                     use the FastRTPS support then?

/// Create a rosidl_message_type_support_t from a TypeDescription message
rmw_ret_t
rmw_dynamic_message_typesupport_handle_init(
  rosidl_dynamic_typesupport_serialization_support_t * serialization_support,
  bool middleware_supports_type_discovery,
  const rosidl_runtime_c__type_description__TypeDescription * description,
  rosidl_message_type_support_t ** ts)
{
  if (!middleware_supports_type_discovery && description == NULL) {
    RMW_SET_ERROR_MSG(
      "Middleware does not support type discovery. Deferred dynamic type message type support will "
      "never be populated. You must provide a type description.");
    return RMW_RET_INVALID_ARGUMENT;
  }
  // TODO(methylDragon): Remove if and when the deferred description path is supported
  if (description == NULL) {
    RMW_SET_ERROR_MSG(
      "Deferred type description is not currently supported. You must provide a type description.");
    return RMW_RET_INVALID_ARGUMENT;
  }
  RMW_CHECK_ARGUMENT_FOR_NULL(serialization_support, RMW_RET_INVALID_ARGUMENT);

  rmw_ret_t ret = RMW_RET_ERROR;
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  *ts = allocator.zero_allocate(1, sizeof(rosidl_message_type_support_t), allocator.state);
  if (!*ts) {
    RMW_SET_ERROR_MSG("Could not allocate rosidl_message_type_support_t struct");
    return RMW_RET_BAD_ALLOC;
  }

  (*ts)->typesupport_identifier = rmw_dynamic_typesupport_c__identifier;

  (*ts)->data = allocator.zero_allocate(
    1, sizeof(rmw_dynamic_message_typesupport_impl_t), allocator.state);
  if (!ts) {
    RMW_SET_ERROR_MSG("Could not allocate rmw_dynamic_message_typesupport_impl_t struct");
    ret = RMW_RET_BAD_ALLOC;
    goto fail;
  }

  rmw_dynamic_message_typesupport_impl_t * ts_impl =
    (rmw_dynamic_message_typesupport_impl_t *) (*ts)->data;

  ts_impl->serialization_support = serialization_support;
  (*ts)->func = get_message_typesupport_handle_function;

  if (description) {
    ts_impl->description = rosidl_runtime_c__type_description__TypeDescription__create();
    if (ts_impl->description == NULL) {
      RMW_SET_ERROR_MSG("Could not create type description to assign into");
      ret = RMW_RET_BAD_ALLOC;
      goto fail;
    }

    if (!rosidl_runtime_c__type_description__TypeDescription__copy(
        description, ts_impl->description))
    {
      RMW_SET_ERROR_MSG("Could not copy type description");
      ret = RMW_RET_ERROR;
      goto fail;
    }
  }

  ret = rmw_convert_rcutils_ret_to_rmw_ret(
    rmw_init_dynamic_message_type_from_description(
      ts_impl->serialization_support, description, &ts_impl->dynamic_message_type));
  if (ret != RMW_RET_OK || !ts_impl->dynamic_message_type) {
    RMW_SET_ERROR_MSG(
      "Could not construct dynamic type for rmw_dynamic_message_typesupport_impl_t struct");
    goto fail;
  }

  ret = rmw_init_dynamic_message_from_dynamic_message_type(
    ts_impl->dynamic_message_type, &ts_impl->dynamic_message);
  if (ret != RMW_RET_OK || !ts_impl->dynamic_message) {
    RMW_SET_ERROR_MSG(
      "Could not construct dynamic data for rmw_dynamic_message_typesupport_impl_t struct");
    goto fail;
  }

  return RMW_RET_OK;

fail:
  if (rmw_dynamic_message_typesupport_handle_fini(*ts) != RMW_RET_OK) {
    RCUTILS_LOG_ERROR_NAMED(
      rmw_dynamic_typesupport_c__identifier,
      "Could not finalize dynamic type typesupport");
  }
  return ret;
}

rmw_ret_t
rmw_dynamic_message_typesupport_handle_fini(rosidl_message_type_support_t * ts)
{
  RCUTILS_CHECK_ARGUMENT_FOR_NULL(ts, RMW_RET_INVALID_ARGUMENT);

  // NOTE(methylDragon): Ignores const...
  if (ts->typesupport_identifier != rmw_dynamic_typesupport_c__identifier) {
    RCUTILS_SET_ERROR_MSG("type support not from this implementation");
    return RMW_RET_INVALID_ARGUMENT;
  }

  if (ts->data) {
    rmw_dynamic_message_typesupport_impl_t * ts_impl =
      (rmw_dynamic_message_typesupport_impl_t *)ts->data;
    if (ts_impl->description) {
      rosidl_runtime_c__type_description__TypeDescription__fini(ts_impl->description);
    }
    if (ts_impl->dynamic_message_type) {
      rosidl_dynamic_typesupport_dynamic_type_fini(ts_impl->dynamic_message_type);
    }
    if (ts_impl->dynamic_message) {
      rosidl_dynamic_typesupport_dynamic_data_fini(ts_impl->dynamic_message);
    }
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  allocator.deallocate((void *)ts->data, allocator.state);
  allocator.deallocate((void *)ts->type_hash, allocator.state);
  allocator.deallocate(ts, allocator.state);
  return RMW_RET_OK;
}


rmw_ret_t
rmw_init_dynamic_message_type_from_description(
  rosidl_dynamic_typesupport_serialization_support_t * serialization_support,
  const rosidl_runtime_c__type_description__TypeDescription * description,
  rosidl_dynamic_typesupport_dynamic_type_t ** ts)
{
  return rmw_convert_rcutils_ret_to_rmw_ret(
    rosidl_dynamic_typesupport_dynamic_type_init_from_description(
      serialization_support, description, ts));
}


rmw_ret_t
rmw_init_dynamic_message_from_dynamic_message_type(
  rosidl_dynamic_typesupport_dynamic_type_t * dynamic_message_type,
  rosidl_dynamic_typesupport_dynamic_data_t ** cloned_dynamic_message_type)
{
  return rmw_convert_rcutils_ret_to_rmw_ret(
    rosidl_dynamic_typesupport_dynamic_data_init_from_dynamic_type(
      dynamic_message_type, cloned_dynamic_message_type));
}


rmw_ret_t
rmw_clone_dynamic_message(
  const rosidl_dynamic_typesupport_dynamic_data_t * dynamic_message,
  rosidl_dynamic_typesupport_dynamic_data_t ** cloned_dynamic_message)
{
  return rmw_convert_rcutils_ret_to_rmw_ret(
    rosidl_dynamic_typesupport_dynamic_data_clone(dynamic_message, cloned_dynamic_message));
}


rmw_ret_t
rmw_serialized_to_dynamic_message(
  rmw_serialized_message_t * serialized_message,
  rosidl_dynamic_typesupport_dynamic_data_t * dynamic_message)
{
  bool success;
  rmw_ret_t ret = rmw_convert_rcutils_ret_to_rmw_ret(
    rosidl_dynamic_typesupport_dynamic_data_deserialize(
      dynamic_message, serialized_message, &success));

  if (success) {
    return RMW_RET_OK;
  } else {
    RMW_SET_ERROR_MSG(
      "could not deserialize serialized message to dynamic data: "
      "dynamic data not enough memory");
    return ret;
  }
}


rmw_ret_t
rmw_dynamic_message_to_serialized(
  rosidl_dynamic_typesupport_dynamic_data_t * dynamic_message,
  rmw_serialized_message_t * serialized_message)
{
  bool success;
  rmw_ret_t ret = rmw_convert_rcutils_ret_to_rmw_ret(
    rosidl_dynamic_typesupport_dynamic_data_serialize(
      dynamic_message, serialized_message, &success));

  if (success) {
    return RMW_RET_OK;
  } else {
    RMW_SET_ERROR_MSG("could not serialize dynamic data to serialized message!");
    return ret;
  }
}


#ifdef __cplusplus
}
#endif

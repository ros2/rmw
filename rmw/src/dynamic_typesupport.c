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

#include "rmw/dynamic_typesupport.h"

#include <rcutils/logging_macros.h>
#include <rosidl_runtime_c/message_type_support_struct.h>
#include <rosidl_runtime_c/type_description/type_description__struct.h>
#include <rosidl_runtime_c/type_description/type_description__functions.h>


const char * rmw_dynamic_typesupport_c__identifier =
  "rmw_dynamic_typesupport_c";

// NOTE(methylDragon): How do we test this? It depends on specific serialization support. Do I just
//                     use the FastRTPS support then?

/// Create a rosidl_message_type_support_t from a TypeDescription message
rosidl_message_type_support_t *
rmw_get_dynamic_message_typesupport_handle(
  rosidl_dynamic_typesupport_serialization_support_t * serialization_support,
  bool middleware_supports_type_discovery,
  bool middleware_can_take_dynamic_data,
  rosidl_runtime_c__type_description__TypeDescription * description)
{
  if (!middleware_supports_type_discovery && description == NULL) {
    RCUTILS_LOG_ERROR_NAMED(rmw_dynamic_typesupport_c__identifier,
                            "Middleware does not support type discovery! Deferred dynamic type"
                            "message type support will never be populated! You must provide a type "
                            "description!");
    return NULL;
  }

  // TODO(methylDragon): Do I need to use an allocator...?
  rosidl_message_type_support_t * ts =
    (rosidl_message_type_support_t *)(calloc(1, sizeof(rosidl_message_type_support_t)));
  if(!ts) {
    RCUTILS_LOG_ERROR_NAMED(rmw_dynamic_typesupport_c__identifier,
                            "Could not allocate rosidl_message_type_support_t struct");
    return NULL;
  }

  ts->typesupport_identifier = rmw_dynamic_typesupport_c__identifier;

  // NOTE(methylDragon): To populate dynamic_type and description if deferred, OUTSIDE
  ts->data = calloc(1, sizeof(rmw_dynamic_typesupport_impl_t));
  if(!ts) {
    RCUTILS_LOG_ERROR_NAMED(rmw_dynamic_typesupport_c__identifier,
                            "Could not allocate rmw_dynamic_typesupport_impl_t struct");
    goto fail;
  }

  rmw_dynamic_typesupport_impl_t * ts_impl = (rmw_dynamic_typesupport_impl_t *)ts->data;

  ts_impl->take_dynamic_data = middleware_can_take_dynamic_data;
  ts_impl->serialization_support = serialization_support;
  ts->func = get_message_typesupport_handle_function;

  if (description) {
    ts_impl->description = rosidl_runtime_c__type_description__TypeDescription__create();
    if (ts_impl->description == NULL) {
      RCUTILS_LOG_ERROR_NAMED(rmw_dynamic_typesupport_c__identifier,
                              "Could not create type description to assign into");
      goto fail;
    }

    if (!rosidl_runtime_c__type_description__TypeDescription__copy(
        description, ts_impl->description))
    {
      RCUTILS_LOG_ERROR_NAMED(rmw_dynamic_typesupport_c__identifier,
                              "Could not copy type description");
      goto fail;
    }
  }

  ts_impl->dynamic_type = rmw_init_dynamic_type_from_description(ts_impl->serialization_support, description);
  if (!ts_impl->dynamic_type) {
    RCUTILS_LOG_ERROR_NAMED(rmw_dynamic_typesupport_c__identifier,
                            "Could not construct dynamic type for rmw_dynamic_typesupport_impl_t struct");
    goto fail;
  }

  ts_impl->dynamic_data =
    rmw_init_dynamic_data_from_dynamic_type(ts_impl->dynamic_type);
  if (!ts_impl->dynamic_data) {
    RCUTILS_LOG_ERROR_NAMED(rmw_dynamic_typesupport_c__identifier,
                            "Could not construct dynamic data for rmw_dynamic_typesupport_impl_t struct");
    goto fail;
  }

  return ts;

fail:
  if (rmw_dynamic_message_typesupport_handle_fini(ts) != RMW_RET_OK) {
    RCUTILS_LOG_ERROR_NAMED(rmw_dynamic_typesupport_c__identifier,
                            "Could not finalize dynamic type typesupport");
  }
  return NULL;
}

rmw_ret_t
rmw_dynamic_message_typesupport_handle_fini(rosidl_message_type_support_t * ts)
{
  RCUTILS_CHECK_ARGUMENT_FOR_NULL(ts, RMW_RET_INVALID_ARGUMENT);

  // NOTE(methylDragon): Ignores const...
  if (ts->typesupport_identifier != rmw_dynamic_typesupport_c__identifier)
  {
    RCUTILS_SET_ERROR_MSG("type support not from this implementation");
    return RMW_RET_INVALID_ARGUMENT;
  }

  if (ts->data) {
    rmw_dynamic_typesupport_impl_t * ts_impl = (rmw_dynamic_typesupport_impl_t *)ts->data;

    if (ts_impl->description) {
      rosidl_runtime_c__type_description__TypeDescription__fini(ts_impl->description);
    }

    if (ts_impl->dynamic_type) {
      rosidl_dynamic_typesupport_dynamic_type_fini(ts_impl->dynamic_type);
    }

    if (ts_impl->dynamic_data) {
      rosidl_dynamic_typesupport_dynamic_data_fini(ts_impl->dynamic_data);
    }
  }
  free(ts);

  return RMW_RET_OK;
}


rosidl_dynamic_typesupport_dynamic_type_t *
rmw_init_dynamic_type_from_description(rosidl_dynamic_typesupport_serialization_support_t * serialization_support, rosidl_runtime_c__type_description__TypeDescription * description)
{
  return rosidl_dynamic_typesupport_dynamic_type_init_from_description(serialization_support, description);
}


rosidl_dynamic_typesupport_dynamic_data_t *
rmw_init_dynamic_data_from_dynamic_type(rosidl_dynamic_typesupport_dynamic_type_t * dynamic_type)
{
  return rosidl_dynamic_typesupport_dynamic_data_init_from_dynamic_type(dynamic_type);
}


rosidl_dynamic_typesupport_dynamic_data_t *
rmw_clone_dynamic_data(const rosidl_dynamic_typesupport_dynamic_data_t * dynamic_data)
{
  return rosidl_dynamic_typesupport_dynamic_data_clone(dynamic_data);
}


rmw_ret_t
rmw_serialized_to_dynamic_data(
  rmw_serialized_message_t * serialized_message,
  rosidl_dynamic_typesupport_dynamic_data_t * dynamic_data)
{
  if (rosidl_dynamic_typesupport_dynamic_data_deserialize(dynamic_data, serialized_message)) {
    return RMW_RET_OK;
  } else {
    RCUTILS_LOG_ERROR_NAMED(rmw_dynamic_typesupport_c__identifier,
                            "could not deserialize serialized message to dynamic data: "
                            "dynamic data not enough memory");
    return RMW_RET_ERROR;
  }
}


rmw_ret_t
rmw_dynamic_data_to_serialized(
  rosidl_dynamic_typesupport_dynamic_data_t * dynamic_data,
  rmw_serialized_message_t * serialized_message)
{
  if (rosidl_dynamic_typesupport_dynamic_data_serialize(dynamic_data, serialized_message)) {
    return RMW_RET_OK;
  } else {
    RCUTILS_LOG_ERROR_NAMED(rmw_dynamic_typesupport_c__identifier,
                            "could not serialize dynamic data to serialized message!");
    return RMW_RET_ERROR;
  }
}


#ifdef __cplusplus
}
#endif

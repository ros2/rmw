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

#include "rcutils/logging_macros.h"
#include "serialization_support_lib/description.h"
#include "rosidl_runtime_c/message_type_support_struct.h"

#include "rmw/runtime_type.h"


const char * rmw_typesupport_runtime_type_introspection_c__identifier =
  "rmw_typesupport_runtime_type_introspection_c";

// NOTE(methylDragon): My use of the serialization_support_lib::type_description_t struct is for
//                     convenience only. We should be passing the TypeDescription message

// NOTE(methylDragon): How do we test this? It depends on specific serialization support. Do I just
//                     use the FastRTPS support then?

/// Create a rosidl_message_type_support_t from a TypeDescription message
rosidl_message_type_support_t *
rmw_get_runtime_type_message_typesupport_handle(
  serialization_support_t * ser_support,
  bool middleware_supports_type_discovery,
  bool middleware_can_take_dynamic_data,
  type_description_t * desc)
{
  if (!middleware_supports_type_discovery && desc == NULL) {
    RCUTILS_LOG_ERROR_NAMED(rmw_typesupport_runtime_type_introspection_c__identifier,
                            "Middleware does not support type discovery! Deferred runtime type"
                            "message type support will never be populated! You must provide a type "
                            "description!");
    return NULL;
  }

  // TODO(methylDragon): Do I need to use an allocator...?
  rosidl_message_type_support_t * ts =
    (rosidl_message_type_support_t *)(calloc(1, sizeof(rosidl_message_type_support_t)));
  if(!ts) {
    RCUTILS_LOG_ERROR_NAMED(rmw_typesupport_runtime_type_introspection_c__identifier,
                            "Could not allocate rosidl_message_type_support_t struct");
    return NULL;
  }

  ts->typesupport_identifier = rmw_typesupport_runtime_type_introspection_c__identifier;

  // NOTE(methylDragon): To populate dynamic_type and description if deferred, OUTSIDE
  ts->data = calloc(1, sizeof(runtime_type_ts_impl_t));
  if(!ts) {
    RCUTILS_LOG_ERROR_NAMED(rmw_typesupport_runtime_type_introspection_c__identifier,
                            "Could not allocate runtime_type_ts_impl_t struct");
    if (rmw_runtime_type_message_typesupport_handle_fini(ts) != RMW_RET_OK) {
      RCUTILS_LOG_ERROR_NAMED(rmw_typesupport_runtime_type_introspection_c__identifier,
                              "Could not finalize runtime type typesupport");
    }
    return NULL;
  }

  runtime_type_ts_impl_t * ts_impl = (runtime_type_ts_impl_t *)ts->data;

  ts_impl->take_dynamic_data = middleware_can_take_dynamic_data;
  ts_impl->ser = ser_support;
  ts->func = get_message_typesupport_handle_function;

  // TODO(methylDragon): This is just temporary. We'd ideally pass in the TypeDescription msg
  //                     The type_description_t object doesn't support default values
  ts_impl->desc = desc;

  ts_impl->dynamic_type = rmw_get_dynamic_type_from_description(ts_impl->ser, desc);
  if (!ts_impl->dynamic_type) {
    RCUTILS_LOG_ERROR_NAMED(rmw_typesupport_runtime_type_introspection_c__identifier,
                            "Could not construct dynamic type for runtime_type_ts_impl_t struct");
    if (rmw_runtime_type_message_typesupport_handle_fini(ts) != RMW_RET_OK) {
      RCUTILS_LOG_ERROR_NAMED(rmw_typesupport_runtime_type_introspection_c__identifier,
                              "Could not finalize runtime type typesupport");
    }
    return NULL;
  }

  ts_impl->dynamic_data =
    rmw_get_dynamic_data_from_dynamic_type(ts_impl->ser, ts_impl->dynamic_type);
  if (!ts_impl->dynamic_data) {
    RCUTILS_LOG_ERROR_NAMED(rmw_typesupport_runtime_type_introspection_c__identifier,
                            "Could not construct dynamic data for runtime_type_ts_impl_t struct");
    if (rmw_runtime_type_message_typesupport_handle_fini(ts) != RMW_RET_OK) {
      RCUTILS_LOG_ERROR_NAMED(rmw_typesupport_runtime_type_introspection_c__identifier,
                              "Could not finalize runtime type typesupport");
    }
    return NULL;
  }

  return ts;
}

rmw_ret_t
rmw_runtime_type_message_typesupport_handle_fini(rosidl_message_type_support_t * ts)
{
  RCUTILS_CHECK_ARGUMENT_FOR_NULL(ts, RMW_RET_INVALID_ARGUMENT);

  // NOTE(methylDragon): Ignores const...
  if (ts->typesupport_identifier != rmw_typesupport_runtime_type_introspection_c__identifier)
  {
    RCUTILS_SET_ERROR_MSG("type support not from this implementation");
    return RMW_RET_INVALID_ARGUMENT;
  }

  if (ts->data) {
    runtime_type_ts_impl_t * ts_impl = (runtime_type_ts_impl_t *)ts->data;

    if (ts_impl->desc) {
      type_description_fini(ts_impl->desc);
    }

    if (ts_impl->dynamic_type) {
      ser_type_fini(ts_impl->ser, ts_impl->dynamic_type);
    }

    if (ts_impl->dynamic_data) {
      ser_data_fini(ts_impl->ser, ts_impl->dynamic_data);
    }
  }
  free(ts);

  return RMW_RET_OK;
}


ser_dynamic_type_t *
rmw_get_dynamic_type_from_description(serialization_support_t * ser, type_description_t * desc)
{
  return ser_construct_type_from_description(ser, desc);
}


ser_dynamic_data_t *
rmw_get_dynamic_data_from_dynamic_type(serialization_support_t * ser, ser_dynamic_type_t * type)
{
  return ser_data_init_from_type(ser, type);
}


#ifdef __cplusplus
}
#endif

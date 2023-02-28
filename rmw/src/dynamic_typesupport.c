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
#include "rosidl_dynamic_typesupport/description.h"
#include "rosidl_runtime_c/message_type_support_struct.h"

#include "rmw/dynamic_typesupport.h"


const char * rmw_dynamic_typesupport_c__identifier =
  "rmw_dynamic_typesupport_c";

// NOTE(methylDragon): My use of the rosidl_dynamic_typesupport::type_description_t struct is for
//                     convenience only. We should be passing the TypeDescription message

// NOTE(methylDragon): How do we test this? It depends on specific serialization support. Do I just
//                     use the FastRTPS support then?

/// Create a rosidl_message_type_support_t from a TypeDescription message
rosidl_message_type_support_t *
rmw_get_dynamic_message_typesupport_handle(
  rosidl_dynamic_typesupport_serialization_support_t * serialization_support,
  bool middleware_supports_type_discovery,
  bool middleware_can_take_dynamic_data,
  type_description_t * desc)
{
  if (!middleware_supports_type_discovery && desc == NULL) {
    RCUTILS_LOG_ERROR_NAMED(rmw_dynamic_typesupport_c__identifier,
                            "Middleware does not support type discovery! Deferred runtime type"
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
    if (rmw_dynamic_message_typesupport_handle_fini(ts) != RMW_RET_OK) {
      RCUTILS_LOG_ERROR_NAMED(rmw_dynamic_typesupport_c__identifier,
                              "Could not finalize runtime type typesupport");
    }
    return NULL;
  }

  rmw_dynamic_typesupport_impl_t * ts_impl = (rmw_dynamic_typesupport_impl_t *)ts->data;

  ts_impl->take_dynamic_data = middleware_can_take_dynamic_data;
  ts_impl->serialization_support = serialization_support;
  ts->func = get_message_typesupport_handle_function;

  // TODO(methylDragon): This is just temporary. We'd ideally pass in the TypeDescription msg
  //                     The type_description_t object doesn't support default values
  ts_impl->desc = desc;

  ts_impl->dynamic_type = rmw_init_dynamic_type_from_description(ts_impl->serialization_support, desc);
  if (!ts_impl->dynamic_type) {
    RCUTILS_LOG_ERROR_NAMED(rmw_dynamic_typesupport_c__identifier,
                            "Could not construct dynamic type for rmw_dynamic_typesupport_impl_t struct");
    if (rmw_dynamic_message_typesupport_handle_fini(ts) != RMW_RET_OK) {
      RCUTILS_LOG_ERROR_NAMED(rmw_dynamic_typesupport_c__identifier,
                              "Could not finalize runtime type typesupport");
    }
    return NULL;
  }

  ts_impl->dynamic_data =
    rmw_init_dynamic_data_from_dynamic_type(ts_impl->dynamic_type);
  if (!ts_impl->dynamic_data) {
    RCUTILS_LOG_ERROR_NAMED(rmw_dynamic_typesupport_c__identifier,
                            "Could not construct dynamic data for rmw_dynamic_typesupport_impl_t struct");
    if (rmw_dynamic_message_typesupport_handle_fini(ts) != RMW_RET_OK) {
      RCUTILS_LOG_ERROR_NAMED(rmw_dynamic_typesupport_c__identifier,
                              "Could not finalize runtime type typesupport");
    }
    return NULL;
  }

  return ts;
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

    if (ts_impl->desc) {
      type_description_fini(ts_impl->desc);
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
rmw_init_dynamic_type_from_description(rosidl_dynamic_typesupport_serialization_support_t * serialization_support, type_description_t * desc)
{
  return rosidl_dynamic_typesupport_dynamic_type_init_from_description(serialization_support, desc);
}


rosidl_dynamic_typesupport_dynamic_data_t *
rmw_init_dynamic_data_from_dynamic_type(rosidl_dynamic_typesupport_dynamic_type_t * type)
{
  return rosidl_dynamic_typesupport_dynamic_data_init_from_dynamic_type(type);
}


rosidl_dynamic_typesupport_dynamic_data_t *
rmw_clone_dynamic_data(const rosidl_dynamic_typesupport_dynamic_data_t * data)
{
  return rosidl_dynamic_typesupport_dynamic_data_clone(data);
}


#ifdef __cplusplus
}
#endif

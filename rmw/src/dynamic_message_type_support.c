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
#include <rosidl_dynamic_typesupport/dynamic_message_type_support_struct.h>
#include <rosidl_dynamic_typesupport/identifier.h>
#include <rosidl_runtime_c/message_type_support_struct.h>
#include <rosidl_runtime_c/type_description/type_description__struct.h>
#include <rosidl_runtime_c/type_description/type_description__functions.h>
#include <rosidl_runtime_c/type_description/type_source__functions.h>

#include "rmw/convert_rcutils_ret_to_rmw_ret.h"
#include "rmw/dynamic_message_type_support.h"
#include "rmw/error_handling.h"
#include "rmw/visibility_control.h"

// NOTE(methylDragon): How do we test this? It depends on specific serialization support. Do I just
//                     use the FastRTPS support then?

/// Create a rosidl_message_type_support_t from a TypeDescription message
rmw_ret_t
rmw_dynamic_message_type_support_handle_init(
  rosidl_dynamic_typesupport_serialization_support_t * serialization_support,
  bool middleware_supports_type_discovery,
  const rosidl_type_hash_t * type_hash,
  const rosidl_runtime_c__type_description__TypeDescription * type_description,
  const rosidl_runtime_c__type_description__TypeSource__Sequence * type_description_sources,
  rosidl_message_type_support_t ** ts)
{
  if (!middleware_supports_type_discovery && type_description == NULL) {
    RMW_SET_ERROR_MSG(
      "Middleware does not support type discovery. Deferred dynamic type message type support will "
      "never be populated. You must provide a type description.");
    return RMW_RET_INVALID_ARGUMENT;
  }
  // TODO(methylDragon): Remove if and when the deferred description path is supported
  if (type_description == NULL) {
    RMW_SET_ERROR_MSG(
      "Deferred type description is not currently supported. You must provide a type description.");
    return RMW_RET_INVALID_ARGUMENT;
  }

  RMW_CHECK_ARGUMENT_FOR_NULL(serialization_support, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(type_hash, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(type_description, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(ts, RMW_RET_INVALID_ARGUMENT);

  // NOTE(methylDragon): Not supported for now
  // RMW_CHECK_ARGUMENT_FOR_NULL(type_description_sources, RMW_RET_INVALID_ARGUMENT);

  return rmw_convert_rcutils_ret_to_rmw_ret(
    rosidl_dynamic_message_type_support_handle_init(
      serialization_support, type_hash, type_description, type_description_sources, ts));
}

rmw_ret_t
rmw_dynamic_message_type_support_handle_destroy(rosidl_message_type_support_t * ts)
{
  RCUTILS_CHECK_ARGUMENT_FOR_NULL(ts, RMW_RET_INVALID_ARGUMENT);
  return rmw_convert_rcutils_ret_to_rmw_ret(rosidl_dynamic_message_type_support_handle_destroy(ts));
}

const char *
rmw_get_dynamic_typesupport_identifier(void)
{
  return rosidl_dynamic_typesupport_c__identifier;
}

rmw_ret_t
rmw_init_dynamic_message_type_from_description(
  rosidl_dynamic_typesupport_serialization_support_t * serialization_support,
  const rosidl_runtime_c__type_description__TypeDescription * type_description,
  rosidl_dynamic_typesupport_dynamic_type_t ** ts)
{
  return rmw_convert_rcutils_ret_to_rmw_ret(
    rosidl_dynamic_typesupport_dynamic_type_init_from_description(
      serialization_support, type_description, ts));
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

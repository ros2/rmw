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

#ifndef RMW__RUNTIME_TYPE_H_
#define RMW__RUNTIME_TYPE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "rmw/features.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"

#include "serialization_support_lib/types.h"
#include "serialization_support_lib/api/serialization_support.h"
#include "rosidl_runtime_c/message_type_support_struct.h"

/// Interfaces for runtime interface reflection

// RUNTIME INTERFACE REFLECTION TYPE SUPPORT =======================================================
/// String identifying the typesupport introspection implementation in use.
// NOTE(methylDragon): I don't know if this is supposed to be in an identifier.h file
RMW_PUBLIC
extern const char * rmw_typesupport_runtime_type_introspection_c__identifier;


// This struct is meant to be populated fully if the rosidl_message_type_support_t is constructed
// with a given description on the client library end (since the descrpition is obtained via
// services).
//
// Otherwise, it is deferred MUST be populated on type discovery, if the middleware supports it
typedef struct runtime_type_ts_impl_s {
  bool take_dynamic_data;  // Take dynamic data at the middleware level
                           // Get from middleware specific link-time function:
                           // rmw_middleware_can_take_dynamic_data()

  const char * topic_name; // MAYBE???
  type_description_t * desc;  // Might be unused if dynamic_type is obtained directly
  serialization_support_t * ser;
  ser_dynamic_type_t * dynamic_type;
  ser_dynamic_data_t * dynamic_data;
} runtime_type_ts_impl_t;

// TODO(methylDragon): !!! Document that the user is in charge of the lifetime of the struct...
// TODO(methylDragon): ... How do I do this? ^
// NOTE(methylDragon): My use of the serialization_support_lib::type_description_t struct is for
//                     convenience only. We should be passing the TypeDescription message

/// Get runtime type message typesupport with bound message description
/// If the user passes a NULL desc, it is deferred instead, the middleware is responsibile for
/// populating the fields on type discovery!!!
RMW_PUBLIC
RMW_WARN_UNUSED
rosidl_message_type_support_t *
rmw_get_runtime_type_message_typesupport_handle(
  serialization_support_t * ser_support,
  bool middleware_supports_type_discovery,
  bool middleware_can_take_dynamic_data,
  type_description_t * desc);

/// Finalize a rosidl_message_type_support_t obtained with
/// rmw_get_runtime_type_message_typesupport_handle
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_runtime_type_message_typesupport_handle_fini(rosidl_message_type_support_t * type_support);

/// Construct serialization support-specific ser_dynamic_type_t from a given type description
RMW_PUBLIC
RMW_WARN_UNUSED
ser_dynamic_type_t *
rmw_get_dynamic_type_from_description(serialization_support_t * ser, type_description_t * desc);

RMW_PUBLIC
RMW_WARN_UNUSED
ser_dynamic_data_t *
rmw_get_dynamic_data_from_dynamic_type(serialization_support_t * ser, ser_dynamic_type_t * type);


// INTERFACES FOR RMW IMPLEMENTATIONS TO FULFILL ===================================================
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_take_dynamic_data_message_with_info(
  const rmw_subscription_t * subscription,
  ser_dynamic_data_t * dynamic_data,
  bool * taken,
  rmw_message_info_t * message_info,
  rmw_subscription_allocation_t * allocation);

RMW_PUBLIC
RMW_WARN_UNUSED
serialization_support_t *
rmw_get_serialization_support(const char * serialization_lib_name);

// NOTE(methylDragon): The responsibility is on the user to ensure that the dynamic data's
//                     dynamic type matches the layout of the buffer
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_serialized_to_dynamic_data(
  rmw_serialized_message_t * serialized_message, ser_dynamic_data_t * dyn_data);

// TODO(methylDragon): Nice to have only
// RMW_PUBLIC
// RMW_WARN_UNUSED
// rmw_ret_t
// rmw_get_dynamic_type_from_middleware(
//   const rmw_node_t * node,
//   const char * topic_name,
//   const rosidl_message_type_support_t type_support,
//   ser_dynamic_type_t * dynamic_type);
//
// RMW_PUBLIC
// RMW_WARN_UNUSED
// rmw_ret_t
// rmw_get_dynamic_data_from_middleware(
//   const rmw_node_t * node,
//   const char * topic_name,
//   const rosidl_message_type_support_t type_support,
//   ser_dynamic_data_t * dynamic_data);


#ifdef __cplusplus
}
#endif

#endif  // RMW__RUNTIME_TYPE_H_

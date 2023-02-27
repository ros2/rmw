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

#ifndef RMW__DYNAMIC_TYPESUPPORT_H_
#define RMW__DYNAMIC_TYPESUPPORT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "rmw/features.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"

#include "rosidl_dynamic_typesupport/types.h"
#include "rosidl_dynamic_typesupport/api/serialization_support.h"
#include "rosidl_runtime_c/message_type_support_struct.h"

/// Interfaces for runtime interface reflection

// RUNTIME INTERFACE REFLECTION TYPE SUPPORT =======================================================
/// String identifying the typesupport introspection implementation in use.
// NOTE(methylDragon): I don't know if this is supposed to be in an identifier.h file
RMW_PUBLIC
extern const char * rmw_dynamic_typesupport_c__identifier;


// This struct is meant to be populated fully if the rosidl_message_type_support_t is constructed
// with a given description on the client library end (since the descrpition is obtained via
// services).
//
// Otherwise, it is deferred MUST be populated on type discovery, if the middleware supports it
typedef struct rmw_dynamic_typesupport_impl_s {
  bool take_dynamic_data;  // Take dynamic data at the middleware level
                           // Get from middleware specific link-time function:
                           // rmw_middleware_can_take_dynamic_data()

  const char * topic_name; // MAYBE???
  type_description_t * desc;  // Might be unused if dynamic_type is obtained directly
  rosidl_dynamic_typesupport_serialization_support_t * serialization_support;

  // NOTE(methylDragon): I'm unsure if these are necessary. Though I think they are convenient.
  //                     dynamic_type moreso than dynamic_data.
  //
  //                     I'd err on including them to be able to support more middlewares
  //
  //                     The dynamic_type allows us to do a one time alloc and reuse it for
  //                     subscription creation and data creation
  //                     The dynamic_data allows us to either reuse it, or clone it, but it's
  //                     technically redundant because data can be created from dynamic_type
  rosidl_dynamic_typesupport_dynamic_type_t * dynamic_type;
  rosidl_dynamic_typesupport_dynamic_data_t * dynamic_data;
} rmw_dynamic_typesupport_impl_t;

// TODO(methylDragon): !!! Document that the user is in charge of the lifetime of the struct...
// TODO(methylDragon): ... How do I do this? ^
// NOTE(methylDragon): My use of the rosidl_dynamic_typesupport::type_description_t struct is for
//                     convenience only. We should be passing the TypeDescription message

/// Get runtime type message typesupport with bound message description
/// If the user passes a NULL desc, it is deferred instead, the middleware is responsibile for
/// populating the fields on type discovery!!!
RMW_PUBLIC
RMW_WARN_UNUSED
rosidl_message_type_support_t *
rmw_get_dynamic_message_typesupport_handle(
  rosidl_dynamic_typesupport_serialization_support_t * serialization_support,
  bool middleware_supports_type_discovery,
  bool middleware_can_take_dynamic_data,
  type_description_t * desc);

/// Finalize a rosidl_message_type_support_t obtained with
/// rmw_get_dynamic_message_typesupport_handle
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_dynamic_message_typesupport_handle_fini(rosidl_message_type_support_t * type_support);

/// Construct serialization support-specific rosidl_dynamic_typesupport_dynamic_type_t from a given type description
RMW_PUBLIC
RMW_WARN_UNUSED
rosidl_dynamic_typesupport_dynamic_type_t *
rmw_init_dynamic_type_from_description(rosidl_dynamic_typesupport_serialization_support_t * serialization_support, type_description_t * desc);

RMW_PUBLIC
RMW_WARN_UNUSED
rosidl_dynamic_typesupport_dynamic_data_t *
rmw_init_dynamic_data_from_dynamic_type(rosidl_dynamic_typesupport_serialization_support_t * serialization_support, rosidl_dynamic_typesupport_dynamic_type_t * type);

RMW_PUBLIC
RMW_WARN_UNUSED
rosidl_dynamic_typesupport_dynamic_data_t *
rmw_clone_dynamic_data(rosidl_dynamic_typesupport_serialization_support_t * serialization_support, const rosidl_dynamic_typesupport_dynamic_data_t * data);


// INTERFACES FOR RMW IMPLEMENTATIONS TO FULFILL ===================================================
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_take_dynamic_message_with_info(
  const rmw_subscription_t * subscription,
  // TODO(methylDragon): To replace with rmw_dynamic_message_t *
  rosidl_dynamic_typesupport_dynamic_data_t * dynamic_data,
  bool * taken,
  rmw_message_info_t * message_info,
  rmw_subscription_allocation_t * allocation);

RMW_PUBLIC
RMW_WARN_UNUSED
rosidl_dynamic_typesupport_serialization_support_t *
rmw_get_serialization_support(const char * serialization_lib_name);

// NOTE(methylDragon): The responsibility is on the user to ensure that the dynamic data's
//                     dynamic type matches the layout of the buffer
/// The user must provide a rosidl_dynamic_typesupport_dynamic_data_t with dynamic data impl
/// that matches the serialization library used. It must also match the layout of the buffer if the
/// serialization library cannot infer the layout from the buffer alone.
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
// TODO(methylDragon): Convert to rmw_serialized_message_to_dynamic_message
//                     Or maybe just an additional interface?
rmw_serialized_to_dynamic_data(
  rmw_serialized_message_t * serialized_message, rosidl_dynamic_typesupport_dynamic_data_t * dyn_data);

// TODO(methylDragon): Nice to have only
// RMW_PUBLIC
// RMW_WARN_UNUSED
// rmw_ret_t
// rmw_get_dynamic_type_from_middleware(
//   const rmw_node_t * node,
//   const char * topic_name,
//   const rosidl_message_type_support_t type_support,
//   rosidl_dynamic_typesupport_dynamic_type_t * dynamic_type);
//
// RMW_PUBLIC
// RMW_WARN_UNUSED
// rmw_ret_t
// rmw_get_dynamic_data_from_middleware(
//   const rmw_node_t * node,
//   const char * topic_name,
//   const rosidl_message_type_support_t type_support,
//   rosidl_dynamic_typesupport_dynamic_data_t * dynamic_data);


#ifdef __cplusplus
}
#endif

#endif  // RMW__DYNAMIC_TYPESUPPORT_H_

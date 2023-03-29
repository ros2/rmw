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

#ifndef RMW__DYNAMIC_MESSAGE_TYPESUPPORT_H_
#define RMW__DYNAMIC_MESSAGE_TYPESUPPORT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "rmw/features.h"
#include "rmw/serialized_message.h"
#include "rmw/visibility_control.h"

#include <rosidl_dynamic_typesupport/types.h>
#include <rosidl_dynamic_typesupport/api/serialization_support.h>
#include <rosidl_runtime_c/message_type_support_struct.h>
#include <rosidl_runtime_c/type_description/type_description__struct.h>


/// Interfaces for runtime interface reflection

// RUNTIME INTERFACE REFLECTION TYPE SUPPORT =======================================================
/// String identifying the typesupport introspection implementation in use.
// NOTE(methylDragon): I don't know if this is supposed to be in an identifier.h file
RMW_PUBLIC
extern const char * rmw_dynamic_typesupport_c__identifier;

// Every field of this struct is expected to be populated.
//
// NOTE(methylDragon): There is an opportunity to defer the population of the members by waiting
//                     for discovery, but this path is currently not supported.
//
// Ownership:
//   - The struct owns its `description` field. It is responsible for deallocating it.
//   - The struct owns its `serialization_support` field. It is responsible for deallocating it.
//   - The struct owns its `dynamic_message_type` field. It is responsible for deallocating it.
//   - The struct owns its `dynamic_message` field. It is responsible for deallocating it.
//
// Downstream classes are expected to borrow the `serialization_support` field, and potentially the
// `dynamic_message_type` and `dynamic_message` fields. As such, it is important that this struct
// outlives those downstream classes.
typedef struct rmw_dynamic_message_typesupport_impl_s {
  rosidl_runtime_c__type_description__TypeDescription * description;
  rosidl_dynamic_typesupport_serialization_support_t * serialization_support;

  // NOTE(methylDragon): I'm unsure if these are necessary. Though I think they are convenient.
  //                     dynamic_message_type moreso than dynamic_message.
  //
  //                     I'd err on including them to be able to support more middlewares
  //
  //                     The dynamic_message_type allows us to do a one time alloc and reuse it for
  //                     subscription creation and data creation
  //                     The dynamic_message allows us to either reuse it, or clone it, but it's
  //                     technically redundant because data can be created from dynamic_message_type
  rosidl_dynamic_typesupport_dynamic_type_t * dynamic_message_type;
  rosidl_dynamic_typesupport_dynamic_data_t * dynamic_message;
} rmw_dynamic_message_typesupport_impl_t;

/// Get dynamic type message typesupport with bound message description
/**
 * NOTE: Take note of the ownership rules for the returned struct and the `description` argument!
 *
 * If the user passes a NULL description, it is deferred instead, the middleware is responsibile
 * for populating the fields on type discovery!!!
 *
 * Ownership:
 *   - The `rosidl_message_type_support_t *` returned from this function has different ownership
 *     rules compared to the statically allocated `rosidl_message_type_support_t` structs from
 *     code-generated types!
 *    - The caller is responsible for deallocating the returned pointer
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | No
 * Uses Atomics       | No
 * Lock-Free          | Yes
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rosidl_message_type_support_t *
rmw_dynamic_message_typesupport_handle_init(
  rosidl_dynamic_typesupport_serialization_support_t * serialization_support,
  bool middleware_supports_type_discovery,
  const rosidl_runtime_c__type_description__TypeDescription * description);

/// Finalize a rosidl_message_type_support_t obtained with
/// rmw_dynamic_message_typesupport_handle_init, which has dynamically allocated members
///
/// NOTE: Using this on a statically allocated typesupport will cause undefined behavior!
///       (Static memory will get freed in that case.)
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_dynamic_message_typesupport_handle_fini(rosidl_message_type_support_t * type_support);

/// Construct serialization support-specific rosidl_dynamic_typesupport_dynamic_type_t from a given type description
RMW_PUBLIC
RMW_WARN_UNUSED
rosidl_dynamic_typesupport_dynamic_type_t *
rmw_init_dynamic_message_type_from_description(
  rosidl_dynamic_typesupport_serialization_support_t * serialization_support,
  const rosidl_runtime_c__type_description__TypeDescription * description);

RMW_PUBLIC
RMW_WARN_UNUSED
rosidl_dynamic_typesupport_dynamic_data_t *
rmw_init_dynamic_message_from_dynamic_message_type(rosidl_dynamic_typesupport_dynamic_type_t * dynamic_message_type);

RMW_PUBLIC
RMW_WARN_UNUSED
rosidl_dynamic_typesupport_dynamic_data_t *
rmw_clone_dynamic_message(const rosidl_dynamic_typesupport_dynamic_data_t * dynamic_message);

// NOTE(methylDragon): The responsibility is on the user to ensure that the dynamic message's
//                     dynamic type matches the layout of the buffer
/// The user must provide a rosidl_dynamic_typesupport_dynamic_data_t with dynamic data impl
/// that matches the serialization library used to serialize the buffer. It must also match the
/// layout of the buffer if the serialization library cannot infer the layout from the buffer alone.
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_serialized_to_dynamic_message(
  rmw_serialized_message_t * serialized_message,
  rosidl_dynamic_typesupport_dynamic_data_t * dynamic_message);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_dynamic_message_to_serialized(
  rosidl_dynamic_typesupport_dynamic_data_t * dynamic_message,
  rmw_serialized_message_t * serialized_message);

// INTERFACES FOR RMW IMPLEMENTATIONS TO FULFILL ===================================================
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_take_dynamic_message(
  const rmw_subscription_t * subscription,
  rosidl_dynamic_typesupport_dynamic_data_t * dynamic_message,
  bool * taken,
  rmw_subscription_allocation_t * allocation);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_take_dynamic_message_with_info(
  const rmw_subscription_t * subscription,
  rosidl_dynamic_typesupport_dynamic_data_t * dynamic_message,
  bool * taken,
  rmw_message_info_t * message_info,
  rmw_subscription_allocation_t * allocation);

RMW_PUBLIC
RMW_WARN_UNUSED
rosidl_dynamic_typesupport_serialization_support_t *
rmw_get_serialization_support(const char * serialization_lib_name);

// TODO(methylDragon): Nice to have only
// RMW_PUBLIC
// RMW_WARN_UNUSED
// rmw_ret_t
// rmw_get_dynamic_type_from_middleware(
//   const rmw_node_t * node,
//   const char * topic_name,
//   const rosidl_message_type_support_t type_support,
//   rosidl_dynamic_typesupport_dynamic_type_t * dynamic_message_type);
//
// RMW_PUBLIC
// RMW_WARN_UNUSED
// rmw_ret_t
// rmw_get_dynamic_data_from_middleware(
//   const rmw_node_t * node,
//   const char * topic_name,
//   const rosidl_message_type_support_t type_support,
//   rosidl_dynamic_typesupport_dynamic_data_t * dynamic_message);


#ifdef __cplusplus
}
#endif

#endif  // RMW__DYNAMIC_MESSAGE_TYPESUPPORT_H_
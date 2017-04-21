// Copyright 2014-2017 Open Source Robotics Foundation, Inc.
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

/** \mainpage rmw: ROS Middleware Abstraction Interface
 *
 * `rmw` defines an interface of middleware primitives that are used by the higher level ROS API's.
 * It consists of these main components:
 *
 * - Nodes
 *   - rmw/rmw.h
 * - Publisher
 *   - rmw/rmw.h
 * - Subscription
 *   - rmw/rmw.h
 * - Service Client
 *   - rmw/rmw.h
 * - Service Server
 *   - rmw/rmw.h
 *
 * It also has some machinery that is necessary to wait on and act on these concepts:
 *
 * - Initialization and shutdown management (global for now)
 *   - rmw/rmw.h
 * - Wait sets for waiting on messages and service requests / responses to be ready
 *   - rmw/rmw.h
 * - Guard conditions for waking up wait sets asynchronously
 *   - rmw/rmw.h
 *
 * Further still there are some useful abstractions and utilities:
 *
 * - Allocator functions for various types
 *   - rmw/allocators.h
 * - Error handling functionality (C style)
 *   - rmw/error_handling.h
 * - Macros
 *   - rmw/macros.h
 * - Return code types and other utility types
 *   - rmw/types.h
 * - Sanity checks for some of the types
 *   - rmw/sanity_checks.h
 * - Macros for controlling symbol visibility on the library
 *   - rmw/visibility_control.h
 * - Utility function to demangle a type to a human readable string (C++ specific):
 *   - rmw/impl/cpp/demangle.hpp
 */

#ifndef RMW__RMW_H_
#define RMW__RMW_H_

#if __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "rcutils/types.h"

#include "rosidl_generator_c/message_type_support_struct.h"
#include "rosidl_generator_c/service_type_support.h"

#include "rmw/macros.h"
#include "rmw/qos_profiles.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"

RMW_PUBLIC
RMW_WARN_UNUSED
const char *
rmw_get_implementation_identifier(void);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_init(void);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_node_t *
rmw_create_node(
  const char * name,
  const char * namespace_,
  size_t domain_id,
  const char * security_root_path);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_destroy_node(rmw_node_t * node);

/// Return a guard condition which is triggered when the ROS graph changes.
/**
 * The handle returned is a pointer to an internally held rmw guard condition.
 * This function can fail, and therefore return `NULL`, if:
 *   - node is `NULL`
 *   - node is invalid
 *
 * The returned handle is made invalid if the node is destroyed or if
 * rmw_shutdown() is called.
 *
 * The guard condition will be triggered anytime a change to the ROS graph
 * occurs.
 * A ROS graph change includes things like (but not limited to) a new publisher
 * advertises, a new subscription is created, a new service becomes available,
 * a subscription is canceled, etc.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | Yes
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * \param[in] node pointer to the rmw node
 * \return rmw guard condition handle if successful, otherwise `NULL`
 *
 */
RMW_PUBLIC
RMW_WARN_UNUSED
const rmw_guard_condition_t *
rmw_node_get_graph_guard_condition(const rmw_node_t * node);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_publisher_t *
rmw_create_publisher(
  const rmw_node_t * node,
  const rosidl_message_type_support_t * type_support,
  const char * topic_name,
  const rmw_qos_profile_t * qos_policies);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_destroy_publisher(rmw_node_t * node, rmw_publisher_t * publisher);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_publish(const rmw_publisher_t * publisher, const void * ros_message);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_subscription_t *
rmw_create_subscription(
  const rmw_node_t * node,
  const rosidl_message_type_support_t * type_support,
  const char * topic_name,
  const rmw_qos_profile_t * qos_policies,
  bool ignore_local_publications);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_destroy_subscription(rmw_node_t * node, rmw_subscription_t * subscription);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_take(const rmw_subscription_t * subscription, void * ros_message, bool * taken);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_take_with_info(
  const rmw_subscription_t * subscription,
  void * ros_message,
  bool * taken,
  rmw_message_info_t * message_info);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_client_t *
rmw_create_client(
  const rmw_node_t * node,
  const rosidl_service_type_support_t * type_support,
  const char * service_name,
  const rmw_qos_profile_t * qos_policies);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_destroy_client(rmw_node_t * node, rmw_client_t * client);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_send_request(
  const rmw_client_t * client,
  const void * ros_request,
  int64_t * sequence_id);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_take_response(
  const rmw_client_t * client,
  rmw_request_id_t * request_header,
  void * ros_response,
  bool * taken);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_service_t *
rmw_create_service(
  const rmw_node_t * node,
  const rosidl_service_type_support_t * type_support,
  const char * service_name,
  const rmw_qos_profile_t * qos_policies);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_destroy_service(rmw_node_t * node, rmw_service_t * service);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_take_request(
  const rmw_service_t * service,
  rmw_request_id_t * request_header,
  void * ros_request,
  bool * taken);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_send_response(
  const rmw_service_t * service,
  rmw_request_id_t * request_header,
  void * ros_response);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_guard_condition_t *
rmw_create_guard_condition(void);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_destroy_guard_condition(rmw_guard_condition_t * guard_condition);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_trigger_guard_condition(const rmw_guard_condition_t * guard_condition);

/// Create a waitset to store conditions that the middleware will block on.
/**
 * If `max_conditions` is `0`, the waitset can store an unbounded number of
 * conditions to wait on.
 * If `max_conditions` is greater than `0`, the number of conditions that can
 * be attached to the waitset is bounded at `max_conditions`.
 * \param[in] max_conditions
 *   The maximum number of conditions that can be attached to the waitset.
 * \return A pointer to the created waitset, `NULL` if an error occurred.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_waitset_t *
rmw_create_waitset(size_t max_conditions);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_destroy_waitset(rmw_waitset_t * waitset);

/// Waits on sets of different waitable entities and returns when one is ready.
/**
 * Add conditions to the wait set and wait until a response comes in, or until
 * the timeout is reached.
 * The arrays contain type-erased representations of waitable entities.
 * This function casts the pointers to middleware-specific conditions and adds
 * them to the waitset.
 *
 * The count variables in the arrays represents the number of valid pointers
 * in the array.
 * `NULL` pointers are in the array considered invalid.
 * If they are encountered, an error is returned.
 *
 * The array structs are allocated and deallocated outside of this function.
 * They do not have any information about how much memory is allocated for the
 * arrays.
 *
 * After the wait wakes up, the entries in each array that correspond to
 * conditions that were not triggered are set to `NULL`.
 *
 * \param subscriptions Array of subscriptions to wait on
 * \param guard_conditions Array of guard conditions to wait on
 * \param services Array of services to wait on
 * \param clients Array of clients to wait on
 * \param waitset Storage for the waitset
 * \param wait_timeout
 *   If negative, block indefinitely or until a condition is ready.
 *   If zero, check only for immediately available conditions and don't block.
 *   Else, this represents the maximum time to wait for a response from the
 *   waitset.
 * \return `RMW_RET_OK` if success, or
 * \return `RMW_RET_ERROR` if error, or
 * \return `RMW_RET_TIMEOUT` if wait timed out.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_wait(
  rmw_subscriptions_t * subscriptions,
  rmw_guard_conditions_t * guard_conditions,
  rmw_services_t * services,
  rmw_clients_t * clients,
  rmw_waitset_t * waitset,
  const rmw_time_t * wait_timeout);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_get_topic_names_and_types(
  const rmw_node_t * node,
  rmw_topic_names_and_types_t * topic_names_and_types);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_destroy_topic_names_and_types(
  rmw_topic_names_and_types_t * topic_names_and_types);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_get_node_names(
  const rmw_node_t * node,
  rcutils_string_array_t * node_names);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_count_publishers(
  const rmw_node_t * node,
  const char * topic_name,
  size_t * count);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_count_subscribers(
  const rmw_node_t * node,
  const char * topic_name,
  size_t * count);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_get_gid_for_publisher(const rmw_publisher_t * publisher, rmw_gid_t * gid);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_compare_gids_equal(const rmw_gid_t * gid1, const rmw_gid_t * gid2, bool * result);

/// Check if a service server is available for the given service client.
/**
 * This function will return true for `is_available` if there is a service
 * server available for the given client.
 *
 * The node parameter must not be `NULL`, and must point to a valid node.
 *
 * The client parameter must not be `NULL`, and must point to a valid client.
 *
 * The given client and node must match, i.e. the client must have been created
 * using the given node.
 *
 * The is_available parameter must not be `NULL`, and must point to a bool
 * variable.
 * The result of the check will be stored in the is_available parameter.
 *
 * This function does manipulate heap memory.
 * This function is not thread-safe.
 * This function is lock-free.
 *
 * \param[in] node the handle to the node being used to query the ROS graph
 * \param[in] client the handle to the service client being queried
 * \param[out] is_available
 *   set to true if there is a service server available, else false
 * \return `RMW_RET_OK` if node the check was made successfully, or
 * \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_service_server_is_available(
  const rmw_node_t * node,
  const rmw_client_t * client,
  bool * is_available);

#if __cplusplus
}
#endif

#endif  // RMW__RMW_H_

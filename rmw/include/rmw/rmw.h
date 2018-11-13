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
 * There are some common utility functions in combination with "Topics" or "Services":
 * - A function to validate a fully qualified topic or service name
 *   - rmw_validate_full_topic_name()
 *   - rmw/validate_full_topic_name.h
 * - A function to validate a node's namespace
 *   - rmw_validate_namespace()
 *   - rmw/validate_namespace.h
 * - A function to validate a node's name
 *   - rmw_validate_node_name()
 *   - rmw/validate_node_name.h
 *
 * It also has some machinery that is necessary to wait on and act on these concepts:
 *
 * - Initialization and shutdown management (global for now)
 *   - rmw/rmw.h
 * - Wait sets for waiting on messages and service requests / responses to be ready
 *   - rmw/rmw.h
 * - Guard conditions for waking up wait sets asynchronously
 *   - rmw/rmw.h
 * - Introspection of the ROS graph
 *   - rmw_names_and_types_t
 *   - rmw_get_topic_names_and_types()
 *   - rmw_get_service_names_and_types()
 *   - rmw/names_and_types.h
 *   - rmw/get_topic_names_and_types.h
 *   - rmw/get_service_names_and_types.h
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

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "rcutils/types.h"

#include "rosidl_generator_c/message_type_support_struct.h"
#include "rosidl_generator_c/service_type_support_struct.h"

#include "rmw/macros.h"
#include "rmw/qos_profiles.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"

RMW_PUBLIC
RMW_WARN_UNUSED
const char *
rmw_get_implementation_identifier(void);

/// Get the unique serialization format for this middleware.
/**
 * Return the format in which binary data is serialized.
 * One middleware can only have one encoding.
 * In contrast to the implementation identifier, the serialization format can be equal between
 * multiple RMW implementations.
 * This means, that the same binary messages can be deserialized by RMW implementations with the
 * same format.
 * \sa rmw_serialize
 * \sa rmw_deserialize
 * \return serialization format
 */
RMW_PUBLIC
RMW_WARN_UNUSED
const char *
rmw_get_serialization_format(void);

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
  const rmw_node_security_options_t * security_options);

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

/// Retrieve the number of matched subscriptions to a publisher
/**
 * Query the underlying middleware to determine how many subscriptions are
 * matched to a given publisher.
 *
 * \param publisher the publisher object to inspect
 * \param subscription_count the number of subscriptions matched
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if either argument is null, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_publisher_count_matched_subscriptions(
  const rmw_publisher_t * publisher,
  size_t * subscription_count);

/// Publish an already serialized message.
/**
 * The publisher must already be registered with the correct message type
 * support so that it can send serialized data corresponding to that type.
 * This function sends the serialized byte stream directly over the wire without
 * having to serialize the message first.
 * A ROS message can be serialized manually using the rmw_serialize() function.
 *
 * \param publisher the publisher object registered to send the message
 * \param serialized_message the serialized message holding the byte stream
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_publish_serialized_message(
  const rmw_publisher_t * publisher, const rmw_serialized_message_t * serialized_message);

/// Serialize a ROS message into a rmw_serialized_message_t.
/**
 * The ROS message is serialized into a byte stream contained within the
 * rmw_serialized_message_t structure.
 * The serialization format depends on the underlying middleware.
 *
 * \param ros_message the typed ROS message
 * \param type_support the typesupport for the ROS message
 * \param serialized_message the destination for the serialize ROS message
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation failed, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_serialize(
  const void * ros_message,
  const rosidl_message_type_support_t * type_support,
  rmw_serialized_message_t * serialized_message);

/// Deserialize a ROS message.
/**
 * The given rmw_serialized_message_t's internal byte stream buffer is deserialized
 * into the given ROS message.
 * The ROS message must already be allocated and initialized, and must match
 * the given typesupport structure.
 * The serialization format expected in the rmw_serialized_message_t depends on the
 * underlying middleware.
 *
 * \param serialized_message the serialized message holding the byte stream
 * \param type_support the typesupport for the typed ros message
 * \param ros_message destination for the deserialized ROS message
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation failed, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_deserialize(
  const rmw_serialized_message_t * serialized_message,
  const rosidl_message_type_support_t * type_support,
  void * ros_message);

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

/// Retrieve the number of matched publishers to a subscription
/**
 * Query the underlying middleware to determine how many publishers are
 * matched to a given subscription.
 *
 * \param subscription the publisher object to inspect
 * \param publisher_count the number of subscriptions matched
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if either argument is null, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_subscription_count_matched_publishers(
  const rmw_subscription_t * subscription,
  size_t * publisher_count);

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

/// Take a message without deserializing it.
/**
 * The message is taken in its serialized form. In contrast to rmw_take, the message
 * is not deserialized in its ROS type but rather returned as a byte stream.
 * The subscriber has to be registered for a specific type. But instead of receiving
 * the message as its corresponding message type, it is taken as a byte stream.
 * If needed, this byte stream can then be deserialized in a ROS message with a call to
 * rmw_deserialize.
 *
 * \param subscription subscription object to take from
 * \param serialized_message the destination in which to store the serialized message
 * \param taken boolean flag indicating if a message was taken or not
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation failed, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_take_serialized_message(
  const rmw_subscription_t * subscription,
  rmw_serialized_message_t * serialized_message,
  bool * taken);

/// Take a message without deserializing it and with its additional message information.
/**
 * The same as rmw_take_serialized_message(), except it also includes the
 * rmw_message_info_t.
 *
 * \param subscription subscription object to take from
 * \param serialized_message the destination in which to store the serialized message
 * \param taken boolean flag indicating if a message was taken or not
 * \param message_info a structure containing meta information about the taken message
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation failed, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_take_serialized_message_with_info(
  const rmw_subscription_t * subscription,
  rmw_serialized_message_t * serialized_message,
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

/// Create a wait set to store conditions that the middleware will block on.
/**
 * If `max_conditions` is `0`, the wait set can store an unbounded number of
 * conditions to wait on.
 * If `max_conditions` is greater than `0`, the number of conditions that can
 * be attached to the wait set is bounded at `max_conditions`.
 * \param[in] max_conditions
 *   The maximum number of conditions that can be attached to the wait set.
 * \return A pointer to the created wait set, `NULL` if an error occurred.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_wait_set_t *
rmw_create_wait_set(size_t max_conditions);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_destroy_wait_set(rmw_wait_set_t * wait_set);

/// Waits on sets of different waitable entities and returns when one is ready.
/**
 * Add conditions to the wait set and wait until a response comes in, or until
 * the timeout is reached.
 * The arrays contain type-erased representations of waitable entities.
 * This function casts the pointers to middleware-specific conditions and adds
 * them to the wait set.
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
 * \param wait_set Storage for the wait set
 * \param wait_timeout
 *   If NULL, block until a condition is ready.
 *   If zero, check only for immediately available conditions and don't block.
 *   Else, this represents the maximum time to wait for a response from the
 *   wait set.
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
  rmw_wait_set_t * wait_set,
  const rmw_time_t * wait_timeout);

/// Return a list of node name and namespaces discovered via a node.
/**
 * This function will return a list of node names and a list of node namespaces
 * that are discovered via the middleware.
 * The two lists represent pairs of namespace and name for each discovered
 * node.
 * The lists will be the same length and the same position will refer to the
 * same node across lists.
 *
 * The node parameter must not be `NULL`, and must point to a valid node.
 *
 * The node_names parameter must not be `NULL`, and must point to a valid
 * string array.
 *
 * The node_namespaces parameter must not be `NULL`, and must point to a
 * valid string array.
 *
 * This function does manipulate heap memory.
 * This function is not thread-safe.
 * This function is lock-free.
 *
 * \param[in] node the handle to the node being used to query the ROS graph
 * \param[out] node_names a list of discovered node names
 * \param[out] node_namespaces a list of discovered node namespaces
 * \return `RMW_RET_OK` if node the query was made successfully, or
 * \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_get_node_names(
  const rmw_node_t * node,
  rcutils_string_array_t * node_names,
  rcutils_string_array_t * node_namespaces);

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

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_set_log_severity(rmw_log_severity_t severity);

#ifdef __cplusplus
}
#endif

#endif  // RMW__RMW_H_

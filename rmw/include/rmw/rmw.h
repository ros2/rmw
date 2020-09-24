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
 * - Initialization and Shutdown:
 *   - rmw/init.h
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
 *   - rmw/get_topic_endpoint_info.h
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

#include "rcutils/macros.h"
#include "rcutils/types.h"

#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_runtime_c/service_type_support_struct.h"
#include "rosidl_runtime_c/sequence_bound.h"

#include "rmw/init.h"
#include "rmw/macros.h"
#include "rmw/qos_profiles.h"
#include "rmw/subscription_options.h"
#include "rmw/message_sequence.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"

/// Get the name of the rmw implementation being used
/**
 * \return Name of rmw implementation
 */
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

// TODO(wjwwood): refactor this API to return a return code when updated to use an allocator
/// Create a node and return a handle to that node.
/**
 * This function can fail, and therefore return `NULL`, if:
 *   - name is not a valid non-null node name
 *   - namespace_ is not a valid non-null namespace
 *   - context is not valid i.e. it is zero-initialized, or
 *     its implementation identifier does not match that of
 *     this API implementation, or has been invalidated by
 *     `rmw_shutdown()`
 *   - memory allocation fails during node creation
 *   - an unspecified error occurs
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | No
 * Uses Atomics       | No [1]
 * Lock-Free          | No [1]
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * This should be defined by the rmw implementation.
 *
 * \param[in] context init context that this node should be associated with
 * \param[in] name the node name
 * \param[in] namespace_ the node namespace
 * \return rmw node handle, or `NULL` if there was an error
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_node_t *
rmw_create_node(
  rmw_context_t * context,
  const char * name,
  const char * namespace_);

/// Finalize a given node handle, reclaim the resources, and deallocate the node handle.
/**
 * This function will return early if a logical error, such as `RMW_RET_INVALID_ARGUMENT`
 * or `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`, ensues, leaving the given node handle unchanged.
 * Otherwise, it will proceed despite errors, freeing as many resources as it can, including
 * the node handle. Usage of a deallocated node handle is undefined behavior.
 *
 * \pre All publishers, subscribers, services, and clients created from this node must
 *   have been destroyed prior to this call. Some rmw implementations may verify this,
 *   returning `RMW_RET_ERROR` and setting a human readable error message if any entity
 *   created from this node has not yet been destroyed. However, this is not guaranteed
 *   and so callers should ensure that this is the case before calling this function.
 *
 * \param[in] node the node handle to be destroyed
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if node is invalid, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the implementation
 *   identifier does not match, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_destroy_node(rmw_node_t * node);

RMW_PUBLIC
RCUTILS_DEPRECATED_WITH_MSG(
  "rmw_node_assert_liveliness implementation was removed."
  " If manual liveliness assertion is needed, use MANUAL_BY_TOPIC.")
rmw_ret_t
rmw_node_assert_liveliness(const rmw_node_t * node);

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
 */
RMW_PUBLIC
RMW_WARN_UNUSED
const rmw_guard_condition_t *
rmw_node_get_graph_guard_condition(const rmw_node_t * node);

/// Initialize a publisher allocation to be used with later publications.
/**
 * This creates an allocation object that can be used in conjunction with
 * the rmw_publish method to perform more carefully control memory allocations.
 *
 * This will allow the middleware to preallocate the correct amount of memory
 * for a given message type and message bounds.
 * As allocation is performed in this method, it will not be necessary to allocate
 * in the `rmw_publish` method.
 *
 * \param[in] type_support Type support of the message to be preallocated.
 * \param[in] message_bounds Bounds structure of the message to be preallocated.
 * \param[out] allocation Allocation structure to be passed to `rmw_publish`.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_UNSUPPORTED` if it's unimplemented
 * \return `RMW_RET_INVALID_ARGUMENT` if an argument is null, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_init_publisher_allocation(
  const rosidl_message_type_support_t * type_support,
  const rosidl_runtime_c__Sequence__bound * message_bounds,
  rmw_publisher_allocation_t * allocation);

/// Destroy a publisher allocation object.
/**
 * This deallocates any memory allocated by `rmw_init_publisher_allocation`.
 *
 * \param[in] allocation Allocation object to be destroyed.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_UNSUPPORTED` if it's unimplemented
 * \return `RMW_RET_INVALID_ARGUMENT` if argument is null, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_fini_publisher_allocation(
  rmw_publisher_allocation_t * allocation);

/// Return a rmw_publisher_options_t initialized with default values.
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_publisher_options_t
rmw_get_default_publisher_options(void);

/// Create a publisher and return a handle to that publisher.
/**
 * This function can fail, and therefore return `NULL`, if:
 *   - node is not a valid non-null handle for this rmw implementation,
 *     as returned by `rmw_create_node()`
 *   - type_support is a not valid non-null message type support, as returned by
 *     `ROSIDL_GET_MSG_TYPE_SUPPORT()`
 *   - topic_name is not a valid non-null topic name, according to
 *     `rmw_validate_full_topic_name()`
 *   - qos_profile is not a fully specified non-null profile i.e. no UNKNOWN policies
 *   - publisher_options is not a valid non-null option set, as returned by
 *     `rmw_get_default_publisher_options()`
 *   - memory allocation fails during publisher creation
 *   - an unspecified error occurs
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | No
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \param[in] node Handle to node with which to register this publisher
 * \param[in] type_support Type support for the messages to be published
 * \param[in] topic_name Name of the topic to publish to, often a fully qualified
 *   topic name unless `qos_profile` is configured to avoid ROS namespace conventions
 *   i.e. to create a native topic publisher
 * \param[in] qos_profile QoS policies for this publisher
 * \param[in] publisher_options Options to configure this publisher
 * \return rmw publisher handle, or `NULL` if there was an error
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_publisher_t *
rmw_create_publisher(
  const rmw_node_t * node,
  const rosidl_message_type_support_t * type_support,
  const char * topic_name,
  const rmw_qos_profile_t * qos_profile,
  const rmw_publisher_options_t * publisher_options);

/// Finalize a given publisher handle, reclaim the resources, and deallocate the publisher handle.
/**
 * This function will return early if a logical error, such as `RMW_RET_INVALID_ARGUMENT`
 * or `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`, ensues, leaving the given publisher handle unchanged.
 * Otherwise, it will proceed despite errors, freeing as many resources as it can, including
 * the publisher handle. Usage of a deallocated publisher handle is undefined behavior.
 *
 * \pre Given node must be the one the publisher was registered with.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | No
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \param[in] node Handle to node with which the given publisher is registered
 * \param[in] publisher Handle to publisher to be finalized
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if node or publisher is `NULL`, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if node or publisher
 *   implementation identifier does not match, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_destroy_publisher(rmw_node_t * node, rmw_publisher_t * publisher);

/// Borrow a loaned ROS message.
/**
 * This ROS message is owned by the middleware, that will keep it alive (i.e. in valid
 * memory space) until the caller publishes it using rmw_publish_loaned_message() or
 * returns it using rmw_return_loaned_message_from_publisher().
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par Runtime behavior
 *   To borrow a ROS message is a synchronous operation.
 *   It is also non-blocking, but it is not guaranteed to be lock-free.
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par Memory allocation
 *   It is implementation defined whether memory will be allocated on borrow or not.
 *   Check the implementation documentation to learn about memory allocation
 *   guarantees when using ROS message loaning support.
 *
 * \par Thread-safety
 *   Publishers are thread-safe objects, and so are all operations on them except for finalization.
 *   Therefore, it is safe to borrow ROS messages from the same publisher concurrently.
 *
 * \pre Given `publisher` must be a valid publisher, as returned by rmw_create_publisher().
 * \pre Given `type_support` must be a valid `rosidl` message type support, matching the
 *   one registered with the `publisher` on creation.
 *
 * \param[in] publisher Publisher to which the loaned ROS message will be associated.
 * \param[in] type_support Message type support of the loaned ROS message.
 * \param[out] ros_message Pointer to type erased ROS message loaned by the middleware.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `publisher` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `type_support` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `ros_message` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `*ros_message` is not NULL (to prevent leaks), or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if `publisher` implementation identifier
 *   does not match this implementation, or
 * \return `RMW_RET_UNSUPPORTED` if the implementation does not support ROS message loaning, or
 * \return `RMW_RET_ERROR` if an unexpected error occured.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_borrow_loaned_message(
  const rmw_publisher_t * publisher,
  const rosidl_message_type_support_t * type_support,
  void ** ros_message);

/// Return a loaned message previously borrowed from a publisher.
/**
 * Tells the middleware that a borrowed ROS message is no longer needed by the caller.
 * Ownership of the ROS message is given back to the middleware.
 * If this function fails early due to a logical error, such as an invalid argument,
 * the loaned ROS message will be left unchanged.
 * Otherwise, ownership of the ROS message will be given back to the middleware.
 * It is up to the middleware what will be made of the returned ROS message.
 * It is undefined behavior to use a loaned ROS message after returning it.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par Runtime behavior
 *   To return a ROS message is a synchronous operation.
 *   It is also non-blocking, but it is not guaranteed to be lock-free.
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par Thread-safety
 *   Publishers are thread-safe objects, and so are all operations on them except for finalization.
 *   Therefore, it is safe to return borrowed ROS messages to the same publisher concurrently.
 *   However, since ownership of the loaned ROS message is given back to the middleware and
 *   this transfer is not synchronized, it is not safe to return the same loaned ROS message
 *   concurrently.
 *
 * \pre Given `publisher` must be a valid publisher, as returned by rmw_create_publisher().
 * \pre Given `loaned_message` must have been previously borrowed from the same publisher
 *   using rmw_borrow_loaned_message().
 *
 * \param[in] publisher Publisher to which the loaned ROS message is associated.
 * \param[in] loaned_message Type erased loaned ROS message to be returned.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `publisher` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `loaned_message` is NULL, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if `publisher` implementation identifier
 *   does not match this implementation, or
 * \return `RMW_RET_UNSUPPORTED` if the implementation does not support ROS message loaning, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs and no message can be initialized.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_return_loaned_message_from_publisher(
  const rmw_publisher_t * publisher,
  void * loaned_message);

/// Publish a ROS message.
/**
 * Send a ROS message to all subscriptions with matching QoS policies using the given publisher.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par Runtime behavior
 *   It is implementation defined whether to publish a ROS message is a
 *   synchronous or asynchronous, blocking or non-blocking operation.
 *   However, asynchronous implementations are not allowed to access the
 *   given ROS message after this function returns.
 *   Check the implementation documentation to learn about publish behavior.
 *
 * \par Memory allocation
 *   It is implementation defined whether memory will be allocated on publish or not.
 *   For instance, implementations that serialize ROS messages to send it over the
 *   wire may need to perform additional memory allocations when dealing with
 *   unbounded (dynamically-sized) fields.
 *   A publisher allocation, if provided, may or may not be used.
 *   Check the implementation documentation to learn about memory allocation
 *   guarantees when publishing ROS messages with and without publisher allocations.
 *
 * \par Thread-safety
 *   Publishers are thread-safe objects, and so are all operations on them except for finalization.
 *   Therefore, it is safe to publish using the same publisher concurrently.
 *   However, when publishing regular ROS messages:
 *   - Access to the ROS message is read-only but it is not synchronized.
 *     Concurrent `ros_message` reads are safe, but concurrent reads and writes are not.
 *   - Access to the publisher allocation is not synchronized, unless specifically stated
 *     otherwise by the implementation.
 *     Thus, it is generally not safe to read or write `allocation` while rmw_publish() uses it.
 *     Check the implementation documentation to learn about publisher allocations' thread-safety.
 *
 * \pre Given `publisher` must be a valid publisher, as returned by rmw_create_publisher().
 * \pre Given `ros_message` must be a valid message, whose type matches the message type
 *   support the `publisher` was registered with on creation.
 * \pre If not NULL, given `allocation` must be a valid publisher allocation, initialized
 *   with rmw_publisher_allocation_init() with a message type support that matches the
 *   one registered with `publisher` on creation.
 *
 * \param[in] publisher Publisher to be used to send message.
 * \param[in] ros_message Type erased ROS message to be sent.
 * \param[in] allocation Pre-allocated memory to be used. May be NULL.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `publisher` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `ros_message` is NULL, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if `publisher` implementation
 *   identifier does not match this implementation, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_publish(
  const rmw_publisher_t * publisher,
  const void * ros_message,
  rmw_publisher_allocation_t * allocation);

/// Publish a loaned ROS message.
/**
 * Send a previously borrowed ROS message to all subscriptions with matching QoS policies
 * using the given publisher, then return ROS message ownership to the middleware.
 *
 * If this function fails early due to a logical error, such as an invalid argument,
 * the loaned ROS message will be left unchanged.
 * Otherwise, ownership of the ROS message will be given back to the middleware.
 * It is up to the middleware what will be made of the returned ROS message.
 * It is undefined behavior to use a loaned ROS message after publishing it.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check the implementation documentation.</i>
 *
 * \par Runtime behavior
 *   It is implementation defined whether to publish a loaned ROS message is a
 *   synchronous or asynchronous, blocking or non-blocking operation.
 *   Check the implementation documentation to learn about publish behavior.
 *
 * \par Memory allocation
 *   It is implementation defined whether memory will be allocated on publish or not.
 *   For instance, implementations that serialize ROS messages to send it over the
 *   wire may need to perform additional memory allocations when dealing with
 *   unbounded (dynamically-sized) fields.
 *   A publisher allocation, if provided, may or may not be used.
 *   Check the implementation documentation to learn about memory allocation
 *   guarantees when publishing loaned ROS messages with and without publisher allocations.
 *
 * \par Thread-safety
 *   Publishers are thread-safe objects, and so are all operations on them except for finalization.
 *   Therefore, it is safe to publish using the same publisher concurrently.
 *   However, when publishing loaned ROS messages:
 *   - Ownership of the loaned ROS message is given back to the middleware.
 *     This transfer is not synchronized, and thus it is not safe to publish the
 *     same loaned ROS message concurrently.
 *   - Access to the publisher allocation is not synchronized, unless specifically stated
 *     otherwise by the implementation.
 *     Thus, it is generally not safe to read or write `allocation` while rmw_publish() uses it.
 *     Check the implementation documentation to learn about publisher allocations' thread-safety.
 *
 * \pre Given `publisher` must be a valid publisher, as returned by rmw_create_publisher().
 * \pre Given `ros_message` must be a valid message, borrowed from the same publisher using
 *   rmw_borrow_loaned_message().
 * \pre If not NULL, given `allocation` must be a valid publisher allocation, initialized
 *   with rmw_publisher_allocation_init() with a message type support that matches the
 *   one registered with `publisher` on creation.
 *
 * \param[in] publisher Publisher to be used to send message.
 * \param[in] ros_message Loaned type erased ROS message to be sent.
 * \param[in] allocation Pre-allocated memory to be used. May be NULL.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `publisher` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `ros_message` is NULL, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if `publisher` implementation
 *   identifier does not match this implementation, or
 * \return `RMW_RET_UNSUPPORTED` if the implementation does not support ROS message loaning, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_publish_loaned_message(
  const rmw_publisher_t * publisher,
  void * ros_message,
  rmw_publisher_allocation_t * allocation);

/// Retrieve the number of matched subscriptions to a publisher.
/**
 * Query the underlying middleware to determine how many subscriptions are
 * matched to a given publisher.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | No
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \param[in] publisher the publisher object to inspect
 * \param[out] subscription_count the number of subscriptions matched
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if either argument is null, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if publisher
 *   implementation identifier does not match, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_publisher_count_matched_subscriptions(
  const rmw_publisher_t * publisher,
  size_t * subscription_count);

/// Retrieve the actual qos settings of the publisher.
/**
 * Query the underlying middleware to determine the qos settings
 * of the publisher.
 * The actual configuration applied when using RMW_*_SYSTEM_DEFAULT
 * can only be resolved after the creation of the publisher, and it
 * depends on the underlying rmw implementation.
 * If the underlying setting in use can't be represented in ROS terms,
 * it will be set to RMW_*_UNKNOWN.
 *
 * \note The value of avoid_ros_namespace_conventions field is not resolved
 *   with this function. The rcl function `rcl_publisher_get_actual_qos()`
 *   resolves it.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe [1]
 * Thread-Safe        | No
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \param[in] publisher the publisher object to inspect
 * \param[out] qos the actual qos settings
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if either argument is null, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if publisher
 *   implementation identifier does not match, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_publisher_get_actual_qos(
  const rmw_publisher_t * publisher,
  rmw_qos_profile_t * qos);

/// Publish a ROS message as a byte stream.
/**
 * Send a ROS message serialized as a byte stream to all subscriptions with
 * matching QoS policies using the given publisher.
 * A ROS message can be serialized manually using rmw_serialize().
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check the implementation documentation.</i>
 *
 * \par Runtime behavior
 *   It is implementation defined whether to publish a loaned ROS message is a
 *   synchronous or asynchronous, blocking or non-blocking operation.
 *   However, asynchronous implementations are not allowed to access the
 *   given byte stream after this function returns.
 *   Check the implementation documentation to learn about publish behavior.
 *
 * \par Memory allocation
 *   It is implementation defined whether memory will be allocated on publish or not.
 *   Even if a publisher allocation is provided, an implementation may ignore it.
 *   Check the implementation documentation to learn about memory allocation
 *   guarantees when publishing serialized messages with and without publisher allocations.
 *
 * \par Thread-safety
 *   Publishers are thread-safe objects, and so are all operations on them except for finalization.
 *   Therefore, it is safe to publish using the same publisher concurrently.
 *   However, when publishing serialized ROS messages:
 *   - Access to the byte stream is read-only but it is not synchronized.
 *     Concurrent `serialized_message` reads are safe, but concurrent reads and writes are not.
 *   - Access to the publisher allocation is not synchronized, unless specifically stated
 *     otherwise by the implementation.
 *     Thus, it is generally not safe to read or write `allocation` while rmw_publish() uses it.
 *     Check the implementation documentation to learn about publisher allocations' thread-safety.
 *
 * \pre Given `publisher` must be a valid publisher, as returned by rmw_create_publisher().
 * \pre Given `serialized_message` must be a valid serialized message, initialized by
 *   rmw_serialized_message_init() and containing the serialization of a ROS message whose
 *   type matches the message type support the `publisher` was registered with on creation.
 * \pre If not NULL, given `allocation` must be a valid publisher allocation, initialized
 *   with rmw_publisher_allocation_init() with a message type support that matches the
 *   one registered with `publisher` on creation.
 *
 * \param[in] publisher Publisher to be used to send message.
 * \param[in] ros_message Serialized ROS message to be sent.
 * \param[in] allocation Pre-allocated memory to be used. May be NULL.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `publisher` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `serialized_message` is NULL, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if `publisher` implementation
 *   identifier does not match this implementation, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_publish_serialized_message(
  const rmw_publisher_t * publisher,
  const rmw_serialized_message_t * serialized_message,
  rmw_publisher_allocation_t * allocation);

/// Compute the size of a serialized message.
/**
 * Given a message definition and bounds, compute the serialized size.
 *
 * \param[in] type_support The type support of the message to compute.
 * \param[in] bounds Artifical bounds to use on unbounded fields.
 * \param[out] size The computed size of the serialized message.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if either argument is null, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_get_serialized_message_size(
  const rosidl_message_type_support_t * type_support,
  const rosidl_runtime_c__Sequence__bound * message_bounds,
  size_t * size);

/// Manually assert that this Publisher is alive (for RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC)
/**
 * If the rmw Liveliness policy is set to RMW_QOS_POLICY_LIVELINESS_MANUAL_BY_TOPIC, the creator of
 * this publisher may manually call `assert_liveliness` at some point in time to signal to the rest
 * of the system that this Node is still alive.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | Yes
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * \param[in] publisher handle to the publisher that needs liveliness to be asserted
 * \return `RMW_RET_OK` if the liveliness assertion was completed successfully, or
 * \return `RMW_RET_ERROR` if an unspecified error occurs, or
 * \return `RMW_RET_UNSUPPORTED` if the rmw implementation does not support asserting liveliness.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_publisher_assert_liveliness(const rmw_publisher_t * publisher);

/// Serialize a ROS message into a rmw_serialized_message_t.
/**
 * The ROS message is serialized into a byte stream contained within the
 * rmw_serialized_message_t structure.
 * The serialization format depends on the underlying implementation.
 *
 * \pre Given ROS message must be a valid non-null instance, initialized
 *   by the caller and matching the provided typesupport.
 * \pre Given typesupport must be a valid non-null instance, as provided
 *   by `rosidl` APIs.
 * \pre Given serialized message must be a valid non-null instance, initialized
 *   by the caller.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe [1]
 * Thread-Safe        | No
 * Uses Atomics       | Maybe [2]
 * Lock-Free          | Maybe [2]
 * <i>[1] if the given serialized message does not have enough capacity to hold
 *        the ROS message serialization</i>
 * <i>[2] rmw implementation defined, check the implementation documentation</i>
 *
 * \param[in] ros_message the typed ROS message
 * \param[in] type_support the typesupport for the ROS message
 * \param[out] serialized_message the destination for the serialize ROS message
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
 * The serialization format expected in the rmw_serialized_message_t depends on the
 * underlying implementation.
 *
 * \pre Given serialized message must be a valid non-null instance, such
 *   as that returned by `rmw_serialize()`, matching provided typesupport
 *   and ROS message.
 * \pre Given typesupport must be a valid non-null instance, as provided
 *   by `rosidl` APIs.
 * \pre Given ROS message must be a valid non-null instance, initialized
 *   by the caller.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe [1]
 * Thread-Safe        | No
 * Uses Atomics       | Maybe [2]
 * Lock-Free          | Maybe [2]
 * <i>[1] if the given ROS message contains unbounded fields</i>
 * <i>[2] rmw implementation defined, check the implementation documentation</i>
 *
 * \param[in] serialized_message the serialized message holding the byte stream
 * \param[in] type_support the typesupport for the typed ros message
 * \param[out] ros_message destination for the deserialized ROS message
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

/// Initialize a subscription allocation to be used with later `take`s.
/**
 * This creates an allocation object that can be used in conjunction with
 * the rmw_take method to perform more carefully control memory allocations.
 *
 * This will allow the middleware to preallocate the correct amount of memory
 * for a given message type and message bounds.
 * As allocation is performed in this method, it will not be necessary to allocate
 * in the `rmw_take` method.
 *
 * \param[in] type_support Type support of the message to be preallocated.
 * \param[in] message_bounds Bounds structure of the message to be preallocated.
 * \param[out] allocation Allocation structure to be passed to `rmw_take`.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_UNSUPPORTED` if it's unimplemented
 * \return `RMW_RET_INVALID_ARGUMENT` if an argument is null, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_init_subscription_allocation(
  const rosidl_message_type_support_t * type_support,
  const rosidl_runtime_c__Sequence__bound * message_bounds,
  rmw_subscription_allocation_t * allocation);

/// Destroy a publisher allocation object.
/**
 * This deallocates memory allocated by `rmw_init_subscription_allocation`.
 *
 * \param[in] allocation Allocation object to be destroyed.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_UNSUPPORTED` if it's unimplemented
 * \return `RMW_RET_INVALID_ARGUMENT` if argument is null, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_fini_subscription_allocation(
  rmw_subscription_allocation_t * allocation);

/// Create a subscription and return a handle to that subscription.
/**
 * This function can fail, and therefore return `NULL`, if:
 *   - node is not a valid non-null handle for this rmw implementation,
 *     as returned by `rmw_create_node()`
 *   - type_support is a not valid non-null message type support, as returned by
 *     `ROSIDL_GET_MSG_TYPE_SUPPORT()`
 *   - topic_name is not a valid non-null topic name, according to
 *     `rmw_validate_full_topic_name()` if ROS namespace conventions apply
 *   - qos_profile is not a fully specified non-null profile i.e. no UNKNOWN policies
 *   - subscription_options is not a valid non-null option set, such as the one
 *     returned by `rmw_get_default_subscription_options()`
 *   - memory allocation fails during subscription creation
 *   - an unspecified error occurs
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | No
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \param[in] node Handle to node with which to register this subscription
 * \param[in] type_support Type support for the messages to be subscribed to
 * \param[in] topic_name Name of the topic to subscribe to, often a fully qualified
 *   topic name unless `qos_profile` is configured to avoid ROS namespace conventions
 *   i.e. to create a native topic subscription
 * \param[in] qos_profile QoS policies for this subscription
 * \param[in] subscription_options Options for configuring this subscription
 * \return rmw subscription handle, or `NULL` if there was an error
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_subscription_t *
rmw_create_subscription(
  const rmw_node_t * node,
  const rosidl_message_type_support_t * type_support,
  const char * topic_name,
  const rmw_qos_profile_t * qos_policies,
  const rmw_subscription_options_t * subscription_options);

/// Finalize a given subscription handle, reclaim the resources, and deallocate the subscription
/// handle.
/**
 * This function will return early if a logical error, namely `RMW_RET_INVALID_ARGUMENT`
 * or `RMW_RET_INCORRECT_RMW_IMPLEMENTATION`, ensues, leaving the given subscription handle
 * unchanged.
 * Otherwise, it will proceed despite errors, freeing as many resources as it can, including
 * the subscription handle, and return `RMW_RET_ERROR`. Usage of a deallocated subscription
 * handle is undefined behavior.
 *
 * \pre Given node must be the one the subscription was registered with.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | No
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \param[in] node Handle to node with which the given subscription is registered
 * \param[in] subscription Handle to subscription to be finalized
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if node or subscription is `NULL`, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if node or subscription
 *   implementation identifier does not match, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_destroy_subscription(rmw_node_t * node, rmw_subscription_t * subscription);

/// Retrieve the number of matched publishers to a subscription.
/**
 * Query the underlying middleware to determine how many publishers are
 * matched to a given subscription.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | No
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \param[in] subscription the subscription object to inspect
 * \param[out] publisher_count the number of publishers matched
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if either argument is null, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if subscription
 *   implementation identifier does not match, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_subscription_count_matched_publishers(
  const rmw_subscription_t * subscription,
  size_t * publisher_count);

/// Retrieve the actual qos settings of the subscription.
/**
 * Query the underlying middleware to determine the qos settings
 * of the subscription.
 * The actual configuration applied when using RMW_*_SYSTEM_DEFAULT
 * can only be resolved after the creation of the subscription, and it
 * depends on the underlying rmw implementation.
 * If the underlying setting in use can't be represented in ROS terms,
 * it will be set to RMW_*_UNKNOWN.
 *
 * \note The value of avoid_ros_namespace_conventions field is not resolved
 *   with this function. The rcl function `rcl_subscription_get_actual_qos()`
 *   resolves it.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe [1]
 * Thread-Safe        | No
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \param[in] subscription the subscription object to inspect
 * \param[out] qos the actual qos settings
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if either argument is null, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if subscription
 *   implementation identifier does not match, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_subscription_get_actual_qos(
  const rmw_subscription_t * subscription,
  rmw_qos_profile_t * qos);

/// Take an incoming ROS message.
/**
 * Take a ROS message already received by the given subscription, removing it from internal queues.
 * This function will succeed even if no ROS message was received, but `taken` will be false.
 *
 * \remarks The same ROS message cannot be taken twice.
 *   Callers do not have to deal with duplicates.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par Runtime behavior
 *   To take a ROS message is a synchronous operation.
 *   It is also non-blocking, to the extent it will not wait for new ROS messages to arrive,
 *   but it is not guaranteed to be lock-free.
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par Memory allocation
 *   It is implementation defined whether memory will be allocated on take or not.
 *   For instance, implementations that deserialize ROS messages received over
 *   the wire may need to perform additional memory allocations when dealing with
 *   unbounded (dynamically-sized) fields.
 *   A subscription allocation, if provided, may or may not be used.
 *   Check the implementation documentation to learn about memory allocation
 *   guarantees when taking ROS messages with and without subscription allocations.
 *
 * \par Thread-safety
 *   Subscriptions are thread-safe objects, and so are all operations on them except for
 *   finalization.
 *   Therefore, it is safe to take from the same subscription concurrently.
 *   However, when taking regular ROS messages:
 *   - Access to the given ROS message is not synchronized.
 *     It is not safe to read or write `ros_message` while rmw_take() uses it.
 *   - Access to given primitive data-type arguments is not synchronized.
 *     It is not safe to read or write `taken` while rmw_take() uses it.
 *   - Access to the given subscription allocation is not synchronized,
 *     unless specifically stated otherwise by the implementation.
 *     Thus, it is generally not safe to read or write `allocation` while rmw_take() uses it.
 *     Check the implementation documentation to learn about subscription allocations'
 *     thread-safety.
 *
 * \pre Given `subscription` must be a valid subscription, as returned
 *   by rmw_create_subscription().
 * \pre Given `ros_message` must be a valid message, whose type matches the message type support
 *   registered with the `subscription` on creation.
 * \pre If not NULL, given `allocation` must be a valid subscription allocation initialized
 *   with rmw_subscription_allocation_init() with a message type support that matches the
 *   one registered with the `subscription` on creation.
 * \post Given `ros_message` will remain a valid message.
 *   It will be left unchanged if this function fails early due to a logical error, such as an
 *   invalid argument, or in an unknown yet valid state if it fails due to a runtime error.
 *   It will also be left unchanged if this function succeeds but `taken` is false.
 *
 * \param[in] subscription Subscription to take message from.
 * \param[out] ros_message Type erased ROS message to write to.
 * \param[out] taken Boolean flag indicating if a ROS message was taken or not.
 * \param[in] allocation Pre-allocated memory to be used. May be NULL.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `subscription` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `ros_message` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `taken` is NULL, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `subscription`
 *   implementation identifier does not match this implementation, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_take(
  const rmw_subscription_t * subscription,
  void * ros_message,
  bool * taken,
  rmw_subscription_allocation_t * allocation);

/// Take an incoming ROS message with its metadata.
/**
 * Same as rmw_take(), except it also takes ROS message metadata.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par Runtime behavior
 *   To take a ROS message with its metadata is a synchronous operation.
 *   It is also non-blocking, to the extent it will not wait for new ROS messages to arrive,
 *   but it is not guaranteed to be lock-free.
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par Memory allocation
 *   It is implementation defined whether memory will be allocated on take or not.
 *   For instance, implementations that deserialize ROS messages received over
 *   the wire may need to perform additional memory allocations when dealing with
 *   unbounded (dynamically-sized) fields.
 *   A subscription allocation, if provided, may or may not be used.
 *   Check the implementation documentation to learn about memory allocation
 *   guarantees when taking ROS messages with and without subscription allocations.
 *
 * \par Thread-safety
 *   Subscriptions are thread-safe objects, and so are all operations on them except for
 *   finalization.
 *   Therefore, it is safe to take from the same subscription concurrently.
 *   However, when taking regular ROS messages with metadata:
 *   - Access to the given ROS message is not synchronized.
 *     It is not safe to read or write `ros_message` while rmw_take_with_info() uses it.
 *   - Access to given primitive data-type arguments is not synchronized.
 *     It is not safe to read or write `taken` while rmw_take_with_info() uses it.
 *   - Access to the given ROS message metadata is not synchronized.
 *     It is not safe to read or write `message_info` while rmw_take_with_info() uses it.
 *   - Access to the given subscription allocation is not synchronized,
 *     unless specifically stated otherwise by the implementation.
 *     Thus, it is generally not safe to read or write `allocation` while rmw_take_with_info()
 *     uses it.
 *     Check the implementation documentation to learn about subscription allocations'
 *     thread-safety.
 *
 * \pre Given `subscription` must be a valid subscription, as returned
 *   by rmw_create_subscription().
 * \pre Given `ros_message` must be a valid message, whose type matches
 *   the message type support registered with the `subscription` on creation.
 * \pre If not NULL, given `allocation` must be a valid subscription allocation
 *   initialized with rmw_subscription_allocation_init() with a message type support
 *   that matches the one registered with the `subscription` on creation.
 * \post Given `ros_message` will remain a valid message, and
 *   `message_info`, valid message metadata.
 *   Both will be left unchanged if this function fails early due to a logical error, such as
 *   an invalid argument, or in an unknown yet valid state if it fails due to a runtime error.
 *   Both will also be left unchanged if this function succeeds but `taken` is false.
 *
 * \param[in] subscription Subscription to take ROS message from.
 * \param[out] ros_message Type erased ROS message to write to.
 * \param[out] taken Boolean flag indicating if a ROS message was taken or not.
 * \param[out] message_info Taken ROS message metadata.
 * \param[in] allocation Pre-allocated memory to be used. May be NULL.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `subscription` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `ros_message` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `taken` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `message_info` is NULL, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `subscription`
 *   implementation identifier does not match this implementation, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_take_with_info(
  const rmw_subscription_t * subscription,
  void * ros_message,
  bool * taken,
  rmw_message_info_t * message_info,
  rmw_subscription_allocation_t * allocation);

/// Take multiple incoming ROS messages with their metadata.
/**
 * Take a sequence of consecutive ROS messages already received by the given
 * subscription, removing them from internal queues.
 * While `count` ROS messages may be requested, fewer messages may have been
 * received by the subscription.
 * This function will only take what has been already received, and it will
 * succeed even if fewer (or zero) messages were received.
 * In this case, only currently available messages will be returned.
 * The `taken` flag indicates the number of ROS messages actually taken.
 *
 * \remarks Once taken, ROS messages in the sequence cannot be taken again.
 *   Callers do not have to deal with duplicates.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par Runtime behavior
 *   To take a sequence of ROS messages is a synchronous operation.
 *   It is also non-blocking, to the extent it will not wait for new ROS messages to arrive,
 *   but it is not guaranteed to be lock-free.
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par Memory allocation
 *   It is implementation defined whether memory will be allocated on take or not.
 *   For instance, implementations that deserialize ROS messages received over
 *   the wire may need to perform additional memory allocations when dealing with
 *   unbounded (dynamically-sized) fields.
 *   A subscription allocation, if provided, may or may not be used.
 *   Check the implementation documentation to learn about memory allocation
 *   guarantees when taking ROS messages with and without subscription allocations.
 *
 * \par Thread-safety
 *   Subscriptions are thread-safe objects, and so are all operations on them except for
 *   finalization.
 *   Therefore, it is safe to take from the same subscription concurrently.
 *   Moreover, the sequence of ROS messages taken is guaranteed to be consecutive and to
 *   preserve the order in the subscription queues, despite any concurrent takes.
 *   However, when taking a sequence of ROS messages with metadata:
 *   - Access to the given ROS message sequence is not synchronized.
 *     It is not safe to read or write `message_sequence` while rmw_take_sequence() uses it.
 *   - Access to the given ROS message metadata sequence is not synchronized.
 *     It is not safe to read or write `message_info_sequence` while rmw_take_sequence() uses it.
 *   - Access to given primitive data-type arguments is not synchronized.
 *     It is not safe to read or write `taken` while rmw_take_sequence() uses it.
 *   - Access to the given subscription allocation is not synchronized,
 *     unless specifically stated otherwise by the implementation.
 *     Thus, it is generally not safe to read or write `allocation` while rmw_take_sequence()
 *     uses it.
 *     Check the implementation documentation to learn about subscription allocations'
 *     thread-safety.
 *
 * \pre Given `subscription` must be a valid subscription, as returned
 *   by rmw_create_subscription().
 * \pre Given `message_sequence` must be a valid message sequence, initialized
 *   by rmw_message_sequence_init() and populated with ROS messages whose
 *   type matches the message type support registered with the `subscription`
 *   on creation.
 * \pre Given `message_info_sequence` must be a valid message metadata sequence,
 *   initialized by rmw_message_info_sequence_init().
 * \pre If not NULL, given `allocation` must be a valid subscription allocation initialized
 *   with rmw_subscription_allocation_init() with a message type support that matches the
 *   one registered with `subscription` on creation.
 * \post Given `message_sequence` will remain a valid message sequence, and
 *   `message_info_sequence`, a valid message metadata sequence.
 *   Both will be left unchanged if this function fails early due to a logical error, such as
 *   an invalid argument, or in an unknown yet valid state if it fails due to a runtime error.
 *   Both will also be left unchanged if this function succeeds but `taken` is false.
 *
 * \param[in] subscription Subscription to take ROS message from.
 * \param[in] count Number of messages to attempt to take.
 * \param[out] message_sequence Sequence of type erase ROS messages to write to.
 *   Message sequence capacity has to be enough to hold all requested messages
 *   i.e. capacity has to be equal or greater than `count`.
 *   It does not have to match that of `message_info_sequence`.
 * \param[out] message_info_sequence Sequence of additional message metadata.
 *   Message info sequence capacity has to be enough to hold all requested messages
 *   metadata i.e. capacity has to be equal or greater than `count`.
 *   It does not have to match that of `message_sequence`.
 * \param[out] taken Number of messages actually taken from subscription.
 * \param[in] allocation Pre-allocated memory to use. May be NULL.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `subscription` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `message_sequence` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `message_info_sequence` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `taken` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `count` is 0, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `message_sequence` capacity is less than `count`, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `message_info_sequence` capacity is less than `count`, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `subscription` implementation
 *   identifier does not match this implementation, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_take_sequence(
  const rmw_subscription_t * subscription,
  size_t count,
  rmw_message_sequence_t * message_sequence,
  rmw_message_info_sequence_t * message_info_sequence,
  size_t * taken,
  rmw_subscription_allocation_t * allocation);

/// Take an incoming ROS message as a byte stream.
/**
 * Take a ROS message already received by the given subscription, removing it from internal queues.
 * This function will succeed even if no ROS message was received, but `taken` will be false.
 * Unlike rmw_take(), the ROS message is taken in its serialized form, as a byte stream.
 * If needed, this byte stream can then be deserialized into a ROS message with rmw_deserialize().
 *
 * \remarks The same ROS message, serialized or not, cannot be taken twice.
 *   Callers do not have to deal with duplicates.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par Runtime behavior
 *   To take a ROS message a byte stream is a synchronous operation.
 *   It is also non-blocking, to the extent it will not wait for new ROS messages to arrive,
 *   but it is not guaranteed to be lock-free.
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par Memory allocation
 *   It is implementation defined whether memory will be allocated on take or not.
 *   For instance, implementations may have to perform additional memory allocations
 *   when dealing with ROS messages that contain unbounded (dynamically-sized) fields
 *   i.e. these implementations may have to resize the given byte stream.
 *   A subscription allocation, if provided, may or may not be used.
 *   Check the implementation documentation to learn about memory allocation guarantees
 *   when taking serialized ROS messages with and without subscription allocations.
 * \par
 *   For ROS messages that only contain bounded (fixed-size) fields, callers can query
 *   their size using rmw_get_serialized_message_size() and resize `serialized_message`
 *   using rmw_serialized_message_resize() accordingly to prevent byte stream resizing
 *   on take.
 *   Nonetheless, byte stream resizing is not guaranteed to be the sole memory operation.
 *
 * \par Thread-safety
 *   Subscriptions are thread-safe objects, and so are all operations on them except for
 *   finalization.
 *   Therefore, it is safe to take from the same subscription concurrently.
 *   However, when taking serialized ROS messages:
 *   - Access to the given byte stream for serialized ROS messages is not synchronized.
 *     It is not safe to read or write `serialized_message` while
 *     rmw_take_serialized_message() uses it.
 *   - Access to given primitive data-type arguments is not synchronized.
 *     It is not safe to read or write `taken` while rmw_take_serialized_message() uses it.
 *   - Access to the given subscription allocation is not synchronized,
 *     unless specifically stated otherwise by the implementation.
 *     Thus, it is generally not safe to read or write `allocation` while
 *     rmw_take_serialized_message() uses it.
 *     Check the implementation documentation to learn about subscription allocations'
 *     thread-safety.
 *
 * \pre Given `subscription` must be a valid subscription, as returned by
 *   rmw_create_subscription().
 * \pre Given `serialized_message` must be a valid serialized message, initialized by
 *   rmw_serialized_message_init().
 * \pre If not NULL, given `allocation` must be a valid subscription allocation initialized
 *   with rmw_subscription_allocation_init() with a message type support that matches the
 *   one registered with `subscription` on creation.
 * \post Given `serialized_message` will remain a valid serialized message.
 *   It will be left unchanged if this function fails early due to a logical error,
 *   such as an invalid argument, or in an unknown yet valid state if it fails due to a
 *   runtime error.
 *   It will also be left unchanged if this function succeeds but `taken` is false.
 *
 * \param[in] subscription Subscription to take ROS message from.
 * \param[out] serialized_message Byte stream to write to.
 * \param[out] taken Boolean flag indicating if a ROS message was taken or not.
 * \param[in] allocation Pre-allocated memory to use. May be NULL.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `subscription` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `serialized_message` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `taken` is NULL, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `subscription` implementation
 *   identifier does not match this implementation, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_take_serialized_message(
  const rmw_subscription_t * subscription,
  rmw_serialized_message_t * serialized_message,
  bool * taken,
  rmw_subscription_allocation_t * allocation);

/// Take an incoming ROS message as a byte stream with its metadata.
/**
 * Same as rmw_take_serialized_message(), except it also takes ROS message metadata.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par Runtime behavior
 *   To take a ROS message a byte stream with its metadata is a synchronous operation.
 *   It is also non-blocking, to the extent it will not wait for new ROS messages to arrive,
 *   but it is not guaranteed to be lock-free.
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par Memory allocation
 *   It is implementation defined whether memory will be allocated on take or not.
 *   For instance, implementations may have to perform additional memory allocations
 *   when dealing with ROS messages that contain unbounded (dynamically-sized) fields
 *   i.e. these implementations may have to resize the given byte stream.
 *   A subscription allocation, if provided, may or may not be used.
 *   Check the implementation documentation to learn about memory allocation guarantees
 *   when taking serialized ROS messages with and without subscription allocations.
 * \par
 *   For ROS messages that only contain bounded (fixed-size) fields, callers can query
 *   their size using rmw_get_serialized_message_size() and resize `serialized_message`
 *   using rmw_serialized_message_resize() accordingly to prevent byte stream resizing
 *   on take.
 *   Nonetheless, byte stream resizing is not guaranteed to be the sole memory operation.
 *
 * \par Thread-safety
 *   Subscriptions are thread-safe objects, and so are all operations on them except for
 *   finalization.
 *   Therefore, it is safe to take from the same subscription concurrently.
 *   However, when taking serialized ROS messages with metadata:
 *   - Access to the given byte stream for serialized ROS messages is not synchronized.
 *     It is not safe to read or write `serialized_message` while
 *     rmw_take_serialized_message_with_info() uses it.
 *   - Access to the given ROS message metadata is not synchronized.
 *     It is not safe to read or write `message_info` while
 *     rmw_take_serialized_message_with_info() uses it.
 *   - Access to given primitive data-type arguments is not synchronized.
 *     It is not safe to read or write `taken` while rmw_take_serialized_message_with_info()
 *     uses it.
 *   - Access to the given subscription allocation is not synchronized,
 *     unless specifically stated otherwise by the implementation.
 *     Thus, it is generally not safe to read or write `allocation` while
 *     rmw_take_serialized_message_with_info() uses it.
 *     Check the implementation documentation to learn about subscription allocations'
 *     thread-safety.
 *
 * \pre Given `serialized_message` must be a valid serialized message, initialized by
 *   rmw_serialized_message_init().
 * \pre If not NULL, given `allocation` must be a valid subscription allocation initialized
 *   with rmw_subscription_allocation_init() with a message type support that matches the
 *   one registered with `subscription` on creation.
 * \post Given `serialized_message` will remain a valid serialized message, and `message_info`,
 *   valid message metadata.
 *   Both will be left unchanged if this function fails early due to a logical error,
 *   such as an invalid argument, or in an unknown yet valid state if it fails due to a
 *   runtime error.
 *   It will also be left unchanged if this function succeeds but `taken` is false.
 *
 * \param[in] subscription Subscription to take ROS message from.
 * \param[out] serialized_message Byte stream to write to.
 * \param[out] taken Boolean flag indicating if a ROS message was taken or not.
 * \param[out] message_info Taken ROS message metadata.
 * \param[in] allocation Pre-allocated memory to use. May be NULL.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `subscription` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `serialized_message` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `taken` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `message_info` is NULL, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `subscription` implementation
 *   identifier does not match this implementation, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_take_serialized_message_with_info(
  const rmw_subscription_t * subscription,
  rmw_serialized_message_t * serialized_message,
  bool * taken,
  rmw_message_info_t * message_info,
  rmw_subscription_allocation_t * allocation);

/// Take an incoming ROS message, loaned by the middleware.
/**
 * Take a ROS message already received by the given subscription, removing it from internal queues.
 * This function will succeed even if no ROS message was received, but `taken` will be false.
 * The loaned ROS message is owned by the middleware, which will keep it alive (i.e. in valid
 * memory space) until the caller returns it using rmw_return_loaned_message_from_subscription().
 *
 * \remarks The same ROS message, loaned or not, cannot be taken twice.
 *   Callers do not have to deal with duplicates.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par Runtime behavior
 *   To take a loaned ROS message is a synchronous operation.
 *   It is also non-blocking, to the extent it will not wait for new ROS messages to arrive
 *   nor for internal memory loaning pools, if any, to be replenished, but it is not
 *   guaranteed to be lock-free.
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par Memory allocation
 *   It is implementation defined whether memory will be allocated on take or not.
 *   For instance, implementations that deserialize ROS messages received over
 *   the wire may need to perform additional memory allocations when dealing with
 *   unbounded (dynamically-sized) fields.
 *   A subscription allocation, if provided, may or may not be used.
 *   Check the implementation documentation to learn about memory allocation
 *   guarantees when taking loaned ROS messages with and without subscription allocations.
 *
 * \par Thread-safety
 *   Subscriptions are thread-safe objects, and so are all operations on them except for
 *   finalization.
 *   Therefore, it is safe to take from the same subscription concurrently.
 *   However, when taking loaned ROS messages:
 *   - Access to given primitive data-type arguments is not synchronized.
 *     It is not safe to read or write `taken` nor `loaned_message`
 *     while rmw_take_loaned_message() uses them.
 *   - Access to the given subscription allocation is not synchronized,
 *     unless specifically stated otherwise by the implementation.
 *     Thus, it is generally not safe to read or write `allocation` while
 *     rmw_take_loaned_message() uses it.
 *     Check the implementation documentation to learn about subscription allocations'
 *     thread-safety.
 *
 * \pre Given `subscription` must be a valid subscription, as returned
 *   by rmw_create_subscription().
 * \pre If not NULL, given `allocation` must be a valid subscription allocation initialized
 *   with rmw_subscription_allocation_init() with a message type support that matches the
 *   one registered with `subscription` on creation.
 * \post Given `loaned_message` will remain unchanged, or point to a valid message if
 *   this function was successful and `taken` is true.
 *
 * \param[in] subscription Subscription to take ROS message from.
 * \param[inout] loaned_message Pointer to type erased ROS message taken
 *   and loaned by the middleware.
 * \param[out] taken Boolean flag indicating if a ROS message was taken or not.
 * \param[in] allocation Pre-allocated memory to use. May be NULL.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `subscription` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `loaned_message` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `*loaned_message` is not NULL (to prevent leaks), or
 * \return `RMW_RET_INVALID_ARGUMENT` if `taken` is NULL, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `subscription` implementation
 *   identifier does not match this implementation, or
 * \return `RMW_RET_UNSUPPORTED` if the implementation does not support loaned ROS messages, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_take_loaned_message(
  const rmw_subscription_t * subscription,
  void ** loaned_message,
  bool * taken,
  rmw_subscription_allocation_t * allocation);

/// Take a loaned message and with its additional message information.
/**
 * Same as rmw_take_loaned_message(), except it also takes ROS message metadata.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par Runtime behavior
 *   To take a loaned ROS message with its metadata is a synchronous operation.
 *   It is also non-blocking, to the extent it will not wait for new ROS messages to arrive
 *   nor for internal memory loaning pools, if any, to be replenished, but it is not
 *   guaranteed to be lock-free.
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par Memory allocation
 *   It is implementation defined whether memory will be allocated on take or not.
 *   For instance, implementations that deserialize ROS messages received over
 *   the wire may need to perform additional memory allocations when dealing with
 *   unbounded (dynamically-sized) fields.
 *   A subscription allocation, if provided, may or may not be used.
 *   Check the implementation documentation to learn about memory allocation
 *   guarantees when taking loaned ROS messages with and without subscription allocations.
 *
 * \par Thread-safety
 *   Subscriptions are thread-safe objects, and so are all operations on them except for
 *   finalization.
 *   Therefore, it is safe to take from the same subscription concurrently.
 *   However, when taking loaned ROS messages with metadata:
 *   - Access to given primitive data-type arguments is not synchronized.
 *     It is not safe to read or write `taken` nor `loaned_message`
 *     while rmw_take_loaned_message_with_info() uses them.
 *   - Access to the given ROS message metadata is not synchronized.
 *     It is not safe to read or write `message_info` while
 *     rmw_take_loaned_message_with_info() uses it.
 *   - Access to the given subscription allocation is not synchronized,
 *     unless specifically stated otherwise by the implementation.
 *     Thus, it is generally not safe to read or write `allocation` while
 *     rmw_take_loaned_message_with_info() uses it.
 *     Check the implementation documentation to learn about subscription allocations'
 *     thread-safety.
 *
 * \pre Given `subscription` must be a valid subscription, as returned
 *   by rmw_create_subscription().
 * \pre If not NULL, given `allocation` must be a valid subscription allocation initialized
 *   with rmw_subscription_allocation_init() with a message type support that matches the
 *   one registered with `subscription` on creation.
 * \post Given `loaned_message` will remain unchanged, or point to a valid message if
 *   this function was successful and `taken` is true.
 * \post Given `message_info` will remain valid message metadata.
 *   It will be left unchanged if this function fails early due to a logical error,
 *   such as an invalid argument, or in an unknown yet valid state if it fails due to a
 *   runtime error.
 *   It will also be left unchanged if this function succeeds but `taken` is false.
 *
 * \param[in] subscription Subscription to take ROS message from.
 * \param[inout] loaned_message Pointer to type erased ROS message taken
 *   and loaned by the middleware.
 * \param[out] taken Boolean flag indicating if a ROS message was taken or not.
 * \param[out] message_info Taken ROS message metadata.
 * \param[in] allocation Pre-allocated memory to use. May be NULL.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `subscription` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `loaned_message` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `*loaned_message` is not NULL to prevent leaks, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `taken` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `message_info` is NULL, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `subscription` implementation
 *   identifier does not match this implementation, or
 * \return `RMW_RET_UNSUPPORTED` if the implementation does not support loaned ROS messages, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_take_loaned_message_with_info(
  const rmw_subscription_t * subscription,
  void ** loaned_message,
  bool * taken,
  rmw_message_info_t * message_info,
  rmw_subscription_allocation_t * allocation);

/// Return a loaned ROS message previously taken from a subscription.
/**
 * Tells the middleware that previously loaned ROS message is no longer needed by the caller.
 * If this function fails early due to a logical error, such as an invalid argument,
 * the loaned ROS message will be left unchanged.
 * Otherwise, ownership of the ROS message will be given back to the middleware.
 * It is up to the middleware what will be made of the returned ROS message.
 * It is undefined behavior to use a loaned ROS message after returning it.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par Runtime behavior
 *   To return a loaned ROS message is a synchronous operation.
 *   It is also non-blocking, but it is not guaranteed to be lock-free.
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par Thread-safety
 *   Subscriptions are thread-safe objects, and so are all operations on them except for
 *   finalization.
 *   Therefore, it is safe to return loaned ROS messages to the same subscription concurrently.
 *   However, since ownership of the loaned ROS message is given back to middleware and this
 *   transfer is not synchronized, it is not safe to return the same loaned ROS message
 *   concurrently.
 *
 * \pre Given `subscription` must be a valid subscription, as returned
 *   by rmw_create_subscription().
 * \pre Given `loaned_message` must be a loaned ROS message, previously taken from
 *   `subscription` using rmw_take_loaned_message() or rmw_take_loaned_message_with_info().
 *
 * \param[in] subscription Subscription the ROS message was taken and loaned from.
 * \param[in] loaned_message Loaned type erased ROS message to be returned to the middleware.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `subscription` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `loaned_message` is NULL, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `subscription` implementation
 *   identifier does not match this implementation, or
 * \return `RMW_RET_UNSUPPORTED` if the implementation does not support loaned ROS messages, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_return_loaned_message_from_subscription(
  const rmw_subscription_t * subscription,
  void * loaned_message);

/// Create an rmw client to communicate with the specified service
/**
 * \param[in] node Handle to node with which to register this client
 * \param[in] type_support The type_support of this rosidl service
 * \param[in] service_name The name of the ROS 2 service to connect with
 * \param[in] qos_policies The QoS profile policies to utilize for this connection
 * \return The initialized client if successful, `nullptr` if not
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_client_t *
rmw_create_client(
  const rmw_node_t * node,
  const rosidl_service_type_support_t * type_support,
  const char * service_name,
  const rmw_qos_profile_t * qos_policies);

/// Destroy and unregister a service client
/**
 * \param[in] node The associated node whose client will be destroyed
 * \param[in] client The service client to destroy
 * \return RMW_RET_OK if successful, otherwise an appropriate error code
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_destroy_client(rmw_node_t * node, rmw_client_t * client);

/// Send a ROS service request.
/**
 * Send a ROS service request to one or more service servers, with matching QoS policies,
 * using the given client.
 *
 * \note It is implementation defined how many service requests may get the same request,
 *   considering there may be more than one service server for the same client in the ROS graph.
 *
 * On success, this function will return a sequence number.
 * It is up to callers to keep that sequence number to pair the ROS service request just sent
 * with future ROS service responses, to be taken using rmw_take_response().
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par Runtime behavior
 *   It is implementation defined whether to send a ROS service request is a
 *   synchronous or asynchronous, blocking or non-blocking operation.
 *   However, asynchronous implementations are not allowed to access the
 *   given ROS service request after this function returns.
 *   Check the implementation documentation to learn about request behavior.
 *
 * \par Memory allocation
 *   It is implementation defined whether memory will be allocated on send or not.
 *   For instance, implementations that serialize ROS service requests may need to
 *   perform additional memory allocations when dealing with unbounded (dynamically-sized)
 *   fields.
 *
 * \par Thread-safety
 *   Service clients are thread-safe objects, and so are all operations on them except for
 *   finalization.
 *   Therefore, it is safe to send requests using the same service client concurrently.
 *   However:
 *   - Access to the given ROS service request is read-only but it is not synchronized.
 *     Concurrent `ros_request` reads are safe, but concurrent reads and writes are not.
 *   - Access to given primitive data-type arguments is not synchronized.
 *     It is not safe to read or write `sequence_id` while rmw_send_request() uses it.
 *
 * \pre Given `client` must be a valid client, as returned by rmw_create_client().
 * \pre Given `ros_request` must be a valid service request, whose type matches the
 *   service type support registered with the `client` on creation.
 *
 * \param[in] client Service client to send a request with.
 * \param[in] ros_request ROS service request to be sent.
 * \param[out] sequence_id Sequence number for the `ros_request` just sent
 *   i.e. a unique identification number for it, populated on success.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `client` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `ros_request` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `sequence_id` is NULL, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `client`
 *   implementation identifier does not match this implementation, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_send_request(
  const rmw_client_t * client,
  const void * ros_request,
  int64_t * sequence_id);

/// Take an incoming ROS service response.
/**
 * Take a ROS service response already received by the given service server, removing
 * it from internal queues.
 * The response header (i.e. its metadata), including, but not limited to, writer guid
 * and sequence number, is also retrieved.
 * Both writer guid and sequence number allow callers to pair, potentially for each
 * remote service server, a ROS service response with its corresponding ROS service
 * request, previously sent using rmw_send_request().
 *
 * \note It is implementation defined how many responses a given request may get,
 *   considering there may be more than one service server for the same client
 *   in the ROS graph.
 *
 * This function will succeed even if no ROS service request was received,
 * but `taken` will be false.
 *
 * \remarks The same ROS service response cannot be taken twice.
 *   Callers do not have to deal with duplicates.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par Runtime behavior
 *   To take a ROS service response is a synchronous operation.
 *   It is also non-blocking, to the extent it will not wait for new ROS service responses
 *   to arrive, but it is not guaranteed to be lock-free.
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par Memory allocation
 *   It is implementation defined whether memory will be allocated on take or not.
 *   For instance, implementations that deserialize ROS service responses received over
 *   the wire may need to perform additional memory allocations when dealing with
 *   unbounded (dynamically-sized) fields.
 *
 * \par Thread-safety
 *   Service clients are thread-safe objects, and so are all operations on them except for
 *   finalization.
 *   Therefore, it is safe to take responses from the same service client concurrently.
 *   However:
 *   - Access to the given ROS service response is not synchronized.
 *     It is not safe to read or write `ros_response` while rmw_take_request() uses it.
 *   - Access to the given ROS service response header is not synchronized.
 *     It is not safe to read or write `response_header` while rmw_take_response() uses it.
 *   - Access to given primitive data-type arguments is not synchronized.
 *     It is not safe to read or write `taken` while rmw_take_response() uses it.
 *
 * \pre Given `client` must be a valid client, as returned by rmw_create_client().
 * \pre Given `ros_response` must be a valid service response, whose type matches the
 *   service type support registered with the `client` on creation.
 * \post Given `ros_response` will remain a valid service response.
 *   It will be left unchanged if this function fails early due to a logical error, such as an
 *   invalid argument, or in an unknown yet valid state if it fails due to a runtime error.
 *   It will also be left unchanged if this function succeeds but `taken` is false.
 *
 * \param[in] client Service client to take response from.
 * \param[out] response_header Service response header to write to.
 * \param[out] ros_request Type erased ROS service response to write to.
 * \param[out] taken Boolean flag indicating if a ROS service response was taken or not.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `client` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `response_header` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `ros_response` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `taken` is NULL, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `client`
 *   implementation identifier does not match this implementation, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_take_response(
  const rmw_client_t * client,
  rmw_service_info_t * request_header,
  void * ros_response,
  bool * taken);

/// Create an rmw service server that responds to requests
/**
 * \param[in] node The node that responds the service requests
 * \param[in] type_support The type support description of this service
 * \param[in] service_name The name of this service advertised across the ROS graph
 * \param[in] qos_policies The QoS profile policies to utilize for connections
 * \return The created service object if successful, otherwise a nullptr
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_service_t *
rmw_create_service(
  const rmw_node_t * node,
  const rosidl_service_type_support_t * type_support,
  const char * service_name,
  const rmw_qos_profile_t * qos_policies);

/// Destroy and unregister the service from this node
/**
 * \param[in] node The node that owns the service that is being destroyed
 * \param[in] service Pointer to the service type created in `rmw_create_service`
 * \return RMW_RET_OK if successful, otherwise an appropriate error code
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_destroy_service(rmw_node_t * node, rmw_service_t * service);

/// Take an incoming ROS service request.
/**
 * Take a ROS service request already received by the given service server, removing
 * it from internal queues.
 * The request header (i.e. its metadata), including, but not limited to, writer guid
 * and sequence number, is also retrieved.
 * Both writer guid and sequence number allow callers to pair, for each remote service
 * client, a ROS service request with its corresponding ROS service response, to be later
 * sent using rmw_send_response().
 *
 * This function will succeed even if no ROS service request was received,
 * but `taken` will be false.
 *
 * \remarks The same ROS service request cannot be taken twice.
 *   Callers do not have to deal with duplicates.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par Runtime behavior
 *   To take a ROS service request is a synchronous operation.
 *   It is also non-blocking, to the extent it will not wait for new ROS service requests
 *   to arrive, but it is not guaranteed to be lock-free.
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par Memory allocation
 *   It is implementation defined whether memory will be allocated on take or not.
 *   For instance, implementations that deserialize ROS service requests received over
 *   the wire may need to perform additional memory allocations when dealing with
 *   unbounded (dynamically-sized) fields.
 *
 * \par Thread-safety
 *   Service servers are thread-safe objects, and so are all operations on them except for
 *   finalization.
 *   Therefore, it is safe to take requests from the same service server concurrently.
 *   However:
 *   - Access to the given ROS service request is not synchronized.
 *     It is not safe to read or write `ros_request` while rmw_take_request() uses it.
 *   - Access to the given ROS service request header is not synchronized.
 *     It is not safe to read or write `request_header` while rmw_take_request() uses it.
 *   - Access to given primitive data-type arguments is not synchronized.
 *     It is not safe to read or write `taken` while rmw_take_request() uses it.
 *
 * \pre Given `service` must be a valid service, as returned by rmw_create_service().
 * \pre Given `ros_request` must be a valid service request, whose type matches the
 *   service type support registered with the `service` on creation.
 * \post Given `ros_request` will remain a valid service request.
 *   It will be left unchanged if this function fails early due to a logical error, such as an
 *   invalid argument, or in an unknown yet valid state if it fails due to a runtime error.
 *   It will also be left unchanged if this function succeeds but `taken` is false.
 *
 * \param[in] service Service server to take request from.
 * \param[out] request_header Service request header to write to.
 * \param[out] ros_request Type erased ROS service request to write to.
 * \param[out] taken Boolean flag indicating if a ROS service request was taken or not.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `service` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `request_header` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `ros_request` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `taken` is NULL, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `service`
 *   implementation identifier does not match this implementation, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_take_request(
  const rmw_service_t * service,
  rmw_service_info_t * request_header,
  void * ros_request,
  bool * taken);

/// Send a ROS service response.
/**
 * Send a ROS service response the service client, with matching QoS policies,
 * from which the previously taken ROS service request was originally sent.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Maybe
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par Runtime behavior
 *   It is implementation defined whether to send a ROS service response is a
 *   synchronous or asynchronous, blocking or non-blocking operation.
 *   However, asynchronous implementations are not allowed to access the
 *   given ROS service request after this function returns.
 *   Check the implementation documentation to learn about request behavior.
 *
 * \par Memory allocation
 *   It is implementation defined whether memory will be allocated on send or not.
 *   For instance, implementations that serialize ROS service responses may need to
 *   perform additional memory allocations when dealing with unbounded (dynamically-sized)
 *   fields.
 *
 * \par Thread-safety
 *   Service servers are thread-safe objects, and so are all operations on them except for
 *   finalization.
 *   Therefore, it is safe to send responses using the same service server concurrently.
 *   However:
 *   - Access to the given ROS service request header is read-only but it is not synchronized.
 *     Concurrent `request_header` reads are safe, but concurrent reads and writes are not.
 *   - Access to the given ROS service response is read-only but it is not synchronized.
 *     Concurrent `ros_request` reads are safe, but concurrent reads and writes are not.
 *
 * \pre Given `service` must be a valid service server, as returned by rmw_create_service().
 * \pre Given `request_header` must be the one previously taken along with the ROS service
 *   request to which we reply.
 * \pre Given `ros_response` must be a valid service response, whose type matches the
 *   service type support registered with the `service` on creation.
 *
 * \param[in] client Service server to send a response with.
 * \param[in] request_header Service response header, same as the one taken
 *   with the corresponding ROS service request.
 * \param[in] ros_response ROS service response to be sent.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `service` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `request_header` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `ros_response` is NULL, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `service`
 *   implementation identifier does not match this implementation, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_send_response(
  const rmw_service_t * service,
  rmw_request_id_t * request_header,
  void * ros_response);

// TODO(wjwwood): refactor this API to return a return code when updated to use an allocator
/// Create a guard condition and return a handle to that guard condition.
/**
 * This function can fail, and therefore return `NULL`, if:
 *   - context is `NULL`
 *   - context is invalid
 *   - memory allocation fails during guard condition creation
 *   - an unspecified error occurs
 *
 * The context must be non-null and valid, i.e. it has been initialized
 * by `rmw_init()` and has not been finalized by `rmw_shutdown()`.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | No
 * Uses Atomics       | No [1]
 * Lock-Free          | No [1]
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * This should be defined by the rmw implementation.
 *
 * \param[in] context init context that this node should be associated with
 * \return rmw guard condition handle or `NULL` if there was an error
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_guard_condition_t *
rmw_create_guard_condition(rmw_context_t * context);

/// Finalize a given guard condition handle, reclaim the resources, and deallocate the handle.
/**
 * \param[in] guard_condition the guard condition handle to be destroyed
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if guard_condition is null, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
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
 * This function can fail, and therefore return `NULL`, if:
 *   - context is `NULL`
 *   - context is invalid
 *   - memory allocation fails during wait set creation
 *   - an unspecified error occurs
 *
 * If `max_conditions` is `0`, the wait set can store an unbounded number of
 * conditions to wait on.
 * If `max_conditions` is greater than `0`, the number of conditions that can
 * be attached to the wait set is bounded at `max_conditions`.
 *
 * \param[in] context init context that this node should be associated with
 * \param[in] max_conditions
 *   The maximum number of conditions that can be attached to the wait set.
 * \return A pointer to the created wait set, `NULL` if an error occurred.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_wait_set_t *
rmw_create_wait_set(rmw_context_t * context, size_t max_conditions);

/// Destroy and free memory of this wait_set
/**
 * \param[in] wait_set The wait_set object to destroy
 * \return RMW_RET_OK if successful, otherwise an appropriate error code
 */
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
 * \param[in] subscriptions Array of subscriptions to wait on
 * \param[in] guard_conditions Array of guard conditions to wait on
 * \param[in] services Array of services to wait on
 * \param[in] clients Array of clients to wait on
 * \param[in] wait_set Storage for the wait set
 * \param[in] wait_timeout
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
  rmw_events_t * events,
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

/// Return a list of node name and namespaces discovered via a node with its enclave.
/**
 * Similar to \ref rmw_get_node_names, but it also provides the enclave name.
 *
 * \param[in] node the handle to the node being used to query the ROS graph
 * \param[out] node_names a list of discovered node names
 * \param[out] node_namespaces a list of discovered node namespaces
 * \param[out] enclaves list of discovered nodes' enclave names
 * \return `RMW_RET_OK` if node the query was made successfully, or
 * \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_get_node_names_with_enclaves(
  const rmw_node_t * node,
  rcutils_string_array_t * node_names,
  rcutils_string_array_t * node_namespaces,
  rcutils_string_array_t * enclaves);

/// Count the number of publishers matching a topic name
/**
* \param[in] node rmw node connected to the ROS graph
* \param[in] topic_name The name of the topic to match under possible prefixes
* \param[out] count The number of publishers matching the topic name
* \return RMW_RET_OK if successful, otherwise an appropriate error code
*/
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_count_publishers(
  const rmw_node_t * node,
  const char * topic_name,
  size_t * count);

/// Count the number of subscribers matching a topic name
/**
 * \param[in] node rmw node connected to the ROS graph
 * \param[in] topic_name The name of the topic to match under possible prefixes
 * \param[out] count The number of subscribers matching the topic name
 * \return RMW_RET_OK if successful, otherwise an appropriate error code
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_count_subscribers(
  const rmw_node_t * node,
  const char * topic_name,
  size_t * count);

/// Get the unique identifier (gid) of a publisher.
/**
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par Thread-safety
 *   Publishers are thread-safe objects, and so are all operations on them except for
 *   finalization.
 *   Therefore, it is safe to get the unique identifier from the same publisher concurrently.
 *   However, access to the gid is not synchronized.
 *   It is not safe to read or write `gid` while rmw_get_gid_for_publisher() uses it.
 *
 * \pre Given `publisher` must be a valid subscription, as returned by rmw_create_publisher().
 *
 * \param[in] publisher Publisher to get a gid from.
 * \param[out] gid Publisher's unique identifier, populated on success
 *   but left unchanged on failure.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `publisher` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `gid` is NULL, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `publisher` implementation
 *   identifier does not match this implementation, or
 * \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_get_gid_for_publisher(const rmw_publisher_t * publisher, rmw_gid_t * gid);

/// Check if two unique identifiers (gids) are equal.
/**
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 *
 * <i>[1] implementation defined, check implementation documentation.</i>
 *
 * \par Thread-safety
 *   Unique identifier comparison is a reentrant function, but:
 *   - Access to both gids is read-only but it is not synchronized.
 *     Concurrent `gid1` and `gid2` reads are safe, but concurrent reads and writes are not.
 *   - Access to primitive data-type arguments is not synchronized.
 *     It is not safe to read or write `result` while rmw_compare_gids_equal() uses it.
 *
 * \param[in] gid1 First unique identifier to compare.
 * \param[in] gid2 Second unique identifier to compare.
 * \param[out] bool true if both gids are equal, false otherwise.
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `gid1` or `gid2` is NULL, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the implementation
 *   identifier of `gid1` or `gid2` does not match this implementation, or
 * \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
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

/// Set the current log severity
/**
 * \param[in] severity The log severity to set
 * \return RMW_RET_OK if successful, otherwise an appropriate error code
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_set_log_severity(rmw_log_severity_t severity);

#ifdef __cplusplus
}
#endif

#endif  // RMW__RMW_H_

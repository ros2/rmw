// Copyright 2025 Minju Lee (이민주).
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

#ifndef RMW__GET_SERVICE_ENDPOINT_INFO_H_
#define RMW__GET_SERVICE_ENDPOINT_INFO_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "rmw/service_endpoint_info_array.h"
#include "rmw/visibility_control.h"
/// Retrieve endpoint information for each known client of a given service.
/**
 * This function returns an array of endpoint information for each client
 * of a given service, as discovered so far by the given node.
 * Endpoint information includes the client's node name and namespace,
 * the associated service type, the client's gid, and the client QoS profile.
 * Names of non-existent services are allowed, in which case an empty array will be returned.
 *
 * Depending on the RMW in use, discovery may be asynchronous.  Therefore, creating a client
 * and then calling this API may not show the newly created client immediately.
 *
 * \par Internal behavior
 * The internal representation of services depends on the underlying RMW implementation.
 * For example, in DDS-based RMWs, services are implemented using two topics:
 * one for requests and one for responses. The client and server each create both
 * a DataReader and a DataWriter — the client writes requests and reads responses,
 * while the server reads requests and writes responses.
 * In this case, all fields of `rmw_topic_endpoint_info_t` can be queried from the graph cache.
 *
 * Other middleware implementations, such as Zenoh, may offer native support for services,
 * without using topics internally. This API is designed to support both models and
 * can return endpoint information regardless of the underlying implementation strategy.
 *
 * \par QoS that are correctly read
 * The QoS profiles returned might have some invalid fields.
 * The rmw implementation must set the invalid fields to `RMW_QOS_POLICY_*_UNKNOWN`.
 * For DDS based implementations, the only QoS policies that are guaranteed to be shared
 * during discovery are the ones that participate in endpoint matching.
 * From the current QoS settings available, the only ones not shared by DDS based
 * implementations are `history` and `history_depth`.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \par Runtime behavior
 *   To query the ROS graph is a synchronous operation.
 *   It is also non-blocking, but it is not guaranteed to be lock-free.
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par Thread-safety
 *   Nodes are thread-safe objects, and so are all operations on them except for finalization.
 *   Therefore, it is safe to query the ROS graph using the same node concurrently.
 *   However, when querying service names and types:
 *   - Access to the array of service endpoint information is not synchronized.
 *     It is not safe to read or write `clients_info`
 *     while rmw_get_clients_info_by_service() uses it.
 *   - Access to C-style string arguments is read-only but it is not synchronized.
 *     Concurrent `service_name` reads are safe, but concurrent reads and writes are not.
 *   - The default allocators are thread-safe objects, but any custom `allocator` may not be.
 *     Check your allocator documentation for further reference.
 *
 * \pre Given `node` must be a valid node handle, as returned by rmw_create_node().
 * \pre Given `clients_info` must be a zero-initialized array of endpoints' information,
 *   as returned by rmw_get_zero_initialized_service_endpoint_info_array().
 *
 * \param[in] node Node to query the ROS graph.
 * \param[in] allocator Allocator to be used when populating the `clients_info` array.
 * \param[in] service_name Name of the service for client lookup, often a fully qualified
 *   service name but not necessarily (see rmw_create_client()).
 * \param[in] no_mangle Whether to mangle the service name before client lookup or not.
 *  Note: DDS-based RMWs do not support `no_mangle = true` because services are implemented
 *  as topics with mangled names. Use `rmw_get_publishers_info_by_topic` or
 * `rmw_get_subscriptions_info_by_topic` for unmangled topic queries in such cases.
 *  Other RMWs (e.g., Zenoh) may support `no_mangle = true` if they natively handle services
 *  without topic-based mangling.
 * \param[out] clients_info Array of client information, populated on success,
 *   left unchanged on failure.
 *   If populated, it is up to the caller to finalize this array later on,
 *   using rmw_service_endpoint_info_array_fini().
 *   QoS Profiles in the info array will use RMW_DURATION_INFINITE for infinite durations,
 *   avoiding exposing any implementation-specific values.
 * \return `RMW_RET_OK` if the query was successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `node` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `allocator` is not valid,
 *   by rcutils_allocator_is_valid() definition, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `service_name` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `clients_info` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `clients_info` is not a
 *   zero-initialized array, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `node` implementation
 *   identifier does not match this implementation, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_get_clients_info_by_service(
  const rmw_node_t * node,
  rcutils_allocator_t * allocator,
  const char * service_name,
  bool no_mangle,
  rmw_service_endpoint_info_array_t * clients_info);

/// Retrieve endpoint information for each known server of a given server.
/**
 * This function returns an array of endpoint information for each server
 * of a given service, as discovered so far by the given node.
 * Endpoint information includes the server's node name and namespace,
 * the associated service type, the server's gid, and the server QoS profile.
 * Names of non-existent services are allowed, in which case an empty array will be returned.
 *
 * Depending on the RMW in use, discovery may be asynchronous.  Therefore, creating a server
 * and then calling this API may not show the newly created server immediately.
 *
 * \par Internal behavior
 * The internal representation of services depends on the underlying RMW implementation.
 * For example, in DDS-based RMWs, services are implemented using two topics:
 * one for requests and one for responses. The client and server each create both
 * a DataReader and a DataWriter — the client writes requests and reads responses,
 * while the server reads requests and writes responses.
 * In this case, all fields of `rmw_topic_endpoint_info_t` can be queried from the graph cache.
 *
 * Other middleware implementations, such as Zenoh, may offer native support for services,
 * without using topics internally. This API is designed to support both models and
 * can return endpoint information regardless of the underlying implementation strategy.
 *
 * \par QoS that are correctly read
 * Not all QoS may be read correctly, \sa rmw_get_publishers_info_by_topic() for more details.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | Yes
 * Uses Atomics       | Maybe [1]
 * Lock-Free          | Maybe [1]
 * <i>[1] rmw implementation defined, check the implementation documentation</i>
 *
 * \par Runtime behavior
 *   To query the ROS graph is a synchronous operation.
 *   It is also non-blocking, but it is not guaranteed to be lock-free.
 *   Generally speaking, implementations may synchronize access to internal resources using
 *   locks but are not allowed to wait for events with no guaranteed time bound (barring
 *   the effects of starvation due to OS scheduling).
 *
 * \par Thread-safety
 *   Nodes are thread-safe objects, and so are all operations on them except for finalization.
 *   Therefore, it is safe to query the ROS graph using the same node concurrently.
 *   However, when querying service names and types:
 *   - Access to the array of service endpoint information is not synchronized.
 *     It is not safe to read or write `servers_info`
 *     while rmw_get_servers_info_by_service() uses it.
 *   - Access to C-style string arguments is read-only but it is not synchronized.
 *     Concurrent `service_name` reads are safe, but concurrent reads and writes are not.
 *   - The default allocators are thread-safe objects, but any custom `allocator` may not be.
 *     Check your allocator documentation for further reference.
 *
 * \pre Given `node` must be a valid node handle, as returned by rmw_create_node().
 * \pre Given `servers_info` must be a zero-initialized array of endpoints' information,
 *   as returned by rmw_get_zero_initialized_service_endpoint_info_array().
 *
 * \param[in] node Node to query the ROS graph.
 * \param[in] allocator Allocator to be used when populating the `servers_info` array.
 * \param[in] service_name Name of the service for server lookup, often a fully qualified
 *   service name but not necessarily (see rmw_create_service()).
 * \param[in] no_mangle Whether to mangle the service name before client lookup or not.
 *  Note: DDS-based RMWs do not support `no_mangle = true` because services are implemented
 *  as topics with mangled names. Use `rmw_get_publishers_info_by_topic` or
 * `rmw_get_subscriptions_info_by_topic` for unmangled topic queries in such cases.
 *  Other RMWs (e.g., Zenoh) may support `no_mangle = true` if they natively handle services
 *  without topic-based mangling.
 * \param[out] servers_info Array of server information, populated on success,
 *   left unchanged on failure.
 *   If populated, it is up to the caller to finalize this array later on,
 *   using rmw_service_endpoint_info_array_fini().
 *   QoS Profiles in the info array will use RMW_DURATION_INFINITE for infinite durations,
 *   avoiding exposing any implementation-specific values.
 * \return `RMW_RET_OK` if the query was successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `node` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `allocator` is not valid,
 *   by rcutils_allocator_is_valid() definition, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `service_name` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `servers_info` is NULL, or
 * \return `RMW_RET_INVALID_ARGUMENT` if `servers_info` is not a
 *   zero-initialized array, or
 * \return `RMW_RET_INCORRECT_RMW_IMPLEMENTATION` if the `node` implementation
 *   identifier does not match this implementation, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_get_servers_info_by_service(
  const rmw_node_t * node,
  rcutils_allocator_t * allocator,
  const char * service_name,
  bool no_mangle,
  rmw_service_endpoint_info_array_t * servers_info);

#ifdef __cplusplus
}
#endif

#endif  // RMW__GET_SERVICE_ENDPOINT_INFO_H_

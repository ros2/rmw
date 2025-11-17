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

#ifndef RMW__SERVICE_ENDPOINT_INFO_H_
#define RMW__SERVICE_ENDPOINT_INFO_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "rcutils/allocator.h"
#include "rosidl_runtime_c/type_hash.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"

/// A data structure that encapsulates the node name, node namespace,
/// service_type, service_type_hash, endpoint_count, gids and qos_profiles of clients and servers.
/// for a service.
typedef struct RMW_PUBLIC_TYPE rmw_service_endpoint_info_s
{
  /// Name of the node
  const char * node_name;
  /// Namespace of the node
  const char * node_namespace;
  /// The associated service type's name
  const char * service_type;
  /// Hashed value for service type's description
  rosidl_type_hash_t service_type_hash;
  /// The endpoint type
  rmw_endpoint_type_t endpoint_type;
  /// The `endpoint_count` value is determined as follows:
  /// - 1 if the middleware explicitly supports services (e.g., Zenoh).
  /// - 2 if request/response are represented as separate reader/writer topics (e.g., DDS).
  size_t endpoint_count;
  /// The GIDs of the endpoint(s). The array size is `endpoint_count`.
  /// If the middleware represents the request and response as separate topics,
  /// the GIDs should be provided sequentially in the order: reader, writer.
  uint8_t (*endpoint_gids)[RMW_GID_STORAGE_SIZE];
  /// QoS profiles of the endpoint(s). The array size is `endpoint_count`.
  /// If the middleware represents the request and response as separate topics,
  /// the QoS profiles should be provided sequentially in the order: reader, writer
  rmw_qos_profile_t * qos_profiles;
} rmw_service_endpoint_info_t;

/// Return zero initialized service endpoint info data structure.
/**
 * Endpoint type will be invalid.
 * Endpoint QoS profile will be the system default.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_service_endpoint_info_t
rmw_get_zero_initialized_service_endpoint_info(void);

/// Finalize a service endpoint info data structure.
/**
 * Deallocates all allocated members of the given data structure,
 * and then zero initializes it.
 * If a logical error, such as `RMW_RET_INVALID_ARGUMENT`, ensues, this function
 * will return early, leaving the given data structure unchanged.
 * Otherwise, it will proceed despite errors.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | No
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * \par Thread-safety
 *   Finalization is a reentrant procedure, but:
 *   - Access to the service endpoint info data structure is not synchronized.
 *     It is not safe to read or write `service_endpoint` during finalization.
 *   - The default allocators are thread-safe objects, but any custom `allocator` may not be.
 *     Check your allocator documentation for further reference.
 *
 * \param[inout] service_endpoint_info Data structure to be finalized.
 * \param[in] allocator Allocator used to populate the given `service_endpoint_info`.
 * \returns `RMW_RET_OK` if successful, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `service_endpoint_info` is NULL, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `allocator` is invalid,
 *   by rcutils_allocator_is_valid() definition, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 * \remark This function sets the RMW error state on failure.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_service_endpoint_info_fini(
  rmw_service_endpoint_info_t * service_endpoint_info,
  rcutils_allocator_t * allocator);

/// Set the service type in the given service endpoint info data structure.
/**
 * Allocates memory and copies the value of the `service_type`
 * argument to set the data structure's `service_type` member.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | No
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * \par Thread-safety
 *   Setting a member is a reentrant procedure, but:
 *   - Access to the service endpoint info data structure is not synchronized.
 *     It is not safe to read or write the `service_type` member of the given `service_endpoint`
 *     while setting it.
 *   - Access to C-style string arguments is read-only but it is not synchronized.
 *     Concurrent `service_type` reads are safe, but concurrent reads and writes are not.
 *   - The default allocators are thread-safe objects, but any custom `allocator` may not be.
 *     Check your allocator documentation for further reference.
 *
 * \pre Given `service_type` is a valid C-style string i.e. NULL terminated.
 *
 * \param[inout] service_endpoint_info Data structure to be populated.
 * \param[in] service_type Type name to be set.
 * \param[in] allocator Allocator to be used.
 * \returns `RMW_RET_OK` if successful, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `service_endpoint_info` is NULL, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `service_type` is NULL, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `allocator` is NULL, or
 * \returns `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 * \remark This function sets the RMW error state on failure.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_service_endpoint_info_set_service_type(
  rmw_service_endpoint_info_t * service_endpoint_info,
  const char * service_type,
  rcutils_allocator_t * allocator);

/// Set the service type hash in the given service endpoint info data structure.
/**
 * Assigns the value of the `service_type_hash` argument to the data structure's
 * `service_type_hash` member.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | No
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * \par Thread-safety
 *   Setting a member is a reentrant procedure, but:
 *   - Access to the service endpoint info data structure is not synchronized.
 *     It is not safe to read or write the `service_type_hash` member of the given `service_endpoint`
 *     while setting it.
 *     Concurrent `service_type_hash` reads are safe, but concurrent reads and writes are not.
 *
 * \param[inout] service_endpoint_info Data structure to be populated.
 * \param[in] type_hash Service type hash to be copied.
 * \returns `RMW_RET_OK` if successful, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `service_endpoint_info` is NULL, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `service_type_hash` is NULL, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 * \remark This function sets the RMW error state on failure.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_service_endpoint_info_set_service_type_hash(
  rmw_service_endpoint_info_t * service_endpoint_info,
  const rosidl_type_hash_t * type_hash);

/// Set the node name in the given service endpoint info data structure.
/**
 * Allocates memory and copies the value of the `node_name`
 * argument to set the data structure's `node_name` member.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | No
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * \par Thread-safety
 *   Setting a member is a reentrant procedure, but:
 *   - Access to the service endpoint info data structure is not synchronized.
 *     It is not safe to read or write the `node_name` member of the given `service_endpoint`
 *     while setting it.
 *   - Access to C-style string arguments is read-only but it is not synchronized.
 *     Concurrent `node_name` reads are safe, but concurrent reads and writes are not.
 *   - The default allocators are thread-safe objects, but any custom `allocator` may not be.
 *     Check your allocator documentation for further reference.
 *
 * \pre Given `node_name` is a valid C-style string i.e. NULL terminated.
 *
 * \param[inout] service_endpoint_info Data structure to be populated.
 * \param[in] node_name Node name to be set.
 * \param[in] allocator Allocator to be used.
 * \returns `RMW_RET_OK` if successful, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `service_endpoint_info` is NULL, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `node_name` is NULL, or
 * \returns `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 * \remark This function sets the RMW error state on failure.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_service_endpoint_info_set_node_name(
  rmw_service_endpoint_info_t * service_endpoint_info,
  const char * node_name,
  rcutils_allocator_t * allocator);

/// Set the node namespace in the given service endpoint info data structure.
/**
 * Allocates memory and copies the value of the `node_namespace`
 * argument to set the data structure's `node_namespace` member.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | No
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * \par Thread-safety
 *   Setting a member is a reentrant procedure, but:
 *   - Access to the service endpoint info data structure is not synchronized.
 *     It is not safe to read or write the `node_namespace` member of the given `service_endpoint`
 *     while setting it.
 *   - Access to C-style string arguments is read-only but it is not synchronized.
 *     Concurrent `node_namespace` reads are safe, but concurrent reads and writes are not.
 *   - The default allocators are thread-safe objects, but any custom `allocator` may not be.
 *     Check your allocator documentation for further reference.
 *
 * \pre Given `node_namespace` is a valid C-style string i.e. NULL terminated.
 *
 * \param[inout] service_endpoint_info Data structure to be populated.
 * \param[in] node_namespace Node namespace to be set.
 * \param[in] allocator Allocator to be used.
 * \returns `RMW_RET_OK` if successful, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `service_endpoint_info` is NULL, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `node_namespace` is NULL, or
 * \returns `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 * \remark This function sets the RMW error state on failure.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_service_endpoint_info_set_node_namespace(
  rmw_service_endpoint_info_t * service_endpoint_info,
  const char * node_namespace,
  rcutils_allocator_t * allocator);

/// Set the endpoint type in the given service endpoint info data structure.
/**
 * Assigns the value of the `type` argument to the data structure's
 * `endpoint_type` member.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | No
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * \par Thread-safety
 *   Setting a member is a reentrant procedure, but access to the
 *   service endpoint info data structure is not synchronized.
 *   It is not safe to read or write the `endpoint_type` member of the
 *   given `service_endpoint` while setting it.
 *
 * \param[inout] service_endpoint_info Data structure to be populated.
 * \param[in] type Endpoint type to be set.
 * \returns `RMW_RET_OK` if successful, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `service_endpoint_info` is NULL, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 * \remark This function sets the RMW error state on failure.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_service_endpoint_info_set_endpoint_type(
  rmw_service_endpoint_info_t * service_endpoint_info,
  rmw_endpoint_type_t type);

/// Set the endpoint count in the given service endpoint info data structure.
/**
 * Assigns the value of the `endpoint_count` argument to the data structure's
 * `endpoint_count` member.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | No
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * \par Thread-safety
 *   Setting a member is a reentrant procedure, but access to the
 *   service endpoint info data structure is not synchronized.
 *   It is not safe to read or write the `endpoint_type` member of the
 *   given `service_endpoint` while setting it.
 *
 * \param[inout] service_endpoint_info Data structure to be populated.
 * \param[in] endpoint_count Endpoint count to be set.
 * \returns `RMW_RET_OK` if successful, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `service_endpoint_info` is NULL, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if 'endpoint_count' is not 1 or 2, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 * \remark This function sets the RMW error state on failure.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_service_endpoint_info_set_endpoint_count(
  rmw_service_endpoint_info_t * service_endpoint_info,
  size_t endpoint_count);

/// Set the endpoint gids in the given service endpoint info data structure.
/**
 * Copies the value of the `gids` argument to the data structure's
 * `endpoint_gids` member.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | No
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * \par Thread-safety
 *   Setting a member is a reentrant procedure, but access to the
 *   service endpoint info data structure is not synchronized.
 *   It is not safe to read or write the `gid` member of the
 *   given `service_endpoint` while setting it.
 *
 * \param[inout] service_endpoint_info Data structure to be populated.
 * \param[in] gids Pointer to a buffer containing endpoint GIDs to set.
 * \param[in] endpoint_count Number of endpoints used to construct the service.
 * \param[in] size Size in bytes of each individual `gid`.
 * \param[in] allocator Allocator to be used.
 * \returns `RMW_RET_OK` if successful, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `service_endpoint_info` is NULL, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `gids` is NULL, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if 'endpoint_count' is not 1 or 2, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `size` is greater than RMW_GID_STORAGE_SIZE, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `allocator` is NULL, or
 * \returns `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 * \remark This function sets the RMW error state on failure.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_service_endpoint_info_set_gids(
  rmw_service_endpoint_info_t * service_endpoint_info,
  const uint8_t * gids,
  size_t endpoint_count,
  size_t size,
  rcutils_allocator_t * allocator);

/// Set the endpoint QoS profiles in the given service endpoint info data structure.
/**
 * Assigns the value of the `qos_profiles` argument to the data structure's
 * `qos_profiles` member.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | No
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * \par Thread-safety
 *   Setting a member is a reentrant procedure, but access to the
 *   service endpoint info data structure is not synchronized.
 *   It is not safe to read or write the `qos_profile` member of the
 *   given `service_endpoint` while setting it.
 *
 * \param[inout] service_endpoint_info Data structure to be populated.
 * \param[in] qos_profiles An array of QoS profiles to be assigned.
 * \param[in] endpoint_count Number of endpoints used to construct the service.
 * \param[in] allocator Allocator to be used.
 * \returns `RMW_RET_OK` if successful, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `service_endpoint_info` is NULL, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `qos_profiles` is NULL, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if 'endpoint_count' is not 1 or 2, or
 * \returns `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 * \remark This function sets the RMW error state on failure.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_service_endpoint_info_set_qos_profiles(
  rmw_service_endpoint_info_t * service_endpoint_info,
  const rmw_qos_profile_t * qos_profiles,
  size_t endpoint_count,
  rcutils_allocator_t * allocator);

#ifdef __cplusplus
}
#endif

#endif    // RMW__SERVICE_ENDPOINT_INFO_H_

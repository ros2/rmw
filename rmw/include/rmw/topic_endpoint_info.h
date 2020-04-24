// Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

#ifndef RMW__TOPIC_ENDPOINT_INFO_H_
#define RMW__TOPIC_ENDPOINT_INFO_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "rcutils/allocator.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"

/// A structure that encapsulates the name, namespace, topic_type, gid and qos_profile
/// of publishers and subscriptions for a topic.
typedef struct RMW_PUBLIC_TYPE rmw_topic_endpoint_info_t
{
  /// Name of the node
  const char * node_name;
  /// Namespace of the node
  const char * node_namespace;
  /// The associated topic type
  const char * topic_type;
  /// The endpoint type
  rmw_endpoint_type_t endpoint_type;
  /// The GID of the endpoint
  uint8_t endpoint_gid[RMW_GID_STORAGE_SIZE];
  /// QoS profile of the endpoint
  rmw_qos_profile_t qos_profile;
} rmw_topic_endpoint_info_t;

/// Return a rmw_topic_endpoint_info_t struct with members initialized to `NULL`.
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_topic_endpoint_info_t
rmw_get_zero_initialized_topic_endpoint_info(void);

/// Finalize a rmw_topic_endpoint_info_t object.
/**
 * The rmw_topic_endpoint_info_t struct has members which require memory to be allocated to them before
 * setting values.
 * This function reclaims any allocated resources within the object and zeroes out all other
 * members.
 *
 * If a non RMW_RET_OK return value is returned, the RMW error message will be set
 *
 * \param[inout] topic_endpoint_info object to be finalized
 * \param[in] allocator the allocator used to allocate memory to the object
 * \returns `RMW_RET_OK` on successfully reclaiming memory, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if any parameters are NULL, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_topic_endpoint_info_fini(
  rmw_topic_endpoint_info_t * topic_endpoint_info,
  rcutils_allocator_t * allocator);

/// Set the topic_type in rmw_topic_endpoint_info_t.
/**
 * rmw_topic_endpoint_info_t has a member topic_type of type const char *;
 * this function allocates memory and copies the value of param passed to it.
 *
 * If a non RMW_RET_OK return value is returned, the RMW error message will be set
 *
 * \param[inout] topic_endpoint_info pointer to an initialized instance of rmw_topic_endpoint_info_t
 * \param[in] topic_type the topic_type value to set in rmw_topic_endpoint_info_t
 * \param[in] allocator the allocator that will be used to allocate memory
 * \returns `RMW_RET_OK` on successfully setting the topic_type, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if any parameters are NULL, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_topic_endpoint_info_set_topic_type(
  rmw_topic_endpoint_info_t * topic_endpoint_info,
  const char * topic_type,
  rcutils_allocator_t * allocator);

/// Set the node_name in rmw_topic_endpoint_info_t.
/**
 * rmw_topic_endpoint_info_t has a member node_name of type const char *;
 * this function allocates memory and copies the value of param passed to it.
 *
 * If a non RMW_RET_OK return value is returned, the RMW error message will be set
 *
 * \param[inout] topic_endpoint_info pointer to an initialized instance of rmw_topic_endpoint_info_t
 * \param[in] node_name the node_name value to set in rmw_topic_endpoint_info_t
 * \param[in] allocator the allocator that will be used to allocate memory
 * \returns `RMW_RET_OK` on successfully setting the node_name, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if any parameters are NULL, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_topic_endpoint_info_set_node_name(
  rmw_topic_endpoint_info_t * topic_endpoint_info,
  const char * node_name,
  rcutils_allocator_t * allocator);

/// Set the node_namespace in rmw_topic_endpoint_info_t.
/**
 * rmw_topic_endpoint_info_t has a member node_namespace of type const char *;
 * this function allocates memory and copies the value of param passed to it.
 *
 * If a non RMW_RET_OK return value is returned, the RMW error message will be set
 *
 * \param[inout] topic_endpoint_info pointer to an initialized instance of rmw_topic_endpoint_info_t
 * \param[in] node_namespace the node_namespace value to set in rmw_topic_endpoint_info_t
 * \param[in] allocator the allocator that will be used to allocate memory
 * \returns `RMW_RET_OK` on successfully setting the node_namespace, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if any parameters are NULL, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_topic_endpoint_info_set_node_namespace(
  rmw_topic_endpoint_info_t * topic_endpoint_info,
  const char * node_namespace,
  rcutils_allocator_t * allocator);

/// Set the gid in rmw_topic_endpoint_info_t.
/**
 * Copies the values from gid into the gid member inside topic_endpoint_info.
 *
 * If a non RMW_RET_OK return value is returned, the RMW error message will be set
 *
 * \param[inout] topic_endpoint_info pointer to an initialized instance of rmw_topic_endpoint_info_t
 * \param[in] gid the gid value to set in rmw_topic_endpoint_info_t
 * \param[in] size the size of the gid param
 * \returns `RMW_RET_OK` on successfully setting the gid, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if any parameters are NULL, or
 * \returns `RMW_RET_INVALID_ARGUMENT` size is greater than RMW_GID_STORAGE_SIZE, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_topic_endpoint_info_set_endpoint_type(
  rmw_topic_endpoint_info_t * topic_endpoint_info,
  rmw_endpoint_type_t type);

/// Set the gid in rmw_topic_endpoint_info_t.
/**
 * Copies the values from gid into the gid member inside topic_endpoint_info.
 *
 * If a non RMW_RET_OK return value is returned, the RMW error message will be set
 *
 * \param[inout] topic_endpoint_info pointer to an initialized instance of rmw_topic_endpoint_info_t
 * \param[in] gid the gid value to set in rmw_topic_endpoint_info_t
 * \param[in] size the size of the gid param
 * \returns `RMW_RET_OK` on successfully setting the gid, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if any parameters are NULL, or
 * \returns `RMW_RET_INVALID_ARGUMENT` size is greater than RMW_GID_STORAGE_SIZE, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_topic_endpoint_info_set_gid(
  rmw_topic_endpoint_info_t * topic_endpoint_info,
  const uint8_t gid[],
  size_t size);

/// Set the qos_profile in rmw_topic_endpoint_info_t.
/**
 * rmw_topic_endpoint_info_t has a member qos_profile of type const rmw_qos_profile_t *.
 * This function assigns the passed qos_profile pointer to the member.
 *
 * If a non RMW_RET_OK return value is returned, the RMW error message will be set
 *
 * \param[inout] topic_endpoint_info pointer to an initialized instance of rmw_topic_endpoint_info_t
 * \param[in] qos_profile the qos_profile to set in rmw_topic_endpoint_info_t
 * \returns `RMW_RET_OK` on successfully setting the qos_profile, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if any parameters are NULL, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_topic_endpoint_info_set_qos_profile(
  rmw_topic_endpoint_info_t * topic_endpoint_info,
  const rmw_qos_profile_t * qos_profile);

#ifdef __cplusplus
}
#endif

#endif    // RMW__TOPIC_ENDPOINT_INFO_H_

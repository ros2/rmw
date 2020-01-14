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

#ifndef RMW__GET_TOPIC_ENDPOINT_INFO_H_
#define RMW__GET_TOPIC_ENDPOINT_INFO_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "rmw/topic_endpoint_info_array.h"
#include "rmw/visibility_control.h"

/// Retrieve the information for all publishers to a given topic.
/**
 * The retrieved information will contain the publisher's node name, node namespace,
 * associated topic type, publisher gid and qos profile.
 *
 * The node parameter must not be `NULL` and must point to a valid node.
 *
 * The topic_name parameter must not be `NULL` and must follow the topic naming rules
 * mentioned at http://design.ros2.org/articles/topic_and_service_names.html
 * Names of non-existent topics are allowed.
 * In that case, this function will return an empty array.
 *
 * It is the responsibility of the caller to ensure that `publishers_info` parameter points
 * to a valid struct of type rmw_topic_endpoint_info_array_t.
 * The rmw_topic_endpoint_info_array_t struct must be zero initialized.
 * \see rmw_get_zero_initialized_topic_endpoint_info_array
 *
 * The `allocator` will be used to allocate memory to the `info_array` member
 * inside of `publishers_info`.
 * Moreover, every `const char *` member inside of
 * rmw_topic_endpoint_info_t will be assigned a copied value on allocated memory.
 * \see rmw_topic_endpoint_info_set_topic_type
 * \see rmw_topic_endpoint_info_set_node_name
 * \see rmw_topic_endpoint_info_set_node_namespace
 * However, it is the responsibility of the caller to
 * reclaim any allocated resources to `publishers_info` to avoid leaking memory.
 * \see rmw_topic_endpoint_info_array_fini
 *
 * \param[in] node the handle to the node being used to query the ROS graph.
 * \param[in] allocator the allocator to be used when allocating space for the array.
 * \param[in] topic_name the name of the topic for which the list of publishers will be retrieved.
 * \param[in] no_mangle if true, the topic name will not be mangled.
 * \param[out] publishers_info an array of rmw_topic_endpoint_info_t.
 * \return `RMW_RET_OK` if the query was successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if the node is invalid, or
 * \return `RMW_RET_INVALID_ARGUMENT` if any arguments are invalid, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_get_publishers_info_by_topic(
  const rmw_node_t * node,
  rcutils_allocator_t * allocator,
  const char * topic_name,
  bool no_mangle,
  rmw_topic_endpoint_info_array_t * publishers_info);

/// Retrieve the information for all subscriptions to a given topic.
/**
 * The retrieved information will contain the subscriptions's node name, node namespace,
 * associated topic type, subscription gid and qos profile.
 *
 * The node parameter must not be `NULL` and must point to a valid node.
 *
 * The topic_name parameter must not be `NULL` and must follow the topic naming rules
 * mentioned at http://design.ros2.org/articles/topic_and_service_names.html
 * Names of non-existent topics are allowed.
 * They will return an empty array.
 *
 * It is the responsibility of the caller to ensure that `subscriptions_info` parameter points
 * to a valid struct of type rmw_topic_endpoint_info_array_t.
 * The rmw_topic_endpoint_info_array_t struct must be zero initialized.
 * \see rmw_get_zero_initialized_topic_endpoint_info_array
 *
 * The `allocator` will be used to allocate memory to the `info_array` member
 * inside of `publishers_info`.
 * Moreover, every `const char *` member inside of
 * rmw_topic_endpoint_info_t will be assigned a copied value on allocated memory.
 * \see rmw_topic_endpoint_info_set_topic_type
 * \see rmw_topic_endpoint_info_set_node_name
 * \see rmw_topic_endpoint_info_set_node_namespace
 * However, it is the responsibility of the caller to
 * reclaim any allocated resources to `publishers_info` to avoid leaking memory.
 * \see rmw_topic_endpoint_info_array_fini
 *
 * \param[in] node the handle to the node being used to query the ROS graph.
 * \param[in] allocator the allocator to be used when allocating space for the array.
 * \param[in] topic_name the name of the topic for which the list of subscriptions will be retrieved.
 * \param[in] no_mangle if true, the topic name will not be mangled.
 * \param[out] subscriptions_info an array of rmw_topic_endpoint_info_t..
 * \return `RMW_RET_OK` if the query was successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if the node is invalid, or
 * \return `RMW_RET_INVALID_ARGUMENT` if any arguments are invalid, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation fails, or
 * \return `RMW_RET_ERROR` if an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_get_subscriptions_info_by_topic(
  const rmw_node_t * node,
  rcutils_allocator_t * allocator,
  const char * topic_name,
  bool no_mangle,
  rmw_topic_endpoint_info_array_t * subscriptions_info);

#ifdef __cplusplus
}
#endif

#endif  // RMW__GET_TOPIC_ENDPOINT_INFO_H_

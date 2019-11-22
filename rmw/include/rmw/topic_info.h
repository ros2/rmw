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

#ifndef RMW__TOPIC_INFO_H_
#define RMW__TOPIC_INFO_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "rcutils/allocator.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"

/// Set the gid in rmw_topic_info_t.
/**
 * Copies the values from gid into the gid member inside topic_info.
 *
 * \param[in] gid the gid value to set in rmw_topic_info_t
 * \param[out] topic_info pointer to an initialized instance of rmw_topic_info_t
 * \returns `RMW_RET_OK` on successfully setting the gid, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if any parameters are NULL, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_topic_info_set_gid(
  rmw_topic_info_t * topic_info,
  const uint8_t gid[],
  size_t size);

/// Set the topic_type in rmw_topic_info_t.
/**
 * rmw_topic_info_t has a member topic_type of type const char *;
 * this function allocates memory and copies the value of param passed to it.
 *
 * \param[in] topic_type the topic_type value to set in rmw_topic_info_t
 * \param[out] topic_info pointer to an initialized instance of rmw_topic_info_t
 * \returns `RMW_RET_OK` on successfully setting the topic_type, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if any parameters are NULL, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_topic_info_set_topic_type(
  rmw_topic_info_t * topic_info,
  const char * topic_type,
  rcutils_allocator_t * allocator);

/// Set the node_name in rmw_topic_info_t.
/**
 * rmw_topic_info_t has a member node_name of type const char *;
 * this function allocates memory and copies the value of param passed to it.
 *
 * \param[in] node_name the node_name value to set in rmw_topic_info_t
 * \param[out] topic_info pointer to an initialized instance of rmw_topic_info_t
 * \returns `RMW_RET_OK` on successfully setting the node_name, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if any parameters are NULL, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_topic_info_set_node_name(
  rmw_topic_info_t * topic_info,
  const char * node_name,
  rcutils_allocator_t * allocator);

/// Set the node_namespace in rmw_topic_info_t.
/**
 * rmw_topic_info_t has a member node_namespace of type const char *;
 * this function allocates memory and copies the value of param passed to it.
 *
 * \param[in] node_namespace the node_namespace value to set in rmw_topic_info_t
 * \param[out] topic_info pointer to an initialized instance of rmw_topic_info_t
 * \returns `RMW_RET_OK` on successfully setting the node_namespace, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if any parameters are NULL, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_topic_info_set_node_namespace(
  rmw_topic_info_t * topic_info,
  const char * node_namespace,
  rcutils_allocator_t * allocator);

/// Set the qos_profile in rmw_topic_info_t.
/**
 * rmw_topic_info_t has a member qos_profile of type const rmw_qos_profile_t *.
 * This function assigns the passed qos_profile pointer to the member.
 *
 * \param[in] qos_profile the qos_profile to set in rmw_topic_info_t
 * \param[out] topic_info pointer to an initialized instance of rmw_topic_info_t
 * \returns `RMW_RET_OK` on successfully setting the qos_profile, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if any parameters are NULL, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_topic_info_set_qos_profile(
  rmw_topic_info_t * topic_info,
  rmw_qos_profile_t * qos_profile);

#ifdef __cplusplus
}
#endif

#endif    // RMW__TOPIC_INFO_H_

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

#ifndef RMW__TOPIC_INFO_ARRAY_H_
#define RMW__TOPIC_INFO_ARRAY_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "rcutils/allocator.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"

/// Return a rmw_topic_info_array_t struct with members initialized to `NULL`.
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_topic_info_array_t
rmw_get_zero_initialized_topic_info_array(void);

/// Check that a rmw_topic_info_array_t struct is zero initialized.
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_topic_info_array_check_zero(rmw_topic_info_array_t * topic_info_array);

/// Initialize the info_array member inside rmw_topic_info_array_t with the given size
/**
 * The rmw_topic_info_array_t has a member variable info_array which is an array of
 * type rmw_topic_info_t. This function allocates memory to this array to hold n elements,
 * where n is the value of the size param to this function.
 *
 * \param[in] allocator the allocator to be used to allocate space
 * \param[in] size the size of the array
 * \param[out] topic_info_array the data structure to initialise
 * \returns `RMW_RET_OK` on successful init, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if any of the parameters are NULL, or
 * \returns `RMW_BAD_ALLOC` if memory allocation fails, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_topic_info_array_init_with_size(
  rcutils_allocator_t * allocator,
  size_t size,
  rmw_topic_info_array_t * topic_info_array);

/// Finalize a rmw_topic_info_array_t object.
/**
 * The info_array member variable inside of rmw_topic_info_array represents an array of
 * rmw_topic_info_t. When initializing this array, memory is allocated for it using the allocator.
 * This function reclaims any allocated resources within the object and also sets the value of count
 * to 0.
 *
 * \param[in] allocator the allocator used to allocate memory to the object
 * \param[out] topic_info_array object to be finalized
 * \returns `RMW_RET_OK` on successfully reclaiming memory, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if any parameters are NULL, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_topic_info_array_fini(
  rcutils_allocator_t * allocator,
  rmw_topic_info_array_t * topic_info_array);

/// Set the gid in rmw_topic_info_t.
/**
 * rmw_topic_info_t has a member gid of type const char *;
 * this function allocates memory and copies the value of param passed to it.
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
  rcutils_allocator_t * allocator,
  const char * gid,
  rmw_topic_info_t * topic_info);

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
  rcutils_allocator_t * allocator,
  const char * topic_type,
  rmw_topic_info_t * topic_info);

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
  rcutils_allocator_t * allocator,
  const char * node_name,
  rmw_topic_info_t * topic_info);

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
  rcutils_allocator_t * allocator,
  const char * node_namespace,
  rmw_topic_info_t * topic_info);

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
  rmw_qos_profile_t * qos_profile,
  rmw_topic_info_t * topic_info);

#ifdef __cplusplus
}
#endif

#endif    // RMW__TOPIC_INFO_ARRAY_H_

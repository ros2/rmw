// Copyright 2014 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__RMW_H_
#define RMW__RMW_H_

#if __cplusplus
extern "C"
{
#endif

#ifndef _WIN32
#define RMW_WARN_UNUSED __attribute__((warn_unused_result))
#else
#define RMW_WARN_UNUSED _Check_return_
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "rosidl_generator_c/message_type_support_struct.h"
#include "rosidl_generator_c/service_type_support.h"

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
rmw_create_node(const char * name, size_t domain_id);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_destroy_node(rmw_node_t * node);

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
rmw_destroy_client(rmw_client_t * client);

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
rmw_destroy_service(rmw_service_t * service);

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
 * If max_conditions is 0, the waitset can store an unbounded number of conditions to wait on.
 * If max_conditions is greater than 0, the number of conditions that can be attached to the
 * waitset is bounded at max_conditions.
 * \param[in] max_conditions The maximum number of conditions that can be attached to the waitset.
 * \return A pointer to the created waitset, nullptr if an error occurred.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_waitset_t *
rmw_create_waitset(size_t max_conditions);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_destroy_waitset(rmw_waitset_t * waitset);

/**
 * Add conditions to the wait set and wait until a response comes in, or until the timeout is
 * reached.
 * The arrays contain type-erased representations of waitable entities.
 * This function casts the pointers to middleware-specific conditions and adds them to the
 * waitset.
 *
 * The count variables in the arrays represents the number of valid pointers in the array.
 * Null pointers are in the array considered invalid. If they are encountered, an error is
 * returned.
 *
 * The array structs are allocated and deallocated outside of this function.
 * They do not have any information about how much memory is allocated for the arrays.
 *
 * After the wait wakes up, the entries in each array that correspond to conditions that were
 * not triggered are set to null.
 *
 * \param subscriptions Array of subscriptions to wait on
 * \param guard_conditions Array of guard conditions to wait on
 * \param services Array of services to wait on
 * \param clients Array of clients to wait on
 * \param waitset Storage for the waitset
 * \param wait_timeout If negative, block indefinitely or until a condition is ready.
 * If zero,  check only for immediately available conditions and don't block.
 * Else, this represents the maximum time to wait for a response from the waitset.
 * \return RMW_RET_OK if success, RMW_RET_ERROR if error, RMW_RET_TIMEOUT if wait timed out.
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
rmw_get_remote_topic_names_and_types(
  rmw_topic_names_and_types_t * topic_names_and_types);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_destroy_topic_names_and_types(
  rmw_topic_names_and_types_t * topic_names_and_types);

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

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ros_meta_t *  
rmw_get_node_names(void);

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t 
rmw_destroy_ros_meta(rmw_ros_meta_t * rosmeta);

#if __cplusplus
}
#endif

#endif  // RMW__RMW_H_

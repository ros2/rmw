/* Copyright 2014 Open Source Robotics Foundation, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ROS_MIDDLEWARE_INTERFACE_ROS_MIDDLEWARE_INTERFACE_RMW_H_
#define ROS_MIDDLEWARE_INTERFACE_ROS_MIDDLEWARE_INTERFACE_RMW_H_

#if __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <rosidl_generator_c/message_type_support.h>
#include <rosidl_generator_c/service_type_support.h>

#include "types.h"
#include "visibility_control.h"

RMW_PUBLIC
const char *
rmw_get_implementation_identifier();

RMW_PUBLIC
rmw_ret_t
rmw_init();

RMW_PUBLIC
rmw_node_t *
rmw_create_node(const char * name);

RMW_PUBLIC
rmw_ret_t
rmw_destroy_node(rmw_node_t * node);

RMW_PUBLIC
rmw_publisher_t *
rmw_create_publisher(const rmw_node_t * node,
                     const rosidl_message_type_support_t * type_support,
                     const char * topic_name,
                     size_t queue_size);

RMW_PUBLIC
rmw_ret_t
rmw_destroy_publisher(rmw_publisher_t * publisher);

RMW_PUBLIC
rmw_ret_t
rmw_publish(const rmw_publisher_t * publisher, const void * ros_message);

RMW_PUBLIC
rmw_subscription_t *
rmw_create_subscription(const rmw_node_t * node,
                        const rosidl_message_type_support_t * type_support,
                        const char * topic_name,
                        size_t queue_size);

RMW_PUBLIC
rmw_ret_t
rmw_destroy_subscription(rmw_subscription_t * subscription);

RMW_PUBLIC
rmw_ret_t
rmw_take(const rmw_subscription_t * subscription, void * ros_message, bool * taken);

RMW_PUBLIC
rmw_client_t *
rmw_create_client(const rmw_node_t * node,
                  const rosidl_service_type_support_t * type_support,
                  const char * service_name);

RMW_PUBLIC
rmw_ret_t
rmw_destroy_client(rmw_client_t * client);

RMW_PUBLIC
rmw_ret_t
rmw_send_request(const rmw_client_t * client, const void * ros_request,
                 int64_t * sequence_id);

// If there is no reponse to take, ros_response and ros_request_header should
// be set to NULL. This is done instead of returning true or false.
RMW_PUBLIC
rmw_ret_t
rmw_take_response(const rmw_client_t * client,
                  void * ros_request_header, void * ros_response, bool * taken);

RMW_PUBLIC
rmw_service_t *
rmw_create_service(const rmw_node_t * node,
                   const rosidl_service_type_support_t * type_support,
                   const char * service_name);

RMW_PUBLIC
rmw_ret_t
rmw_destroy_service(rmw_service_t * service);

// If there is no reponse to take, ros_response and ros_request_header should
// be set to NULL. This is done instead of returning true or false.
RMW_PUBLIC
rmw_ret_t
rmw_take_request(const rmw_service_t * service,
                 void * ros_request_header, void * ros_request, bool * taken);

RMW_PUBLIC
rmw_ret_t
rmw_send_response(const rmw_service_t * service,
                  void * ros_request, void * ros_response);

RMW_PUBLIC
rmw_guard_condition_t *
rmw_create_guard_condition();

RMW_PUBLIC
rmw_ret_t
rmw_destroy_guard_condition(rmw_guard_condition_t * guard_condition);

RMW_PUBLIC
rmw_ret_t
rmw_trigger_guard_condition(const rmw_guard_condition_t * guard_condition);

RMW_PUBLIC
rmw_ret_t
rmw_wait(rmw_subscriptions_t * subscriptions,
         rmw_guard_conditions_t * guard_conditions,
         rmw_services_t * services,
         rmw_clients_t * clients,
         bool non_blocking);

#if __cplusplus
}
#endif

#endif  /* ROS_MIDDLEWARE_INTERFACE_ROS_MIDDLEWARE_INTERFACE_RMW_H_ */

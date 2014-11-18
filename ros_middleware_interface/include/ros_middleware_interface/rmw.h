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

#include <rosidl_generator_c/message_type_support.h>

#include "types.h"
#include "visibility_control.h"

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
rmw_take(const rmw_subscription_t * subscription, void * ros_message);

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
         bool non_blocking);

#if __cplusplus
}
#endif

#endif  /* ROS_MIDDLEWARE_INTERFACE_ROS_MIDDLEWARE_INTERFACE_RMW_H_ */

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

#ifndef ROS_MIDDLEWARE_INTERFACE_ROS_MIDDLEWARE_INTERFACE_ALLOCATORS_H_
#define ROS_MIDDLEWARE_INTERFACE_ROS_MIDDLEWARE_INTERFACE_ALLOCATORS_H_

#if __cplusplus
extern "C"
{
#endif

#include <stdlib.h>

#include "types.h"
#include "visibility_control.h"

RMW_LOCAL
void *
rmw_allocate(size_t size)
{
  // Could be overridden with a general purpose allocator
  return malloc(size);
}

RMW_LOCAL
void
rmw_free(void * pointer)
{
  // Should have a corresponding overide with rmw_allocate
  free(pointer);
}

RMW_LOCAL
rmw_node_t *
rmw_node_allocate()
{
  // Could be overridden with custom (maybe static) node struct allocator
  return (rmw_node_t *)rmw_allocate(sizeof(rmw_node_t));
}

RMW_LOCAL
void
rmw_node_free(rmw_node_t * node)
{
  // Should have matching overide with rmw_node_allocate
  rmw_free(node);
}

RMW_LOCAL
rmw_publisher_t *
rmw_publisher_allocate()
{
  // Could be overridden with custom (maybe static) publisher struct allocator
  return (rmw_publisher_t *)rmw_allocate(sizeof(rmw_publisher_t));
}

RMW_LOCAL
void
rmw_publisher_free(rmw_publisher_t * publisher)
{
  // Should have matching overide with rmw_publisher_allocate
  rmw_free(publisher);
}

RMW_LOCAL
rmw_subscription_t *
rmw_subscription_allocate()
{
  // Could be overridden with custom (maybe static) subscription
  // struct allocator
  return (rmw_subscription_t *)rmw_allocate(sizeof(rmw_subscription_t));
}

RMW_LOCAL
void
rmw_subscription_free(rmw_subscription_t * subscription)
{
  // Should have matching overide with rmw_subscription_allocate
  rmw_free(subscription);
}

RMW_LOCAL
rmw_guard_condition_t *
rmw_guard_condition_allocate()
{
  // Could be overridden with custom (maybe static) guard_condition
  // struct allocator
  return (rmw_guard_condition_t *)rmw_allocate(sizeof(rmw_guard_condition_t));
}

RMW_LOCAL
void
rmw_guard_condition_free(rmw_guard_condition_t * guard_condition)
{
  // Should have matching overide with rmw_guard_condition_allocate
  rmw_free(guard_condition);
}

#if __cplusplus
}
#endif

#endif  /* ROS_MIDDLEWARE_INTERFACE_ROS_MIDDLEWARE_INTERFACE_ALLOCATORS_H_ */

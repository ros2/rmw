// Copyright 2018 Open Source Robotics Foundation, Inc.
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

#include "rcutils/error_handling.h"

#include "rmw/error_handling.h"
#include "rmw/serialized_message.h"

rmw_serialized_message_t
rmw_get_zero_initialized_serialized_message(void)
{
  static rmw_serialized_message_t serialized_message = {
    .buffer = NULL,
    .buffer_length = 0u,
    .buffer_capacity = 0u
  };
  serialized_message.allocator = rcutils_get_zero_initialized_allocator();
  return serialized_message;
}

rmw_ret_t
rmw_serialized_message_init(
  rmw_serialized_message_t * msg,
  size_t buffer_capacity,
  const rcutils_allocator_t * allocator)
{
  rcutils_allocator_t error_msg_allocator = rcutils_get_default_allocator();
  RCUTILS_CHECK_FOR_NULL_WITH_MSG(
    msg, "serialized message pointer is null", return RMW_RET_ERROR, error_msg_allocator);

  if (!rcutils_allocator_is_valid(allocator)) {
    RMW_SET_ERROR_MSG("serialized message has no valid allocator");
    return RMW_RET_ERROR;
  }

  msg->buffer_length = 0;
  msg->buffer_capacity = buffer_capacity;
  msg->allocator = *allocator;

  if (buffer_capacity > 0u) {
    msg->buffer = (char *)allocator->allocate(buffer_capacity * sizeof(char), allocator->state);
    RCUTILS_CHECK_FOR_NULL_WITH_MSG(
      msg->buffer,
      "failed to allocate memory for serialized message",
      return RMW_RET_BAD_ALLOC,
      *allocator);
  }

  return RMW_RET_OK;
}

rmw_ret_t
rmw_serialized_message_fini(rmw_serialized_message_t * msg)
{
  rcutils_allocator_t error_msg_allocator = rcutils_get_default_allocator();
  RCUTILS_CHECK_FOR_NULL_WITH_MSG(
    msg, "serialized message pointer is null", return RMW_RET_ERROR, error_msg_allocator);

  rcutils_allocator_t * allocator = &msg->allocator;
  if (!rcutils_allocator_is_valid(allocator)) {
    RMW_SET_ERROR_MSG("serialized message has no valid allocator");
    return RMW_RET_ERROR;
  }

  allocator->deallocate(msg->buffer, allocator->state);
  msg->buffer = NULL;
  msg->buffer_length = 0u;
  msg->buffer_capacity = 0u;

  return RMW_RET_OK;
}

rmw_ret_t
rmw_serialized_message_resize(rmw_serialized_message_t * msg, size_t new_size)
{
  rcutils_allocator_t error_msg_allocator = rcutils_get_default_allocator();
  RCUTILS_CHECK_FOR_NULL_WITH_MSG(
    msg, "serialized message pointer is null", return RMW_RET_ERROR, error_msg_allocator);

  rcutils_allocator_t * allocator = &msg->allocator;
  if (!rcutils_allocator_is_valid(allocator)) {
    RMW_SET_ERROR_MSG("serialized message has no valid allocator");
    return RMW_RET_ERROR;
  }

  if (new_size == msg->buffer_capacity) {
    // nothing to do here
    return RMW_RET_OK;
  }

  msg->buffer = rcutils_reallocf(msg->buffer, new_size * sizeof(char), allocator);
  RCUTILS_CHECK_FOR_NULL_WITH_MSG(
    msg->buffer,
    "failed to reallocate memory for serialized message",
    return RMW_RET_BAD_ALLOC,
    *allocator);

  msg->buffer_capacity = new_size;
  if (new_size < msg->buffer_length) {
    msg->buffer_length = new_size;
  }

  return RMW_RET_OK;
}

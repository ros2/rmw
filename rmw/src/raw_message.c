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

#include "rmw/error_handling.h"
#include "rmw/raw_message.h"

RMW_PUBLIC
rmw_message_raw_t
rmw_get_zero_initialized_raw_message(void)
{
  static rmw_message_raw_t raw_message = {
    .buffer = NULL,
    .buffer_length = 0u,
    .buffer_capacity = 0u
  };
  raw_message.allocator = rcutils_get_zero_initialized_allocator();
  return raw_message;
}

RMW_PUBLIC
rmw_ret_t
rmw_initialize_raw_message(
  rmw_message_raw_t * msg,
  unsigned int buffer_capacity,
  const rcutils_allocator_t * allocator)
{
  msg->buffer = (char *)allocator->allocate(buffer_capacity * sizeof(char), allocator->state);
  if (!msg->buffer) {
    return RMW_RET_ERROR;
  }
  msg->buffer_length = 0;
  msg->buffer_capacity = buffer_capacity;
  msg->allocator = *allocator;

  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_raw_message_fini(rmw_message_raw_t * msg)
{
  if (!msg) {
    RMW_SET_ERROR_MSG("raw message pointer is null");
    return RMW_RET_ERROR;
  }

  rcutils_allocator_t * allocator = &msg->allocator;
  if (!rcutils_allocator_is_valid(allocator)) {
    RMW_SET_ERROR_MSG("raw message has no valid allocator");
    return RMW_RET_ERROR;
  }

  allocator->deallocate(msg->buffer, allocator->state);
  msg->buffer = NULL;
  msg->buffer_length = 0u;
  msg->buffer_capacity = 0u;

  return RMW_RET_OK;
}

RMW_PUBLIC
rmw_ret_t
rmw_raw_message_resize(rmw_message_raw_t * msg, unsigned int new_size)
{
  if (!msg) {
    RMW_SET_ERROR_MSG("raw message pointer is null");
    return RMW_RET_ERROR;
  }

  rcutils_allocator_t * allocator = &msg->allocator;
  if (!rcutils_allocator_is_valid(allocator)) {
    RMW_SET_ERROR_MSG("raw message has no valid allocator");
    return RMW_RET_ERROR;
  }

  if (new_size == msg->buffer_capacity) {
    // nothing to do here
    return RMW_RET_OK;
  }

  char * new_buffer = allocator->allocate(new_size * sizeof(char), allocator->state);
  if (!new_buffer) {
    RMW_SET_ERROR_MSG("failed to allocate memory for resizing raw message");
    return RMW_RET_ERROR;
  }
  if (new_size < msg->buffer_capacity) {
    memcpy(new_buffer, msg->buffer, new_size);
  } else {  // new_size > msg->buffer_capacity
    memcpy(new_buffer, msg->buffer, msg->buffer_capacity);
  }
  allocator->deallocate(msg->buffer, allocator->state);
  msg->buffer = new_buffer;
  msg->buffer_capacity = new_size;
  if (new_size < msg->buffer_length) {
    msg->buffer_length = new_size;
  }

  return RMW_RET_OK;
}

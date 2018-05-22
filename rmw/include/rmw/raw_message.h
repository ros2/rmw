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

#ifndef RMW__RAW_MESSAGE_H_
#define RMW__RAW_MESSAGE_H_

#if __cplusplus
extern "C"
{
#endif

#include "rcutils/allocator.h"

#include "rmw/types.h"
#include "rmw/visibility_control.h"

/// Create a zero initialized raw message struct
/**
 * \return rmw_message_raw_t a zero initialized raw message struct
 */
RMW_PUBLIC
rmw_message_raw_t
rmw_get_zero_initialized_raw_message(void);

/// Initialize a newly created raw message struct
/**
 * Initialize a zero initialized raw message struct
 * This function might leak if the raw message struct is already pre-initialized.
 * If the capacity is set to 0, no memory is allocated and the internal buffer
 * is still NULL.
 * \return rmw_ret_t indicate whether the object was successfully initialized
 * \param msg pointer to the to be initialized raw message struct
 * \param buffer_capacity the size of the memory to allocate for the bytestream
 * \param allocator the allocator to use for the memory allocation.
 */
RMW_PUBLIC
rmw_ret_t
rmw_raw_message_init(
  rmw_message_raw_t * msg,
  unsigned int buffer_capacity,
  const rcutils_allocator_t * allocator);

/// Cleanup a raw message struct
/**
 * Deallocate and cleanup a raw message struct. This functions has to be created by a
 * call to rmw_get_zero_initialized_raw_message prior to this or it might lead to
 * undefined behavior.
 * \return rmw_ret_t indicate whether the object was successfully cleaned up.
 * \param msg pointer to the raw message to be cleaned up
 */
RMW_PUBLIC
rmw_ret_t
rmw_raw_message_fini(rmw_message_raw_t * msg);

/// Resize the internal buffer for the message bytestream
/**
 * The internal buffer of the raw message can be resized dynamically if needed.
 * If the new size is smaller than the current capacity, the memory is truncated.
 * Be aware, that this will deallocate the memory and invalidates their pointers.
 * If the new size is larger, new memory is getting allocated and the existing
 * content is copied over.
 * \return rmw_ret_t indicate whether the resizing was successful.
 * \param msg pointer to the raw message struct which internal buffer is getting resized.
 * \param new_size the new size of the internal buffer
 */
RMW_PUBLIC
rmw_ret_t
rmw_raw_message_resize(rmw_message_raw_t * msg, unsigned int new_size);

#if __cplusplus
}
#endif

#endif  // RMW__RAW_MESSAGE_H_

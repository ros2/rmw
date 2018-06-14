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

#ifndef RMW__SERIALIZED_MESSAGE_H_
#define RMW__SERIALIZED_MESSAGE_H_

#if __cplusplus
extern "C"
{
#endif

#include "rcutils/allocator.h"

#include "rmw/macros.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"

/// Create a zero initialized serialized message struct
/**
 * \return rmw_serialized_message_t a zero initialized serialized message struct
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_serialized_message_t
rmw_get_zero_initialized_serialized_message(void);

/// Initialize a newly created serialized message struct
/**
 * Initialize a zero initialized serialized message struct
 * This function might leak if the serialized message struct is already pre-initialized.
 * If the capacity is set to 0, no memory is allocated and the internal buffer
 * is still NULL.
 * \return rmw_ret_t indicate whether the object was successfully initialized
 * \param msg pointer to the to be initialized serialized message struct
 * \param buffer_capacity the size of the memory to allocate for the bytestream
 * \param allocator the allocator to use for the memory allocation.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_serialized_message_init(
  rmw_serialized_message_t * msg,
  unsigned int buffer_capacity,
  const rcutils_allocator_t * allocator);

/// Cleanup a serialized message struct
/**
 * Deallocate and cleanup a serialized message struct. This functions has to be created by a
 * call to rmw_get_zero_initialized_serialized_message prior to this or it might lead to
 * undefined behavior.
 * \return rmw_ret_t indicate whether the object was successfully cleaned up.
 * \param msg pointer to the serialized message to be cleaned up
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_serialized_message_fini(rmw_serialized_message_t * msg);

/// Resize the internal buffer for the message bytestream
/**
 * The internal buffer of the serialized message can be resized dynamically if needed.
 * If the new size is smaller than the current capacity, the memory is truncated.
 * Be aware, that this will deallocate the memory and invalidates their pointers.
 * If the new size is larger, new memory is getting allocated and the existing
 * content is copied over.
 * \return rmw_ret_t indicate whether the resizing was successful.
 * \param msg pointer to the serialized message struct which internal buffer is getting resized.
 * \param new_size the new size of the internal buffer
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_serialized_message_resize(rmw_serialized_message_t * msg, unsigned int new_size);

#if __cplusplus
}
#endif

#endif  // RMW__SERIALIZED_MESSAGE_H_

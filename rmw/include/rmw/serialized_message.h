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

/// Return a zero initialized serialized message struct.
/**
 * \return rmw_serialized_message_t a zero initialized serialized message struct
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_serialized_message_t
rmw_get_zero_initialized_serialized_message(void);

/// Initialize a zero initialized serialized message struct.
/**
 * This function may leak if the serialized message struct is already
 * pre-initialized.
 * If the capacity is set to 0, no memory is allocated and the internal buffer
 * is still NULL.
 *
 * \param msg a pointer to the to be initialized serialized message struct
 * \param buffer_capacity the size of the memory to allocate for the byte stream
 * \param allocator the allocator to use for the memory allocation
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_serialized_message_init(
  rmw_serialized_message_t * msg,
  size_t buffer_capacity,
  const rcutils_allocator_t * allocator);

/// Finalize a serialized message struct.
/**
 * Cleans up and deallocates any resources used in a rmw_message_serialized_t.
 * Passing a rmw_serialized_message_t which has not been zero initialized using
 * rmw_get_zero_initialized_serialized_message() to this function is undefined
 * behavior.
 *
 * \param msg pointer to the serialized message to be cleaned up
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation failed, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_serialized_message_fini(rmw_serialized_message_t * msg);

/// Resize the internal buffer for the message byte stream.
/**
 * The internal buffer of the serialized message can be resized dynamically if needed.
 * If the new size is smaller than the current capacity, then the memory is
 * truncated.
 * Be aware, that this will deallocate the memory and therefore invalidates any
 * pointers to this storage.
 * If the new size is larger, new memory is getting allocated and the existing
 * content is copied over.
 *
 * \param msg pointer to the instance of rmw_serialized_message_t which is being resized
 * \param new_size the new size of the internal buffer
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_BAD_ALLOC` if memory allocation failed, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_serialized_message_resize(rmw_serialized_message_t * msg, size_t new_size);

#if __cplusplus
}
#endif

#endif  // RMW__SERIALIZED_MESSAGE_H_

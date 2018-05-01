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

#ifndef RMW__ERROR_HANDLING_H_
#define RMW__ERROR_HANDLING_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>

#include <rcutils/error_handling.h>

#include "rmw/visibility_control.h"

typedef rcutils_error_state_t rmw_error_state_t;

#define rmw_error_state_copy rcutils_error_state_copy

#define rmw_error_state_fini rcutils_error_state_fini

// TODO(wjwwood): replace this completely with rcutils_set_error_state()
//                once the rmw APIs take an allocator that can be passed
//                by the rmw implementations on to the error functions
/// Set the error state, implicitly uses rcutils_get_default_allocator().
/**
 * \see rcutils_get_default_allocator()
 * \see rcutils_set_error_state()
 */
RMW_PUBLIC
void
rmw_set_error_state(const char * error_msg, const char * file, size_t line_number);

/// Set the error message, as well as append the current file and line number.
/**
 * \see RCUTILS_SET_ERROR_MSG
 */
#define RMW_SET_ERROR_MSG(msg) rmw_set_error_state(msg, __FILE__, __LINE__);

#define RMW_SET_ERROR_MSG_ALLOC(msg, allocator) \
  rcutils_set_error_state(msg, __FILE__, __LINE__, allocator);

#define rmw_error_is_set rcutils_error_is_set

#define rmw_get_error_state rcutils_get_error_state

#define rmw_get_error_string rcutils_get_error_string

#define rmw_get_error_string_safe rcutils_get_error_string_safe

#define rmw_reset_error rcutils_reset_error

#ifdef __cplusplus
}
#endif

#endif  // RMW__ERROR_HANDLING_H_

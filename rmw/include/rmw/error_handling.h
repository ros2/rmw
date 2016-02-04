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

#if __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>

#include "rmw/macros.h"
#include "rmw/visibility_control.h"

/// Struct which encapsulates the error state set by RMW_SET_ERROR_MSG().
typedef struct rmw_error_state_t
{
  const char * message;
  const char * file;
  size_t line_number;
} rmw_error_state_t;

/// Set the error message, the file it occurred in, and the line on which it occurred.
/**
 * This is not meant to be used directly, but instead via the RMW_SET_ERROR_MSG(msg) macro.
 *
 * The error_msg parameter is copied into the interal error storage and must be null terminated.
 * The file parameter is not copied, but instead is assumed to be a global as it should be set to
 * the __FILE__ preprocessor literal when used with the RMW_SET_ERROR_MSG macro.
 * It should also be null terminated.
 *
 * \param error_msg The error message to set.
 * \param file The path to the file in which the error occurred.
 * \param line_number The line number on which the error occurred.
 */
RMW_PUBLIC
void
rmw_set_error_state(const char * error_msg, const char * file, size_t line_number);

/// Set the error message, automatically appending the current file and line number.
/**
 * If an error message was previously set, and rmw_reset_error was not called
 * since, and this library was built with RMW_REPORT_ERROR_HANDLING_ERRORS set
 * on, then previously set error message will be printed to stderr.
 * Error state storage is thread local and so all error related functions are also thread local.
 *
 * \param msg The error message to be set.
 */
#define RMW_SET_ERROR_MSG(msg) rmw_set_error_state(msg, __FILE__, __LINE__);

/// Return true if the error has been set and has not been reset since, otherwise false.
RMW_PUBLIC
bool
rmw_error_is_set(void);

/// Return a struct with the error message and the file and line number on which is was set.
/**
 * The returned pointer will be NULL if no error has been set in this thread.
 *
 * The returned pointer is valid until RMW_SET_ERROR_MSG, rmw_set_error_state, or rmw_reset_error
 * are called in the same thread.
 *
 * \return A pointer to the current error state struct.
 */
RMW_PUBLIC
const rmw_error_state_t *
rmw_get_error_state(void);

/// Return the error message followed by ', at <file>:<line>' if set, else NULL.
/**
 * The returned pointer is valid until RMW_SET_ERROR_MSG, rmw_set_error_state, or rmw_reset_error
 * are called in the same thread.
 *
 * \return The current error string, with file and line number, or NULL if not set.
 */
RMW_PUBLIC
const char *
rmw_get_error_string(void);

/// Return the error message followed by ', at <file>:<line>' if set, else "error not set".
/**
 * This function is guaranteed to return a valid c-string.
 *
 * The returned pointer is valid until RMW_SET_ERROR_MSG, rmw_set_error_state, or rmw_reset_error
 * are called in the same thread.
 *
 * \return The current error string, with file and line number, or "error not set" if not set.
 */
RMW_PUBLIC
const char *
rmw_get_error_string_safe(void);

/// Resets the error state by clearing any previously set error state.
RMW_PUBLIC
void
rmw_reset_error(void);

#if __cplusplus
}
#endif

#endif  // RMW__ERROR_HANDLING_H_

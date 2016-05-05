// Copyright 2015 Open Source Robotics Foundation, Inc.
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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rmw/allocators.h>
#include <rmw/error_handling.h>
#include <rmw/impl/config.h>
#include <rmw/macros.h>

#define SAFE_FWRITE_TO_STDERR(msg) fwrite(msg, sizeof(char), sizeof(msg), stderr)

RMW_THREAD_LOCAL rmw_error_state_t * __rmw_error_state = NULL;
RMW_THREAD_LOCAL char * __rmw_error_string = NULL;

static const char __error_format_string[] = "%s, at %s:%zu";

bool
__rmw_error_is_set(rmw_error_state_t * error_state);

void
__rmw_reset_error_string(char ** error_string_ptr);

void
__rmw_reset_error(rmw_error_state_t ** error_state_ptr_ptr);

void
rmw_set_error_state(const char * error_string, const char * file, size_t line_number)
{
  rmw_error_state_t * old_error_state = __rmw_error_state;
#if RMW_REPORT_ERROR_HANDLING_ERRORS
  const char * old_error_string = rmw_get_error_string_safe();
#endif
  __rmw_error_state = (rmw_error_state_t *)rmw_allocate(sizeof(rmw_error_state_t));
  if (!__rmw_error_state) {
#if RMW_REPORT_ERROR_HANDLING_ERRORS
    // rmw_allocate failed, but fwrite might work?
    SAFE_FWRITE_TO_STDERR(
      "[rmw|error_handling.c:" RMW_STRINGIFY(__LINE__)
      "] failed to allocate memory for the error state struct\n");
#endif
    return;
  }
  size_t error_string_length = strlen(error_string);
  // the memory must be one byte bigger to store the NULL character
  __rmw_error_state->message = (char *)malloc(error_string_length + 1);
  if (!__rmw_error_state->message) {
#if RMW_REPORT_ERROR_HANDLING_ERRORS
    // malloc failed, but fwrite might work?
    SAFE_FWRITE_TO_STDERR(
      "[rmw|error_handling.c:" RMW_STRINGIFY(__LINE__)
      "] failed to allocate memory for the error message in the error state struct\n");
#endif
    rmw_reset_error();  // This will free any allocations done so far.
    return;
  }
  // Cast the const away to set ->message initially.
#ifndef _WIN32
  snprintf((char *)__rmw_error_state->message, error_string_length + 1, "%s", error_string);
#else
  auto retcode = strcpy_s(
    (char *)__rmw_error_state->message, error_string_length + 1, error_string);
  if (retcode) {
#if RMW_REPORT_ERROR_HANDLING_ERRORS
    SAFE_FWRITE_TO_STDERR(
      "[rmw|error_handling.c:" RMW_STRINGIFY(__LINE__)
      "] failed to copy error message in the error state struct\n");
#endif
  }
#endif
  __rmw_error_state->file = file;
  __rmw_error_state->line_number = line_number;
  if (__rmw_error_is_set(old_error_state)) {
#if RMW_REPORT_ERROR_HANDLING_ERRORS
    // Only warn of overwritting if the new error string is different from the old ones.
    if (error_string != old_error_string && error_string != old_error_state->message) {
      fprintf(
        stderr,
        "[rmw|error_handling.c:" RMW_STRINGIFY(__LINE__) "] error string being overwritten: %s\n",
        old_error_string);
    }
#endif
    __rmw_reset_error(&old_error_state);
  }
  __rmw_reset_error_string(&__rmw_error_string);
}

const rmw_error_state_t *
rmw_get_error_state()
{
  return __rmw_error_state;
}

static void
format_error_string()
{
  if (!__rmw_error_is_set(__rmw_error_state)) {
    return;
  }
  size_t bytes_it_would_have_written = snprintf(
    NULL, 0,
    __error_format_string,
    __rmw_error_state->message, __rmw_error_state->file, __rmw_error_state->line_number);
  __rmw_error_string = (char *)rmw_allocate(bytes_it_would_have_written + 1);
  if (!__rmw_error_string) {
#if RMW_REPORT_ERROR_HANDLING_ERRORS
    // rmw_allocate failed, but fwrite might work?
    SAFE_FWRITE_TO_STDERR(
      "[rmw|error_handling.c:" RMW_STRINGIFY(__LINE__)
      "] failed to allocate memory for the error string\n");
#endif
    return;
  }
  snprintf(
    __rmw_error_string, bytes_it_would_have_written + 1,
    __error_format_string,
    __rmw_error_state->message, __rmw_error_state->file, __rmw_error_state->line_number);
  // The Windows version of snprintf does not null terminate automatically in all cases.
  __rmw_error_string[bytes_it_would_have_written] = '\0';
}

const char *
rmw_get_error_string()
{
  if (!__rmw_error_string) {
    format_error_string();
  }
  return __rmw_error_string;
}

bool
__rmw_error_is_set(rmw_error_state_t * error_state)
{
  return error_state != NULL;
}

bool
rmw_error_is_set()
{
  return __rmw_error_is_set(__rmw_error_state);
}

const char *
rmw_get_error_string_safe()
{
  if (!rmw_error_is_set()) {
    return "error not set";
  }
  return rmw_get_error_string();
}

void
__rmw_reset_error_string(char ** error_string_ptr)
{
  char * error_string = *error_string_ptr;
  if (error_string) {
    if (error_string) {
      rmw_free(error_string);
    }
  }
  *error_string_ptr = NULL;
}

void
__rmw_reset_error(rmw_error_state_t ** error_state_ptr_ptr)
{
  rmw_error_state_t * error_state_ptr = *error_state_ptr_ptr;
  if (error_state_ptr_ptr) {
    if (error_state_ptr) {
      if (error_state_ptr->message) {
        // Cast const away to delete previously allocated memory.
        rmw_free((char *)error_state_ptr->message);
      }
      rmw_free(error_state_ptr);
    }
  }
  *error_state_ptr_ptr = NULL;
}

void
rmw_reset_error()
{
  __rmw_reset_error_string(&__rmw_error_string);
  __rmw_reset_error(&__rmw_error_state);
}

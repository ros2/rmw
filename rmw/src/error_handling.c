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
#include <string.h>

#include <rmw/allocators.h>
#include <rmw/error_handling.h>
#include <rmw/impl/config.h>
#include <rmw/macros.h>

#define SAFE_FWRITE_TO_STDERR(msg) fwrite(msg, sizeof(char), sizeof(msg), stderr)

#ifndef _WIN32
#define SNPRINTF snprintf
#else
#define SNPRINTF _snprintf
#endif

RMW_THREAD_LOCAL rmw_error_state_t * __rmw_error_state = NULL;
RMW_THREAD_LOCAL char * __rmw_error_string = NULL;

static const char __error_format_string[] = "%s, at %s:%zu";

void
rmw_set_error_state(const char * error_string, const char * file, size_t line_number)
{
  if (rmw_error_is_set()) {
#if RMW_REPORT_ERROR_HANDLING_ERRORS
    fprintf(
      stderr,
      "[rmw|error_handling.c:" RMW_STRINGIFY(__LINE__) "] error string being overwritten: %s\n",
      rmw_get_error_string_safe());
#endif
    rmw_reset_error();
  }
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
  __rmw_error_state->message = (char *)malloc(strlen(error_string));
  if (!__rmw_error_state->message) {
#if RMW_REPORT_ERROR_HANDLING_ERRORS
    // rmw_allocate failed, but fwrite might work?
    SAFE_FWRITE_TO_STDERR(
      "[rmw|error_handling.c:" RMW_STRINGIFY(__LINE__)
      "] failed to allocate memory for the error message in the error state struct\n");
#endif
    free(__rmw_error_state);
    __rmw_error_state = NULL;
    return;
  }
  // Cast the const away to set ->message initially.
  strcpy((char *)__rmw_error_state->message, error_string);
  __rmw_error_state->file = file;
  __rmw_error_state->line_number = line_number;
}

const rmw_error_state_t *
rmw_get_error_state()
{
  return __rmw_error_state;
}

static void
format_error_string(char * dst, const rmw_error_state_t * error_state)
{
  size_t bytes_it_would_have_written = SNPRINTF(
    NULL, 0,
    __error_format_string,
    error_state->message, error_state->file, error_state->line_number);
  dst = (char *)rmw_allocate(bytes_it_would_have_written + 1);
  if (!dst) {
#if RMW_REPORT_ERROR_HANDLING_ERRORS
    // rmw_allocate failed, but fwrite might work?
    SAFE_FWRITE_TO_STDERR(
      "[rmw|error_handling.c:" RMW_STRINGIFY(__LINE__)
      "] failed to allocate memory for the error string\n");
#endif
    return;
  }
  SNPRINTF(
    dst, bytes_it_would_have_written + 1,
    __error_format_string,
    error_state->message, error_state->file, error_state->line_number);
  // The Windows version of snprintf does not null terminate automatically in all cases.
  dst[bytes_it_would_have_written] = '\0';
}

const char *
rmw_get_error_string()
{
  if (!__rmw_error_string) {
    format_error_string(__rmw_error_string, __rmw_error_state);
  }
  return __rmw_error_string;
}

bool
rmw_error_is_set()
{
  return __rmw_error_state == NULL;
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
rmw_reset_error()
{
  if (__rmw_error_state) {
    if (__rmw_error_state->message) {
      // Cast const away to delete previously allocated memory.
      rmw_free((char *)__rmw_error_state->message);
    }
    rmw_free(__rmw_error_state);
  }
  __rmw_error_state = NULL;
  if (__rmw_error_string) {
    rmw_free(__rmw_error_string);
  }
  __rmw_error_string = NULL;
}

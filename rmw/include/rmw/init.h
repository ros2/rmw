// Copyright 2014-2018 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__RMW_H_
#define RMW__RMW_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#include "rmw/macros.h"
#include "rmw/types.h"
#include "rmw/visibility_control.h"

/// Implementation defined options structure used during rmw_init().
/**
 * This should be defined by the rmw implementation.
 */
typedef struct rmw_init_options_impl_t rmw_init_options_impl_t;

/// Options structure used during rmw_init().
typedef struct RMW_PUBLIC_TYPE rmw_init_options_t {
  /// Locally (process local) unique ID that represents this init/shutdown cycle.
  /**
   * This should be set by the caller of `rmw_init()` to a number that is
   * unique within this process.
   * It is designed to be used with `rcl_init()` and `rcl_get_instance_id()`.
   */
  uint64_t instance_id;
  /// Implementation defined init options.
  rmw_init_options_impl_t * impl;
} rmw_init_options_t;

/// Return the default init options.
/**
 * This should be defined by the rmw implementation.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_init_options_t
rmw_get_default_init_options(void);

/// Implementation defined context structure returned by rmw_init().
/**
 * This should be defined by the rmw implementation.
 */
typedef struct rmw_init_context_impl_t rmw_init_context_impl_t;

/// Initialization context structure which is used to store init specific information.
typedef struct RMW_PUBLIC_TYPE rmw_init_context_t {
  /// Locally (process local) unique ID that represents this init/shutdown cycle.
  uint64_t instance_id;
  /// Implementation defined init context information.
  rmw_init_context_impl_t * impl;
} rmw_init_context_t;

/// Return a zero initialized init context structure.
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_init_context_t
rmw_get_zero_initialized_init_context(void);

/// Initialize the middleware with the given options, and yielding an init context.
/**
 * The given init context must be zero initialized, and is filled with
 * middleware specific data upon success of this function.
 * The init context is used when initializing some entities like nodes and
 * guard conditions, and is also required to properly call rmw_shutdown().
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | No
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * This should be defined by the rmw implementation.
 *
 * \param[in] options initialization options to be used during initialization
 * \param[out] context resulting context struct
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if any arguments are null or invalid, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_init(const rmw_init_options_t * options, rmw_init_context_t * context);

/// Shutdown the middleware for a given init context.
/**
 * The given init context must be a valid context which has been initialized
 * with rmw_init().
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | No
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * This should be defined by the rmw implementation.
 *
 * \param[in] context resulting context struct
 * \return `RMW_RET_OK` if successful, or
 * \return `RMW_RET_INVALID_ARGUMENT` if the argument is null or invalid, or
 * \return `RMW_RET_ERROR` if an unexpected error occurs.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_shutdown(rmw_init_context_t * context);

#ifdef __cplusplus
}
#endif

#endif  // RMW__RMW_H_

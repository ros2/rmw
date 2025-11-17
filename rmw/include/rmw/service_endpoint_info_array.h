// Copyright 2025 Minju Lee (이민주).
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

#ifndef RMW__SERVICE_ENDPOINT_INFO_ARRAY_H_
#define RMW__SERVICE_ENDPOINT_INFO_ARRAY_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>

#include "rcutils/allocator.h"
#include "rmw/service_endpoint_info.h"
#include "rmw/visibility_control.h"

/// Array of service endpoint information
typedef struct RMW_PUBLIC_TYPE rmw_service_endpoint_info_array_s
{
  /// Size of the array.
  size_t size;
  /// Contiguous storage for service endpoint information elements.
  rmw_service_endpoint_info_t * info_array;
} rmw_service_endpoint_info_array_t;

/// Return a zero initialized array of service endpoint information.
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_service_endpoint_info_array_t
rmw_get_zero_initialized_service_endpoint_info_array(void);

/// Check that the given `service_endpoint_info_array` is zero initialized.
/**
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | Yes
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * \par Thread-safety
 *   Access to the array of service endpoint information is read-only, but it is not synchronized.
 *   Concurrent `service_endpoint_info_array` reads are safe, but concurrent reads
 *   and writes are not.
 *
 * \param[in] service_endpoint_info_array Array to be checked.
 * \returns `RMW_RET_OK` if array is zero initialized, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `service_endpoint_info_array` is NULL, or
 * \returns `RMW_RET_ERROR` if `service_endpoint_info_array` is not zero initialized.
 * \remark This function sets the RMW error state on failure.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_service_endpoint_info_array_check_zero(
  rmw_service_endpoint_info_array_t * service_endpoint_info_array);

/// Initialize an array of service endpoint information.
/**
 * This function allocates space to hold `size` service endpoint information elements.
 * Both `info_array` and `size` members are updated accordingly.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | Yes
 * Thread-Safe        | No
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * \par Thread-safety
 *   Initialization is a reentrant procedure, but:
 *   - Access to the array of service endpoint information is not synchronized.
 *     It is not safe to read or write `service_endpoint_info_array` during initialization.
 *   - The default allocators are thread-safe objects, but any custom `allocator` may not be.
 *     Check your allocator documentation for further reference.
 *
 * \param[inout] service_endpoint_info_array Array to be initialized on success,
 *   but left unchanged on failure.
 * \param[in] size Size of the array.
 * \param[in] allocator Allocator to be used to populate `names_and_types`.
 * \returns `RMW_RET_OK` if successful, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `service_endpoint_info_array` is NULL, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `service_endpoint_info_array` is not
 *   a zero initialized array, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `allocator` is invalid,
 *   by rcutils_allocator_is_valid() definition, or
 * \returns `RMW_BAD_ALLOC` if memory allocation fails, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 * \remark This function sets the RMW error state on failure.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_service_endpoint_info_array_init_with_size(
  rmw_service_endpoint_info_array_t * service_endpoint_info_array,
  size_t size,
  rcutils_allocator_t * allocator);

/// Finalize an array of service endpoint information.
/**
 * This function deallocates the given array storage, and then zero initializes it.
 * If a logical error, such as `RMW_RET_INVALID_ARGUMENT`, ensues, this function will
 * return early, leaving the given array unchanged.
 * Otherwise, it will proceed despite errors.
 *
 * <hr>
 * Attribute          | Adherence
 * ------------------ | -------------
 * Allocates Memory   | No
 * Thread-Safe        | No
 * Uses Atomics       | No
 * Lock-Free          | Yes
 *
 * \par Thread-safety
 *   Finalization is a reentrant procedure, but:
 *   - Access to the array of service endpoint information is not synchronized.
 *     It is not safe to read or write `service_endpoint_info_array` during finalization.
 *   - The default allocators are thread-safe objects, but any custom `allocator` may not be.
 *     Check your allocator documentation for further reference.
 *
 * \pre Given `allocator` must be the same used to initialize the given `service_endpoint_info_array`.
 *
 * \param[inout] service_endpoint_info_array object to be finalized.
 * \param[in] allocator Allocator used to populate the given `service_endpoint_info_array`.
 * \returns `RMW_RET_OK` if successful, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `service_endpoint_info_array` is NULL, or
 * \returns `RMW_RET_INVALID_ARGUMENT` if `allocator` is invalid,
 *   by rcutils_allocator_is_valid() definition, or
 * \returns `RMW_RET_ERROR` when an unspecified error occurs.
 * \remark This function sets the RMW error state on failure.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_service_endpoint_info_array_fini(
  rmw_service_endpoint_info_array_t * service_endpoint_info_array,
  rcutils_allocator_t * allocator);

#ifdef __cplusplus
}
#endif

#endif    // RMW__SERVICE_ENDPOINT_INFO_ARRAY_H_

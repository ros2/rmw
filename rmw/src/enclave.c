// Copyright 2025 Sony Group Corporation.
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

#include <stddef.h>
#include <string.h>

#include "rcutils/allocator.h"
#include "rcutils/strdup.h"

#include "rmw/enclave.h"
#include "rmw/error_handling.h"

rmw_ret_t
rmw_enclave_options_copy(
  const char * src,
  const rcutils_allocator_t * allocator,
  char ** dst)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(src, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(dst, RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CHECK_ALLOCATOR(allocator, return RMW_RET_INVALID_ARGUMENT);

  char * new_enclave = rcutils_strdup(src, *allocator);
  if (!new_enclave) {
    RMW_SET_ERROR_MSG("failed to copy enclave options");
    return RMW_RET_BAD_ALLOC;
  }
  *dst = new_enclave;
  return RMW_RET_OK;
}

rmw_ret_t
rmw_enclave_options_fini(
  char * enclave_options,
  const rcutils_allocator_t * allocator)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(enclave_options, RMW_RET_INVALID_ARGUMENT);
  RCUTILS_CHECK_ALLOCATOR(allocator, return RMW_RET_INVALID_ARGUMENT);

  allocator->deallocate(enclave_options, allocator->state);
  return RMW_RET_OK;
}

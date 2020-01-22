// Copyright 2020 Open Source Robotics Foundation, Inc.
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

#include "rcutils/strdup.h"

#include "rmw/security_options.h"

rmw_security_options_t
rmw_get_zero_initialized_security_options()
{
  rmw_security_options_t zero_initialized_options = {0, NULL};
  return zero_initialized_options;
}

rmw_security_options_t
rmw_get_default_security_options()
{
  rmw_security_options_t default_options = {
    RMW_SECURITY_ENFORCEMENT_PERMISSIVE,
    NULL};
  return default_options;
}

rmw_ret_t
rmw_security_options_set_root_path(
  const char * security_root_path,
  rcutils_allocator_t * allocator,
  rmw_security_options_t * security_options)
{
  security_options->security_root_path = rcutils_strdup(security_root_path, *allocator);
  if (!security_options->security_root_path) {
    return RMW_RET_BAD_ALLOC;
  }
  return RMW_RET_OK;
}

rmw_ret_t
rmw_security_options_fini(
  rmw_security_options_t * security_options,
  rcutils_allocator_t * allocator)
{
  if (!allocator) {
    return RMW_RET_INVALID_ARGUMENT;
  }
  allocator->deallocate(security_options->security_root_path, allocator->state);
  *security_options = rmw_get_zero_initialized_security_options();
  return RMW_RET_OK;
}

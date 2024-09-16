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

#include <stddef.h>

#include "rmw/init_options.h"
#include "rmw/discovery_options.h"

#ifdef __cplusplus
extern "C"
{
#endif

rmw_init_options_t
rmw_get_zero_initialized_init_options(void)
{
  // TODO(@fujitatomoya): This is not exatly zero initialized structure.
  /// We should introduce xxx_get_default_init_optionst to return the default values.
  static const rmw_init_options_t init_option = {
    .domain_id = RMW_DEFAULT_DOMAIN_ID,
    .discovery_options = {RMW_AUTOMATIC_DISCOVERY_RANGE_NOT_SET, 0},
    .implementation_identifier = NULL,
    .impl = NULL,
    .instance_id = 0,
    .enclave = NULL,
    .security_options = {RMW_SECURITY_ENFORCEMENT_PERMISSIVE, NULL},
  };
  return init_option;
}

#ifdef __cplusplus
}
#endif

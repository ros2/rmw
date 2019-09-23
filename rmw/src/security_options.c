// Copyright 2017-2019 Open Source Robotics Foundation, Inc.
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

#include "rmw/security_options.h"

rmw_security_options_t
rmw_get_zero_initialized_security_options()
{
  static rmw_security_options_t null_security_options = {0, NULL};
  return null_security_options;
}

rmw_security_options_t
rmw_get_default_security_options()
{
  static rmw_security_options_t default_options;
  default_options.enforce_security = RMW_SECURITY_ENFORCEMENT_PERMISSIVE;
  default_options.security_root_path = NULL;
  return default_options;
}

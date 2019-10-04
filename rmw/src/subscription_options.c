// Copyright 2019 Open Source Robotics Foundation, Inc.
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

#include "rmw/subscription_options.h"

#ifdef __cplusplus
extern "C"
{
#endif

rmw_subscription_options_t
rmw_get_default_subscription_options(void)
{
  rmw_subscription_options_t subscription_options = {
    .rmw_specific_subscription_payload = NULL,
    .ignore_local_publications = false,
  };
  return subscription_options;
}

#ifdef __cplusplus
}
#endif

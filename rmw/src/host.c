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

#include <rmw/host.h>

#include "rcutils/get_env.h"

#include <stdlib.h>
#include <string.h>

bool
rmw_local_host_only()
{
	const char * ros_local_host_env_val = NULL;
  return rcutils_get_env(RMW_LOCAL_HOST_ENV_VAR, &ros_local_host_env_val) == NULL &&
    ros_local_host_env_val != NULL &&
    (strcmp(ros_local_host_env_val, "true") == 0 || strcmp(ros_local_host_env_val, "1") == 0);
}

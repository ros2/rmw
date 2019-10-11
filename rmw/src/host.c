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

#include <stdlib.h>
#include <string.h>

rmw_ret_t
rmw_allowed_hosts(
	char * allowed_hosts)
{
  rmw_ret_t ret = RMW_ENV_VAR_NOT_DEFINED_OR_EMPTY;
	const char * ros_host_env = "ROS_ALLOWED_HOSTS";
#ifndef _WIN32
  const char * ros_host_env_val = getenv(ros_host_env);
  if (ros_host_env_val != NULL && strlen(ros_host_env_val) > 0) {
    allowed_hosts = malloc(strlen(ros_host_env_val)+1);
    strcpy(allowed_hosts, ros_host_env_val);
  }
#else
  size_t ros_host_env_val_size;
  _dupenv_s(&allowed_hosts, &ros_host_env_val_size, ros_host_env);
#endif
  if (allowed_hosts != NULL && strcmp(allowed_hosts, "") != 0) {
    if (strcmp(allowed_hosts, "localhost") != 0) {
      ret = RMW_INVALID_ALLOWED_HOSTS;
      free(allowed_hosts);
    } else {
      ret = RMW_LOCAL_HOST_ENABLED;
    }
  } else {
    ret = RMW_ENV_VAR_NOT_DEFINED_OR_EMPTY;
    if (allowed_hosts != NULL) {
      free(allowed_hosts);
    }
  }
  return ret;
}

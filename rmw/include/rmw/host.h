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

#ifndef RMW__IMPL__CPP__HOST_H
#define RMW__IMPL__CPP__HOST_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "rmw/macros.h"
#include "rmw/types.h"

#define RMW_ENV_VAR_NOT_DEFINED_OR_EMPTY 0
#define RMW_LOCAL_HOST_ENABLED 1
#define RMW_INVALID_ALLOWED_HOSTS 2


/// Determine if the user provided a list of hosts to communicate to in the
/**
 * Checks the allowed hosts declared by the user in the environment variable ROS_ALLOWED_HOSTS.
 * localhost is the only host supported besides using all network interfaces. Allocates memory for
 * the list of hosts if it's not empty and supported.
 * \param[out] allowed_hosts string that contains the list of allowed hosts.
 * \returns `RMW_ENV_VAR_NOT_DEFINED_OR_EMPTY` if the env var is not defined or is empty.
 * \returns `RMW_LOCAL_HOST_ENABLED` if the environment variable is localhost
 * \returns `RMW_INVALID_ALLOWED_HOSTS` when the user provides a non empty string
 * different than localhost.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_ret_t
rmw_allowed_hosts(
	char * allowed_hosts);

#ifdef __cplusplus
}
#endif

#endif // RMW__IMPL__CPP__HOST_H

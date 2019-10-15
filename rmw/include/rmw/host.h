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

const char * RMW_LOCAL_HOST_ENV_VAR = "ROS_LOCALHOST_ONLY";

/// Determine if the user wants to communicate using loopback only.
/**
 * Checks if localhost should be used for network communication checking ROS_LOCALHOST_ONLY env
 * variable
 * \returns true if ROS_LOCALHOST_ONLY is 1 or true, false otherwise.
 */
RMW_PUBLIC
RMW_WARN_UNUSED
bool
rmw_local_host_only();

#ifdef __cplusplus
}
#endif

#endif // RMW__IMPL__CPP__HOST_H

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

#ifndef RMW__SECURITY_H_
#define RMW__SECURITY_H_

#include <stdbool.h>

#include "rmw/visibility_control.h"

#ifdef __cplusplus
extern "C"
{
#endif

/// Indicates if node or context name have to be used in security directory lookup.
/**
 * \returns true if node name should be used, or
 * \returns false if context name should be used.
 */
RMW_PUBLIC
bool
rmw_use_node_name_in_security_directory_lookup();

#ifdef __cplusplus
}
#endif

#endif  // RMW__SECURITY_H_

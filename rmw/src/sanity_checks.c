// Copyright 2016 Open Source Robotics Foundation, Inc.
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

#include "rmw/sanity_checks.h"

#include <stdlib.h>
#include <string.h>

#include "rmw/error_handling.h"
#include "rmw/types.h"

//  Test that a rmw_topic_names_and_types_t struct is zero initialized
rmw_ret_t
rmw_check_zero_rmw_topic_names_and_types(rmw_topic_names_and_types_t * tnat)
{
  if (!tnat) {
    RMW_SET_ERROR_MSG("null topics_names_and_types");
    return RMW_RET_ERROR;
  }
  if (tnat->topic_count != 0) {
    RMW_SET_ERROR_MSG("topic count is not zero");
    return RMW_RET_ERROR;
  }
  if (tnat->topic_names) {
    RMW_SET_ERROR_MSG("topic names is not null");
    return RMW_RET_ERROR;
  }
  if (tnat->type_names) {
    RMW_SET_ERROR_MSG("type names is not null");
    return RMW_RET_ERROR;
  }
  return RMW_RET_OK;
}

// Copyright 2015 Open Source Robotics Foundation, Inc.
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

#include <string.h>

#include <rmw/error_handling.h>

RMW_THREAD_LOCAL char * __rmw_error_string = 0;

void
rmw_set_error_string(const char * error_string)
{
  __rmw_error_string = strdup(error_string);
}

const char *
rmw_get_error_string()
{
  return __rmw_error_string;
}

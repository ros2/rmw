// Copyright 2020 Robert Bosch GmbH. All Rights Reserved.
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

#include "rmw/types.h"

#include "rcutils/time.h"

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_message_info_t
rmw_get_zero_initialized_message_info(void)
{
  rmw_message_info_t zero_initialized_message_info = {0, 0, {NULL, {0}}, false};
  return zero_initialized_message_info;
}

RMW_PUBLIC
RMW_WARN_UNUSED
bool
rmw_time_equal(const rmw_time_t left, const rmw_time_t right)
{
  return rmw_time_total_nsec(left) == rmw_time_total_nsec(right);
}

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_duration_t
rmw_time_total_nsec(const rmw_time_t time)
{
  return RCUTILS_S_TO_NS(time.sec) + time.nsec;
}

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_time_t
rmw_time_from_nsec(const rmw_duration_t nanoseconds)
{
  // Avoid typing the 1 billion constant
  rmw_time_t time;
  time.sec = RCUTILS_NS_TO_S(nanoseconds);
  time.nsec = nanoseconds % RCUTILS_S_TO_NS(1);
  return time;
}

RMW_PUBLIC
RMW_WARN_UNUSED
rmw_time_t
rmw_time_normalize(const rmw_time_t time)
{
  return rmw_time_from_nsec(rmw_time_total_nsec(time));
}

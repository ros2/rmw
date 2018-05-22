// Copyright 2018 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__RAW_MESSAGE_H_
#define RMW__RAW_MESSAGE_H_

#if __cplusplus
extern "C"
{
#endif

#include "rcutils/allocator.h"

#include "rmw/types.h"
#include "rmw/visibility_control.h"

RMW_PUBLIC
rmw_message_raw_t
rmw_get_zero_initialized_raw_message(void);

RMW_PUBLIC
rmw_ret_t
rmw_raw_message_init(
  rmw_message_raw_t * msg,
  unsigned int buffer_capacity,
  const rcutils_allocator_t * allocator);

RMW_PUBLIC
rmw_ret_t
rmw_raw_message_fini(rmw_message_raw_t * msg);

RMW_PUBLIC
rmw_ret_t
rmw_raw_message_resize(rmw_message_raw_t * msg, unsigned int new_size);

#if __cplusplus
}
#endif

#endif  // RMW__RAW_MESSAGE_H_

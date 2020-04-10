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

#ifndef RMW__MESSAGE_SEQUENCE_H_
#define RMW__MESSAGE_SEQUENCE_H_

#include <stddef.h>

#include "rmw/macros.h"
#include "rmw/visibility_control.h"
#include "rmw/types.h"

#if __cplusplus
extern "C"
{
#endif

RMW_PUBLIC
rmw_message_sequence_t
rmw_get_zero_initialized_message_sequence(void);

RMW_PUBLIC
rmw_ret_t
rmw_message_sequence_init(rmw_message_sequence_t * sequence, size_t size);

RMW_PUBLIC
rmw_ret_t
rmw_message_sequence_fini(rmw_message_sequence_t * sequence);

RMW_PUBLIC
rmw_message_info_sequence_t
rmw_get_zero_initialized_message_info_sequence(void);

RMW_PUBLIC
rmw_ret_t
rmw_message_info_sequence_init(rmw_message_info_sequence_t * sequence, size_t size);

RMW_PUBLIC
rmw_ret_t
rmw_message_info_sequence_fini(rmw_message_info_sequence_t * sequence);

#if __cplusplus
}
#endif

#endif  // RMW__MESSAGE_SEQUENCE_H_

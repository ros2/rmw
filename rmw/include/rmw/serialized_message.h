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

#ifndef RMW__SERIALIZED_MESSAGE_H_
#define RMW__SERIALIZED_MESSAGE_H_

#if __cplusplus
extern "C"
{
#endif

#include "rcutils/types/serialized_message.h"

// aliases for rcutils_serialized_message_t
typedef rcutils_serialized_message_t rmw_serialized_message_t;
#define rmw_get_zero_initialized_serialized_message rcutils_get_zero_initialized_serialized_message
#define rmw_serialized_message_init rcutils_serialized_message_init
#define rmw_serialized_message_fini rcutils_serialized_message_fini
#define rmw_serialized_message_resize rcutils_serialized_message_resize

#if __cplusplus
}
#endif

#endif  // RMW__SERIALIZED_MESSAGE_H_

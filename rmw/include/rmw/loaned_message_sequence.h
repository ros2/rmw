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

#ifndef RMW__LOANED_MESSAGE_SEQUENCE_H_
#define RMW__LOANED_MESSAGE_SEQUENCE_H_

#include <stddef.h>

#include "rmw/macros.h"
#include "rmw/visibility_control.h"

#if __cplusplus
extern "C"
{
#endif

/// A sequence of loaned messages
typedef struct RMW_PUBLIC_TYPE rmw_loaned_message_sequence_t
{
  /// Type erased pointer to the messages
  void * message_sequence;

  /// Number of messages currently in this sequence
  size_t size;

  /// Maximum number of messages this sequence has been allocated for
  size_t capacity;
} rmw_loaned_message_sequence_t;

/// Gets a zero initialized `rmw_loaned_message_sequence_t`
/**
 *\return rmw_loaned_message_sequence_t with NULL message_sequence and size and capacity as 0
 */
RMW_PUBLIC
RMW_WARN_UNUSED
rmw_loaned_message_sequence_t
rmw_get_zero_initialized_loaned_message_sequence(void);

#if __cplusplus
}
#endif

#endif  // RMW__LOANED_MESSAGE_SEQUENCE_H_

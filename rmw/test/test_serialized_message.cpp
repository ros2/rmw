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

#include "gmock/gmock.h"

#include "rcutils/allocator.h"

#include "rmw/types.h"

TEST(test_serialized_message, default_initialization) {
  auto serialized_msg = rmw_get_zero_initialized_serialized_message();

  auto allocator = rcutils_get_default_allocator();
  EXPECT_EQ(RMW_RET_OK, rmw_serialized_message_init(&serialized_msg, 0, &allocator));
  EXPECT_EQ(0u, serialized_msg.buffer_capacity);
  EXPECT_EQ(RMW_RET_OK, rmw_serialized_message_fini(&serialized_msg));
  EXPECT_EQ(0u, serialized_msg.buffer_capacity);
  EXPECT_FALSE(serialized_msg.buffer);
}

TEST(test_serialized_message, resize) {
  auto serialized_msg = rmw_get_zero_initialized_serialized_message();
  auto allocator = rcutils_get_default_allocator();
  auto ret = rmw_serialized_message_init(&serialized_msg, 5, &allocator);
  ASSERT_EQ(RMW_RET_OK, ret);

  for (size_t i = 0; i < 5; ++i) {
    uint8_t c = 1u << i;
    memcpy(serialized_msg.buffer + i, &c, 1);
  }
  serialized_msg.buffer_length = 5;
  for (size_t i = 0; i < serialized_msg.buffer_length; ++i) {
    EXPECT_EQ(1u << i, serialized_msg.buffer[i]);
  }

  ret = rmw_serialized_message_resize(&serialized_msg, 11);
  ASSERT_EQ(RMW_RET_OK, ret);
  EXPECT_EQ(11u, serialized_msg.buffer_capacity);
  EXPECT_EQ(5u, serialized_msg.buffer_length);

  for (size_t i = 0; i < 11; ++i) {
    uint8_t c = 0xFF - static_cast<uint8_t>(i);
    memcpy(serialized_msg.buffer + i, &c, 1);
  }
  serialized_msg.buffer_length = 11;
  for (size_t i = 0; i < 11; ++i) {
    EXPECT_EQ(0xFF - i, serialized_msg.buffer[i]);
  }

  ret = rmw_serialized_message_resize(&serialized_msg, 3);
  ASSERT_EQ(RMW_RET_OK, ret);
  EXPECT_EQ(3u, serialized_msg.buffer_capacity);
  EXPECT_EQ(3u, serialized_msg.buffer_length);
  EXPECT_EQ(0xFF, serialized_msg.buffer[0]);
  EXPECT_EQ(0xFF - 1, serialized_msg.buffer[1]);
  EXPECT_EQ(0xFF - 2, serialized_msg.buffer[2]);
  // the other fields are garbage.

  // cleanup only 3 fields
  EXPECT_EQ(RMW_RET_OK, rmw_serialized_message_fini(&serialized_msg));
}

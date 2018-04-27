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

#include "rmw/raw_message.h"

TEST(test_raw_message, default_initialization) {
  auto raw_msg = rmw_get_zero_initialized_raw_message();

  auto allocator = rcutils_get_default_allocator();
  EXPECT_EQ(RMW_RET_OK, rmw_initialize_raw_message(&raw_msg, 0, &allocator));
  EXPECT_EQ(0u, raw_msg.buffer_capacity);
  EXPECT_TRUE(raw_msg.buffer);
  EXPECT_EQ(RMW_RET_OK, rmw_raw_message_fini(&raw_msg));
  EXPECT_EQ(0u, raw_msg.buffer_capacity);
  EXPECT_FALSE(raw_msg.buffer);
}

TEST(test_raw_message, resize) {
  auto raw_msg = rmw_get_zero_initialized_raw_message();
  auto allocator = rcutils_get_default_allocator();
  rmw_initialize_raw_message(&raw_msg, 5, &allocator);

  memcpy(raw_msg.buffer, "1234\0", 5);
  raw_msg.buffer_length = 5;
  EXPECT_STREQ("1234\0", raw_msg.buffer);

  auto ret = rmw_raw_message_resize(&raw_msg, 11);
  EXPECT_EQ(RMW_RET_OK, ret);
  EXPECT_EQ(11u, raw_msg.buffer_capacity);
  EXPECT_EQ(5u, raw_msg.buffer_length);

  memcpy(raw_msg.buffer, "0987654321\0", 11);
  raw_msg.buffer_length = 11;
  EXPECT_STREQ("0987654321\0", raw_msg.buffer);

  ret = rmw_raw_message_resize(&raw_msg, 3);
  EXPECT_EQ(RMW_RET_OK, ret);
  EXPECT_EQ(3u, raw_msg.buffer_capacity);
  EXPECT_EQ(3u, raw_msg.buffer_length);
  EXPECT_EQ('0', raw_msg.buffer[0]);
  EXPECT_EQ('9', raw_msg.buffer[1]);
  EXPECT_EQ('8', raw_msg.buffer[2]);
  // the other fields are garbage.

  // cleanup only 3 fields
  EXPECT_EQ(RMW_RET_OK, rmw_raw_message_fini(&raw_msg));
}

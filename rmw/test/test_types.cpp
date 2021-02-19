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

#include "gmock/gmock.h"
#include "rmw/types.h"

TEST(test_types, zero_initialized_message_info) {
  rmw_message_info_t info = rmw_get_zero_initialized_message_info();
  EXPECT_EQ(0u, info.source_timestamp);
  EXPECT_EQ(0u, info.received_timestamp);
  EXPECT_EQ(nullptr, info.publisher_gid.implementation_identifier);
  for (size_t i = 0; i < RMW_GID_STORAGE_SIZE; ++i) {
    EXPECT_EQ(0u, info.publisher_gid.data[i]);
  }

  EXPECT_FALSE(info.from_intra_process);
}

TEST(test_types, time_equal) {
  {
    rmw_time_t a {100, 100};
    rmw_time_t b {100, 100};
    ASSERT_TRUE(rmw_time_equal(a, b));
  }
  {
    rmw_time_t a {0, 0};
    rmw_time_t b {0, 0};
    ASSERT_TRUE(rmw_time_equal(a, b));
  }
  {
    rmw_time_t a {0, 100};
    rmw_time_t b {100, 0};
    ASSERT_FALSE(rmw_time_equal(a, b));
  }
  {
    rmw_time_t a {0, 1000000000};
    rmw_time_t b {1, 0};
    ASSERT_TRUE(rmw_time_equal(a, b));
  }
  {
    rmw_time_t a {2, 100};
    rmw_time_t b {1, 1000000100};
    ASSERT_TRUE(rmw_time_equal(a, b));
  }
  {
    rmw_time_t a {1, 100};
    rmw_time_t b {1, 101};
    ASSERT_FALSE(rmw_time_equal(a, b));
  }
}

TEST(test_types, time_total_nsec) {
  {
    rmw_time_t time {1, 1};
    EXPECT_EQ(rmw_time_total_nsec(time), 1000000001);
  }
  {
    rmw_time_t time {0, 0};
    EXPECT_EQ(rmw_time_total_nsec(time), 0);
  }
  {
    rmw_time_t time {0, 123456789};
    EXPECT_EQ(rmw_time_total_nsec(time), 123456789);
  }
}

TEST(test_types, time_from_nsec) {
  {
    rmw_time_t zero = rmw_time_from_nsec(0);
    EXPECT_TRUE(rmw_time_equal({0, 0}, zero));
  }
  {
    rmw_time_t less_than_sec = rmw_time_from_nsec(100000);
    EXPECT_TRUE(rmw_time_equal({0, 100000}, less_than_sec));
  }
  {
    rmw_time_t time = rmw_time_from_nsec(INT64_MAX);
    EXPECT_TRUE(rmw_time_equal(time, RMW_DURATION_INFINITE));
  }
}

TEST(test_types, time_normalize) {
  {
    rmw_time_t bad {0, 1234567890};
    rmw_time_t good {1, 234567890};
    rmw_time_t normalized = rmw_time_normalize(bad);
    EXPECT_EQ(good.sec, normalized.sec);
    EXPECT_EQ(good.nsec, normalized.nsec);
  }
  {
    rmw_time_t good {10, 10};
    rmw_time_t normalized = rmw_time_normalize(good);
    EXPECT_EQ(good.sec, normalized.sec);
    EXPECT_EQ(good.nsec, normalized.nsec);
  }
}

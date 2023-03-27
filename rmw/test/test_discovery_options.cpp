// Copyright 2023 Open Source Robotics Foundation, Inc.
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

#include "rmw/discovery_options.h"

#include "rcutils/allocator.h"
#include "rmw/error_handling.h"

TEST(discovery_options, zero_init_fini) {
  rmw_discovery_options_t dopts = rmw_get_zero_initialized_discovery_options();
  auto allocator = rcutils_get_default_allocator();
  EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_init(&dopts, 0, &allocator));
  EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_fini(&dopts));
}

TEST(discovery_options, init_invalid_args) {
  rmw_discovery_options_t dopts = rmw_get_zero_initialized_discovery_options();
  rmw_discovery_options_t dopts_valid = rmw_get_zero_initialized_discovery_options();
  auto allocator = rcutils_get_default_allocator();

  EXPECT_EQ(RMW_RET_INVALID_ARGUMENT, rmw_discovery_options_init(NULL, 0, &allocator));
  EXPECT_EQ(RMW_RET_INVALID_ARGUMENT, rmw_discovery_options_init(&dopts, 0, NULL));
  EXPECT_EQ(RMW_RET_INVALID_ARGUMENT, rmw_discovery_options_init(NULL, 0, NULL));

  EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_init(&dopts_valid, 5, &allocator));
  EXPECT_EQ(RMW_RET_INVALID_ARGUMENT, rmw_discovery_options_init(&dopts_valid, 5, &allocator));

  EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_fini(&dopts_valid));
}

TEST(discovery_options, zero_are_equal) {
  rmw_discovery_options_t dopts1 = rmw_get_zero_initialized_discovery_options();
  rmw_discovery_options_t dopts2 = rmw_get_zero_initialized_discovery_options();

  bool result = false;
  EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_equal(&dopts1, &dopts2, &result));
  EXPECT_TRUE(result);
}

TEST(discovery_options, unequal) {
  auto allocator = rcutils_get_default_allocator();
  {  // Different size static_peers size
    rmw_discovery_options_t dopts1 = rmw_get_zero_initialized_discovery_options();
    rmw_discovery_options_t dopts2 = rmw_get_zero_initialized_discovery_options();

    EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_init(&dopts1, 0, &allocator));
    EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_init(&dopts2, 1, &allocator));

    bool result = true;
    EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_equal(&dopts1, &dopts2, &result));
    EXPECT_FALSE(result);

    EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_fini(&dopts1));
    EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_fini(&dopts2));
  }
  {  // Different discovery range
    rmw_discovery_options_t dopts1 = rmw_get_zero_initialized_discovery_options();
    rmw_discovery_options_t dopts2 = rmw_get_zero_initialized_discovery_options();

    dopts1.automatic_discovery_range = RMW_AUTOMATIC_DISCOVERY_RANGE_LOCALHOST;
    dopts2.automatic_discovery_range = RMW_AUTOMATIC_DISCOVERY_RANGE_OFF;

    bool result = true;
    EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_equal(&dopts1, &dopts2, &result));
    EXPECT_FALSE(result);

    EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_fini(&dopts1));
    EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_fini(&dopts2));
  }
  {  // Different static peer
    rmw_discovery_options_t dopts1 = rmw_get_zero_initialized_discovery_options();
    rmw_discovery_options_t dopts2 = rmw_get_zero_initialized_discovery_options();

    EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_init(&dopts1, 1, &allocator));
    EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_init(&dopts2, 1, &allocator));

    dopts1.static_peers[0].peer_address[0] = 'a';
    dopts1.static_peers[0].peer_address[1] = '\0';
    dopts2.static_peers[0].peer_address[0] = 'b';
    dopts2.static_peers[0].peer_address[1] = '\0';

    bool result = true;
    EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_equal(&dopts1, &dopts2, &result));
    EXPECT_FALSE(result);

    EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_fini(&dopts1));
    EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_fini(&dopts2));
  }
}

TEST(discovery_options, copy_equal) {
  auto allocator = rcutils_get_default_allocator();
  rmw_discovery_options_t dopts1 = rmw_get_zero_initialized_discovery_options();
  rmw_discovery_options_t dopts2 = rmw_get_zero_initialized_discovery_options();

  EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_init(&dopts1, 1, &allocator));

  dopts1.automatic_discovery_range = RMW_AUTOMATIC_DISCOVERY_RANGE_OFF;
  dopts1.static_peers[0].peer_address[0] = 'a';
  dopts1.static_peers[0].peer_address[1] = '\0';

  EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_copy(&dopts1, &allocator, &dopts2));

  EXPECT_EQ(dopts2.automatic_discovery_range, RMW_AUTOMATIC_DISCOVERY_RANGE_OFF);
  EXPECT_EQ(dopts2.static_peers_count, 1);
  EXPECT_EQ(dopts2.static_peers[0].peer_address[0], 'a');

  bool result = false;
  EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_equal(&dopts1, &dopts2, &result));
  EXPECT_TRUE(result);
}

TEST(discovery_options, copy_invalid_args) {
  auto allocator = rcutils_get_default_allocator();
  rmw_discovery_options_t dopts1 = rmw_get_zero_initialized_discovery_options();
  rmw_discovery_options_t dopts2 = rmw_get_zero_initialized_discovery_options();

  EXPECT_EQ(RMW_RET_OK, rmw_discovery_options_init(&dopts1, 1, &allocator));

  EXPECT_EQ(RMW_RET_INVALID_ARGUMENT, rmw_discovery_options_copy(NULL, &allocator, &dopts2));
  EXPECT_EQ(RMW_RET_INVALID_ARGUMENT, rmw_discovery_options_copy(NULL, NULL, &dopts2));
  EXPECT_EQ(RMW_RET_INVALID_ARGUMENT, rmw_discovery_options_copy(NULL, &allocator, NULL));
  EXPECT_EQ(RMW_RET_INVALID_ARGUMENT, rmw_discovery_options_copy(NULL, NULL, NULL));
  EXPECT_EQ(RMW_RET_INVALID_ARGUMENT, rmw_discovery_options_copy(&dopts1, NULL, &dopts2));
  EXPECT_EQ(RMW_RET_INVALID_ARGUMENT, rmw_discovery_options_copy(&dopts1, NULL, NULL));
  EXPECT_EQ(RMW_RET_INVALID_ARGUMENT, rmw_discovery_options_copy(&dopts1, &allocator, NULL));
  EXPECT_EQ(RMW_RET_INVALID_ARGUMENT, rmw_discovery_options_copy(&dopts1, &allocator, &dopts1));
  EXPECT_EQ(RMW_RET_INVALID_ARGUMENT, rmw_discovery_options_copy(&dopts2, &allocator, &dopts1));
  EXPECT_EQ(RMW_RET_INVALID_ARGUMENT, rmw_discovery_options_copy(&dopts2, &allocator, &dopts2));
}

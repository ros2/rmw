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

#include "gtest/gtest.h"

#include "rmw/allocators.h"
#include "rmw/types.h"

TEST(test_rmw_participant_qos_profile_allocator, test_allocate_does_not_return_null) {
  rmw_participant_qos_profile_t * qos_profile = rmw_participant_qos_profile_allocate();
  ASSERT_TRUE(qos_profile != NULL);
}

TEST(test_rmw_participant_qos_profile_allocator, test_allocate_allocates_different_pointers) {
  rmw_participant_qos_profile_t * qos_profile1 = rmw_participant_qos_profile_allocate();
  rmw_participant_qos_profile_t * qos_profile2 = rmw_participant_qos_profile_allocate();
  ASSERT_TRUE(qos_profile1 != qos_profile2);
}

TEST(test_rmw_participant_qos_profile_allocator, test_free_null) {
  EXPECT_NO_THROW(rmw_participant_qos_profile_free(NULL));
}

TEST(test_rmw_participant_qos_profile_allocator, test_free_allocated) {
  rmw_participant_qos_profile_t * qos_profile = rmw_participant_qos_profile_allocate();
  EXPECT_NO_THROW(rmw_participant_qos_profile_free(qos_profile));
}

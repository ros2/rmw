// Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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
#include "osrf_testing_tools_cpp/scope_exit.hpp"
#include "rcutils/allocator.h"

#include "rmw/topic_endpoint_info_array.h"
#include "rmw/types.h"

TEST(test_topic_endpoint_info_array, zero_initialize) {
  rmw_topic_endpoint_info_array_t arr = rmw_get_zero_initialized_topic_endpoint_info_array();
  EXPECT_EQ(arr.count, 0u);
  EXPECT_FALSE(arr.info_array);
}

TEST(test_topic_endpoint_info_array, check_zero) {
  rmw_topic_endpoint_info_array_t arr = rmw_get_zero_initialized_topic_endpoint_info_array();
  EXPECT_EQ(rmw_topic_endpoint_info_array_check_zero(&arr), RMW_RET_OK);
  rmw_topic_endpoint_info_array_t arr_count_not_zero = {1, nullptr};
  EXPECT_EQ(rmw_topic_endpoint_info_array_check_zero(&arr_count_not_zero), RMW_RET_ERROR);
  rmw_topic_endpoint_info_t topic_endpoint_info;
  rmw_topic_endpoint_info_array_t arr_info_array_not_null = {0, &topic_endpoint_info};
  EXPECT_EQ(rmw_topic_endpoint_info_array_check_zero(&arr_info_array_not_null), RMW_RET_ERROR);
  EXPECT_EQ(rmw_topic_endpoint_info_array_check_zero(nullptr), RMW_RET_INVALID_ARGUMENT);
}

TEST(test_topic_endpoint_info_array, check_init_with_size) {
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmw_topic_endpoint_info_array_t arr = rmw_get_zero_initialized_topic_endpoint_info_array();
  OSRF_TESTING_TOOLS_CPP_SCOPE_EXIT(
  {
    rmw_ret_t fini_ret = rmw_topic_endpoint_info_array_fini(&arr, &allocator);
    EXPECT_EQ(fini_ret, RMW_RET_OK);
  });
  EXPECT_EQ(
    rmw_topic_endpoint_info_array_init_with_size(&arr, 1, nullptr),
    RMW_RET_INVALID_ARGUMENT);
  EXPECT_EQ(
    rmw_topic_endpoint_info_array_init_with_size(
      nullptr, 1,
      &allocator), RMW_RET_INVALID_ARGUMENT);
  EXPECT_FALSE(arr.info_array);
  rmw_ret_t ret = rmw_topic_endpoint_info_array_init_with_size(&arr, 5, &allocator);
  EXPECT_EQ(ret, RMW_RET_OK);
  EXPECT_TRUE(arr.info_array);
}

TEST(test_topic_endpoint_info_array, check_fini) {
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmw_topic_endpoint_info_array_t arr = rmw_get_zero_initialized_topic_endpoint_info_array();
  rmw_ret_t ret = rmw_topic_endpoint_info_array_init_with_size(&arr, 5, &allocator);
  EXPECT_EQ(ret, RMW_RET_OK);
  EXPECT_TRUE(arr.info_array);
  ret = rmw_topic_endpoint_info_array_fini(&arr, &allocator);
  EXPECT_EQ(ret, RMW_RET_OK);
  EXPECT_FALSE(arr.info_array);
}

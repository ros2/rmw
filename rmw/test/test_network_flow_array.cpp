// Copyright 2020 Ericsson AB
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

#include "rmw/error_handling.h"
#include "rmw/network_flow_array.h"
#include "rmw/types.h"

namespace
{
void *
dummy_allocate(size_t size, void * state)
{
  RCUTILS_UNUSED(size);
  RCUTILS_UNUSED(state);
  return nullptr;
}
}  // namespace


TEST(test_network_flow_array, zero_init) {
  rmw_network_flow_array_t network_flow_array = rmw_get_zero_initialized_network_flow_array();
  EXPECT_EQ(network_flow_array.size, 0u);
  EXPECT_FALSE(network_flow_array.network_flow);
}

TEST(test_network_flow_array, check_zero) {
  rmw_network_flow_array_t network_flow_array = rmw_get_zero_initialized_network_flow_array();
  EXPECT_EQ(rmw_network_flow_array_check_zero(&network_flow_array), RMW_RET_OK);
  rmw_network_flow_array_t network_flow_array_bad_1 = {1, nullptr};  // size not zero
  EXPECT_EQ(rmw_network_flow_array_check_zero(&network_flow_array_bad_1), RMW_RET_ERROR);
  rmw_reset_error();
  rmw_network_flow_t network_flow;
  rmw_network_flow_array_t network_flow_array_bad_2 = {0, &network_flow};  // network_flow not NULL
  EXPECT_EQ(rmw_network_flow_array_check_zero(&network_flow_array_bad_2), RMW_RET_ERROR);
  rmw_reset_error();
  EXPECT_EQ(rmw_network_flow_array_check_zero(nullptr), RMW_RET_INVALID_ARGUMENT);
  rmw_reset_error();
}

TEST(test_network_flow_array, init) {
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmw_network_flow_array_t network_flow_array = rmw_get_zero_initialized_network_flow_array();
  OSRF_TESTING_TOOLS_CPP_SCOPE_EXIT(
  {
    rmw_ret_t fini_ret = rmw_network_flow_array_fini(&network_flow_array, &allocator);
    EXPECT_EQ(fini_ret, RMW_RET_OK);
  });
  EXPECT_EQ(
    rmw_network_flow_array_init(&network_flow_array, 1, nullptr),
    RMW_RET_INVALID_ARGUMENT);
  rmw_reset_error();
  EXPECT_EQ(
    rmw_network_flow_array_init(
      nullptr, 1,
      &allocator), RMW_RET_INVALID_ARGUMENT);
  EXPECT_FALSE(network_flow_array.network_flow);
  rmw_reset_error();

  rmw_ret_t ret = rmw_network_flow_array_init(&network_flow_array, 10, &allocator);
  EXPECT_EQ(ret, RMW_RET_OK);
  EXPECT_TRUE(network_flow_array.network_flow);

  rcutils_allocator_t bad_allocator = rcutils_get_default_allocator();
  rmw_network_flow_array_t bad_network_flow_array = rmw_get_zero_initialized_network_flow_array();
  bad_allocator.allocate = &dummy_allocate;
  EXPECT_EQ(
    rmw_network_flow_array_init(
      &bad_network_flow_array, 1,
      &bad_allocator), RMW_RET_BAD_ALLOC);
  rmw_reset_error();
}

TEST(test_network_flow_array, fini) {
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  rmw_network_flow_array_t network_flow_array = rmw_get_zero_initialized_network_flow_array();
  rmw_ret_t ret = rmw_network_flow_array_init(&network_flow_array, 10, &allocator);
  EXPECT_EQ(ret, RMW_RET_OK);
  EXPECT_TRUE(network_flow_array.network_flow);
  ret = rmw_network_flow_array_fini(&network_flow_array, &allocator);
  EXPECT_EQ(ret, RMW_RET_OK);
  EXPECT_FALSE(network_flow_array.network_flow);

  EXPECT_EQ(rmw_network_flow_array_fini(&network_flow_array, nullptr), RMW_RET_INVALID_ARGUMENT);
  rmw_reset_error();
  EXPECT_EQ(rmw_network_flow_array_fini(nullptr, &allocator), RMW_RET_INVALID_ARGUMENT);
  rmw_reset_error();
}

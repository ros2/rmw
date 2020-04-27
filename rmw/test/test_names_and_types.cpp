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

#include "rcutils/error_handling.h"
#include "rmw/error_handling.h"
#include "rmw/names_and_types.h"

namespace
{
void *
bad_zero_allocator(size_t number, size_t size, void * state)
{
  RCUTILS_UNUSED(number);
  RCUTILS_UNUSED(size);
  RCUTILS_UNUSED(state);
  return nullptr;
}
}  // namespace


TEST(rmw_names_and_types, get_zero_init)
{
  rmw_names_and_types_t names_and_types = rmw_get_zero_initialized_names_and_types();
  EXPECT_EQ(names_and_types.names.size, 0u);
  EXPECT_EQ(names_and_types.names.data, nullptr);

  EXPECT_EQ(names_and_types.names.allocator.allocate, nullptr);
  EXPECT_EQ(names_and_types.names.allocator.deallocate, nullptr);
  EXPECT_EQ(names_and_types.names.allocator.reallocate, nullptr);
  EXPECT_EQ(names_and_types.names.allocator.zero_allocate, nullptr);
  EXPECT_EQ(names_and_types.names.allocator.state, nullptr);
  EXPECT_EQ(names_and_types.types, nullptr);
}

TEST(rmw_names_and_types, rmw_names_and_types_check_zero) {
  EXPECT_EQ(rmw_names_and_types_check_zero(nullptr), RMW_RET_INVALID_ARGUMENT);
  rmw_names_and_types_t names_and_types;
  rmw_reset_error();

  // Size is not 0
  names_and_types.names.size = 1;
  EXPECT_EQ(rmw_names_and_types_check_zero(&names_and_types), RMW_RET_INVALID_ARGUMENT);
  names_and_types.names.size = 0;
  rmw_reset_error();

  // data is not null
  char s[20] = "I'm a string!";
  char * data[2] = {&s[0], &s[1]};
  names_and_types.names.data = &data[0];
  EXPECT_EQ(rmw_names_and_types_check_zero(&names_and_types), RMW_RET_INVALID_ARGUMENT);
  names_and_types.names.data = nullptr;
  rcutils_reset_error();

  // types is not null
  rcutils_string_array_t string_array;
  names_and_types.types = &string_array;
  EXPECT_EQ(rmw_names_and_types_check_zero(&names_and_types), RMW_RET_INVALID_ARGUMENT);
  names_and_types.types = nullptr;
  rmw_reset_error();

  // OK
  names_and_types = rmw_get_zero_initialized_names_and_types();
  EXPECT_EQ(rmw_names_and_types_check_zero(&names_and_types), RMW_RET_OK);
}

TEST(rmw_names_and_types, rmw_names_and_types_init) {
  rmw_names_and_types_t names_and_types;
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  size_t size = 100;

  // allocator is null
  rmw_ret_t result = rmw_names_and_types_init(&names_and_types, size, nullptr);
  EXPECT_EQ(result, RMW_RET_INVALID_ARGUMENT);
  rmw_reset_error();

  // names_and_types is null
  result = rmw_names_and_types_init(nullptr, size, &allocator);
  EXPECT_EQ(result, RMW_RET_INVALID_ARGUMENT);
  rmw_reset_error();

  // allocator fails to allocate memory to names
  allocator.zero_allocate = &bad_zero_allocator;
  result = rmw_names_and_types_init(&names_and_types, size, &allocator);
  EXPECT_EQ(result, RMW_RET_BAD_ALLOC);
  allocator = rcutils_get_default_allocator();
  rmw_reset_error();

  // Fails to deallocate after failing to zero allocate types
  allocator.zero_allocate = &bad_zero_allocator;
  allocator.deallocate = nullptr;
  result = rmw_names_and_types_init(&names_and_types, size, &allocator);
  EXPECT_EQ(result, RMW_RET_BAD_ALLOC);
  allocator = rcutils_get_default_allocator();
  rmw_reset_error();

  result = rmw_names_and_types_init(&names_and_types, size, &allocator);
  EXPECT_EQ(result, RMW_RET_OK);
}

TEST(rmw_names_and_types, rmw_names_and_types_fini) {
  rmw_names_and_types_t names_and_types = rmw_get_zero_initialized_names_and_types();
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  size_t size = 100;

  rmw_ret_t result = rmw_names_and_types_init(&names_and_types, size, &allocator);
  EXPECT_EQ(result, RMW_RET_OK);

  // names_and_types is nullptr
  EXPECT_EQ(rmw_names_and_types_fini(nullptr), RMW_RET_INVALID_ARGUMENT);
  rmw_reset_error();

  // Ok
  EXPECT_EQ(rmw_names_and_types_fini(&names_and_types), RMW_RET_OK);

  // Size != 0 and types is null
  auto types_ptr = names_and_types.types;
  names_and_types.types = nullptr;
  EXPECT_EQ(rmw_names_and_types_fini(&names_and_types), RMW_RET_INVALID_ARGUMENT);
  rmw_reset_error();
  names_and_types.types = types_ptr;

  // bad allocator, rcutils fails to finalize string array
  names_and_types.names.allocator.deallocate = nullptr;
  EXPECT_EQ(rmw_names_and_types_fini(&names_and_types), RMW_RET_INVALID_ARGUMENT);
  rmw_reset_error();
}

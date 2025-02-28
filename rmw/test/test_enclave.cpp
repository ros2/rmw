// Copyright 2025 Sony Group Corporation.
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

#include "./time_bomb_allocator_testing_utils.h"
#include "rmw/enclave.h"
#include "rmw/error_handling.h"

TEST(rmw_enclave_options, options_copy) {
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  char * dst = nullptr;
  EXPECT_EQ(RMW_RET_INVALID_ARGUMENT, rmw_enclave_options_copy(nullptr, &allocator, &dst));
  rmw_reset_error();

  EXPECT_EQ(RMW_RET_INVALID_ARGUMENT, rmw_enclave_options_copy("src", nullptr, &dst));
  rmw_reset_error();

  EXPECT_EQ(RMW_RET_INVALID_ARGUMENT, rmw_enclave_options_copy("src", &allocator, nullptr));
  rmw_reset_error();

  EXPECT_EQ(RMW_RET_OK, rmw_enclave_options_copy("src", &allocator, &dst));
  EXPECT_STREQ(dst, "src");
  EXPECT_EQ(RMW_RET_OK, rmw_enclave_options_fini(dst, &allocator));
}

TEST(rmw_enclave_options, options_fini) {
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  char * dst = nullptr;
  EXPECT_EQ(RMW_RET_OK, rmw_enclave_options_copy("src", &allocator, &dst));
  EXPECT_STREQ(dst, "src");

  EXPECT_EQ(RMW_RET_INVALID_ARGUMENT, rmw_enclave_options_fini(nullptr, &allocator));

  EXPECT_EQ(RMW_RET_INVALID_ARGUMENT, rmw_enclave_options_fini(dst, nullptr));

  EXPECT_EQ(RMW_RET_OK, rmw_enclave_options_fini(dst, &allocator));
}

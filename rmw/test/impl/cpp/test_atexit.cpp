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

#include "rmw/impl/cpp/atexit.hpp"

TEST(test_atexit, should_run) {
  bool called = false;

  {
    rmw::impl::cpp::atexit{[&called]() {
        called = true;
      }};
  }

  EXPECT_TRUE(called);
}

TEST(test_atexit, should_not_run) {
  bool called = false;

  {
    rmw::impl::cpp::atexit test{[&called]() {
        called = true;
      }};
    test.cancel();
  }

  EXPECT_FALSE(called);
}

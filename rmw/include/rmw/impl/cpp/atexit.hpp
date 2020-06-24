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

#ifndef RMW__IMPL__CPP__ATEXIT_HPP_
#define RMW__IMPL__CPP__ATEXIT_HPP_

#include <functional>
#include <utility>

namespace rmw
{
namespace impl
{
namespace cpp
{

class atexit final
{
public:
  using callable_type = std::function<void (void)>;

  explicit atexit(callable_type callable)
  : callable_(std::move(callable))
  {
  }

  atexit(const atexit & other) = delete;
  atexit(atexit && other) = delete;
  atexit & operator=(const atexit & other) = delete;
  atexit & operator=(atexit && other) = delete;

  ~atexit()
  {
    if (callable_) {
      callable_();
    }
  }

  void cancel()
  {
    callable_ = nullptr;
  }

private:
  callable_type callable_;
};

}  // namespace cpp
}  // namespace impl
}  // namespace rmw

#endif  // RMW__IMPL__CPP__ATEXIT_HPP_

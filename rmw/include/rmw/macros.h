// Copyright 2015 Open Source Robotics Foundation, Inc.
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

#ifndef RMW__MACROS_H_
#define RMW__MACROS_H_

#if defined _WIN32 || defined __CYGWIN__
  #define RMW_THREAD_LOCAL __declspec(thread)
#elif defined __APPLE__
  #include <Availability.h>
  #if defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
    // iOS 10 added support for thread local storage
    #if __IPHONE_OS_VERSION_MAX_ALLOWED < 100000
      #define RMW_THREAD_LOCAL_PTHREAD 1
      #define RMW_THREAD_LOCAL
    #endif
  #endif

  #ifndef RMW_THREAD_LOCAL_PTHREAD
    #define RMW_THREAD_LOCAL _Thread_local
  #endif
#else
  #define RMW_THREAD_LOCAL _Thread_local
#endif

#define RMW_STRINGIFY_IMPL(x) #x
#define RMW_STRINGIFY(x) RMW_STRINGIFY_IMPL(x)

#ifndef _WIN32
#define RMW_WARN_UNUSED __attribute__((warn_unused_result))
#else
#define RMW_WARN_UNUSED _Check_return_
#endif

#endif  // RMW__MACROS_H_

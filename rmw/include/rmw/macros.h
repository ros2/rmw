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

// This block either sets RMW_THREAD_LOCAL or RMW_THREAD_LOCAL_PTHREAD.
#if defined _WIN32 || defined __CYGWIN__
// Windows or Cygwin
  #define RMW_THREAD_LOCAL __declspec(thread)
#elif defined __APPLE__
// Apple OS's
  #include <TargetConditionals.h>
  #if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
// iOS Simulator or iOS device
    #include <Availability.h>
    #if defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
      #if __IPHONE_OS_VERSION_MAX_ALLOWED >= 100000
// iOS >= 10, thread local storage was added in iOS 10
        #define RMW_THREAD_LOCAL _Thread_local
      #else
// iOS < 10, no thread local storage, so use pthread instead
        #define RMW_THREAD_LOCAL_PTHREAD 1
        #undef RMW_THREAD_LOCAL
      #endif
    #else
      #error "Unknown iOS version"
    #endif
  #elif TARGET_OS_MAC
// macOS
    #define RMW_THREAD_LOCAL _Thread_local
  #else
    #error "Unknown Apple platform"
  #endif
#else
// Some other non-Windows, non-cygwin, non-apple OS
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

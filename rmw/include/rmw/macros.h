/* Copyright 2015 Open Source Robotics Foundation, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef RMW_RMW_MACROS_H_
#define RMW_RMW_MACROS_H_

#if defined _WIN32 || defined __CYGWIN__
  #define RMW_THREAD_LOCAL __declspec( thread )
#else
  #define RMW_THREAD_LOCAL __thread
#endif

#endif  // RMW_RMW_MACROS_H_
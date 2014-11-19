/* Copyright 2014 Open Source Robotics Foundation, Inc.
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

#ifndef ROS_MIDDLEWARE_INTERFACE_ROS_MIDDLEWARE_INTERFACE_VISIBILITY_CONTROL_H_
#define ROS_MIDDLEWARE_INTERFACE_ROS_MIDDLEWARE_INTERFACE_VISIBILITY_CONTROL_H_

#if __cplusplus
extern "C"
{
#endif

#if defined _WIN32 || defined __CYGWIN__
  #ifdef RMW_BUILDING_DLL
    #ifdef __GNUC__
      #define RMW_PUBLIC __attribute__ ((dllexport))
    #else
      #define RMW_PUBLIC __declspec(dllexport)
    #endif
  #else
    #ifdef __GNUC__
      #define RMW_PUBLIC __attribute__ ((dllimport))
    #else
      #define RMW_PUBLIC __declspec(dllimport)
    #endif
  #endif
  #define RMW_LOCAL
#else
  #if __GNUC__ >= 4
    #define RMW_PUBLIC __attribute__ ((visibility ("default")))
    #define RMW_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define RMW_PUBLIC
    #define RMW_LOCAL
  #endif
#endif

#if __cplusplus
}
#endif

#endif  /* ROS_MIDDLEWARE_INTERFACE_ROS_MIDDLEWARE_INTERFACE_VISIBILITY_CONTROL_H_ */

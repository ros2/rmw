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

#ifndef RMW__LISTENER_CALLBACK_TYPE_H_
#define RMW__LISTENER_CALLBACK_TYPE_H_

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (* rmw_listener_callback_t)(const void * user_data);

#ifdef __cplusplus
}
#endif

#endif  // RMW__LISTENER_CALLBACK_TYPE_H_

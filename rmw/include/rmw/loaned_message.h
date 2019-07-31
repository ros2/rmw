// Copyright 2019 Open Source Robotics Foundation, Inc.
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

#ifdef __cplusplus
extern "C"
{
#endif

/// Loaned message allocated by the middleware containing a ros message to be published.
/**
 * A loaned message contains a ros message which was previously allocated by the middleware.
 * The memory allocated belongs to the middleware and must not be destroyed outside of it.
 */
typedef struct RMW_PUBLIC_TYPE rmw_loaned_message_t
{
  /// A unique string identifying the middleware
  const char * implementation_identifier;
  /// The type erased memory for the ros message
  void * ros_message;
  /// Extra data which might be necessary by the middleware
  void * data;
} rmw_loaned_message_t;


// get_zero_initializd
// rmw_init
// rmw_fini
#ifdef __cplusplus
}
#endif

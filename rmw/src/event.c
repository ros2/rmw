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

#include <stddef.h>

#include "rmw/error_handling.h"
#include "rmw/event.h"

#ifdef __cplusplus
extern "C" {
#endif

rmw_event_t
rmw_get_zero_initialized_event(void)
{
  const rmw_event_t event = {
    .implementation_identifier = NULL,
    .data = NULL,
    .event_type = RMW_EVENT_INVALID
  };  // NOLINT(readability/braces): false positive
  return event;
}

rmw_ret_t
__rmw_init_event(
  rmw_event_t * rmw_event,
  const char * implementation_identifier,
  void * data,
  const rmw_event_type_t event_type)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(implementation_identifier, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(data, RMW_RET_INVALID_ARGUMENT);
  RMW_CHECK_ARGUMENT_FOR_NULL(rmw_event, RMW_RET_INVALID_ARGUMENT);
  if (NULL != rmw_event->implementation_identifier) {
    RMW_SET_ERROR_MSG("expected zero-initialized rmw_event");
    return RMW_RET_INVALID_ARGUMENT;
  }
  rmw_event->implementation_identifier = implementation_identifier;
  rmw_event->data = data;
  rmw_event->event_type = event_type;
  return RMW_RET_OK;
}

rmw_ret_t
rmw_publisher_event_init(
  rmw_event_t * rmw_event,
  const rmw_publisher_t * publisher,
  rmw_event_type_t event_type)
{
  return __rmw_init_event(
    rmw_event,
    publisher->implementation_identifier,
    publisher->data,
    event_type);
}

rmw_ret_t
rmw_subscription_event_init(
  rmw_event_t * rmw_event,
  const rmw_subscription_t * subscription,
  rmw_event_type_t event_type)
{
  return __rmw_init_event(
    rmw_event,
    subscription->implementation_identifier,
    subscription->data,
    event_type);
}

rmw_ret_t
rmw_event_fini(rmw_event_t * rmw_event)
{
  RMW_CHECK_ARGUMENT_FOR_NULL(rmw_event, RMW_RET_INVALID_ARGUMENT);
  *rmw_event = rmw_get_zero_initialized_event();
  return RMW_RET_OK;
}

#ifdef __cplusplus
}
#endif

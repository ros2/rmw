// Copyright 2017 Open Source Robotics Foundation, Inc.
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

#include <rmw/impl/validate_topic_name.h>

#include <ctype.h>
#include <string.h>

rmw_ret_t
rmw_impl_validate_topic_name(
  const char * topic_name,
  int * validation_result,
  size_t * invalid_index)
{
  if (!topic_name) {
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (!validation_result) {
    return RMW_RET_INVALID_ARGUMENT;
  }
  if (!invalid_index) {
    return RMW_RET_INVALID_ARGUMENT;
  }
  size_t topic_name_length = strlen(topic_name);
  if (topic_name_length == 0) {
    *validation_result = RMW_IMPL_INVALID_TOPIC_IS_EMPTY_STRING;
    *invalid_index = 0;
    return RMW_RET_OK;
  }
  if (topic_name[0] != '/') {
    *validation_result = RMW_IMPL_INVALID_TOPIC_NOT_ABSOLUTE;
    *invalid_index = 0;
    return RMW_RET_OK;
  }
  // note topic_name_length is >= 1 at this point
  if (topic_name[topic_name_length - 1] == '/') {
    // catches both "/foo/" and "/"
    *validation_result = RMW_IMPL_INVALID_TOPIC_ENDS_WITH_FORWARD_SLASH;
    *invalid_index = topic_name_length - 1;
    return RMW_RET_OK;
  }
  // check for unallowed characters
  for (size_t i = 0; i < topic_name_length; ++i) {
    // TODO(wjwwood): isalnum is affected by the locale, need to see if this is an issue for us
    if (isalnum(topic_name[i]) != 0) {
      // if it is an alpha numeric character, i.e. [0-9,a-z,A-Z], continue
      continue;
    } else if (topic_name[i] == '_') {
      // if it is an underscore, continue
      continue;
    } else if (topic_name[i] == '/') {
      // if it is a forward slash, continue
      continue;
    } else {
      // if it is none of these, then it is an unallowed character in a FQN topic name
      *validation_result = RMW_IMPL_INVALID_TOPIC_CONTAINS_UNALLOWED_CHARACTERS;
      *invalid_index = i;
      return RMW_RET_OK;
    }
  }
  // check for double '/' and tokens that start with a number
  for (size_t i = 0; i < topic_name_length; ++i) {
    if (i == topic_name_length - 1) {
      // if this is the last character, then nothing to check
      continue;
    }
    // past this point, assuming i+1 is a valid index
    if (topic_name[i] == '/') {
      if (topic_name[i + 1] == '/') {
        *validation_result = RMW_IMPL_INVALID_TOPIC_CONTAINS_REPEATED_FORWARD_SLASH;
        *invalid_index = i + 1;
        return RMW_RET_OK;
      }
      if (isdigit(topic_name[i + 1]) != 0) {
        // this is the case where a '/' if followed by a number, i.e. [0-9]
        *validation_result = RMW_IMPL_INVALID_TOPIC_NAME_TOKEN_STARTS_WITH_NUMBER;
        *invalid_index = i + 1;
        return RMW_RET_OK;
      }
    }
  }
  // check if the topic name is too long last, since it might be a soft invalidation
  if (topic_name_length > RMW_IMPL_MAX_TOPIC_NAME_LENGTH) {
    *validation_result = RMW_IMPL_INVALID_TOPIC_TOO_LONG;
    *invalid_index = RMW_IMPL_MAX_TOPIC_NAME_LENGTH - 1;
    return RMW_RET_OK;
  }
  // everything was ok, set result to valid topic, avoid setting invalid_index, and return
  *validation_result = RMW_IMPL_VALID_TOPIC;
  return RMW_RET_OK;
}

const char *
rmw_impl_validation_result_string(int validation_result)
{
  switch (validation_result) {
    case RMW_IMPL_VALID_TOPIC:
      return "topic name is valid";
    case RMW_IMPL_INVALID_TOPIC_IS_EMPTY_STRING:
      return "topic name must not be empty";
    case RMW_IMPL_INVALID_TOPIC_NOT_ABSOLUTE:
      return "topic name must be absolute, it must lead with a '/'";
    case RMW_IMPL_INVALID_TOPIC_ENDS_WITH_FORWARD_SLASH:
      return "topic name must not end with a '/'";
    case RMW_IMPL_INVALID_TOPIC_CONTAINS_UNALLOWED_CHARACTERS:
      return "topic name must not contain characters other than alphanumerics, '_', or '/'";
    case RMW_IMPL_INVALID_TOPIC_CONTAINS_REPEATED_FORWARD_SLASH:
      return "topic name must not contain repeated '/'";
    case RMW_IMPL_INVALID_TOPIC_NAME_TOKEN_STARTS_WITH_NUMBER:
      return "topic name must not have a token that starts with a number";
    case RMW_IMPL_INVALID_TOPIC_TOO_LONG:
      return "topic length should not exceed '" RMW_STRINGIFY(RMW_IMPL_MAX_TOPIC_NAME_LENGTH) "'";
    default:
      return NULL;
  }
}

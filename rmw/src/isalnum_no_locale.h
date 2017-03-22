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

#ifndef ISALNUM_NO_LOCALE_H_
#define ISALNUM_NO_LOCALE_H_

#if __cplusplus
extern "C"
{
#endif

/// Custom isalnum which is not affected by locale.
static inline
bool
isalnum_no_locale(char c)
{
  // if in '0', ..., '9', then ok
  if (c >= 0x30 /*0*/ && c <= 0x39 /*9*/) {
    return true;
  }
  // if in 'A', ..., 'Z', then ok
  if (c >= 0x41 /*A*/ && c <= 0x5a /*Z*/) {
    return true;
  }
  // if in 'a', ..., 'z', then ok
  if (c >= 0x61 /*a*/ && c <= 0x7a /*z*/) {
    return true;
  }
  return false;
}

#if __cplusplus
}
#endif

#endif  // ISALNUM_NO_LOCALE_H_

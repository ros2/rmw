# Copyright 2014 Open Source Robotics Foundation, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# copied from rmw/rmw-extras.cmake

include("${rmw_DIR}/configure_rmw_library.cmake")
include("${rmw_DIR}/get_rmw_typesupport.cmake")
include("${rmw_DIR}/register_rmw_implementation.cmake")

# TODO(wjwwood): we have to fix linking with unresolved symbols on Windows before removing this.
if(WIN32)
  find_package(ament_cmake REQUIRED)  # Without this find_package(rmw_implementation_cmake) fails.
  find_package(rmw_implementation_cmake REQUIRED)
  get_available_rmw_implementations(_rmw_implementations)
  list(LENGTH _rmw_implementations _len)
  # If there are no middleware yet, then continue.
  # This happends when the first middleware implementation is built.
  if(${_len} EQUAL 0)
    return()
  endif()
  # Ensure only one middleware is available.
  # If more than one is available error out in order to prevent other packages from trying
  # to build libraries and executables for more than one rmw implementation at a time.
  if(${_len} GREATER 1)
    message(FATAL_ERROR
      "Only one rmw implementation per workspace is supported on Windows, but ${_len} were found."
    )
  endif()
  # Use get_default_rmw_implementation to export the default middleware as a transitive dependency.
  get_default_rmw_implementation(_middleware_implementation)
  find_package(${_middleware_implementation} REQUIRED)
  list(APPEND rmw_DEFINITIONS ${${_middleware_implementation}_DEFINITIONS})
  list(APPEND rmw_INCLUDE_DIRS ${${_middleware_implementation}_INCLUDE_DIRS})
  list(APPEND rmw_LIBRARIES ${${_middleware_implementation}_LIBRARIES})
endif()

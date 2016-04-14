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

#
# Register the current package as a ROS middleware implementation.
# Takes as input a list of <language:typesupport> pairs where language is the
# language of the typesupport package and typesupport is the name of the
# package.
#
# For example, a valid usage of this function would be
# register_rmw_implementation("c:rosidl_typesupport_introspection_c"
#                             "cpp:rosidl_typesupport_introspection_cpp")
# If there were multiple valid typesupports for one language in a package,
# they would be separated by colons, e.g.
# register_rmw_implementation(
#   "cpp:rosidl_typesupport_a_cpp:rosidl_typesupport_b_cpp")
# If there are multiple inputs with the same language flag (first entry) the
# later inputs will overwrite the older ones.
#
# @public
#
# String replace colon with semicolon to create list out of the pairs
function(register_rmw_implementation)
  if("${ARGN}" STREQUAL "")
    message(FATAL_ERROR "register_rmw_implementation() called with no arguments!")
  endif()

  # Get the length of ARGN
  set(argn ${ARGN})
  set(all_typesupports "")

  foreach(input ${argn})
    string(TOLOWER ${input} input)
    # replace colon with semicolon to turn into a list
    string(REGEX REPLACE ":/;" input ${input})
    list(LENGTH ${input} input_length)
    if(${input_length})
    endif()
    list(GET ${input} 0 language_label)
    list(REMOVE_AT ${input} 0)
    ament_index_register_resource(
      "rmw_typesupport_${language_label}" "${input}"
    )
    list_append_unique(${all_typesupports} ${input})
  endforeach()

  ament_index_register_resource("rmw_typesupport" "${all_typesupports}")
endfunction()

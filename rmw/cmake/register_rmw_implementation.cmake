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
# Helper function to append items to an ament resource list, or create a new
# entry for the resource if no entry exists.
#
# :param _ARG_C: Name of the C typesupport package for the rmw implementation.
#
function(ament_register_resource_append_if_found resource_type)
  if("${ARGN} " STREQUAL " ")
    message(FATAL_ERROR "register_rmw_implementation() called with no arguments!")
  endif()
  ament_index_has_resource(HAS_RESOURCE "${resource_type}" "${PROJECT_NAME}")
  if(${HAS_RESOURCE})
    # Get the entry and append it to a list
    list_append_unique(rmw_typesupport ${CONTENT}})
    ament_index_register_resource("${resource_type}" CONTENT "${ARGN}")
  else()
    ament_index_register_resource("${resource_type}" CONTENT "${ARGN}")
  endif()
endfunction()

#
# Register the current package as a ROS middleware implementation.
# register_rmw_implementation must be called with an even number of arguments,
# greater than or equal to 2.
# The arguments specify the type support packages for this rmw implementation.
# The name of each type support package is preceded by a label for the language
# of this type support package.
# For example, a valid usage of this function would be
# register_rmw_implementation(c rosidl_typesupport_introspection_c
#                             cpp rosidl_typesupport_introspection_cpp)
#
# :param _ARG_LANGUAGE: Name of the language represented by this typesupport package
# :param _ARG_CONTENT: Name of the typesupport package.
#
# @public
#
macro(register_rmw_implementation)
  if("${ARGN} " STREQUAL " ")
    message(FATAL_ERROR "register_rmw_implementation() called with no arguments!")
  endif()

  # Get the length of ARGN
  #message(STATUS "Got ${ARGN}")
  set(argn ${ARGN})
  list(LENGTH argn num_args)
  # Error if ARGN has an odd number of arguments
  math(EXPR parity "${num_args} % 2")
  if(NOT ${parity} EQUAL 0)
    message(FATAL_ERROR "register_rmw_implementation() called with odd number of arguments!")
  endif()

    math(EXPR num_args "${num_args}-1")
  foreach(idx RANGE 0 ${num_args} 2)
    list(GET argn ${idx} LABEL)
    # The key should not be case sensitive
    string(TOLOWER ${LABEL} language_label)
    math(EXPR next_idx "${idx}+1")
    list(GET argn ${next_idx} CONTENT)
    ament_register_resource_append_if_found(
      "rmw_typesupport_${language_label}"
      "${CONTENT}")
    ament_register_resource_append_if_found("rmw_typesupport" "${CONTENT}")
  endforeach()

endmacro()

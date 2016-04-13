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
# register_rmw_implementation must be called with one or more arguments.
# The arguments each must be specified with flags corresonding to their type
# e.g. C specifies a C typesupport, CPP specifies a C++ typesupport.
#
# :param _ARG_C: Name of the C typesupport package for the rmw implementation.
# :param _ARG_CPP: Name of the C++ typesupport package.
# :type _ARG_C: string
# :type _ARG_CPP: string
#
# @public
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

macro(register_rmw_implementation)
  if("${ARGN} " STREQUAL " ")
    message(FATAL_ERROR "register_rmw_implementation() called with no arguments!")
  endif()

  cmake_parse_arguments(_ARG
    ""
    ""
    "C;CPP"
    ${ARGN}
  )
  if(_ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR
      "register_rmw_implementation() called with unused arguments:
      ${_ARG_UNPARSED_ARGUMENTS}")
  endif()
  if(_ARG_C)
    ament_register_resource_append_if_found("rmw_typesupport_c" "${_ARG_C}")
    ament_register_resource_append_if_found("rmw_typesupport" "${_ARG_C}")
  endif()

  if(_ARG_CPP)
    ament_register_resource_append_if_found("rmw_typesupport_cpp" "${_ARG_CPP}")
    ament_register_resource_append_if_found("rmw_typesupport" "${_ARG_CPP}")
  endif()

endmacro()

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
macro(register_rmw_implementation)
  if("${ARGN} " STREQUAL " ")
    message(FATAL_ERROR "register_rmw_implementation() called with no arguments!")
  endif()

  cmake_parse_arguments(_ARG
    "TRACE"
    ""
    "C;CPP"
    ${ARGN}
  )
  if(_ARG_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR
      "register_rmw_implementation() called with unused arguments:\
      ${_ARG_UNPARSED_ARGUMENTS}")
  endif()
  if(_ARG_C)
    ament_index_register_resource("rmw_typesupport_c" CONTENT "${_ARG_C}")
    ament_index_register_resource("rmw_typesupport" CONTENT "${_ARG_C}")
  endif()

  if(_ARG_CPP)
    ament_index_register_resource("rmw_typesupport_cpp" CONTENT "${_ARG_CPP}")
    ament_index_register_resource("rmw_typesupport" CONTENT "${_ARG_CPP}")
  endif()

endmacro()

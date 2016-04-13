# Copyright 2015 Open Source Robotics Foundation, Inc.
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
# Get type support package name(s) for specific ROS middleware implementation.
#
# :param rmw_implementation: the package name of the ROS middleware
#   implementation
# :type rmw_implementation: string
# :param language: Optional flag specifying the language of the type support to get.
#
# @public
#
macro(get_rmw_typesupport var rmw_implementation)
  cmake_parse_arguments(_ARG
    ""
    "LANGUAGE"
    ""
    ${ARGN}
  )
  if("${_ARG_LANGUAGE}" STREQUAL "C")
    ament_index_get_resource(${var} "rmw_typesupport_c" ${rmw_implementation})
  elseif("${_ARG_LANGUAGE}" STREQUAL "CPP")
    ament_index_get_resource(${var} "rmw_typesupport_cpp" ${rmw_implementation})
  else()
    # This output will include type supports for all languages.
    ament_index_get_resource(${var} "rmw_typesupport" ${rmw_implementation})
  endif()
endmacro()

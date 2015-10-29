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
# Add executables using the default rmw implementation as well as each
# available rmw implementation.
#
# The executable using the default rmw implementation has the given target
# name.
# The other executables use the target name as the prefix and `__<rmw_name>` as
# the suffix.
#
# :param target: the name of the executable target
# :type target: string
# :param ARGN: the source files
# :type ARGN: list of strings
# :param TARGET_DEPENDENCIES: the package names passed to
#   `ament_target_dependencies` for each target
# :type TARGET_DEPENDENCIES: list of strings
# :param ALL_TARGET_NAMES_VAR: the output variable name containing all target
#   names
# :type ALL_TARGET_NAMES_VAR: string
# :param SKIP_DEFAULT: if set skip the target without a rmw implementation
#   suffix
# :type SKIP_DEFAULT: option
# :param INSTALL: if set install all executables
# :type INSTALL: option
#
macro(add_executable_for_each_rmw_implementations)
  # get available rmw implementations
  get_available_rmw_implementations(_rmw_implementations)
  foreach(_rmw_implementation ${_rmw_implementations})
    find_package("${_rmw_implementation}" REQUIRED)
  endforeach()
  _add_executable_for_each_rmw_implementations("${_rmw_implementations}" ${ARGN})
endmacro()

function(_add_executable_for_each_rmw_implementations rmw_implementations target)
  cmake_parse_arguments(ARG
    "INSTALL;SKIP_DEFAULT" "ALL_TARGET_NAMES_VAR" "TARGET_DEPENDENCIES" ${ARGN})

  set(targets "")
  if(NOT ARG_SKIP_DEFAULT)
    # add executable with the default rmw implementation
    add_executable(${target} ${ARG_UNPARSED_ARGUMENTS})
    ament_target_dependencies(${target}
      "rmw_implementation"
      ${ARG_TARGET_DEPENDENCIES})
    if(ARG_INSTALL)
      install(TARGETS ${target} DESTINATION bin)
    endif()
    list(APPEND targets "${target}")
  endif()

  # add executable for each rmw implementation
  foreach(rmw_implementation ${rmw_implementations})
    add_executable(${target}__${rmw_implementation} ${ARG_UNPARSED_ARGUMENTS})
    ament_target_dependencies(${target}__${rmw_implementation}
      "${rmw_implementation}"
      ${ARG_TARGET_DEPENDENCIES})
    if(ARG_INSTALL)
      install(TARGETS ${target}__${rmw_implementation} DESTINATION bin)
    endif()
    list(APPEND targets "${target}__${rmw_implementation}")
  endforeach()

  if(ARG_ALL_TARGET_NAMES_VAR)
    set(${ARG_ALL_TARGET_NAMES_VAR} "${targets}" PARENT_SCOPE)
  endif()
endfunction()

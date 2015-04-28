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
#
# :param typesupport_package_name: the package name providing type support for
#   for the registered RMW implementation
# :type typesupport_package_name: string
#
# @public
#
macro(register_rmw_implementation typesupport_package_name)
  if(NOT "${ARGN} " STREQUAL " ")
    message(FATAL_ERROR "register_rmw_implementation() called with "
      "unused arguments: ${ARGN}")
  endif()

  ament_index_register_resource("rmw_implementation" CONTENT "${typesupport_package_name}")
endmacro()

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
# Get the package names of the available ROS middleware implemenations.
#
# :param var: the output variable name containing the package names
# :type var: list of strings
#
function(get_available_rmw_implementations var)
  ament_index_get_resources(middleware_implementations "rmw_typesupport")
  if(DEFINED middleware_implementations)
    list(SORT middleware_implementations)
  endif()
  set(${var} ${middleware_implementations} PARENT_SCOPE)
endfunction()

#
# Register the current package as a ROS middleware implementation.
#
# @public
#
macro(register_ros_middleware_implementation)
  if(NOT "${ARGN} " STREQUAL " ")
    message(FATAL_ERROR "register_ros_middleware_implementation() called with "
      "unused arguments: ${ARGN}")
  endif()

  ament_index_register_resource("ros_middleware_implementation")
endmacro()

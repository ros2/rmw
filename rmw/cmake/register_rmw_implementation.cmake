#
# Register the current package as a ROS middleware implementation.
#
# @public
#
macro(register_rmw_implementation)
  if(NOT "${ARGN} " STREQUAL " ")
    message(FATAL_ERROR "register_rmw_implementation() called with "
      "unused arguments: ${ARGN}")
  endif()

  ament_index_register_resource("rmw_implementation")
endmacro()

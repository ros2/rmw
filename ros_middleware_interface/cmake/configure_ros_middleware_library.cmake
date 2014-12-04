#
# Configures a library which implements the rmw interface with custom settings.
#
# This should be called on any library which is built to implement the rmw API.
# The custom settings are all related to library symbol visibility, see:
#
# https://gcc.gnu.org/wiki/Visibility
# http://www.ibm.com/developerworks/aix/library/au-aix-symbol-visibility/
#
# Thought about using:
#
# http://www.cmake.org/cmake/help/v2.8.8/cmake.html#module:GenerateExportHeader
#
# But it doesn't seem to set the compiler flags correctly on clang and
# also doesn't work very well when the headers and library are in
# different projects like this
#
# @public
#
macro(configure_ros_middleware_library library_target)
  if(TARGET ros_middleware_opensplice_cpp)
    # Set the visibility to hidden by default if possible
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
       "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
      # Set the visibility of symbols to hidden by default for gcc and clang
      # (this is already the default on Windows)
      set_target_properties(ros_middleware_opensplice_cpp
        PROPERTIES
          COMPILE_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden -std=c++11"
      )
    endif()
    if(WIN32)
      # Causes the visibility macros to use dllexport rather than dllimport
      # which is appropriate when building the dll but not consuming it.
      set_target_properties(ros_middleware_opensplice_cpp
        PROPERTIES COMPILE_FLAGS "-DRMW_BUILDING_DLL")
    endif()
  elseif(TARGET ros_middleware_connext_cpp)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
       "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
      # Set the visibility of symbols to hidden by default for gcc and clang
      # (this is already the default on Windows)
      set_target_properties(ros_middleware_connext_cpp
        PROPERTIES
          COMPILE_FLAGS "-fvisibility=hidden -fvisibility-inlines-hidden -std=c++11"
      )
    endif()
  endif()
endmacro()

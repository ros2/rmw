cmake_minimum_required(VERSION 3.5)

project(rmw)

if(NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

find_package(ament_cmake_ros REQUIRED)

include(cmake/configure_rmw_library.cmake)

include_directories(include)

set(rmw_sources
  "src/allocators.c"
  "src/error_handling.c"
  "src/validate_namespace.c"
  "src/validate_node_name.c"
  "src/validate_topic_name.c"
  "src/sanity_checks.c")
set_source_files_properties(
  ${rmw_sources}
  PROPERTIES LANGUAGE "C")
add_library(${PROJECT_NAME} ${rmw_sources})
configure_rmw_library(${PROJECT_NAME} LANGUAGE "C")

ament_export_dependencies(rosidl_generator_c)
ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})

if(IOS AND IOS_SDK_VERSION LESS 10.0)
  ament_export_libraries(pthread)
endif()

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()

  add_subdirectory(test)
endif()

ament_package(CONFIG_EXTRAS "rmw-extras.cmake")

install(
  DIRECTORY cmake
  DESTINATION share/${PROJECT_NAME}
)
install(
  DIRECTORY include/
  DESTINATION include
)
install(
  TARGETS ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

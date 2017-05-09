cmake_minimum_required(VERSION 3.5)

project(rmw)

if(NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

find_package(ament_cmake_ros REQUIRED)

find_package(rcutils REQUIRED)

include(cmake/configure_rmw_library.cmake)

include_directories(include)

set(rmw_sources
  "src/allocators.c"
  "src/error_handling.c"
  "src/sanity_checks.c"
  "src/validate_full_topic_name.c"
  "src/validate_namespace.c"
  "src/validate_node_name.c"
)
set_source_files_properties(${rmw_sources} PROPERTIES LANGUAGE "C")
add_library(${PROJECT_NAME} ${rmw_sources})
ament_target_dependencies(${PROJECT_NAME}
  "rcutils"
)
configure_rmw_library(${PROJECT_NAME} LANGUAGE "C")

ament_export_dependencies(
  rcutils
  rosidl_generator_c
)
ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})

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

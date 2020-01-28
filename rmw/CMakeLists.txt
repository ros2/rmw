cmake_minimum_required(VERSION 3.5)

project(rmw)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake_ros REQUIRED)

find_package(rcutils REQUIRED)

include(cmake/configure_rmw_library.cmake)

include_directories(include)

set(rmw_sources
  "src/allocators.c"
  "src/convert_rcutils_ret_to_rmw_ret.c"
  "src/event.c"
  "src/init.c"
  "src/init_options.c"
  "src/loaned_message_sequence.c"
  "src/names_and_types.c"
  "src/security_options.c"
  "src/publisher_options.c"
  "src/sanity_checks.c"
  "src/security_options.c"
  "src/subscription_options.c"
  "src/topic_endpoint_info_array.c"
  "src/topic_endpoint_info.c"
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

ament_export_development_version_if_higher_than_manifest("0.8.2-dev")
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

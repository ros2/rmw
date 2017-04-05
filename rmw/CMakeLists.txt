cmake_minimum_required(VERSION 3.5)

project(rmw C)

find_package(ament_cmake REQUIRED)

include(cmake/configure_rmw_library.cmake)

include_directories(include)
add_library(${PROJECT_NAME} SHARED
  "src/allocators.c"
  "src/error_handling.c"
  "src/validate_node_name.c"
  "src/validate_topic_name.c"
  "src/sanity_checks.c")
configure_rmw_library(${PROJECT_NAME})

ament_export_dependencies(rosidl_generator_c)
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

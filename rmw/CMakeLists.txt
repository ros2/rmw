cmake_minimum_required(VERSION 2.8.3)

project(rmw C)

find_package(ament_cmake REQUIRED)

include(cmake/configure_rmw_library.cmake)

include_directories(include)
add_library(${PROJECT_NAME} SHARED
  "src/allocators.c"
  "src/error_handling.c")
configure_rmw_library(${PROJECT_NAME})

ament_export_dependencies(rosidl_generator_c)
ament_export_include_directories(include)
ament_export_libraries(${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
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

cmake_minimum_required(VERSION 2.8.3)

project(rmw NONE)

find_package(ament_cmake REQUIRED)

ament_export_dependencies(rosidl_generator_c)
ament_export_include_directories(include)

if(AMENT_ENABLE_TESTING)
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

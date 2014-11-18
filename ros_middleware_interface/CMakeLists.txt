cmake_minimum_required(VERSION 2.8.3)

project(ros_middleware_interface NONE)

find_package(ament_cmake REQUIRED)

ament_export_dependencies(rosidl_generator_c)
ament_export_include_directories(include)

ament_package(
  CONFIG_EXTRAS "ros_middleware_interface-extras.cmake"
)

install(
  DIRECTORY cmake
  DESTINATION share/${PROJECT_NAME}
)
install(
  DIRECTORY include/
  DESTINATION include
)

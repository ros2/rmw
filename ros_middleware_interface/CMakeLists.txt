cmake_minimum_required(VERSION 2.8.3)

project(ros_middleware_interface)

find_package(ament_cmake REQUIRED)

ament_export_dependencies(rosidl_generator_cpp)
ament_export_include_directories(include)

ament_package()

install(
  DIRECTORY include/
  DESTINATION include
)

cmake_minimum_required(VERSION 3.14)

project(rmw_security_common)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()


# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rcutils REQUIRED)
find_package(rmw REQUIRED)

ament_add_default_options()
ament_export_dependencies(rcutils)
ament_export_dependencies(rmw)

add_library(${PROJECT_NAME}_library
  src/security.cpp)

set_target_properties(${PROJECT_NAME}_library
  PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}_library PUBLIC
  rcutils::rcutils
  rmw::rmw)
target_include_directories(${PROJECT_NAME}_library
  PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>")

# Causes the visibility macros to use dllexport rather than dllimport,
# which is appropriate when building the dll but not consuming it.
target_compile_definitions(${PROJECT_NAME}_library
  PRIVATE "RMW_SECURITY_COMMON_BUILDING_LIBRARY")

install(
  TARGETS ${PROJECT_NAME}_library EXPORT ${PROJECT_NAME}_library
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# Export old-style CMake variables
ament_export_include_directories("include/${PROJECT_NAME}")
ament_export_libraries(${PROJECT_NAME}_library)

# Export modern CMake targets
ament_export_targets(${PROJECT_NAME}_library)

install(
  DIRECTORY include/
  DESTINATION include/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  find_package(ament_cmake_gmock REQUIRED)
  ament_lint_auto_find_test_dependencies()

  ament_add_gmock(test_security test/test_security.cpp)
  if(TARGET test_security)
    target_link_libraries(test_security
      ${PROJECT_NAME}_library
      rcutils::rcutils)
  endif()
endif()

ament_package()

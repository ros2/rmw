find_package(ament_cmake_gmock REQUIRED)

ament_add_gmock(test_error_handling test_error_handling.cpp)
target_link_libraries(test_error_handling ${PROJECT_NAME})

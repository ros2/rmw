enable_language(CXX)

find_package(ament_cmake_gmock REQUIRED)

ament_add_gmock(test_error_handling
  test_error_handling.cpp
  # Append the directory of librmw so it is found at test time.
  APPEND_LIBRARY_DIRS "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
if(TARGET test_error_handling)
  target_link_libraries(test_error_handling ${PROJECT_NAME})
  if(UNIX AND NOT APPLE)
    target_link_libraries(test_error_handling pthread)
  endif()
  set_target_properties(test_error_handling PROPERTIES COMPILE_FLAGS "-std=c++11")
endif()

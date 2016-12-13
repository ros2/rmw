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
  if(NOT WIN32)
    set_target_properties(test_error_handling PROPERTIES COMPILE_FLAGS "-std=c++14")
  endif()
endif()

ament_add_gmock(test_getenv impl/test_getenv.cpp
  ENV
    ${extra_test_env}
    EMPTY_TEST=
    NORMAL_TEST=foo
  APPEND_LIBRARY_DIRS "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
if(TARGET test_getenv)
  target_link_libraries(test_getenv ${PROJECT_NAME})
  if(UNIX AND NOT APPLE)
    target_link_libraries(test_error_handling pthread)
  endif()
  if(NOT WIN32)
    set_target_properties(test_error_handling PROPERTIES COMPILE_FLAGS "-std=c++11")
  endif()
endif()

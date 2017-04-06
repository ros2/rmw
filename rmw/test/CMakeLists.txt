find_package(ament_cmake_gmock REQUIRED)

ament_add_gmock(test_error_handling
  test_error_handling.cpp
  # Append the directory of librmw so it is found at test time.
  APPEND_LIBRARY_DIRS "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
if(TARGET test_error_handling)
  target_link_libraries(test_error_handling ${PROJECT_NAME})
endif()

ament_add_gmock(test_isalnum_no_locale
  test_isalnum_no_locale.cpp
  # Append the directory of librmw so it is found at test time.
  APPEND_LIBRARY_DIRS "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
if(TARGET test_isalnum_no_locale)
  target_link_libraries(test_isalnum_no_locale ${PROJECT_NAME})
endif()

ament_add_gmock(test_validate_topic_name
  test_validate_topic_name.cpp
  # Append the directory of librmw so it is found at test time.
  APPEND_LIBRARY_DIRS "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
if(TARGET test_validate_topic_name)
  target_link_libraries(test_validate_topic_name ${PROJECT_NAME})
endif()

ament_add_gmock(test_validate_node_name
  test_validate_node_name.cpp
  # Append the directory of librmw so it is found at test time.
  APPEND_LIBRARY_DIRS "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
if(TARGET test_validate_node_name)
  target_link_libraries(test_validate_node_name ${PROJECT_NAME})
endif()

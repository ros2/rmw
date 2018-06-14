find_package(ament_cmake_gmock REQUIRED)

ament_add_gmock(test_serialized_message
  test_serialized_message.cpp
  # Append the directory of librmw so it is found at test time.
  APPEND_LIBRARY_DIRS "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
if(TARGET test_serialized_message)
  target_link_libraries(test_serialized_message ${PROJECT_NAME})
endif()

ament_add_gmock(test_validate_full_topic_name
  test_validate_full_topic_name.cpp
  # Append the directory of librmw so it is found at test time.
  APPEND_LIBRARY_DIRS "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
if(TARGET test_validate_full_topic_name)
  target_link_libraries(test_validate_full_topic_name ${PROJECT_NAME})
endif()

ament_add_gmock(test_validate_node_name
  test_validate_node_name.cpp
  # Append the directory of librmw so it is found at test time.
  APPEND_LIBRARY_DIRS "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
if(TARGET test_validate_node_name)
  target_link_libraries(test_validate_node_name ${PROJECT_NAME})
endif()

ament_add_gmock(test_validate_namespace
  test_validate_namespace.cpp
  # Append the directory of librmw so it is found at test time.
  APPEND_LIBRARY_DIRS "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
if(TARGET test_validate_namespace)
  target_link_libraries(test_validate_namespace ${PROJECT_NAME})
  if(UNIX AND NOT APPLE)
    target_link_libraries(test_validate_namespace pthread)
  endif()
  if(NOT WIN32)
    set_target_properties(test_validate_namespace PROPERTIES COMPILE_FLAGS "-std=c++14")
  endif()
endif()

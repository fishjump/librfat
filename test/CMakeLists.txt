cmake_minimum_required(VERSION 3.22)

# Enable this definition to cleanup test files after the test.
add_compile_definitions(${PROJECT_TEST_NAME} PRIVATE TEST_CLEANUP)

add_executable(${PROJECT_TEST_NAME})

target_include_directories(${PROJECT_TEST_NAME}
    PUBLIC
        ${GTEST_INCLUDE_DIRS}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${PROJECT_TEST_NAME}
    PRIVATE
        rfat_fs_test.cpp
        type_size_test.cpp
)

target_link_libraries(${PROJECT_TEST_NAME}
    PUBLIC
        ${GTEST_BOTH_LIBRARIES}
        ${CMAKE_PROJECT_NAME}
)

list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")

add_test(
  NAME ${PROJECT_TEST_NAME}
  COMMAND ${PROJECT_TEST_NAME}
)

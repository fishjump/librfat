cmake_minimum_required(VERSION 3.22)

# Enable this definition to cleanup test files after the test.
# add_compile_definitions(${PROJECT_TEST_NAME} PRIVATE TEST_CLEANUP)

add_executable(${PROJECT_TEST_NAME})

find_package(Threads REQUIRED)

target_include_directories(${PROJECT_TEST_NAME}
    PUBLIC
        ${GTEST_INCLUDE_DIRS}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${PROJECT_TEST_NAME}
    PRIVATE
        rfat_fs_test.cpp
        rfat_file_test.cpp
)

target_link_libraries(${PROJECT_TEST_NAME}
    PUBLIC
        ${GTEST_BOTH_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${CMAKE_PROJECT_NAME}
)

add_test(
  NAME ${PROJECT_TEST_NAME}
  COMMAND ${PROJECT_TEST_NAME}
)

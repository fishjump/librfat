cmake_minimum_required(VERSION 3.22)

project(rfat)

# Generate the compile_commands.json for ide autocompletion.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

add_library(rfat STATIC)

# +--------------------+
# | Choose One Backend |
# +--------------------+
# | POSIX              |
# +--------------------+
# | Zephyr             |
# +--------------------+

add_compile_definitions(rfat PRIVATE POSIX_BACKEND)
# add_compile_definitions(rfat PRIVATE ZEPHYR_BACKEND)

target_include_directories(rfat
    PUBLIC
        modules/zephyr/include
        include
    PRIVATE
        src
)

target_sources(rfat
    PRIVATE
        src/rfat_fs_close.c
        src/rfat_fs_init.c
        src/rfat_fs_open.c
        src/rfat_fs_validate.c
        src/rfat_close.c
        src/rfat_create.c
        src/rfat_open.c
        src/rfat_read.c
        src/rfat_write.c
        src/backend/zephyr.c
        src/backend/posix.c
)

find_package(GTest)
if(NOT GTEST_FOUND)
  message(WARNING "GTest not found")
  return()
endif()

list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")

enable_testing()

add_executable(rfat_tests)
# add_compile_definitions(rfat_tests PRIVATE TEST_CLEANUP)

target_include_directories(rfat_tests
  PUBLIC
      ${GTEST_INCLUDE_DIRS}
  PRIVATE
      test
)

target_sources(rfat_tests
  PRIVATE
      test/rfat_fs_test.cpp
      test/type_size_test.cpp
)

target_link_libraries(rfat_tests
  PUBLIC
      rfat
      ${GTEST_BOTH_LIBRARIES}
)

add_test(
  NAME rfat_tests
  COMMAND rfat_tests
)
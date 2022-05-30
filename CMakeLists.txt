cmake_minimum_required(VERSION 3.22)

project(minifs)

# Generate the compile_commands.json for ide autocompletion.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(minifs STATIC)

# +--------------------+
# | Choose One Backend |
# +--------------------+
# | POSIX              |
# +--------------------+
# | Zephyr             |
# +--------------------+

add_compile_definitions(minifs PRIVATE POSIX_BACKEND)
# add_compile_definitions(minifs PRIVATE ZEPHYR_BACKEND)

target_include_directories(minifs
    PUBLIC
        modules/zephyr/include
        include
    PRIVATE
        src
)

target_sources(minifs
    PRIVATE
        src/minifs.c
        src/backend/zephyr.c
        src/backend/posix.c
)

find_package(GTest)
if(GTEST_FOUND)
  enable_testing()

  add_executable(minifs_tests)

  target_include_directories(minifs_tests
    PUBLIC
        ${GTEST_INCLUDE_DIRS}
    PRIVATE
        test
  )

  target_sources(minifs_tests
    PRIVATE
        test/minifs_tests.cpp
  )

  target_link_libraries(minifs_tests
    PUBLIC
        minifs
        ${GTEST_BOTH_LIBRARIES}
  )

  add_test(
    NAME minifs_tests
    COMMAND minifs_tests
  )
endif()
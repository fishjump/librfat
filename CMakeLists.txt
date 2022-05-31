cmake_minimum_required(VERSION 3.22)

project(rfat)

# Generate the compile_commands.json for ide autocompletion.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
        src/rfat.c
        src/backend/zephyr.c
        src/backend/posix.c
)

find_package(GTest)
if(GTEST_FOUND)
  list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
  
  enable_testing()

  add_executable(rfat_tests)

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
endif()
cmake_minimum_required(VERSION 3.22)

project(minifs)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(minifs STATIC)

# +--------------------+
# | Choose One Backend |
# +--------------------+
# | POSIX              |
# +--------------------+
# | Zephyr             |
# +--------------------+

target_compile_definitions(minifs PRIVATE POSIX_BACKEND)
# target_compile_definitions(minifs PRIVATE ZEPHYR_BACKEND)

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

cmake_minimum_required(VERSION 3.22)

add_library(${CMAKE_PROJECT_NAME} STATIC)

# +--------------------+
# | Choose One Backend |
# +--------------------+
# | POSIX              |
# +--------------------+
# | Zephyr             |
# +--------------------+

add_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE POSIX_BACKEND)
# add_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE ZEPHYR_BACKEND)

target_include_directories(${CMAKE_PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/modules/zephyr/include
        ${PROJECT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
        rfat_close.c
        rfat_create.c
        rfat_fs_close.c
        rfat_fs_init.c
        rfat_fs_open.c
        rfat_fs_validate.c
        rfat_fs_zip.c
        rfat_open.c
        rfat_read.c
        rfat_write.c
        backend/zephyr.c
        backend/posix.c
)

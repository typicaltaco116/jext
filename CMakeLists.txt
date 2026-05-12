cmake_minimum_required(VERSION 3.10)

project(jext LANGUAGES C)

set(SOURCES
    src/main.c
    src/terminal.c
    src/drawing.c
    src/input.c
    src/data_structures.c
    src/file_handler.c
    src/cursor.c
    src/toolbar.c
)

add_executable(jext
    ${SOURCES}
)

set(LINKING_FLAGS
    -lncurses -ltinfo
)

set(C_FLAGS
    -Wall -std=c99
)

target_link_options(jext PRIVATE ${LINKING_FLAGS})
target_compile_options(jext PRIVATE ${C_FLAGS})

set(CMAKE_C_COMPILER gcc)


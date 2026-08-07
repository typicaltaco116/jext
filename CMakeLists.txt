cmake_minimum_required(VERSION 3.10)

project(jext LANGUAGES C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_VERBOSE_MAKEFILE ON)

set(JEXT_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)

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

find_package(Curses REQUIRED)

add_executable(jext
    ${SOURCES}
)
target_include_directories(jext PRIVATE ${CURSES_INCLUDE_DIRS})
target_include_directories(jext PRIVATE ${JEXT_INCLUDE_PATH})
target_link_libraries(jext ${CURSES_LIBRARIES})

# Enable all warnings
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    # GCC Compiler
    set(CMAKE_C_COMPILER "gcc")
    target_compile_options(jext PRIVATE -Wall)
elseif (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    # Microsoft Visual Studio Compiler
    target_compile_options(jext PRIVATE /W4)
endif()

# DOS Version of NCurses does not support runtime dynamic linking.
# Linking errors will if NCURSES_STATIC is not defined.
if (WIN32)
	add_definitions(-DNCURSES_STATIC)
endif()

# Jext
A simple text editor written in C.

## Dependencies
- CMake v3.10
- Ncurses v4.2

## Build Instructions
Jext uses CMake to abstract platform specific build workflows.
First, employ CMake to generate build directory.
```
cmake -B build
```
Have CMake call platform specific build command.
```
cmake --build build
```
The jext binary will be compiled into a bin directory in the root of
the repository.

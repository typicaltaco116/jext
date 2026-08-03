# Jext
A simple text editor written in C.

## Dependencies
- CMake v3.10
- Ncurses v4.2

#### Linux Dependency installation
For Debian based distros (apt package manager)
```
sudo apt update
sudo apt install cmake libncurses-dev
```
For RHEL/Fedora (RPM package manager)
```
sudo dnf install cmake ncurses-devel
```

### Windows Dependency installation
Using MSYS2 package manager
```
pacman -Syu
pacman -S mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-ncurses
```

## Multiplatform Build Instructions
Jext uses CMake to abstract platform specific build workflows.
First, employ CMake to generate build directory.
```
cmake -B build
```
Have CMake handle platform specific build command.
```
cmake --build build
```

# CMakeList.txt : CMake project for gui, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (sqlite "shell.c" "sqlite3.c" "sqlite3.h" "sqlite3ext.h")

# TODO: Add tests and install targets if needed.

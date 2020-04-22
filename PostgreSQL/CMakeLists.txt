# CMakeList.txt : CMake project for gui, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_library (PostgreSQL "Column.cpp" "Column.h" "DataType.cpp" "DataType.h" "Table.cpp" "Table.h")

# TODO: Add tests and install targets if needed.

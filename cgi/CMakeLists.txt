# CMakeList.txt : CMake project for gui, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_library (cgi "CGI.cpp" "CGI.h" "Dispatcher.cpp" "Dispatcher.h" "Session.cpp" "Session.h")

# TODO: Add tests and install targets if needed.

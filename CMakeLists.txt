# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("RestData")

include_directories(folly)

#target_compile_features(RestData PUBLIC cxx_std_17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a") #  /std:c++17

# Include sub-projects.
add_subdirectory ("cgi")
add_subdirectory ("compress")
add_subdirectory ("Concurrent")
add_subdirectory ("CSV")
add_subdirectory ("data")
add_subdirectory ("FS")
add_subdirectory ("log")
add_subdirectory ("media")
add_subdirectory ("PostgreSQL")
add_subdirectory ("sqlite")
add_subdirectory ("text")
add_subdirectory ("yajl")

add_executable (RestData "main.cpp")

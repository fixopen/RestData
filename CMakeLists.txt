cmake_minimum_required(VERSION 3.1)
project(RestData)

#include_directories(~/app/fmt)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(SOURCE_FILES
        sqlite/sqlite3.h
        sqlite/sqlite3ext.h
        sqlite/sqlite3.c
        yajl/yajl_version.c
        yajl/yajl_alloc.h
        yajl/yajl_alloc.c
        yajl/yajl_buf.h
        yajl/yajl_buf.c
        yajl/yajl_bytestack.h
        yajl/yajl_encode.h
        yajl/yajl_encode.c
        yajl/yajl_gen.c
        yajl/yajl_lex.h
        yajl/yajl_lex.c
        yajl/yajl_parser.h
        yajl/yajl_parser.c
        yajl/yajl_tree.c
        yajl/yajl.c
        CSV/CSV.h
        CSV/CSV.cpp
        log/Log.h
        log/Log.cpp
        text/StringOp.h
        text/StringOp.cpp
        Concurrent/Process.h
        Concurrent/Process.cpp
        cgi/CGI.h
        cgi/CGI.cpp
        cgi/Dispatcher.h
        cgi/Dispatcher.cpp
        cgi/Session.cpp
        cgi/Session.h
        compress/Compress.h
        compress/Compress.cpp
        data/Model.h
        data/Model.cpp
        data/users.h
        data/users.cpp
        data/TypeList.cpp
        data/TypeList.h
        PostgreSQL/DataType.cpp
        PostgreSQL/DataType.h
        PostgreSQL/Column.cpp
        PostgreSQL/Column.h
        PostgreSQL/Table.cpp
        PostgreSQL/Table.h
        FS/FileSystem.cpp
        FS/FileSystem.h
        media/Meta.cpp
        media/Meta.h
        main.cpp)
add_executable(RestData ${SOURCE_FILES})

#include_directories(${CMAKE_SOURCE_DIR}/inc)
#link_directories(${CMAKE_SOURCE_DIR}/lib)

#add_executable(foo ${FOO_SRCS})
#target_link_libraries(foo bar) # libbar.so is found in ${CMAKE_SOURCE_DIR}/lib

#add_library(bar SHARED IMPORTED) # or STATIC instead of SHARED
#set_target_properties(bar PROPERTIES
#        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/libbar.so"
#        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/libbar"
#        )

#set(FOO_SRCS "foo.cpp")
#add_executable(foo ${FOO_SRCS})
#target_link_libraries(foo bar) # also adds the required include path


project(main)
cmake_minimum_required(VERSION 2.6)
set(CMAKE_SOURCE_DIR .)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_CXX_COMPILER "g++")
aux_source_directory(. DIR_SRCS)
add_executable(main ${DIR_SRCS})

find_package(libprotobuf REQUIRED)

mark_as_advanced(
    LIBPROTOBUF_INCLUDE_DIR
    LIBPROTOBUF_LIBRARIES
    )

if (LIBPROTOBUF_INCLUDE_DIR AND LIBPROTOBUF_LIBRARIES)
    message(STATUS "Found libprotobuf libraries")
    include_directories(${LIBPROTOBUF_INCLUDE_DIR})
    message(${LIBPROTOBUF_LIBRARIES})
    target_link_libraries(main ${LIBPROTOBUF_LIBRARIES})
endif (LIBDB_CXX_INCLUDE_DIR AND LIBDB_CXX_LIBRARIES)

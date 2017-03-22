
# step 2

project(main)
cmake_minimum_required(VERSION 2.6)
add_subdirectory(src)
aux_source_directory(./ DIR_SRCS)
add_executable(main ${DIR_SRCS})
target_link_libraries(main Test)


#t4 CMakeLists.txt

PROJECT(NEWHELLO)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

ADD_SUBDIRECTORY(src)

MESSAGE(STATUS "CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})
MESSAGE(STATUS "PROJECT_BINARY_DIR: " ${PROJECT_BINARY_DIR})
MESSAGE(STATUS "NEWHELLO_BINARY_DIR: " ${NEWHELLO_BINARY_DIR})

MESSAGE(STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})
MESSAGE(STATUS "PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "NEWHELLO_SOURCE_DIR: " ${NEWHELLO_SOURCE_DIR})

MESSAGE(STATUS "EXECUTABLE_OUTPUT_PATH: " ${EXECUTABLE_OUTPUT_PATH})
MESSAGE(STATUS "LIBRARY_OUTPUT_PATH: " ${LIBRARY_OUTPUT_PATH})
MESSAGE(STATUS "PROJECT_NAME: " ${PROJECT_NAME})

MESSAGE(STATUS "HOME dir: $ENV{HOME}")

MESSAGE(STATUS "CMAKE_MAJOR_VERSION: " ${CMAKE_MAJOR_VERSION})
MESSAGE(STATUS "CMAKE_MINOR_VERSION: " ${CMAKE_MINOR_VERSION})
MESSAGE(STATUS "CMAKE_PATCH_VERSION: " ${CMAKE_PATCH_VERSION})
MESSAGE(STATUS "CMAKE_SYSTEM: " ${CMAKE_SYSTEM})
MESSAGE(STATUS "CMAKE_SYSTEM_NAME: " ${CMAKE_SYSTEM_NAME})
MESSAGE(STATUS "CMAKE_SYSTEM_VERSION: " ${CMAKE_SYSTEM_VERSION})
MESSAGE(STATUS "CMAKE_SYSTEM_PROCESSOR: " ${CMAKE_SYSTEM_PROCESSOR})
MESSAGE(STATUS "UNIX: " ${UNIX})
MESSAGE(STATUS "WIN32: " ${WIN32})

ADD_TEST(mytest /home/aut/study/tools/dev_tools/cmake/backup4/cmake/t4/build/src/main)
ADD_TEST(lstest /usr/bin/ls)
ENABLE_TESTING()

FIND_LIBRARY(libx boost_date_time /usr/lib/boost/)
IF(libx)
    MESSAGE(STATUS "boost_date_time path: " ${libx})
ENDIF(libx)

SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)
IF(WIN32)
    MESSAGE(STATUS "WIN32")
ELSEIF(UNIX)
    MESSAGE(STATUS "UNIX")
ELSEIF(APPLE)
    MESSAGE(STATUS "APPLE")
ENDIF(WIN32)

FOREACH(VAR RANGE 10)
    MESSAGE(${VAR})
ENDFOREACH(VAR)

FOREACH(A RANGE 5 15 3)
    MESSAGE(${A})
ENDFOREACH(A)

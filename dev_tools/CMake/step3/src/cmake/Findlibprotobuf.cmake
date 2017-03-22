
message(STATUS "Using bundled Findlibprotobuf.cmake...")

find_path (
    LIBPROTOBUF_INCLUDE_DIR
    message.h
    /usr/local/include/google/ /usr/include/
)

find_path (
    LIBPROTOBUF_LIBRARIES NAMES protobuf
    PATHS /usr/lib/ /usr/local/lib/
)

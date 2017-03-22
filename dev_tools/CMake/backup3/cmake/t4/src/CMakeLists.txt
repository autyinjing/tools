
# t4-src CmakeLists.txt

ADD_EXECUTABLE(main main.c)

LINK_DIRECTORIES(/tmp/t2/usr/lib/)
INCLUDE_DIRECTORIES(/tmp/t2/usr/include/hello)
#TARGET_LINK_LIBRARIES(main /tmp/t2/usr/lib/libhello.so)
#TARGET_LINK_LIBRARIES(main /tmp/t2/usr/lib/libhello.a)
TARGET_LINK_LIBRARIES(main libhello.a)

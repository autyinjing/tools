
# Find glib library

FIND_PATH(GLIB_INCLUDE_DIR 
    glib.h /usr/include/glib-2.0/
    DOC "glib include dir")

FIND_LIBRARY(GLIB_LIBRARY_DIR 
    NAMES glib
    PATHS /usr/lib/
    DOC "glib library dir")

IF (GLIB_INCLUDE_DIR)
    SET(GLIB_FOUND 1)
ELSE (GLIB_INCLUDE_DIR)
    SET(GLIB_FOUND 0)
ENDIF(GLIB_INCLUDE_DIR)

IF (GLIB_LIBRARY_DIR)
    MESSAGE(STATUS "Found the glib library !")
ELSE (GLIB_LIBRARY_DIR)
    MESSAGE(FATAL_ERROR "Not found glib library ! Please install glib before cmake !")
ENDIF(GLIB_LIBRARY_DIR)

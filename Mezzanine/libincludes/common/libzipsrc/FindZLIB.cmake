# We're building our own, so set the variables explicitly.
# Variables to set are:
#
#   ZLIB_INCLUDE_DIRS   - where to find zlib.h, etc.
#   ZLIB_LIBRARIES      - List of libraries when using zlib.
#   ZLIB_FOUND          - True if zlib found.
# 
#   ZLIB_VERSION_STRING - The version of zlib found (x.y.z)
#   ZLIB_VERSION_MAJOR  - The major version of zlib
#   ZLIB_VERSION_MINOR  - The minor version of zlib
#   ZLIB_VERSION_PATCH  - The patch version of zlib
#   ZLIB_VERSION_TWEAK  - The tweak version of zlib
# 
#   ZLIB_MAJOR_VERSION  - The major version of zlib
#   ZLIB_MINOR_VERSION  - The minor version of zlib
#   ZLIB_PATCH_VERSION  - The patch version of zlib
#
# These are the same vaiables set by the default "FindZLIB.cmake" implementation.
# Below are the variables we also set because libzip explicitly expects them.
#
#   ZLIB_LIBRARY        - Like ZLIB_LIBRARIES but just one.

set(ZLIB_FOUND TRUE)
set(ZLIB_INCLUDE_DIR "${Mezz_DependencyLibDir}/zlibsrc/zlib/")
set(ZLIB_INCLUDE_DIRS "${ZLIB_INCLUDE_DIR}")
get_target_property(ZLIB_OUTPUT_NAME zlib LIBRARY_OUTPUT_NAME)
get_target_property(ZLIBSTATIC_OUTPUT_NAME zlibstatic LIBRARY_OUTPUT_NAME)
set(ZLIB_LIBRARY "${LIBRARY_OUTPUT_PATH}${ZLIBSTATIC_OUTPUT_NAME}")
set(ZLIB_LIBRARIES 
    "${LIBRARY_OUTPUT_PATH}${ZLIB_OUTPUT_NAME}" 
    "${ZLIB_LIBRARY}"
)

# This if block is based on one that appears in the official FindZLIB.cmake from kitware.
# That file is distributed under the OSI-approved BSD 3-Clause License.  The website
# https://cmake.org/licensing has more details.
if(ZLIB_INCLUDE_DIR AND EXISTS "${ZLIB_INCLUDE_DIR}/zlib.h")
    file(STRINGS "${ZLIB_INCLUDE_DIR}/zlib.h" ZLIB_H REGEX "^#define ZLIB_VERSION \"[^\"]*\"$")

    string(REGEX REPLACE "^.*ZLIB_VERSION \"([0-9]+).*$" "\\1" ZLIB_VERSION_MAJOR "${ZLIB_H}")
    string(REGEX REPLACE "^.*ZLIB_VERSION \"[0-9]+\\.([0-9]+).*$" "\\1" ZLIB_VERSION_MINOR  "${ZLIB_H}")
    string(REGEX REPLACE "^.*ZLIB_VERSION \"[0-9]+\\.[0-9]+\\.([0-9]+).*$" "\\1" ZLIB_VERSION_PATCH "${ZLIB_H}")
    set(ZLIB_VERSION_STRING "${ZLIB_VERSION_MAJOR}.${ZLIB_VERSION_MINOR}.${ZLIB_VERSION_PATCH}")

    # only append a TWEAK version if it exists:
    set(ZLIB_VERSION_TWEAK "")
    if( "${ZLIB_H}" MATCHES "ZLIB_VERSION \"[0-9]+\\.[0-9]+\\.[0-9]+\\.([0-9]+)")
        set(ZLIB_VERSION_TWEAK "${CMAKE_MATCH_1}")
        string(APPEND ZLIB_VERSION_STRING ".${ZLIB_VERSION_TWEAK}")
    endif()

    set(ZLIB_MAJOR_VERSION "${ZLIB_VERSION_MAJOR}")
    set(ZLIB_MINOR_VERSION "${ZLIB_VERSION_MINOR}")
    set(ZLIB_PATCH_VERSION "${ZLIB_VERSION_PATCH}")
endif()

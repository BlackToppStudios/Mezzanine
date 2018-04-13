# Edited Files:
# CMakeLists.txt
#
# CMake Variable renames:
# ASM686 -> ZIP_ASM686
# AMD64 -> ZIP_AMD64
message ( "ZLib - Begin" )

# Add cache forcing here

add_subdirectory(zlib)

# Cache forcing for the sake of other projects
set(ZLIB_FOUND TRUE CACHE BOOL "" FORCE)
set(ZLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/zlib/" CACHE PATH "" FORCE)
set(ZLIB_LIBRARY "zlibstatic" CACHE STRING "" FORCE)
set(ZLIB_LIBRARIES "zlibstatic" "zlib" CACHE LIST "" FORCE)
set(ZLIB_VERSION_STRING "1.2.11" CACHE STRING "" FORCE)

# Mark advanced here
mark_as_advanced(
    ZLIB_ASM686
    ZLIB_AMD64
    
    INSTALL_BIN_DIR
    INSTALL_LIB_DIR
    INSTALL_INC_DIR
    INSTALL_MAN_DIR
    INSTALL_PKGCONFIG_DIR
    
    ZLIB_FOUND
    ZLIB_INCLUDE_DIR
    ZLIB_LIBRARY
    ZLIB_LIBRARIES
    ZLIB_VERSION_STRING
)

message ( "ZLib - End" )

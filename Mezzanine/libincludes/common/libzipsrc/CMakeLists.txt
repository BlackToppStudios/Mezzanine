# Edited Files:
# appveyor.yml
# CMakeLists.txt
# INSTALL.md
# lib/CMakeLists.txt
#
# CMake Variable renames:
# ENABLE_GNUTLS -> LIBZIP_ENABLE_GNUTLS
# ENABLE_OPENSSL -> LIBZIP_ENABLE_OPENSSL
# ENABLE_COMMONCRYPTO -> LIBZIP_ENABLE_COMMONCRYPTO
# BUILD_SHARED_LIBS -> LIBZIP_BUILD_SHARED_LIBS
message ( "LibZip - Begin" )

# Add cache forcing here

set(LIBZIP_ENABLE_GNUTLS OFF CACHE BOOL "Enable use of GnuTLS" FORCE)
set(LIBZIP_ENABLE_OPENSSL OFF CACHE BOOL "Enable use of OpenSSL" FORCE)
set(LIBZIP_ENABLE_COMMONCRYPTO OFF CACHE BOOL "Enable use of CommonCrypto" FORCE)
set(LIBZIP_BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

add_subdirectory(libzip)

add_dependencies(zip ${ZLIB_LIBRARY})

# Mark advanced here
mark_as_advanced(
    LIBZIP_ENABLE_COMMONCRYPTO
    LIBZIP_ENABLE_GNUTLS
    LIBZIP_ENABLE_OPENSSL
    LIBZIP_BUILD_SHARED_LIBS
    
    NETTLE_INCLUDE_DIR
    NETTLE_LIBRARY
)

message ( "LibZip - End" )

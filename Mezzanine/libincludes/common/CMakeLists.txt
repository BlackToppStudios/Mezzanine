# Convenience file for setting the common directory for dependency libraries.
set(Mezz_DependencyLibDir ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "")

add_subdirectory (zlibsrc)
add_subdirectory (libzipsrc)
add_subdirectory (oggvorbis)
add_subdirectory (sdlsrc)
add_subdirectory (openalsoftsrc)
add_subdirectory (bulletsrc)
add_subdirectory (ogresrc)

mark_as_advanced( FORCE Mezz_DependencyLibDir )

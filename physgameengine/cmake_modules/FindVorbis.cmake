# Finds Vorbis library
#
#  Vorbis_INCLUDE_DIR - where to find vorbisenc.h, etc.
#  Vorbis_LIBRARIES   - List of libraries when using Vorbis.
#  Vorbis_FOUND       - True if Vorbis found.
#
# $VORBISDIR is an environment variable used
# for finding Vorbis.
#

if (Vorbis_INCLUDE_DIR)
  # Already in cache, be silent
  set(Vorbis_FIND_QUIETLY TRUE)
endif (Vorbis_INCLUDE_DIR)

find_path(Vorbis_INCLUDE_DIR vorbis/vorbisenc.h
  /opt/local/include
  /usr/local/include
  /usr/include
  $ENV{VORBISDIR}
  $ENV{VORBISDIR}/include
)

set(Vorbis_NAMES vorbis)
find_library(Vorbis_LIBRARY
  NAMES ${Vorbis_NAMES}
  PATHS /usr/lib /usr/local/lib /opt/local/lib $ENV{VORBISDIR} $ENV{VORBISDIR}/lib
)

if (Vorbis_INCLUDE_DIR AND Vorbis_LIBRARY)
   set(Vorbis_FOUND TRUE)
   set( Vorbis_LIBRARIES ${Vorbis_LIBRARY} )
else (Vorbis_INCLUDE_DIR AND Vorbis_LIBRARY)
   set(Vorbis_FOUND FALSE)
   set(Vorbis_LIBRARIES)
endif (Vorbis_INCLUDE_DIR AND Vorbis_LIBRARY)

if (Vorbis_FOUND)
   if (NOT Vorbis_FIND_QUIETLY)
      message(STATUS "Found Vorbis: ${Vorbis_LIBRARY}")
   endif (NOT Vorbis_FIND_QUIETLY)
else (Vorbis_FOUND)
   if (Vorbis_FIND_REQUIRED)
      message(STATUS "Looked for Vorbis libraries named ${Vorbis_NAMES}.")
      message(STATUS "Include file detected: [${Vorbis_INCLUDE_DIR}].")
      message(STATUS "Lib file detected: [${Vorbis_LIBRARY}].")
      message(FATAL_ERROR "=========> Could NOT find Vorbis library")
   endif (Vorbis_FIND_REQUIRED)
endif (Vorbis_FOUND)

mark_as_advanced(
  Vorbis_LIBRARY
  Vorbis_INCLUDE_DIR
  )

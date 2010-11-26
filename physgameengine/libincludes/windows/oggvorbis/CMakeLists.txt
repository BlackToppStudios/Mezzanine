cmake_minimum_required (VERSION 2.8)
Project( oggvorbis )
message ("${PROJECT_NAME} - Begin")

#Add some source files

set(${PROJECT_NAME}_sources
	"${${PROJECT_NAME}_SOURCE_DIR}/libogg-1.2.1/src/bitwise.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libogg-1.2.1/src/framing.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libogg-1.2.1/include/ogg/ogg.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libogg-1.2.1/include/ogg/os_types.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/analysis.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/bitrate.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/block.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/codebook.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/envelope.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/floor0.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/floor1.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/info.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/lookup.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/lpc.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/lsp.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/mapping0.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/mdct.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/psy.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/registry.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/res0.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/sharedbook.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/smallft.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/synthesis.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/vorbisenc.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/window.c"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/backends.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/bitrate.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/codebook.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/include/vorbis/codec.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/codec_internal.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/envelope.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/modes/floor_all.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/books/floor/floor_books.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/highlevel.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/lookup.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/lookup_data.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/lpc.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/lsp.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/masking.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/mdct.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/misc.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/os.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/psy.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/modes/psych_11.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/modes/psych_16.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/modes/psych_44.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/modes/psych_8.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/registry.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/books/coupled/res_books_stereo.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/books/uncoupled/res_books_uncoupled.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/modes/residue_16.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/modes/residue_44.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/modes/residue_44u.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/modes/residue_8.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/scales.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/modes/setup_11.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/modes/setup_16.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/modes/setup_22.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/modes/setup_32.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/modes/setup_44.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/modes/setup_44u.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/modes/setup_8.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/modes/setup_X.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/smallft.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/include/vorbis/vorbisenc.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/include/vorbis/vorbisfile.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/window.h"
)

include_directories(
	"${${PROJECT_NAME}_SOURCE_DIR}/libogg-1.2.1/include"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/lib/"
	"${${PROJECT_NAME}_SOURCE_DIR}/libvorbis-1.3.2/include"
)

#add_definitions( ${CommonBuildFlags} -fPIC )

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_sources})

message ( "${PROJECT_NAME} - End" )

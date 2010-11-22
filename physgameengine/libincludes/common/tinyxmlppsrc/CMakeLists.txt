Project( ticpp )
message ("${PROJECT_NAME} - Begin")

#Add some source files
set(${PROJECT_NAME}_sources
	"${${PROJECT_NAME}_SOURCE_DIR}/tinyxmlpp/ticpp.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/tinyxmlpp/ticpp.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/tinyxmlpp/ticpprc.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/tinyxmlpp/tinystr.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/tinyxmlpp/tinystr.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/tinyxmlpp/tinyxml.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/tinyxmlpp/tinyxmlerror.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/tinyxmlpp/tinyxml.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/tinyxmlpp/tinyxmlparser.cpp"
)

#Choose the destination directory
set (${PROJECT_NAME}_BINARY_DIR "${physgame_ROOT_DIR}data/${BuildPlatform}/tinyxmlpp/")
set (PROJECT_BINARY_DIR "${physgame_ROOT_DIR}data/${BuildPlatform}/tinyxmlpp/")
set (LIBRARY_OUTPUT_PATH "${physgame_ROOT_DIR}data/${BuildPlatform}/tinyxmlpp/")
message ("Putting ticpp binaries in: ${${PROJECT_NAME}_BINARY_DIR}")

add_definitions( ${CommonBuildFlags} -DTIXML_USE_TICPP -DWIN32 -D_WINDOWS -DNDEBUG)
add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_sources})

message ("${PROJECT_NAME} - End")

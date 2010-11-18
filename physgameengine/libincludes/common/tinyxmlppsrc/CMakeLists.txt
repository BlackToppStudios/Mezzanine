Project( ticpp )
message ("${PROJECT_NAME} - Begin")

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

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_sources} )

message ("${PROJECT_NAME} - End")

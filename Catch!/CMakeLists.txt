cmake_minimum_required (VERSION 2.8)
project (Catch)

message ("${PROJECT_NAME} - Begin")

##############################################################################
#EngineDemo initializations

add_definitions( ${CommonBuildFlags} )

set (${PROJECT_NAME}_DATA_DIR "${${PROJECT_NAME}_SOURCE_DIR}/data/")
message ("Base Directory for data input: ${${PROJECT_NAME}_DATA_DIR}")

set (${PROJECT_NAME}_SOURCE_DIR "${${PROJECT_NAME}_SOURCE_DIR}/src/")
message ("Looking for source code in: ${${PROJECT_NAME}_SOURCE_DIR}")

set (${PROJECT_NAME}_PHYSGAME_DIR "${ProjectRootDir}/physgameengine/")
message ("Base for physgame: ${${PROJECT_NAME}_PHYSGAME_DIR}")

#this next line should work, but I guess CMake applies somekind of scope to variables
#set (${PROJECT_NAME}_PHYSGAME_BIN_DIR "${physgame_BINARY_DIR}")
set (${PROJECT_NAME}_PHYSGAME_BIN_DIR "${physgame_BINARY_DIR}/bin/")
message ("Physgame Binaries: ${${PROJECT_NAME}_PHYSGAME_BIN_DIR}")

set (${PROJECT_NAME}_PHYSGAME_BIN_SRC "${${PROJECT_NAME}_PHYSGAME_DIR}src/")
message ("Physgame source and headers: ${${PROJECT_NAME}_PHYSGAME_BIN_SRC}")

set (${PROJECT_NAME}_BINARY_DIR "${${PROJECT_NAME}_BINARY_DIR}/bin/")
set (EXECUTABLE_OUTPUT_PATH "${${PROJECT_NAME}_BINARY_DIR}")
message ("Base Directory for Executable binaries and data output: ${${PROJECT_NAME}_BINARY_DIR}")

##############################################################################
#A complete list of our source files and headers
set(${PROJECT_NAME}_sources
	"${${PROJECT_NAME}_SOURCE_DIR}gamebase.h"
	"${${PROJECT_NAME}_SOURCE_DIR}gamebase.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}areaofplay.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}areaofplay.h"
	"${${PROJECT_NAME}_SOURCE_DIR}callbacks.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}callbacks.h"
	"${${PROJECT_NAME}_SOURCE_DIR}catchapp.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}catchapp.h"
	"${${PROJECT_NAME}_SOURCE_DIR}gamebase.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}gamebase.h"
	"${${PROJECT_NAME}_SOURCE_DIR}scorearea.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}scorearea.h"
	"${${PROJECT_NAME}_SOURCE_DIR}startingarea.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}startingarea.h"
)

##############################################################################
# Functions to use to copy files at build time

#Description: This function copies items from the Data/lib directory into it's expected location in the output directory During build time. It does not need leading slashes
#PreCondition: This function can only be called after "add_library(${PROJECT_NAME} ...)" is called, otherwise it will fail silently
function(CopyDemoFilesDuringBuild SourceFileInDataTree TargetFileInBinaryTree)
	add_custom_command(
		TARGET ${PROJECT_NAME}
		PRE_LINK
		COMMAND ${CMAKE_COMMAND} -E echo "Preparing File: ${${PROJECT_NAME}_DATA_DIR}${TargetFileInBinaryTree}"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "${${PROJECT_NAME}_DATA_DIR}${SourceFileInDataTree}" "${${PROJECT_NAME}_BINARY_DIR}${TargetFileInBinaryTree}"
		VERBATIM
	)
endfunction(CopyDemoFilesDuringBuild)

function(CopyFilesFromPhygameDuringBuild FileName)
	add_custom_command(
		TARGET ${PROJECT_NAME}
		PRE_LINK
		COMMAND ${CMAKE_COMMAND} -E echo "Preparing File: ${${PROJECT_NAME}_PHYSGAME_BIN_DIR}${FileName}"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "${${PROJECT_NAME}_PHYSGAME_BIN_DIR}${FileName}" "${${PROJECT_NAME}_BINARY_DIR}${FileName}"
		VERBATIM
	)
endfunction(CopyFilesFromPhygameDuringBuild)

##############################################################################
#define the executable and say it requires physgame
include_directories( ${${PROJECT_NAME}_SOURCE_DIR} ${${PROJECT_NAME}_PHYSGAME_BIN_SRC} )
link_directories( ${${PROJECT_NAME}_PHYSGAME_BIN_DIR} )

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_sources} )
add_dependencies( ${PROJECT_NAME} physgame )
target_link_libraries( ${PROJECT_NAME} physgame )

##############################################################################
# File copies during all builds
message ("${PROJECT_NAME} - Begin File Copy Process")

CopyDemoFilesDuringBuild( common/Catch_1.png data/common/Catch_1.png )
CopyDemoFilesDuringBuild( common/Catch!.gorilla data/common/Catch!.gorilla )
CopyDemoFilesDuringBuild( common/sounds/welcomefun-1.ogg data/ )
CopyDemoFilesDuringBuild( common/ubuntu-font-family/ubuntu-font-licence-1.0.txt data/common/ubuntu-font-family/ubuntu-font-licence-1.0.txt )
CopyDemoFilesDuringBuild( common/ubuntu-font-family/Ubuntu-B.ttf data/common/ubuntu-font-family/Ubuntu-B.ttf )
CopyDemoFilesDuringBuild( common/ubuntu-font-family/Ubuntu-BI.ttf data/common/ubuntu-font-family/Ubuntu-BI.ttf )
CopyDemoFilesDuringBuild( common/ubuntu-font-family/Ubuntu-I.ttf data/common/ubuntu-font-family/Ubuntu-I.ttf )
CopyDemoFilesDuringBuild( common/ubuntu-font-family/Ubuntu-R.ttf data/common/ubuntu-font-family/Ubuntu-R.ttf )
CopyDemoFilesDuringBuild( common/music/cAudioTheme1.ogg data/common/music/cAudioTheme1.ogg )
CopyDemoFilesDuringBuild( common/music/cAudioTheme2.ogg data/common/music/cAudioTheme2.ogg )

CopyFilesFromPhygameDuringBuild( data/common/Ogre-lgpl-3.0.txt )
CopyFilesFromPhygameDuringBuild( data/common/PugiXML-MITLicense.txt )

##############################################################################
#Copy System Dependant files during build time

# Windows File Copy Specific Items
if(${WIN32})
	message ("${PROJECT_NAME} - Begin Windows32/64 Specific File Copy Process")
	CopyFilesFromPhygameDuringBuild( data/windows/ogre/RenderSystem_GL.dll )
	CopyFilesFromPhygameDuringBuild( data/windows/plugins.cfg )
	CopyFilesFromPhygameDuringBuild( data/windows/settings.cfg )
	CopyFilesFromPhygameDuringBuild( libphysgame.dll )
	CopyFilesFromPhygameDuringBuild( OgreMain.dll )
	CopyFilesFromPhygameDuringBuild( libboost_thread-mgw45-mt-1_44.dll )
	CopyFilesFromPhygameDuringBuild( OpenAL32.dll )
	CopyFilesFromPhygameDuringBuild( SDL.dll )
	CopyFilesFromPhygameDuringBuild( wrap_oal.dll )
	message ("${PROJECT_NAME} - End Windows32/64 Specific File Copy Process")
endif(${WIN32})

# Linux File Copy Specific Items

if(${UNIX})
	if(${APPLE})
	else(${APPLE})

	message ("${PROJECT_NAME} - Begin Unix/Linux Specific File Copy Process")
	#CopyDemoFilesDuringBuild( linux/EngineDemo EngineDemo )
	#CopyFilesFromPhygameDuringBuild( libOgreMain.so )
	CopyFilesFromPhygameDuringBuild( data/linux/settings.cfg )
	CopyFilesFromPhygameDuringBuild( data/linux/ogre/RenderSystem_GL.so )
	CopyFilesFromPhygameDuringBuild( data/linux/ogre/Plugin_ParticleFX.so )
	CopyFilesFromPhygameDuringBuild( data/linux/plugins.cfg )
	CopyFilesFromPhygameDuringBuild( libphysgame.so )
	CopyFilesFromPhygameDuringBuild( libOgreMain.so.1.7.2 )
	message ("${PROJECT_NAME} - End Unix/Linux Specific File Copy Process")

	endif(${APPLE})
endif(${UNIX})


# Mac File Copy pecific Items
if(${APPLE})
	message ("${PROJECT_NAME} - Begin Apple Specific File Copy Process")
	CopyFilesFromPhygameDuringBuild( data/common/Ogre-lgpl-3.0.txt )
	CopyFilesFromPhygameDuringBuild( data/macosx/settings.cfg )
	CopyFilesFromPhygameDuringBuild( data/macosx/ogre/RenderSystem_GL.dylib )
	CopyFilesFromPhygameDuringBuild( data/macosx/plugins.cfg )
	CopyFilesFromPhygameDuringBuild( libphysgame.dylib )
#	CopyFilesFromPhygameDuringBuild( data/macosx/ogre/Ogre.framework)


	message ("${PROJECT_NAME} - End Apple Specific File Copy Process")
endif(${APPLE})

message ("${PROJECT_NAME} - End Copy Process")

message ("${PROJECT_NAME} - End")

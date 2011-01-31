cmake_minimum_required (VERSION 2.8)
project (EngineDemo)

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
CopyDemoFilesDuringBuild( common/r2skin.jpg data/common/r2skin.jpg )
CopyDemoFilesDuringBuild( common/wood-texture.jpg data/common/wood-texture.jpg )
CopyDemoFilesDuringBuild( common/Sphere_Metal.psd data/common/Sphere_Metal.psd )
CopyDemoFilesDuringBuild( common/dejavu.png data/common/dejavu.png )
CopyDemoFilesDuringBuild( common/Plane_UVW_Text.tga data/common/Plane_UVW_Text.tga )
CopyDemoFilesDuringBuild( common/Plane.mesh data/common/Plane.mesh )
CopyDemoFilesDuringBuild( common/robot.mesh data/common/robot.mesh )
CopyDemoFilesDuringBuild( common/Sphere_Metal.mesh data/common/Sphere_Metal.mesh )
CopyDemoFilesDuringBuild( common/Sphere_Wood.mesh data/common/Sphere_Wood.mesh )
CopyDemoFilesDuringBuild( common/dejavu.gorilla data/common/dejavu.gorilla )
CopyDemoFilesDuringBuild( common/robot.skeleton data/common/robot.skeleton )
CopyDemoFilesDuringBuild( common/Sphere_Wood.material data/common/Sphere_Wood.material )
CopyDemoFilesDuringBuild( common/Examples.material data/common/Examples.material )
CopyDemoFilesDuringBuild( common/Plane.material data/common/Plane.material )
CopyDemoFilesDuringBuild( common/Sphere_Metal.material data/common/Sphere_Metal.material )
CopyDemoFilesDuringBuild( common/RobotDecomp3.bullet data/common/RobotDecomp3.bullet )
CopyDemoFilesDuringBuild( common/test.xml data/common/test.xml )
CopyDemoFilesDuringBuild( common/test.zip data/common/test.zip )
CopyDemoFilesDuringBuild( common/music/cAudioTheme1.ogg data/common/music/cAudioTheme1.ogg )
CopyDemoFilesDuringBuild( common/music/cAudioTheme2.ogg data/common/music/cAudioTheme2.ogg )
CopyDemoFilesDuringBuild( common/sounds/welcomefun-1.ogg data/common/sounds/welcomefun-1.ogg )

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
	message ("${PROJECT_NAME} - Begin Unix/Linux Specific File Copy Process")
	#CopyDemoFilesDuringBuild( linux/EngineDemo EngineDemo )
	#CopyFilesFromPhygameDuringBuild( libOgreMain.so )
	CopyFilesFromPhygameDuringBuild( data/common/Ogre-lgpl-3.0.txt )
	CopyFilesFromPhygameDuringBuild( data/linux/settings.cfg )
	CopyFilesFromPhygameDuringBuild( data/linux/ogre/RenderSystem_GL.so )
	CopyFilesFromPhygameDuringBuild( data/linux/plugins.cfg )
	CopyFilesFromPhygameDuringBuild( libphysgame.so )
	CopyFilesFromPhygameDuringBuild( libOgreMain.so.1.8.0 )
	message ("${PROJECT_NAME} - End Unix/Linux Specific File Copy Process")
endif(${UNIX})

# Mac File Copy pecific Items
if(${APPLE})
	message ("${PROJECT_NAME} - Begin Apple Specific File Copy Process")

	message ("${PROJECT_NAME} - End Apple Specific File Copy Process")
endif(${APPLE})

message ("${PROJECT_NAME} - End Copy Process")

message ("${PROJECT_NAME} - End")

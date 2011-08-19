cmake_minimum_required (VERSION 2.8)
project (UnitTests)
message ("${PROJECT_NAME} - Begin")

###############################################################################
#Variables
add_definitions( ${CommonBuildFlags} )

set (${PROJECT_NAME}_DATA_DIR "${${PROJECT_NAME}_SOURCE_DIR}/data/")
message ("Base Directory for data input: ${${PROJECT_NAME}_DATA_DIR}")

set (${PROJECT_NAME}_SOURCE_DIR "${${PROJECT_NAME}_SOURCE_DIR}/src/")
message ("Looking for source code in: ${${PROJECT_NAME}_SOURCE_DIR}")

set (${PROJECT_NAME}_PHYSGAME_DIR "${ProjectRootDir}/physgameengine/")
message ("Base for physgame: ${${PROJECT_NAME}_PHYSGAME_DIR}")

set (${PROJECT_NAME}_PHYSGAME_BIN_DIR "${physgame_BINARY_DIR}/bin/")
message ("Physgame Binaries: ${${PROJECT_NAME}_PHYSGAME_BIN_DIR}")

set (${PROJECT_NAME}_PHYSGAME_BIN_SRC "${${PROJECT_NAME}_PHYSGAME_DIR}src/")
message ("Physgame source and headers: ${${PROJECT_NAME}_PHYSGAME_BIN_SRC}")

set (${PROJECT_NAME}_BINARY_DIR "${${PROJECT_NAME}_BINARY_DIR}/bin/")
set (EXECUTABLE_OUTPUT_PATH "${${PROJECT_NAME}_BINARY_DIR}")
message ("Base Directory for Executable Test binaries and test data output: ${${PROJECT_NAME}_BINARY_DIR}")

set (physgame_LIBINCLUDE_DIR ${ProjectRootDir}/physgameengine/libincludes)

###############################################################################
#Sources
set(${PROJECT_NAME}_sources
	"${${PROJECT_NAME}_SOURCE_DIR}/main.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/main.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/actortests.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/compilerflagtests.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/constrainttests.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventmanagertests.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/particleeffecttests.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/scenemanagertests.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/vector2tests.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/vector3tests.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/worldnodetests.h"
)

###############################################################################
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
# Accessing correct headers

include_directories( ${${PROJECT_NAME}_SOURCE_DIR}
	"${physgame_LIBINCLUDE_DIR}/common/bulletsrc/bulletsvnsrc/src"
	"${physgame_LIBINCLUDE_DIR}/common/bulletsrc/bulletsvnsrc/src/LinearMath"
	"${physgame_LIBINCLUDE_DIR}/common/bulletsrc/bulletsvnsrc/Extras/ConvexDecomposition"
	"${physgame_LIBINCLUDE_DIR}/common/bulletsrc/bulletsvnsrc/Extras/Serialize/BulletWorldImporter"
	"${physgame_LIBINCLUDE_DIR}/common/caudiosrc/cAudio2.1.0/include"
)

if(${WIN32})
	message ("${PROJECT_NAME} - Begin Windows32/64 Specific Build Process")
	set(BuildPlatform windows)

	include_directories(
		"${physgame_LIBINCLUDE_DIR}/${BuildPlatform}/sdl"
		"${physgame_LIBINCLUDE_DIR}/${BuildPlatform}/ogre"
	)

	message ("${PROJECT_NAME} - End Windows32/64 Specific Build Process")
endif(${WIN32})

if(${UNIX})
	if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 	#if(${UNIX} AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin") #does not appear to work as expected.
	else(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		message ("${PROJECT_NAME} - Begin Unix/Linux Specific Build Process")
		set(BuildPlatform linux)

		#message("${physgame_LIBINCLUDE_DIR}/${BuildPlatform}/ogresrc/ogre/OgreMain/include/")
		include_directories(
			"${physgame_LIBINCLUDE_DIR}/${BuildPlatform}/ogresrc/ogre/OgreMain/include/"
			"${physgame_LIBINCLUDE_DIR}/${BuildPlatform}/ogresrc/ogre/include/"
			"${physgame_LIBINCLUDE_DIR}/common/sdlsrc/SDL/include/"

		)

		message ("${PROJECT_NAME} - End Unix/Linux Specific Build Process")
	endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
endif(${UNIX})

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message ("${PROJECT_NAME} - Begin Apple Specific Build Process")


	include_directories(
		${${PROJECT_NAME}_ROOT_DIR}/libincludes/common/sdlsrc/SDL/include
		${OPENAL_INCLUDE_DIR}
		"${OGRE_INCLUDE_DIR}"
		${${PROJECT_NAME}_ROOT_DIR}/libincludes/macosx/ogresrc/boost_1_42
		"${OGG_INCLUDE_DIR}"
	)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
##############################################################################
#Executable definitions
include_directories( ${${PROJECT_NAME}_SOURCE_DIR} ${${PROJECT_NAME}_PHYSGAME_BIN_SRC} )
link_directories( ${${PROJECT_NAME}_PHYSGAME_BIN_DIR} )

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_sources} )
add_dependencies( ${PROJECT_NAME} physgame )
target_link_libraries( ${PROJECT_NAME} physgame )

##############################################################################
# File copies during all builds
message ("${PROJECT_NAME} - Begin File Copy Process")


CopyDemoFilesDuringBuild( common/r2skin.jpg data/common/r2skin.jpg )
CopyDemoFilesDuringBuild( common/robot.mesh data/common/robot.mesh )
CopyDemoFilesDuringBuild( common/robot.skeleton data/common/robot.skeleton )
CopyDemoFilesDuringBuild( common/dejavu.png data/common/dejavu.png )
CopyDemoFilesDuringBuild( common/dejavu.gorilla data/common/dejavu.gorilla )
CopyDemoFilesDuringBuild( common/sounds/welcomefun-1.ogg data/ )
CopyDemoFilesDuringBuild( common/ubuntu-font-family/ubuntu-font-licence-1.0.txt data/common/ubuntu-font-family/ubuntu-font-licence-1.0.txt )
CopyDemoFilesDuringBuild( common/ubuntu-font-family/Ubuntu-B.ttf data/common/ubuntu-font-family/Ubuntu-B.ttf )
CopyDemoFilesDuringBuild( common/ubuntu-font-family/Ubuntu-BI.ttf data/common/ubuntu-font-family/Ubuntu-BI.ttf )
CopyDemoFilesDuringBuild( common/ubuntu-font-family/Ubuntu-I.ttf data/common/ubuntu-font-family/Ubuntu-I.ttf )
CopyDemoFilesDuringBuild( common/ubuntu-font-family/Ubuntu-R.ttf data/common/ubuntu-font-family/Ubuntu-R.ttf )
CopyDemoFilesDuringBuild( common/music/cAudioTheme1.ogg data/common/music/cAudioTheme1.ogg )
CopyDemoFilesDuringBuild( common/Examples.particle data/common/Examples.particle )
CopyDemoFilesDuringBuild( common/flare.png data/common/flare.png )
CopyDemoFilesDuringBuild( common/Examples.material data/common/Examples.material )
CopyDemoFilesDuringBuild( common/clouds.jpg data/common/clouds.jpg )

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
	CopyFilesFromPhygameDuringBuild( data/windows/ogre/Plugin_ParticleFX.dll )
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
	CopyFilesFromPhygameDuringBuild( data/linux/ogre/Plugin_CgProgramManager.so )
	CopyFilesFromPhygameDuringBuild( data/linux/plugins.cfg )
	CopyFilesFromPhygameDuringBuild( libphysgame.so )
	CopyFilesFromPhygameDuringBuild( libOgreMain.so.1.7.3 )
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


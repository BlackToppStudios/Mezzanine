# © Copyright 2010 - 2011 BlackTopp Studios Inc.
# This file is part of The PhysGame Engine.
#
#    The PhysGame Engine is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    The PhysGame Engine is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
#
#   The original authors have included a copy of the license specified above in the
#   'Docs' folder. See 'gpl.txt'
#
#   We welcome the use of The PhysGame anyone, including companies who wish to
#   Build professional software and charge for their product.
#
#   However there are some practical restrictions, so if your project involves
#   any of the following you should contact us and we will try to work something
#   out:
#    - DRM or Copy Protection of any kind(except Copyrights)
#    - Software Patents You Do Not Wish to Freely License
#    - Any Kind of Linking to Non-GPL licensed Works
#    - Are Currently In Violation of Another Copyright Holder's GPL License
#    - If You want to change our code and not add a few hundred MB of stuff to
#        your distribution
#
#   These and other limitations could cause serious legal problems if you ignore
#   them, so it is best to simply contact us or the Free Software Foundation, if
#   you have any questions.
#
#   Joseph Toppi - toppij@gmail.com
#   John Blackwood - makoenergy02@gmail.com


cmake_minimum_required (VERSION 2.8)
project (UnitTests)
message ("${PROJECT_NAME} - Begin")

###############################################################################
#Variables
add_definitions( ${CommonBuildFlags} )

set (${PROJECT_NAME}_SOURCE_DIR "${${PROJECT_NAME}_SOURCE_DIR}/")
set (${PROJECT_NAME}_ROOT_DIR "${${PROJECT_NAME}_SOURCE_DIR}")
message ( STATUS "Looking build resources in: ${${PROJECT_NAME}_ROOT_DIR}")

set (${PROJECT_NAME}_DATA_DIR "${${PROJECT_NAME}_SOURCE_DIR}data/")
message ( STATUS "Base Directory for data input: ${${PROJECT_NAME}_DATA_DIR}")

set (${PROJECT_NAME}_SRC_DIR "${${PROJECT_NAME}_SOURCE_DIR}src/")
message ( STATUS "Looking for source code in: ${${PROJECT_NAME}_SRC_DIR}")

set (${PROJECT_NAME}_PHYSGAME_DIR "${ProjectRootDir}/physgameengine/")
message ( STATUS "Base for physgame: ${${PROJECT_NAME}_PHYSGAME_DIR}")

set (${PROJECT_NAME}_PHYSGAME_BIN_DIR "${physgame_BINARY_DIR}/bin/")
message ( STATUS "Physgame Binaries: ${${PROJECT_NAME}_PHYSGAME_BIN_DIR}")

set (${PROJECT_NAME}_PHYSGAME_BIN_SRC "${${PROJECT_NAME}_PHYSGAME_DIR}src/")
message ( STATUS "Physgame source and headers: ${${PROJECT_NAME}_PHYSGAME_BIN_SRC}")

set (${PROJECT_NAME}_BINARY_DIR "${${PROJECT_NAME}_BINARY_DIR}/bin/")
set (EXECUTABLE_OUTPUT_PATH "${${PROJECT_NAME}_BINARY_DIR}")
message ( STATUS "Base Directory for Executable Test binaries and test data output: ${${PROJECT_NAME}_BINARY_DIR}")

set (physgame_LIBINCLUDE_DIR ${ProjectRootDir}/physgameengine/libincludes)

###############################################################################
#Sources
set(${PROJECT_NAME}_sources
	"${${PROJECT_NAME}_SRC_DIR}/main.h"
	"${${PROJECT_NAME}_SRC_DIR}/main.cpp"
	"${${PROJECT_NAME}_SRC_DIR}/actortests.h"
	"${${PROJECT_NAME}_SRC_DIR}/collisionshapetests.h"
	"${${PROJECT_NAME}_SRC_DIR}/compilerflagtests.h"
	"${${PROJECT_NAME}_SRC_DIR}/constrainttests.h"
	"${${PROJECT_NAME}_SRC_DIR}/eventmanagertests.h"
	"${${PROJECT_NAME}_SRC_DIR}/metacodetests.h"
	"${${PROJECT_NAME}_SRC_DIR}/particleeffecttests.h"
	"${${PROJECT_NAME}_SRC_DIR}/scenemanagertests.h"
	"${${PROJECT_NAME}_SRC_DIR}/transformtests.h"
	"${${PROJECT_NAME}_SRC_DIR}/vector2tests.h"
	"${${PROJECT_NAME}_SRC_DIR}/vector3tests.h"
	"${${PROJECT_NAME}_SRC_DIR}/worldnodetests.h"
)

###############################################################################
# Functions to use to copy files at build time

#Description: This function copies items from the Data/lib directory into it's expected location in the output directory During build time. It does not need leading slashes
#PreCondition: This function can only be called after "add_library(${PROJECT_NAME} ...)" is called, otherwise it will fail silently
function(CopyFilesDuringBuild SourceFileInDataTree TargetFileInBinaryTree)
	add_custom_command(
		TARGET ${PROJECT_NAME}
		PRE_LINK
		COMMAND ${CMAKE_COMMAND} -E echo "Preparing File: ${${PROJECT_NAME}_DATA_DIR}${TargetFileInBinaryTree}"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "${${PROJECT_NAME}_DATA_DIR}${SourceFileInDataTree}" "${${PROJECT_NAME}_BINARY_DIR}${TargetFileInBinaryTree}"
		VERBATIM
	)
endfunction(CopyFilesDuringBuild)

#Description: This function copies items from the Data/lib directory in physgameengine into it's expected location in the output directory  for catch during build time. It does not need leading slashes
#PreCondition: This function can only be called after "add_library(${PROJECT_NAME} ...)" is called, otherwise it will fail silently
function(CopyFilesFromPhygameDuringBuild FileName)
	add_custom_command(
		TARGET ${PROJECT_NAME}
		PRE_LINK
		COMMAND ${CMAKE_COMMAND} -E echo "Preparing File: ${${PROJECT_NAME}_PHYSGAME_BIN_DIR}${FileName}"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "${${PROJECT_NAME}_PHYSGAME_BIN_DIR}${FileName}" "${${PROJECT_NAME}_BINARY_DIR}${FileName}"
		VERBATIM
	)
endfunction(CopyFilesFromPhygameDuringBuild)

#Description: This creates a symlink in the Binary directory to another file in the binary directory. Used on systems that expect versioned shared object names when you want to link against a more base version.
#PreCondition: This function can only be called after "add_library(${PROJECT_NAME} ...)" is called and only on system that support symlinks, otherwise it will fail silently
function(CreateSymlinkDuringBuild Targeting LinkName)
	add_custom_command(
		TARGET ${PROJECT_NAME}
		PRE_LINK
		COMMAND ${CMAKE_COMMAND} -E echo "Preparing Symlink: ${${PROJECT_NAME}_BINARY_DIR}${Targeting}"
		COMMAND ${CMAKE_COMMAND} -E create_symlink "${Targeting}" "${${PROJECT_NAME}_BINARY_DIR}${LinkName}"
		VERBATIM
	)
endfunction(CreateSymlinkDuringBuild)

##############################################################################
# Accessing correct headers

include_directories( ${${PROJECT_NAME}_SRC_DIR}
	"${physgame_LIBINCLUDE_DIR}/common/bulletsrc/bulletsvnsrc/src"
	"${physgame_LIBINCLUDE_DIR}/common/bulletsrc/bulletsvnsrc/src/LinearMath"
	"${physgame_LIBINCLUDE_DIR}/common/bulletsrc/bulletsvnsrc/Extras/ConvexDecomposition"
	"${physgame_LIBINCLUDE_DIR}/common/bulletsrc/bulletsvnsrc/Extras/Serialize/BulletWorldImporter"
	"${physgame_LIBINCLUDE_DIR}/common/caudiosrc/cAudio2.1.0/include"
	"${physgame_LIBINCLUDE_DIR}/${BuildPlatform}/ogre"
)

if(${WIN32})
	message ( STATUS "${PROJECT_NAME} - Begin Windows32/64 Specific Build Process")
	set(BuildPlatform windows)

	include_directories(

	)

	message ( STATUS "${PROJECT_NAME} - End Windows32/64 Specific Build Process")
endif(${WIN32})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message ( STATUS "${PROJECT_NAME} - Begin Linux Specific Build Process")
	set(BuildPlatform linux)

	#message( STATUS "${physgame_LIBINCLUDE_DIR}/${BuildPlatform}/ogresrc/ogre/OgreMain/include/")
	include_directories(
		${Phys_OgreBuildDir}/ogre/OgreMain/include/
		${Phys_OgreBuildDir}/ogre/include/
	)

	message ( STATUS "${PROJECT_NAME} - End Linux Specific Build Process")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message ( STATUS "${PROJECT_NAME} - Begin Apple Specific Build Process")

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
include_directories( 
	${${PROJECT_NAME}_SRC_DIR}
	${${PROJECT_NAME}_PHYSGAME_BIN_SRC} 
	"${physgame_LIBINCLUDE_DIR}/${BuildPlatform}/sdl"
	"${physgame_LIBINCLUDE_DIR}/${BuildPlatform}/ogre"
)
link_directories( ${${PROJECT_NAME}_PHYSGAME_BIN_DIR} )

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_sources} )
add_dependencies( ${PROJECT_NAME} physgame OgreMain)
target_link_libraries( ${PROJECT_NAME} ${PhysgameLinkName} ${OgreLinkName})

##############################################################################
# File copies during all builds
message ( STATUS "${PROJECT_NAME} - Begin File Copy Process")

CopyFilesDuringBuild( common/r2skin.jpg data/common/r2skin.jpg )
CopyFilesDuringBuild( common/robot.mesh data/common/robot.mesh )
CopyFilesDuringBuild( common/robot.skeleton data/common/robot.skeleton )
CopyFilesDuringBuild( common/dejavu.png data/common/dejavu.png )
CopyFilesDuringBuild( common/dejavu.gorilla data/common/dejavu.gorilla )
CopyFilesDuringBuild( common/sounds/welcomefun-1.ogg data/ )

CopyFilesDuringBuild( common/ubuntu-font-family/ubuntu-font-licence-1.0.txt data/common/ubuntu-font-family/ubuntu-font-licence-1.0.txt )
CopyFilesDuringBuild( common/ubuntu-font-family/Ubuntu-B.ttf data/common/ubuntu-font-family/Ubuntu-B.ttf )
CopyFilesDuringBuild( common/ubuntu-font-family/Ubuntu-BI.ttf data/common/ubuntu-font-family/Ubuntu-BI.ttf )
CopyFilesDuringBuild( common/ubuntu-font-family/Ubuntu-I.ttf data/common/ubuntu-font-family/Ubuntu-I.ttf )
CopyFilesDuringBuild( common/ubuntu-font-family/Ubuntu-R.ttf data/common/ubuntu-font-family/Ubuntu-R.ttf )

CopyFilesDuringBuild( common/music/cAudioTheme1.ogg data/common/music/cAudioTheme1.ogg )
CopyFilesDuringBuild( common/Examples.particle data/common/Examples.particle )
CopyFilesDuringBuild( common/flare.png data/common/flare.png )
CopyFilesDuringBuild( common/Examples.material data/common/Examples.material )
CopyFilesDuringBuild( common/clouds.jpg data/common/clouds.jpg )

CopyFilesFromPhygameDuringBuild( data/common/BulletZLibLicense.txt )
CopyFilesFromPhygameDuringBuild( data/common/lgpl-3.0.txt )
CopyFilesFromPhygameDuringBuild( data/common/Ogre-MITLicense.txt )
CopyFilesFromPhygameDuringBuild( data/common/SDL-MITLicense.txt )
CopyFilesFromPhygameDuringBuild( data/common/zlib-libpngLicense.txt )
CopyFilesFromPhygameDuringBuild( data/common/gpl-3.0.txt )
CopyFilesFromPhygameDuringBuild( data/common/Licenses.txt )
CopyFilesFromPhygameDuringBuild( data/common/PugiXML-MITLicense.txt )
CopyFilesFromPhygameDuringBuild( data/common/XiphCopyrightNotice.txt )


CopyFilesFromPhygameDuringBuild( data/${BuildPlatform}/plugins.cfg )
CopyFilesFromPhygameDuringBuild( ${OgreLibFileName} )
CopyFilesFromPhygameDuringBuild( data/${BuildPlatform}/ogre/${OgrePluginCGPM} )
CopyFilesFromPhygameDuringBuild( data/${BuildPlatform}/ogre/${OgrePluginRenderGL} )
CopyFilesFromPhygameDuringBuild( data/${BuildPlatform}/ogre/${OgrePluginParticle} )

#The Engine itself
CopyFilesFromPhygameDuringBuild( ${PhysgameLibFileName} ${GameDir}${PhysgameLibFileName} )

##############################################################################
#Copy System Dependant files during build time

# Windows File Copy Specific Items
if(${WIN32})
	message ( STATUS "${PROJECT_NAME} - Begin Windows32/64 Specific File Copy Process")
	CopyFilesFromPhygameDuringBuild( data/windows/ogre/cg${OgresD}.dll )
	CopyFilesFromPhygameDuringBuild( physgame.dll )
	CopyFilesFromPhygameDuringBuild( libboost_thread-mgw45-mt-1_44.dll )
	CopyFilesFromPhygameDuringBuild( OpenAL32.dll )
	CopyFilesFromPhygameDuringBuild( wrap_oal.dll )
	message ( STATUS "${PROJECT_NAME} - End Windows32/64 Specific File Copy Process")
endif(${WIN32})

# Linux File Copy Specific Items
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message ( STATUS "${PROJECT_NAME} - Begin Linux Specific File Copy Process")
	CreateSymlinkDuringBuild( ${OgreLibFileName} ${OgreLinuxSoName} )
	CopyFilesFromPhygameDuringBuild( data/linux/GLX_icon.png )
	message ( STATUS "${PROJECT_NAME} - End Linux Specific File Copy Process")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# Mac File Copy pecific Items
if(${APPLE})
	message ( STATUS "${PROJECT_NAME} - Begin Apple Specific File Copy Process")
	CopyFilesFromPhygameDuringBuild( data/macosx/plugins.cfg )
	CopyFilesFromPhygameDuringBuild( physgame.dylib )
#	CopyFilesFromPhygameDuringBuild( data/macosx/ogre/Ogre.framework)
	message ( STATUS "${PROJECT_NAME} - End Apple Specific File Copy Process")
endif(${APPLE})

message ( STATUS "${PROJECT_NAME} - End Copy Process")

message ("${PROJECT_NAME} - End")


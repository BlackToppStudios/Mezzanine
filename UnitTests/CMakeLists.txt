# © Copyright 2010 - 2011 BlackTopp Studios Inc.
# This file is part of The Mezzanine Engine.
#
#    The Mezzanine Engine is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    The Mezzanine Engine is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
#
#   The original authors have included a copy of the license specified above in the
#   'Docs' folder. See 'gpl.txt'
#
#   We welcome the use of the Mezzanine engine to anyone, including companies who wish to
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

set (${PROJECT_NAME}_TEST_DIR "${${PROJECT_NAME}_SOURCE_DIR}tests/")
message ( STATUS "Looking for individual tests per files in: ${${PROJECT_NAME}_TEST_DIR}")

set (${PROJECT_NAME}_MEZZANINE_DIR "${ProjectRootDir}Mezzanine/")
message ( STATUS "Base for Mezzanine Unit Tests: ${${PROJECT_NAME}_MEZZANINE_DIR}")

set (${PROJECT_NAME}_MEZZANINE_BIN_DIR "${Mezzanine_BINARY_DIR}/bin/")
message ( STATUS "Mezzanine Unit Tests Binaries: ${${PROJECT_NAME}_MEZZANINE_BIN_DIR}")

set (${PROJECT_NAME}_MEZZANINE_BIN_SRC "${${PROJECT_NAME}_MEZZANINE_DIR}src/")
message ( STATUS "Mezzanine Unit Tests source and headers: ${${PROJECT_NAME}_MEZZANINE_BIN_SRC}")

set (${PROJECT_NAME}_BINARY_DIR "${${PROJECT_NAME}_BINARY_DIR}/bin/")
set (EXECUTABLE_OUTPUT_PATH "${${PROJECT_NAME}_BINARY_DIR}")
message ( STATUS "Base Directory for Executable Test binaries and test data output: ${${PROJECT_NAME}_BINARY_DIR}")

set (Mezzanine_LIBINCLUDE_DIR ${ProjectRootDir}/Mezzanine/libincludes/)

###############################################################################
# Sources
File(GLOB ${PROJECT_NAME}_test "${${PROJECT_NAME}_TEST_DIR}"  "${${PROJECT_NAME}_TEST_DIR}*.h")

message( STATUS "Looking for Tests")
foreach(CurrentFileName ${${PROJECT_NAME}_test})
    String(REGEX MATCH "/([^/]*)\\.h$" DirtyName "${CurrentFileName}")
    String(LENGTH ${DirtyName} DirtyNameLength)
    MATH(EXPR TargetLength "${DirtyNameLength} - 3")
    String(SUBSTRING "${DirtyName}" "1" "${TargetLength}" TargetName)
    #message (${TargetLength})
    #message (${DirtyName})
    #message("${TargetName}")
    list(APPEND FoundTests ${TargetName})
endforeach(CurrentFileName ${${PROJECT_NAME}_test})

message(STATUS "Found Tests: ")
foreach(CurrentTest ${FoundTests})
    message(STATUS "   ${CurrentTest}")
endforeach(CurrentTest ${FoundTests})

message(STATUS "Generating Test Instantiation and include Code: ")
foreach(CurrentTest ${FoundTests})
    set(CppInstantiations "${CppInstantiations} Instant(${CurrentTest})")
    #set(CppIncludes "${CppIncludes} Include(${CurrentTest})")
    set(CppIncludes "${CppIncludes} \\#include Q${CurrentTest} NL")
endforeach(CurrentTest ${FoundTests})

message(${D}TESTINT="${CppInstantiations}")
add_definitions(${D}TESTINT="${CppInstantiations}")
message(${D}TESTINC="${CppIncludes}")
add_definitions(${D}TESTINC="${CppIncludes}")

set(${PROJECT_NAME}_sources
    "${${PROJECT_NAME}_SRC_DIR}/autodetect.h"
    "${${PROJECT_NAME}_SRC_DIR}/main.h"
    "${${PROJECT_NAME}_SRC_DIR}/main.cpp"
    ${${PROJECT_NAME}_test}
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

#Description: This function copies items from the Data/lib directory in Mezzanine into it's expected location in the output directory for Unittests during build time. It does not need leading slashes
#PreCondition: This function can only be called after "add_library(${PROJECT_NAME} ...)" is called, otherwise it will fail silently
function(CopyFilesFromMezzanineDuringBuild FileName)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E echo "Preparing File: ${${PROJECT_NAME}_MEZZANINE_BIN_DIR}${FileName}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${${PROJECT_NAME}_MEZZANINE_BIN_DIR}${FileName}" "${${PROJECT_NAME}_BINARY_DIR}${FileName}"
        VERBATIM
    )
endfunction(CopyFilesFromMezzanineDuringBuild)

#Description: This function copies items from the Data/lib directory in Mezzanine into it's renamed location in the output directory for Unittests during build time. It does not need leading slashes
#PreCondition: This function can only be called after "add_library(${PROJECT_NAME} ...)" is called, otherwise it will fail silently
function(CopyFilesFromMezzanineDuringBuildAndRename FileName NewFileName)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E echo "Preparing File: ${${PROJECT_NAME}_MEZZANINE_BIN_DIR}${FileName}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${${PROJECT_NAME}_MEZZANINE_BIN_DIR}${FileName}" "${${PROJECT_NAME}_BINARY_DIR}${NewFileName}"
        VERBATIM
    )
endfunction(CopyFilesFromMezzanineDuringBuildAndRename)

##############################################################################
# Accessing correct headers

include_directories( ${${PROJECT_NAME}_SRC_DIR} ${${PROJECT_NAME}_TEST_DIR}
    "${Mezzanine_LIBINCLUDE_DIR}common/bulletsrc/bulletsvnsrc/src"
    "${Mezzanine_LIBINCLUDE_DIR}common/bulletsrc/bulletsvnsrc/src/LinearMath"
    "${Mezzanine_LIBINCLUDE_DIR}common/bulletsrc/bulletsvnsrc/Extras/ConvexDecomposition"
    "${Mezzanine_LIBINCLUDE_DIR}common/bulletsrc/bulletsvnsrc/Extras/Serialize/BulletWorldImporter"
    "${Mezzanine_LIBINCLUDE_DIR}common/caudiosrc/cAudio2.1.0/include"
    ${OgreConfigHeaderIncludeDir}
    "${Mezz_SDLBuildDir}/SDL/include/"
    ${OgreHeaderIncludeDir}
)

if(${WIN32})
    message ( STATUS "${PROJECT_NAME} - Begin Windows32/64 Specific Build Process")
    include_directories()
    message ( STATUS "${PROJECT_NAME} - End Windows32/64 Specific Build Process")
endif(${WIN32})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    message ( STATUS "${PROJECT_NAME} - Begin Linux Specific Build Process")
    include_directories(
        ${Mezzanine_OgreBuildDir}/ogre/OgreMain/include/
        ${Mezzanine_OgreBuildDir}/ogre/include/
    )

    message ( STATUS "${PROJECT_NAME} - End Linux Specific Build Process")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(${CMAKE_SYSTEM_NAME} MATCHES "APPLE")
    message ( STATUS "${PROJECT_NAME} - Begin Apple Specific Build Process")

    include_directories(
        ${Mezzanine_OgreBuildDir}/ogre/OgreMain/include/
        ${Mezzanine_OgreBuildDir}/ogre/include/
    )
endif(${CMAKE_SYSTEM_NAME} MATCHES "APPLE")

##############################################################################
#Executable definitions
include_directories(
    ${${PROJECT_NAME}_SRC_DIR}
    ${${PROJECT_NAME}_MEZZANINE_BIN_SRC}
    "${Mezzanine_LIBINCLUDE_DIR}/${BuildPlatform}/ogre" 			#only for windows which doesn't compile OGRE
    "${Mezz_SDLBuildDir}/SDL/include/"
)
link_directories( ${${PROJECT_NAME}_MEZZANINE_BIN_DIR} )

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_sources} )
add_dependencies( ${PROJECT_NAME} Mezzanine OgreMain)

# Link to all of the Mezzanine Dependencies
target_link_libraries( ${PROJECT_NAME}
    ${MezzanineLinkName}
    ${NetworkLibrary}
    ${OgreLibraryAllNames}
    ${LuaLinkName}
    ${Bullet_LibraryNames}
    ${SDLLibraryNames}
    ${SoundLibraryNames}
)
#target_link_libraries( ${PROJECT_NAME} ${MezzanineLinkName} ${OgreLinkName})

##############################################################################
# File copies during all builds
message ( STATUS "${PROJECT_NAME} - Begin File Copy Process")

CopyFilesDuringBuild( common/r2skin.jpg data/r2skin.jpg )
CopyFilesDuringBuild( common/robot.mesh data/robot.mesh )
CopyFilesDuringBuild( common/robot.skeleton data/robot.skeleton )
CopyFilesDuringBuild( common/dejavu.png data/dejavu.png )
CopyFilesDuringBuild( common/dejavu.mta data/dejavu.mta )
CopyFilesDuringBuild( common/sounds/welcomefun-1.ogg data/ )

CopyFilesDuringBuild( common/ubuntu-font-family/ubuntu-font-licence-1.0.txt data/common/ubuntu-font-family/ubuntu-font-licence-1.0.txt )
CopyFilesDuringBuild( common/ubuntu-font-family/Ubuntu-B.ttf data/ubuntu-font-family/Ubuntu-B.ttf )
CopyFilesDuringBuild( common/ubuntu-font-family/Ubuntu-BI.ttf data/ubuntu-font-family/Ubuntu-BI.ttf )
CopyFilesDuringBuild( common/ubuntu-font-family/Ubuntu-I.ttf data/ubuntu-font-family/Ubuntu-I.ttf )
CopyFilesDuringBuild( common/ubuntu-font-family/Ubuntu-R.ttf data/ubuntu-font-family/Ubuntu-R.ttf )

CopyFilesDuringBuild( common/music/cAudioTheme1.ogg data/music/cAudioTheme1.ogg )
CopyFilesDuringBuild( common/Examples.particle data/Examples.particle )
CopyFilesDuringBuild( common/flare.png data/flare.png )
CopyFilesDuringBuild( common/Examples.material data/Examples.material )
CopyFilesDuringBuild( common/clouds.jpg data/clouds.jpg )

CopyFilesFromMezzanineDuringBuild( data/common/BulletZLibLicense.txt )
CopyFilesFromMezzanineDuringBuild( data/common/lgpl-3.0.txt )
CopyFilesFromMezzanineDuringBuild( data/common/Ogre-MITLicense.txt )
CopyFilesFromMezzanineDuringBuild( data/common/SDL-MITLicense.txt )
CopyFilesFromMezzanineDuringBuild( data/common/zlib-libpngLicense.txt )
CopyFilesFromMezzanineDuringBuild( data/common/gpl-3.0.txt )
CopyFilesFromMezzanineDuringBuild( data/common/Licenses.txt )
CopyFilesFromMezzanineDuringBuild( data/common/PugiXML-MITLicense.txt )
CopyFilesFromMezzanineDuringBuild( data/common/XiphCopyrightNotice.txt )

#The Engine itself
CopyFilesFromMezzanineDuringBuild( ${MezzanineLibFileName} ${GameDir}${MezzanineLibFileName} )

##############################################################################
#Copy System Dependant files during build time

# Windows File Copy Specific Items
if(${WIN32})
	message ( STATUS "${PROJECT_NAME} - Begin Windows32/64 Specific File Copy Process")
	CopyFilesFromMezzanineDuringBuild( cg.dll ${GameDir}cg.dll )
	#CopyFilesFromMezzanineDuringBuild( Mezzanine.dll )
	message ( STATUS "${PROJECT_NAME} - End Windows32/64 Specific File Copy Process")
endif(${WIN32})

# Linux File Copy Specific Items
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message ( STATUS "${PROJECT_NAME} - Begin Linux Specific File Copy Process")
	include_directories(
		${Mezz_OgreBuildDir}/ogre/OgreMain/include/
		${Mezz_OgreBuildDir}/ogre/include/
	)
	CopyFilesFromMezzanineDuringBuildAndRename( data/linux/GLX_icon.png data/GLX_icon.png )
	message ( STATUS "${PROJECT_NAME} - End Linux Specific File Copy Process")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# Mac File Copy pecific Items
if(${APPLE})
	message ( STATUS "${PROJECT_NAME} - Begin Apple Specific File Copy Process")

	message ( STATUS "${PROJECT_NAME} - End Apple Specific File Copy Process")
endif(${APPLE})

message ( STATUS "${PROJECT_NAME} - End Copy Process")

message ("${PROJECT_NAME} - End")


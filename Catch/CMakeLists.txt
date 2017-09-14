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
project (Catch)

message ("${PROJECT_NAME} - Begin")

##############################################################################
#Catch! initializations

#add_definitions( ${CommonBuildFlags} )
#target_compile_options( ${PROJECT_NAME} PRIVATE ${CommonBuildFlags} )

set (${PROJECT_NAME}_ROOT_DIR "${${PROJECT_NAME}_SOURCE_DIR}/")
set (${PROJECT_NAME}_SOURCE_DIR "${${PROJECT_NAME}_SOURCE_DIR}/")
message ( STATUS "Looking build resources in: ${${PROJECT_NAME}_ROOT_DIR}")

set (${PROJECT_NAME}_DATA_DIR "${${PROJECT_NAME}_SOURCE_DIR}Data/")
message ( STATUS "Base Directory for data input: ${${PROJECT_NAME}_DATA_DIR}")

set (${PROJECT_NAME}_SRC_DIR "${${PROJECT_NAME}_SOURCE_DIR}src/")
message ( STATUS "Looking for source code in: ${${PROJECT_NAME}_SRC_DIR}")

set (${PROJECT_NAME}_MEZZANINE_DIR "${ProjectRootDir}/Mezzanine/")
message ( STATUS "Base for Mezzanine: ${${PROJECT_NAME}_MEZZANINE_DIR}")

set (${PROJECT_NAME}_MEZZANINE_BIN_DIR "${Mezzanine_BINARY_DIR}/bin/")
message ( STATUS "Mezzanine Binaries: ${${PROJECT_NAME}_MEZZANINE_BIN_DIR}")

set (${PROJECT_NAME}_MEZZANINE_BIN_SRC "${${PROJECT_NAME}_MEZZANINE_DIR}src/")
message ( STATUS "Mezzanine source and headers: ${${PROJECT_NAME}_MEZZANINE_BIN_SRC}")

set (${PROJECT_NAME}_BINARY_DIR "${${PROJECT_NAME}_BINARY_DIR}/bin/")
set (EXECUTABLE_OUTPUT_PATH "${${PROJECT_NAME}_BINARY_DIR}")
message ( STATUS "Base Directory for Executable binaries and data output: ${${PROJECT_NAME}_BINARY_DIR}")

##############################################################################
# Catch! Folder Structure

# Root
set (GameDir "${${PROJECT_NAME}_BINARY_DIR}")
message ( STATUS "Catch! Complete Output: ${GameDir}")

# Levels
set (LevelDir "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Levels/")
message ( STATUS "Catch! Levels Output: ${LevelDir}")

# Common Data
set (CommonDataDir "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Data/")
message ( STATUS "Catch! Common Data Output: ${CommonDataDir}")

# Fonts
set (FontDir "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Fonts/")
message ( STATUS "Catch! Font Output: ${FontDir}")

# Licenses
set (LicenseDir "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Licenses/")
message ( STATUS "Catch! Licenses Output: ${LicenseDir}")

# Plugins
set (PluginDir "$<TARGET_FILE_DIR:${PROJECT_NAME}>/Plugins/")
message ( STATUS "Catch! Plugins Output: ${PluginDir}")

# Content Archive Cache
set(Catch_ArchiveDir "" CACHE FILEPATH "The Directory that contains The Content Archives for Catch!")
mark_as_advanced(Catch_ArchiveDir)
if(EXISTS ${Catch_ArchiveDir})
    set(UseCatchArchives true)
    #if the user select a file in the content archive folder, pull that filename out, without using FIND, because was introduced as recently as 2.8.5
    string( LENGTH ${Catch_ArchiveDir} FilePos)
    math(EXPR FilePos "${FilePos}-1")
    string( SUBSTRING ${Catch_ArchiveDir} "${FilePos}" 1 OneChar )
    while(NOT (${OneChar} STREQUAL "/" OR ${OneChar} STREQUAL "\\") )
        string( SUBSTRING ${Catch_ArchiveDir} 0 "${FilePos}" Catch_ArchiveDir ) 	#snip one char off the end of the string
        string( LENGTH ${Catch_ArchiveDir} FilePos)				#get the info about the string again
        math(EXPR FilePos "${FilePos}-1")
        string( SUBSTRING ${Catch_ArchiveDir} "${FilePos}" 1 OneChar )
        if(${FilePos} LESS 2)							#safe exit strategy, in case gibberish is fed in
            set(OneChar "/")
            set(UseCatchArchives false)
        endif(${FilePos} LESS 2)
    endwhile(NOT (${OneChar} STREQUAL "/" OR ${OneChar} STREQUAL "\\") )
else(EXISTS ${Catch_ArchiveDir})
    set(UseCatchArchives false)
endif(EXISTS ${Catch_ArchiveDir})
if(${UseCatchArchives})									#Let user the user know wht is actually happening
    message ( STATUS "Using content archives for Catch! in folder: ${Catch_ArchiveDir}")
else(${UseCatchArchives})
    message ( STATUS "Not Using content archives for Catch!")
endif(${UseCatchArchives})

##############################################################################
#A complete list of our source files and headers
set(${PROJECT_NAME}_sources
    "${${PROJECT_NAME}_SRC_DIR}WorldEntities/levelzone.cpp"
    "${${PROJECT_NAME}_SRC_DIR}WorldEntities/levelzone.h"
    "${${PROJECT_NAME}_SRC_DIR}WorldEntities/scorearea.cpp"
    "${${PROJECT_NAME}_SRC_DIR}WorldEntities/scorearea.h"
    "${${PROJECT_NAME}_SRC_DIR}WorldEntities/startarea.cpp"
    "${${PROJECT_NAME}_SRC_DIR}WorldEntities/startarea.h"
    "${${PROJECT_NAME}_SRC_DIR}catchapp.cpp"
    "${${PROJECT_NAME}_SRC_DIR}catchapp.h"
    "${${PROJECT_NAME}_SRC_DIR}catchplayer.cpp"
    "${${PROJECT_NAME}_SRC_DIR}catchplayer.h"
    "${${PROJECT_NAME}_SRC_DIR}catchplayercontrol.cpp"
    "${${PROJECT_NAME}_SRC_DIR}catchplayercontrol.h"
    "${${PROJECT_NAME}_SRC_DIR}gamebase.cpp"
    "${${PROJECT_NAME}_SRC_DIR}gamebase.h"
    "${${PROJECT_NAME}_SRC_DIR}itemshop.cpp"
    "${${PROJECT_NAME}_SRC_DIR}itemshop.h"
    "${${PROJECT_NAME}_SRC_DIR}itemshopitems.cpp"
    "${${PROJECT_NAME}_SRC_DIR}itemshopitems.h"
    "${${PROJECT_NAME}_SRC_DIR}levelmanager.cpp"
    "${${PROJECT_NAME}_SRC_DIR}levelmanager.h"
    "${${PROJECT_NAME}_SRC_DIR}levelscorer.cpp"
    "${${PROJECT_NAME}_SRC_DIR}levelscorer.h"
    "${${PROJECT_NAME}_SRC_DIR}leveltriggers.cpp"
    "${${PROJECT_NAME}_SRC_DIR}leveltriggers.h"
    "${${PROJECT_NAME}_SRC_DIR}profilemanager.cpp"
    "${${PROJECT_NAME}_SRC_DIR}profilemanager.h"
    "${${PROJECT_NAME}_SRC_DIR}queuedsubscriber.cpp"
    "${${PROJECT_NAME}_SRC_DIR}queuedsubscriber.h"
    "${${PROJECT_NAME}_SRC_DIR}throwablegenerator.cpp"
    "${${PROJECT_NAME}_SRC_DIR}throwablegenerator.h"
    "${${PROJECT_NAME}_SRC_DIR}uicallbacks.cpp"
    "${${PROJECT_NAME}_SRC_DIR}uicallbacks.h"
    "${${PROJECT_NAME}_SRC_DIR}workunits.cpp"
    "${${PROJECT_NAME}_SRC_DIR}workunits.h"
)
message( "Catch Sources - ${${PROJECT_NAME}_sources}" )

##############################################################################
# Functions to use to copy files at build time

#Description: This function copies items from the Catch Project directory into the location specified. The output is not relative to the Binary Path, the caller must be explicit.
#PreCondition: This function can only be called after "add_library(${PROJECT_NAME} ...)" is called, otherwise it will fail silently
function(CopyFilesDuringBuild SourceFileInDataTree TargetFileInBinaryTree)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E echo "Preparing File: ${TargetFileInBinaryTree}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${${PROJECT_NAME}_SOURCE_DIR}${SourceFileInDataTree}" "${TargetFileInBinaryTree}"
        VERBATIM
    )
endfunction(CopyFilesDuringBuild)

#Description: This function copies items from the output directory in Mezzanine into the location of the Catch executable.
#PreCondition: This function can only be called after "add_library(${PROJECT_NAME} ...)" is called, otherwise it will fail silently
function(CopyLibraryFromMezzanineDuringBuild TargetName)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E echo "Preparing File: $<TARGET_FILE:${TargetName}>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${TargetName}>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        VERBATIM
    )
endfunction(CopyLibraryFromMezzanineDuringBuild)

#Description: This function copies license files in Mezzanine into the Catch License folder.
#PreCondition: This function can only be called after "add_library(${PROJECT_NAME} ...)" is called, otherwise it will fail silently
function(CopyLicenseFromMezzanineDuringBuild TargetPath DestName)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E echo "Preparing File: ${TargetPath}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${${PROJECT_NAME}_MEZZANINE_BIN_DIR}${TargetPath}" "${LicenseDir}${DestName}"
        VERBATIM
    )
endfunction(CopyLicenseFromMezzanineDuringBuild)

function(CreateDirectoryStructure)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E echo "Creating Directory Structure"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${LevelDir}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CommonDataDir}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${FontDir}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${LicenseDir}"
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PluginDir}"
        VERBATIM
    )
endfunction(CreateDirectoryStructure)

##############################################################################
# Define the executable and say it requires Mezzanine
include_directories( ${${PROJECT_NAME}_SRC_DIR} ${${PROJECT_NAME}_MEZZANINE_BIN_SRC} ${ProjectIncludeDir})
link_directories( ${${PROJECT_NAME}_MEZZANINE_BIN_DIR} )

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_sources} )
add_dependencies( ${PROJECT_NAME} Mezzanine )
target_link_libraries( ${PROJECT_NAME} ${MezzanineLinkName} ${OgreLinkName} )

target_compile_options( ${PROJECT_NAME} PRIVATE ${CommonBuildFlags} )

##############################################################################
# File copies during all builds
message ( STATUS "${PROJECT_NAME} - Begin File Copy Process")

CreateDirectoryStructure(  )

#####################################
# Fonts
CopyFilesDuringBuild( data/common/ubuntu-font-family/ubuntu-font-licence-1.0.txt ${LicenseDir}ubuntu-font-licence-1.0.txt )
CopyFilesDuringBuild( data/common/ubuntu-font-family/Ubuntu-B.ttf ${FontDir}Ubuntu-B.ttf )
CopyFilesDuringBuild( data/common/ubuntu-font-family/Ubuntu-BI.ttf ${FontDir}Ubuntu-BI.ttf )
CopyFilesDuringBuild( data/common/ubuntu-font-family/Ubuntu-I.ttf ${FontDir}Ubuntu-I.ttf )
CopyFilesDuringBuild( data/common/ubuntu-font-family/Ubuntu-R.ttf ${FontDir}Ubuntu-R.ttf )

#####################################
# Config Files
CopyFilesDuringBuild( data/common/config-files/Catch.mxi ${CommonDataDir}Catch.mxi )
CopyFilesDuringBuild( data/common/config-files/Mezzanine.mxi ${CommonDataDir}Mezzanine.mxi )
CopyFilesDuringBuild( data/common/config-files/Resources.mxi ${CommonDataDir}Resources.mxi )

#####################################
# Licences
CopyLicenseFromMezzanineDuringBuild( data/common/BulletZLibLicense.txt BulletZLibLicense.txt )
CopyLicenseFromMezzanineDuringBuild( data/common/gpl-2.0.txt gpl-2.0.txt )
CopyLicenseFromMezzanineDuringBuild( data/common/gpl-3.0.txt gpl-3.0.txt)
CopyLicenseFromMezzanineDuringBuild( data/common/lgpl-2.1.txt lgpl-2.1.tx)
CopyLicenseFromMezzanineDuringBuild( data/common/lgpl-3.0.txt lgpl-3.0.tx)
CopyLicenseFromMezzanineDuringBuild( data/common/Licenses.txt Licenses.txt)
CopyLicenseFromMezzanineDuringBuild( data/common/Lua-MIT.txt Lua-MIT.txt)
CopyLicenseFromMezzanineDuringBuild( data/common/Ogre-MITLicense.txt Ogre-MITLicense.txt )
CopyLicenseFromMezzanineDuringBuild( data/common/PugiXML-MITLicense.txt PugiXML-MITLicense.txt )
CopyLicenseFromMezzanineDuringBuild( data/common/ReneNyffenegger-MIT.txt ReneNyffenegger-MIT.txt )
CopyLicenseFromMezzanineDuringBuild( data/common/SDL-MITLicense.txt SDL-MITLicense.txt )
CopyLicenseFromMezzanineDuringBuild( data/common/XiphCopyrightNotice.txt XiphCopyrightNotice.txt )
CopyLicenseFromMezzanineDuringBuild( data/common/zlib-libpngLicense.txt zlib-libpngLicense.txt )
CopyLicenseFromMezzanineDuringBuild( data/common/zlib-zliblicense.txt zlib-zliblicense.txt )

#####################################
# Common Data
#if(${UseCatchArchives})
    #message( STATUS "Copying Common Content")
    #file(GLOB ContentDirRootContents ${Catch_ArchiveDir}/*.zip)
    #file( COPY ${ContentDirRootContents} DESTINATION ${CommonDataDir})
    #message( STATUS "Copying Level archives")
    #file(GLOB ContentDirLevelContents "${Catch_ArchiveDir}/Lvl Files/*.lvl")
    #file( COPY ${ContentDirLevelContents} DESTINATION ${LevelDir})
    #message( STATUS "Copying Preview Content")
    #file(GLOB ContentDirPreviewContents "${Catch_ArchiveDir}/Lvl Previews/*")
    #file( COPY ${ContentDirPreviewContents} DESTINATION ${PreviewDir})
#endif(${UseCatchArchives})

#Ogre stuff
#CopyFilesDuringBuild( data/${BuildPlatform}/plugins${OgresD}.cfg ${PluginDir}plugins.cfg)

#The Engine itself
CopyLibraryFromMezzanineDuringBuild( "Mezzanine" )

##############################################################################
#Copy System Dependant files during build time

# Windows File Copy Specific Items
if(${WIN32})
    message ( STATUS "${PROJECT_NAME} - Begin Windows32/64 Specific File Copy Process")
    message ( STATUS "${PROJECT_NAME} - End Windows32/64 Specific File Copy Process")
    if(CMAKE_BUILD_TYPE STREQUAL "Release" OR "MinSizeRel")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows" )
    endif(CMAKE_BUILD_TYPE STREQUAL "Release" OR "MinSizeRel")
endif(${WIN32})

# Linux File Copy Specific Items
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    message ( STATUS "${PROJECT_NAME} - Begin Linux Specific File Copy Process")
    add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E echo "Preparing File: GLX_icon.png"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${${PROJECT_NAME}_MEZZANINE_BIN_DIR}data/${BuildPlatform}/GLX_icon.png" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/${PluginDir}GLX_icon.png"
        VERBATIM
    )
    message ( STATUS "${PROJECT_NAME} - End Linux Specific File Copy Process")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# Mac File Copy pecific Items
#if(${APPLE})
    #message ( STATUS "${PROJECT_NAME} - Begin Apple Specific File Copy Process")
    #CopyFilesFromMezzanineDuringBuild( data/macosx/settings.cfg )
    #message ( STATUS "${PROJECT_NAME} - End Apple Specific File Copy Process")
#endif(${APPLE})

message ( STATUS "${PROJECT_NAME} - End Copy Process")

message ("${PROJECT_NAME} - End")

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
#	your distribution
#
#   These and other limitations could cause serious legal problems if you ignore
#   them, so it is best to simply contact us or the Free Software Foundation, if
#   you have any questions.
#
#   Joseph Toppi - toppij@gmail.com
#   John Blackwood - makoenergy02@gmail.com


###############################################################################
# The build script for the Mezzanine engine
cmake_minimum_required (VERSION 2.8)
project (Mezzanine)
message ("${PROJECT_NAME} - Begin")

###############################################################################
# Initializations

add_definitions( ${CommonBuildFlags} )
message("Compiler Flags used on all Builds: ${CommonBuildFlags}")

set (${PROJECT_NAME}_ROOT_DIR "${${PROJECT_NAME}_SOURCE_DIR}/")
set (${PROJECT_NAME}_SOURCE_DIR "${${PROJECT_NAME}_SOURCE_DIR}/")
message ( STATUS "Base Directory for ${PROJECT_NAME}: ${${PROJECT_NAME}_ROOT_DIR}")

set (${PROJECT_NAME}_LIBINCLUDE_DIR "${${PROJECT_NAME}_SOURCE_DIR}libincludes/")
message ( STATUS "Base Directory for Library headers: ${${PROJECT_NAME}_LIBINCLUDE_DIR}")

set (${PROJECT_NAME}_LIBBINARY_DIR "${${PROJECT_NAME}_SOURCE_DIR}data/")
message ( STATUS "Base Directory for Library binaries and data: ${${PROJECT_NAME}_LIBBINARY_DIR}")

set (${PROJECT_NAME}_SRC_DIR "${${PROJECT_NAME}_SOURCE_DIR}src/")
message ( STATUS "Looking for source code in: ${${PROJECT_NAME}_SRC_DIR}")

set (${PROJECT_NAME}_BINARY_DIR "${${PROJECT_NAME}_BINARY_DIR}/bin/")
set (PROJECT_BINARY_DIR "${${PROJECT_NAME}_BINARY_DIR}")
set (LIBRARY_OUTPUT_PATH "${${PROJECT_NAME}_BINARY_DIR}")
message ( STATUS "Putting complete binaries in: ${${PROJECT_NAME}_BINARY_DIR}")

#######################################
# A complete listing of our header files
set(${PROJECT_NAME}_sources
	"${${PROJECT_NAME}_SRC_DIR}actorbase.h"
	"${${PROJECT_NAME}_SRC_DIR}actorcharacter.h"
	"${${PROJECT_NAME}_SRC_DIR}actorcontainerbase.h"
	"${${PROJECT_NAME}_SRC_DIR}actorcontainervector.h"
	"${${PROJECT_NAME}_SRC_DIR}actormanager.h"
	"${${PROJECT_NAME}_SRC_DIR}actorphysicssettings.h"
	"${${PROJECT_NAME}_SRC_DIR}actorrigid.h"
	"${${PROJECT_NAME}_SRC_DIR}actorserializer.h"
	"${${PROJECT_NAME}_SRC_DIR}actorsoft.h"
	"${${PROJECT_NAME}_SRC_DIR}areaeffect.h"
	"${${PROJECT_NAME}_SRC_DIR}attachable.h"
	"${${PROJECT_NAME}_SRC_DIR}audio.h"
	"${${PROJECT_NAME}_SRC_DIR}audioenumerations.h"
	"${${PROJECT_NAME}_SRC_DIR}audiolistener.h"
	"${${PROJECT_NAME}_SRC_DIR}audiomanager.h"
	"${${PROJECT_NAME}_SRC_DIR}audiomusicplayer.h"
	"${${PROJECT_NAME}_SRC_DIR}audioplaylist.h"
	"${${PROJECT_NAME}_SRC_DIR}audiosound.h"
	"${${PROJECT_NAME}_SRC_DIR}audiosoundset.h"
	"${${PROJECT_NAME}_SRC_DIR}camera.h"
	"${${PROJECT_NAME}_SRC_DIR}cameracontroller.h"
	"${${PROJECT_NAME}_SRC_DIR}cameramanager.h"
	"${${PROJECT_NAME}_SRC_DIR}collision.h"
	"${${PROJECT_NAME}_SRC_DIR}collisionshape.h"
	"${${PROJECT_NAME}_SRC_DIR}collisionshapemanager.h"
	"${${PROJECT_NAME}_SRC_DIR}colourvalue.h"
	"${${PROJECT_NAME}_SRC_DIR}constraint.h"
	"${${PROJECT_NAME}_SRC_DIR}crossplatform.h"
	"${${PROJECT_NAME}_SRC_DIR}crossplatformexport.h"
	"${${PROJECT_NAME}_SRC_DIR}datatypes.h"
	"${${PROJECT_NAME}_SRC_DIR}entity.h"
	"${${PROJECT_NAME}_SRC_DIR}enumerations.h"
	"${${PROJECT_NAME}_SRC_DIR}eventbase.h"
	"${${PROJECT_NAME}_SRC_DIR}eventcollision.h"
	"${${PROJECT_NAME}_SRC_DIR}eventgamewindow.h"
	"${${PROJECT_NAME}_SRC_DIR}eventmanager.h"
	"${${PROJECT_NAME}_SRC_DIR}eventquit.h"
	"${${PROJECT_NAME}_SRC_DIR}eventrendertime.h"
	"${${PROJECT_NAME}_SRC_DIR}eventuserinput.h"
	"${${PROJECT_NAME}_SRC_DIR}exception.h"
	"${${PROJECT_NAME}_SRC_DIR}extendedtimer.h"
	"${${PROJECT_NAME}_SRC_DIR}gamewindow.h"
	"${${PROJECT_NAME}_SRC_DIR}graphicsmanager.h"
	"${${PROJECT_NAME}_SRC_DIR}graphicssettings.h"
	"${${PROJECT_NAME}_SRC_DIR}inputquerytool.h"
	"${${PROJECT_NAME}_SRC_DIR}internalbulletfile.h.cpp"
	"${${PROJECT_NAME}_SRC_DIR}internalbulletfilemanager.h.cpp"
	"${${PROJECT_NAME}_SRC_DIR}internaldecompinterface.h.cpp"
	"${${PROJECT_NAME}_SRC_DIR}internalmeshtools.h.cpp"
	"${${PROJECT_NAME}_SRC_DIR}internalmotionstate.h.cpp"
	"${${PROJECT_NAME}_SRC_DIR}internalogredatastreambuf.h.cpp"
	"${${PROJECT_NAME}_SRC_DIR}light.h"
	"${${PROJECT_NAME}_SRC_DIR}linegroup.h"
	"${${PROJECT_NAME}_SRC_DIR}managerbase.h"
	"${${PROJECT_NAME}_SRC_DIR}mathtool.h"
	"${${PROJECT_NAME}_SRC_DIR}matrix3x3.h"
	"${${PROJECT_NAME}_SRC_DIR}matrix4x4.h"
	"${${PROJECT_NAME}_SRC_DIR}mesh.h"
	"${${PROJECT_NAME}_SRC_DIR}meshmanager.h"
	"${${PROJECT_NAME}_SRC_DIR}meshterrain.h"
	"${${PROJECT_NAME}_SRC_DIR}metacode.h"
	"${${PROJECT_NAME}_SRC_DIR}mezzanine.h"
	"${${PROJECT_NAME}_SRC_DIR}network.h"
	"${${PROJECT_NAME}_SRC_DIR}networkipaddress.h"
	"${${PROJECT_NAME}_SRC_DIR}networkmanager.h"
	"${${PROJECT_NAME}_SRC_DIR}networkpacket.h"
	"${${PROJECT_NAME}_SRC_DIR}networksocket.h"
	"${${PROJECT_NAME}_SRC_DIR}networksystemaddress.h"
	"${${PROJECT_NAME}_SRC_DIR}objectpair.h"
	"${${PROJECT_NAME}_SRC_DIR}objectreference.h"
	"${${PROJECT_NAME}_SRC_DIR}particleaffector.h"
	"${${PROJECT_NAME}_SRC_DIR}particleeffect.h"
	"${${PROJECT_NAME}_SRC_DIR}particleemitter.h"
	"${${PROJECT_NAME}_SRC_DIR}physicsmanager.h"
	"${${PROJECT_NAME}_SRC_DIR}plane.h"
	"${${PROJECT_NAME}_SRC_DIR}quaternion.h"
	"${${PROJECT_NAME}_SRC_DIR}ray.h"
	"${${PROJECT_NAME}_SRC_DIR}rayquerytool.h"
	"${${PROJECT_NAME}_SRC_DIR}resourceinputstream.h"
	"${${PROJECT_NAME}_SRC_DIR}resourcemanager.h"
	"${${PROJECT_NAME}_SRC_DIR}resourcetextsettingsfile.h"
	"${${PROJECT_NAME}_SRC_DIR}scenemanager.h"
	#"${${PROJECT_NAME}_SRC_DIR}scriptbindinglua51.h"
	"${${PROJECT_NAME}_SRC_DIR}scriptingmanager.h"
	"${${PROJECT_NAME}_SRC_DIR}scriptinglua51.h"
	"${${PROJECT_NAME}_SRC_DIR}scripting.h"
	"${${PROJECT_NAME}_SRC_DIR}scriptargument.h"
	"${${PROJECT_NAME}_SRC_DIR}script.h"
	"${${PROJECT_NAME}_SRC_DIR}simpletimer.h"
	"${${PROJECT_NAME}_SRC_DIR}singleton.h"
	"${${PROJECT_NAME}_SRC_DIR}serialization.h"
	"${${PROJECT_NAME}_SRC_DIR}stringtool.h"
	"${${PROJECT_NAME}_SRC_DIR}terrainbase.h"
	"${${PROJECT_NAME}_SRC_DIR}terrainmanager.h"
	"${${PROJECT_NAME}_SRC_DIR}timer.h"
	"${${PROJECT_NAME}_SRC_DIR}timermanager.h"
	"${${PROJECT_NAME}_SRC_DIR}transform.h"
	"${${PROJECT_NAME}_SRC_DIR}ui.h"
	"${${PROJECT_NAME}_SRC_DIR}uibasicrenderable.h"
	"${${PROJECT_NAME}_SRC_DIR}uibutton.h"
	"${${PROJECT_NAME}_SRC_DIR}uicaption.h"
	"${${PROJECT_NAME}_SRC_DIR}uicell.h"
	"${${PROJECT_NAME}_SRC_DIR}uicellgrid.h"
	"${${PROJECT_NAME}_SRC_DIR}uicheckbox.h"
	"${${PROJECT_NAME}_SRC_DIR}uidropdownlist.h"
	"${${PROJECT_NAME}_SRC_DIR}uienumerations.h"
	"${${PROJECT_NAME}_SRC_DIR}uiglyph.h"
	"${${PROJECT_NAME}_SRC_DIR}uikerning.h"
	"${${PROJECT_NAME}_SRC_DIR}uilayer.h"
	"${${PROJECT_NAME}_SRC_DIR}uilinelist.h"
	"${${PROJECT_NAME}_SRC_DIR}uilistbox.h"
	"${${PROJECT_NAME}_SRC_DIR}uimanager.h"
	"${${PROJECT_NAME}_SRC_DIR}uimarkuptext.h"
	"${${PROJECT_NAME}_SRC_DIR}uimenu.h"
	"${${PROJECT_NAME}_SRC_DIR}uimenuwindow.h"
	"${${PROJECT_NAME}_SRC_DIR}uipagedcellgrid.h"
	"${${PROJECT_NAME}_SRC_DIR}uiradiobutton.h"
	"${${PROJECT_NAME}_SRC_DIR}uirectangle.h"
	"${${PROJECT_NAME}_SRC_DIR}uirenderablecontainerwidget.h"
	"${${PROJECT_NAME}_SRC_DIR}uirenderablerect.h"
	"${${PROJECT_NAME}_SRC_DIR}uiresizinginfo.h"
	"${${PROJECT_NAME}_SRC_DIR}uiscreen.h"
	"${${PROJECT_NAME}_SRC_DIR}uiscrollbar.h"
	"${${PROJECT_NAME}_SRC_DIR}uiscrolledcellgrid.h"
	"${${PROJECT_NAME}_SRC_DIR}uispinner.h"
	"${${PROJECT_NAME}_SRC_DIR}uisprite.h"
	"${${PROJECT_NAME}_SRC_DIR}uitabset.h"
	"${${PROJECT_NAME}_SRC_DIR}uitextbutton.h"
	"${${PROJECT_NAME}_SRC_DIR}uitextureatlas.h"
	"${${PROJECT_NAME}_SRC_DIR}uivertex.h"
	"${${PROJECT_NAME}_SRC_DIR}uiviewportupdatetool.h"
	"${${PROJECT_NAME}_SRC_DIR}uiwidget.h"
	"${${PROJECT_NAME}_SRC_DIR}uiwindow.h"
	"${${PROJECT_NAME}_SRC_DIR}vector2.h"
	"${${PROJECT_NAME}_SRC_DIR}vector3.h"
	"${${PROJECT_NAME}_SRC_DIR}vector3wactor.h"
	"${${PROJECT_NAME}_SRC_DIR}viewport.h"
	"${${PROJECT_NAME}_SRC_DIR}world.h"
	"${${PROJECT_NAME}_SRC_DIR}worldgetset.h"
	"${${PROJECT_NAME}_SRC_DIR}worldnode.h"
	"${${PROJECT_NAME}_SRC_DIR}worldobject.h"
	"${${PROJECT_NAME}_SRC_DIR}worldobjectgraphicssettings.h"
	"${${PROJECT_NAME}_SRC_DIR}worldobjectphysicssettings.h"
	"${${PROJECT_NAME}_SRC_DIR}worldtrigger.h"
	"${${PROJECT_NAME}_SRC_DIR}xml.h"
	"${${PROJECT_NAME}_SRC_DIR}xmldoc.h"

#######################################
# A complete listing of our source files

	"${${PROJECT_NAME}_SRC_DIR}actorbase.cpp"
	"${${PROJECT_NAME}_SRC_DIR}actorcharacter.cpp"
	"${${PROJECT_NAME}_SRC_DIR}actorcontainerbase.cpp"
	"${${PROJECT_NAME}_SRC_DIR}actorcontainervector.cpp"
	"${${PROJECT_NAME}_SRC_DIR}actormanager.cpp"
	"${${PROJECT_NAME}_SRC_DIR}actorphysicssettings.cpp"
	"${${PROJECT_NAME}_SRC_DIR}actorrigid.cpp"
	"${${PROJECT_NAME}_SRC_DIR}actorserializer.cpp"
	"${${PROJECT_NAME}_SRC_DIR}actorsoft.cpp"
	"${${PROJECT_NAME}_SRC_DIR}areaeffect.cpp"
	"${${PROJECT_NAME}_SRC_DIR}attachable.cpp"
	"${${PROJECT_NAME}_SRC_DIR}audiolistener.cpp"
	"${${PROJECT_NAME}_SRC_DIR}audiomanager.cpp"
	"${${PROJECT_NAME}_SRC_DIR}audiomusicplayer.cpp"
	"${${PROJECT_NAME}_SRC_DIR}audioplaylist.cpp"
	"${${PROJECT_NAME}_SRC_DIR}audiosound.cpp"
	"${${PROJECT_NAME}_SRC_DIR}audiosoundset.cpp"
	"${${PROJECT_NAME}_SRC_DIR}camera.cpp"
	"${${PROJECT_NAME}_SRC_DIR}cameracontroller.cpp"
	"${${PROJECT_NAME}_SRC_DIR}cameramanager.cpp"
	"${${PROJECT_NAME}_SRC_DIR}collision.cpp"
	"${${PROJECT_NAME}_SRC_DIR}collisionshape.cpp"
	"${${PROJECT_NAME}_SRC_DIR}collisionshapemanager.cpp"
	"${${PROJECT_NAME}_SRC_DIR}colourvalue.cpp"
	"${${PROJECT_NAME}_SRC_DIR}constraint.cpp"
	"${${PROJECT_NAME}_SRC_DIR}crossplatform.cpp"
	"${${PROJECT_NAME}_SRC_DIR}entity.cpp"
	"${${PROJECT_NAME}_SRC_DIR}eventbase.cpp"
	"${${PROJECT_NAME}_SRC_DIR}eventcollision.cpp"
	"${${PROJECT_NAME}_SRC_DIR}eventgamewindow.cpp"
	"${${PROJECT_NAME}_SRC_DIR}eventmanager.cpp"
	"${${PROJECT_NAME}_SRC_DIR}eventquit.cpp"
	"${${PROJECT_NAME}_SRC_DIR}eventrendertime.cpp"
	"${${PROJECT_NAME}_SRC_DIR}eventuserinput.cpp"
	"${${PROJECT_NAME}_SRC_DIR}exception.cpp"
	"${${PROJECT_NAME}_SRC_DIR}extendedtimer.cpp"
	"${${PROJECT_NAME}_SRC_DIR}gamewindow.cpp"
	"${${PROJECT_NAME}_SRC_DIR}graphicsmanager.cpp"
	"${${PROJECT_NAME}_SRC_DIR}inputquerytool.cpp"
	"${${PROJECT_NAME}_SRC_DIR}internalbulletfile.cpp"
	"${${PROJECT_NAME}_SRC_DIR}internalbulletfilemanager.cpp"
	"${${PROJECT_NAME}_SRC_DIR}internaldecompinterface.cpp"
	"${${PROJECT_NAME}_SRC_DIR}internalmeshtools.cpp"
	"${${PROJECT_NAME}_SRC_DIR}internalmotionstate.cpp"
	"${${PROJECT_NAME}_SRC_DIR}internalogredatastreambuf.cpp"
	"${${PROJECT_NAME}_SRC_DIR}light.cpp"
	"${${PROJECT_NAME}_SRC_DIR}linegroup.cpp"
	"${${PROJECT_NAME}_SRC_DIR}managerbase.cpp"
	"${${PROJECT_NAME}_SRC_DIR}mathtool.cpp"
	"${${PROJECT_NAME}_SRC_DIR}matrix3x3.cpp"
	"${${PROJECT_NAME}_SRC_DIR}matrix4x4.cpp"
	"${${PROJECT_NAME}_SRC_DIR}mesh.cpp"
	"${${PROJECT_NAME}_SRC_DIR}meshmanager.cpp"
	"${${PROJECT_NAME}_SRC_DIR}meshterrain.cpp"
	"${${PROJECT_NAME}_SRC_DIR}metacode.cpp"
 	"${${PROJECT_NAME}_SRC_DIR}networkipaddress.cpp"
 	"${${PROJECT_NAME}_SRC_DIR}networkmanager.cpp"
 	"${${PROJECT_NAME}_SRC_DIR}networkpacket.cpp"
 	"${${PROJECT_NAME}_SRC_DIR}networksocket.cpp"
 	"${${PROJECT_NAME}_SRC_DIR}networksystemaddress.cpp"
	"${${PROJECT_NAME}_SRC_DIR}objectpair.cpp"
	"${${PROJECT_NAME}_SRC_DIR}particleaffector.cpp"
	"${${PROJECT_NAME}_SRC_DIR}particleeffect.cpp"
	"${${PROJECT_NAME}_SRC_DIR}particleemitter.cpp"
	"${${PROJECT_NAME}_SRC_DIR}physicsmanager.cpp"
	"${${PROJECT_NAME}_SRC_DIR}plane.cpp"
	"${${PROJECT_NAME}_SRC_DIR}quaternion.cpp"
	"${${PROJECT_NAME}_SRC_DIR}ray.cpp"
	"${${PROJECT_NAME}_SRC_DIR}rayquerytool.cpp"
	"${${PROJECT_NAME}_SRC_DIR}resourceinputstream.cpp"
	"${${PROJECT_NAME}_SRC_DIR}resourcemanager.cpp"
	"${${PROJECT_NAME}_SRC_DIR}resourcetextsettingsfile.cpp"
	"${${PROJECT_NAME}_SRC_DIR}scenemanager.cpp"
	"${${PROJECT_NAME}_SRC_DIR}scriptbindinglua51.cpp"
	"${${PROJECT_NAME}_SRC_DIR}scriptinglua51.cpp"
	"${${PROJECT_NAME}_SRC_DIR}scriptingmanager.cpp"
	"${${PROJECT_NAME}_SRC_DIR}serialization.cpp"
	"${${PROJECT_NAME}_SRC_DIR}simpletimer.cpp"
	"${${PROJECT_NAME}_SRC_DIR}stringtool.cpp"
	"${${PROJECT_NAME}_SRC_DIR}terrainbase.cpp"
	"${${PROJECT_NAME}_SRC_DIR}terrainmanager.cpp"
	"${${PROJECT_NAME}_SRC_DIR}timermanager.cpp"
	"${${PROJECT_NAME}_SRC_DIR}transform.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uibasicrenderable.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uibutton.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uicaption.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uicell.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uicellgrid.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uicheckbox.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uidropdownlist.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uilayer.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uilinelist.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uilistbox.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uimanager.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uimarkuptext.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uimenu.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uimenuwindow.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uipagedcellgrid.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uiradiobutton.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uirectangle.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uirenderablecontainerwidget.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uiresizinginfo.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uiscreen.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uiscrollbar.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uiscrolledcellgrid.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uispinner.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uitabset.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uitextbutton.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uitextureatlas.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uiviewportupdatetool.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uiwidget.cpp"
	"${${PROJECT_NAME}_SRC_DIR}uiwindow.cpp"
	"${${PROJECT_NAME}_SRC_DIR}vector2.cpp"
	"${${PROJECT_NAME}_SRC_DIR}vector3.cpp"
	"${${PROJECT_NAME}_SRC_DIR}vector3wactor.cpp"
	"${${PROJECT_NAME}_SRC_DIR}viewport.cpp"
	"${${PROJECT_NAME}_SRC_DIR}world.cpp"
	"${${PROJECT_NAME}_SRC_DIR}worldgetset.cpp"
	"${${PROJECT_NAME}_SRC_DIR}worldnode.cpp"
	"${${PROJECT_NAME}_SRC_DIR}worldobject.cpp"
	"${${PROJECT_NAME}_SRC_DIR}worldobjectgraphicssettings.cpp"
	"${${PROJECT_NAME}_SRC_DIR}worldobjectphysicssettings.cpp"
	"${${PROJECT_NAME}_SRC_DIR}worldtrigger.cpp"
	"${${PROJECT_NAME}_SRC_DIR}xml.cpp"
	"${${PROJECT_NAME}_SRC_DIR}xmldoc.cpp"
)

###############################################################################
# Functions for Later use

#Description: This function copies items from the Data/lib directory into it's expected location in the output directory During build time. It does not need leading slashes
#PreCondition: This function can only be called after "add_library(${PROJECT_NAME} ...)" is called, otherwise it will fail silently
function(CopyFilesDuringBuild SourceFileInDataTree TargetFileInBinaryTree)
	add_custom_command(
		TARGET ${PROJECT_NAME}
		PRE_LINK
		COMMAND ${CMAKE_COMMAND} -E echo "Preparing File: ${${PROJECT_NAME}_BINARY_DIR}${TargetFileInBinaryTree}"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "${${PROJECT_NAME}_LIBBINARY_DIR}${SourceFileInDataTree}" "${${PROJECT_NAME}_BINARY_DIR}${TargetFileInBinaryTree}"
		VERBATIM
	)
endfunction(CopyFilesDuringBuild)

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

#Description: This function copies items from the root directory into it's expected location in the data/library directory During build time. It does not need leading slashes
#PreCondition: This function can only be called after "add_library(${PROJECT_NAME} ...)" is called, otherwise it will fail silently
function(CopyLibraryDuringBuild SourceFile TargetFileInLibraryDir)
	add_custom_command(
		TARGET ${PROJECT_NAME}
		PRE_LINK
		COMMAND ${CMAKE_COMMAND} -E echo "Preparing Library: ${${PROJECT_NAME}_LIBBINARY_DIR}${TargetFileInLibraryDir}"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "${${PROJECT_NAME}_ROOT_DIR}${SourceFile}" "${${PROJECT_NAME}_LIBBINARY_DIR}${TargetFileInLibraryDir}"
		VERBATIM
	)
endfunction(CopyLibraryDuringBuild)

###############################################################################
# Common pre-build configuration

# Auto building of SDL could go here.


###############################################################################
# System Specific Build Items
#######################################
# All Changes that could be system specific should be defined in these variables
# These will set sane defaults (Where possible), but specific platforms should
# set a value of their own.
set(SDL SDL)

set(MezzanineBuildFlags "")

set( Bullet_LibraryNames
	"ConvexDecomposition"
	"GIMPACTUtils"
	"BulletWorldImporter"
	"BulletFileLoader"
	#"BulletSoftBodySolvers_OpenCL"
	"BulletSoftBodySolvers_CPU"
	"MiniCL"
	"BulletMultiThreaded"
	"BulletSoftBody" "BulletDynamics" "BulletCollision" "LinearMath"
)

#######################################
# Windows Build Specific Items
if(${WIN32})
	message ( STATUS "${PROJECT_NAME} - Begin Windows32/64 Specific Build Process")
	add_subdirectory ("${Mezzanine_LIBINCLUDE_DIR}${BuildPlatform}/oggvorbis") #required for cAudio
	add_definitions( -DWINDOWS -DEXPORTINGMEZZANINEDLL )	
	message ( STATUS "${PROJECT_NAME} - End Windows32/64 Specific Build Process")
endif(${WIN32})

#######################################
# Linux Build Specific Items

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message ( STATUS "${PROJECT_NAME} - Begin Linux Specific Build Process")

	set(SDL "${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/sdl/libSDL.a")

	add_definitions( -DOGRE_GUI_GLX -DLINUX )
	include_directories(
		${Mezz_OgreBuildDir}/ogre/OgreMain/include/
		${Mezz_OgreBuildDir}/ogre/include/
	)
	message (${Mezz_OgreBuildDir}/ogre/OgreMain/include/)

	message ( STATUS "${PROJECT_NAME} - End Linux Specific Build Process")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

#######################################
# Mac OS X and iPhone Build pecific Items
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message ( STATUS "${PROJECT_NAME} - Begin Apple Specific Build Process")


	add_definitions( -DMACOSX)
	message ( STATUS "${PROJECT_NAME} - End Apple Specific Build Process")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

###############################################################################
#Common Library Configuration items
include_directories( ${${PROJECT_NAME}_SRC_DIR}
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/bulletsrc/bulletsvnsrc/src"
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/bulletsrc/bulletsvnsrc/src/LinearMath"
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/bulletsrc/bulletsvnsrc/Extras/ConvexDecomposition"
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/bulletsrc/bulletsvnsrc/Extras/Serialize/BulletWorldImporter"
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/caudiosrc/cAudio2.1.0/include"
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/${BuildPlatform}/ogre"			#only on windows, which doesn't build ogre
	"${Mezz_SDLBuildDir}/SDL/include/"
)
if(${Mezz_Lua51})
	include_directories( libincludes/common/lua51/lua5.1/src )
endif(${Mezz_Lua51})


link_directories(
	"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/ogre"
	"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/bullet"
	"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/caudio"
	"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/openal"
	"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/oggvorbis"
	"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/sdl"
)
if(${Mezz_Lua51})
	link_directories( ${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/sdl )
endif(${Mezz_Lua51})


###############################################################################
# Prepare the actual Build
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_sources} )

#######################################
# Prepare Common Dependencies
add_subdirectory (libincludes/common/caudiosrc)
add_subdirectory (libincludes/common/bulletsrc)
if(${Mezz_Lua51})
	add_subdirectory (libincludes/common/lua51)
endif(${Mezz_Lua51})
#add_subdirectory (libincludes/common/ogresrc) # not quite ready yet

add_dependencies( ${PROJECT_NAME} ${BuildSpecificDependencies} cAudio Bullet Ogre ${LuaProject})

#######################################
# Library Link Dependencies

add_definitions( ${MezzanineBuildFlags} )
target_link_libraries( ${PROJECT_NAME} 				# Link to all of the Mezzanine Dependencies
	${OgreLinkName}
	${LuaLinkName}
	${Bullet_LibraryNames}
	cAudio
	${SDL}
	${NetworkLibrary}
)

###############################################################################
# Build Time File copying

#######################################
# Copy Common Files at Build Time

# Licenses
CopyFilesDuringBuild( common/BulletZLibLicense.txt data/common/BulletZLibLicense.txt )
CopyFilesDuringBuild( common/lgpl-3.0.txt data/common/lgpl-3.0.txt )
CopyFilesDuringBuild( common/Ogre-MITLicense.txt data/common/Ogre-MITLicense.txt )
CopyFilesDuringBuild( common/SDL-MITLicense.txt data/common/SDL-MITLicense.txt )
CopyFilesDuringBuild( common/zlib-libpngLicense.txt data/common/zlib-libpngLicense.txt )
CopyFilesDuringBuild( common/gpl-3.0.txt data/common/gpl-3.0.txt )
CopyFilesDuringBuild( common/Licenses.txt data/common/Licenses.txt )
CopyFilesDuringBuild( common/PugiXML-MITLicense.txt data/common/PugiXML-MITLicense.txt )
CopyFilesDuringBuild( common/XiphCopyrightNotice.txt data/common/XiphCopyrightNotice.txt )

# Ogre
CopyFilesDuringBuild( ${BuildPlatform}/ogre/${OgrePluginRenderGL} data/${BuildPlatform}/ogre/${OgrePluginRenderGL} )
CopyFilesDuringBuild( ${BuildPlatform}/ogre/${OgrePluginParticle} data/${BuildPlatform}/ogre/${OgrePluginParticle} )
CopyFilesDuringBuild( ${BuildPlatform}/ogre/${OgrePluginCGPM} data/${BuildPlatform}/ogre/${OgrePluginCGPM} )
CopyFilesDuringBuild( ${BuildPlatform}/ogre/${OgreLibFileName} ${OgreLibFileName} )
CopyFilesDuringBuild( ${BuildPlatform}/plugins${OgresD}.cfg data/${BuildPlatform}/plugins.cfg )

#We should use the new variables to copy the ogre files and Mezzanine library in the common code.

#######################################
# Windows File Copy Specific Items
if(${WIN32})
	message ( STATUS "${PROJECT_NAME} - Begin Windows32/64 Specific File Copy Process")
	set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")					 # remove lib prefix from DLL outputs
	CopyFilesDuringBuild( ${BuildPlatform}/ogre/cg${OgresD}.dll data/${BuildPlatform}/ogre/cg${OgresD}.dll )
	CopyFilesDuringBuild( ${BuildPlatform}/ogre/libboost_thread-mgw45-mt-1_44.dll libboost_thread-mgw45-mt-1_44.dll ) #can this be statically linked
	CopyFilesDuringBuild( ${BuildPlatform}/openal/OpenAL32.dll OpenAL32.dll )
	#CopyFilesDuringBuild( ${BuildPlatform}/sdl/SDL.dll SDL.dll )
	#CopyFilesDuringBuild( ${BuildPlatform}/openal/OpenAL32.lib OpenAL32.lib )


	target_link_libraries( ${PROJECT_NAME} #Statically linking against SDL means that these must be resolved now, rather than when these were compiled
		SDLmain
		Winmm
		Imm32
		Version
	)

	CopyFilesDuringBuild( ${BuildPlatform}/openal/wrap_oal.dll wrap_oal.dll )
	message ( STATUS "${PROJECT_NAME} - End Windows32/64 Specific File Copy Process")
endif(${WIN32})

#######################################
# Linux File Copy Specific Items
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	message ( STATUS "${PROJECT_NAME} - Begin Linux Specific File Copy Process")
	CopyFilesDuringBuild( ${BuildPlatform}/GLX_icon.png data/${BuildPlatform}/GLX_icon.png )
	CreateSymlinkDuringBuild( ${OgreLibFileName} ${OgreLinuxSoName} )
	message ( STATUS "${PROJECT_NAME} - End Linux Specific File Copy Process")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


#######################################
# Mac File Copy Specific Items
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message ( STATUS "${PROJECT_NAME} - Begin Apple Specific File Copy Process")
	CopyFilesDuringBuild( ${BuildPlatform}/ogre/RenderSystem_GL.dylib data/${BuildPlatform}/ogre/RenderSystem_GL.dylib )
	#CopyFilesDuringBuild( ${BuildPlatform}/ogre/Ogre.framework Ogre.framework )

	message ( STATUS "${PROJECT_NAME} - End Apple Specific File Copy Process")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

###############################################################################
# The End

message ( "${PROJECT_NAME} - End" )

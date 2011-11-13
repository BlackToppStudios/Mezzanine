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

###############################################################################
# This is the root cmake file for the physgame engine. It includes the example files 
# from the engine demo, Catch! and a series of UnitTests. These may be freely copied
# and used to build your projects as long as you release any changes you make to the
# build scripts back to the public in a usable form.
#
# This file primarily gathers information from the developer about the build environment
# and desired game engine options. It makes this information available to the rest of
# the include cmake files to minimizie duplicated effort.
#
# Here is a listing of some of the values that this files sets up for consumption by other
# build scripts, that can be set as cmake options:
#	PhysDebug	 		- True if The Engine will be compile with Debug Symbols and without compiler optimization, False and it will be optimized and omit debug symbols.
#	PhysProfile			- If Set to True, then a number of engine level and compiler tools will be enabled that track performance in the engine. This should be disabled when not debugging and benchmarking.
#	PhysXML			- True if the engine will be compiled with XML support. This includes support for serialization
#
#	UseOgreDebug		- True if the developer wants to the use the ogre debug libraries, this adjusts a number of other files that child cmake scripts can use.
#	OgreVersion			- This defaults to the version of cmake that ships with the engine, but can be changed (and should be) if a different version of ogre is desired.
#
# Here are some more values that can be used when creating cmake scripts that are indirectly
# set and should not be directly passed as cmake values
#	CommonBuildFlags 		- A listing of all the currently set build flags to pass to the compiler at compile time. This is the mechanism that enforces PhysDebug's and PhysProfile's settings.
#	OgreLinkName			- This is the name of the ogre library that the compiler expects. Examples: OgreMain, OgreMain_d
#	OgresD					- This can be used to help pick/select ogre libraries that may or may not have an "_d" depending on how they are compiled. If debug libraries are in use this is set to "_d" otherwise "".
#	OgreLibFileName		- The filename of the actual Ogre library. On some platforms this file may just be a symlink. Examples: OgreMain.dll, libOgreMain.so
#	OgreLinuxSoName		- Linux's versioned library system expects specific version of so files. This is the more specific file, that must copied along with the regular so. Examples libOgreMain.so.1.7.1, libOgreMain.so.1.9.3
#
#LibPrefix
#LibSuffix
#BuildPlatform
#OgrePluginCGPM
#OgrePluginRenderGL
#OgrePluginParticle
###############################################################################
# The name of our project is "PhysgameAndCatch". 
cmake_minimum_required (VERSION 2.8)
project (PhysgameAndCatch)

###############################################################################
# Check For the PhysDebug option and set values accordingly
option(PhysDebug "Compile with no optimizations and debug symbols" OFF)
if(${PhysDebug})
	message ("${PROJECT_NAME} - Enabling PHYSDEBUG Messages")
	if(${WIN32})
		message ("${PROJECT_NAME} - Setting Windows Debug Specific Initialiations")
		set(CommonBuildFlags "-g")
	else(${WIN32})
		message ("${PROJECT_NAME} - Setting Non-Windows Debug Initialiations")
		set(CommonBuildFlags " -fPIC -g")
	endif(${WIN32})
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSDEBUG=1")
else(${PhysDebug})
	message ("${PROJECT_NAME} - Disabling PHYSDEBUG Messages")
	if(${WIN32})
		message ("${PROJECT_NAME} - Setting Windows Specific Initialiations")
		set(CommonBuildFlags " -O2 -s ")
	else(${WIN32})
		message ("${PROJECT_NAME} - Setting Non-Windows Initialiations")
		set(CommonBuildFlags " -fPIC -O2 -s")
	endif(${WIN32})
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSDEBUG=0")
endif(${PhysDebug})

###############################################################################
# Check For the PhysProfile option and set values accordingly
option(PhysProfile "Executables will gather runtime statistics." OFF)
if(${PhysProfile})
	message ("${PROJECT_NAME} - Enabling gcc profiling and PHYSPROFILE")
	set(CommonBuildFlags "${CommonBuildFlags} -pg")
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSPROFILE=1")
else(${PhysProfile})
	message ("${PROJECT_NAME} - Disabling gcc profiling and PHYSPROFILE")
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSPROFILE=0")
endif(${PhysProfile})

###############################################################################
# Check For the PhysXML option and set values accordingly
option(PhysXML "Enable XML and XML serialization supprt." ON)
if(${PhysXML})
	message ("${PROJECT_NAME} - Enabling XML subsystem, object serialization to XML, and PHYSXML")
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSXML=1")
else(${PhysXML})
	message ("${PROJECT_NAME} - Disabling XML subsystem, object serialization to XML, and PHYSXML")
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSXML=0")
endif(${PhysXML})

###############################################################################
# Set Platform Specific Variables
set(LibPrefix "")
set(LibSuffix "")

if(${WIN32})
	set(BuildPlatform windows)
	set(LibSuffix .dll)
endif(${WIN32})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(BuildPlatform linux)
	set(LibPrefix lib)
	set(LibSuffix .so)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(BuildPlatform macosx)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

#######################################
# Physgame

set(PhysgameLibFileName ${LibPrefix}physgame${LibSuffix})

#######################################
# Ogre related values
option(UseOgreDebug "Use Ogre Debug Libraries, instead of Release Libs" OFF)
set(OgreVersion 1.7.3 CACHE STRING "The version of the ogre libraries to link against")
if(${UseOgreDebug})
	message ("${PROJECT_NAME} - Will we use Ogre Debug libraries")
	set(OgreLinkName OgreMain_d)
	set(OgresD _d)
else(${UseOgreDebug})
	message ("${PROJECT_NAME} - Will we use Ogre Release libraries")
	set(OgreLinkName OgreMain)
	set(OgresD "")
endif(${UseOgreDebug})

set(OgreLibFileName ${LibPrefix}${OgreLinkName}${LibSuffix})
set(OgreLinuxSoName ${OgreLibFileName}.${OgreVersion})		#This value is gibberish on non-Linux platforms

# Ogre Plugins
set(OgrePluginCGPM Plugin_CgProgramManager${LibSuffix} )
set(OgrePluginRenderGL RenderSystem_GL${LibSuffix} )
set(OgrePluginParticle Plugin_ParticleFX${LibSuffix} )

###############################################################################
# Additional Variables
set(ProjectRootDir ${${PROJECT_NAME}_SOURCE_DIR})


#Add subprojects
add_subdirectory (physgameengine)
#add_subdirectory (EngineDemo)
add_subdirectory (Catch!) 
add_subdirectory (UnitTests) 



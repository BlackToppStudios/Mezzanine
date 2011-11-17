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
#	Phys_Debug	 		- True if The Engine will be compile with Debug Symbols and without compiler optimization, False and it will be optimized and omit debug symbols.
#	Phys_Lua			- True if lua is to be included and link against. False if lua was not selected to be compiled in.
#	Phys_Profile			- If Set to True, then a number of engine level and compiler tools will be enabled that track performance in the engine. This should be disabled when not debugging and benchmarking.
#	Phys_XML			- True if the engine will be compiled with XML support. This includes support for serialization
#
#	Phys_UseOgreDebug		- True if the developer wants to the use the ogre debug libraries, this adjusts a number of other files that child cmake scripts can use.
#	Phys_UseOgreVersion		- This defaults to the version of cmake that ships with the engine, but can be changed (and should be) if a different version of ogre is desired.
#
# Here are some more values that can be used when creating cmake scripts that are indirectly
# set and should not be directly passed as cmake values
#	BuildPlatform			- A string that Contains either "windows", "linux", or "macosx" at the time of this writing. This directly corresponds to the folders names in the physgame engine repository.
#	CommonBuildFlags 		- A listing of all the currently set build flags to pass to the compiler at compile time. This is the mechanism that enforces Phys_Debug's and Phys_Profile's settings.
#	LibPrefix			- This is a string that is required by a given platform to begin the filename of shared libraries. One windows it is "" on Linux/Unix it is "lib"
#	LibSuffix			- This is a string that is required by a given platform to end the filename of shared libraries. One windows it is ".dll" on Linux/Unix it is ".so"
#	LuaLinkName			- This is the name that of the actual compiled lua library, it will be blank if lua is not to be linked
#	LuaProject			- The Name of the lua cmake project
#	OgreLinkName			- This is the name of the ogre library that the compiler expects. Examples: OgreMain, OgreMain_d
#	OgresD				- This can be used to help pick/select ogre libraries that may or may not have an "_d" depending on how they are compiled. If debug libraries are in use this is set to "_d" otherwise "".
#	OgreLibFileName			- The filename of the actual Ogre library. On some platforms this file may just be a symlink. Examples: OgreMain.dll, libOgreMain.so
#	OgreLinuxSoName			- Linux's versioned library system expects specific version of so files. This is the more specific file, that must copied along with the regular so. Examples libOgreMain.so.1.7.1, libOgreMain.so.1.9.3
#	OgrePluginCGPM			- The File name of the Ogre CG manager plugin
#	OgrePluginRenderGL		- The File name of the Ogre OpenGL rendersystem plugin
#	OgrePluginParticle		- The File name of the Ogre ParticleFX plugin

###############################################################################
# The name of our project is "PhysgameAndCatch". 
cmake_minimum_required (VERSION 2.8)
project (PhysgameAndCatch)

###############################################################################
# Check For the Phys_Debug option and set values accordingly
option(Phys_Debug "Compile with no optimizations and debug symbols" OFF)
if(${Phys_Debug})
	message ("${PROJECT_NAME} - Enabling Phys_Debug Messages")
	if(${WIN32})
		message ("${PROJECT_NAME} - Setting Windows Debug Specific Initialiations")
		set(CommonBuildFlags "-g")
	else(${WIN32})
		message ("${PROJECT_NAME} - Setting Non-Windows Debug Initialiations")
		set(CommonBuildFlags " -fPIC -g")
	endif(${WIN32})
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSDEBUG=1")
else(${Phys_Debug})
	message ("${PROJECT_NAME} - Disabling Phys_Debug Messages")
	if(${WIN32})
		message ("${PROJECT_NAME} - Setting Windows Specific Initialiations")
		set(CommonBuildFlags " -O2 -s ")
	else(${WIN32})
		message ("${PROJECT_NAME} - Setting Non-Windows Initialiations")
		set(CommonBuildFlags " -fPIC -O2 -s")
	endif(${WIN32})
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSDEBUG=0")
endif(${Phys_Debug})
					
#consider adding options -mmmx -msse -msse2 -m3dnow
# sse2 is the newest and is supported by the pentium4 first.
# sse3 and sse4 should also be considered
# A chart of compatible CPUs should be made, to guide developers to target their code better

###############################################################################
# Check For the Phys_Profile option and set values accordingly
option(Phys_Profile "Executables will gather runtime statistics." OFF)
if(${Phys_Profile})
	message ("${PROJECT_NAME} - Enabling gcc profiling and setting PHYSPROFILE")
	set(CommonBuildFlags "${CommonBuildFlags} -pg")
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSPROFILE=1")
else(${Phys_Profile})
	message ("${PROJECT_NAME} - Disabling gcc profiling and unsetting PHYSPROFILE")
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSPROFILE=0")
endif(${Phys_Profile})

###############################################################################
# Check For the Phys_XML option and set values accordingly
option(Phys_XML "Enable XML and XML serialization supprt." ON)
if(${Phys_XML})
	message ("${PROJECT_NAME} - Enabling XML subsystem, object serialization to XML, and setting PHYSXML")
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSXML=1")
else(${Phys_XML})
	message ("${PROJECT_NAME} - Disabling XML subsystem, object serialization to XML, and unsetting PHYSXML")
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSXML=0")
endif(${Phys_XML})

###############################################################################
# Lua Library
option(Phys_Lua "Include Support for Lua Scripting Language" ON)
if(${Phys_Lua})
	set(LuaProject "Lua")
	set(LuaLinkName "lua")
	message ("${PROJECT_NAME} - Enabling Support for Lua Scripting Language, and setting PHYSLUA")
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSLUA=1")
else(${Phys_Lua})
	message ("${PROJECT_NAME} - Disabling Support for Lua Scripting Language, and unsetting PHYSLUA")
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSLUA=0")
endif(${Phys_Lua})

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
option(Phys_UseOgreDebug "Use Ogre Debug Libraries, instead of Release Libs" OFF)
set(Phys_UseOgreVersion 1.7.3 CACHE STRING "The version of the ogre libraries to link against")
mark_as_advanced(Phys_UseOgreDebug Phys_UseOgreVersion)
if(${Phys_UseOgreDebug})
	message ("${PROJECT_NAME} - Will we use Ogre Debug libraries")
	set(OgreLinkName OgreMain_d)
	set(OgresD _d)
else(${Phys_UseOgreDebug})
	message ("${PROJECT_NAME} - Will we use Ogre Release libraries")
	set(OgreLinkName OgreMain)
	set(OgresD "")
endif(${Phys_UseOgreDebug})

set(OgreLibFileName ${LibPrefix}${OgreLinkName}${LibSuffix})
set(OgreLinuxSoName ${OgreLibFileName}.${Phys_UseOgreVersion})		#This value is gibberish on non-Linux platforms

# Ogre Plugins
set(OgrePluginCGPM Plugin_CgProgramManager${LibSuffix} )
set(OgrePluginRenderGL RenderSystem_GL${LibSuffix} )
set(OgrePluginParticle Plugin_ParticleFX${LibSuffix} )


###############################################################################
# Additional Variables
set(ProjectRootDir ${${PROJECT_NAME}_SOURCE_DIR})


###############################################################################
# Add subprojects
add_subdirectory (physgameengine)
#add_subdirectory (EngineDemo)
add_subdirectory (Catch!) 
add_subdirectory (UnitTests) 

###############################################################################
# Hide Variables not directly related to the Physgame Engine
mark_as_advanced(CMAKE_BACKWARDS_COMPATIBILITY CMAKE_BUILD_TYPE CMAKE_CODEBLOCKS_EXECUTABLE CMAKE_INSTALL_PREFIX EXECUTABLE_OUTPUT_PATH LIBRARY_OUTPUT_PATH NVIDIA_OPENCL_INCLUDES NVIDIA_OPENCL_LIBRARIES OGG_LIBRARY VORBISFILE_LIBRARY VORBIS_LIBRARY)



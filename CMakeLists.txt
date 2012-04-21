# @ Copyright 2010 - 2011 BlackTopp Studios Inc.
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

###############################################################################
# This is the root cmake file for the Mezzanine engine. It includes the example files
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
#
# That Every project using the Mezzanine will need to use as an include directory in their cmake files.
#       ProjectIncludeDir               - This is the place that mezzconfig.h and maybe other important headers
#
# That affect/add/remove behavior of the engine
#	Mezz_Debug	 		- True if The Engine will be forced to compile with symbols, will include more logging messages and libraries will also include debug symbols
#	Mezz_Lua51			- True if lua is to be included and link against. False if lua was not selected to be compiled in.
#	Mezz_Profile			- If Set to True, then a number of engine level and compiler tools will be enabled that track performance in the engine. This should be disabled when not debugging and benchmarking. Not supported in visual studio currently
#	Mezz_Network			- Enable or disable networking support
#	Mezz_XML			- True if the engine will be compiled with XML support. This includes support for serialization
#	Mezz_ThrowOnSingletonFetch	- When retrieving an invalid Singleton, should an exception be thrown, defaults to false.
#
# That affect only build time values:
#	Mezz_SDLBuildDir		- The location of the SDL source code. The cmake routine attempts to find it in a few specific directory, or the developer can enter it manually
#	OgreLinkName			- This is the name of the ogre library that the compiler expects. Examples: OgreMain, OgreMain_d
#	OgresD				- This can be used to help pick/select ogre libraries that may or may not have an "_d" depending on how they are compiled. If debug libraries are in use this is set to "_d" otherwise "".
#	OgreLibFileName			- The filename of the actual Ogre library. On some platforms this file may just be a symlink. Examples: OgreMain.dll, libOgreMain.so
#	OgreLinuxSoName			- Linux's versioned library system expects specific version of so files. This is the more specific file, that must copied along with the regular so. Examples libOgreMain.so.1.7.1, libOgreMain.so.1.9.3
#	OgrePluginCGPM			- The File name of the Ogre CG manager plugin
#	OgrePluginRenderGL		- The File name of the Ogre OpenGL rendersystem plugin
#	OgrePluginParticle		- The File name of the Ogre ParticleFX plugin
#       OgreConfigHeaderIncludeDir      - The Folder where Ogre puts its configured Headers. This is only needed by subprojects if they link directly against Ogre.
#       OgreHeaderIncludeDir            - The Folder where all the non-configured headers go. This is only needed by subprojects if they link directly against Ogre.
#
# Here are some more values that can be used when creating cmake scripts that are indirectly
# set and should not be directly passed as cmake values
#	BuildPlatform			- A string that Contains either "windows", "linux", or "macosx" at the time of this writing. This directly corresponds to the folders names in the Mezzanine engine repository.
#	CommonBuildFlags 		- A listing of all the currently set build flags to pass to the compiler at compile time. This is the mechanism that enforces Mezz_Debug's and Mezz_Profile's settings.
#	LibPrefix			- This is a string that is required by a given platform to begin the filename of shared libraries. One windows it is "" on Linux/Unix it is "lib"
#	LibSuffix			- This is a string that is required by a given platform to end the filename of shared libraries. One windows it is ".dll" on Linux/Unix it is ".so"
#	LuaLinkName			- This is the name that of the actual compiled lua library, it will be blank if lua is not to be linked
#	LuaProject			- The Name of the lua cmake project
#	MezzanineLinkName		- What should be entered when somethings needs to link against Mezzanine usually "Mezzanine" sometime it specifies and import library, like
#	MezzanineLibFileName		- The actual name of the file the Mezzanine is called.

###############################################################################
# The name of our project is "MezzanineAndCatch".
cmake_minimum_required (VERSION 2.8)
project (MezzanineAndCatch)

message("${PROJECT_NAME} - Starting Configuration")

###############################################################################
# Check For the Mezz_Debug option and set values accordingly
option(Mezz_Debug "Compile with no optimizations and debug symbols" OFF)
if(${Mezz_Debug})
        message ( STATUS "${PROJECT_NAME} - Enabling Mezz_Debug Messages")
        set(Mezz_Debug_Defined "MEZZDEBUG=1")
	if(${WIN32})
		message ( STATUS "${PROJECT_NAME} - Setting Windows Debug Specific Initialiations")
		set(CommonBuildFlags ${CMAKE_CXX_FLAGS_DEBUG})
	else(${WIN32})
		message ( STATUS "${PROJECT_NAME} - Setting Non-Windows Debug Initialiations")
		set(CommonBuildFlags " -fPIC ${CMAKE_CXX_FLAGS_DEBUG}")
	endif(${WIN32})
else(${Mezz_Debug})
	message ("${PROJECT_NAME} - Disabling Mezz_Debug Messages")
        set(Mezz_Debug_Defined "MEZZDEBUG_undefined")
	if(${WIN32})
		message ( STATUS "${PROJECT_NAME} - Setting Windows Specific Initialiations")
                #set(CommonBuildFlags)
	else(${WIN32})
		message ( STATUS "${PROJECT_NAME} - Setting Non-Windows Initialiations")
                set(CommonBuildFlags " -fPIC")
	endif(${WIN32})
endif(${Mezz_Debug})

#consider adding options -mmmx -msse -msse2 -m3dnow
# sse2 is the newest and is supported by the pentium4 first.
# sse3 and sse4 should also be considered
# A chart of compatible CPUs should be made, to guide developers to target their code better

###############################################################################
# Check For the Mezz_Profile option and set values accordingly
option(Mezz_Profile "Executables will gather runtime statistics." OFF)
if(${Mezz_Profile})
        message ( STATUS "${PROJECT_NAME} - Enabling gcc profiling and setting MEZZPROFILE")
        set(Mezz_Profile_Defined "MEZZPROFILE=1")
        set(CommonBuildFlags "${CommonBuildFlags} -pg") # The pg makes this only work with gcc, clang and imitators
else(${Mezz_Profile})
        message ( STATUS "${PROJECT_NAME} - Disabling gcc profiling and unsetting MEZZPROFILE")
        set(Mezz_Profile_Defined "MEZZPROFILE_undefined")
endif(${Mezz_Profile})

###############################################################################
# Check For the Mezz_XML option and set values accordingly
option(Mezz_XML "Enable XML and XML serialization support." ON)
if(${Mezz_XML})
        message ( STATUS "${PROJECT_NAME} - Enabling XML subsystem, object serialization to XML, and setting MEZZXML")
        set(Mezz_XML_Defined "MEZZXML=1")
else(${Mezz_XML})
        message ( STATUS "${PROJECT_NAME} - Disabling XML subsystem, object serialization to XML, and unsetting MEZZXML")
        set(Mezz_XML_Defined "MEZZXML_undefined")
endif(${Mezz_XML})

###############################################################################
# Check For the Mezz_Network option and set values accordingly
option(Mezz_Network "Enable Networking support." ON)
if(${Mezz_Network})
        message ( STATUS "${PROJECT_NAME} - Enabling Network subsystem, and setting MEZZNETWORK")
        set(Mezz_Network_Defined "MEZZNETWORK=1")
else(${Mezz_Network})
        message ( STATUS "${PROJECT_NAME} - Disabling Network subsystem, and unsetting MEZZNETWORK")
        set(Mezz_Network_Defined "MEZZNETWORK_undefined")
endif(${Mezz_Network})

###############################################################################
# Lua Library
option(Mezz_Lua51 "Include Support for Lua 5.1 Scripting Language" ON)
if(${Mezz_Lua51})
	set(LuaProject "Lua")
	set(LuaLinkName "lua")
        message ( STATUS "${PROJECT_NAME} - Enabling Support for Lua Scripting Language, and setting MEZZLUA51")
        set(Mezz_Lua51_Defined "MEZZLUA51=1")
else(${Mezz_Lua51})
        message ( STATUS "${PROJECT_NAME} - Disabling Support for Lua Scripting Language, and unsetting MEZZLUA51")
        set(Mezz_Lua51_Defined "MEZZLUA51_undefined")
endif(${Mezz_Lua51})

###############################################################################
# Misc simple defines
option(Mezz_ThrowOnSingletonFetch "When retrieving an invalid Singleton, should an exception be thrown" OFF)
if(${Mezz_ThrowOnSingletonFetch})
        message ( STATUS "${PROJECT_NAME} - Enabling exceptions on bad singleton fetches")
        set(Mezz_ThrowOnSingletonFetch_Defined "THROW_ON_FETCH_FAIL=1")
else(${Mezz_ThrowOnSingletonFetch})
        message ( STATUS "${PROJECT_NAME} - Disabling  exceptions on bad singleton fetches")
        set(Mezz_ThrowOnSingletonFetch_Defined "THROW_ON_FETCH_FAIL_undefined")
endif(${Mezz_ThrowOnSingletonFetch})
mark_as_advanced(Mezz_ThrowOnSingletonFetch)


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
# Mezzanine

set(ProjectRootDir ${${PROJECT_NAME}_SOURCE_DIR}/)
Message(STATUS "Using Source From: ${ProjectRootDir}")

set(ProjectBinaryDir ${${PROJECT_NAME}_BINARY_DIR}/)
Message(STATUS "Putting all major output in: ${ProjectBinaryDir}")

set(ProjectIncludeDir ${ProjectBinaryDir}/include/)
Message(STATUS "Putting mezzconfig.h in: ${ProjectIncludeDir}")


configure_file("${ProjectRootDir}Mezzanine/src/mezzconfig.h.original" "${ProjectIncludeDir}mezzconfig.h")
set(MezzanineLibFileName ${LibPrefix}Mezzanine${LibSuffix})
set(MezzanineLinkName "Mezzanine")

if(${WIN32})
	set(MezzanineLinkName lib${MezzanineLinkName}.dll.a)
endif(${WIN32})

#######################################
# SDL related values

#Find the SDL source
message ( STATUS "${PROJECT_NAME} - Looking for SDL")
file(GLOB PossibleMatches ${ProjectRootDir}Mezzanine/libincludes/*/sdlsrc/SDLBuild ${ProjectRootDir}Mezzanine/SDLBuild ${ProjectRootDir}../*/SDLBuild ${ProjectRootDir}../*/Mezzanine/SDLBuild)
foreach(Current ${PossibleMatches})
	message ( STATUS "${PROJECT_NAME} - Checking for SDL in ${Current}")
	if(IS_DIRECTORY ${Current})
		set(SDLTempDir ${Current})
	else(IS_DIRECTORY ${Current})
		message( STATUS "Found something similar to SDL build directory, not using: ${Current}")
	endif(IS_DIRECTORY ${Current})
endforeach(Current ${PossibleMatches})

set(TempDirDefined 0)
if(DEFINED SDLTempDir)
	set(TempDirDefined 1)
endif(DEFINED SDLTempDir)

set(Mezz_SDLBuildDir ${SDLTempDir} CACHE FILEPATH "Where were SDL was built")

if(${TempDirDefined} EQUAL 0)
	message( SEND_ERROR "Could not find SDL source, please provide path in Mezz_SDLBuildDir")
else(${TempDirDefined} EQUAL 0)
	message( STATUS "Found SDL build directory. Using: ${SDLTempDir}")
	mark_as_advanced(Mezz_SDLBuildDir)
endif(${TempDirDefined} EQUAL 0)

#######################################
# Ogre3d related values
if(${Mezz_Debug})
	message ( STATUS "${PROJECT_NAME} - Will we use Ogre Debug libraries")
	#set(OgreLinkName OgreMain_d)
	#set(OgresD _d)
	set(OgreLinkName OgreMain)
	set(OgresD "")
else(${Mezz_Debug})
	message ( STATUS "${PROJECT_NAME} - Will we use Ogre Release libraries")
	set(OgreLinkName OgreMain)
	set(OgresD "")
endif(${Mezz_Debug})

#This must be set manually each time ogre is upgraded.
set(Mezz_UseOgreVersion 1.8.0 CACHE STRING "The version of the ogre libraries to link against")

set(OgreLibFileName ${LibPrefix}${OgreLinkName}${LibSuffix})
set(OgreLinuxSoName ${OgreLibFileName}.${Mezz_UseOgreVersion})		#This value is gibberish on non-Linux platforms

set(OgreConfigHeaderIncludeDir "${ProjectBinaryDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/include/")
Message(STATUS "Putting Ogre3d config.h in: ${OgreConfigHeaderIncludeDir}")

set(OgreHeaderIncludeDir "${ProjectRootDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/OgreMain/include")
Message(STATUS "Putting Ogre3d headers in: ${OgreHeaderIncludeDir}")

# Ogre Plugins
set(OgrePluginCGPM Plugin_CgProgramManager${OgresD}${LibSuffix} )
set(OgrePluginRenderGL RenderSystem_GL${OgresD}${LibSuffix} )
set(OgrePluginParticle Plugin_ParticleFX${OgresD}${LibSuffix} )
###############################################################################
# Network Library Define

if(${Mezz_Network})
	if(${WIN32})
		set(NetworkLibrary ws2_32)
	endif(${WIN32})
endif(${Mezz_Network})


###############################################################################
# Add subprojects

Message(STATUS "Starting Subproject Configuration.")
add_subdirectory (Mezzanine)
#add_subdirectory (EngineDemo)
add_subdirectory (Catch!)
add_subdirectory (UnitTests)
Message(STATUS "Finishing Subproject Configuration.")

###############################################################################
# Hide Variables not directly related to the Mezzanine Engine
mark_as_advanced(CMAKE_BACKWARDS_COMPATIBILITY CMAKE_BUILD_TYPE CMAKE_CODEBLOCKS_EXECUTABLE CMAKE_INSTALL_PREFIX EXECUTABLE_OUTPUT_PATH LIBRARY_OUTPUT_PATH NVIDIA_OPENCL_INCLUDES NVIDIA_OPENCL_ICD_CFG NVIDIA_OPENCL_LIBRARIES OGG_LIBRARY VORBISFILE_LIBRARY VORBIS_LIBRARY )

message("${PROJECT_NAME} - Finishing Configuration")

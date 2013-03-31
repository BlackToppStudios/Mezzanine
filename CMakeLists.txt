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
# That affect/add/remove behavior of the Mezzanine engine
#        Mezz_Debug                         - True - The Engine will be forced to compile with debug symbols by setting CMAKE_BUILD_TYPE, this will also include more logging messages and libraries will also include debug symbols
#        Mezz_Lua51                         - True if lua is to be included and link against. False if lua was not selected to be compiled in.
#        Mezz_Network                       - Enable or disable networking support
#
# Advanced Mezzanine build options
#        Mezz_ForceBuildType                - When True this will force the CMAKE_BUILD_TYPE to correspond with Mezz_Debug, otherwise the user is expcte to manually set CMAKE_BUILD_TYPE.
#        Mezz_Profile                       - If Set to True, then a number of engine level and compiler tools will be enabled that track performance in the engine. This should be disabled when not debugging and benchmarking. Not supported in visual studio currently
#        Mezz_ThrowOnSingletonFetch         - When retrieving an invalid Singleton, should an exception be thrown, defaults to false.
#
# Depracated options
#        Mezz_XML                           - Previously disabled/enabled XML options, now XML is used everywhere. Thsi option has been removed.
#
# That affect only build time values:
#        OgreLinkName                       - This is the name of the ogre library that the compiler expects. Examples: OgreMain, OgreMain_d
#        OgresD                             - This can be used to help pick/select ogre libraries that may or may not have an "_d" depending on how they are compiled. If debug libraries are in use this is set to "_d" otherwise "".
#        OgreLibFileName                    - The filename of the actual Ogre library. On some platforms this file may just be a symlink. Examples: OgreMain.dll, libOgreMain.so
#        OgreLinuxSoName                    - Linux's versioned library system expects specific version of so files. This is the more specific file, that must copied along with the regular so. Examples libOgreMain.so.1.7.1, libOgreMain.so.1.9.3
#        OgrePluginCGPM                     - The File name of the Ogre CG manager plugin
#        OgrePluginRenderGL                 - The File name of the Ogre OpenGL rendersystem plugin
#        OgrePluginParticle                 - The File name of the Ogre ParticleFX plugin
#        OgreConfigHeaderIncludeDir         - The Folder where Ogre puts its configured Headers. This is only needed by subprojects if they link directly against Ogre.
#        OgreHeaderIncludeDir               - The Folder where all the non-configured headers go. This is only needed by subprojects if they link directly against Ogre.
#
# Here are some more values that can be used when creating cmake scripts that are indirectly
# set and should not be directly passed as cmake values
#        BuildPlatform                      - A string that Contains either "windows", "linux", or "macosx" at the time of this writing. This directly corresponds to the folders names in the Mezzanine engine repository.
#        CommonBuildFlags                   - A listing of all the currently set build flags to pass to the compiler at compile time. This is the mechanism that enforces Mezz_Debug's and Mezz_Profile's settings.
#        DynamicLibPrefix                   - This is a string that is required by a given platform to begin the filename of shared libraries. One windows it is "" on Linux/Unix it is "lib"
#        DynamicLibSuffix                   - This is a string that is required by a given platform to end the filename of shared libraries. One windows it is ".dll" on Linux/Unix it is ".so"
#        LuaLinkName                        - This is the name that of the actual compiled lua library, it will be blank if lua is not to be linked
#        LuaProject                         - The Name of the lua cmake project
#        MezzanineLinkName                  - What should be entered when somethings needs to link against Mezzanine usually "Mezzanine" sometime it specifies and import library, like
#        MezzanineLibFileName               - The actual name of the file the Mezzanine is called.

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "Prevented in source tree build. Please create a build directory outside of the Mezzanine source code and have cmake build from there.")
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

###############################################################################
# The name of our project is "MezzanineAndCatch".
cmake_minimum_required (VERSION 2.8)
project (MezzanineAndCatch)
message ("${PROJECT_NAME} - Starting Configuration.")

#######################################
# Mezzanine helper variables
set(ProjectRootDir ${${PROJECT_NAME}_SOURCE_DIR}/)
Message(STATUS "Using Source From: ${ProjectRootDir}")

set(ProjectBinaryDir ${${PROJECT_NAME}_BINARY_DIR}/)
Message(STATUS "Putting all major output in: ${ProjectBinaryDir}")

#######################################
# PostFix configuration
SET(CMAKE_DEBUG_POSTFIX "_Debug" CACHE STRING "Adds a postfix for debug-built libraries.")
SET(CMAKE_MINSIZEREL_POSTFIX "_MinsizeRel" CACHE STRING "Adds a postfix for MinsizeRelease-built libraries.")
SET(CMAKE_RELWITHDEBINFO_POSTFIX "_RelWithDebugInfo" CACHE STRING "Adds a postfix for ReleaseWithDebug-built libraries.")
mark_as_advanced(CMAKE_DEBUG_POSTFIX CMAKE_MINSIZEREL_POSTFIX CMAKE_RELWITHDEBINFO_POSTFIX)

###############################################################################
# Set Platform Specific Variables
set(DynamicLibPrefix "")
set(DynamicLibSuffix "")
set(StaticLibPrefix "")
set(StaticLibSuffix ".a")
set(LibPostfix "")

if(${WIN32})
    set(BuildPlatform windows)
    set(DynamicLibSuffix .dll)
    add_definitions( -DWINDOWS )
endif(${WIN32})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(BuildPlatform linux)
    set(DynamicLibPrefix lib)
    set(DynamicLibSuffix .so)
    add_definitions( -DLINUX )
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(BuildPlatform macosx)
    set(DynamicLibSuffix .dylib)
    add_definitions( -DMACOSX )
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

if(${WIN32})
    set(FPIC "")
else(${WIN32})
    set(FPIC "-fPIC")
endif(${WIN32})

if(MSVC)
    set(D "/D")
else(MSVC)
    set(D "-D")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=i486")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=i486")
    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=athlon64")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=athlon64")
endif(MSVC)


###############################################################################
# Check For the Mezz_Debug option and set values accordingly
option (Mezz_ForceBuildType "If true forces CMAKE_BUILD_TYPE to match Mezz_Debug." ON)
option (Mezz_Debug "Compile with no optimizations and debug symbols" OFF)
if (Mezz_Debug)
    message ( STATUS "${PROJECT_NAME} - Enabling Mezz_Debug Messages")
    set(Mezz_Debug_Arguments "${CMAKE_CXX_FLAGS_DEBUG} ${D}MEZZDEBUG")
    set(Mezz_Debug_Defined "MEZZDEBUG = 1")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Set by MEZZDEBUG, see Mezz_ForceBuildType for details" FORCE)
else (Mezz_Debug)
    message ("${PROJECT_NAME} - Disabling Mezz_Debug Messages")
    set (Mezz_Debug_Arguments "${D}MEZZDEBUG_undefined")
    set (Mezz_Debug_Defined "MEZZDEBUG_undefined")
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Set by MEZZDEBUG, see Mezz_ForceBuildType for details" FORCE)
endif (Mezz_Debug)

###############################################################################
# Now that we know our build type better, configure our library postfix.

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(LibPostFix ${CMAKE_DEBUG_POSTFIX})
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(LibPostFix ${CMAKE_RELEASE_POSTFIX})
endif(CMAKE_BUILD_TYPE STREQUAL "Release")
if(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
	set(LibPostFix ${CMAKE_RELWITHDEBINFO_POSTFIX})
endif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
if(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
	set(LibPostFix ${CMAKE_MINSIZEREL_POSTFIX})
endif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")

###############################################################################
# Set our other cmake variables that are dependent on the just set information

set(MezzanineLibFileName ${DynamicLibPrefix}Mezzanine${LibPostFix}${DynamicLibSuffix})
Message(STATUS "Expected name of compiled Mezzanine library: ${MezzanineLibFileName}")

set(MezzanineLinkName "Mezzanine")
if(${WIN32})
    #set(MezzanineLinkName lib${MezzanineLinkName}.dll.a)
	set(MezzanineLinkName ${MezzanineLibFileName})
endif(${WIN32})

###############################################################################
# Check For the Mezz_Profile option and set values accordingly
option(Mezz_Profile "Executables will gather runtime statistics." OFF)
if(${Mezz_Profile})
    message ( STATUS "${PROJECT_NAME} - Enabling gcc profiling and setting MEZZPROFILE")
    set(Mezz_Profile_Arguments "-pg ${D}MEZZPROFILE") # The pg makes this only work with gcc, clang and imitators
    set(Mezz_Profile_Defined "MEZZPROFILE = 1")
else(${Mezz_Profile})
    message ( STATUS "${PROJECT_NAME} - Disabling gcc profiling and unsetting MEZZPROFILE")
    set(Mezz_Profile_Arguments "${D}MEZZPROFILE_undefined")
    set(Mezz_Profile_Defined "MEZZPROFILE_undefined")
endif(${Mezz_Profile})

###############################################################################
# Check For the Mezz_XML option and set values accordingly
#option(Mezz_XML "Enable XML and XML serialization support." ON)
#if(${Mezz_XML})
#    message ( STATUS "${PROJECT_NAME} - Enabling XML subsystem, object serialization to XML, and setting MEZZXML")
#    set(Mezz_XML_Arguments "${D}MEZZXML")
#    set(Mezz_XML_Defined "MEZZXML = 1")
#else(${Mezz_XML})
#    message ( STATUS "${PROJECT_NAME} - Disabling XML subsystem, object serialization to XML, and unsetting MEZZXML")
#    set(Mezz_XML_Arguments "${D}MEZZXML_undefined")
#    set(Mezz_XML_Defined "MEZZXML_undefined")
#endif(${Mezz_XML})

###############################################################################
# Check For the Mezz_Network option and set values accordingly
option(Mezz_Network "Enable Networking support." ON)
if(${Mezz_Network})
    message ( STATUS "${PROJECT_NAME} - Enabling Network subsystem, and setting MEZZNETWORK")
    set(Mezz_Network_Arguments "${D}MEZZNETWORK")
else(${Mezz_Network})
    message ( STATUS "${PROJECT_NAME} - Disabling Network subsystem, and unsetting MEZZNETWORK")
    set(Mezz_Network_Arguments "${D}MEZZNETWORK_undefined")
endif(${Mezz_Network})

###############################################################################
# Lua Library
option(Mezz_Lua51 "Include Support for Lua 5.1 Scripting Language" ON)
if(${Mezz_Lua51})
    set(LuaProject "Lua")
    set(LuaLinkName "lua")
    message ( STATUS "${PROJECT_NAME} - Enabling Support for Lua Scripting Language, and setting MEZZLUA51")
    set(Mezz_Lua51_Arguments "${D}MEZZLUA51")
else(${Mezz_Lua51})
    message ( STATUS "${PROJECT_NAME} - Disabling Support for Lua Scripting Language, and unsetting MEZZLUA51")
    set(Mezz_Lua51_Arguments "${D}MEZZLUA51_undefined")
endif(${Mezz_Lua51})

###############################################################################
# Misc simple defines
option(Mezz_ThrowOnSingletonFetch "When retrieving an invalid Singleton, should an exception be thrown" OFF)
if(${Mezz_ThrowOnSingletonFetch})
    message ( STATUS "${PROJECT_NAME} - Enabling exceptions on bad singleton fetches")
    set(Mezz_ThrowOnSingletonFetch_Arguments "${D}THROW_ON_FETCH_FAIL")
else(${Mezz_ThrowOnSingletonFetch})
    message ( STATUS "${PROJECT_NAME} - Disabling exceptions on bad singleton fetches")
    set(Mezz_ThrowOnSingletonFetch_Arguments "${D}THROW_ON_FETCH_FAIL_undefined")
endif(${Mezz_ThrowOnSingletonFetch})

#######################################
# Ogre3d related values

set(OgreLibraryAllNames ${OgreLinkName} Plugin_ParticleFX Plugin_CgProgramManager)
set(Ogre_Arguments "")
set(OgreHeaderIncludeDir "")

# Detect available RenderSystems by abusing Ogre internal CMake variables
option (Mezz_BuildRenderSystemDirectX9 "Include Support for DirectX 9" OFF)
set(OGRE_BUILD_RENDERSYSTEM_D3D9 ${Mezz_BuildRenderSystemDirectX9} CACHE BOOL "Forcing Ogre option to match Mezzanine." FORCE)
if(${OGRE_BUILD_RENDERSYSTEM_D3D9})
    message ( STATUS "${PROJECT_NAME} - Enabling DirectX 9 Rendering Support")
    set(Ogre_Arguments "${Ogre_Arguments} ${D}MEZZ_BUILD_DIRECTX9_SUPPORT")
    set(OgreHeaderIncludeDir ${OgreHeaderIncludeDir} "${ProjectRootDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/RenderSystems/Direct3D9/include")
    set(OgreLibraryAllNames ${OgreLibraryAllNames} RenderSystem_Direct3D9)
else(${OGRE_BUILD_RENDERSYSTEM_D3D9})
    message ( STATUS "${PROJECT_NAME} - Disabling DirectX 9 Rendering Support")
    set(Ogre_Arguments "${Ogre_Arguments} ${D}MEZZ_BUILD_DIRECTX9_SUPPORT_undefined")
endif(${OGRE_BUILD_RENDERSYSTEM_D3D9})

option (Mezz_BuildRenderSystemDirectX11 "Include Support for DirectX 11" OFF)
set(OGRE_BUILD_RENDERSYSTEM_D3D11 ${Mezz_BuildRenderSystemDirectX11} CACHE BOOL "Forcing Ogre option to match Mezzanine." FORCE)
if(${OGRE_BUILD_RENDERSYSTEM_D3D11})
    message ( STATUS "${PROJECT_NAME} - Enabling DirectX 11 Rendering Support")
    set(Ogre_Arguments "${Ogre_Arguments} ${D}MEZZ_BUILD_DIRECTX11_SUPPORT")
    set(OgreHeaderIncludeDir ${OgreHeaderIncludeDir} "${ProjectRootDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/RenderSystems/Direct3D11/include")
    set(OgreLibraryAllNames ${OgreLibraryAllNames} RenderSystem_Direct3D11)
else(${OGRE_BUILD_RENDERSYSTEM_D3D11})
    message ( STATUS "${PROJECT_NAME} - Disabling DirectX 11 Rendering Support")
    set(Ogre_Arguments "${Ogre_Arguments} ${D}MEZZ_BUILD_DIRECTX11_SUPPORT_undefined")
endif(${OGRE_BUILD_RENDERSYSTEM_D3D11})

option (Mezz_BuildRenderSystemOpenGL "Include Support for OpenGL" ON)
set(OGRE_BUILD_RENDERSYSTEM_GL ${Mezz_BuildRenderSystemOpenGL} CACHE BOOL "Forcing Ogre option to match Mezzanine." FORCE)
if(${OGRE_BUILD_RENDERSYSTEM_GL})
    message ( STATUS "${PROJECT_NAME} - Enabling OpenGL Rendering Support")
    set(Ogre_Arguments ${Ogre_Arguments} "${D}MEZZ_BUILD_OPENGL_SUPPORT")
    set(OgreHeaderIncludeDir ${OgreHeaderIncludeDir} "${ProjectRootDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/RenderSystems/GL/include")
    set(OgreLibraryAllNames ${OgreLibraryAllNames} RenderSystem_GL)
else(${OGRE_BUILD_RENDERSYSTEM_GL})
    message ( STATUS "${PROJECT_NAME} - Disabling OpenGL Rendering Support")
    set(Ogre_Arguments "${Ogre_Arguments} ${D}MEZZ_BUILD_OPENGL_SUPPORT_undefined")
endif(${OGRE_BUILD_RENDERSYSTEM_GL})

option (Mezz_BuildRenderSystemOpenGLES "Include Support for OpenGLES" OFF)
set(OGRE_BUILD_RENDERSYSTEM_GLES ${Mezz_BuildRenderSystemOpenGLES} CACHE BOOL "Forcing Ogre option to match Mezzanine." FORCE)
if(${OGRE_BUILD_RENDERSYSTEM_GLES})
    message ( STATUS "${PROJECT_NAME} - Enabling OpenGLES Rendering Support")
    set(Ogre_Arguments ${Ogre_Arguments} "${D}MEZZ_BUILD_OPENGLES_SUPPORT")
    set(OgreHeaderIncludeDir ${OgreHeaderIncludeDir} "${ProjectRootDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/RenderSystems/GLES/include")
    set(OgreLibraryAllNames ${OgreLibraryAllNames} RenderSystem_GLES)
else(${OGRE_BUILD_RENDERSYSTEM_GLES})
    message ( STATUS "${PROJECT_NAME} - Disabling OpenGLES Rendering Support")
    set(Ogre_Arguments "${Ogre_Arguments} ${D}MEZZ_BUILD_OPENGLES_SUPPORT_undefined")
endif(${OGRE_BUILD_RENDERSYSTEM_GLES})

option (Mezz_BuildRenderSystemOpenGLES2 "Include Support for OpenGLES2" OFF)
set(OGRE_BUILD_RENDERSYSTEM_GLES2 ${Mezz_BuildRenderSystemOpenGLES2} CACHE BOOL "Forcing Ogre option to match Mezzanine." FORCE)
if(${OGRE_BUILD_RENDERSYSTEM_GLES2})
    message ( STATUS "${PROJECT_NAME} - Enabling OpenGLES2 Rendering Support")
    set(Ogre_Arguments ${Ogre_Arguments} "${D}MEZZ_BUILD_OPENGLES2_SUPPORT")
    set(OgreHeaderIncludeDir ${OgreHeaderIncludeDir} "${ProjectRootDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/RenderSystems/GLES2/include")
    set(OgreLibraryAllNames ${OgreLibraryAllNames} RenderSystem_GLES2)
else(${OGRE_BUILD_RENDERSYSTEM_GLES2})
    message ( STATUS "${PROJECT_NAME} - Disabling OpenGLES2 Rendering Support")
    set(Ogre_Arguments ${Ogre_Arguments} "${D}MEZZ_BUILD_OPENGLES2_SUPPORT_undefined")
endif(${OGRE_BUILD_RENDERSYSTEM_GLES2})

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
set(Mezz_UseOgreVersion 1.8.1 CACHE STRING "The version of the ogre libraries to link against")
set(OgreDepName Ogre)
set(OgreLibFileName ${StaticLibPrefix}${OgreLinkName}${StaticLibSuffix})
set(OgreLinuxSoName ${OgreLibFileName}.${Mezz_UseOgreVersion})                #This value is gibberish on non-Linux platforms

set(OgreConfigHeaderIncludeDir "${ProjectBinaryDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/include/")
Message(STATUS "Putting Ogre3d config.h in: ${OgreConfigHeaderIncludeDir}")

set(OgreHeaderIncludeDir ${OgreHeaderIncludeDir} "${ProjectRootDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/OgreMain/include")
set(OgreHeaderIncludeDir ${OgreHeaderIncludeDir} "${ProjectRootDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/PlugIns/ParticleFX/include")
set(OgreHeaderIncludeDir ${OgreHeaderIncludeDir} "${ProjectRootDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/PlugIns/CgProgramManager/include")

# Line below is needed on Windows, maybe other platforms as well.
if(${WIN32})
	set(OgreHeaderIncludeDir ${OgreHeaderIncludeDir} "${ProjectRootDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/Dependencies/src/Cg/include")
endif(${WIN32})
Message(STATUS "Looking for Ogre3d headers in: ${OgreHeaderIncludeDir}")

# Ogre Plugins
#set(OgrePluginCGPM ${StaticLibPrefix}Plugin_CgProgramManager${OgresD}${StaticLibSuffix})
#set(OgrePluginRenderGL ${StaticLibPrefix}RenderSystem_GL${OgresD}${StaticLibSuffix})
#set(OgrePluginParticle ${StaticLibPrefix}Plugin_ParticleFX${OgresD}${StaticLibSuffix})



###############################################################################
# Network Library Define
if(${Mezz_Network})
    if(${WIN32})
        set(NetworkLibrary ws2_32)
    endif(${WIN32})
    # set windows and linux options here if required
endif(${Mezz_Network})

###############################################################################
# Misc library linking names
set(Bullet_LibraryNames
    "ConvexDecomposition"
    "GIMPACTUtils"
    "BulletWorldImporter"
    "BulletFileLoader"
    #"BulletSoftBodySolvers_OpenCL"
    #"BulletSoftBodySolvers_CPU"
    "MiniCL"
    "BulletMultiThreaded"
    "BulletSoftBody" "BulletDynamics" "BulletCollision" "LinearMath"
)

set(SoundLibraryNames
    "cAudio"
    "openal"
    "oggvorbis"
)

set(SDLLibraryNames
	"SDL2-static"
)

###############################################################################
# Perform Common Tasks for build options and configuration.
set(CommonBuildFlags "${Mezz_Debug_Arguments} ${Mezz_Profile_Arguments} ${Mezz_Network_Arguments} ${Mezz_Lua51_Arguments} ${Mezz_ThrowOnSingletonFetch_Arguments} ${Ogre_Arguments} ${FPIC}")
#message(FATAL_ERROR ${CommonBuildFlags})

###############################################################################
# Add subprojects

Message(STATUS "Starting Subproject Configuration.")
add_subdirectory (Mezzanine)
add_subdirectory (EngineDemo)
add_subdirectory (Catch!)
add_subdirectory (UnitTests)
Message(STATUS "Finishing Subproject Configuration.")

###############################################################################
# Hide Variables not directly related to the Mezzanine Engine
mark_as_advanced( FORCE

    # Mezzanine options
    Mezz_ForceBuildType Mezz_UseOgreVersion Mezz_Profile Mezz_ThrowOnSingletonFetch

    # Misc Options
    CMAKE_BACKWARDS_COMPATIBILITY CMAKE_BUILD_TYPE CMAKE_CODEBLOCKS_EXECUTABLE CMAKE_INSTALL_PREFIX CMAKE_DEBUG_POSTFIX CMAKE_GNUtoMS
    CMAKE_CXX_COMPILER CMAKE_C_COMPILER CMAKE_RC_COMPILER
    EXECUTABLE_OUTPUT_PATH LIBRARY_OUTPUT_PATH
    NVIDIA_OPENCL_INCLUDES NVIDIA_OPENCL_ICD_CFG NVIDIA_OPENCL_LIBRARIES
    OGG_LIBRARY VORBISFILE_LIBRARY VORBIS_LIBRARY

    # Open AL soft options
    ALSOFT_WINE ALSOFT_FORCE_STATIC_VCRT
    ALSOFT_CPUEXT_SSE ALSOFT_CPUEXT_NEON ALSOFT_REQUIRE_SSE ALSOFT_REQUIRE_NEON
    ALSOFT_BACKEND_ALSA ALSOFT_BACKEND_OSS ALSOFT_BACKEND_SOLARIS ALSOFT_BACKEND_SNDIO ALSOFT_BACKEND_MMDEVAPI
    ALSOFT_BACKEND_DSOUND ALSOFT_BACKEND_WINMM ALSOFT_BACKEND_PORTAUDIO ALSOFT_BACKEND_PULSEAUDIO ALSOFT_BACKEND_COREAUDIO
    ALSOFT_BACKEND_OPENSL ALSOFT_BACKEND_WAVE
    ALSOFT_REQUIRE_ALSA ALSOFT_REQUIRE_OSS ALSOFT_REQUIRE_SOLARIS ALSOFT_REQUIRE_SNDIO ALSOFT_REQUIRE_MMDEVAPI
    ALSOFT_REQUIRE_DSOUND ALSOFT_REQUIRE_WINMM ALSOFT_REQUIRE_PORTAUDIO  ALSOFT_REQUIRE_PULSEAUDIO ALSOFT_REQUIRE_COREAUDIO ALSOFT_REQUIRE_OPENSL
    ALSOFT_DLOPEN ALSOFT_WERROR ALSOFT_UTILS ALSOFT_EXAMPLES ALSOFT_CONFIG

    D_PULSE_LIB _ALSA_LIB

    # SDL Sub-systems
    SDL_ATOMIC SDL_AUDIO SDL_CPUINFO SDL_EVENTS SDL_FILE SDL_HAPTIC SDL_JOYSTICK SDL_LOADSO
    SDL_POWER SDL_RENDER SDL_SHARED SDL_STATIC SDL_THREADS SDL_TIMERS SDL_VIDEO

    # SDL options
    SDLOPTION_ASSEMBLY SDLOPTION_SSEMATH
    SDLOPTION_ASSERTIONS SDLOPTION_LIBC SDLOPTION_GCC_ATOMICS SDLOPTION_GCC_ATOMICS SDLOPTION_MMX SDLOPTION_3DNOW SDLOPTION_SSE SDLOPTION_SSE2
    SDLOPTION_ALTIVEC SDLOPTION_DISKAUDIO SDLOPTION_DUMMYAUDIO SDLOPTION_VIDEO_DIRECTFB SDLOPTION_DIRECTFB_SHARED SDLOPTION_FUSIONSOUND
    SDLOPTION_FUSIONSOUND_SHARED SDLOPTION_VIDEO_DUMMY SDLOPTION_VIDEO_OPENGL SDLOPTION_VIDEO_OPENGLES SDLOPTION_PTHREADS SDLOPTION_PTHREADS_SEM
    SDL_DLOPEN SDLOPTION_OSS SDLOPTION_ALSA SDLOPTION_ALSA_SHARED SDLOPTION_ESD SDLOPTION_ESD_SHARED SDLOPTION_PULSEAUDIO SDLOPTION_PULSEAUDIO_SHARED
    SDLOPTION_ARTS SDLOPTION_ARTS_SHARED SDLOPTION_NAS SDLOPTION_RPATH SDLOPTION_CLOCK_GETTIME SDLOPTION_INPUT_TSLIB SDLOPTION_VIDEO_X11
    SDLOPTION_X11_SHARED SDLOPTION_VIDEO_X11_XCURSOR SDLOPTION_VIDEO_X11_XINERAMA SDLOPTION_VIDEO_X11_XINPUT SDLOPTION_VIDEO_X11_XRANDR
    SDLOPTION_VIDEO_X11_XSCRNSAVER SDLOPTION_VIDEO_X11_XSHAPE SDLOPTION_VIDEO_X11_XVM SDLOPTION_VIDEO_COCOA SDLOPTION_DIRECTX
    SDLOPTION_RENDER_D3D

    # SDL Indirect Options
    ARTS_CONFIG D_NAS_LIB XCURSOR_LIB XEXT_LIB XI_LIB XINERAMA_LIB XRANDR_LIB XRENDER_LIB XSS_LIB XXF86VM_LIB X11_LIB

    # Ogre indirect
    Boost_DIR Boost_LIB_DIAGNOSTIC_DEFINITIONS OGRE_SET_USE_BOOST
    DirectX_DINPUT8_LIBRARY
    Cg_LIBRARY_REL Cg_LIBRARY_DBG Cg_INCLUDE_DIR

    # Ogre
    OGRE_STATIC
    OGRE_BUILD_RENDERSYSTEM_D3D9
    OGRE_BUILD_RENDERSYSTEM_D3D11
    OGRE_BUILD_RENDERSYSTEM_GL
    OGRE_BUILD_RENDERSYSTEM_GLES
    OGRE_BUILD_RENDERSYSTEM_GLES2
    OGRE_BUILD_PLATFORM_APPLE_IOS
    OGRE_BUILD_PLATFORM_NACL
    OGRE_BUILD_PLUGIN_BSP
    OGRE_BUILD_PLUGIN_OCTREE
    OGRE_BUILD_PLUGIN_PFX
    OGRE_BUILD_PLUGIN_PCZ
    OGRE_BUILD_COMPONENT_PAGING
    OGRE_BUILD_COMPONENT_TERRAIN
    OGRE_BUILD_COMPONENT_PROPERTY
    OGREDEPS_BUILD_CG
    OGRE_BUILD_PLUGIN_CG
    OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
    OGRE_BUILD_RTSHADERSYSTEM_CORE_SHADERS
    OGRE_BUILD_RTSHADERSYSTEM_EXT_SHADERS
    OGRE_BUILD_SAMPLES
    OGRE_BUILD_TOOLS
    OGRE_BUILD_XSIEXPORTER
    OGRE_BUILD_TESTS
    OGRE_CONFIG_DOUBLE
    OGRE_COPY_DEPENDENCIES OGRE_INSTALL_DEPENDENCIES
    OGRE_DEPENDENCIES_DIR
    OGRE_CONFIG_THREADS OGRE_CONFIG_THREAD_PROVIDER
    OGRE_CONFIG_CONTAINERS_USE_CUSTOM_ALLOCATOR OGRE_CONFIG_STRING_USE_CUSTOM_ALLOCATOR OGRE_CONFIG_MEMTRACK_DEBUG OGRE_CONFIG_MEMTRACK_RELEASE
    OGRE_UNITY_BUILD OGRE_UNITY_FILES_PER_UNIT

    WINDRES WINE
)

message("${PROJECT_NAME} - Finishing Configuration")

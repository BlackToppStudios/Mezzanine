cmake_minimum_required (VERSION 2.8)
project (Ogre)
message ( "${PROJECT_NAME} - Begin" )

#Choose the destination directory
set( Ogre_BINARY_DIR ${LIBRARY_OUTPUT_PATH} )
#set( OGRE_BINARY_DIR ${LIBRARY_OUTPUT_PATH} )
set( OgreDeps_BINARY_DIR ${LIBRARY_OUTPUT_PATH} )
#set( OGREDEPS_BINARY_DIR ${LIBRARY_OUTPUT_PATH} )
message ( STATUS "Putting Ogre binaries in: ${Ogre_BINARY_DIR}" )
message ( STATUS "Putting Ogre Dependancy binaries in: ${OgreDeps_BINARY_DIR}" )

# Long list of changes necessary to get ogre properly merged with our project.
# One quick thing to note is that all I applied these changes from top to bottom noting their line number as I went.  If you try to use the mentioned
# line numbers in an unaltered version of the file you will have a bad time.  I try to explain the area of the file by their settings where I could to alleviate this.
#
# Before you get started it's good to know that the Dependencies folder referenced here has to be created and placed manually, and the contents of the dependency source 
# repo placed in it minus the hg config files.  A dependencies folder with most/all of the necessary changes should already exist in the repo and be reusable from one 
# version of Ogre to the next for the foreseeable future.
#
## Ogre Core Setup
#
# In ogresvnsrc/OgreMain/Src/OgreZip.cpp:
# At around line 37 change the include '#include <zzip/zzip.h>' to '#include <zzip.h>'
#
# On windows, you need to change line 260 in ogresvnsrc/Rendersystems/GL/src/Win32/OgreWin32Window.cpp from
# + ::Ogre::StringConverter::toString(le)
# to this,
# + ::Ogre::StringConverter::toString(static_cast<unsigned int>(le))
# 
# OpenGL and OpenGL3Plus rendersystems don't seem to want to play nice.  It's entirely possible that other similar rendersystems have similar behavior.  The issue with the OGL
# rendersystems is that in the include paths they have an identically named file, one that includes OGL2 headers, and another includes OGL3 headers.  These headers are entirely 
# incompatible and if both are included in the same file/include chain then a compilation error will result.  Exactly that happens in graphicsmanager.cpp.  Even if they aren't,
# there isn't much guarentee that the wrong "OgreGLSLExtSupport.h" file will be included.  The directory structure for the GL and GL3Plus rendersystems differs greatly though
# and it seems like the cleanest fix is to use this and alter the include entry in the files to ensure it gets the proper file.
# So a find and replace should be performed where "#include "OgreGLSL" is replaced with "#include "GLSL/OgreGLSL" for all files stored under the "ogresvnsrc\RenderSystems\GL3Plus"
# directory.
# 
## Ogre Dependencies Setup
#
# The "ios" and "Cg" folders of the dependencies should be deleted and their entries to their directory 
#   in the cmake configuration removed, as we do not use Ogre functionality that requires them.
# 
# In ogresvnsrc/Dependencies/src/zziplib/CMakeLists.txt:
# At line 38, remove the "zzip/" portion of the filename.  Also move the actual file to it's parent folder to properly account for the other change.
# The above change also necessitates removal of the "zzip/" portion of the includes in each file of the zziplib that needs it.  The files that need it are below:
# 1. lib.h
# 2. plugin.h
#
# In ogresvnsrc/Dependencies/src/FreeImage/CMakeLists.txt
# Add a line that explicitly tells cmake we need to link against winsock2 if we are on windows.  Add an "if(WIN32)" block with "target_link_libraries(FreeImage ws2_32)" to around line 536.
#
# --ogresvnsrc/Dependencies/src/FreeImage/Source/LibMNG/libmng_conf.h
# --remark out line 80 to disable the definition of MNG_OPTIMIZE_CHUNKREADER to allow compilation
#
# To get -std=c++11
# in dcraw_common.cpp 
# around 9125 this: tiff_set (&th->ntag, 270, , 512, TOFF(th->t_desc));
# should change to: tiff_set (&th->ntag, 270, 0, 512, TOFF(th->t_desc));
# and 
# around 9125 this: strcpy (th->soft, "dcraw v"DCRAW_VERSION);
# should change to: strcpy (th->soft, "dcraw v9.12");


# Ogre Core
set(OGRE_STATIC TRUE CACHE BOOL "Forcing static build for Mezzanine build." FORCE)
set(OGRE_SET_USE_BOOST FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_USE_BOOST FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_CONFIG_THREADS 0 CACHE INT "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_CONFIG_THREAD_PROVIDER "" CACHE STRING "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_BUILD_PLUGIN_CG FALSE CACHE BOOL "Forcing Ogre to disable CG, because it is disgusting." FORCE)
set(OGRE_CONFIG_ALLOCATOR 1 CACHE STRING "Forcing static build for Mezzanine build." FORCE)

# Ogre Components
#set(OGRE_BUILD_COMPONENT_HLMS FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
#set(OGRE_BUILD_COMPONENT_MESHLODGENERATOR FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_BUILD_COMPONENT_OVERLAY FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_BUILD_COMPONENT_PAGING FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_BUILD_COMPONENT_RTSHADERSYSTEM FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_BUILD_COMPONENT_TERRAIN FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_BUILD_COMPONENT_VOLUME FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)

# Ogre Dependencies
set(OGREDEPS_BUILD_CG FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
set(OGREDEPS_BUILD_FREEIMAGE TRUE CACHE BOOL "Enabling extended image format support for Ogre." FORCE)
set(OGREDEPS_BUILD_FREETYPE TRUE CACHE BOOL "Enabling vector text support for Ogre." FORCE)
set(OGREDEPS_BUILD_OIS FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
set(OGREDEPS_BUILD_ZLIB TRUE CACHE BOOL "Enabling enhanced resource reading for Ogre." FORCE)
set(OGREDEPS_BUILD_ZZIPLIB TRUE CACHE BOOL "Enabling enhanced resource reading for Ogre." FORCE)
set(OGREDEPS_INSTALL_DEV FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)

# Ogre Extras
set(OGRE_INSTALL_DOCS FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_INSTALL_SAMPLES FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_INSTALL_TOOLS FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_INSTALL_VSPROPS FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)

# Ogre Paths
set(OGREDEPS_LIB_DIRECTORY ${OgreDeps_BINARY_DIR} CACHE STRING "Forcing output directory to unified folder." FORCE)
set(OGRE_ARCHIVE_OUTPUT ${Ogre_BINARY_DIR} CACHE STRING "Forcing output directory to unified folder." FORCE)
set(OGRE_LIBRARY_OUTPUT ${Ogre_BINARY_DIR} CACHE STRING "Forcing output directory to unified folder." FORCE)
set(OGRE_RUNTIME_OUTPUT ${Ogre_BINARY_DIR} CACHE STRING "Forcing output directory to unified folder." FORCE)

set(OGRE_RELEASE_PATH "" CACHE STRING "Forcing output directory to unified folder." FORCE)
set(OGRE_RELWDBG_PATH "" CACHE STRING "Forcing output directory to unified folder." FORCE)
set(OGRE_MINSIZE_PATH "" CACHE STRING "Forcing output directory to unified folder." FORCE)
set(OGRE_DEBUG_PATH "" CACHE STRING "Forcing output directory to unified folder." FORCE)
set(OGRE_LIB_RELEASE_PATH "" CACHE STRING "Forcing output directory to unified folder." FORCE)
set(OGRE_LIB_RELWDBG_PATH "" CACHE STRING "Forcing output directory to unified folder." FORCE)
set(OGRE_LIB_MINSIZE_PATH "" CACHE STRING "Forcing output directory to unified folder." FORCE)
set(OGRE_LIB_DEBUG_PATH "" CACHE STRING "Forcing output directory to unified folder." FORCE)
set(OGRE_PLUGIN_PATH "/" CACHE STRING "Forcing output directory to unified folder." FORCE)
set(OGRE_SAMPLE_PATH "/" CACHE STRING "Forcing output directory to unified folder." FORCE)

add_subdirectory ( ogresvnsrc )

mark_as_advanced( FORCE
    # Ogre indirect
    Boost_DIR 
    Boost_LIB_DIAGNOSTIC_DEFINITIONS 
    OGRE_SET_USE_BOOST 
    BOOST_THREAD_LIBRARY
    DirectX_DINPUT8_LIBRARY
    Cg_LIBRARY_REL 
    Cg_LIBRARY_DBG 
    Cg_INCLUDE_DIR
    
    # Ogre Core
    OGRE_STATIC
    OGRE_ASSERT_MODE
    OGRE_BUILD_RENDERSYSTEM_D3D9
    OGRE_BUILD_RENDERSYSTEM_D3D11
    OGRE_BUILD_RENDERSYSTEM_GL
    OGRE_BUILD_RENDERSYSTEM_GL3PLUS
    OGRE_BUILD_RENDERSYSTEM_GLES
    OGRE_BUILD_RENDERSYSTEM_GLES2
    OGRE_BUILD_PLATFORM_APPLE_IOS
    OGRE_BUILD_PLATFORM_NACL
    OGRE_BUILD_PLUGIN_BSP
    OGRE_BUILD_PLUGIN_OCTREE
    OGRE_BUILD_PLUGIN_PFX
    OGRE_BUILD_PLUGIN_PCZ
    OGRE_BUILD_PLUGIN_CG
    
    OGRE_CONFIG_DOUBLE
    OGRE_CONFIG_THREADS 
    OGRE_CONFIG_THREAD_PROVIDER
    OGRE_CONFIG_CONTAINERS_USE_CUSTOM_ALLOCATOR 
    OGRE_CONFIG_STRING_USE_CUSTOM_ALLOCATOR 
    OGRE_CONFIG_MEMTRACK_DEBUG 
    OGRE_CONFIG_MEMTRACK_RELEASE
    OGRE_CONFIG_ENABLE_QUAD_BUFFER_STEREO
    
    OGRE_UNITY_BUILD 
    OGRE_UNITY_FILES_PER_UNIT
    
    # Ogre Components
    OGRE_BUILD_COMPONENT_HLMS
    OGRE_BUILD_COMPONENT_MESHLODGENERATOR
    OGRE_BUILD_COMPONENT_OVERLAY
    OGRE_BUILD_COMPONENT_PAGING
    OGRE_BUILD_COMPONENT_PROPERTY
    OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
    OGRE_BUILD_COMPONENT_TERRAIN
    OGRE_BUILD_COMPONENT_VOLUME
    OGRE_BUILD_RTSHADERSYSTEM_CORE_SHADERS
    OGRE_BUILD_RTSHADERSYSTEM_EXT_SHADERS
    
    # Ogre Dependencies
    OGREDEPS_BUILD_CG
    OGREDEPS_BUILD_FREEIMAGE
    OGREDEPS_BUILD_FREETYPE
    OGREDEPS_BUILD_OIS
    OGREDEPS_BUILD_ZLIB
    OGREDEPS_BUILD_ZZIPLIB
    OGREDEPS_INSTALL_DEV
    OGRE_COPY_DEPENDENCIES 
    OGRE_INSTALL_DEPENDENCIES
    OGRE_DEPENDENCIES_DIR
    
    # Ogre Extras
    OGRE_BUILD_SAMPLES
    OGRE_BUILD_TOOLS
    OGRE_BUILD_XSIEXPORTER
    OGRE_BUILD_TESTS
    OGRE_INSTALL_DOCS
    OGRE_INSTALL_SAMPLES
    OGRE_INSTALL_TOOLS
    OGRE_INSTALL_VSPROPS
    
    # Ogre Paths
    Ogre_BINARY_DIR
    OGRE_BINARY_DIR
    OgreDeps_BINARY_DIR
    OGREDEPS_BINARY_DIR
    
    OGRE_ARCHIVE_OUTPUT
    OGRE_LIBRARY_OUTPUT
    OGRE_RUNTIME_OUTPUT
    
    OGRE_RELEASE_PATH
    OGRE_RELWDBG_PATH
    OGRE_MINSIZE_PATH
    OGRE_DEBUG_PATH
    OGRE_LIB_RELEASE_PATH
    OGRE_LIB_RELWDBG_PATH
    OGRE_LIB_MINSIZE_PATH
    OGRE_LIB_DEBUG_PATH
    OGRE_PLUGIN_PATH
    OGRE_SAMPLE_PATH
)

message ("${PROJECT_NAME} - End")

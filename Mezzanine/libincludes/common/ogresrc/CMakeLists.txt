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
# In ogresvnsrc/CMakeLists.txt:
# Around line 547 the cmake configuration will add the Samples subdirectory regardless of the cmake config, where additional code is run that depends on data not set.
# Commenting out this line resolves the issue of this cmake config running on CMake 3.4+.  A better solution to the cmake config should be made for something long term.
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
#
# In the root CMakeLists.txt right after the "project(OGRE)" declaration
# Add the line "set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")"
#
# In OgrePrerequisites.h around line 337 where all the resource shared pointers are declared:
# Change all "typedef SharedPtr<" to "typedef std::shared_ptr<".  Was 18 lines in total on last edit.
# Same file, just underneath the "#include <string>" around line 32, added "#include <memory>"
#
#
#
# The final change to make std::shared_ptr work throughout Ogre requires repo-wide edits using a series of normal and regex replaces.
# The first two stages are just to make inconsistent styling consistent with the rest of the Ogre code base.
# All of these were performed in Notepad++ and most used "*.h *.hpp *.c *.cpp" as the filter.
#
# 1)
# Find: (\()(\w+?)\.isNull\(\)[ \t]?==[ \t]?true[ \t]?(\))
# Replace: $1!$2$3
# 2)
# Find: (\()(\w+?)\.isNull\(\)[ \t]?==[ \t]?false[ \t]?(\))
# Replace: $1$2$3
# 3)
# Find: (return[ \t]|=[ \t]?)([^;{}]+?).staticCast<(\w+?)>\(\)
# Replace(NP++): $1std::static_pointer_cast<$3>\( $2 \)
# Replace(RegExr): $1std::static_pointer_cast<$3>( $2 )
# 4)
# (Only *.c *.cpp filters for this)
# Find: (return|=|\(|[\&\|]{2}|\n[ \t]+?)([ \t]?!?)([ \t]?)([\w\[\]\.\->_:]+?)\.isNull\(\)
# Replace: $1$2!$4
# 5)
# (Only *.c *.cpp filters for this)
# Find: (if\([ \t]?)?(!?[\w\.\(\)->_]+)get(\w+)\(\)\.isNull\(\)
# Replace(NP++): $1!$2get$3\(\)
# Replace(RegExr): $1!$2get$3()
# 6)
# Find: (return|=|\(|[\&\|]{2}|\n[ \t]+?)([ \t\n]+)?!!(\w*?)
# Replace: $1$2$3
# 7)
# (Be sure to check the "Match Case" option)
# Find: (\.|\->)(use)[Cc](ount\(\))
# Replace: $1$2_c$3
# 8)
# Find: (:[ \t]?)(public[ \t])?SharedPtr<H
# Replace: $1$2std::shared_ptr<H
# 9)
# Find: (Ogre::)?SharedPtr<
# Replace: std::shared_ptr<
# 10)
# Find: (\(OGRE_NEW_T\()(\w+?)(,[ \t]?MEMCATEGORY_GENERAL\)\(\),[ \t]?)SPFM_DELETE_T(\))
# Replace(NP++): $1$2$3[]\($2* Ptr\){ OGRE_DELETE_T\(Ptr,$2,MEMCATEGORY_GENERAL\); }$4
# Replace(RegExr): $1$2$3[]($2* Ptr){ OGRE_DELETE_T(Ptr,$2,MEMCATEGORY_GENERAL); }$4
#
# Additionally, a few non-regex find/replaces were needed:
# 1)
# Find: ".getPointer()"
# Replace: ".get()"
# 2)
# Find: "->getPointer()"
# Replace: "->get()"
# 3)
# Find: ".setNull()"
# Replace: ".reset()"
# 4)
# Find: "->setNull()"
# Replace: "->reset()"
# 5)
# Find: ".bind("
# Replace: ".reset("
# 6)
# Find: "GpuProgramManager::getSingleton().getByName(evt.mName).isNull()"
# Replace: "!GpuProgramManager::getSingleton().getByName(evt.mName)"
#
# Here is a list of found false positives with the above regex's:
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\Dependencies\src\FreeImage\Source\FreeImage\PluginTARGA.cpp:L574
# Action: Revert changes in file.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\include\OgreAxisAlignedBox.h:L107,L130,L565,L595,L801
# Action: (AABB Fix) Revert changes in file.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\include\OgreGpuProgramParams.h:L1535,L1541
# Action: Added " != nullptr" to the return statement to convert it to bool.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\include\OgreGpuProgram.h:L367
# Action: Added " != nullptr" to the return statement to convert it to bool.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\include\OgrePlaneBoundedVolume.h:L67
# Action: (AABB Fix) Revert changes in file.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\include\OgreResourceManager.h:L357,L362
# Action: Was missed entirely because of extra () in the line.  Added " != nullptr" to the line.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\include\OgreSharedPtr.h:>5 lines
# Action: Revert changes in file. We don't care about anything in the original shared ptr implementation.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgreBillboardChain.cpp:L438,L469
# Action: (AABB Fix) Revert the two lines with the AABBs, since other edits to the file were valid.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgreBillboardSet.cpp:L555
# Action: (AABB Fix) Revert the one line back to ".setNull()", since other edits to the file were valid.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgreEntity.cpp:L528,L575,L600,L610
# Action: (AABB Fix) Revert the four lines with the AABBs, since other edits to the file were valid.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgreFileSystem.cpp:L320,L330,L341,L353
# Action: (Defunct/Solved) 4 lines containing "SPFM_DELETE_T" should be replaced with one of the following as appropriate:
# "[](StringVector* Ptr){ OGRE_DELETE_T(Ptr,StringVector,MEMCATEGORY_GENERAL); }" or "[](FileInfoList* Ptr){ OGRE_DELETE_T(Ptr,FileInfoList,MEMCATEGORY_GENERAL); }"
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgreFrustrum.cpp:L227
# Action: (AABB Fix) Revert changes in file.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgreInstanceBatch.cpp:L137
# Action: (AABB Fix) Revert changes in file.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgreInstancedGeometry.cpp:L106
# Action: (AABB Fix) Revert the one line, other changes in file are valid.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgreManualObject.cpp:L75
# Action: (AABB Fix) Revert the one line, other changes in file are valid.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgreMath.cpp:L506,L635,L823
# Action: (AABB Fix) Revert changes in file.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgreMesh.cpp:L2135
# Action: (Regex Miss) Manually remove the "!" and ".isNull()" on that line.  This was missed by the regex because of a * dereference on the line.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgreMeshSerializerImpl.cpp:L75
# Action: (AABB Fix) Revert the one line, other changes in file are valid.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgreMeshSerializerImpl.cpp:L833
# Action: (AABB Fix) Revert the one line, other changes in file are valid.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgrePlane.cpp:L91
# Action: (AABB Fix) Revert changes in file.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgreResourceGroupManager.cpp:L783,L1040,L1541,L1572,L1603,L1633,L1820
# Action: (Regex Made) 6 lines containing SPFM_DELETE_T should be replaced with appropriate lambdas.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgreSceneManager.cpp:L7347,L7348
# Action: (AABB Fix) Revert those two lines, other changes in file are valid.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgreSceneNode.cpp:L253
# Action: (AABB Fix) Revert changes in file.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgreShadowCameraSetupFocused.cpp:L436,L674
# Action: (AABB Fix) Revert changes in file.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgreShadowCameraSetupLiSPSM.cpp:L249
# Action: (AABB Fix) Revert changes in file.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgreShadowCaster.cpp:L439
# Action: (AABB Fix) Revert changes in file.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgreShadowVolumeExtrudeProgram.cpp:L901-902
# Action: (Regex Miss) Manually edit the line to remove the ".isNull()" and including a "!".  The regex missed this because of the line split.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgreStaticGeometry.cpp:L80
# Action: (AABB Fix) Revert the one line, other changes in file are valid.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\src\OgreUnifiedHighLevelGpuProgram.cpp:L184
# Action: Manually add ".operator bool()" to the return statement to convert to a bool.
#
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\PlugIns\OctreeSceneManager\src\OgreOctreeCamera.cpp:L54
# Action: (AABB Fix) Revert changes in file.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\PlugIns\OctreeSceneManager\src\OgreOctreeNode.cpp:L110,L111,L133,L145
# Action: (AABB Fix) Revert changes in file.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\PlugIns\OctreeSceneManager\src\OgreOctreeSceneManager.cpp:L75,L146,L186,L228,L411
# Action: (AABB Fix) Revert changes in file.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\PlugIns\OctreeZone\src\OgreOctreeZone.cpp:L1106,L1255,L1270,L1281
# Action: (AABB Fix) Revert changes in file.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\PlugIns\OctreeZone\src\OgreOctreeZoneOctree.cpp:L59,L129,L169,L209
# Action: (AABB Fix) Revert changes in file.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\PlugIns\PCZSceneManager\src\OgrePCZCamera.cpp:L61.L95
# Action: (AABB Fix) Revert changes in file.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\PlugIns\PCZSceneManager\src\OgrePCZFrustum.cpp:L83,L278,L422
# Action: (AABB Fix) Revert changes in file.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\PlugIns\PCZSceneManager\src\OgrePCZone.cpp:L173
# Action: (AABB Fix) Revert changes in file.
#
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\OgreMain\Components\MeshLodGenerator\src\OgreLodOutsideMarker.cpp:L445
# Action: Manually change suffix (.isNull()) to new prefix (!).
#
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\Tools\XMLConverter\src\OgreXMLMeshSerializer.cpp:L1334
# Action: (AABB Fix) Revert the one line, other changes in file are valid.
# Mezzanine\libincludes\common\ogresrc\ogresvnsrc\Tools\XSIExport\src\OgreXSISkeletonExporter.cpp
# Action: (AABB Fix) Revert changes in file.


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
set(OGREDEPS_BUILD_FREETYPE FALSE CACHE BOOL "Enabling vector text support for Ogre." FORCE)
set(OGREDEPS_BUILD_OIS FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
set(OGREDEPS_BUILD_ZLIB FALSE CACHE BOOL "Enabling enhanced resource reading for Ogre." FORCE)
set(OGREDEPS_BUILD_ZZIPLIB FALSE CACHE BOOL "Enabling enhanced resource reading for Ogre." FORCE)
set(OGREDEPS_INSTALL_DEV FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)

set(FREETYPE_FT2BUILD_INCLUDE_DIR "" CACHE PATH "Forcing empty include path to avoid CMake errors." FORCE)
set(FREETYPE_INCLUDE_DIR "" CACHE PATH "Forcing empty include path to avoid CMake errors." FORCE)

# Ogre Extras
set(OGRE_BUILD_SAMPLES FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_INSTALL_DOCS FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_INSTALL_SAMPLES FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_INSTALL_SAMPLES_SOURCE FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
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
    ZLIB_LIBRARY_DBG
    ZLIB_LIBRARY_REL

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

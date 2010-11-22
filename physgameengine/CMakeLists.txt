###############################################################################
# The build script for the physgame engine
cmake_minimum_required (VERSION 2.8)
project (physgame)
message ("${PROJECT_NAME} - Begin")

###############################################################################
# Initializations

set(CommonBuildFlags " -fPIC -O3 -s ")
message("Compiler Flags used on all dependent Builds: ${CommonBuildFlags}")

set (${PROJECT_NAME}_ROOT_DIR "${${PROJECT_NAME}_SOURCE_DIR}/")
message ("Base Directory for ${PROJECT_NAME}: ${${PROJECT_NAME}_ROOT_DIR}")

set (${PROJECT_NAME}_LIBINCLUDE_DIR "${${PROJECT_NAME}_SOURCE_DIR}/libincludes/")
message ("Base Directory for Library headers: ${${PROJECT_NAME}_LIBINCLUDE_DIR}")

set (${PROJECT_NAME}_LIBBINARY_DIR "${${PROJECT_NAME}_SOURCE_DIR}/data/")
message ("Base Directory for Library binaries and data: ${${PROJECT_NAME}_LIBBINARY_DIR}")

set (${PROJECT_NAME}_SOURCE_DIR "${${PROJECT_NAME}_SOURCE_DIR}/src/")
message ("Looking for source code in: ${${PROJECT_NAME}_SOURCE_DIR}")

set (${PROJECT_NAME}_BINARY_DIR "${${PROJECT_NAME}_BINARY_DIR}/bin/")
set (PROJECT_BINARY_DIR "${${PROJECT_NAME}_BINARY_DIR}")
set (LIBRARY_OUTPUT_PATH "${${PROJECT_NAME}_BINARY_DIR}")
message ("Putting complete binaries in: ${${PROJECT_NAME}_BINARY_DIR}")

#######################################
# A complete listing of our header files
set(${PROJECT_NAME}_headers
	"${${PROJECT_NAME}_SOURCE_DIR}/actorbase.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorbase.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorcontainerbase.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorcontainervector.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorragdoll.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorrigid.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorsoft.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorterrain.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/areaeffect.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/attachable.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/camera.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cameramanager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/colourvalue.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/constraint.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/containerbase.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/crossplatform.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/datatypes.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/enumerations.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventbase.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventcollision.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventmanager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventquit.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventrendertime.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventuserinput.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/exception.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/graphicsmanager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/internaldecompinterface.h.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/internalGorilla.h.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/internalmeshinfo.h.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/internalmotionstate.h.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/light.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/linegroup.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/managerbase.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/metacode.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/node.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/ogredatastreambuf.h.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/particleeffect.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/physgame.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/physicsmanager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/plane.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/quaternion.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/ray.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/resourceinputstream.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/resourcemanager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/scenemanager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/sound.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/soundlistener.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/soundmanager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uibutton.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uicaption.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uilayer.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uimanager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uirectangle.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uiscreen.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uiscrollbar.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uitextbutton.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uiwidget.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/vector2.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/vector3.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/vector3wactor.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/worldgetset.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/world.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/worldquerytool.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/xmlattribute.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/xmlbase.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/xmlcomment.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/xmldeclaration.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/xmldocument.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/xmlelement.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/xmlnode.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/xmlstylesheetreference.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/xmltext.h"
)

#######################################
# A complete listing of our source files
set(${PROJECT_NAME}_sources
	"${${PROJECT_NAME}_SOURCE_DIR}/actorbase.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorcontainerbase.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorcontainervector.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorragdoll.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorrigid.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorsoft.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorterrain.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/areaeffect.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/attachable.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/camera.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cameramanager.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/colourvalue.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/constraint.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/containerbase.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/crossplatform.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/datatypes.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventbase.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventcollision.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventmanager.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventquit.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventrendertime.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventuserinput.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/exception.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/graphicsmanager.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/internaldecompinterface.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/internalGorilla.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/internalmeshinfo.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/internalmotionstate.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/light.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/linegroup.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/managerbase.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/metacode.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/node.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/ogredatastreambuf.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/particleeffect.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/physicsmanager.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/plane.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/quaternion.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/ray.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/resourceinputstream.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/resourcemanager.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/scenemanager.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/sound.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/soundlistener.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/soundmanager.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uibutton.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uicaption.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uilayer.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uimanager.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uirectangle.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uiscreen.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uiscrollbar.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uitextbutton.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uiwidget.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/vector2.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/vector3.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/vector3wactor.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/world.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/worldgetset.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/worldquerytool.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/xmlattribute.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/xmlbase.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/xmlcomment.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/xmldeclaration.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/xmldocument.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/xmlelement.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/xmlnode.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/xmlstylesheetreference.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/xmltext.cpp"
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
		#COMMAND ${CMAKE_COMMAND} -E create_symlink "${${PROJECT_NAME}_BINARY_DIR}${Linkname}" "${${PROJECT_NAME}_BINARY_DIR}${Target}"
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
# System Specific Build Items

#######################################
# Windows Build Specific Items
if(${WIN32})
	message ("${PROJECT_NAME} - Begin Windows32/64 Specific Build Process")
	set(BuildPlatform windows)
	add_definitions( -DWINDOWS )
	message ("${PROJECT_NAME} - End Windows32/64 Specific Build Process")
endif(${WIN32})

#######################################
# Linux Build Specific Items
if(${UNIX})
	message ("${PROJECT_NAME} - Begin Unix/Linux Specific Build Process")
	set(BuildPlatform linux)

	include(FindSDL)
	message("SDL Headers and Library Found - ${SDL_FOUND}")

	include_directories(
		"${SDL_INCLUDE_DIR}"
		"${${PROJECT_NAME}_LIBINCLUDE_DIR}/linux/ogresrc/ogre/OgreMain/include"
		"${${PROJECT_NAME}_LIBINCLUDE_DIR}/linux/ogresrc/ogre/include"
	)

	link_directories(
		"${SDL_LIBRARY}"
	)

	add_definitions( -DOGRE_GUI_GLX -DLINUX )

	message ("${PROJECT_NAME} - End Unix/Linux Specific Build Process")
endif(${UNIX})

#######################################
# Mac OS X and iPhone Build pecific Items
if(${APPLE})
	message ("${PROJECT_NAME} - Begin Apple Specific Build Process")
	set(BuildPlatform macosx)
	add_definitions( -DMACOSX )
	message ("${PROJECT_NAME} - End Apple Specific Build Process")
endif(${APPLE})

###############################################################################
#Common Library Configuration items
include_directories( ${Platform_LIBINCLUDES} ${${PROJECT_NAME}_SOURCE_DIR}
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/bulletsrc/bulletsvnsrc/src"
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/bulletsrc/bulletsvnsrc/src/LinearMath"
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/bulletsrc/bulletsvnsrc/Extras/ConvexDecomposition"
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/bulletsrc/bulletsvnsrc/Extras/Serialize/BulletWorldImporter"
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/tinyxmlppsrc/tinyxmlpp"
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/caudiosrc/cAudio2.1.0/include"
)

link_directories(
	"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/ogre"
	"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/bullet"
	"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/caudio"
	"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/tinyxmlpp"
)

###############################################################################
# Prepare the actual Build
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_headers}  ${${PROJECT_NAME}_sources} )

#######################################
# Prepare Common Dependencies
add_subdirectory (libincludes/common/tinyxmlppsrc)
add_subdirectory (libincludes/common/bulletsrc)
add_subdirectory (libincludes/common/caudiosrc)

add_dependencies( ${PROJECT_NAME}  cAudio  Bullet  ticpp )

#######################################
# Library Link Dependencies
set( Bullet_LibraryNames
	"BulletMultiThreaded" "BulletSoftBody"  "BulletDynamics" "BulletCollision" "BulletFileLoader" "LinearMath" 
	"GIMPACTUtils" "ConvexDecomposition"   "BulletWorldImporter"  
	"MiniCL" "BulletSoftBodySolvers_CPU" #"BulletSoftBodySolvers_OpenCL"
)

target_link_libraries( ${PROJECT_NAME} OgreMain ${Bullet_LibraryNames} ticpp cAudio SDL )

###############################################################################
# Build Time File copying

#######################################
#Copy Common Files at Build Time
CopyFilesDuringBuild( common/Ogre-lgpl-3.0.txt data/common/Ogre-lgpl-3.0.txt )

#######################################
# Windows File Copy Specific Items
if(${WIN32})
	message ("${PROJECT_NAME} - Begin Windows32/64 Specific Build Process")
	message ("${PROJECT_NAME} - End Windows32/64 Specific Build Process")
endif(${WIN32})

#######################################
# Linux File Copy Specific Items
if(${UNIX})
	message ("${PROJECT_NAME} - Begin Unix/Linux Specific File Copy Process")

	#Copy System Dependant Libraries On initial build
	#TODO: ogre bullet caudio

	#Copy System Dependant files during build time
	CopyFilesDuringBuild( linux/plugins.cfg data/linux/plugins.cfg )
	CopyFilesDuringBuild( linux/settings.cfg data/linux/settings.cfg )
	CopyFilesDuringBuild( linux/ogre/RenderSystem_GL.so data/linux/ogre/RenderSystem_GL.so )
	CopyFilesDuringBuild( linux/ogre/libOgreMain.so.1.8.0 libOgreMain.so.1.8.0 )
	CreateSymlinkDuringBuild( libOgreMain.so.1.8.0 libOgreMain.so )
#	CopyFilesDuringBuild( linux/caudio/libcAudio.2.1.0.so libcAudio.2.1.0.so )
#	CreateSymlinkDuringBuild( libcAudio.2.1.0.so libcAudio.so  )
	
	message ("${PROJECT_NAME} - End Unix/Linux Specific File Copy Process")
endif(${UNIX})

#######################################
# Mac File Copy pecific Items
if(${APPLE})
	message ("${PROJECT_NAME} - Begin Apple Specific File Copy Process")
	message ("${PROJECT_NAME} - End Apple Specific File Copy Process")
endif(${APPLE})

###############################################################################
# The End

message ( "${PROJECT_NAME} - End" )


###############################################################################
# The build script for the physgame engine
cmake_minimum_required (VERSION 2.8)
project (physgame)
message ("${PROJECT_NAME} - Begin")

###############################################################################
# Initializations

add_definitions( ${CommonBuildFlags} )
message("Compiler Flags used on all Builds: ${CommonBuildFlags}")

set (${PROJECT_NAME}_ROOT_DIR "${${PROJECT_NAME}_SOURCE_DIR}/")
message ("Base Directory for ${PROJECT_NAME}: ${${PROJECT_NAME}_ROOT_DIR}")

set (${PROJECT_NAME}_LIBINCLUDE_DIR "${${PROJECT_NAME}_SOURCE_DIR}/libincludes/")
message ("Base Directory for Library headers: ${${PROJECT_NAME}_LIBINCLUDE_DIR}")

set (${PROJECT_NAME}_LIBBINARY_DIR "${${PROJECT_NAME}_SOURCE_DIR}/data/")
message ("Base Directory for Library binaries and data: ${${PROJECT_NAME}_LIBBINARY_DIR}")

set (${PROJECT_NAME}_SOURCE_DIR "${${PROJECT_NAME}_SOURCE_DIR}/src/")
message ("Looking for source code in: ${${PROJECT_NAME}_SOURCE_DIR}")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")

set (${PROJECT_NAME}_BINARY_DIR "${${PROJECT_NAME}_BINARY_DIR}/bin/")
set (PROJECT_BINARY_DIR "${${PROJECT_NAME}_BINARY_DIR}")
set (LIBRARY_OUTPUT_PATH "${${PROJECT_NAME}_BINARY_DIR}")
message ("Putting complete binaries in: ${${PROJECT_NAME}_BINARY_DIR}")

#######################################
# A complete listing of our header files
set(${PROJECT_NAME}_sources
	"${${PROJECT_NAME}_SOURCE_DIR}/actorbase.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorcharacter.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorcontainerbase.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorcontainervector.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorgraphicssettings.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/actormanager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorphysicssettings.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorrigid.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorsoft.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorterrain.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/areaeffect.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/attachable.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/camera.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cameracontroller.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/cameramanager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/collisionshapegenerator.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/colourvalue.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/constraint.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/crossplatform.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/crossplatformexport.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/datatypes.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/enumerations.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventbase.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventcollision.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventgamewindow.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventmanager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventquit.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventrendertime.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventuserinput.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/exception.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/extendedtimer.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/gamewindow.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/graphicsmanager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/graphicssettings.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/inputquerytool.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/internalbulletfile.h.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/internalbulletfilemanager.h.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/internaldecompinterface.h.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/internalGorilla.h.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/internalmeshtools.h.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/internalmotionstate.h.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/internalogredatastreambuf.h.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/light.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/linegroup.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/managerbase.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/mathtool.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/meshgenerator.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/metacode.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/objectreference.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/particleeffect.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/physgame.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/physicsmanager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/plane.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/quaternion.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/ray.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/rayquerytool.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/resourceinputstream.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/resourcemanager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/scenemanager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/simpletimer.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/singleton.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/sound.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/soundlistener.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/soundmanager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/stringtool.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/timer.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/timermanager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uibasicrenderable.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uibutton.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uibuttonlistbox.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uicaption.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uicell.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uicellgrid.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uicheckbox.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uidropdownlist.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uilayer.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uilinelist.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uilistbox.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uimanager.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uimarkuptext.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uimenu.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uimenuwindow.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uipagedcellgrid.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uirectangle.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uirenderablerect.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uiresizinginfo.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uiscreen.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uiscrollbar.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uiscrolledcellgrid.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uispinner.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uitextbutton.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uiwidget.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/uiwindow.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/vector2.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/vector3.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/vector3wactor.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/viewport.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/world.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/worldgetset.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/worldnode.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/worldtrigger.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/xml.h"
	"${${PROJECT_NAME}_SOURCE_DIR}/xmldoc.h"

#######################################
# A complete listing of our source files

	"${${PROJECT_NAME}_SOURCE_DIR}/actorbase.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorcharacter.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorcontainerbase.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorcontainervector.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorgraphicssettings.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/actormanager.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorphysicssettings.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorrigid.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorsoft.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/actorterrain.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/areaeffect.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/attachable.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/camera.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cameracontroller.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/cameramanager.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/collisionshapegenerator.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/colourvalue.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/constraint.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/crossplatform.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/datatypes.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventbase.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventcollision.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventgamewindow.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventmanager.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventquit.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventrendertime.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/eventuserinput.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/exception.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/extendedtimer.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/gamewindow.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/graphicsmanager.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/inputquerytool.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/internalbulletfile.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/internalbulletfilemanager.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/internaldecompinterface.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/internalGorilla.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/internalmeshtools.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/internalmotionstate.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/internalogredatastreambuf.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/light.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/linegroup.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/managerbase.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/mathtool.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/meshgenerator.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/metacode.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/particleeffect.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/physicsmanager.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/plane.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/quaternion.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/ray.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/rayquerytool.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/resourceinputstream.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/resourcemanager.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/scenemanager.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/simpletimer.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/sound.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/soundlistener.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/soundmanager.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/stringtool.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/timermanager.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uibasicrenderable.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uibutton.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uibuttonlistbox.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uicaption.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uicell.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uicellgrid.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uicheckbox.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uidropdownlist.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uilayer.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uilinelist.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uilistbox.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uimanager.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uimarkuptext.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uimenu.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uimenuwindow.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uipagedcellgrid.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uirectangle.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uiresizinginfo.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uiscreen.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uiscrollbar.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uiscrolledcellgrid.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uispinner.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uitextbutton.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uiwidget.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/uiwindow.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/vector2.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/vector3.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/vector3wactor.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/viewport.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/world.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/worldgetset.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/worldnode.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/worldtrigger.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/xml.cpp"
	"${${PROJECT_NAME}_SOURCE_DIR}/xmldoc.cpp"
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
set(SDL SDL)
#######################################
# Windows Build Specific Items
if(${WIN32})
	message ("${PROJECT_NAME} - Begin Windows32/64 Specific Build Process")
	set(BuildPlatform windows)

	#required for cAudio
	add_subdirectory ("${physgame_LIBINCLUDE_DIR}${BuildPlatform}/oggvorbis")

	include_directories( "${${PROJECT_NAME}_LIBINCLUDE_DIR}/${BuildPlatform}/ogre" )

	add_definitions( -DWINDOWS -DEXPORTINGPHYSGAMEDLL )
	message ("${PROJECT_NAME} - End Windows32/64 Specific Build Process")
endif(${WIN32})

#######################################
# Linux Build Specific Items

if(${UNIX})
	if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 	#if(${UNIX} AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin") #does not appear to work as expected.
	else(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		message ("${PROJECT_NAME} - Begin Unix/Linux Specific Build Process")
		set(BuildPlatform linux)

		#Autobuild SDL and ogre could go here
	
		set(OgreSoName libOgreMain.so.1.7.3)
		file( COPY 
				"${${PROJECT_NAME}_LIBINCLUDE_DIR}${BuildPlatform}/ogresrc/ogre/lib/${OgreSoName}"
				"${${PROJECT_NAME}_LIBINCLUDE_DIR}${BuildPlatform}/ogresrc/ogre/lib/libOgreMain.so"
				"${${PROJECT_NAME}_LIBINCLUDE_DIR}${BuildPlatform}/ogresrc/ogre/lib/RenderSystem_GL.so"
				"${${PROJECT_NAME}_LIBINCLUDE_DIR}${BuildPlatform}/ogresrc/ogre/lib/Plugin_ParticleFX.so"
				"${${PROJECT_NAME}_LIBINCLUDE_DIR}${BuildPlatform}/ogresrc/ogre/lib/Plugin_CgProgramManager.so"
			DESTINATION "${${PROJECT_NAME}_LIBBINARY_DIR}${BuildPlatform}/ogre"
		)
		include_directories(
			"${${PROJECT_NAME}_LIBINCLUDE_DIR}/${BuildPlatform}/ogresrc/ogre/OgreMain/include"
			"${${PROJECT_NAME}_LIBINCLUDE_DIR}/${BuildPlatform}/ogresrc/ogre/include"
		)
		add_definitions( -DOGRE_GUI_GLX -DLINUX )

		file( COPY 
				"${${PROJECT_NAME}_LIBINCLUDE_DIR}common/sdlsrc/SDL/build/.libs/libSDL.a"
			DESTINATION "${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/sdl/"
		)
	    	set( SDL "${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/sdl/libSDL.a" )
		message ("${PROJECT_NAME} - End Unix/Linux Specific Build Process")
	endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
endif(${UNIX})

#######################################
# Mac OS X and iPhone Build pecific Items
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message ("${PROJECT_NAME} - Begin Apple Specific Build Process")
	set(BuildPlatform macosx)
	set(CMAKE_OSX_ARCHITECTURES i386 CACHE STRING "do not build for 64-bit" FORCE)
	set(ARCH i386)
	
        
        
        include(FindOpenAL)
	message("OpenAL Headers and Library Found - ${OPENAL_INCLUDE_DIR}")


	include(FindOgg)
	message("Ogg Headers and Library Found - ${OGG_FOUND}")

	include(FindOgre)
	message("Ogre Headers And library Found -	"${OGRE_INCLUDE_DIR}"")

	include(/Library/Frameworks/Ogre.framework)

	find_package(OGRE REQUIRED)
        
	include_directories(
		${OPENAL_INCLUDE_DIR}
		"${OGRE_INCLUDE_DIR}"
		${${PROJECT_NAME}_ROOT_DIR}/libincludes/macosx/ogresrc/boost_1_42
		"${OGG_INCLUDE_DIR}"
	)

	link_directories("${OGRE_LIB_DIR}")
    

	add_definitions( -DMACOSX)
	message ("${PROJECT_NAME} - End Apple Specific Build Process")

endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

###############################################################################
#Common Library Configuration items
include_directories( ${${PROJECT_NAME}_SOURCE_DIR}
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/sdlsrc/SDL/include"
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/bulletsrc/bulletsvnsrc/src"
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/bulletsrc/bulletsvnsrc/src/LinearMath"
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/bulletsrc/bulletsvnsrc/Extras/ConvexDecomposition"
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/bulletsrc/bulletsvnsrc/Extras/Serialize/BulletWorldImporter"
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/caudiosrc/cAudio2.1.0/include"
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/${BuildPlatform}/ogre"
)

link_directories(
	"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/ogre"
	"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/bullet"
	"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/caudio"
	"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/openal"
	"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/oggvorbis"
	"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/sdl"
)

###############################################################################
# Prepare the actual Build
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_sources} )

#######################################
# Prepare Common Dependencies
add_subdirectory (libincludes/common/caudiosrc)
add_subdirectory (libincludes/common/bulletsrc)
#add_subdirectory (${BuildSpecificDirs})

add_dependencies( ${PROJECT_NAME} ${BuildSpecificDependencies} cAudio  Bullet )

#######################################
# Library Link Dependencies
set( Bullet_LibraryNames
	"ConvexDecomposition"
	"GIMPACTUtils"
	"BulletWorldImporter"
	"BulletFileLoader"
	#"BulletSoftBodySolvers_OpenCL"
	"BulletSoftBodySolvers_CPU"
	"MiniCL"
	"BulletMultiThreaded"
	"BulletSoftBody" "BulletDynamics" "BulletCollision"  "LinearMath"
)


if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	target_link_libraries(
	    "${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/sdl"
	    ${PROJECT_NAME}
	    "${OGRE_LIBRARIES}"
	    ${Bullet_LibraryNames}
	    "${OPENAL_LIBRARY}" 
	    ${${PROJECT_NAME}_ROOT_DIR}/data/macosx/sdl/libSDL.dylib
	    ${${PROJECT_NAME}_ROOT_DIR}/data/macosx/caudio/libcAudio.dylib
	)

else(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	target_link_libraries( ${PROJECT_NAME} OgreMain ${Bullet_LibraryNames} cAudio ${SDL} )
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

###############################################################################
# Build Time File copying

#######################################
#Copy Common Files at Build Time
CopyFilesDuringBuild( common/Ogre-lgpl-3.0.txt data/common/Ogre-lgpl-3.0.txt )
CopyFilesDuringBuild( common/PugiXML-MITLicense.txt data/common/PugiXML-MITLicense.txt )
CopyFilesDuringBuild( ${BuildPlatform}/plugins.cfg data/${BuildPlatform}/plugins.cfg )
CopyFilesDuringBuild( ${BuildPlatform}/settings.cfg data/${BuildPlatform}/settings.cfg )

#######################################
# Windows File Copy Specific Items
if(${WIN32})
	message ("${PROJECT_NAME} - Begin Windows32/64 Specific File Copy Process")

	#Copy System Dependant files during build time
	CopyFilesDuringBuild( ${BuildPlatform}/ogre/RenderSystem_GL.dll data/${BuildPlatform}/ogre/RenderSystem_GL.dll )
	CopyFilesDuringBuild( ${BuildPlatform}/ogre/Plugin_ParticleFX.dll data/${BuildPlatform}/ogre/Plugin_ParticleFX.dll )
	CopyFilesDuringBuild( ${BuildPlatform}/ogre/OgreMain.dll OgreMain.dll )
	CopyFilesDuringBuild( ${BuildPlatform}/ogre/libboost_thread-mgw45-mt-1_44.dll libboost_thread-mgw45-mt-1_44.dll )
	CopyFilesDuringBuild( ${BuildPlatform}/sdl/SDL.dll SDL.dll )
	CopyFilesDuringBuild( ${BuildPlatform}/openal/OpenAL32.dll OpenAL32.dll )
	CopyFilesDuringBuild( ${BuildPlatform}/openal/OpenAL32.lib OpenAL32.lib )
	CopyFilesDuringBuild( ${BuildPlatform}/openal/wrap_oal.dll wrap_oal.dll )

	message ("${PROJECT_NAME} - End Windows32/64 Specific File Copy Process")
endif(${WIN32})

#######################################
# Linux File Copy Specific Items
if(${UNIX})
	if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	else(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

		message ("${PROJECT_NAME} - Begin Unix/Linux Specific File Copy Process")

		#Copy System Dependant files during build time
		#CopyFilesDuringBuild( ${BuildPlatform}/sdl/${SDLLibName} ${SDLLibName})
		CopyFilesDuringBuild( ${BuildPlatform}/ogre/RenderSystem_GL.so data/${BuildPlatform}/ogre/RenderSystem_GL.so)
		CopyFilesDuringBuild( ${BuildPlatform}/ogre/Plugin_ParticleFX.so data/${BuildPlatform}/ogre/Plugin_ParticleFX.so)
		CopyFilesDuringBuild( ${BuildPlatform}/ogre/Plugin_CgProgramManager.so data/${BuildPlatform}/ogre/Plugin_CgProgramManager.so)
		CopyFilesDuringBuild( ${BuildPlatform}/ogre/${OgreSoName} ${OgreSoName})
		CreateSymlinkDuringBuild( ${OgreSoName} libOgreMain.so )

		message ("${PROJECT_NAME} - End Unix/Linux Specific File Copy Process")
	endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
endif(${UNIX})

#######################################
# Mac File Copy Specific Items
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	message ("${PROJECT_NAME} - Begin Apple Specific File Copy Process")
	
	CopyFilesDuringBuild( ${BuildPlatform}/ogre/RenderSystem_GL.dylib data/${BuildPlatform}/ogre/RenderSystem_GL.dylib )
#	CopyFilesDuringBuild( ${BuildPlatform}/ogre/Ogre.framework Ogre.framework )
	
	message ("${PROJECT_NAME} - End Apple Specific File Copy Process")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

###############################################################################
# The End

message ( "${PROJECT_NAME} - End" )


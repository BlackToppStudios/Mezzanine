#The build script for the physgame engine
cmake_minimum_required (VERSION 2.8)
project (physgame)

message ("${PROJECT_NAME} - Begin")

set (${PROJECT_NAME}_LIBINCLUDE_DIR "${${PROJECT_NAME}_SOURCE_DIR}/libincludes/")
message ("Base Directory for Library headers: ${${PROJECT_NAME}_LIBINCLUDE_DIR}")

set (${PROJECT_NAME}_LIBBINARY_DIR "${${PROJECT_NAME}_SOURCE_DIR}/data/")
message ("Base Directory for Library binaries: ${${PROJECT_NAME}_LIBBINARY_DIR}")

set (${PROJECT_NAME}_SOURCE_DIR "${${PROJECT_NAME}_SOURCE_DIR}/src/")
message ("Looking for source code in: ${${PROJECT_NAME}_SOURCE_DIR}")

set (${PROJECT_NAME}_BINARY_DIR "${${PROJECT_NAME}_BINARY_DIR}/bin/")
message ("Putting complete binaries in: ${${PROJECT_NAME}_BINARY_DIR}")

# Set header and source files
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

#Where are the libraries and their headers
if(${WIN32})
	message ("Using Windows Build Process")
endif(${WIN32})

if(${UNIX})	
	message ("Using Unix/Linux Build Process")

	include(FindSDL)
	message("SDL Headers and Library Found - ${SDL_FOUND}")

	include_directories(
		"${SDL_INCLUDE_DIR}"
		"${${PROJECT_NAME}_LIBINCLUDE_DIR}/linux/ogresrc/OgreMain/include"
		"${${PROJECT_NAME}_LIBINCLUDE_DIR}/linux/ogresrc/include"
	)

	link_directories(
		"${SDL_LIBRARY}"
		"${${PROJECT_NAME}_LIBBINARY_DIR}/linux/ogre"
		"${${PROJECT_NAME}_LIBBINARY_DIR}/linux/bullet"
		"${${PROJECT_NAME}_LIBBINARY_DIR}/linux/caudio"
		"${${PROJECT_NAME}_LIBBINARY_DIR}/linux/tinyxmlpp"
	)

	add_definitions( -DOGRE_GUI_GLX -DLINUX )
endif(${UNIX})

if(${APPLE})
	message ("Using Windows Build Process")
endif(${APPLE})

#Common Configuration items
include_directories( ${Platform_LIBINCLUDES} ${${PROJECT_NAME}_SOURCE_DIR}
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/bulletsrc/bullet-2.76/src"
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/bulletsrc/bullet-2.76/src/LinearMath"
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/bulletsrc/convexdecomp"
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/tinyxmlppsrc/tinyxmlpp"
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/caudiosrc/include"
	"${${PROJECT_NAME}_LIBINCLUDE_DIR}/common/bulletsrc/bulletserializer"
)

# Prepare the actual Build
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_headers}  ${${PROJECT_NAME}_sources} )

target_link_libraries( ${PROJECT_NAME} OgreMain Bullet ticpp cAudio SDL )

#just testing a command.
#configure_file(test copy.txt COPYFILE)

message ( "${PROJECT_NAME} - End" )


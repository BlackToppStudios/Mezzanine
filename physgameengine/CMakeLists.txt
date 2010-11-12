# The name of our project is "Physgame". CMakeLists files in this project can
# refer to the root source directory of the project as ${PhysgameEngine_SOURCE_DIR} and
# to the root binary directory of the project as ${PhysgameEngine_BINARY_DIR}.
cmake_minimum_required (VERSION 2.6)
project (PhysgameEngine)

message ("${PROJECT_NAME} - Begin")

set (PhysgameEngine_LIBINCLUDE_DIR "${PhysgameEngine_SOURCE_DIR}/libincludes/")
message ("Base Directory for Library headers: ${PhysgameEngine_LIBINCLUDE_DIR}")

set (PhysgameEngine_LIBBINARY_DIR "${PhysgameEngine_SOURCE_DIR}/data/")
message ("Base Directory for Library binaries: ${PhysgameEngine_LIBBINARY_DIR}")

set (PhysgameEngine_SOURCE_DIR "${PhysgameEngine_SOURCE_DIR}/src/")
message ("Looking for source code in: ${PhysgameEngine_SOURCE_DIR}")

set (PhysgameEngine_BINARY_DIR "${PhysgameEngine_BINARY_DIR}/bin/")
message ("Putting complete binaries in: ${PhysgameEngine_BINARY_DIR}")

# Set header and source files
set(${PROJECT_NAME}_headers
	"${PhysgameEngine_SOURCE_DIR}/actorbase.h"
	"${PhysgameEngine_SOURCE_DIR}/actorbase.h"
	"${PhysgameEngine_SOURCE_DIR}/actorcontainerbase.h"
	"${PhysgameEngine_SOURCE_DIR}/actorcontainervector.h"
	"${PhysgameEngine_SOURCE_DIR}/actorragdoll.h"
	"${PhysgameEngine_SOURCE_DIR}/actorrigid.h"
	"${PhysgameEngine_SOURCE_DIR}/actorsoft.h"
	"${PhysgameEngine_SOURCE_DIR}/actorterrain.h"
	"${PhysgameEngine_SOURCE_DIR}/areaeffect.h"
	"${PhysgameEngine_SOURCE_DIR}/attachable.h"
	"${PhysgameEngine_SOURCE_DIR}/camera.h"
	"${PhysgameEngine_SOURCE_DIR}/cameramanager.h"
	"${PhysgameEngine_SOURCE_DIR}/colourvalue.h"
	"${PhysgameEngine_SOURCE_DIR}/constraint.h"
	"${PhysgameEngine_SOURCE_DIR}/containerbase.h"
	"${PhysgameEngine_SOURCE_DIR}/crossplatform.h"
	"${PhysgameEngine_SOURCE_DIR}/datatypes.h"
	"${PhysgameEngine_SOURCE_DIR}/eventbase.h"
	"${PhysgameEngine_SOURCE_DIR}/eventcollision.h"
	"${PhysgameEngine_SOURCE_DIR}/eventmanager.h"
	"${PhysgameEngine_SOURCE_DIR}/eventquit.h"
	"${PhysgameEngine_SOURCE_DIR}/eventrendertime.h"
	"${PhysgameEngine_SOURCE_DIR}/eventuserinput.h"
	"${PhysgameEngine_SOURCE_DIR}/exception.h"
	"${PhysgameEngine_SOURCE_DIR}/graphicsmanager.h"
	"${PhysgameEngine_SOURCE_DIR}/internaldecompinterface.h.cpp"
	"${PhysgameEngine_SOURCE_DIR}/internalGorilla.h.cpp"
	"${PhysgameEngine_SOURCE_DIR}/internalmeshinfo.h.cpp"
	"${PhysgameEngine_SOURCE_DIR}/internalmotionstate.h.cpp"
	"${PhysgameEngine_SOURCE_DIR}/light.h"
	"${PhysgameEngine_SOURCE_DIR}/linegroup.h"
	"${PhysgameEngine_SOURCE_DIR}/managerbase.h"
	"${PhysgameEngine_SOURCE_DIR}/metacode.h"
	"${PhysgameEngine_SOURCE_DIR}/node.h"
	"${PhysgameEngine_SOURCE_DIR}/ogredatastreambuf.h.cpp"
	"${PhysgameEngine_SOURCE_DIR}/particleeffect.h"
	"${PhysgameEngine_SOURCE_DIR}/physgame.h"
	"${PhysgameEngine_SOURCE_DIR}/physicsmanager.h"
	"${PhysgameEngine_SOURCE_DIR}/plane.h"
	"${PhysgameEngine_SOURCE_DIR}/quaternion.h"
	"${PhysgameEngine_SOURCE_DIR}/ray.h"
	"${PhysgameEngine_SOURCE_DIR}/resourceinputstream.h"
	"${PhysgameEngine_SOURCE_DIR}/resourcemanager.h"
	"${PhysgameEngine_SOURCE_DIR}/scenemanager.h"
	"${PhysgameEngine_SOURCE_DIR}/sound.h"
	"${PhysgameEngine_SOURCE_DIR}/soundlistener.h"
	"${PhysgameEngine_SOURCE_DIR}/soundmanager.h"
	"${PhysgameEngine_SOURCE_DIR}/uibutton.h"
	"${PhysgameEngine_SOURCE_DIR}/uilayer.h"
	"${PhysgameEngine_SOURCE_DIR}/uimanager.h"
	"${PhysgameEngine_SOURCE_DIR}/uirectangle.h"
	"${PhysgameEngine_SOURCE_DIR}/uiscreen.h"
	"${PhysgameEngine_SOURCE_DIR}/uiscrollbar.h"
	"${PhysgameEngine_SOURCE_DIR}/uiwidget.h"
	"${PhysgameEngine_SOURCE_DIR}/vector2.h"
	"${PhysgameEngine_SOURCE_DIR}/vector3.h"
	"${PhysgameEngine_SOURCE_DIR}/vector3wactor.h"
	"${PhysgameEngine_SOURCE_DIR}/worldgetset.h"
	"${PhysgameEngine_SOURCE_DIR}/world.h"
	"${PhysgameEngine_SOURCE_DIR}/worldquerytool.h"
	"${PhysgameEngine_SOURCE_DIR}/xmlattribute.h"
	"${PhysgameEngine_SOURCE_DIR}/xmlbase.h"
	"${PhysgameEngine_SOURCE_DIR}/xmlcomment.h"
	"${PhysgameEngine_SOURCE_DIR}/xmldeclaration.h"
	"${PhysgameEngine_SOURCE_DIR}/xmldocument.h"
	"${PhysgameEngine_SOURCE_DIR}/xmlelement.h"
	"${PhysgameEngine_SOURCE_DIR}/xmlnode.h"
	"${PhysgameEngine_SOURCE_DIR}/xmlstylesheetreference.h"
	"${PhysgameEngine_SOURCE_DIR}/xmltext.h"
)

set(${PROJECT_NAME}_sources
	"${PhysgameEngine_SOURCE_DIR}/actorbase.cpp"
	"${PhysgameEngine_SOURCE_DIR}/actorcontainerbase.cpp"
	"${PhysgameEngine_SOURCE_DIR}/actorcontainervector.cpp"
	"${PhysgameEngine_SOURCE_DIR}/actorragdoll.cpp"
	"${PhysgameEngine_SOURCE_DIR}/actorrigid.cpp"
	"${PhysgameEngine_SOURCE_DIR}/actorsoft.cpp"
	"${PhysgameEngine_SOURCE_DIR}/actorterrain.cpp"
	"${PhysgameEngine_SOURCE_DIR}/areaeffect.cpp"
	"${PhysgameEngine_SOURCE_DIR}/attachable.cpp"
	"${PhysgameEngine_SOURCE_DIR}/camera.cpp"
	"${PhysgameEngine_SOURCE_DIR}/cameramanager.cpp"
	"${PhysgameEngine_SOURCE_DIR}/colourvalue.cpp"
	"${PhysgameEngine_SOURCE_DIR}/constraint.cpp"
	"${PhysgameEngine_SOURCE_DIR}/containerbase.cpp"
	"${PhysgameEngine_SOURCE_DIR}/crossplatform.cpp"
	"${PhysgameEngine_SOURCE_DIR}/datatypes.cpp"
	"${PhysgameEngine_SOURCE_DIR}/eventbase.cpp"
	"${PhysgameEngine_SOURCE_DIR}/eventcollision.cpp"
	"${PhysgameEngine_SOURCE_DIR}/eventmanager.cpp"
	"${PhysgameEngine_SOURCE_DIR}/eventquit.cpp"
	"${PhysgameEngine_SOURCE_DIR}/eventrendertime.cpp"
	"${PhysgameEngine_SOURCE_DIR}/eventuserinput.cpp"
	"${PhysgameEngine_SOURCE_DIR}/exception.cpp"
	"${PhysgameEngine_SOURCE_DIR}/graphicsmanager.cpp"
	"${PhysgameEngine_SOURCE_DIR}/internaldecompinterface.cpp"
	"${PhysgameEngine_SOURCE_DIR}/internalGorilla.cpp"
	"${PhysgameEngine_SOURCE_DIR}/internalmeshinfo.cpp"
	"${PhysgameEngine_SOURCE_DIR}/internalmotionstate.cpp"
	"${PhysgameEngine_SOURCE_DIR}/light.cpp"
	"${PhysgameEngine_SOURCE_DIR}/linegroup.cpp"
	"${PhysgameEngine_SOURCE_DIR}/managerbase.cpp"
	"${PhysgameEngine_SOURCE_DIR}/metacode.cpp"
	"${PhysgameEngine_SOURCE_DIR}/node.cpp"
	"${PhysgameEngine_SOURCE_DIR}/ogredatastreambuf.cpp"
	"${PhysgameEngine_SOURCE_DIR}/particleeffect.cpp"
	"${PhysgameEngine_SOURCE_DIR}/physicsmanager.cpp"
	"${PhysgameEngine_SOURCE_DIR}/plane.cpp"
	"${PhysgameEngine_SOURCE_DIR}/quaternion.cpp"
	"${PhysgameEngine_SOURCE_DIR}/ray.cpp"
	"${PhysgameEngine_SOURCE_DIR}/resourceinputstream.cpp"
	"${PhysgameEngine_SOURCE_DIR}/resourcemanager.cpp"
	"${PhysgameEngine_SOURCE_DIR}/scenemanager.cpp"
	"${PhysgameEngine_SOURCE_DIR}/sound.cpp"
	"${PhysgameEngine_SOURCE_DIR}/soundlistener.cpp"
	"${PhysgameEngine_SOURCE_DIR}/soundmanager.cpp"
	"${PhysgameEngine_SOURCE_DIR}/uibutton.cpp"
	"${PhysgameEngine_SOURCE_DIR}/uilayer.cpp"
	"${PhysgameEngine_SOURCE_DIR}/uimanager.cpp"
	"${PhysgameEngine_SOURCE_DIR}/uirectangle.cpp"
	"${PhysgameEngine_SOURCE_DIR}/uiscreen.cpp"
	"${PhysgameEngine_SOURCE_DIR}/uiscrollbar.cpp"
	"${PhysgameEngine_SOURCE_DIR}/uiwidget.cpp"
	"${PhysgameEngine_SOURCE_DIR}/vector2.cpp"
	"${PhysgameEngine_SOURCE_DIR}/vector3.cpp"
	"${PhysgameEngine_SOURCE_DIR}/vector3wactor.cpp"
	"${PhysgameEngine_SOURCE_DIR}/world.cpp"
	"${PhysgameEngine_SOURCE_DIR}/worldgetset.cpp"
	"${PhysgameEngine_SOURCE_DIR}/worldquerytool.cpp"
	"${PhysgameEngine_SOURCE_DIR}/xmlattribute.cpp"
	"${PhysgameEngine_SOURCE_DIR}/xmlbase.cpp"
	"${PhysgameEngine_SOURCE_DIR}/xmlcomment.cpp"
	"${PhysgameEngine_SOURCE_DIR}/xmldeclaration.cpp"
	"${PhysgameEngine_SOURCE_DIR}/xmldocument.cpp"
	"${PhysgameEngine_SOURCE_DIR}/xmlelement.cpp"
	"${PhysgameEngine_SOURCE_DIR}/xmlnode.cpp"
	"${PhysgameEngine_SOURCE_DIR}/xmlstylesheetreference.cpp"
	"${PhysgameEngine_SOURCE_DIR}/xmltext.cpp"
)

#Where are the libraries and their headers
if(${WIN32})
	message ("Using Windows Build Process")
endif(${WIN32})

if(${UNIX})	
	message ("Using Unix/Linux Build Process")

	INCLUDE(FindSDL)
	Message("SDL Headers and Library Found - ${SDL_FOUND}")

	include_directories(
		"${SDL_INCLUDE_DIR}"
		"${PhysgameEngine_LIBINCLUDE_DIR}/linux/ogresrc/OgreMain/include"
		"${PhysgameEngine_LIBINCLUDE_DIR}/linux/ogresrc/include"
	)

	link_directories(
		"${SDL_LIBRARY}"
		"${PhysgameEngine_LIBBINARY_DIR}/linux/ogre"
		"${PhysgameEngine_LIBBINARY_DIR}/linux/bullet"
		"${PhysgameEngine_LIBBINARY_DIR}/linux/caudio"
		"${PhysgameEngine_LIBBINARY_DIR}/linux/tinyxmlpp"
	)
endif(${UNIX})

if(${APPLE})
	message ("Using Windows Build Process")
endif(${APPLE})

#Common 
link_directories(

)

include_directories( ${Platform_LIBINCLUDES} ${PhysgameEngine_SOURCE_DIR}
	"${PhysgameEngine_LIBINCLUDE_DIR}/common/bulletsrc/bullet-2.76/src"
	"${PhysgameEngine_LIBINCLUDE_DIR}/common/bulletsrc/bullet-2.76/src/LinearMath"
	"${PhysgameEngine_LIBINCLUDE_DIR}/common/bulletsrc/convexdecomp"
	"${PhysgameEngine_LIBINCLUDE_DIR}/common/tinyxmlppsrc/tinyxmlpp"
	"${PhysgameEngine_LIBINCLUDE_DIR}/common/caudiosrc/include"
	"${PhysgameEngine_LIBINCLUDE_DIR}/common/bulletsrc/bulletserializer"
)

#Where are the header files at
#include_directories()

#
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_headers}  ${${PROJECT_NAME}_sources} )


#just testing a command.
#configure_file(test copy.txt COPYFILE)

message ( "${PROJECT_NAME} - End" )


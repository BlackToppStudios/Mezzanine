# © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
# The build script for the Mezzanine engine
cmake_minimum_required (VERSION 2.8)
project (Mezzanine)
message ("${PROJECT_NAME} - Begin")

###############################################################################
# Initializations

#add_definitions( ${CommonBuildFlags} ) #-std=c++11 )
#target_compile_options( ${PROJECT_NAME} PRIVATE ${CommonBuildFlags} )
message("Compiler Flags used on all Builds: ${CommonBuildFlags}")

set (${PROJECT_NAME}_ROOT_DIR "${${PROJECT_NAME}_SOURCE_DIR}/")
set (${PROJECT_NAME}_SOURCE_DIR "${${PROJECT_NAME}_SOURCE_DIR}/")
message ( STATUS "Base Directory for ${PROJECT_NAME}: ${${PROJECT_NAME}_ROOT_DIR}")

set (${PROJECT_NAME}_LIBINCLUDE_DIR "${${PROJECT_NAME}_SOURCE_DIR}libincludes/")
message ( STATUS "Base Directory for Library headers: ${${PROJECT_NAME}_LIBINCLUDE_DIR}")

set (${PROJECT_NAME}_LIBBINARY_DIR "${${PROJECT_NAME}_SOURCE_DIR}data/")
message ( STATUS "Base Directory for Library binaries and data: ${${PROJECT_NAME}_LIBBINARY_DIR}")

set (${PROJECT_NAME}_SRC_DIR "${${PROJECT_NAME}_SOURCE_DIR}src/")
message ( STATUS "Looking for source code in: ${${PROJECT_NAME}_SRC_DIR}")

set (${PROJECT_NAME}_OUTPUT_DIR "${${PROJECT_NAME}_BINARY_DIR}/")
message ( STATUS "Putting all the output in: ${${PROJECT_NAME}_OUTPUT_DIR}")

set (${PROJECT_NAME}_BINARY_DIR "${${PROJECT_NAME}_OUTPUT_DIR}bin/")
set (PROJECT_BINARY_DIR "${${PROJECT_NAME}_BINARY_DIR}")
set (LIBRARY_OUTPUT_PATH "${${PROJECT_NAME}_BINARY_DIR}")
message ( STATUS "Putting complete binaries in: ${${PROJECT_NAME}_BINARY_DIR}")

#######################################
# A complete listing of our header files
set(${PROJECT_NAME}_sources
        # Root Namespace(Mezzanine) Items
        "${${PROJECT_NAME}_SRC_DIR}actor.h"
        "${${PROJECT_NAME}_SRC_DIR}actormanager.h"
        "${${PROJECT_NAME}_SRC_DIR}areaeffect.h"
        "${${PROJECT_NAME}_SRC_DIR}areaeffectmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}assetid.h"
        "${${PROJECT_NAME}_SRC_DIR}attachable.h"
        "${${PROJECT_NAME}_SRC_DIR}axisalignedbox.h"
        "${${PROJECT_NAME}_SRC_DIR}binarybuffer.h"
        "${${PROJECT_NAME}_SRC_DIR}cameracontroller.h"
        "${${PROJECT_NAME}_SRC_DIR}circle.h"
        "${${PROJECT_NAME}_SRC_DIR}colourvalue.h"
        "${${PROJECT_NAME}_SRC_DIR}countedptr.h"
        "${${PROJECT_NAME}_SRC_DIR}crossplatform.h"
        "${${PROJECT_NAME}_SRC_DIR}crossplatformexport.h"
        "${${PROJECT_NAME}_SRC_DIR}cubicspline.h"
        "${${PROJECT_NAME}_SRC_DIR}datastream.h"
        "${${PROJECT_NAME}_SRC_DIR}datatypes.h"
        "${${PROJECT_NAME}_SRC_DIR}debris.h"
        "${${PROJECT_NAME}_SRC_DIR}debrismanager.h"
        "${${PROJECT_NAME}_SRC_DIR}dox/best_practices_doxygen.h"
        "${${PROJECT_NAME}_SRC_DIR}dox/building_the_engine.h"
        "${${PROJECT_NAME}_SRC_DIR}dox/contact_us.h"
        "${${PROJECT_NAME}_SRC_DIR}dox/cxx_training.h"
        "${${PROJECT_NAME}_SRC_DIR}dox/dev_tools_linux.h"
        "${${PROJECT_NAME}_SRC_DIR}dox/dev_tools_macosx.h"
        "${${PROJECT_NAME}_SRC_DIR}dox/dev_tools_windows.h"
        "${${PROJECT_NAME}_SRC_DIR}dox/installing_engine_deps_linux.h"
        "${${PROJECT_NAME}_SRC_DIR}dox/installing_engine_deps_macosx.h"
        "${${PROJECT_NAME}_SRC_DIR}dox/installing_engine_deps_windows.h"
        "${${PROJECT_NAME}_SRC_DIR}dox/path_on_windows.h"
        "${${PROJECT_NAME}_SRC_DIR}dox/jagati_package.h"
        "${${PROJECT_NAME}_SRC_DIR}dox/team_and_credits.h"
        "${${PROJECT_NAME}_SRC_DIR}dox/tech_doc_index.h"
        "${${PROJECT_NAME}_SRC_DIR}dox/mainpage.h"
        "${${PROJECT_NAME}_SRC_DIR}dox/mezzanine_engine.h"
        "${${PROJECT_NAME}_SRC_DIR}dox/running_doxygen.h"
        "${${PROJECT_NAME}_SRC_DIR}entresol.h"
        "${${PROJECT_NAME}_SRC_DIR}entresolmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}entresolmanagerfactory.h"
        "${${PROJECT_NAME}_SRC_DIR}enumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}event.h"
        "${${PROJECT_NAME}_SRC_DIR}eventarguments.h"
        "${${PROJECT_NAME}_SRC_DIR}eventbase.h"
        "${${PROJECT_NAME}_SRC_DIR}eventgamewindow.h"
        "${${PROJECT_NAME}_SRC_DIR}eventmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}eventpublisher.h"
        "${${PROJECT_NAME}_SRC_DIR}eventquit.h"
        "${${PROJECT_NAME}_SRC_DIR}eventsubscriber.h"
        "${${PROJECT_NAME}_SRC_DIR}eventsubscriberslot.h"
        "${${PROJECT_NAME}_SRC_DIR}eventuserinput.h"
        "${${PROJECT_NAME}_SRC_DIR}exception.h"
        "${${PROJECT_NAME}_SRC_DIR}fieldofforce.h"
        "${${PROJECT_NAME}_SRC_DIR}filestream.h"
        "${${PROJECT_NAME}_SRC_DIR}gravityfield.h"
        "${${PROJECT_NAME}_SRC_DIR}gravitywell.h"
        "${${PROJECT_NAME}_SRC_DIR}hashedstring.h"
        "${${PROJECT_NAME}_SRC_DIR}interpolator.h"
        "${${PROJECT_NAME}_SRC_DIR}interval.h"
        "${${PROJECT_NAME}_SRC_DIR}linegroup.h"
        "${${PROJECT_NAME}_SRC_DIR}linesegment.h"
        "${${PROJECT_NAME}_SRC_DIR}managedptr.h"
        "${${PROJECT_NAME}_SRC_DIR}managerbase.h"
        "${${PROJECT_NAME}_SRC_DIR}managerfactory.h"
        "${${PROJECT_NAME}_SRC_DIR}matrix3x3.h"
        "${${PROJECT_NAME}_SRC_DIR}matrix4x4.h"
        "${${PROJECT_NAME}_SRC_DIR}memorystream.h"
        "${${PROJECT_NAME}_SRC_DIR}meshterrain.h"
        "${${PROJECT_NAME}_SRC_DIR}mezzanine.h"
        "${${PROJECT_NAME}_SRC_DIR}mousepicker.h"
        "${${PROJECT_NAME}_SRC_DIR}mousepickdragger.h"
        "${${PROJECT_NAME}_SRC_DIR}multibodyvehicle.h"
        "${${PROJECT_NAME}_SRC_DIR}objectsettings.h"
        "${${PROJECT_NAME}_SRC_DIR}plane.h"
        "${${PROJECT_NAME}_SRC_DIR}player.h"
        "${${PROJECT_NAME}_SRC_DIR}playeraction.h"
        "${${PROJECT_NAME}_SRC_DIR}playercontrol.h"
        "${${PROJECT_NAME}_SRC_DIR}playermanager.h"
        "${${PROJECT_NAME}_SRC_DIR}playerprofile.h"
        "${${PROJECT_NAME}_SRC_DIR}playerview.h"
        "${${PROJECT_NAME}_SRC_DIR}pstdint.h"
        "${${PROJECT_NAME}_SRC_DIR}quaternion.h"
        "${${PROJECT_NAME}_SRC_DIR}ray.h"
        "${${PROJECT_NAME}_SRC_DIR}raycastvehicle.h"
        "${${PROJECT_NAME}_SRC_DIR}rayquerytool.h"
        "${${PROJECT_NAME}_SRC_DIR}rayquery.h"
        "${${PROJECT_NAME}_SRC_DIR}rectbase.h"
        "${${PROJECT_NAME}_SRC_DIR}rigiddebris.h"
        "${${PROJECT_NAME}_SRC_DIR}simplevehicle.h"
        "${${PROJECT_NAME}_SRC_DIR}singleton.h"
        "${${PROJECT_NAME}_SRC_DIR}serialization.h"
        "${${PROJECT_NAME}_SRC_DIR}softdebris.h"
        "${${PROJECT_NAME}_SRC_DIR}sortedvector.h"
        "${${PROJECT_NAME}_SRC_DIR}sphere.h"
        "${${PROJECT_NAME}_SRC_DIR}statemachine.h"
        "${${PROJECT_NAME}_SRC_DIR}statetransition.h"
        "${${PROJECT_NAME}_SRC_DIR}statetransitionaction.h"
        "${${PROJECT_NAME}_SRC_DIR}streamlogging.h"
        "${${PROJECT_NAME}_SRC_DIR}stringtool.h"
        "${${PROJECT_NAME}_SRC_DIR}swig.h"
        "${${PROJECT_NAME}_SRC_DIR}terrain.h"
        "${${PROJECT_NAME}_SRC_DIR}terrainmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}timer.h"
        "${${PROJECT_NAME}_SRC_DIR}track.h"
        "${${PROJECT_NAME}_SRC_DIR}trackiterator.h"
        "${${PROJECT_NAME}_SRC_DIR}transform.h"
        "${${PROJECT_NAME}_SRC_DIR}transformableobject.h"
        "${${PROJECT_NAME}_SRC_DIR}triangle.h"
        "${${PROJECT_NAME}_SRC_DIR}trie.h"
        "${${PROJECT_NAME}_SRC_DIR}uidgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}unicode.h"
        "${${PROJECT_NAME}_SRC_DIR}vector2.h"
        "${${PROJECT_NAME}_SRC_DIR}vector3.h"
        "${${PROJECT_NAME}_SRC_DIR}vehicle.h"
        "${${PROJECT_NAME}_SRC_DIR}vehiclemanager.h"
        "${${PROJECT_NAME}_SRC_DIR}version.h"
        "${${PROJECT_NAME}_SRC_DIR}world.h"
        "${${PROJECT_NAME}_SRC_DIR}worldmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}worldmanagerfactory.h"
        "${${PROJECT_NAME}_SRC_DIR}worldobject.h"
        "${${PROJECT_NAME}_SRC_DIR}worldobjectmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}worldproxy.h"
        "${${PROJECT_NAME}_SRC_DIR}worldproxymanager.h"
        "${${PROJECT_NAME}_SRC_DIR}worldtrigger.h"
        # Audio Items
        "${${PROJECT_NAME}_SRC_DIR}Audio/acousticproxy.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/audio.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/audioenumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/audiomanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/decoder.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/decoderfactory.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/effect.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/effectparameters.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/effectshandler.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/filter.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/listener.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/musicplayer.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/playlist.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/rawdecoder.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/rawdecoderfactory.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/recorder.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/sound.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/soundproxy.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/soundscapemanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/vorbisdecoder.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/vorbisdecoderfactory.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/wavdecoder.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/wavdecoderfactory.h"
        # Graphics Items
        "${${PROJECT_NAME}_SRC_DIR}Graphics/graphics.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/billboard.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/billboardsetproxy.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/bone.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/cameraproxy.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/entityproxy.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/gamewindow.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/graphicsenumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/graphicsmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/graphicsutilities.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/guillotinetexturepacker.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/image.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/lightproxy.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/material.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/maxrectstexturepacker.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/mesh.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/meshinfo.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/meshmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/particleaffector.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/particleemitter.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/particlesystemproxy.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/pass.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/renderableproxy.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/renderablerayquery.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/resolution.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/scenemanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/shelftexturepacker.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/skeleton.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/skylinetexturepacker.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/submesh.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/technique.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/texture.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/texturemanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/texturepacker.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/vertextools.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/viewport.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/windowsettings.h"
        # Graphics - Procedural Items
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/procedural.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/multipath.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/multishape.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/path.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/proceduraldatatypes.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/proceduralenumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/proceduralutilities.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/shape.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/triangulator.h"
        # Graphics - Procedural - Mesh Items
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/booleanmeshgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/boxcornergenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/boxgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/boxuvmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/capsulegenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/conegenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/cylindergenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/cylinderuvmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/extruder.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/hemisphereuvmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/icospheregenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/lathe.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/meshgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/meshloader.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/meshmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/planegenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/planeuvmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/roundedboxgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/spheregenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/sphereuvmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/spherifymodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/springgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/torusgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/torusknotgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/trianglebuffer.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/tubegenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/vertexnormalsmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/vertextransformmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/vertexuvmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/weldverticesmodifier.h"
        # Graphics - Procedural - Texture Items
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/alphamodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/alphamaskmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/atlasgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/blitmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/blurmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/cellgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/channelmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/circlemodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/cloudgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/coloursmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/combinemodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/convolutionmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/crackmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/cycloidmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/dilatemodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/edgedetectionmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/ellipsemodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/flaremodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/flipmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/gradientgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/imagegenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/invertmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/jittermodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/labyrinthgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/lerpgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/lookupmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/marblegenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/noisegenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/normalsmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/oilpaintmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/randompixelsmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/rectanglemodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/rotationzoommodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/segmentmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/sharpenmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/solidgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/textilegenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/texttexturemodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/texturebuffer.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/texturegenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/texturemodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/thresholdmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/vortexmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/weighedlerpmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/woodgenerator.h"
        # Input Items
        "${${PROJECT_NAME}_SRC_DIR}Input/input.h"
        "${${PROJECT_NAME}_SRC_DIR}Input/inputmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Input/inputenumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}Input/buttondevice.h"
        "${${PROJECT_NAME}_SRC_DIR}Input/controller.h"
        "${${PROJECT_NAME}_SRC_DIR}Input/device.h"
        "${${PROJECT_NAME}_SRC_DIR}Input/keyboard.h"
        "${${PROJECT_NAME}_SRC_DIR}Input/metacode.h"
        "${${PROJECT_NAME}_SRC_DIR}Input/metacodekey.h"
        "${${PROJECT_NAME}_SRC_DIR}Input/mouse.h"
        "${${PROJECT_NAME}_SRC_DIR}Input/sequencecontainer.h"
        "${${PROJECT_NAME}_SRC_DIR}Input/inpututilities.h"
        # Internal Items
        "${${PROJECT_NAME}_SRC_DIR}Internal/decompinterface.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/iostreamwrapper.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/materialloaderlistener.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/meshloaderlistener.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/meshtools.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/motionstate.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/murmurhash3.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/ogredatastreambuf.h.cpp"
        # MathTools Items
        "${${PROJECT_NAME}_SRC_DIR}MathTools/mathtools.h"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/mathtypes.h"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/arithmetic.h"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/constants.h"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/intersections.h"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/intersectiontools.h"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/mersennetwister.h"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/randomnumber.h"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/trigonometry.h"
        # Network Items
        "${${PROJECT_NAME}_SRC_DIR}Network/network.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/networkenumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/networkutilities.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/networkmanager.h"
		"${${PROJECT_NAME}_SRC_DIR}Network/connection.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/ftpclient.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/ftpcommand.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/ftpmessage.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/ftpresponse.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/httpclient.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/httpmessage.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/httprequest.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/httpresponse.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/ipaddress.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/ipv4socket.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/ipv6socket.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/packet.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/platformincludes.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/platformsocket.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/socket.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/socketdescription.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/systemaddress.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/tcpsocket.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/tcpv4socket.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/tcpv6socket.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/udpsocket.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/udpv4socket.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/udpv6socket.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/uri.h"
        # Noise Items
        "${${PROJECT_NAME}_SRC_DIR}Noise/noise.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/noiseenumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/noiseutilities.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/interpolate.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/noisegen.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/vectortable.h"
        # Noise - Model Items
        "${${PROJECT_NAME}_SRC_DIR}Noise/Model/cylinder.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Model/line.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Model/plane.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Model/sphere.h"
        # Noise - Module Items
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/abs.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/add.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/billow.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/blend.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/cache.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/checkerboard.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/clamp.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/const.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/curve.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/cylinders.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/displace.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/exponent.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/invert.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/max.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/min.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/modulebase.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/multiply.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/perlin.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/power.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/ridgedmulti.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/rotatepoint.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/scalebias.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/scalepoint.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/select.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/spheres.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/terrace.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/translatepoint.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/turbulence.h"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/voronoi.h"
        # Physics Items
        "${${PROJECT_NAME}_SRC_DIR}Physics/physics.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/physicsenumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/physicsmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/boxcollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/capsulecollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collidablepair.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collidableproxy.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collidablerayquery.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collision.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collisiondispatcher.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collisionshapemanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/compoundcollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/conecollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/conetwistconstraint.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/constraint.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/convexhullcollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/cylindercollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/dualtransformconstraint.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/dynamicmeshcollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/fieldcollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/gearconstraint.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/generic6dofconstraint.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/generic6dofspringconstraint.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/ghostproxy.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/heightfieldcollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/hinge2constraint.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/hingeconstraint.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/managerconstructioninfo.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/meshcollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/multispherecollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/planecollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/point2pointconstraint.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/primitivecollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/rigidproxy.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/sliderconstraint.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/softcollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/softproxy.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/spherecollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/staticmeshcollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/universalconstraint.h"
        # Resource Items
        "${${PROJECT_NAME}_SRC_DIR}Resource/resource.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/resourceenumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/resourcemanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/archive.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/archivestream.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/asset.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/assetgroup.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/assethandler.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/resourceutilities.h"
        # Scripting Items
        "${${PROJECT_NAME}_SRC_DIR}Scripting/script.h"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/scripting.h"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/scriptargument.h"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/scriptingmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/scriptworkunit.h"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/lua51script.h"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/lua51scriptargument.h"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/lua51scriptingengine.h"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/lua51workunit.h"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Ruby/rubyscriptargument.h"
        # Threading Items
        "${${PROJECT_NAME}_SRC_DIR}Threading/asynchronousfileloadingworkunit.h"
        "${${PROJECT_NAME}_SRC_DIR}Threading/asynchronousworkunit.h"
        "${${PROJECT_NAME}_SRC_DIR}Threading/atomicoperations.h"
        "${${PROJECT_NAME}_SRC_DIR}Threading/barrier.h"
        "${${PROJECT_NAME}_SRC_DIR}Threading/crossplatformincludes.h"
        "${${PROJECT_NAME}_SRC_DIR}Threading/dagframescheduler.h"
        "${${PROJECT_NAME}_SRC_DIR}Threading/doublebufferedresource.h"
        "${${PROJECT_NAME}_SRC_DIR}Threading/framescheduler.h"
        "${${PROJECT_NAME}_SRC_DIR}Threading/frameschedulerworkunits.h"
        "${${PROJECT_NAME}_SRC_DIR}Threading/lockguard.h"
        "${${PROJECT_NAME}_SRC_DIR}Threading/logtools.h"
        "${${PROJECT_NAME}_SRC_DIR}Threading/monopoly.h"
        "${${PROJECT_NAME}_SRC_DIR}Threading/mutex.h"
        "${${PROJECT_NAME}_SRC_DIR}Threading/readwritespinlock.h"
        "${${PROJECT_NAME}_SRC_DIR}Threading/rollingaverage.h"
        "${${PROJECT_NAME}_SRC_DIR}Threading/spinlock.h"
        "${${PROJECT_NAME}_SRC_DIR}Threading/systemcalls.h"
        "${${PROJECT_NAME}_SRC_DIR}Threading/threadingenumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}Threading/thread.h"
        "${${PROJECT_NAME}_SRC_DIR}Threading/workunit.h"
        "${${PROJECT_NAME}_SRC_DIR}Threading/workunitkey.h"
        # UI (User Interface) Items
        "${${PROJECT_NAME}_SRC_DIR}UI/ui.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/uimanager.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/uienumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/action.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/actionhandler.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/brutestrategy.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/button.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/character.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/charactertraits.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/checkbox.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/defaultmarkupparser.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/dropdownlist.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/editbox.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/font.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/glyph.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/gridcontainer.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/horizontalcontainer.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/horizontallayoutstrategy.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/horizontalscrollbar.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/hotkeyhandler.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/imagelayer.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/kerning.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/layoutcontainer.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/layoutstrategy.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/linearcontainer.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/linelist.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/listbox.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/markupparser.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/menuentry.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/mousehoverstrategy.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/multiimagelayer.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/multilinetextlayer.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/nineboxstrategy.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/pagedcontainer.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/pageprovider.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/positioninginfo.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/quadrenderable.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/quadrenderer.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/radiobutton.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/rect.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/renderable.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/renderlayer.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/renderlayergroup.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/screen.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/scrollbar.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/simplerenderer.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/singleimagelayer.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/singlelinetextlayer.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/sizinginfo.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/spinner.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/sprite.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/stackbutton.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/stackedcontainer.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/tabset.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/textcursor.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/textlayer.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/textline.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/texttoken.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/textureatlas.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/textureatlashandler.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/unifieddim.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/vertex.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/verticalcontainer.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/verticallayoutstrategy.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/verticalscrollbar.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/widget.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/widgetfactory.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/window.h"
        # XML Items
        "${${PROJECT_NAME}_SRC_DIR}XML/objectrange.h"
        "${${PROJECT_NAME}_SRC_DIR}XML/xml.h"       # Going away soon
        "${${PROJECT_NAME}_SRC_DIR}XML/attribute.h"
        "${${PROJECT_NAME}_SRC_DIR}XML/attributeiterator.h"
        "${${PROJECT_NAME}_SRC_DIR}XML/document.h"
        "${${PROJECT_NAME}_SRC_DIR}XML/memorymanagement.h"
        "${${PROJECT_NAME}_SRC_DIR}XML/node.h"
        "${${PROJECT_NAME}_SRC_DIR}XML/nodeiterator.h"
        "${${PROJECT_NAME}_SRC_DIR}XML/nodetext.h"
        "${${PROJECT_NAME}_SRC_DIR}XML/parseresult.h"
        "${${PROJECT_NAME}_SRC_DIR}XML/treewalker.h"
        "${${PROJECT_NAME}_SRC_DIR}XML/writer.h"
        "${${PROJECT_NAME}_SRC_DIR}XML/xmlenumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}XML/xmlstring.cpp"
        "${${PROJECT_NAME}_SRC_DIR}XML/xpathnode.h"
        "${${PROJECT_NAME}_SRC_DIR}XML/xpathnodeset.h"
        "${${PROJECT_NAME}_SRC_DIR}XML/xpathparseresult.h"
        "${${PROJECT_NAME}_SRC_DIR}XML/xpathvariable.h"
        "${${PROJECT_NAME}_SRC_DIR}XML/xpathvariableset.h"
        "${${PROJECT_NAME}_SRC_DIR}XML/xpathquery.h"

#######################################
# A complete listing of our source files

        # Root Namespace(Mezzanine) Items
        "${${PROJECT_NAME}_SRC_DIR}actor.cpp"
        "${${PROJECT_NAME}_SRC_DIR}actormanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}areaeffect.cpp"
        "${${PROJECT_NAME}_SRC_DIR}areaeffectmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}attachable.cpp"
        "${${PROJECT_NAME}_SRC_DIR}axisalignedbox.cpp"
        "${${PROJECT_NAME}_SRC_DIR}binarybuffer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}cameracontroller.cpp"
        "${${PROJECT_NAME}_SRC_DIR}circle.cpp"
        "${${PROJECT_NAME}_SRC_DIR}colourvalue.cpp"
        "${${PROJECT_NAME}_SRC_DIR}crossplatform.cpp"
        "${${PROJECT_NAME}_SRC_DIR}datastream.cpp"
        "${${PROJECT_NAME}_SRC_DIR}debris.cpp"
        "${${PROJECT_NAME}_SRC_DIR}debrismanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}entresol.cpp"
        "${${PROJECT_NAME}_SRC_DIR}entresolmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}event.cpp"
        "${${PROJECT_NAME}_SRC_DIR}eventbase.cpp"
        "${${PROJECT_NAME}_SRC_DIR}eventgamewindow.cpp"
        "${${PROJECT_NAME}_SRC_DIR}eventmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}eventpublisher.cpp"
        "${${PROJECT_NAME}_SRC_DIR}eventquit.cpp"
        "${${PROJECT_NAME}_SRC_DIR}eventsubscriber.cpp"
        "${${PROJECT_NAME}_SRC_DIR}eventsubscriberslot.cpp"
        "${${PROJECT_NAME}_SRC_DIR}eventuserinput.cpp"
        "${${PROJECT_NAME}_SRC_DIR}exception.cpp"
        "${${PROJECT_NAME}_SRC_DIR}fieldofforce.cpp"
        "${${PROJECT_NAME}_SRC_DIR}filestream.cpp"
        "${${PROJECT_NAME}_SRC_DIR}gravityfield.cpp"
        "${${PROJECT_NAME}_SRC_DIR}gravitywell.cpp"
        "${${PROJECT_NAME}_SRC_DIR}hashedstring.cpp"
        "${${PROJECT_NAME}_SRC_DIR}interpolator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}linegroup.cpp"
        "${${PROJECT_NAME}_SRC_DIR}linesegment.cpp"
        "${${PROJECT_NAME}_SRC_DIR}managedptr.cpp"
        "${${PROJECT_NAME}_SRC_DIR}managerbase.cpp"
        "${${PROJECT_NAME}_SRC_DIR}matrix3x3.cpp"
        "${${PROJECT_NAME}_SRC_DIR}matrix4x4.cpp"
        "${${PROJECT_NAME}_SRC_DIR}memorystream.cpp"
        "${${PROJECT_NAME}_SRC_DIR}meshterrain.cpp"
        "${${PROJECT_NAME}_SRC_DIR}mousepicker.cpp"
        "${${PROJECT_NAME}_SRC_DIR}mousepickdragger.cpp"
        "${${PROJECT_NAME}_SRC_DIR}multibodyvehicle.cpp"
        "${${PROJECT_NAME}_SRC_DIR}objectsettings.cpp"
        "${${PROJECT_NAME}_SRC_DIR}plane.cpp"
        "${${PROJECT_NAME}_SRC_DIR}player.cpp"
        "${${PROJECT_NAME}_SRC_DIR}playeraction.cpp"
        "${${PROJECT_NAME}_SRC_DIR}playercontrol.cpp"
        "${${PROJECT_NAME}_SRC_DIR}playermanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}playerview.cpp"
        "${${PROJECT_NAME}_SRC_DIR}quaternion.cpp"
        "${${PROJECT_NAME}_SRC_DIR}ray.cpp"
        "${${PROJECT_NAME}_SRC_DIR}raycastvehicle.cpp"
        "${${PROJECT_NAME}_SRC_DIR}rayquerytool.cpp"
        "${${PROJECT_NAME}_SRC_DIR}rigiddebris.cpp"
        "${${PROJECT_NAME}_SRC_DIR}simplevehicle.cpp"
        "${${PROJECT_NAME}_SRC_DIR}serialization.cpp"
        "${${PROJECT_NAME}_SRC_DIR}softdebris.cpp"
        "${${PROJECT_NAME}_SRC_DIR}sphere.cpp"
        "${${PROJECT_NAME}_SRC_DIR}statemachine.cpp"
        "${${PROJECT_NAME}_SRC_DIR}statetransition.cpp"
        "${${PROJECT_NAME}_SRC_DIR}statetransitionaction.cpp"
        "${${PROJECT_NAME}_SRC_DIR}streamlogging.cpp"
        "${${PROJECT_NAME}_SRC_DIR}stringtool.cpp"
        "${${PROJECT_NAME}_SRC_DIR}terrain.cpp"
        "${${PROJECT_NAME}_SRC_DIR}terrainmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}timer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}track.cpp"
        "${${PROJECT_NAME}_SRC_DIR}transform.cpp"
        "${${PROJECT_NAME}_SRC_DIR}transformableobject.cpp"
        "${${PROJECT_NAME}_SRC_DIR}triangle.cpp"
        "${${PROJECT_NAME}_SRC_DIR}uidgenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}unicode.cpp"
        "${${PROJECT_NAME}_SRC_DIR}vector2.cpp"
        "${${PROJECT_NAME}_SRC_DIR}vector3.cpp"
        "${${PROJECT_NAME}_SRC_DIR}vehicle.cpp"
        "${${PROJECT_NAME}_SRC_DIR}vehiclemanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}world.cpp"
        "${${PROJECT_NAME}_SRC_DIR}worldmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}worldobject.cpp"
        "${${PROJECT_NAME}_SRC_DIR}worldproxy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}worldtrigger.cpp"
        # Audio Items
        "${${PROJECT_NAME}_SRC_DIR}Audio/audiomanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/musicplayer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/playlist.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/rawdecoder.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/soundscapemanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/vorbisdecoder.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/wavdecoder.cpp"
        # Graphics Items
        "${${PROJECT_NAME}_SRC_DIR}Graphics/billboard.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/billboardsetproxy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/bone.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/cameraproxy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/entityproxy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/gamewindow.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/graphicsmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/graphicsutilities.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/guillotinetexturepacker.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/image.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/lightproxy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/material.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/maxrectstexturepacker.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/mesh.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/meshmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/particleaffector.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/particleemitter.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/particlesystemproxy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/pass.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/renderableproxy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/renderablerayquery.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/scenemanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/shelftexturepacker.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/skeleton.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/skylinetexturepacker.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/submesh.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/technique.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/texture.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/texturemanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/texturepacker.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/vertextools.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/viewport.cpp"
        # Graphics - Procedural Items
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/multipath.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/multishape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/path.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/proceduralutilities.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/shape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/triangulator.cpp"
        # Graphics - Procedural - Mesh Items
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/booleanmeshgenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/boxcornergenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/boxgenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/boxuvmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/capsulegenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/conegenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/cylindergenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/cylinderuvmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/extruder.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/hemisphereuvmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/icospheregenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/lathe.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/meshloader.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/planegenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/planeuvmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/roundedboxgenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/spheregenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/sphereuvmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/spherifymodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/springgenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/torusgenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/torusknotgenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/trianglebuffer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/tubegenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/vertexnormalsmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/vertextransformmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/vertexuvmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Mesh/weldverticesmodifier.cpp"
        # Graphics - Procedural - Texture Items
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/alphamodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/alphamaskmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/atlasgenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/blitmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/blurmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/cellgenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/channelmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/circlemodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/cloudgenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/coloursmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/combinemodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/convolutionmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/crackmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/cycloidmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/dilatemodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/edgedetectionmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/ellipsemodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/flaremodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/flipmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/gradientgenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/imagegenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/invertmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/jittermodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/labyrinthgenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/lerpgenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/lookupmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/marblegenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/noisegenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/normalsmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/oilpaintmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/randompixelsmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/rectanglemodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/rotationzoommodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/segmentmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/sharpenmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/solidgenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/textilegenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/texttexturemodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/texturebuffer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/texturegenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/thresholdmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/vortexmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/weighedlerpmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/woodgenerator.cpp"
        # Input Items
        "${${PROJECT_NAME}_SRC_DIR}Input/inputmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/buttondevice.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/controller.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/device.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/keyboard.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/metacode.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/metacodekey.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/mouse.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/sequencecontainer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/inpututilities.cpp"
        # Internal Items
        "${${PROJECT_NAME}_SRC_DIR}Internal/decompinterface.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/iostreamwrapper.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/materialloaderlistener.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/meshloaderlistener.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/meshtools.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/motionstate.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/murmurhash3.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/ogredatastreambuf.cpp"
        # MathTools Items
        "${${PROJECT_NAME}_SRC_DIR}MathTools/mathtools.cpp"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/arithmetic.cpp"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/constants.cpp"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/intersections.cpp"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/intersectiontools.cpp"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/mersennetwister.cpp"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/randomnumber.cpp"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/trigonometry.cpp"
        # Network Items
        "${${PROJECT_NAME}_SRC_DIR}Network/networkutilities.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/networkmanager.cpp"
		"${${PROJECT_NAME}_SRC_DIR}Network/connection.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/ftpclient.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/ftpcommand.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/ftpmessage.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/ftpresponse.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/httpclient.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/httpmessage.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/httprequest.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/httpresponse.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/ipaddress.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/ipv4socket.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/ipv6socket.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/packet.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/platformsocket.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/socket.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/systemaddress.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/tcpsocket.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/tcpv4socket.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/tcpv6socket.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/udpsocket.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/udpv4socket.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/udpv6socket.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/uri.cpp"
        # Noise Items
        "${${PROJECT_NAME}_SRC_DIR}Noise/noiseutilities.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/noisegen.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/vectortable.cpp"
        # Noise - Model Items
        "${${PROJECT_NAME}_SRC_DIR}Noise/Model/cylinder.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Model/line.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Model/plane.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Model/sphere.cpp"
        # Noise - Module Items
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/abs.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/add.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/billow.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/blend.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/cache.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/checkerboard.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/clamp.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/const.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/curve.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/cylinders.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/displace.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/exponent.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/invert.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/max.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/min.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/modulebase.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/multiply.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/perlin.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/power.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/ridgedmulti.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/rotatepoint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/scalebias.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/scalepoint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/select.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/spheres.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/terrace.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/translatepoint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/turbulence.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Noise/Module/voronoi.cpp"
        # Physics Items
        "${${PROJECT_NAME}_SRC_DIR}Physics/physicsmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/boxcollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/capsulecollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collidablepair.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collidableproxy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collidablerayquery.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collision.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collisiondispatcher.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collisionshapemanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/compoundcollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/conecollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/conetwistconstraint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/constraint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/convexhullcollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/cylindercollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/dualtransformconstraint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/dynamicmeshcollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/fieldcollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/gearconstraint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/generic6dofconstraint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/generic6dofspringconstraint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/ghostproxy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/heightfieldcollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/hinge2constraint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/hingeconstraint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/meshcollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/multispherecollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/planecollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/point2pointconstraint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/primitivecollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/rigidproxy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/sliderconstraint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/softcollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/softproxy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/spherecollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/staticmeshcollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/universalconstraint.cpp"
        # Resource Items
        "${${PROJECT_NAME}_SRC_DIR}Resource/resourcemanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/archive.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/archivestream.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/asset.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/assetgroup.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/assethandler.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/resourceutilities.cpp"
        # Scripting Items
        "${${PROJECT_NAME}_SRC_DIR}Scripting/scriptingmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/scriptbindingmainlua51.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/scriptbindingmainlua51unsafe.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/scriptbindingxmllua51.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/scriptbindingxmllua51unsafe.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/scriptbindingthreadinglua51.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/scriptbindingthreadinglua51unsafe.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/scriptbindingphysicslua51.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/scriptbindingphysicslua51unsafe.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/scriptbindingmathtoolslua51.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/scriptbindingmathtoolslua51unsafe.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/lua51script.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/lua51scriptargument.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/lua51scriptingengine.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/lua51workunit.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Ruby/rubyscriptargument.cpp"
        # Threading Items
        "${${PROJECT_NAME}_SRC_DIR}Threading/asynchronousfileloadingworkunit.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Threading/asynchronousworkunit.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Threading/atomicoperations.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Threading/barrier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Threading/doublebufferedresource.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Threading/framescheduler.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Threading/frameschedulerworkunits.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Threading/logtools.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Threading/monopoly.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Threading/mutex.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Threading/readwritespinlock.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Threading/spinlock.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Threading/rollingaverage.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Threading/systemcalls.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Threading/thread.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Threading/workunit.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Threading/workunitkey.cpp"
        # UI (User Interface) Items
        "${${PROJECT_NAME}_SRC_DIR}UI/uimanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/action.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/actionhandler.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/brutestrategy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/button.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/character.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/charactertraits.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/checkbox.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/defaultmarkupparser.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/dropdownlist.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/editbox.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/font.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/gridcontainer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/horizontalcontainer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/horizontallayoutstrategy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/horizontalscrollbar.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/hotkeyhandler.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/imagelayer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/layoutcontainer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/layoutstrategy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/linearcontainer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/linelist.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/listbox.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/markupparser.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/menuentry.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/multiimagelayer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/multilinetextlayer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/nineboxstrategy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/pagedcontainer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/pageprovider.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/quadrenderable.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/quadrenderer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/radiobutton.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/renderable.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/renderlayer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/renderlayergroup.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/screen.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/scrollbar.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/simplerenderer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/singleimagelayer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/singlelinetextlayer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/spinner.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/stackbutton.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/stackedcontainer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/tabset.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/textcursor.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/textlayer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/textline.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/texttoken.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/textureatlas.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/textureatlashandler.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/verticalcontainer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/verticallayoutstrategy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/verticalscrollbar.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/widget.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/window.cpp"
        # XML Items
        "${${PROJECT_NAME}_SRC_DIR}XML/xml.cpp"     # Going away soon
        "${${PROJECT_NAME}_SRC_DIR}XML/attribute.cpp"
        "${${PROJECT_NAME}_SRC_DIR}XML/attributeiterator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}XML/document.cpp"
        "${${PROJECT_NAME}_SRC_DIR}XML/memorymanagement.cpp"
        "${${PROJECT_NAME}_SRC_DIR}XML/node.cpp"
        "${${PROJECT_NAME}_SRC_DIR}XML/nodeiterator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}XML/nodetext.cpp"
        "${${PROJECT_NAME}_SRC_DIR}XML/parseresult.cpp"
        "${${PROJECT_NAME}_SRC_DIR}XML/treewalker.cpp"
        "${${PROJECT_NAME}_SRC_DIR}XML/writer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}XML/xmlstring.cpp"
        "${${PROJECT_NAME}_SRC_DIR}XML/xpathnode.cpp"
        "${${PROJECT_NAME}_SRC_DIR}XML/xpathnodeset.cpp"
        "${${PROJECT_NAME}_SRC_DIR}XML/xpathparseresult.cpp"
        "${${PROJECT_NAME}_SRC_DIR}XML/xpathvariable.cpp"
        "${${PROJECT_NAME}_SRC_DIR}XML/xpathvariableset.cpp"
        "${${PROJECT_NAME}_SRC_DIR}XML/xpathquery.cpp"
)

###############################################################################
# Audio Subsystem Configuration

set(AudioProviderSet 0)
option(Mezz_Audio_UseOALS "Use OpenAL-Soft as the audio provider for the engine." ON)

if(Mezz_Audio_UseOALS)
    if(AudioProviderSet)
        message(FATAL_ERROR "A provider for the Audio subsystem has already been set "
                            "Only one provider may be selected.")
    endif(AudioProviderSet)
    set(AudioProviderSet 1)

    set(Mezzanine_sources ${Mezzanine_sources}

        #######################################
        # Header files

        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oals.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalsaudiomanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalsaudiomanagerfactory.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalsdefines.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalseffect.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalseffectshandler.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalsefxinterface.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalsfilter.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalslistener.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalsrecorder.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalssound.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalssoundproxy.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalssoundscapemanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalssoundscapemanagerfactory.h"

        #######################################
        # Source files

        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalsaudiomanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalsaudiomanagerfactory.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalseffect.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalseffectshandler.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalsefxinterface.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalsfilter.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalslistener.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalsrecorder.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalssound.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalssoundproxy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalssoundscapemanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/OALS/oalssoundscapemanagerfactory.cpp"
    )

    include_directories(
        "${ProjectBinaryDir}Mezzanine/libincludes/common/openalsoftsrc/openalsoft"
        "${${PROJECT_NAME}_LIBINCLUDE_DIR}common/openalsoftsrc/openalsoft/include"
        "${${PROJECT_NAME}_LIBINCLUDE_DIR}common/openalsoftsrc/openalsoft/OpenAL32/include"
    )
    
    IF(WIN32)
        set(SoundLibraryNames ${SoundLibraryNames}
            "OpenAL32"
        )
    ELSE()
        set(SoundLibraryNames ${SoundLibraryNames}
            "openal"
        )
    ENDIF()

    add_definitions( "${D}ENABLE_OALS_AUDIO_IMPLEMENTATION" "${D}AL_LIBTYPE_STATIC" )
endif(Mezz_Audio_UseOALS)

if(NOT AudioProviderSet)
    message(FATAL_ERROR "A provider for the Audio subsystem has not been set.")
endif(NOT AudioProviderSet)

###############################################################################
# Audio Encoding support configuration

set(Mezz_Audio_Encodings "")

option(Mezz_Audio_EnableRawEncoding "Enable the encoding and decoding of unencoded data." ON)
if(Mezz_Audio_EnableRawEncoding)
    message ( STATUS "${PROJECT_NAME} - Enabling Raw encoding.")
    set(Mezz_Audio_Encodings ${Mezz_Audio_Encodings} "${D}ENABLE_RAW_ENCODE")
endif(Mezz_Audio_EnableRawEncoding)

option(Mezz_Audio_EnableWavEncoding "Enable the encoding and decoding of .wav encoded data." ON)
if(Mezz_Audio_EnableWavEncoding)
    message ( STATUS "${PROJECT_NAME} - Enabling .wav encoding.")
    set(Mezz_Audio_Encodings ${Mezz_Audio_Encodings} "${D}ENABLE_WAV_ENCODE")
endif(Mezz_Audio_EnableWavEncoding)

option(Mezz_Audio_EnableFlacEncoding "Enable the encoding and decoding of .flac encoded data." ON)
if(Mezz_Audio_EnableFlacEncoding)
    message ( STATUS "${PROJECT_NAME} - Enabling .flac encoding")
    set(Mezz_Audio_Encodings ${Mezz_Audio_Encodings} "${D}ENABLE_FLAC_ENCODE")
endif(Mezz_Audio_EnableFlacEncoding)

option(Mezz_Audio_EnableVorbisEncoding
        "Enable the encoding and decoding of .ogg (vorbis) encoded data." ON)
if(Mezz_Audio_EnableVorbisEncoding)
    message ( STATUS "${PROJECT_NAME} - Enabling .ogg (vorbis) encoding.")
    set(Mezz_Audio_Encodings ${Mezz_Audio_Encodings} "${D}ENABLE_VORBIS_ENCODE")
endif(Mezz_Audio_EnableVorbisEncoding)

#option(Mezz_Audio_EnableMp3Encoding "Enable the encoding and decoding of .mp3 encoded data." ON)
#if(Mezz_Audio_EnableMp3Encoding)
#   message ( STATUS "${PROJECT_NAME} - Enabling .mp3 encoding.")
#    set(Mezz_Audio_Encodings ${Mezz_Audio_Encodings} "${D}ENABLE_MP3_ENCODE")
#endif(Mezz_Audio_EnableMp3Encoding)

option(Mezz_Audio_EnableSpeexEncoding "Enable the encoding and decoding of .spx encoded data." ON)
if(Mezz_Audio_EnableSpeexEncoding)
    message ( STATUS "${PROJECT_NAME} - Enabling .spx encoding.")
    set(Mezz_Audio_Encodings ${Mezz_Audio_Encodings} "${D}ENABLE_SPEEX_ENCODE")
endif(Mezz_Audio_EnableSpeexEncoding)

option(Mezz_Audio_EnableOpusEncoding "Enable the encoding and decoding of .opus encoded data." ON)
if(Mezz_Audio_EnableOpusEncoding)
    message ( STATUS "${PROJECT_NAME} - Enabling .opus encoding.")
    set(Mezz_Audio_Encodings ${Mezz_Audio_Encodings} "${D}ENABLE_OPUS_ENCODE")
endif(Mezz_Audio_EnableOpusEncoding)

add_definitions( ${Mezz_Audio_Encodings} )

###############################################################################
# Graphics Rendersystem configuration

if(${Mezz_BuildRenderSystemDirectX9})
    set(Mezzanine_sources ${Mezzanine_sources}
        "${${PROJECT_NAME}_SRC_DIR}Graphics/d3d9rendersyshelper.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/d3d9rendersyshelper.cpp"
    )
endif(${Mezz_BuildRenderSystemDirectX9})

if(${Mezz_BuildRenderSystemDirectX11})
    set(Mezzanine_sources ${Mezzanine_sources}
        "${${PROJECT_NAME}_SRC_DIR}Graphics/d3d11rendersyshelper.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/d3d11rendersyshelper.cpp"
    )
endif(${Mezz_BuildRenderSystemDirectX11})

if(${Mezz_BuildRenderSystemOpenGL})
    set(Mezzanine_sources ${Mezzanine_sources}
        "${${PROJECT_NAME}_SRC_DIR}Graphics/oglrendersyshelper.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/oglrendersyshelper.cpp"
    )
endif(${Mezz_BuildRenderSystemOpenGL})

if(${Mezz_BuildRenderSystemOpenGL3Plus})
    set(Mezzanine_sources ${Mezzanine_sources}
        "${${PROJECT_NAME}_SRC_DIR}Graphics/ogl3plusrendersyshelper.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/ogl3plusrendersyshelper.cpp"
    )
endif(${Mezz_BuildRenderSystemOpenGL3Plus})

if(${Mezz_BuildRenderSystemOpenGLES})
    set(Mezzanine_sources ${Mezzanine_sources}
        "${${PROJECT_NAME}_SRC_DIR}Graphics/oglesrendersyshelper.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/oglesrendersyshelper.cpp"
    )
endif(${Mezz_BuildRenderSystemOpenGLES})

if(${Mezz_BuildRenderSystemOpenGLES2})
    set(Mezzanine_sources ${Mezzanine_sources}
        "${${PROJECT_NAME}_SRC_DIR}Graphics/ogles2rendersyshelper.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/ogles2rendersyshelper.cpp"
    )
endif(${Mezz_BuildRenderSystemOpenGLES2})

###############################################################################
# Functions for Later use

#Description: This function copies items from the Data/lib directory into it's expected location in
#   the output directory During build time. It does not need leading slashes
#PreCondition: This function can only be called after "add_library(${PROJECT_NAME} ...)" is called,
#   otherwise it will fail silently
function(CopyFilesDuringBuild SourceFileInDataTree TargetFileInBinaryTree)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E echo
            "Preparing File: ${${PROJECT_NAME}_BINARY_DIR}${TargetFileInBinaryTree}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${${PROJECT_NAME}_LIBBINARY_DIR}${SourceFileInDataTree}"
            "${${PROJECT_NAME}_BINARY_DIR}${TargetFileInBinaryTree}"
        VERBATIM
    )
endfunction(CopyFilesDuringBuild)

###############################################################################
# System Build Items
#######################################
# All Changes that could be system specific should be defined in these variables
# These will set sane defaults (Where possible), but specific platforms should
# set a value of their own.

set(LIBTYPE "STATIC") # OpenAL
file(MAKE_DIRECTORY "${ProjectBinaryDir}include") # To not interfere with SDL build process

# Bullet related values
set(BUILD_AMD_OPENCL_DEMOS OFF CACHE BOOL "Do not build demos!" FORCE)
set(BUILD_INTEL_OPENCL_DEMOS OFF CACHE BOOL "Do not build demos!" FORCE)
set(BUILD_NVIDIA_OPENCL_DEMOS OFF CACHE BOOL "Do not build demos!" FORCE)
set(BUILD_MINICL_OPENCL_DEMOS OFF CACHE BOOL "Do not build demos!" FORCE)
set(BUILD_CPU_DEMOS OFF CACHE BOOL "Do not build demos!" FORCE)
set(USE_GLUT OFF CACHE BOOL "Do not build demos!" FORCE)
set(BUILD_DEMOS OFF CACHE BOOL "Do not build demos!" FORCE)
set(INSTALL_LIBS OFF CACHE BOOL "Do not install libs!" FORCE)
set(INSTALL_EXTRA_LIBS OFF CACHE BOOL "Do not install libs!" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "Do not build unit tests!" FORCE)
set(BUILD_MULTITHREADING ON CACHE BOOL "Do build multithreading constructs!" FORCE)

#######################################
# Windows Build Specific Items
if(${WIN32})
        message ( STATUS "${PROJECT_NAME} - Begin Windows32/64 Specific Build Process")
        add_definitions( -DWINDOWS -DEXPORTINGMEZZANINEDLL )
        message ( STATUS "${PROJECT_NAME} - End Windows32/64 Specific Build Process")
endif(${WIN32})

#######################################
# Linux Build Specific Items

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        message ( STATUS "${PROJECT_NAME} - Begin Linux Specific Build Process")
        add_definitions( -DOGRE_GUI_GLX -DLINUX )
        include_directories(
                ${Mezz_OgreBuildDir}/ogre/OgreMain/include/
                ${Mezz_OgreBuildDir}/ogre/include/
        )
        message ( STATUS "${PROJECT_NAME} - End Linux Specific Build Process")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

#######################################
# Mac OS X and iPhone Build pecific Items
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        message ( STATUS "${PROJECT_NAME} - Begin Apple Specific Build Process")
        add_definitions( -DMACOSX)
    #set_source_files_properties("${${PROJECT_NAME}_SRC_DIR}Graphics/gamewindow.cpp" PROPERTIES
    #   COMPILE_FLAGS "-x objective-c++")
    set_property(SOURCE "${${PROJECT_NAME}_SRC_DIR}Graphics/gamewindow.cpp" PROPERTY
                    COMPILE_FLAGS "-x objective-c++")
    message ( STATUS "${PROJECT_NAME} - End Apple Specific Build Process")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

###############################################################################
# Common Library Configuration items

set(BulletSrcDir "${${PROJECT_NAME}_LIBINCLUDE_DIR}common/bulletsrc/bulletsvnsrc/")

include_directories( ${${PROJECT_NAME}_SRC_DIR}
    "${BulletSrcDir}src"
    "${BulletSrcDir}src/LinearMath"
    "${BulletSrcDir}Extras/ConvexDecomposition"
    "${BulletSrcDir}Extras/Serialize/BulletWorldImporter"

    "${ProjectBinaryDir}Mezzanine/libincludes/common/sdlsrc/SDL/include"
    "${${PROJECT_NAME}_LIBINCLUDE_DIR}common/sdlsrc/SDL/include"

    "${${PROJECT_NAME}_LIBINCLUDE_DIR}common/oggvorbis/"
    "${${PROJECT_NAME}_LIBINCLUDE_DIR}common/oggvorbis/libvorbis-1.3.3/include/"
    "${${PROJECT_NAME}_LIBINCLUDE_DIR}common/oggvorbis/libogg-1.3.0/include/"
    #"${ProjectBinaryDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/include"

    ${OgreConfigHeaderIncludeDir}
    ${OgreHeaderIncludeDir}
)


link_directories(
        "${ProjectBinaryDir}Mezzanine/bin"
)

###############################################################################
# Prepare the actual Build
#add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_sources} )

#######################################
# Prepare Common Dependencies

add_subdirectory (libincludes/common/bulletsrc)
add_subdirectory (libincludes/common/ogresrc)
add_subdirectory (libincludes/common/oggvorbis)
add_subdirectory (libincludes/common/openalsoftsrc)
add_subdirectory (libincludes/common/sdlsrc/SDL)

#######################################
# Extra includes from Ogre

if(${WIN32})
    include_directories( ${DirectX_INCLUDE_DIR} ${DirectX_D3D11_INCLUDE_DIR} )
endif(${WIN32})

# This was moved to below the sub-libraries so the directX includes in the lines above will work.
# This is necessary because of static linking and the lack of foresight on the part of the people
# that wrote those rendersystems.
# It's a hack and makes me a sad panda.
###############################################################################
# Prepare the actual Build
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_sources} )

target_compile_options( ${PROJECT_NAME} PRIVATE ${CommonBuildFlags} )

set(SDL_ATOMIC    off CACHE BOOL "DISABLED as Mezzanine Component" FORCE)
set(SDL_AUDIO     off CACHE BOOL "DISABLED as Mezzanine Component" FORCE)
set(SDL_CPUINFO   off CACHE BOOL "DISABLED as Mezzanine Component" FORCE)
set(SDL_EVENTS    on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)
set(SDL_FILE      on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)
set(SDL_HAPTIC    on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)
set(SDL_JOYSTICK  on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)
set(SDL_LOADSO    on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)
set(SDL_POWER     on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)
set(SDL_RENDER    off CACHE BOOL "DISABLED as Mezzanine Component" FORCE)
set(SDL_THREADS   off CACHE BOOL "DISABLED as Mezzanine Component" FORCE)
set(SDL_VIDEO     on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)

set(SDL_SHARED    off CACHE BOOL "DISABLED as Mezzanine Component" FORCE)
set(SDL_STATIC    on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)

if(${Mezz_Lua51})
        add_subdirectory (libincludes/common/lua51)
        include_directories(${Mezz_Lua51IncludeDirectory})
endif(${Mezz_Lua51})

if(${Mezz_Ruby})
        add_subdirectory (libincludes/common/ruby200)
        include_directories(${RubyIncludeDirectory})
endif(${Mezz_Ruby})

###############################################################################
# Swig binding generation

set(SWIG_PROJECT "")
if(${Mezz_SWIG})
    # Here are the original lines describing the commands from lua51scriptengine.h
    #/// swig2.0 -c++ -v -Wall -lua -importall -includeall -o Scripting/Lua51/scriptbindinglua51.cpp
    #   mezzanine.h
    #/// swig2.0 -c++ -v -Wall -lua -importall -includeall -DSWIG_UNSAFE -o
    #   Scripting/Lua51/scriptbindinglua51unsafe.cpp mezzanine.h

    set(SWIG_PROJECT "SWIG_${PROJECT_NAME}")
    #add_library(${SWIG_PROJECT} SHARED "${${PROJECT_NAME}_SRC_DIR}blank.cpp" )

    # Exists solely to Aggregate other projects
    add_custom_target(
        ${SWIG_PROJECT}
        COMMAND ${CMAKE_COMMAND} -E echo "Preparing Mezzanine with Swig"
    )

    if(${Mezz_Lua51})
        add_custom_target(
            ${SWIG_PROJECT}_MainLua51
            COMMAND ${CMAKE_COMMAND} -E echo "Preparing Mezzanine Main library for Lua."
            COMMAND ${CMAKE_COMMAND} -E echo
                "Using special arguments for SWIG - ${Mezz_Lua51_Arguments}"
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -lua -includeall ${Mezz_Lua51_Arguments}
                -DSWIG_MAIN -o Scripting/Lua51/scriptbindingmainlua51.cpp mezzanine.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        add_custom_target(
            ${SWIG_PROJECT}_MainLua51Unsafe
            COMMAND ${CMAKE_COMMAND} -E echo
                "Preparing Unrestricted (do not use with untrusted scripts) Mezzanine Main library"
                "for Lua."
            COMMAND ${CMAKE_COMMAND} -E echo
                "Using special arguments for SWIG - ${Mezz_Lua51_Arguments}"
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -lua -includeall ${Mezz_Lua51_Arguments}
                -DSWIG_MAIN -DSWIG_UNSAFE -o Scripting/Lua51/scriptbindingmainlua51unsafe.cpp
                mezzanine.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        # XML/xml.h
        add_custom_target(
            ${SWIG_PROJECT}_XMLLua51
            COMMAND ${CMAKE_COMMAND} -E echo "Preparing Mezzanine XML library for Lua."
            COMMAND ${CMAKE_COMMAND} -E echo
                "Using special arguments for SWIG - ${Mezz_Lua51_Arguments}"
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -lua -includeall ${Mezz_Lua51_Arguments}
                -DSWIG_XML -o Scripting/Lua51/scriptbindingxmllua51.cpp XML/xml.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        add_custom_target(
            ${SWIG_PROJECT}_XMLLua51Unsafe
            COMMAND ${CMAKE_COMMAND} -E echo
                "Preparing Unrestricted(do not use with untrusted scripts) Mezzanine XML library "
                "for Lua."
            COMMAND ${CMAKE_COMMAND} -E echo
                "Using special arguments for SWIG - ${Mezz_Lua51_Arguments}"
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -lua -includeall ${Mezz_Lua51_Arguments}
                -DSWIG_XML -DSWIG_UNSAFE -o Scripting/Lua51/scriptbindingxmllua51unsafe.cpp
                XML/xml.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        # Threading/dagframescheduler.h
        add_custom_target(
            ${SWIG_PROJECT}_ThreadingLua51
            COMMAND ${CMAKE_COMMAND} -E echo "Preparing Mezzanine Threading library for Lua."
            COMMAND ${CMAKE_COMMAND} -E echo
                "Using special arguments for SWIG - ${Mezz_Lua51_Arguments}"
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -lua -includeall ${Mezz_Lua51_Arguments}
                -DSWIG_THREADING -o Scripting/Lua51/scriptbindingthreadinglua51.cpp
                Threading/dagframescheduler.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        add_custom_target(
            ${SWIG_PROJECT}_ThreadingLua51Unsafe
            COMMAND ${CMAKE_COMMAND} -E echo
                "Preparing Unrestricted(do not use with untrusted scripts) Mezzanine Threading "
                "library for Lua."
            COMMAND ${CMAKE_COMMAND} -E echo
                "Using special arguments for SWIG - ${Mezz_Lua51_Arguments}"
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -lua -includeall ${Mezz_Lua51_Arguments}
                -DSWIG_THREADING -DSWIG_UNSAFE -o
                Scripting/Lua51/scriptbindingthreadinglua51unsafe.cpp Threading/dagframescheduler.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        # Physics/physics.h
        add_custom_target(
            ${SWIG_PROJECT}_PhysicsLua51
            COMMAND ${CMAKE_COMMAND} -E echo "Preparing Mezzanine Physics library for Lua."
            COMMAND ${CMAKE_COMMAND} -E echo
                "Using special arguments for SWIG - ${Mezz_Lua51_Arguments}"
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -lua -includeall ${Mezz_Lua51_Arguments}
                -DSWIG_PHYSICS -o Scripting/Lua51/scriptbindingphysicslua51.cpp Physics/physics.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        add_custom_target(
            ${SWIG_PROJECT}_PhysicsLua51Unsafe
            COMMAND ${CMAKE_COMMAND} -E echo
                "Preparing Unrestricted(do not use with untrusted scripts) Mezzanine Physics "
                "library for Lua."
            COMMAND ${CMAKE_COMMAND} -E echo
                "Using special arguments for SWIG - ${Mezz_Lua51_Arguments}"
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -lua -includeall ${Mezz_Lua51_Arguments}
                -DSWIG_PHYSICS -DSWIG_UNSAFE -o Scripting/Lua51/scriptbindingphysicslua51unsafe.cpp
                Physics/physics.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        # MathTools/mathtools.h
        add_custom_target(
            ${SWIG_PROJECT}_MathToolsLua51
            COMMAND ${CMAKE_COMMAND} -E echo "Preparing Mezzanine MathTools library for Lua."
            COMMAND ${CMAKE_COMMAND} -E echo
                "Using special arguments for SWIG - ${Mezz_Lua51_Arguments}"
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -lua -includeall ${Mezz_Lua51_Arguments}
                -DSWIG_MATHTOOLS -o Scripting/Lua51/scriptbindingmathtoolslua51.cpp
                MathTools/mathtools.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        add_custom_target(
            ${SWIG_PROJECT}_MathToolsLua51Unsafe
            COMMAND ${CMAKE_COMMAND} -E echo
                "Preparing Unrestricted(do not use with untrusted scripts) Mezzanine MathTools "
                "library for Lua."
            COMMAND ${CMAKE_COMMAND} -E echo
                "Using special arguments for SWIG - ${Mezz_Lua51_Arguments}"
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -lua -includeall ${Mezz_Lua51_Arguments}
                -DSWIG_MATHTOOLS -DSWIG_UNSAFE -o
                Scripting/Lua51/scriptbindingmathtoolslua51unsafe.cpp MathTools/mathtools.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )

#        add_dependencies(${SWIG_PROJECT}
#                            ${SWIG_PROJECT}_MainLua51
#                            ${SWIG_PROJECT}_XMLLua51
#                            ${SWIG_PROJECT}_ThreadingLua51
#                            ${SWIG_PROJECT}_PhysicsLua51
#                            ${SWIG_PROJECT}_MathToolsLua51

#                            ${SWIG_PROJECT}_MainLua51Unsafe
#                            ${SWIG_PROJECT}_XMLLua51Unsafe
#                        )
#        #add_dependencies(${SWIG_PROJECT}_MainLua51 ${SWIG_PROJECT}_MainLua51Unsafe)
        add_dependencies(${SWIG_PROJECT}
            ${SWIG_PROJECT}_MainLua51           ${SWIG_PROJECT}_MainLua51Unsafe
            ${SWIG_PROJECT}_XMLLua51            ${SWIG_PROJECT}_XMLLua51Unsafe
            ${SWIG_PROJECT}_ThreadingLua51      ${SWIG_PROJECT}_ThreadingLua51Unsafe
            ${SWIG_PROJECT}_PhysicsLua51        ${SWIG_PROJECT}_PhysicsLua51Unsafe
            ${SWIG_PROJECT}_MathToolsLua51      ${SWIG_PROJECT}_MathToolsLua51Unsafe
        )
    endif(${Mezz_Lua51})

    if(${Mezz_Ruby})
        # mezzanine.h
        add_custom_target(
            ${SWIG_PROJECT}_MainRuby
            COMMAND ${CMAKE_COMMAND} -E echo
                "Preparing Unrestricted(do not use with untrusted scripts) Mezzanine Main library "
                "for Ruby."
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -ruby -includeall ${Mezz_Ruby_Arguments}
                -autorename -DSWIG_MAIN -DSWIG_UNSAFE -o
                Scripting/Ruby/scriptbindingmainrubyunsafe.cpp mezzanine.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        # XML/xml.h
        add_custom_target(
            ${SWIG_PROJECT}_XMLRuby
            COMMAND ${CMAKE_COMMAND} -E echo
                "Preparing Unrestricted(do not use with untrusted scripts) Mezzanine XML library "
                "for Ruby."
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -ruby -includeall ${Mezz_Ruby_Arguments}
                -autorename -DSWIG_XML -DSWIG_UNSAFE -o
                Scripting/Ruby/scriptbindingxmlrubyunsafe.cpp XML/xml.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        # Threading/dagframescheduler.h
        add_custom_target(
            ${SWIG_PROJECT}_ThreadingRuby
            COMMAND ${CMAKE_COMMAND} -E echo
                "Preparing Unrestricted(do not use with untrusted scripts) Mezzanine Threading "
                "library for Ruby."
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -ruby -includeall ${Mezz_Ruby_Arguments}
                -autorename -DSWIG_THREADING -DSWIG_UNSAFE -o
                Scripting/Ruby/scriptbindingthreadingrubyunsafe.cpp Threading/dagframescheduler.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        # Physics/physics.h
        add_custom_target(
            ${SWIG_PROJECT}_PhysicsRuby
            COMMAND ${CMAKE_COMMAND} -E echo
                "Preparing Unrestricted(do not use with untrusted scripts) Mezzanine Physics "
                "library for Ruby."
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -ruby -includeall ${Mezz_Ruby_Arguments}
                -autorename -DSWIG_PHYSICS -DSWIG_UNSAFE -o
                Scripting/Ruby/scriptbindingphysicsrubyunsafe.cpp Physics/physics.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        # MathTools/mathtools.h
        add_custom_target(
            ${SWIG_PROJECT}_MathToolsRuby
            COMMAND ${CMAKE_COMMAND} -E echo
                "Preparing Unrestricted(do not use with untrusted scripts) Mezzanine Physics "
                "library for Ruby."
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -ruby -includeall ${Mezz_Ruby_Arguments}
                -autorename -DSWIG_PHYSICS -DSWIG_UNSAFE -o
                Scripting/Ruby/scriptbindingphysicsrubyunsafe.cpp MathTools/mathtools.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        add_dependencies(${SWIG_PROJECT}
            ${SWIG_PROJECT}_MainRuby
            ${SWIG_PROJECT}_XMLRuby
            ${SWIG_PROJECT}_ThreadingRuby
            ${SWIG_PROJECT}_PhysicsRuby
            ${SWIG_PROJECT}_MathToolsRuby
        )
    endif(${Mezz_Ruby})
    # Add support for other languages here
endif(${Mezz_SWIG})

#######################################
# Exception Generation Configuration
option (Mezz_Exception "Refresh exceptions on build" OFF)


message(STATUS "Will exceptions be GENERATED: ${Mezz_Exception}")
if(${Mezz_Exception})
    # This names are likely the new style, but this is still poorly integrated.
    set(ExceptionProject "Exception_${PROJECT_NAME}")
    set(ExceptionProjectFolder "${${PROJECT_NAME}_LIBINCLUDE_DIR}common/exceptions/")
    set(ExceptionWorkingDirectory "${${PROJECT_NAME}_BINARY_DIR}ExceptionGeneration/")
    set(ExceptionResults "${${PROJECT_NAME}_SRC_DIR}exception.h")


    file(MAKE_DIRECTORY ${ExceptionWorkingDirectory})

    add_custom_target(
        ${ExceptionProject}
        COMMAND ${CMAKE_COMMAND} -E echo "Generating Exception Hiearchy from configuration."
        #COMMAND ${CMAKE_COMMAND} -E remove ${ExceptionResults}
        #COMMAND ${CMAKE_COMMAND} -E touch ${ExceptionResults}

        COMMAND ${CMAKE_COMMAND} ${ExceptionWorkingDirectory} ${ExceptionProjectFolder}
        COMMAND ${CMAKE_COMMAND} -E echo "Storing Exceptions at ${ExceptionResults}."
        COMMAND ${CMAKE_COMMAND} -E copy ${ExceptionWorkingDirectory}GeneratedExceptions.h
                ${ExceptionResults}
        WORKING_DIRECTORY ${ExceptionWorkingDirectory}
        VERBATIM
        SOURCES ${ExceptionProjectFolder}CMakeLists.txt
                ${ExceptionProjectFolder}enum.in
                ${ExceptionProjectFolder}enumlast.in
                ${ExceptionProjectFolder}exception.in
                ${ExceptionProjectFolder}exceptionfile.in
                ${ExceptionResults}
    )
endif(${Mezz_Exception})

#######################################
# Library Link Dependencies
add_dependencies( ${PROJECT_NAME} ${Bullet_LibraryNames}  ${SDLLibraryNames} ${SoundLibraryNames}
                    "lua" ${SWIG_PROJECT} ${ExceptionProject})

if(${Mezz_Exception})
    add_dependencies( ${PROJECT_NAME} ${ExceptionProject})
endif(${Mezz_Exception})

if(${Mezz_SWIG})
    add_dependencies("lua" ${SWIG_PROJECT})
    #add_dependencies(${RubyProject} ${SWIG_PROJECT})
    if(${Mezz_Exception})
        add_dependencies(${SWIG_PROJECT} ${ExceptionProject})
    endif(${Mezz_Exception})
endif(${Mezz_SWIG})

find_package(Threads)

# Link to all of the Mezzanine Dependencies
target_link_libraries( ${PROJECT_NAME}
    ${NetworkLibrary}
    ${OgreLibraryAllNames}
    ${LuaLinkName}
    ${RubyLinkName}
    ${Bullet_LibraryNames}
    ${SDLLibraryNames}
    ${SoundLibraryNames}
    ${CMAKE_THREAD_LIBS_INIT}
)

###############################################################################
# Build Time File copying

#######################################
# Copy Common Files at Build Time

# Licenses
CopyFilesDuringBuild( common/BulletZLibLicense.txt data/common/BulletZLibLicense.txt )
CopyFilesDuringBuild( common/gpl-2.0.txt data/common/gpl-2.0.txt )
CopyFilesDuringBuild( common/gpl-3.0.txt data/common/gpl-3.0.txt )
CopyFilesDuringBuild( common/lgpl-2.1.txt data/common/lgpl-2.1.txt )
CopyFilesDuringBuild( common/lgpl-3.0.txt data/common/lgpl-3.0.txt )
CopyFilesDuringBuild( common/Licenses.txt data/common/Licenses.txt )
CopyFilesDuringBuild( common/Lua-MIT.txt data/common/Lua-MIT.txt )
CopyFilesDuringBuild( common/Ogre-MITLicense.txt data/common/Ogre-MITLicense.txt )
CopyFilesDuringBuild( common/pstdint-BSD.txt data/common/pstdint-BSD.txt )
CopyFilesDuringBuild( common/PugiXML-MITLicense.txt data/common/PugiXML-MITLicense.txt )
CopyFilesDuringBuild( common/ReneNyffenegger-MIT.txt data/common/ReneNyffenegger-MIT.txt )
CopyFilesDuringBuild( common/SDL-MITLicense.txt data/common/SDL-MITLicense.txt )
CopyFilesDuringBuild( common/XiphCopyrightNotice.txt data/common/XiphCopyrightNotice.txt )
CopyFilesDuringBuild( common/zlib-libpngLicense.txt data/common/zlib-libpngLicense.txt )
CopyFilesDuringBuild( common/zlib-zliblicense.txt data/common/zlib-zliblicense.txt )


# Ogre
#CopyFilesDuringBuild( ${BuildPlatform}/ogre/${OgrePluginRenderGL}
#       data/${BuildPlatform}/ogre/${OgrePluginRenderGL} )
#CopyFilesDuringBuild( ${BuildPlatform}/ogre/${OgrePluginParticle}
#       data/${BuildPlatform}/ogre/${OgrePluginParticle} )
#CopyFilesDuringBuild( ${BuildPlatform}/ogre/${OgrePluginCGPM}
#       data/${BuildPlatform}/ogre/${OgrePluginCGPM} )
#CopyFilesDuringBuild( ${BuildPlatform}/ogre/${OgreLibFileName} ${OgreLibFileName} )
#CopyFilesDuringBuild( ${BuildPlatform}/plugins${OgresD}.cfg data/${BuildPlatform}/plugins.cfg )

# We should use the new variables to copy the ogre files and Mezzanine library in the common code.

#######################################
# Windows File Copy Specific Items
if(${WIN32})
        message ( STATUS "${PROJECT_NAME} - Begin Windows32/64 Specific File Copy Process")

        # remove lib prefix from DLL outputs
        set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

        #CopyFilesDuringBuild( ${BuildPlatform}/ogre/cg${OgresD}.dll
        #       data/${BuildPlatform}/ogre/cg${OgresD}.dll )
        #CopyFilesDuringBuild( ${BuildPlatform}/ogre/libboost_thread-mgw45-mt-1_44.dll
        #       libboost_thread-mgw45-mt-1_44.dll ) #can this be statically linked
        #CopyFilesDuringBuild( ${BuildPlatform}/openal/OpenAL32.dll OpenAL32.dll )
        #CopyFilesDuringBuild( ${BuildPlatform}/openal/OpenAL32.lib OpenAL32.lib )


        target_link_libraries( ${PROJECT_NAME}
                Winmm
                Imm32
                Version
        )

        #CopyFilesDuringBuild( ${BuildPlatform}/openal/wrap_oal.dll wrap_oal.dll )
        message ( STATUS "${PROJECT_NAME} - End Windows32/64 Specific File Copy Process")
endif(${WIN32})

#######################################
# Linux File Copy Specific Items
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        message ( STATUS "${PROJECT_NAME} - Begin Linux Specific File Copy Process")
        CopyFilesDuringBuild( ${BuildPlatform}/GLX_icon.png data/${BuildPlatform}/GLX_icon.png )
        message ( STATUS "${PROJECT_NAME} - End Linux Specific File Copy Process")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


#######################################
# Mac File Copy Specific Items
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        message ( STATUS "${PROJECT_NAME} - Begin Apple Specific File Copy Process")
        message ( STATUS "${PROJECT_NAME} - End Apple Specific File Copy Process")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")


###############################################################################
# The End

message ( "${PROJECT_NAME} - End" )

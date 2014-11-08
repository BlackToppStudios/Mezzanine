# © Copyright 2010 - 2014 BlackTopp Studios Inc.
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

add_definitions( ${CommonBuildFlags} )
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
        "${${PROJECT_NAME}_SRC_DIR}actor.h"
        "${${PROJECT_NAME}_SRC_DIR}actormanager.h"
        "${${PROJECT_NAME}_SRC_DIR}areaeffect.h"
        "${${PROJECT_NAME}_SRC_DIR}areaeffectmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}attachable.h"
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
        "${${PROJECT_NAME}_SRC_DIR}axisalignedbox.h"
        "${${PROJECT_NAME}_SRC_DIR}binarybuffer.h"
        "${${PROJECT_NAME}_SRC_DIR}cameracontroller.h"
        "${${PROJECT_NAME}_SRC_DIR}circle.h"
        "${${PROJECT_NAME}_SRC_DIR}colourvalue.h"
        "${${PROJECT_NAME}_SRC_DIR}countedptr.h"
        "${${PROJECT_NAME}_SRC_DIR}crossplatform.h"
        "${${PROJECT_NAME}_SRC_DIR}crossplatformexport.h"
        "${${PROJECT_NAME}_SRC_DIR}cubicspline.h"
        "${${PROJECT_NAME}_SRC_DIR}datatypes.h"
        "${${PROJECT_NAME}_SRC_DIR}debris.h"
        "${${PROJECT_NAME}_SRC_DIR}debrismanager.h"
        "${${PROJECT_NAME}_SRC_DIR}entresol.h"
        "${${PROJECT_NAME}_SRC_DIR}entresolmanager.h"
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
        "${${PROJECT_NAME}_SRC_DIR}Graphics/graphics.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/billboard.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/billboardsetproxy.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/bone.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/cameramanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/cameraproxy.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/entityproxy.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/gamewindow.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/graphicsenumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/graphicsmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/image.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/lightproxy.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/material.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/mesh.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/meshmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/particleaffector.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/particleemitter.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/particlesystemproxy.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/pass.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/renderableproxy.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/resolution.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/scenemanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/skeleton.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/submesh.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/technique.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/texture.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/texturemanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/viewport.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/windowsettings.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/procedural.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/booleanmeshgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/boxcornergenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/boxgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/capsulegenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/conegenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/curvetrack.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/cylindergenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/icospheregenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/meshgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/multishape.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/noise.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/path.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/planegenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/proceduraldatatypes.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/proceduralenumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/proceduralutilities.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/roundedboxgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/shape.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/spheregenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/springgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/texturebuffer.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/texturegenerators.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/texturemodifiers.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/torusgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/torusknotgenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/trianglebuffer.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/triangulator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/tubegenerator.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/abnormalsmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/alphamodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/alphamaskmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/blitmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/blurmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/channelmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/circlemodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/coloursmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/combinemodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/convolutionmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/crackmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/cycloidmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/dilatemodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/distortmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/edgedetectionmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/ellipsemodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/flipmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/glowmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/invertmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/jittermodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/lerpmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/lightbakemodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/lookupmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/normalsmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/oilpaintmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/randompixelsmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/rectanglemodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/rotationzoommodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/segmentmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/sharpenmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/texttexturemodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/texturemodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/thresholdmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/vortexmodifier.h"
        "${${PROJECT_NAME}_SRC_DIR}gravityfield.h"
        "${${PROJECT_NAME}_SRC_DIR}gravitywell.h"
        "${${PROJECT_NAME}_SRC_DIR}hashedstring.h"
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
        "${${PROJECT_NAME}_SRC_DIR}Internal/decompinterface.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/iostreamwrapper.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/meshtools.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/motionstate.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/murmurhash3.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/ogredatastreambuf.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}interpolator.h"
        "${${PROJECT_NAME}_SRC_DIR}linegroup.h"
        "${${PROJECT_NAME}_SRC_DIR}linesegment.h"
        "${${PROJECT_NAME}_SRC_DIR}macros.h"
        "${${PROJECT_NAME}_SRC_DIR}managedptr.h"
        "${${PROJECT_NAME}_SRC_DIR}managerbase.h"
        "${${PROJECT_NAME}_SRC_DIR}managerfactory.h"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/arithmetic.h"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/constants.h"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/intersections.h"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/mathtools.h"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/trigonometry.h"
        "${${PROJECT_NAME}_SRC_DIR}matrix3x3.h"
        "${${PROJECT_NAME}_SRC_DIR}matrix4x4.h"
        "${${PROJECT_NAME}_SRC_DIR}meshterrain.h"
        "${${PROJECT_NAME}_SRC_DIR}mezzanine.h"
        "${${PROJECT_NAME}_SRC_DIR}network.h"
        "${${PROJECT_NAME}_SRC_DIR}networkmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/ipaddress.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/packet.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/socket.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/systemaddress.h"
        "${${PROJECT_NAME}_SRC_DIR}objectsettings.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/physics.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/physicsenumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/physicsmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/boxcollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/capsulecollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collidablepair.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collidableproxy.h"
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
        "${${PROJECT_NAME}_SRC_DIR}plane.h"
        "${${PROJECT_NAME}_SRC_DIR}pstdint.h"
        "${${PROJECT_NAME}_SRC_DIR}quaternion.h"
        "${${PROJECT_NAME}_SRC_DIR}ray.h"
        "${${PROJECT_NAME}_SRC_DIR}rayquerytool.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/resource.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/resourceenumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/resourcemanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/archive.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/archivestream.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/asset.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/assetgroup.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/assethandler.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/assetid.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/datastream.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/filestream.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/memorystream.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/resourceutilities.h"
        "${${PROJECT_NAME}_SRC_DIR}rigiddebris.h"
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
        "${${PROJECT_NAME}_SRC_DIR}singleton.h"
        "${${PROJECT_NAME}_SRC_DIR}serialization.h"
        "${${PROJECT_NAME}_SRC_DIR}softdebris.h"
        "${${PROJECT_NAME}_SRC_DIR}sphere.h"
        "${${PROJECT_NAME}_SRC_DIR}streamlogging.cpp"
        "${${PROJECT_NAME}_SRC_DIR}stringtool.h"
        "${${PROJECT_NAME}_SRC_DIR}swig.h"
        "${${PROJECT_NAME}_SRC_DIR}terrainbase.h"
        "${${PROJECT_NAME}_SRC_DIR}terrainmanager.h"
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
        "${${PROJECT_NAME}_SRC_DIR}timer.h"
        "${${PROJECT_NAME}_SRC_DIR}track.h"
        "${${PROJECT_NAME}_SRC_DIR}trackiterator.h"
        "${${PROJECT_NAME}_SRC_DIR}transform.h"
        "${${PROJECT_NAME}_SRC_DIR}transformableobject.h"
        "${${PROJECT_NAME}_SRC_DIR}triangle.h"
        "${${PROJECT_NAME}_SRC_DIR}trie.h"
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
        "${${PROJECT_NAME}_SRC_DIR}unicode.h"
        "${${PROJECT_NAME}_SRC_DIR}vector2.h"
        "${${PROJECT_NAME}_SRC_DIR}vector3.h"
        "${${PROJECT_NAME}_SRC_DIR}world.h"
        "${${PROJECT_NAME}_SRC_DIR}worldmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}worldobject.h"
        "${${PROJECT_NAME}_SRC_DIR}worldproxy.h"
        "${${PROJECT_NAME}_SRC_DIR}worldtrigger.h"
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

        "${${PROJECT_NAME}_SRC_DIR}actor.cpp"
        "${${PROJECT_NAME}_SRC_DIR}actormanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}areaeffect.cpp"
        "${${PROJECT_NAME}_SRC_DIR}areaeffectmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}attachable.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/audiomanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/musicplayer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/playlist.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/rawdecoder.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/soundscapemanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/vorbisdecoder.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/wavdecoder.cpp"
        "${${PROJECT_NAME}_SRC_DIR}axisalignedbox.cpp"
        "${${PROJECT_NAME}_SRC_DIR}binarybuffer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}cameracontroller.cpp"
        "${${PROJECT_NAME}_SRC_DIR}circle.cpp"
        "${${PROJECT_NAME}_SRC_DIR}colourvalue.cpp"
        "${${PROJECT_NAME}_SRC_DIR}crossplatform.cpp"
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
        "${${PROJECT_NAME}_SRC_DIR}Graphics/billboard.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/billboardsetproxy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/bone.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/cameramanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/cameraproxy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/entityproxy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/gamewindow.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/graphicsmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/image.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/lightproxy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/material.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/mesh.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/meshmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/particleaffector.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/particleemitter.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/particlesystemproxy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/pass.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/renderableproxy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/scenemanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/skeleton.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/submesh.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/technique.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/texture.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/texturemanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/viewport.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/booleanmeshgenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/boxcornergenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/boxgenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/capsulegenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/conegenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/curvetrack.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/cylindergenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/icospheregenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/multishape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/noise.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/path.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/planegenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/proceduralutilities.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/roundedboxgenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/shape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/spheregenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/springgenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/texturebuffer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/texturegenerators.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/texturemodifiers.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/torusgenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/torusknotgenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/trianglebuffer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/triangulator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/tubegenerator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/abnormalsmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/alphamodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/alphamaskmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/blitmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/blurmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/channelmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/circlemodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/coloursmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/combinemodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/convolutionmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/crackmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/cycloidmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/dilatemodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/distortmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/edgedetectionmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/ellipsemodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/flipmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/glowmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/invertmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/jittermodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/lerpmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/lightbakemodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/lookupmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/normalsmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/oilpaintmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/randompixelsmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/rectanglemodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/rotationzoommodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/segmentmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/sharpenmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/texttexturemodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/thresholdmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/Procedural/Texture/vortexmodifier.cpp"
        "${${PROJECT_NAME}_SRC_DIR}gravityfield.cpp"
        "${${PROJECT_NAME}_SRC_DIR}gravitywell.cpp"
        "${${PROJECT_NAME}_SRC_DIR}hashedstring.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/inputmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/buttondevice.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/controller.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/device.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/keyboard.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/metacode.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/metacodekey.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/mouse.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/sequencecontainer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/decompinterface.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/iostreamwrapper.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/meshtools.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/motionstate.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/murmurhash3.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/ogredatastreambuf.cpp"
        "${${PROJECT_NAME}_SRC_DIR}interpolator.cpp"
        "${${PROJECT_NAME}_SRC_DIR}linegroup.cpp"
        "${${PROJECT_NAME}_SRC_DIR}linesegment.cpp"
        "${${PROJECT_NAME}_SRC_DIR}managedptr.cpp"
        "${${PROJECT_NAME}_SRC_DIR}managerbase.cpp"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/arithmetic.cpp"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/constants.cpp"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/intersections.cpp"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/mathtools.cpp"
        "${${PROJECT_NAME}_SRC_DIR}MathTools/trigonometry.cpp"
        "${${PROJECT_NAME}_SRC_DIR}matrix3x3.cpp"
        "${${PROJECT_NAME}_SRC_DIR}matrix4x4.cpp"
        "${${PROJECT_NAME}_SRC_DIR}meshterrain.cpp"
        "${${PROJECT_NAME}_SRC_DIR}networkmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/ipaddress.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/packet.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/socket.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/systemaddress.cpp"
        "${${PROJECT_NAME}_SRC_DIR}objectsettings.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/physicsmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/boxcollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/capsulecollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collidablepair.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collidableproxy.cpp"
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
        "${${PROJECT_NAME}_SRC_DIR}plane.cpp"
        "${${PROJECT_NAME}_SRC_DIR}quaternion.cpp"
        "${${PROJECT_NAME}_SRC_DIR}ray.cpp"
        "${${PROJECT_NAME}_SRC_DIR}rayquerytool.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/resourcemanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/archive.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/archivestream.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/asset.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/assetgroup.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/assethandler.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/datastream.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/filestream.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/memorystream.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/resourceutilities.cpp"
        "${${PROJECT_NAME}_SRC_DIR}rigiddebris.cpp"
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
        "${${PROJECT_NAME}_SRC_DIR}serialization.cpp"
        "${${PROJECT_NAME}_SRC_DIR}softdebris.cpp"
        "${${PROJECT_NAME}_SRC_DIR}sphere.cpp"
        "${${PROJECT_NAME}_SRC_DIR}streamlogging.cpp"
        "${${PROJECT_NAME}_SRC_DIR}stringtool.cpp"
        "${${PROJECT_NAME}_SRC_DIR}terrainbase.cpp"
        "${${PROJECT_NAME}_SRC_DIR}terrainmanager.cpp"
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
        "${${PROJECT_NAME}_SRC_DIR}timer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}track.cpp"
        "${${PROJECT_NAME}_SRC_DIR}transform.cpp"
        "${${PROJECT_NAME}_SRC_DIR}transformableobject.cpp"
        "${${PROJECT_NAME}_SRC_DIR}triangle.cpp"
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
        "${${PROJECT_NAME}_SRC_DIR}unicode.cpp"
        "${${PROJECT_NAME}_SRC_DIR}vector2.cpp"
        "${${PROJECT_NAME}_SRC_DIR}vector3.cpp"
        "${${PROJECT_NAME}_SRC_DIR}world.cpp"
        "${${PROJECT_NAME}_SRC_DIR}worldmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}worldobject.cpp"
        "${${PROJECT_NAME}_SRC_DIR}worldproxy.cpp"
        "${${PROJECT_NAME}_SRC_DIR}worldtrigger.cpp"
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
        message(FATAL_ERROR "A provider for the Audio subsystem has already been set.  Only one provider may be selected.")
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
# Encoding support configuration

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

option(Mezz_Audio_EnableVorbisEncoding "Enable the encoding and decoding of .ogg (vorbis) encoded data." ON)
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

###############################################################################
# System Build Items
#######################################
# All Changes that could be system specific should be defined in these variables
# These will set sane defaults (Where possible), but specific platforms should
# set a value of their own.

set(LIBTYPE "STATIC") # OpenAL
file(MAKE_DIRECTORY "${ProjectBinaryDir}include") # To not interfere with SDL build process
set(OGRE_STATIC TRUE CACHE BOOL "Forcing static build for Mezzanine build." FORCE)
set(OGRE_SET_USE_BOOST FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_USE_BOOST FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_CONFIG_THREADS 0 CACHE INT "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_CONFIG_THREAD_PROVIDER "" CACHE STRING "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_BUILD_PLUGIN_CG FALSE CACHE BOOL "Forcing Ogre to disable CG, because it is disgusting." FORCE)
set(OGRE_CONFIG_ALLOCATOR 1 CACHE STRING "Forcing static build for Mezzanine build." FORCE)


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
    #set_source_files_properties("${${PROJECT_NAME}_SRC_DIR}Graphics/gamewindow.cpp" PROPERTIES COMPILE_FLAGS "-x objective-c++")
    set_property(SOURCE "${${PROJECT_NAME}_SRC_DIR}Graphics/gamewindow.cpp" PROPERTY COMPILE_FLAGS "-x objective-c++")
    message ( STATUS "${PROJECT_NAME} - End Apple Specific Build Process")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

###############################################################################
# Common Library Configuration items

include_directories( ${${PROJECT_NAME}_SRC_DIR}
        "${${PROJECT_NAME}_LIBINCLUDE_DIR}common/bulletsrc/bulletsvnsrc/src"
        "${${PROJECT_NAME}_LIBINCLUDE_DIR}common/bulletsrc/bulletsvnsrc/src/LinearMath"
        "${${PROJECT_NAME}_LIBINCLUDE_DIR}common/bulletsrc/bulletsvnsrc/Extras/ConvexDecomposition"
        "${${PROJECT_NAME}_LIBINCLUDE_DIR}common/bulletsrc/bulletsvnsrc/Extras/Serialize/BulletWorldImporter"

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
# This is necessary because of static linking and the lack of foresight on the part of the people that wrote those rendersystems.
# It's a hack and makes me a sad panda.
###############################################################################
# Prepare the actual Build
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_sources} )

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
    #/// swig2.0 -c++ -v -Wall -lua -importall -includeall -o Scripting/Lua51/scriptbindinglua51.cpp mezzanine.h
    #/// swig2.0 -c++ -v -Wall -lua -importall -includeall -DSWIG_UNSAFE -o Scripting/Lua51/scriptbindinglua51unsafe.cpp mezzanine.h

    set(SWIG_PROJECT "SWIG_${PROJECT_NAME}")
    add_library(${SWIG_PROJECT} SHARED "${${PROJECT_NAME}_SRC_DIR}blank.cpp" )

    if(${Mezz_Lua51})
        # mezzanine.h
        add_custom_command(
            TARGET ${SWIG_PROJECT}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Preparing Mezzanine Main library for Lua."
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -lua -includeall ${Mezz_Lua51_Arguments} -DSWIG_MAIN -o Scripting/Lua51/scriptbindingmainlua51.cpp mezzanine.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        add_custom_command(
            TARGET ${SWIG_PROJECT}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Preparing Unrestricted(do not use with untrusted scripts) Mezzanine Main library for Lua."
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -lua -includeall ${Mezz_Lua51_Arguments} -DSWIG_MAIN -DSWIG_UNSAFE -o Scripting/Lua51/scriptbindingmainlua51unsafe.cpp mezzanine.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        # XML/xml.h
        add_custom_command(
            TARGET ${SWIG_PROJECT}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Preparing Mezzanine XML library for Lua."
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -lua -includeall ${Mezz_Lua51_Arguments} -DSWIG_XML -o Scripting/Lua51/scriptbindingxmllua51.cpp XML/xml.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        add_custom_command(
            TARGET ${SWIG_PROJECT}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Preparing Unrestricted(do not use with untrusted scripts) Mezzanine XML library for Lua."
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -lua -includeall ${Mezz_Lua51_Arguments} -DSWIG_XML -DSWIG_UNSAFE -o Scripting/Lua51/scriptbindingxmllua51unsafe.cpp XML/xml.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        # Threading/dagframescheduler.h
        add_custom_command(
            TARGET ${SWIG_PROJECT}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Preparing Mezzanine Threading library for Lua."
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -lua -includeall ${Mezz_Lua51_Arguments} -DSWIG_THREADING -o Scripting/Lua51/scriptbindingthreadinglua51.cpp Threading/dagframescheduler.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        add_custom_command(
            TARGET ${SWIG_PROJECT}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Preparing Unrestricted(do not use with untrusted scripts) Mezzanine Threading library for Lua."
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -lua -includeall ${Mezz_Lua51_Arguments} -DSWIG_THREADING -DSWIG_UNSAFE -o Scripting/Lua51/scriptbindingthreadinglua51unsafe.cpp Threading/dagframescheduler.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        # Physics/physics.h
        add_custom_command(
            TARGET ${SWIG_PROJECT}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Preparing Mezzanine Physics library for Lua."
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -lua -includeall ${Mezz_Lua51_Arguments} -DSWIG_PHYSICS -o Scripting/Lua51/scriptbindingphysicslua51.cpp Physics/physics.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        add_custom_command(
            TARGET ${SWIG_PROJECT}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Preparing Unrestricted(do not use with untrusted scripts) Mezzanine Physics library for Lua."
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -lua -includeall ${Mezz_Lua51_Arguments} -DSWIG_PHYSICS -DSWIG_UNSAFE -o Scripting/Lua51/scriptbindingphysicslua51unsafe.cpp Physics/physics.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        # MathTools/mathtools.h
        add_custom_command(
            TARGET ${SWIG_PROJECT}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Preparing Mezzanine MathTools library for Lua."
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -lua -includeall ${Mezz_Lua51_Arguments} -DSWIG_MATHTOOLS -o Scripting/Lua51/scriptbindingmathtoolslua51.cpp MathTools/mathtools.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        add_custom_command(
            TARGET ${SWIG_PROJECT}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Preparing Unrestricted(do not use with untrusted scripts) Mezzanine MathTools library for Lua."
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -lua -includeall ${Mezz_Lua51_Arguments} -DSWIG_MATHTOOLS -DSWIG_UNSAFE -o Scripting/Lua51/scriptbindingmathtoolslua51unsafe.cpp MathTools/mathtools.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
    endif(${Mezz_Lua51})

    if(${Mezz_Ruby})
        # mezzanine.h
        add_custom_command(
            TARGET ${SWIG_PROJECT}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Preparing Unrestricted(do not use with untrusted scripts) Mezzanine Main library for Ruby."
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -ruby -includeall ${Mezz_Ruby_Arguments} -autorename -DSWIG_MAIN -DSWIG_UNSAFE -o Scripting/Ruby/scriptbindingmainrubyunsafe.cpp mezzanine.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        # XML/xml.h
        add_custom_command(
            TARGET ${SWIG_PROJECT}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Preparing Unrestricted(do not use with untrusted scripts) Mezzanine XML library for Ruby."
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -ruby -includeall ${Mezz_Ruby_Arguments} -autorename -DSWIG_XML -DSWIG_UNSAFE -o Scripting/Ruby/scriptbindingxmlrubyunsafe.cpp XML/xml.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        # Threading/dagframescheduler.h
        add_custom_command(
            TARGET ${SWIG_PROJECT}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Preparing Unrestricted(do not use with untrusted scripts) Mezzanine Threading library for Ruby."
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -ruby -includeall ${Mezz_Ruby_Arguments} -autorename -DSWIG_THREADING -DSWIG_UNSAFE -o Scripting/Ruby/scriptbindingthreadingrubyunsafe.cpp Threading/dagframescheduler.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
        # Physics/physics.h
        add_custom_command(
            TARGET ${SWIG_PROJECT}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "Preparing Unrestricted(do not use with untrusted scripts) Mezzanine Physics library for Ruby."
            COMMAND ${SWIG_EXECUTABLE} -c++ -v -Wall -ruby -includeall ${Mezz_Ruby_Arguments} -autorename -DSWIG_PHYSICS -DSWIG_UNSAFE -o Scripting/Ruby/scriptbindingphysicsrubyunsafe.cpp Physics/physics.h
            WORKING_DIRECTORY ${${PROJECT_NAME}_SRC_DIR}
            VERBATIM
        )
    endif(${Mezz_Ruby})
    # Add support for other languagews here
endif(${Mezz_SWIG})

#######################################
# Library Link Dependencies
add_dependencies( ${PROJECT_NAME} ${OgreDepName} ${Bullet_LibraryNames} ${LuaProject} ${RubyProject} ${SDLLibraryNames} ${SoundLibraryNames} ${SWIG_PROJECT} )

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
CopyFilesDuringBuild( common/cAudio-ZLIBLicense.txt data/common/cAudio-ZLIBLicense.txt )


# Ogre
#CopyFilesDuringBuild( ${BuildPlatform}/ogre/${OgrePluginRenderGL} data/${BuildPlatform}/ogre/${OgrePluginRenderGL} )
#CopyFilesDuringBuild( ${BuildPlatform}/ogre/${OgrePluginParticle} data/${BuildPlatform}/ogre/${OgrePluginParticle} )
#CopyFilesDuringBuild( ${BuildPlatform}/ogre/${OgrePluginCGPM} data/${BuildPlatform}/ogre/${OgrePluginCGPM} )
#CopyFilesDuringBuild( ${BuildPlatform}/ogre/${OgreLibFileName} ${OgreLibFileName} )
#CopyFilesDuringBuild( ${BuildPlatform}/plugins${OgresD}.cfg data/${BuildPlatform}/plugins.cfg )

# We should use the new variables to copy the ogre files and Mezzanine library in the common code.

#######################################
# Windows File Copy Specific Items
if(${WIN32})
        message ( STATUS "${PROJECT_NAME} - Begin Windows32/64 Specific File Copy Process")
        set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")                                         # remove lib prefix from DLL outputs
        #CopyFilesDuringBuild( ${BuildPlatform}/ogre/cg${OgresD}.dll data/${BuildPlatform}/ogre/cg${OgresD}.dll )
        #CopyFilesDuringBuild( ${BuildPlatform}/ogre/libboost_thread-mgw45-mt-1_44.dll libboost_thread-mgw45-mt-1_44.dll ) #can this be statically linked
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

# © Copyright 2010 - 2011 BlackTopp Studios Inc.
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
        "${${PROJECT_NAME}_SRC_DIR}actorbase.h"
        "${${PROJECT_NAME}_SRC_DIR}actorcharacter.h"
        "${${PROJECT_NAME}_SRC_DIR}actormanager.h"
        "${${PROJECT_NAME}_SRC_DIR}actorphysicssettings.h"
        "${${PROJECT_NAME}_SRC_DIR}actorrigid.h"
        "${${PROJECT_NAME}_SRC_DIR}actorserializer.h"
        "${${PROJECT_NAME}_SRC_DIR}actorsoft.h"
        "${${PROJECT_NAME}_SRC_DIR}any.h"
        "${${PROJECT_NAME}_SRC_DIR}areaeffect.h"
        "${${PROJECT_NAME}_SRC_DIR}attachable.h"
        "${${PROJECT_NAME}_SRC_DIR}audio.h"
        "${${PROJECT_NAME}_SRC_DIR}audiomanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/audioenumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/listener.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/musicplayer.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/playlist.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/sound.h"
        "${${PROJECT_NAME}_SRC_DIR}Audio/soundset.h"
        "${${PROJECT_NAME}_SRC_DIR}binarytool.h"
        "${${PROJECT_NAME}_SRC_DIR}camera.h"
        "${${PROJECT_NAME}_SRC_DIR}cameracontroller.h"
        "${${PROJECT_NAME}_SRC_DIR}cameramanager.h"
        "${${PROJECT_NAME}_SRC_DIR}collisionshapemanager.h"
        "${${PROJECT_NAME}_SRC_DIR}colourvalue.h"
        "${${PROJECT_NAME}_SRC_DIR}crossplatform.h"
        "${${PROJECT_NAME}_SRC_DIR}crossplatformexport.h"
        "${${PROJECT_NAME}_SRC_DIR}datatypes.h"
        "${${PROJECT_NAME}_SRC_DIR}entity.h"
        "${${PROJECT_NAME}_SRC_DIR}entresol.h"
        "${${PROJECT_NAME}_SRC_DIR}enumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}eventbase.h"
        "${${PROJECT_NAME}_SRC_DIR}eventcollision.h"
        "${${PROJECT_NAME}_SRC_DIR}eventgamewindow.h"
        "${${PROJECT_NAME}_SRC_DIR}eventmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}eventquit.h"
        "${${PROJECT_NAME}_SRC_DIR}eventrendertime.h"
        "${${PROJECT_NAME}_SRC_DIR}eventuserinput.h"
        "${${PROJECT_NAME}_SRC_DIR}exception.h"
        "${${PROJECT_NAME}_SRC_DIR}extendedtimer.h"
        "${${PROJECT_NAME}_SRC_DIR}graphics.h"
        "${${PROJECT_NAME}_SRC_DIR}graphicsmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/gamewindow.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/graphicsenumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/material.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/mesh.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/pass.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/submesh.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/technique.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/texture.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/viewport.h"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/windowsettings.h"
        "${${PROJECT_NAME}_SRC_DIR}input.h"
        "${${PROJECT_NAME}_SRC_DIR}inputmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Input/inputenumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}Input/controller.h"
        "${${PROJECT_NAME}_SRC_DIR}Input/device.h"
        "${${PROJECT_NAME}_SRC_DIR}Input/keyboard.h"
        "${${PROJECT_NAME}_SRC_DIR}Input/metacode.h"
        "${${PROJECT_NAME}_SRC_DIR}Input/mouse.h"
        "${${PROJECT_NAME}_SRC_DIR}Internal/bulletfile.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/bulletfilemanager.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/decompinterface.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/meshtools.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/motionstate.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/ogredatastreambuf.h.cpp"
        "${${PROJECT_NAME}_SRC_DIR}light.h"
        "${${PROJECT_NAME}_SRC_DIR}linegroup.h"
        "${${PROJECT_NAME}_SRC_DIR}managerbase.h"
        "${${PROJECT_NAME}_SRC_DIR}managerfactory.h"
        "${${PROJECT_NAME}_SRC_DIR}mathtool.h"
        "${${PROJECT_NAME}_SRC_DIR}matrix3x3.h"
        "${${PROJECT_NAME}_SRC_DIR}matrix4x4.h"
        "${${PROJECT_NAME}_SRC_DIR}meshmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}meshterrain.h"
        "${${PROJECT_NAME}_SRC_DIR}mezzanine.h"
        "${${PROJECT_NAME}_SRC_DIR}network.h"
        "${${PROJECT_NAME}_SRC_DIR}networkmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/ipaddress.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/packet.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/socket.h"
        "${${PROJECT_NAME}_SRC_DIR}Network/systemaddress.h"
        "${${PROJECT_NAME}_SRC_DIR}objectpair.h"
        "${${PROJECT_NAME}_SRC_DIR}objectreference.h"
        "${${PROJECT_NAME}_SRC_DIR}objectsettings.h"
        "${${PROJECT_NAME}_SRC_DIR}particleaffector.h"
        "${${PROJECT_NAME}_SRC_DIR}particleeffect.h"
        "${${PROJECT_NAME}_SRC_DIR}particleemitter.h"
        "${${PROJECT_NAME}_SRC_DIR}physics.h"
        "${${PROJECT_NAME}_SRC_DIR}physicsmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/physicsenumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/actorsoftcollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/boxcollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/capsulecollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collision.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/compoundcollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/conecollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/conetwistconstraint.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/constraint.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/convexhullcollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/cylindercollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/dualtransformconstraint.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/dynamicmeshcollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/fieldcollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/generic6dofconstraint.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/generic6dofspringconstraint.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/heightfieldcollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/hinge2constraint.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/hingeconstraint.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/meshcollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/multispherecollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/planecollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/point2pointconstraint.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/primitivecollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/sliderconstraint.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/spherecollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/staticmeshcollisionshape.h"
        "${${PROJECT_NAME}_SRC_DIR}Physics/universalconstraint.h"
        "${${PROJECT_NAME}_SRC_DIR}plane.h"
        "${${PROJECT_NAME}_SRC_DIR}quaternion.h"
        "${${PROJECT_NAME}_SRC_DIR}ray.h"
        "${${PROJECT_NAME}_SRC_DIR}rayquerytool.h"
        "${${PROJECT_NAME}_SRC_DIR}resource.h"
        "${${PROJECT_NAME}_SRC_DIR}resourcemanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/resourceenumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/archive.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/asset.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/assetgroup.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/assethandler.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/datastream.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/inputstream.h"
        "${${PROJECT_NAME}_SRC_DIR}Resource/textsettingsfile.h"
        "${${PROJECT_NAME}_SRC_DIR}scenemanager.h"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/scriptingmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}scripting.h"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/scriptargument.h"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/script.h"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/lua51script.h"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/lua51scriptingengine.h"
        "${${PROJECT_NAME}_SRC_DIR}simpletimer.h"
        "${${PROJECT_NAME}_SRC_DIR}singleton.h"
        "${${PROJECT_NAME}_SRC_DIR}serialization.h"
        "${${PROJECT_NAME}_SRC_DIR}smartptr.h"
        "${${PROJECT_NAME}_SRC_DIR}stringtool.h"
        "${${PROJECT_NAME}_SRC_DIR}swig.h"
        "${${PROJECT_NAME}_SRC_DIR}terrainbase.h"
        "${${PROJECT_NAME}_SRC_DIR}terrainmanager.h"
        "${${PROJECT_NAME}_SRC_DIR}timer.h"
        "${${PROJECT_NAME}_SRC_DIR}timermanager.h"
        "${${PROJECT_NAME}_SRC_DIR}transform.h"
        "${${PROJECT_NAME}_SRC_DIR}ui.h"
        "${${PROJECT_NAME}_SRC_DIR}uimanager.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/uienumerations.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/activatable.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/arearenderable.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/basicrenderable.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/button.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/caption.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/cell.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/cellgrid.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/checkbox.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/dropdownlist.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/glyph.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/kerning.h"
        #"${${PROJECT_NAME}_SRC_DIR}UI/layoutcontainer.h"
        #"${${PROJECT_NAME}_SRC_DIR}UI/layoutquad.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/linelist.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/listbox.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/markuptext.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/menu.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/menuwindow.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/pagedcellgrid.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/radiobutton.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/rectangle.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/renderable.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/renderablecontainer.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/renderablecontainerwidget.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/renderablerect.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/resizehandler.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/resizinginfo.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/screen.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/scrollbar.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/scrolledcellgrid.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/spinner.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/sprite.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/tabset.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/textureatlas.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/vertex.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/viewportupdatetool.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/widget.h"
        "${${PROJECT_NAME}_SRC_DIR}UI/window.h"
        "${${PROJECT_NAME}_SRC_DIR}vector2.h"
        "${${PROJECT_NAME}_SRC_DIR}vector3.h"
        "${${PROJECT_NAME}_SRC_DIR}vector3wactor.h"
        "${${PROJECT_NAME}_SRC_DIR}world.h"
        "${${PROJECT_NAME}_SRC_DIR}worldnode.h"
        "${${PROJECT_NAME}_SRC_DIR}worldobject.h"
        "${${PROJECT_NAME}_SRC_DIR}worldobjectgraphicssettings.h"
        "${${PROJECT_NAME}_SRC_DIR}worldobjectphysicssettings.h"
        "${${PROJECT_NAME}_SRC_DIR}worldtrigger.h"
        "${${PROJECT_NAME}_SRC_DIR}XML/xml.h"
        "${${PROJECT_NAME}_SRC_DIR}XML/xmldoc.h"

#######################################
# A complete listing of our source files

        "${${PROJECT_NAME}_SRC_DIR}actorbase.cpp"
        "${${PROJECT_NAME}_SRC_DIR}actorcharacter.cpp"
        "${${PROJECT_NAME}_SRC_DIR}actormanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}actorphysicssettings.cpp"
        "${${PROJECT_NAME}_SRC_DIR}actorrigid.cpp"
        "${${PROJECT_NAME}_SRC_DIR}actorserializer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}actorsoft.cpp"
        "${${PROJECT_NAME}_SRC_DIR}areaeffect.cpp"
        "${${PROJECT_NAME}_SRC_DIR}attachable.cpp"
        "${${PROJECT_NAME}_SRC_DIR}audiomanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/listener.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/musicplayer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/playlist.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/sound.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Audio/soundset.cpp"
        "${${PROJECT_NAME}_SRC_DIR}binarytool.cpp"
        "${${PROJECT_NAME}_SRC_DIR}camera.cpp"
        "${${PROJECT_NAME}_SRC_DIR}cameracontroller.cpp"
        "${${PROJECT_NAME}_SRC_DIR}cameramanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}collisionshapemanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}colourvalue.cpp"
        "${${PROJECT_NAME}_SRC_DIR}crossplatform.cpp"
        "${${PROJECT_NAME}_SRC_DIR}entity.cpp"
        "${${PROJECT_NAME}_SRC_DIR}entresol.cpp"
        #"${${PROJECT_NAME}_SRC_DIR}enumerations.cpp"
        "${${PROJECT_NAME}_SRC_DIR}eventbase.cpp"
        "${${PROJECT_NAME}_SRC_DIR}eventcollision.cpp"
        "${${PROJECT_NAME}_SRC_DIR}eventgamewindow.cpp"
        "${${PROJECT_NAME}_SRC_DIR}eventmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}eventquit.cpp"
        "${${PROJECT_NAME}_SRC_DIR}eventrendertime.cpp"
        "${${PROJECT_NAME}_SRC_DIR}eventuserinput.cpp"
        "${${PROJECT_NAME}_SRC_DIR}exception.cpp"
        "${${PROJECT_NAME}_SRC_DIR}extendedtimer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}graphicsmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/gamewindow.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/material.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/mesh.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/pass.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/submesh.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/technique.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/texture.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Graphics/viewport.cpp"
        "${${PROJECT_NAME}_SRC_DIR}inputmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/controller.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/device.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/keyboard.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/metacode.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Input/mouse.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/bulletfile.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/bulletfilemanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/decompinterface.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/meshtools.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/motionstate.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Internal/ogredatastreambuf.cpp"
        "${${PROJECT_NAME}_SRC_DIR}light.cpp"
        "${${PROJECT_NAME}_SRC_DIR}linegroup.cpp"
        "${${PROJECT_NAME}_SRC_DIR}managerbase.cpp"
        "${${PROJECT_NAME}_SRC_DIR}mathtool.cpp"
        "${${PROJECT_NAME}_SRC_DIR}matrix3x3.cpp"
        "${${PROJECT_NAME}_SRC_DIR}matrix4x4.cpp"
        "${${PROJECT_NAME}_SRC_DIR}meshmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}meshterrain.cpp"
        "${${PROJECT_NAME}_SRC_DIR}networkmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/ipaddress.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/packet.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/socket.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Network/systemaddress.cpp"
        "${${PROJECT_NAME}_SRC_DIR}objectpair.cpp"
        "${${PROJECT_NAME}_SRC_DIR}objectsettings.cpp"
        "${${PROJECT_NAME}_SRC_DIR}particleaffector.cpp"
        "${${PROJECT_NAME}_SRC_DIR}particleeffect.cpp"
        "${${PROJECT_NAME}_SRC_DIR}particleemitter.cpp"
        "${${PROJECT_NAME}_SRC_DIR}physicsmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/actorsoftcollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/boxcollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/capsulecollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collision.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/collisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/compoundcollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/conecollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/conetwistconstraint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/constraint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/convexhullcollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/cylindercollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/dualtransformconstraint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/dynamicmeshcollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/fieldcollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/generic6dofconstraint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/generic6dofspringconstraint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/heightfieldcollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/hinge2constraint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/hingeconstraint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/meshcollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/multispherecollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/planecollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/point2pointconstraint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/primitivecollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/sliderconstraint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/spherecollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/staticmeshcollisionshape.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Physics/universalconstraint.cpp"
        "${${PROJECT_NAME}_SRC_DIR}plane.cpp"
        "${${PROJECT_NAME}_SRC_DIR}quaternion.cpp"
        "${${PROJECT_NAME}_SRC_DIR}ray.cpp"
        "${${PROJECT_NAME}_SRC_DIR}rayquerytool.cpp"
        "${${PROJECT_NAME}_SRC_DIR}resourcemanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/archive.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/asset.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/assetgroup.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/assethandler.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/datastream.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/inputstream.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Resource/textsettingsfile.cpp"
        "${${PROJECT_NAME}_SRC_DIR}scenemanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/scriptingmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/scriptbindinglua51.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/scriptbindinglua51unsafe.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/lua51script.cpp"
        "${${PROJECT_NAME}_SRC_DIR}Scripting/Lua51/lua51scriptingengine.cpp"
        "${${PROJECT_NAME}_SRC_DIR}serialization.cpp"
        "${${PROJECT_NAME}_SRC_DIR}simpletimer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}stringtool.cpp"
        "${${PROJECT_NAME}_SRC_DIR}terrainbase.cpp"
        "${${PROJECT_NAME}_SRC_DIR}terrainmanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}timermanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}transform.cpp"
        "${${PROJECT_NAME}_SRC_DIR}uimanager.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/activatable.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/arearenderable.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/basicrenderable.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/button.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/caption.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/cell.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/cellgrid.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/checkbox.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/dropdownlist.cpp"
        #"${${PROJECT_NAME}_SRC_DIR}UI/layoutcontainer.cpp"
        #"${${PROJECT_NAME}_SRC_DIR}UI/layoutquad.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/linelist.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/listbox.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/markuptext.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/menu.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/menuwindow.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/pagedcellgrid.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/radiobutton.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/rectangle.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/renderable.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/renderablecontainer.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/renderablecontainerwidget.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/screen.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/scrollbar.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/scrolledcellgrid.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/spinner.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/tabset.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/textureatlas.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/viewportupdatetool.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/widget.cpp"
        "${${PROJECT_NAME}_SRC_DIR}UI/window.cpp"
        "${${PROJECT_NAME}_SRC_DIR}vector2.cpp"
        "${${PROJECT_NAME}_SRC_DIR}vector3.cpp"
        "${${PROJECT_NAME}_SRC_DIR}vector3wactor.cpp"
        "${${PROJECT_NAME}_SRC_DIR}world.cpp"
        "${${PROJECT_NAME}_SRC_DIR}worldnode.cpp"
        "${${PROJECT_NAME}_SRC_DIR}worldobject.cpp"
        "${${PROJECT_NAME}_SRC_DIR}worldobjectgraphicssettings.cpp"
        "${${PROJECT_NAME}_SRC_DIR}worldobjectphysicssettings.cpp"
        "${${PROJECT_NAME}_SRC_DIR}worldtrigger.cpp"
        "${${PROJECT_NAME}_SRC_DIR}XML/xml.cpp"
        "${${PROJECT_NAME}_SRC_DIR}XML/xmldoc.cpp"
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

###############################################################################
# System Build Items
#######################################
# All Changes that could be system specific should be defined in these variables
# These will set sane defaults (Where possible), but specific platforms should
# set a value of their own.

set(LIBTYPE "STATIC") # OpenAL
file(MAKE_DIRECTORY "${ProjectBinaryDir}include") # To not interfere with SDL build process
set(OGRE_STATIC TRUE CACHE BOOL "Forcing static build for Mezzanine build." FORCE)
#set(OGRE_STATIC_LIB TRUE CACHE BOOL "Forcing static build for Mezzanine build." FORCE)
set(OGRE_SET_USE_BOOST FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_USE_BOOST FALSE CACHE BOOL "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_CONFIG_THREADS 0 CACHE INT "Forcing minimal build for Mezzanine build." FORCE)
set(OGRE_CONFIG_THREAD_PROVIDER "" CACHE STRING "Forcing minimal build for Mezzanine build." FORCE)


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
        message ( STATUS "${PROJECT_NAME} - End Apple Specific Build Process")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

###############################################################################
# Common Library Configuration items

include_directories( ${${PROJECT_NAME}_SRC_DIR}
        "${${PROJECT_NAME}_LIBINCLUDE_DIR}common/bulletsrc/bulletsvnsrc/src"
        "${${PROJECT_NAME}_LIBINCLUDE_DIR}common/bulletsrc/bulletsvnsrc/src/LinearMath"
        "${${PROJECT_NAME}_LIBINCLUDE_DIR}common/bulletsrc/bulletsvnsrc/Extras/ConvexDecomposition"
        "${${PROJECT_NAME}_LIBINCLUDE_DIR}common/bulletsrc/bulletsvnsrc/Extras/Serialize/BulletWorldImporter"

        "${${PROJECT_NAME}_LIBINCLUDE_DIR}common/caudiosrc/cAudio2.1.0/include"

        "${ProjectBinaryDir}Mezzanine/libincludes/common/sdlsrc/SDL/include"
        "${${PROJECT_NAME}_LIBINCLUDE_DIR}common/sdlsrc/SDL/include"

        "${ProjectBinaryDir}Mezzanine/libincludes/common/openalsoftsrc/openalsoft"
        "${${PROJECT_NAME}_LIBINCLUDE_DIR}common/openalsoftsrc/openalsoft/include"

        "${${PROJECT_NAME}_LIBINCLUDE_DIR}common/oggvorbis/"
        #"${ProjectBinaryDir}Mezzanine/libincludes/common/ogresrc/ogresvnsrc/include"

        ${OgreConfigHeaderIncludeDir}
        ${OgreHeaderIncludeDir}
)

if(${Mezz_Lua51})
        include_directories( libincludes/common/lua51/lua5.1/src )
endif(${Mezz_Lua51})

link_directories(
        "${ProjectBinaryDir}Mezzanine/bin"
        #"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/ogre"
        #"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/bullet"
        #"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/caudio"
        #"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/openal"
        #"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/oggvorbis"
        #"${${PROJECT_NAME}_LIBBINARY_DIR}/${BuildPlatform}/sdl"
)

###############################################################################
# Prepare the actual Build
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_sources} )

#######################################
# Prepare Common Dependencies

add_subdirectory (libincludes/common/caudiosrc)
add_subdirectory (libincludes/common/bulletsrc)
add_subdirectory (libincludes/common/ogresrc)
add_subdirectory (libincludes/common/oggvorbis)
add_subdirectory (libincludes/common/sdlsrc/SDL)
add_subdirectory (libincludes/common/openalsoftsrc/openalsoft)

set(SDL_ATOMIC    off CACHE BOOL "DISABLED as Mezzanine Component" FORCE)
set(SDL_AUDIO     off CACHE BOOL "DISABLED as Mezzanine Component" FORCE)
set(SDL_CPUINFO   off CACHE BOOL "DISABLED as Mezzanine Component" FORCE)
set(SDL_EVENTS    on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)
set(SDL_FILE      off CACHE BOOL "DISABLED as Mezzanine Component" FORCE)
set(SDL_HAPTIC    on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)
set(SDL_JOYSTICK  on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)
set(SDL_LOADSO    off CACHE BOOL "DISABLED as Mezzanine Component" FORCE)
set(SDL_POWER     on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)
set(SDL_RENDER    off CACHE BOOL "DISABLED as Mezzanine Component" FORCE)
set(SDL_THREADS   off CACHE BOOL "DISABLED as Mezzanine Component" FORCE)
set(SDL_VIDEO     on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)

set(SDL_SHARED    off CACHE BOOL "DISABLED as Mezzanine Component" FORCE)
set(SDL_STATIC    on  CACHE BOOL "ENABLED as Mezzanine Component"  FORCE)

if(${Mezz_Lua51})
        add_subdirectory (libincludes/common/lua51)
endif(${Mezz_Lua51})

#######################################
# Library Link Dependencies
add_dependencies( ${PROJECT_NAME} ${OgreDepName} ${Bullet_LibraryNames} ${LuaProject} ${SDLLibraryNames} ${SoundLibraryNames} )

target_link_libraries( ${PROJECT_NAME}                                 # Link to all of the Mezzanine Dependencies
        ${OgreLibraryAllNames}
        ${LuaLinkName}
        ${Bullet_LibraryNames}
        ${SDLLibraryNames}
        ${NetworkLibrary}
        ${SoundLibraryNames}
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
CopyFilesDuringBuild( common/Nvidia-CG-License.txt data/common/Nvidia-CG-License.txt )
CopyFilesDuringBuild( common/Ogre-MITLicense.txt data/common/Ogre-MITLicense.txt )
CopyFilesDuringBuild( common/PugiXML-MITLicense.txt data/common/PugiXML-MITLicense.txt )
CopyFilesDuringBuild( common/RenéNyffenegger-MIT.txt data/common/RenéNyffenegger-MIT.txt )
CopyFilesDuringBuild( common/SDL-MITLicense.txt data/common/SDL-MITLicense.txt )
CopyFilesDuringBuild( common/XiphCopyrightNotice.txt data/common/XiphCopyrightNotice.txt )
CopyFilesDuringBuild( common/zlib-libpngLicense.txt data/common/zlib-libpngLicense.txt )
CopyFilesDuringBuild( common/zlib-zliblicense.txt data/common/zlib-zliblicense.txt )


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

# © Copyright 2010 - 2011 BlackTopp Studios Inc.
# This file is part of The PhysGame Engine.
#
#    The PhysGame Engine is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    The PhysGame Engine is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
#
#   The original authors have included a copy of the license specified above in the
#   'Docs' folder. See 'gpl.txt'
#
#   We welcome the use of The PhysGame anyone, including companies who wish to
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
#


# The name of our project is "PhysgameAndCatch". CMakeLists files in this project can
# refer to the root source directory of the project as ${PhysgameAndCatch_SOURCE_DIR} and
# to the root binary directory of the project as ${PhysgameAndCatch_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8)
project (PhysgameAndCatch)

# Check For the PhysDebug option and set values accordingling
option(PhysDebug "Compile with no optimizations and debug symbols" OFF)
if(${PhysDebug})
	message ("${PROJECT_NAME} - Enabling PHYSDEBUG Messages")
	if(${WIN32})
		message ("${PROJECT_NAME} - Setting Windows Debug Specific Initialiations")
		set(CommonBuildFlags "-g")
	else(${WIN32})
		message ("${PROJECT_NAME} - Setting Non-Windows Debug Initialiations")
		set(CommonBuildFlags " -fPIC -g")
	endif(${WIN32})
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSDEBUG=1")
else(${PhysDebug})
	message ("${PROJECT_NAME} - Disabling PHYSDEBUG Messages")
	if(${WIN32})
		message ("${PROJECT_NAME} - Setting Windows Specific Initialiations")
		set(CommonBuildFlags " -O2 -s ")
	else(${WIN32})
		message ("${PROJECT_NAME} - Setting Non-Windows Initialiations")
		set(CommonBuildFlags " -fPIC -O2 -s")
	endif(${WIN32})
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSDEBUG=0")
endif(${PhysDebug})

option(PhysProfile "Executables will gather runtime statistics." OFF)
if(${PhysProfile})
	message ("${PROJECT_NAME} - Enabling gcc profiling and PHYSPROFILE")
	set(CommonBuildFlags "${CommonBuildFlags} -pg")
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSPROFILE=1")
else(${PhysProfile})
	message ("${PROJECT_NAME} - Disabling gcc profiling and PHYSPROFILE")
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSPROFILE=0")
endif(${PhysProfile})

option(PhysXML "Enable XML and XML serialization supprt." ON)
if(${PhysXML})
	message ("${PROJECT_NAME} - Enabling XML subsystem, object serialization to XML, and PHYSXML")
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSXML=1")
else(${PhysXML})
	message ("${PROJECT_NAME} - Disabling XML subsystem, object serialization to XML, and PHYSXML")
	set(CommonBuildFlags "${CommonBuildFlags} -DPHYSXML=0")
endif(${PhysXML})

# Do all the arranging of data to 
option(UseOgreDebug "Use Ogre Debug Libraries, instead of Release Libs" OFF)
option(OgreVersion "The version of the ogre libraries to link against" 1.7.3)
if(${UseOgreDebug})
	message ("${PROJECT_NAME} - Will Ogre Debug Libraries")
	set(OgreLinkName OgreMain_d)
	set(OgresD _d)
else(${UseOgreDebug})
	message ("${PROJECT_NAME} - Will Ogre Release Libraries")
	set(OgreLinkName OgreMain)
	set(OgresD "")
	set(OgreLinuxSoName ${OgreLibFileName}.${OgreVersion})
endif(${UseOgreDebug})
if(${WIN32})
	set(OgreLibFileName ${OgreLinkName}.dll)
else(${WIN32})
	set(OgreLibFileName lib${OgreLinkName}.so)
	set(OgreLinuxSoName ${OgreLibFileName}.${OgreVersion})
endif(${WIN32})

set(ProjectRootDir ${${PROJECT_NAME}_SOURCE_DIR})

#Add subprojects
add_subdirectory (physgameengine)
#add_subdirectory (EngineDemo)
add_subdirectory (Catch!) 
add_subdirectory (UnitTests) 



//© Copyright 2010 Joseph Toppi and John Blackwood
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef _crossplatform_h
#define _crossplatform_h
///////////////////////////////////////////////////////////////////////////////
// If we did our jobs right this section should be the only place that you
//need to change to compile this on a supported platform. Just remark all the
//lines that are not your platform using "//" and unremark your platform.
//
// Should you choose to port this to your platform, make sure that all the
//required libraries are installed, then make sure to write an implementation
//for each of the functions in crossplatform.cpp
///////////////////////////////////////////////////////////////////////////////

/// @todo Make main page in documentation with wiki link and lisitng of required steps (build target and OS define), and dependencies required to build this.

//#define LINUX
//#define MACOSX
#define WINDOWS


///////////////////////////////////////////////////////////////////////////////
//End Choose Your Platform
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Includes
#include "physdatatypes.h"

#include <string>

using namespace std;

//Forward Declarations
class PhysWorld;

///////////////////////////////////////////////////////////////////////////////
// Platform Specific Functions get Declared here

//gets thePlugins.cfg filename and location
string GetPluginsDotCFG();

//Gets the Settings.cfg filename and location
string GetSettingsDotCFG();

//Gets the default locatio for Game Data
string GetDataDirectory();

//This returns a named parameter list with valid settings to use Ogre rendering
// on a pre-existing SDL context
void* GetSDLOgreBinder();

//A simple crossplatform sleep function, unused, prefer SDLsleep instead
void WaitMilliseconds(PhysWhole WaitTime);

//Handles the actual cross platform swapping of buffers.
void RenderPhysWorld(PhysWorld *TheWorld);

#endif

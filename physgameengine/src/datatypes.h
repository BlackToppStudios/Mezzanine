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
#ifndef _datatypes_h
#define _datatypes_h
///////////////////////////////////////////////////////////////////////////////
//Any Special data types that we need will get declared right here
///////////////////////////////////////
#include <string>

union SDL_Event;

namespace phys
{
    /// @typedef Real
    /// @brief A Datatype used to represent a real floating point number
    /// @details This Datatype is currently a typedef to a float, This is to match
    /// our compilations of Ogre (rendering subsystem ogre::Real), and Bullet (physics
    /// subsystem, btScalar). With a recompilation of all the subsystems and  this
    /// there is no reason theoretical reason why this could not be changed to a
    /// double, or even something more extreme like a GMP datatype
    typedef float Real;

    /// @typedef Whole
    /// @brief A Datatype used to represent an postive integer numbers
    /// @details This is a typedef to unsigned long.
    typedef unsigned long Whole;

    /// @typedef String
    /// @brief A Datatype used to a series of characters
    /// @details This is a typedef to std::string.
    typedef const std::string String;

    /// @internal
    /// @typedef RawEvent
    /// @brief This is an internal datatype use to communicate with the User input Subsystem
    /// @details This is a typedef to SDL_Event. See the SDL Documentation for more details
    typedef SDL_Event RawEvent;


    //TODO make a typedef for RawEvent.type and a function that return that new datatype
    //  this will make it easier to swap out event subsystems when porting between platforms
    //  If done right we will only need to swap out the code specific to message internals
}

#endif


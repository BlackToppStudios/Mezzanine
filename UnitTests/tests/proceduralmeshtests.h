// © Copyright 2010 - 2017 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
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
#ifndef _proceduralmeshtests_h
#define _proceduralmeshtests_h

#include "mezztest.h"

#include <stdexcept> //only used to throw for TEST_THROW

/// @file
/// @brief This file performs all the basic testing and verification of the Mezzanine engine's ability to generate meshes procedurally from code.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A small series of tests to verify basic functionality of procedurally generating meshes from the Mezzanine API.
class proceduralmeshtests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "ProceduralMesh"
    virtual String Name()
        { return String("ProceduralMesh"); }

    /// @brief This is called when Automatic tests are run
    void RunAutomaticTests()
    {
        // For now we have to initialize the Entresol.
        // In future versions of the Mezzanine we should be able to get away with just the graphics manager.
        // The purpose to doing this is to ensure the internal code to initialize the necessary buffer providers are registered.
        Entresol TheEntresol;
        TheEntresol.GetManager(ManagerBase::MT_GraphicsManager)->Initialize();
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual Boole HasAutomaticTests() const
        { return true; }
};//proceduraltexturetests

#endif

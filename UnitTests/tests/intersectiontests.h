// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
#ifndef _intersectiontests_h
#define _intersectiontests_h

#include "mezztest.h"
#include "MathTools/mathtools.h"

#include <stdexcept> //only used to throw for TEST_THROW

/// @file
/// @brief This will test several classesin the MathTools namespace used ni helping with collision math.

using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace Mezzanine::MathTools;

/// @brief The tests for the intersection helpers
class intersectiontests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "Intersection"
        virtual String Name()
            { return String("Intersection"); }

        /// @brief This is called when Automatic tests are run
        void RunAutomaticTests()
        {
            // The TEST macro will capture Line, function file Metadata while

            {
                AxisAlignedQuad ConstructedIn3d(Axis_X, Vector3(5.0,1.4,1.0), Vector3(5.0,2.4,2.0));
                AxisAlignedQuad ConstructedIn2d(Axis_X, 5.0, Vector2(1.0,1.4), Vector2(2.0,2.4));
                TEST(ConstructedIn3d==ConstructedIn2d, "Construction_X");
            }

            {
                AxisAlignedQuad ConstructedIn3d(Axis_Y, Vector3(1.0,5.0,1.4), Vector3(2.0,5.0,2.4));
                AxisAlignedQuad ConstructedIn2d(Axis_Y, 5.0, Vector2(1.0,1.4), Vector2(2.0,2.4));
                TEST(ConstructedIn3d==ConstructedIn2d, "Construction_Y");
            }

            {
                AxisAlignedQuad ConstructedIn3d(Axis_Z, Vector3(1.0,1.4,5.0), Vector3(2.0,2.4,5.0));
                AxisAlignedQuad ConstructedIn2d(Axis_Z, 5.0, Vector2(1.0,1.4), Vector2(2.0,2.4));
                TEST(ConstructedIn3d==ConstructedIn2d, "Construction_Z");
            }


            TEST_THROW  (
                            Mezzanine::ParametersRangeException&,
                            AxisAlignedQuad ConstructedIn3d(Axis_X, Vector3(5.0,1.4,1.0), Vector3(5.1,2.4,2.0));
                            , "FailWithUnequalPlanes_X"
                        );
            TEST_THROW  (
                            Mezzanine::ParametersRangeException&,
                            AxisAlignedQuad ConstructedIn3d(Axis_Y, Vector3(1.0,5.0,1.4), Vector3(2.0,5.01,2.4));
                            , "FailWithUnequalPlanes_Y"
                        );
            TEST_THROW  (
                            Mezzanine::ParametersRangeException&,
                            AxisAlignedQuad ConstructedIn3d(Axis_Z, Vector3(1.0,1.4,5.0), Vector3(2.0,2.4,5.1));
                            , "FailWithUnequalPlanes_Z"
                        );






        }
        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }
};

#endif


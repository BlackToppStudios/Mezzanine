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
#ifndef _triangletests_h
#define _triangletests_h

#include "mezztest.h"

#include "plane.h"
#include "ray.h"
#include "triangle.h"
#include "XML/xml.h"

#include <stdexcept> //only used to throw for TEST_THROW
#include <chrono>

#include <Ogre.h>

// Needed because windows sucks and uses macros improperly
#ifdef CreateDirectory
#undef CreateDirectory
#endif
// Needed because windows sucks and uses macros improperly
#ifdef RemoveDirectory
#undef RemoveDirectory
#endif

/// @file
/// @brief This file should be used as template/example for building future Unit Tests

using namespace Mezzanine;
using namespace Mezzanine::MathTools;
using namespace Mezzanine::Testing;

const Whole FighterVertCount = 60;
extern const Vector3 FighterVerts[FighterVertCount];
const Whole FighterFaceCount = 116;
const Whole FighterIndexCount = FighterFaceCount * 3;
extern const Integer FighterIndexes[FighterIndexCount];

/// @brief A small series of sample tests, which can be used as a boilerplate so creating new test groups
class triangletests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "Triangle"
    virtual String Name()
        { return String("Triangle"); }

    /// @brief Performs a timed test with a Ray against the fighter mesh using the Moller-Trumbore algorithm and logs the result.
    /// @param Cast The Ray to be cast during the test.
    /// @param RayName The name of the ray to use for logging.
    void MollerTrumboreRayTest(const Ray& Cast, const String& RayName)
    {
        TestOutput << "Timing the Mezzanine implementation of the Moller-Trumbore Ray-Triangle intersection algorithm." << std::endl;
        std::chrono::high_resolution_clock::time_point Start = std::chrono::high_resolution_clock::now();

        PlaneTestResult Result(PS_Neither,Vector3(0.0,0.0,0.0));
        for( Whole CurrIter = 1 ; CurrIter < 500 ; ++CurrIter )
        {
            for( Whole Face = 0 ; Face < FighterFaceCount ; ++Face )
            {
                Result = Intersects( FighterVerts[ FighterIndexes[ ( Face * 3 ) + 0 ] ],
                                     FighterVerts[ FighterIndexes[ ( Face * 3 ) + 1 ] ],
                                     FighterVerts[ FighterIndexes[ ( Face * 3 ) + 2 ] ],
                                     Cast);

                if( Result.first == PS_Positive || Result.first == PS_Negative )
                    break;
            }
        }

        std::chrono::high_resolution_clock::time_point End = std::chrono::high_resolution_clock::now();
        std::chrono::microseconds TestTime = std::chrono::duration_cast<std::chrono::microseconds>(End - Start);
        if( Result.first == PS_Positive || Result.first == PS_Negative ) {
            TestOutput << "Hit was found on the " << ( Result.first == PS_Positive ? "Positive" : "Negative" )
                       << " side of the Triangle with the " << RayName << " Ray at: " << Result.second << "." << std::endl;
        }else{
            TestOutput << "No hit was found with the " << RayName << " Ray." << std::endl;
        }
        TestOutput << "Moller-Trumbore Algorithm took " << TestTime.count() << " microseconds after 500 iterations.";
    }
    /// @brief Performs a timed test with a Ray against the fighter mesh using the Ogre algorithm and logs the result.
    /// @param Cast The Ray to be cast during the test.
    /// @param RayName The name of the ray to use for logging.
    void OgreRayTest(const Ray& Cast, const String& RayName)
    {
        TestOutput << "Timing the Mezzanine implementation of the Ogre Ray-Triangle intersection algorithm." << std::endl;
        std::chrono::high_resolution_clock::time_point Start = std::chrono::high_resolution_clock::now();

        PlaneTestResult Result(PS_Neither,Vector3(0.0,0.0,0.0));
        for( Whole CurrIter = 1 ; CurrIter < 500 ; ++CurrIter )
        {
            for( Whole Face = 0 ; Face < FighterFaceCount ; ++Face )
            {
                Vector3 FaceNormal = Triangle3D::CalculateBasicNormal( FighterVerts[ FighterIndexes[ ( Face * 3 ) + 0 ] ],
                                                                       FighterVerts[ FighterIndexes[ ( Face * 3 ) + 1 ] ],
                                                                       FighterVerts[ FighterIndexes[ ( Face * 3 ) + 2 ] ] );
                Result = Intersects( FighterVerts[ FighterIndexes[ ( Face * 3 ) + 0 ] ],
                                     FighterVerts[ FighterIndexes[ ( Face * 3 ) + 1 ] ],
                                     FighterVerts[ FighterIndexes[ ( Face * 3 ) + 2 ] ],
                                     FaceNormal,
                                     Cast);

                if( Result.first == PS_Positive || Result.first == PS_Negative )
                    break;
            }
        }

        std::chrono::high_resolution_clock::time_point End = std::chrono::high_resolution_clock::now();
        std::chrono::microseconds TestTime = std::chrono::duration_cast<std::chrono::microseconds>(End - Start);
        if( Result.first == PS_Positive || Result.first == PS_Negative ) {
            TestOutput << "Hit was found on the " << ( Result.first == PS_Positive ? "Positive" : "Negative" )
                       << " side of the Triangle with the " << RayName << " Ray at: " << Result.second << "." << std::endl;
        }else{
            TestOutput << "No hit was found with the " << RayName << " Ray." << std::endl;
        }
        TestOutput << "Ogre Algorithm took " << TestTime.count() << " microseconds after 500 iterations.";
    }

    /// @brief Test most features on the Triangle.
    void RunAutomaticTests()
    {
        {// Constructors and Operators
            Vector3 PointOne(-4.0,-3.0,0.0);
            Vector3 PointTwo(4.0,5.0,4.0);
            Vector3 PointThree(4.0,5.0,-4.0);

            Triangle3D BlankTest;
            TEST( BlankTest.PointA == Vector3() && BlankTest.PointB == Vector3() && BlankTest.PointC == Vector3(),
                  "Triangle3D()" );

            Triangle3D ExplicitTest(PointOne,PointTwo,PointThree);
            TEST( ExplicitTest.PointA == PointOne && ExplicitTest.PointB == PointTwo && ExplicitTest.PointC == PointThree,
                  "Triangle3D(const_Vector3&,const_Vector3&,const_Vector3&)" );

            Triangle3D CopyTest(ExplicitTest);
            TEST( CopyTest.PointA == PointOne && CopyTest.PointB == PointTwo && CopyTest.PointC == PointThree,
                  "Triangle3D(const_Triangle3D&)" );

            Triangle3D AssignmentTest;
            AssignmentTest = CopyTest;
            TEST( AssignmentTest.PointA == PointOne && AssignmentTest.PointB == PointTwo && AssignmentTest.PointC == PointThree,
                  "Triangle3D::operator=(const_Triangle3D&)" );

            TEST( ExplicitTest == CopyTest && !( BlankTest == CopyTest ),
                  "Triangle3D::operator==(const_Triangle3D&)" );

            TEST( BlankTest != ExplicitTest && !( ExplicitTest != CopyTest ),
                  "Triangle3D::operator!=(const_Triangle3D&)" );

            TEST( CopyTest[0] == PointOne && CopyTest[1] == PointTwo && CopyTest[2] == PointThree,
                  "Triangle3D::operator[](const_Whole)" );
        }// Constructors and Operators

        {// Ray Intersection Tests
            // Vertical Triangle
            Triangle3D TestTriOne( Vector3(0.0,2.0,1.0),
                                   Vector3(-2.0,-2.0,1.0),
                                   Vector3(2.0,-2.0,1.0) );
            // Horizontal Triangle
            Triangle3D TestTriTwo( Vector3(0.0,2.0,1.0),
                                   Vector3(2.0,2.0,-1.0),
                                   Vector3(-2.0,2.0,-1.0) );
            // Diagonal Triangle
            Triangle3D TestTriThree( Vector3(-4.0,-3.0,0.0),
                                     Vector3(4.0,5.0,4.0),
                                     Vector3(4.0,5.0,-4.0) );
            // Triangle behind Ray
            Triangle3D TestTriFour( Vector3(0.0,2.0,12.0),
                                    Vector3(-2.0,-2.0,12.0),
                                    Vector3(2.0,-2.0,12.0) );
            // Middle of nowhere triangle
            Triangle3D TestTriFive( Vector3(100.0,102.0,101.0),
                                    Vector3(98.0,98.0,101.0),
                                    Vector3(102.0,98.0,101.0) );

            // Horizontal Ray
            Ray TestRayOne( Vector3(0.0,1.0,10.0),
                            Vector3(0.0,0.0,-1.0) );
            // Vertical Ray
            Ray TestRayTwo( Vector3(1.0,-10.0,0.0),
                            Vector3(0.0,1.0,0.0) );
            // Diagonal Ray
            Ray TestRayThree( Vector3(-10.0,11.0,0.0),
                              Vector3(1.0,-1.0,0.0) );//Ray automatically normalizes

            {// Moller-Trumbore
                // 1-1
                Triangle3D::RayTestResult ZToVert = TestTriOne.Intersects(TestRayOne);
                TEST( ZToVert.first == MathTools::PS_Positive && CompareEqualityWithEpsilon(ZToVert.second,Vector3(0.0,1.0,1.0),2),
                      "Triangle3D::Intersects(const_Ray&)_Z-Axis_Ray_to_Vertical_Triangle" );
                // 1-3
                Triangle3D::RayTestResult DiagToVert = TestTriOne.Intersects(TestRayThree);
                TEST( DiagToVert.first == MathTools::PS_Neither && CompareEqualityWithEpsilon(DiagToVert.second,Vector3(0.0,0.0,0.0),2),
                      "Triangle3D::Intersects(const_Ray&)_Diagonal_Ray_to_Parallel_Vertical_Triangle" );
                // 2-2
                Triangle3D::RayTestResult YToHori = TestTriTwo.Intersects(TestRayTwo);
                TEST( YToHori.first == MathTools::PS_Negative && CompareEqualityWithEpsilon(YToHori.second,Vector3(1.0,2.0,0.0),2),
                      "Triangle3D::Intersects(const_Ray&)_Y-Axis_Ray_to_Horizontal_Triangle" );
                // 3-3
                Triangle3D::RayTestResult DiagToDiag = TestTriThree.Intersects(TestRayThree);
                TEST( DiagToDiag.first == MathTools::PS_Positive && CompareEqualityWithEpsilon(DiagToDiag.second,Vector3(0.0,1.0,0.0),2),
                      "Triangle3D::Intersects(const_Ray&)_Diagonal_Ray_to_Diagonal_Triangle" );
                // 4-1
                Triangle3D::RayTestResult ZBehind = TestTriFour.Intersects(TestRayOne);
                TEST( ZBehind.first == MathTools::PS_Neither && CompareEqualityWithEpsilon(ZBehind.second,Vector3(0.0,0.0,0.0),2),
                      "Triangle3D::Intersects(const_Ray&)_Z-Axis_Ray_with_Triangle_behind" );
                // 5-2
                Triangle3D::RayTestResult YToNone = TestTriFive.Intersects(TestRayTwo);
                TEST( YToNone.first == MathTools::PS_Neither && CompareEqualityWithEpsilon(YToNone.second,Vector3(0.0,0.0,0.0),2),
                      "Triangle3D::Intersects(const_Ray&)_Y-Axis_Ray_with_Triangle_nowhere_near" );
            }// Moller-Trumbore

            {// Ogre
                // 1-1
                Vector3 TestTriOneNorm = TestTriOne.CalculateBasicNormal();
                Triangle3D::RayTestResult ZToVert = MathTools::Intersects(TestTriOne,TestTriOneNorm,TestRayOne);
                TEST( ZToVert.first == MathTools::PS_Positive && CompareEqualityWithEpsilon(ZToVert.second,Vector3(0.0,1.0,1.0),4),
                      "MathTools::Intersects(const_Triangle3D&,const_Vector3&,const_Ray&)_Z-Axis_Ray_to_Vertical_Triangle" );
                // 1-3
                Triangle3D::RayTestResult DiagToVert = MathTools::Intersects(TestTriOne,TestTriOneNorm,TestRayThree);
                TEST( DiagToVert.first == MathTools::PS_Neither && CompareEqualityWithEpsilon(DiagToVert.second,Vector3(0.0,0.0,0.0),4),
                      "MathTools::Intersects(const_Triangle3D&,const_Vector3&,const_Ray&)_Diagonal_Ray_to_Parallel_Vertical_Triangle" );
                // 2-2
                Vector3 TestTriTwoNorm = TestTriTwo.CalculateBasicNormal();
                Triangle3D::RayTestResult YToHori = MathTools::Intersects(TestTriTwo,TestTriTwoNorm,TestRayTwo);
                TEST( YToHori.first == MathTools::PS_Negative && CompareEqualityWithEpsilon(YToHori.second,Vector3(1.0,2.0,0.0),4),
                      "MathTools::Intersects(const_Triangle3D&,const_Vector3&,const_Ray&)_Y-Axis_Ray_to_Horizontal_Triangle" );
                // 3-3
                Vector3 TestTriThreeNorm = TestTriThree.CalculateBasicNormal();
                Triangle3D::RayTestResult DiagToDiag = MathTools::Intersects(TestTriThree,TestTriThreeNorm,TestRayThree);
                TEST( DiagToDiag.first == MathTools::PS_Positive && CompareEqualityWithEpsilon(DiagToDiag.second,Vector3(0.0,1.0,0.0),8),
                      "MathTools::Intersects(const_Triangle3D&,const_Vector3&,const_Ray&)_Diagonal_Ray_to_Diagonal_Triangle" );
                // 4-1
                Vector3 TestTriFourNorm = TestTriFour.CalculateBasicNormal();
                Triangle3D::RayTestResult ZBehind = MathTools::Intersects(TestTriFour,TestTriFourNorm,TestRayOne);
                TEST( ZBehind.first == MathTools::PS_Neither && CompareEqualityWithEpsilon(ZBehind.second,Vector3(0.0,0.0,0.0),4),
                      "MathTools::Intersects(const_Triangle3D&,const_Vector3&,const_Ray&)_Z-Axis_Ray_with_Triangle_behind" );
                // 5-2
                Vector3 TestTriFiveNorm = TestTriFive.CalculateBasicNormal();
                Triangle3D::RayTestResult YToNone = MathTools::Intersects(TestTriFive,TestTriFiveNorm,TestRayTwo);
                TEST( YToNone.first == MathTools::PS_Neither && CompareEqualityWithEpsilon(YToNone.second,Vector3(0.0,0.0,0.0),4),
                      "MathTools::Intersects(const_Triangle3D&,const_Vector3&,const_Ray&)_Y-Axis_Ray_with_Triangle_far_away" );
            }// Ogre
        }// Ray Intersection Tests

        {// Timing Tests
            // +X Axis
            Ray TestRayOne( Vector3(10.0,0.0,0.0),
                            Vector3(-4.0,1.0,0.0) );
            // -X Axis
            Ray TestRayTwo( Vector3(-10.0,0.0,0.0),
                            Vector3(4.0,1.0,0.0) );
            // +Y Axis
            Ray TestRayThree( Vector3(0.0,10.0,0.0),
                              Vector3(0.0,-8.0,1.0) );
            // -Y Axis
            Ray TestRayFour( Vector3(0.0,-10.0,0.0),
                             Vector3(0.0,1.0,0.0) );
            // +Z Axis
            Ray TestRayFive( Vector3(0.0,0.0,10.0),
                             Vector3(1.0,2.0,-5.0) );
            // -Z Axis
            Ray TestRaySix( Vector3(0.0,0.0,-10.0),
                            Vector3(-1.0,2.0,5.0) );

            {// Moller-Trumbore Algorithm
                this->MollerTrumboreRayTest(TestRayOne,"+X Axis");
                this->MollerTrumboreRayTest(TestRayTwo,"-X Axis");
                this->MollerTrumboreRayTest(TestRayThree,"+Y Axis");
                this->MollerTrumboreRayTest(TestRayFour,"-Y Axis");
                this->MollerTrumboreRayTest(TestRayFive,"+Z Axis");
                this->MollerTrumboreRayTest(TestRaySix,"-Z Axis");
            }// Moller-Trumbore Algorithm

            {// Ogre Algorithm
                this->OgreRayTest(TestRayOne,"+X Axis");
                this->OgreRayTest(TestRayTwo,"-X Axis");
                this->OgreRayTest(TestRayThree,"+Y Axis");
                this->OgreRayTest(TestRayFour,"-Y Axis");
                this->OgreRayTest(TestRayFive,"+Z Axis");
                this->OgreRayTest(TestRaySix,"-Z Axis");
            }// Ogre Algorithm
        }// Timing Tests
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//triangletests

// These vertexes are their index list after are of a space fighter model under the CC0 license.
// Data was extracted by exporting in blender to an .obj, and regex'ing the information into the format you see below.
// The original can be found here:
// http://opengameart.org/content/low-spec-airspace-fighter
const Vector3 FighterVerts[FighterVertCount] = {
    Vector3(0.700000,0.100000,2.600000),
    Vector3(0.200000,0.100000,4.000000),
    Vector3(0.500000,0.200000,2.000000),
    Vector3(0.600000,0.200000,1.400000),
    Vector3(0.600000,0.200000,0.300000),
    Vector3(1.500000,0.000000,1.600000),
    Vector3(2.000000,0.000000,1.200000),
    Vector3(0.700000,0.300000,1.500000),
    Vector3(0.600000,0.300000,0.300000),
    Vector3(1.400000,0.100000,1.600000),
    Vector3(0.200000,0.300000,3.000000),
    Vector3(0.500000,0.600000,1.500000),
    Vector3(0.300000,0.500000,0.300000),
    Vector3(0.500000,0.600000,0.900000),
    Vector3(0.600000,0.900000,0.300000),
    Vector3(0.600000,0.900000,0.600000),
    Vector3(0.400000,0.600000,0.900000),
    Vector3(1.700000,0.000000,2.100000),
    Vector3(1.400000,0.000000,2.100000),
    Vector3(0.300000,0.100000,0.000000),
    Vector3(0.500000,0.300000,0.000000),
    Vector3(0.100000,0.300000,0.000000),
    Vector3(0.300000,0.500000,0.000000),
    Vector3(0.400000,0.500000,0.300000),
    Vector3(0.000000,0.000000,3.000000),
    Vector3(-0.700000,0.100000,2.600000),
    Vector3(0.000000,0.100000,4.000000),
    Vector3(-0.200000,0.100000,4.000000),
    Vector3(0.000000,0.000000,2.100000),
    Vector3(-0.500000,0.200000,2.000000),
    Vector3(0.000000,0.000000,1.400000),
    Vector3(-0.600000,0.200000,1.400000),
    Vector3(0.000000,0.100000,0.300000),
    Vector3(-0.600000,0.200000,0.300000),
    Vector3(-1.500000,0.000000,1.600000),
    Vector3(-2.000000,0.000000,1.200000),
    Vector3(0.000000,0.200000,3.700000),
    Vector3(0.000000,0.400000,3.300000),
    Vector3(0.000000,0.400000,2.500000),
    Vector3(0.000000,0.500000,1.500000),
    Vector3(-0.700000,0.300000,1.500000),
    Vector3(0.000000,0.400000,0.200000),
    Vector3(-0.600000,0.300000,0.300000),
    Vector3(-1.400000,0.100000,1.600000),
    Vector3(-0.200000,0.300000,3.000000),
    Vector3(-0.500000,0.600000,1.500000),
    Vector3(-0.300000,0.500000,0.300000),
    Vector3(-0.500000,0.600000,0.900000),
    Vector3(-0.600000,0.900000,0.300000),
    Vector3(-0.600000,0.900000,0.600000),
    Vector3(-0.400000,0.600000,0.900000),
    Vector3(0.000000,0.500000,0.900000),
    Vector3(-1.700000,0.000000,2.100000),
    Vector3(-1.400000,0.000000,2.100000),
    Vector3(-0.300000,0.100000,0.000000),
    Vector3(-0.500000,0.300000,0.000000),
    Vector3(0.000000,0.300000,0.200000),
    Vector3(-0.100000,0.300000,0.000000),
    Vector3(-0.300000,0.500000,0.000000),
    Vector3(-0.400000,0.500000,0.300000)
};

const Integer FighterIndexes[FighterIndexCount] = {
    25, 2, 27,
    31, 3, 29,
    20, 31, 33,
    29, 1, 25,
    11, 38, 37,
    8, 6, 10,
    21, 5, 20,
    22, 33, 57,
    1, 39, 11,
    2, 37, 27,
    2, 11, 37,
    3, 4, 8,
    2, 25, 1,
    6, 18, 19,
    38, 11, 39,
    6, 4, 5,
    11, 2, 1,
    3, 40, 39,
    8, 12, 40,
    42, 22, 57,
    9, 21, 24,
    8, 14, 12,
    16, 13, 17,
    17, 12, 14,
    40, 3, 8,
    17, 42, 52,
    14, 9, 24,
    15, 24, 13,
    14, 16, 17,
    22, 42, 23,
    18, 6, 7,
    10, 18, 7,
    5, 21, 9,
    23, 42, 13,
    21, 22, 23,
    22, 21, 20,
    23, 24, 21,
    33, 22, 20,
    31, 5, 4,
    24, 16, 14,
    31, 20, 5,
    7, 5, 9,
    9, 10, 7,
    19, 10, 6,
    18, 10, 19,
    12, 17, 40,
    40, 17, 52,
    9, 14, 8,
    5, 7, 6,
    42, 17, 13,
    3, 31, 4,
    1, 29, 3,
    39, 1, 3,
    13, 16, 15,
    24, 23, 13,
    16, 24, 15,
    6, 8, 4,
    10, 9, 8,
    25, 27, 28,
    31, 29, 30,
    55, 33, 31,
    29, 25, 26,
    45, 37, 38,
    41, 44, 35,
    56, 55, 34,
    58, 57, 33,
    26, 45, 39,
    28, 27, 37,
    28, 37, 45,
    30, 41, 32,
    28, 26, 25,
    35, 54, 53,
    38, 39, 45,
    35, 34, 32,
    45, 26, 28,
    30, 39, 40,
    41, 40, 46,
    42, 57, 58,
    43, 60, 56,
    41, 46, 48,
    50, 51, 47,
    51, 48, 46,
    40, 41, 30,
    51, 52, 42,
    48, 60, 43,
    49, 47, 60,
    48, 51, 50,
    58, 59, 42,
    53, 36, 35,
    44, 36, 53,
    34, 43, 56,
    59, 47, 42,
    56, 59, 58,
    58, 55, 56,
    59, 56, 60,
    33, 55, 58,
    31, 32, 34,
    60, 48, 50,
    31, 34, 55,
    36, 43, 34,
    43, 36, 44,
    54, 35, 44,
    53, 54, 44,
    46, 40, 51,
    40, 52, 51,
    43, 41, 48,
    34, 35, 36,
    42, 47, 51,
    30, 32, 31,
    26, 30, 29,
    39, 30, 26,
    47, 49, 50,
    60, 47, 59,
    50, 49, 60,
    35, 32, 41,
    44, 41, 43
};

#endif

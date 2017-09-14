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
#ifndef _versiontests_h
#define _versiontests_h

#include "mezztest.h"

#include "version.h"

#include <stdexcept> //only used to throw for TEST_THROW

/// @file
/// @brief This file should be used as template/example for building future Unit Tests

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A series of tests to verify parsing and comparison functionality of the Version utility classes.
class versiontests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "Version".
    virtual String Name()
        { return String("Version"); }

    /// @brief This is called when Automatic tests are run.
    void RunAutomaticTests()
    {
        {//SimpleVersion
            String VerFirstSource("0.8");
            String VerSecondSource("1.0");
            String VerThirdSource("1.2");
            SimpleVersion VerFirst(VerFirstSource);
            SimpleVersion VerSecond(VerSecondSource);
            SimpleVersion VerThird(VerThirdSource);

            Boole FromStringFirstTest = VerFirst.Major == 0 && VerFirst.Minor == 8;
            Boole FromStringSecondTest = VerSecond.Major == 1 && VerSecond.Minor == 0;
            Boole FromStringThirdTest = VerThird.Major == 1 && VerThird.Minor == 2;
            TEST(FromStringFirstTest && FromStringSecondTest && FromStringThirdTest,"SimpleVersion::FromString(const_String&,const_Char8)");
            TEST(VerFirst.ToString() == VerFirstSource && VerSecond.ToString() == VerSecondSource && VerThird.ToString() == VerThirdSource,"SimpleVersion::ToString(const_Char8)_const");
            TEST(VerFirst < VerSecond && VerSecond < VerThird,"SimpleVersion::operator<(const_SimpleVersion&)");
            TEST(VerThird > VerSecond && VerSecond > VerFirst,"SimpleVersion::operator>(const_SimpleVersion&)");
            TEST(VerFirst == SimpleVersion(0,8) && VerSecond == SimpleVersion(1,0) && VerThird == SimpleVersion(1,2),"SimpleVersion::operator==(const_SimpleVersion&)");
            TEST(VerFirst != SimpleVersion(3,6) && VerSecond != SimpleVersion(1,5) && VerThird != SimpleVersion(0,2),"SimpleVersion::operator!=(const_SimpleVersion&)");
        }//SimpleVersion

        {//SemanticVersion
            String VerFirstSource("0.8.1-alpha");
            String VerSecondSource("1.0.10-rc1");
            String VerThirdSource("1.0.10-rc2+25428724");
            String VerFourthSource("1.4.1");
            SemanticVersion VerFirst(VerFirstSource);
            SemanticVersion VerSecond(VerSecondSource);
            SemanticVersion VerThird(VerThirdSource);
            SemanticVersion VerFourth(VerFourthSource);

            Boole FromStringFirstTest = VerFirst.Major == 0 && VerFirst.Minor == 8 && VerFirst.Patch == 1 && VerFirst.PreRelease == "alpha" && VerFirst.MetaData.empty();
            Boole FromStringSecondTest = VerSecond.Major == 1 && VerSecond.Minor == 0 && VerSecond.Patch == 10 && VerSecond.PreRelease == "rc1" && VerSecond.MetaData.empty();
            Boole FromStringThirdTest = VerThird.Major == 1 && VerThird.Minor == 0 && VerThird.Patch == 10 && VerThird.PreRelease == "rc2" && VerThird.MetaData == "25428724";
            Boole FromStringFourthTest = VerFourth.Major == 1 && VerFourth.Minor == 4 && VerFourth.Patch == 1 && VerFourth.PreRelease.empty() && VerFourth.MetaData.empty();
            TEST(FromStringFirstTest && FromStringSecondTest && FromStringThirdTest && FromStringFourthTest,"SemanticVersion::FromString(const_String&)");
            TEST(VerFirst.ToString() == VerFirstSource && VerSecond.ToString() == VerSecondSource && VerThird.ToString() == VerThirdSource && VerFourth.ToString() == VerFourthSource,"SemanticVersion::ToString()_const");
            TEST(VerFirst < VerSecond && VerSecond < VerThird && VerThird < VerFourth,"SemanticVersion::operator<(const_SemanticVersion&)");
            TEST(VerFourth > VerThird && VerThird > VerSecond && VerSecond > VerFirst,"SemanticVersion::operator>(const_SemanticVersion&)");
            TEST(VerFirst == SemanticVersion(0,8,1,"alpha","") && VerSecond == SemanticVersion(1,0,10,"rc1","") && VerThird == SemanticVersion(1,0,10,"rc2","") && VerFourth == SemanticVersion(1,4,1,"","245123"),"SemanticVersion::operator==(const_SemanticVersion&)");
            TEST(VerFirst != SemanticVersion(3,6,1,"alpha","") && VerSecond != SemanticVersion(1,0,10,"beta","") && VerThird != SemanticVersion(6,2,4) && VerThird != SemanticVersion(1,4,0),"SemanticVersion::operator!=(const_SemanticVersion&)");
        }//SemanticVersion
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return Returns true.
    virtual Boole HasAutomaticTests() const
        { return true; }
};//versiontests

#endif

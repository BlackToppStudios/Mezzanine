// © Copyright 2010 - 2018 BlackTopp Studios Inc.
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
#ifndef _systempathutilitiestests_h
#define _systempathutilitiestests_h

#include "mezztest.h"

#include "Resource/systempathutilities.h"

/// @file
/// @brief A few tests of some utilities that work with the system path.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A few tests covering utilities that work with the system path.
class systempathutilitiestests : public UnitTestGroup
{
public:
    /// @copydoc Testing::UnitTestGroup::Name
    /// @return Returns a String containing "SystemPathUtilities"
    virtual String Name()
        { return String("SystemPathUtilities"); }

    /// @brief This is called when Automatic tests are run
    void RunAutomaticTests()
    {
        TestOutput << "Your Current PATH as parsed by GetSystemPATH():" << std::endl;
        StringVector PATH = std::move( Resource::GetSystemPATH() );
        for(StringVector::const_iterator Iter = PATH.begin();
            Iter!=PATH.end();
            Iter++)
        {
            TestOutput << "\t\"" << *Iter << "\"" << std::endl;
        }
        TestOutput << "End of current system PATH" << std::endl;

        std::stringstream SamplePath;
        SamplePath << "/a/b/c" << Resource::GetPathSeparator_Host()
                   << "/bin" << Resource::GetPathSeparator_Host()
                   << Resource::GetPathSeparator_Host();

        TestOutput << "Test parsing a sample PATH: " << SamplePath.str() << std::endl
             << "Becomes: ";
        PATH = std::move( Resource::GetSystemPATH(SamplePath.str()) );
        for(StringVector::const_iterator Iter = PATH.begin();
            Iter!=PATH.end();
            Iter++)
        {
            TestOutput << "\t\"" << *Iter << "\"" << std::endl;
        }
        TEST(PATH[0]=="/a/b/c"&&PATH[1]=="/bin"&&PATH[2]=="", "PATHParsing");

        TestOutput << "looking for \"ls\" and comparing our results to the system"  << std::endl
             << "\t\"" << (Resource::Which("ls")) << "\"" << std::endl
             << "\t\"" << Resource::GetDirName(GetCommandResults("which ls")) << "\"" << std::endl;
        TEST_WARN((Resource::Which("ls"))==Resource::GetDirName(GetCommandResults("which ls")),"Whichls");

        //Does Windows have which, what does where's output look like?
        TestOutput << "looking for \"cmd.exe\" and comparing our results to the system"  << std::endl
             << "\t\"" << (Resource::Which("cmd.exe")) << "\"" << std::endl
             << "\t\"" << Resource::GetDirName(GetCommandResults("which cmd.exe")) << "\"" << std::endl;
        TEST_WARN((Resource::Which("cmd.exe"))==Resource::GetDirName(GetCommandResults("which cmd.exe")),"Whichcmd");
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};

#endif

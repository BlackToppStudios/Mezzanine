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
#ifndef _filesystemmanagementtests_h
#define _filesystemmanagementtests_h

#include "mezztest.h"

#include "Resource/filesystemmanagement.h"

#include "Resource/platformundefs.h"

/// @file
/// @brief A few tests of some basic filesystem utilities in the resource system. Only partial coverage.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A few tests covering basic filesystem utilities.
class filesystemmanagementtests : public UnitTestGroup
{
public:
    /// @copydoc Testing::UnitTestGroup::Name
    /// @return Returns a String containing "FileSystemManagement"
    virtual String Name()
        { return String("FileSystemManagement"); }

    /// @brief This is called when Automatic tests are run
    void RunAutomaticTests()
    {
        {// Basic File Management
            TEST(!Resource::FileExists("UtilityTestFile.txt"),"FileExists-FailCheck");

            std::ofstream UtilityFile;
            UtilityFile.open("./UtilityTestFile.txt");
            UtilityFile << "I exist!";
            UtilityFile.close();

            TEST(Resource::FileExists("UtilityTestFile.txt"),"FileExists-PassCheck");
            TEST(Resource::CopyFile("UtilityTestFile.txt","UtilityTestCopy.txt",false),"CopyFile-Fresh");
            TEST(Resource::FileExists("UtilityTestCopy.txt"),"CopyFile-Exists");
            TEST(!Resource::CopyFile("UtilityTestCopy.txt","UtilityTestFile.txt",true),"CopyFile-DuplicateFail");
            TEST(Resource::CreateDirectory("MoveTarget/"),"MoveFile-CreateDest");
            TEST(Resource::MoveFile("UtilityTestCopy.txt","MoveTarget/UtilityTestCopy.txt",true),"MoveFile-ActualMove");
            TEST(!Resource::FileExists("UtilityTestCopy.txt"),"MoveFile-SourceDoesntExist");
            TEST(Resource::FileExists("MoveTarget/UtilityTestCopy.txt"),"MoveFile-DestExists");
            TEST(Resource::MoveFile("MoveTarget/UtilityTestCopy.txt","MoveTarget/RenamedCopy.txt",true),"MoveFile-Rename");
            TEST(Resource::RemoveFile("MoveTarget/RenamedCopy.txt"),"RemoveFile-DeleteMoved");
            TEST(!Resource::FileExists("MoveTarget/RenamedCopy.txt"),"RemoveFile-VerifyMoved");
            TEST(Resource::RemoveFile("UtilityTestFile.txt"),"RemoveFile-DeleteOriginal");
            TEST(!Resource::FileExists("UtilityTestFile.txt"),"RemoveFile-VerifyOriginal");
            TEST(Resource::RemoveDirectory("MoveTarget/"),"MoveFile-DestroyDest");
        }// Basic File Management

        {// Symlinks
            // No tests for symlinks exist yet.
        }// Symlinks

        {// Basic Directory Management
            String TestDirString(TestingScratchDir);
            TestDirString += "resourcetesting";

            // Perform cleanup from any unclean runs of the tests
            Boole Exists = Resource::DirectoryExists(TestDirString);
            if( Exists ) {
                Boole Removed = !Resource::RemoveDirectory(TestDirString);
                Boole Verify = Resource::DirectoryExists(TestDirString);
                if( Removed && Verify ) {
                    TEST(false,"TestCleanup");
                    return;
                }
            }

            TEST( Resource::CreateDirectory(TestDirString), "CreateDirectory(const_String&)" );
            TEST( Resource::DirectoryExists(TestDirString), "DirectoryExists(const_String&)" );

            const String BaseDir = "./";
            const String Depth1 = "Depth1/";
            const String Depth2 = "Depth2/";
            const String Depth3 = "Depth3/";
            const String DepthDir = BaseDir + Depth1 + Depth2 + Depth3;

            TEST( Resource::CreateDirectoryPath(DepthDir), "CreateDirectoryPath(const_String&)");
            TEST( Resource::DirectoryExists(DepthDir), "CreateDirectoryPath(const_String&)-Verify");
            TEST( Resource::RemoveDirectory(BaseDir+Depth1+Depth2+Depth3), "RemoveDirectory(const_String&)-PathDepth3" );
            TEST( Resource::RemoveDirectory(BaseDir+Depth1+Depth2), "RemoveDirectory(const_String&)-PathDepth2" );
            TEST( Resource::RemoveDirectory(BaseDir+Depth1), "RemoveDirectory(const_String&)-PathDepth1" );

            TEST( Resource::RemoveDirectory(TestDirString), "RemoveDirectory(const_String&)-BaseDir" );
        }// Basic Directory Management
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};

#endif

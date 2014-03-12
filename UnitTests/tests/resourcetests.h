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
#ifndef _resourcetests_h
#define _resourcetests_h

#include "mezztest.h"

#include "Resource/resourceutilities.h"
#include "Resource/resourcemanager.h"

#include <fstream>

/// @file
/// @brief A few tests of the resource system, only partial coverage

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A few tests covering primarily directory management.
class resourcetests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "Resource"
        virtual String Name()
            { return String("Resource"); }

        /// @brief Test only directory creation and removal
        /// @param TestDirString the directory to attempt to create and remove
        /// @details The functions under test are required to test one another. This creates an unusual
        /// set of checks that need to happen and some situations in which tests will be skipped.
        void CreateRemoveDirectory(const String& TestDirString)
        {
            Boole Exists = Mezzanine::Resource::DoesDirectoryExist(TestDirString);
            TestOutput << "Does testing directory exists: " << Exists << endl;
            if(Exists)
            {
                TestOutput << "Removing " << TestDirString << endl;
                Mezzanine::Resource::RemoveDirectory(TestDirString);
            }else{
                TestOutput << "No cleanup required" << endl;
            }

            Exists = Mezzanine::Resource::DoesDirectoryExist(TestDirString);
            if(Exists)
            {
                TEST(false, "RemoveDirectory");
                TEST_RESULT(Testing::Skipped, "CreateDirectory");
            }else{
                TestOutput << "Attempting to create a directory: " << TestDirString << endl;
                Mezzanine::Resource::CreateDirectory(TestDirString);
                Exists = Mezzanine::Resource::DoesDirectoryExist(TestDirString);
                TEST(Exists, "CreateDirectory");
                TestOutput << "Did it work: " << Exists << endl;

                if(Exists)
                {
                    TestOutput << "Testing removal of: " << TestDirString << endl;
                    Mezzanine::Resource::RemoveDirectory(TestDirString);
                    Exists = Mezzanine::Resource::DoesDirectoryExist(TestDirString);
                    TEST(!Exists, "RemoveDirectory");
                    TestOutput << "Did it work: " << !Exists << endl;
                }else{
                    TEST_RESULT(Testing::Skipped, "RemoveDirectory");
                }
            }
        }

        /// @brief Verify the basename and dirname functions does their jobs right
        /// set of checks that need to happen and some situations in which tests will be skipped.
        void TestBaseAndDirName()
        {
            TestOutput << "Testing DirName: " << endl
                 << "Passing:\t\"File.txt\"\n\tExpecting:\t\"\"\n\tActual:\t\"" << Mezzanine::Resource::DirName("File.txt") << "\"" << endl
                 << "Passing:\t\"c:\\untitled doc.txt\"\n\tExpecting:\t\"c:\\\"\n\tActual:\t\"" << Mezzanine::Resource::DirName("c:\\untitled doc.txt") << "\"" << endl
                 << "Passing:\t\"\"\n\tExpecting:\t\"\"\n\tActual:\t\"" << Mezzanine::Resource::DirName("") << "\"" << endl
                 << "Passing:\t\"/a/b/c\"\n\tExpecting:\t\"/a/b/\"\n\tActual:\t\"" << Mezzanine::Resource::DirName("/a/b/c") << "\"" << endl
                 << "Passing:\t\"/a/b/c/\"\n\tExpecting:\t\"/a/b/c/\"\n\tActual:\t\"" << Mezzanine::Resource::DirName("/a/b/c/") << "\"" << endl
                 << endl;
            Test(Mezzanine::Resource::DirName("File.txt")=="","DirNameShouldBeEmpty");
            Test(Mezzanine::Resource::DirName("c:\\untitled doc.txt")=="c:\\","DirNamewindows");
            Test(Mezzanine::Resource::DirName("")=="","DirNameEmptyNotCrash");
            Test(Mezzanine::Resource::DirName("/a/b/c")=="/a/b/","DirNameUnix");
            Test(Mezzanine::Resource::DirName("/a/b/c/")=="/a/b/c/","DirNameUnixDir");

            TestOutput << "Testing BaseName: " << endl
                 << "Passing:\t\"File.txt\"\n\tExpecting:\t\"File.txt\"\n\tActual:\t\"" << Mezzanine::Resource::BaseName("File.txt") << "\"" << endl
                 << "Passing:\t\"c:\\untitled doc.txt\"\n\tExpecting:\t\"untitled doc.txt\"\n\tActual:\t\"" << Mezzanine::Resource::BaseName("c:\\untitled doc.txt") << "\"" << endl
                 << "Passing:\t\"\"\n\tExpecting:\t\"\"\n\tActual:\t\"" << Mezzanine::Resource::BaseName("") << "\"" << endl
                 << "Passing:\t\"/a/b/c\"\n\tExpecting:\t\"c\"\n\tActual:\t\"" << Mezzanine::Resource::BaseName("/a/b/c") << "\"" << endl
                 << "Passing:\t\"/a/b/c/\"\n\tExpecting:\t\"\"\n\tActual:\t\"" << Mezzanine::Resource::BaseName("/a/b/c/") << "\"" << endl
                 << endl << endl;
            Test(Mezzanine::Resource::BaseName("File.txt")=="File.txt","BaseNameShouldCopy");
            Test(Mezzanine::Resource::BaseName("c:\\untitled doc.txt")=="untitled doc.txt","BaseNamewindows");
            Test(Mezzanine::Resource::BaseName("")=="","BaseNameEmptyNotCrash");
            Test(Mezzanine::Resource::BaseName("/a/b/c")=="c","BaseNameUnix");
            Test(Mezzanine::Resource::BaseName("/a/b/c/")=="","BaseNameUnixDir");

        }

        void TestGetExeDir()
        {
            char* ExeOnly = const_cast<char *>("game.exe");
            char** ExePtr = &ExeOnly;
            char* ExeWithDotSlash = const_cast<char *>("./game");
            char** ExeDSPtr = &ExeWithDotSlash;
            char* ExeWithwindDotSlash = const_cast<char *>(".\\game");
            char** ExewDSPtr = &ExeWithwindDotSlash;
            char* wPath = const_cast<char *>("c:\\progra~1\\fungamedir\\");
            char** PtrwPath = &wPath;
            char* uPath = const_cast<char *>("/usr/bin/");
            char** PtruPath = &uPath;
            char* Reasonablew = const_cast<char *>("c:\\fungamedir\\game.exe");
            char** PtrReasonablew = &Reasonablew;
            char* Reasonableu = const_cast<char *>("/usr/share/bin/game");
            char** PtrReasonableu = &Reasonableu;
            TestOutput << "Testing GetExecutableDirFromArg(ArgC,ArgV)" << endl
                 << "On your system with the real args this provides:\n\t\"" << Mezzanine::Resource::GetExecutableDirFromArg(ArgC,ArgV) << "\"" << endl
                 << "With empty records \n\tExpecting:\t\"\"\n\tActual:\t\"" << Mezzanine::Resource::GetExecutableDirFromArg(0,0) << "\"" << endl
                 << "With \"" << ExePtr[0] << "\" \n\tExpecting:\t\".\"\n\tActual:\t\"" << Mezzanine::Resource::GetExecutableDirFromArg(1,ExePtr) << "\"" << endl
                 << "With \"" << ExeDSPtr[0] << "\" \n\tExpecting:\t\".\"\n\tActual:\t\"" << Mezzanine::Resource::GetExecutableDirFromArg(1,ExeDSPtr) << "\"" << endl
                 << "With \"" << ExewDSPtr[0] << "\" \n\tExpecting:\t\".\"\n\tActual:\t\"" << Mezzanine::Resource::GetExecutableDirFromArg(1,ExewDSPtr) << "\"" << endl
                 << "With \"" << PtrwPath[0] << "\" \n\tExpecting:\t\"\"\n\tActual:\t\"" << Mezzanine::Resource::GetExecutableDirFromArg(1,PtrwPath) << "\"" << endl
                 << "With \"" << PtruPath[0] << "\" \n\tExpecting:\t\"\"\n\tActual:\t\"" << Mezzanine::Resource::GetExecutableDirFromArg(1,PtruPath) << "\"" << endl
                 << "With \"" << PtrReasonablew[0] << "\" \n\tExpecting:\t\"c:\\fungamedir\\\"\n\tActual:\t\"" << Mezzanine::Resource::GetExecutableDirFromArg(1,PtrReasonablew) << "\"" << endl
                 << "With \"" << PtrReasonableu[0] << "\" \n\tExpecting:\t\"/usr/share/bin/\"\n\tActual:\t\"" << Mezzanine::Resource::GetExecutableDirFromArg(1,PtrReasonableu) << "\"" << endl
                 << endl;
            TEST(Mezzanine::Resource::GetExecutableDirFromArg(0,0) == String(""),"GetExeDirEmpty");
            TEST(Mezzanine::Resource::GetExecutableDirFromArg(1,ExePtr) == String("."),"GetExeExeOnly");
            TEST(Mezzanine::Resource::GetExecutableDirFromArg(1,ExeDSPtr) == String("."),"GetExeDothSlash");
            TEST(Mezzanine::Resource::GetExecutableDirFromArg(1,ExewDSPtr) == String("."),"GetExewinDotSlash");
            TEST(Mezzanine::Resource::GetExecutableDirFromArg(1,PtrwPath) == String(""),"GetExeNoExewin");
            TEST(Mezzanine::Resource::GetExecutableDirFromArg(1,PtruPath) == String(""),"GetExeNoExenix");
            TEST(Mezzanine::Resource::GetExecutableDirFromArg(1,PtrReasonablew) == String("c:\\fungamedir\\"),"GetExeValidwin");
            TEST(Mezzanine::Resource::GetExecutableDirFromArg(1,PtrReasonableu) == String("/usr/share/bin/"),"GetExeValidnix");

            Whole Count = 10000; // set to 100000 or higher and remark out env test for faster results
            MaxInt ArgTime = 0;
            MaxInt SyscallTime = 0;
            MaxInt GetTime = 0;

            TestOutput << "Not testing GetExecutableDirFromArg(ArgC,ArgV) but here is the output so you can check if you want" << endl
                 << "On your system with the real args this provides:\n\t\"" << Mezzanine::Resource::GetExecutableDirFromArg(ArgC,ArgV) << "\"" << endl
                 << endl;
            {
                std::vector<String> CacheDefeater;
                CacheDefeater.reserve(Count);
                TestOutput << "Calling GetExecutableDirFromArg(ArgC,ArgV) " << Count << " times and timing it." << endl;
                TimedTest Timed;
                for(Whole C=0; C<Count; C++)
                    { CacheDefeater.push_back(Mezzanine::Resource::GetExecutableDirFromArg(ArgC,ArgV)); }
                Whole N = (rand()%Count);
                TestOutput << "To defeat the cache the " << N << "th call gave us \"" << CacheDefeater[N] << "\" and took ";
                ArgTime = Timed.GetLength();
                TestOutput << ArgTime << " microseconds." << endl << endl;
            }

            TestOutput << "Not testing GetExecutableDirFromSystem() but here is the output so you can check if you want" << endl
                 << "On your system this provides:\n\t\"" << Mezzanine::Resource::GetExecutableDirFromSystem() << "\"" << endl
                 << endl;
            {
                std::vector<String> CacheDefeater;
                CacheDefeater.reserve(Count);
                TestOutput << "Calling GetExecutableDirFromSystem() " << Count << " times and timing it." << endl;
                TimedTest Timed;
                for(Whole C=0; C<Count; C++)
                    { CacheDefeater.push_back(Mezzanine::Resource::GetExecutableDirFromSystem()); }
                Whole N = (rand()%Count);
                TestOutput << "To defeat the cache the " << N << "th call gave us \"" << CacheDefeater[N] << "\" and took ";
                SyscallTime = Timed.GetLength();
                TestOutput << SyscallTime << " microseconds." << endl << endl;
            }


            TestOutput << "Not testing GetExecutableDir(ArgC,ArgV) but here is the output so you can check if you want" << endl
                 << "On your system this provides:\n\t\"" << Mezzanine::Resource::GetExecutableDir(ArgC,ArgV) << "\"" << endl
                 << endl << endl;
            {
                std::vector<String> CacheDefeater;
                CacheDefeater.reserve(Count);
                TestOutput << "Calling GetExecutableDir(ArgC,ArgV) " << Count << " times and timing it." << endl;
                TimedTest Timed;
                for(Whole C=0; C<Count; C++)
                    { CacheDefeater.push_back(Mezzanine::Resource::GetExecutableDir(ArgC,ArgV)); }
                Whole N = (rand()%Count);
                TestOutput << "To defeat the cache the " << N << "th call gave us \"" << CacheDefeater[N] << "\" and took ";
                GetTime = Timed.GetLength();
                TestOutput << GetTime << " microseconds." << endl << endl;
            }

            TEST_WARN(ArgTime<SyscallTime,"ArgIsFastest")
        }

        String GetCommandResults(String Command)
        {
            Command += " > CommandResults.txt";
            system(Command.c_str());
            std::ifstream File("CommandResults.txt");
            return String( std::istreambuf_iterator<char>(File), std::istreambuf_iterator<char>());
        }

        void TestPath()
        {
            TestOutput << "Your Current PATH as parsed by GetSystemPATH():" << endl;
            StringVector PATH(Resource::GetSystemPATH());
            for(StringVector::const_iterator Iter = PATH.begin();
                Iter!=PATH.end();
                Iter++)
            {
                TestOutput << "\t\"" << *Iter << "\"" << endl;
            }
            TestOutput << "End of current system PATH" << endl;


            std::stringstream SamplePath;
            SamplePath << "/a/b/c" << Resource::GetPathSeparator()
                       << "/bin" << Resource::GetPathSeparator()
                       << Resource::GetPathSeparator();

            TestOutput << "Test parsing a sample PATH: " << SamplePath.str() << endl
                 << "Becomes: ";
            PATH=Resource::GetSystemPATH(SamplePath.str());
            for(StringVector::const_iterator Iter = PATH.begin();
                Iter!=PATH.end();
                Iter++)
            {
                TestOutput << "\t\"" << *Iter << "\"" << endl;
            }
            TEST(PATH[0]=="/a/b/c"&&PATH[1]=="/bin"&&PATH[2]=="", "PATHParsing");

            TestOutput << "looking for \"ls\" and comparing our results to the system"  << endl
                 << "\t\"" << (Resource::Which("ls")) << "\"" << endl
                 << "\t\"" << Resource::DirName(GetCommandResults("which ls")) << "\"" << endl;
            TEST_WARN((Resource::Which("ls"))==Resource::DirName(GetCommandResults("which ls")),"Whichls");

            //Does Windows have which, what does where's output look like?
            TestOutput << "looking for \"cmd.exe\" and comparing our results to the system"  << endl
                 << "\t\"" << (Resource::Which("cmd.exe")) << "\"" << endl
                 << "\t\"" << Resource::DirName(GetCommandResults("which cmd.exe")) << "\"" << endl;
            TEST_WARN((Resource::Which("cmd.exe"))==Resource::DirName(GetCommandResults("which cmd.exe")),"Whichcmd");
        }

        void TestChangeDir()
        {
            TestOutput << "Testing Directory Changing." << endl;
            Mezzanine::Resource::CreateDirectory("Change");
            String StartDir(Resource::GetWorkingDirectory());
            String TargetDir(StartDir+Resource::GetDirectorySeparator()+String("Change"));

            TestOutput << "Current directory: " << Resource::GetWorkingDirectory() << endl;
            TestOutput << "\tExpected: " << StartDir << endl;
            Mezzanine::Resource::ChangeWorkingDirectory("Change");
            Test(TargetDir==Resource::GetWorkingDirectory(),"CDRelative");
            TestOutput << "Changing with relative path (cd Change): " << Resource::GetWorkingDirectory() << endl;
            TestOutput << "\tExpected: " << TargetDir << endl;

            Mezzanine::Resource::ChangeWorkingDirectory("..");
            Test(StartDir==Resource::GetWorkingDirectory(),"CDDotDot");
            TestOutput << "Changing back with relative path (cd ..): " << Resource::GetWorkingDirectory() << endl;
            TestOutput << "\tExpected: " << StartDir << endl;


            Mezzanine::Resource::ChangeWorkingDirectory(TargetDir);
            TestOutput << "Changing with absolute path (cd /a/b/Change): " << Resource::GetWorkingDirectory() << endl;
            TestOutput << "\tExpected: " << TargetDir << endl;
            Test(TargetDir==Resource::GetWorkingDirectory(),"CDAbsolute");

            Mezzanine::Resource::ChangeWorkingDirectory(StartDir);
            Test(StartDir==Resource::GetWorkingDirectory(),"CDAbsolute2");
            TestOutput << "Changing back with absolute path (cd /a/b/Change): " << Resource::GetWorkingDirectory() << endl;
            TestOutput << "\tExpected: " << StartDir << endl;


        }

        /// @brief This is called when Automatic tests are run
        void RunAutomaticTests()
        {
            String TestDirString(TestingScratchDir);
            TestDirString += "resourcetesting";

            try
            {
                CreateRemoveDirectory(TestDirString);
            } catch (Mezzanine::IOException& e) {
                TestOutput << "Error testing directory creation and removal: " << e.what() << endl;
            }
            TestOutput << endl;

            TestBaseAndDirName();

            TestGetExeDir();

            TestPath();

            TestChangeDir();

            //TestOutput << "Attempting to create a Mezzanine::ResourceManager" << endl;
            //Mezzanine::ResourceManager res;
            //TEST(true,"ConstructionDidNotFailMiserably");

        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }


        /// @brief asks a few question about the location of the executable dir, because otherwise we don't know
        void RunInteractiveTests()
        {
            Logger Temp;

            Temp << "GetExecutableDirFromSystem() - On your system with the real args this provides:\n\t\""
                 << Mezzanine::Resource::GetExecutableDirFromSystem() << "\"" << endl
                 << "Is that location correct? " ;
            TEST_RESULT(GetTestAnswerFromStdin(Temp.str()), "GetExecutableDirFromSystem()");

            Temp.str("");
            Temp << "GetExecutableDir(ArgC,ArgV) - On your system with the real args this provides:\n\t\""
                 << Mezzanine::Resource::GetExecutableDir(ArgC,ArgV) << "\"" << endl
                 << "Is that location correct? " ;
            TEST_RESULT(GetTestAnswerFromStdin(Temp.str()), "GetExecutableDir(ArgC,ArgV)");
        }

        /// @brief Since RunInteractiveTests is implemented so is this.
        /// @return returns true
        virtual bool HasInteractiveTests() const
            { return true; }

};

#endif


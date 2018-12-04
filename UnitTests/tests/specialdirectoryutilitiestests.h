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
#ifndef _specialdirectoryutilitiestests_h
#define _specialdirectoryutilitiestests_h

#include "mezztest.h"

#include "Resource/specialdirectoryutilities.h"

/// @file
/// @brief A few tests of some utilities that work with special platform directories.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A few tests covering utilities that work with special platform directories.
class specialdirectoryutilitiestests : public UnitTestGroup
{
public:
    /// @copydoc Testing::UnitTestGroup::Name
    /// @return Returns a String containing "SpecialDirectoryUtilities"
    virtual String Name()
        { return String("SpecialDirectoryUtilities"); }

    /// @brief This is called when Automatic tests are run
    void RunAutomaticTests()
    {
        {//ExecutableDir
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
            TestOutput << "Testing GetExecutableDirFromArg(ArgC,ArgV)" << std::endl
                 << "On your system with the real args this provides:\n\t\"" << Resource::GetExecutableDirFromArg(GetMainArgumentCount(), GetMainArgumentVector()) << "\"" << std::endl
                 << "With empty records \n\tExpecting:\t\"\"\n\tActual:\t\"" << Resource::GetExecutableDirFromArg(0,0) << "\"" << std::endl
                 << "With \"" << ExePtr[0] << "\" \n\tExpecting:\t\".\"\n\tActual:\t\"" << Resource::GetExecutableDirFromArg(1,ExePtr) << "\"" << std::endl
                 << "With \"" << ExeDSPtr[0] << "\" \n\tExpecting:\t\".\"\n\tActual:\t\"" << Resource::GetExecutableDirFromArg(1,ExeDSPtr) << "\"" << std::endl
                 << "With \"" << ExewDSPtr[0] << "\" \n\tExpecting:\t\".\"\n\tActual:\t\"" << Resource::GetExecutableDirFromArg(1,ExewDSPtr) << "\"" << std::endl
                 << "With \"" << PtrwPath[0] << "\" \n\tExpecting:\t\"\"\n\tActual:\t\"" << Resource::GetExecutableDirFromArg(1,PtrwPath) << "\"" << std::endl
                 << "With \"" << PtruPath[0] << "\" \n\tExpecting:\t\"\"\n\tActual:\t\"" << Resource::GetExecutableDirFromArg(1,PtruPath) << "\"" << std::endl
                 << "With \"" << PtrReasonablew[0] << "\" \n\tExpecting:\t\"c:\\fungamedir\\\"\n\tActual:\t\"" << Resource::GetExecutableDirFromArg(1,PtrReasonablew) << "\"" << std::endl
                 << "With \"" << PtrReasonableu[0] << "\" \n\tExpecting:\t\"/usr/share/bin/\"\n\tActual:\t\"" << Resource::GetExecutableDirFromArg(1,PtrReasonableu) << "\"" << std::endl
                 << std::endl;
            TEST(Resource::GetExecutableDirFromArg(0,0) == String(""),"GetExeDirEmpty");
            TEST(Resource::GetExecutableDirFromArg(1,ExePtr) == String("."),"GetExeExeOnly");
            TEST(Resource::GetExecutableDirFromArg(1,ExeDSPtr) == String("."),"GetExeDothSlash");
            TEST(Resource::GetExecutableDirFromArg(1,ExewDSPtr) == String("."),"GetExewinDotSlash");
            TEST(Resource::GetExecutableDirFromArg(1,PtrwPath) == String(""),"GetExeNoExeWin");
            TEST(Resource::GetExecutableDirFromArg(1,PtruPath) == String(""),"GetExeNoExeNix");
            TEST(Resource::GetExecutableDirFromArg(1,PtrReasonablew) == String("c:\\fungamedir\\"),"GetExeValidWin");
            TEST(Resource::GetExecutableDirFromArg(1,PtrReasonableu) == String("/usr/share/bin/"),"GetExeValidNix");

            Whole Count = 10000; // set to 100000 or higher and remark out env test for faster results
            MaxInt ArgTime = 0;
            MaxInt SyscallTime = 0;
            MaxInt GetTime = 0;

            TestOutput << "Not testing GetExecutableDirFromArg(ArgC,ArgV) but here is the output so you can check if you want" << std::endl
                 << "On your system with the real args this provides:\n\t\"" << Resource::GetExecutableDirFromArg(GetMainArgumentCount(), GetMainArgumentVector()) << "\"" << std::endl
                 << std::endl;
            {
                std::vector<String> CacheDefeater;
                CacheDefeater.reserve(Count);
                TestOutput << "Calling GetExecutableDirFromArg(ArgC,ArgV) " << Count << " times and timing it." << std::endl;
                TimedTest Timed;
                for(Whole C=0; C<Count; C++)
                    { CacheDefeater.push_back(Resource::GetExecutableDirFromArg(GetMainArgumentCount(),GetMainArgumentVector())); }
                Whole N = (rand()%Count);
                TestOutput << "To defeat the cache the " << N << "th call gave us \"" << CacheDefeater[N] << "\" and took ";
                ArgTime = Timed.GetLength();
                TestOutput << ArgTime << " microseconds." << std::endl << std::endl;
            }

            TestOutput << "Not testing GetExecutableDirFromSystem() but here is the output so you can check if you want" << std::endl
                 << "On your system this provides:\n\t\"" << Resource::GetExecutableDirFromSystem() << "\"" << std::endl
                 << std::endl;
            {
                std::vector<String> CacheDefeater;
                CacheDefeater.reserve(Count);
                TestOutput << "Calling GetExecutableDirFromSystem() " << Count << " times and timing it." << std::endl;
                TimedTest Timed;
                for(Whole C=0; C<Count; C++)
                    { CacheDefeater.push_back(Resource::GetExecutableDirFromSystem()); }
                Whole N = (rand()%Count);
                TestOutput << "To defeat the cache the " << N << "th call gave us \"" << CacheDefeater[N] << "\" and took ";
                SyscallTime = Timed.GetLength();
                TestOutput << SyscallTime << " microseconds." << std::endl << std::endl;
            }


            TestOutput << "Not testing GetExecutableDir(ArgC,ArgV) but here is the output so you can check if you want" << std::endl
                 << "On your system this provides:\n\t\"" << Resource::GetExecutableDir(GetMainArgumentCount(),GetMainArgumentVector()) << "\"" << std::endl
                 << std::endl << std::endl;
            {
                std::vector<String> CacheDefeater;
                CacheDefeater.reserve(Count);
                TestOutput << "Calling GetExecutableDir(ArgC,ArgV) " << Count << " times and timing it." << std::endl;
                TimedTest Timed;
                for(Whole C=0; C<Count; C++)
                    { CacheDefeater.push_back(Resource::GetExecutableDir(GetMainArgumentCount(),GetMainArgumentVector())); }
                Whole N = (rand()%Count);
                TestOutput << "To defeat the cache the " << N << "th call gave us \"" << CacheDefeater[N] << "\" and took ";
                GetTime = Timed.GetLength();
                TestOutput << GetTime << " microseconds." << std::endl << std::endl;
            }

            TEST_WARN( ArgTime < SyscallTime,"ArgIsFastest");
        }//ExecutableDir

        {//WorkingDir
            TestOutput << "Testing Directory Changing." << std::endl;
            Resource::CreateDirectory("Change");
            String StartDir(Resource::GetWorkingDirectory());
            String TargetDir(StartDir+Resource::GetDirectorySeparator_Host()+String("Change"));

            TestOutput << "Current directory: " << Resource::GetWorkingDirectory() << std::endl;
            TestOutput << "\tExpected: " << StartDir << std::endl;
            Resource::ChangeWorkingDirectory("Change");
            Test(TargetDir==Resource::GetWorkingDirectory(),"CDRelative");
            TestOutput << "Changing with relative path (cd Change): " << Resource::GetWorkingDirectory() << std::endl;
            TestOutput << "\tExpected: " << TargetDir << std::endl;

            Resource::ChangeWorkingDirectory("..");
            Test(StartDir==Resource::GetWorkingDirectory(),"CDDotDot");
            TestOutput << "Changing back with relative path (cd ..): " << Resource::GetWorkingDirectory() << std::endl;
            TestOutput << "\tExpected: " << StartDir << std::endl;


            Resource::ChangeWorkingDirectory(TargetDir);
            TestOutput << "Changing with absolute path (cd /a/b/Change): " << Resource::GetWorkingDirectory() << std::endl;
            TestOutput << "\tExpected: " << TargetDir << std::endl;
            Test(TargetDir==Resource::GetWorkingDirectory(),"CDAbsolute");

            Resource::ChangeWorkingDirectory(StartDir);
            Test(StartDir==Resource::GetWorkingDirectory(),"CDAbsolute2");
            TestOutput << "Changing back with absolute path (cd /a/b/Change): " << Resource::GetWorkingDirectory() << std::endl;
            TestOutput << "\tExpected: " << StartDir << std::endl;

            Resource::RemoveDirectory("Change");
        }//WorkingDir
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
             << Resource::GetExecutableDirFromSystem() << "\"" << std::endl
             << "Is that location correct? " ;
        TEST_RESULT(GetTestAnswerFromStdin(Temp.str()), "GetExecutableDirFromSystem()");

        Temp.str("");
        Temp << "GetExecutableDir(ArgC,ArgV) - On your system with the real args this provides:\n\t\""
             << Resource::GetExecutableDir(GetMainArgumentCount(),GetMainArgumentVector()) << "\"" << std::endl
             << "Is that location correct? " ;
        TEST_RESULT(GetTestAnswerFromStdin(Temp.str()), "GetExecutableDir(ArgC,ArgV)");
    }

    /// @brief Since RunInteractiveTests is implemented so is this.
    /// @return returns true
    virtual bool HasInteractiveTests() const
        { return true; }
};

#endif

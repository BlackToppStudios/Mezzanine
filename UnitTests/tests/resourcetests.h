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
#ifndef _resourcetests_h
#define _resourcetests_h

#include "mezztest.h"

#include "Resource/resourceutilities.h"
#include "Resource/resourcemanager.h"

#include <fstream>

#ifdef CopyFile
#undef CopyFile
#endif

#ifdef MoveFile
#undef MoveFile
#endif

#ifdef DeleteFile
#undef DeleteFile
#endif

#ifdef CreateDirectory
#undef CreateDirectory
#endif

#ifdef RemoveDirectory
#undef RemoveDirectory
#endif

/// @file
/// @brief A few tests of the resource system, only partial coverage

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A few tests covering primarily directory management.
class resourcetests : public UnitTestGroup
{
    public:
        /// @copydoc Testing::UnitTestGroup::Name
        /// @return Returns a String containing "Resource"
        virtual String Name()
            { return String("Resource"); }

        /// @brief Tests only file utility functions.
        void TestFileUtilities()
        {
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
        }

        /// @brief Tests the directory contents functions.
        void TestDirectoryContents()
        {
            String FileData1 = "This is a simple test file.";
            String FileData2 = "This is a larger test file text for testing.";
            String FileData3 = "How much wood would a woodchuck chuck if a woodchuck could chuck wood?";
            String FileData4 = "When in the Course of human events, it becomes necessary for one people to dissolve "
                               "the political bands which have connected them with another, and to assume among the "
                               "powers of the earth, the separate and equal station to which the Laws of Nature and "
                               "of Nature's God entitle them, a decent respect to the opinions of mankind requires "
                               "that they should declare the causes which impel them to the separation.";

            Resource::CreateDirectory("Content/");
            {// GetDirectoryContentNames
                std::ofstream NameTestFile1;
                NameTestFile1.open("./Content/NameTestFile1.txt");
                NameTestFile1 << FileData1;
                NameTestFile1.close();
                std::ofstream NameTestFile2;
                NameTestFile2.open("./Content/NameTestFile2.txt");
                NameTestFile2 << FileData2;
                NameTestFile2.close();
                std::ofstream NameTestFile3;
                NameTestFile3.open("./Content/NameTestFile3.txt");
                NameTestFile3 << FileData3;
                NameTestFile3.close();
                std::ofstream NameTestFile4;
                NameTestFile4.open("./Content/NameTestFile4.txt");
                NameTestFile4 << FileData4;
                NameTestFile4.close();

                Resource::CreateDirectory("Content/TestDir/");

                StringVector ContentNames = std::move( Resource::GetDirectoryContentNames("Content/") );
                TEST(ContentNames.size() == 5,"GetDirectoryContentNames-Count");
                TEST(std::find(ContentNames.begin(),ContentNames.end(),"TestDir") != ContentNames.end(),"GetDirectoryContentNames-First");
                TEST(std::find(ContentNames.begin(),ContentNames.end(),"NameTestFile1.txt") != ContentNames.end(),"GetDirectoryContentNames-Second");
                TEST(std::find(ContentNames.begin(),ContentNames.end(),"NameTestFile2.txt") != ContentNames.end(),"GetDirectoryContentNames-Third");
                TEST(std::find(ContentNames.begin(),ContentNames.end(),"NameTestFile3.txt") != ContentNames.end(),"GetDirectoryContentNames-Fourth");
                TEST(std::find(ContentNames.begin(),ContentNames.end(),"NameTestFile4.txt") != ContentNames.end(),"GetDirectoryContentNames-Fifth");

                Resource::RemoveFile("./Content/NameTestFile1.txt");
                Resource::RemoveFile("./Content/NameTestFile2.txt");
                Resource::RemoveFile("./Content/NameTestFile3.txt");
                Resource::RemoveFile("./Content/NameTestFile4.txt");
                Resource::RemoveDirectory("Content/TestDir/");
            }// GetDirectoryContentNames

            {// GetDirectoryContents
                using ArchiveEntry = Resource::ArchiveEntry;
                using ArchiveEntryVector = Resource::ArchiveEntryVector;
                using ArchiveEntryIterator = ArchiveEntryVector::iterator;

                std::ofstream ContentTestFile2;
                ContentTestFile2.open("./Content/ContentTestFile2.txt");
                ContentTestFile2 << FileData2;
                ContentTestFile2.close();
                std::ofstream ContentTestFile3;
                ContentTestFile3.open("./Content/ContentTestFile3.txt");
                ContentTestFile3 << FileData3;
                ContentTestFile3.close();
                std::ofstream ContentTestFile4;
                ContentTestFile4.open("./Content/ContentTestFile4.txt");
                ContentTestFile4 << FileData4;
                ContentTestFile4.close();

                Resource::CreateDirectory("Content/TestDir/");

                ArchiveEntryVector ContentEntries = std::move( Resource::GetDirectoryContents("Content/") );
                TEST(ContentEntries.size() == 4,"GetDirectoryContents-Count");
                ArchiveEntryIterator EntryIt = ContentEntries.end();
                EntryIt = std::find_if(ContentEntries.begin(),ContentEntries.end(),[](const ArchiveEntry& Entry){
                    return ( Entry.Name == "TestDir" );
                });
                TEST(EntryIt != ContentEntries.end(),"GetDirectoryContents-FirstFound");
                if( EntryIt != ContentEntries.end() ) {
                    TEST((*EntryIt).ArchType == Resource::AT_FileSystem,"GetDirectoryContents-FirstArchiveType");
                    TEST((*EntryIt).EntType == Resource::ET_Directory,"GetDirectoryContents-FirstEntryType");
                    TEST((*EntryIt).Name == "TestDir","GetDirectoryContents-FirstName");
                    TEST((*EntryIt).Size == 0,"GetDirectoryContents-FirstSize");
                }else{
                    TEST(false,"GetDirectoryContents-FirstArchiveType");
                    TEST(false,"GetDirectoryContents-FirstEntryType");
                    TEST(false,"GetDirectoryContents-FirstName");
                    TEST(false,"GetDirectoryContents-FirstSize");
                }
                EntryIt = std::find_if(ContentEntries.begin(),ContentEntries.end(),[](const ArchiveEntry& Entry){
                    return ( Entry.Name == "ContentTestFile2.txt" );
                });
                TEST(EntryIt != ContentEntries.end(),"GetDirectoryContents-SecondFound");
                if( EntryIt != ContentEntries.end() ) {
                    TEST((*EntryIt).ArchType == Resource::AT_FileSystem,"GetDirectoryContents-SecondArchiveType");
                    TEST((*EntryIt).EntType == Resource::ET_File,"GetDirectoryContents-SecondEntryType");
                    TEST((*EntryIt).Name == "ContentTestFile2.txt","GetDirectoryContents-SecondName");
                    TEST((*EntryIt).Size == FileData2.size(),"GetDirectoryContents-SecondSize");
                }else{
                    TEST(false,"GetDirectoryContents-SecondArchiveType");
                    TEST(false,"GetDirectoryContents-SecondEntryType");
                    TEST(false,"GetDirectoryContents-SecondName");
                    TEST(false,"GetDirectoryContents-SecondSize");
                }
                EntryIt = std::find_if(ContentEntries.begin(),ContentEntries.end(),[](const ArchiveEntry& Entry){
                    return ( Entry.Name == "ContentTestFile3.txt" );
                });
                TEST(EntryIt != ContentEntries.end(),"GetDirectoryContents-ThirdFound");
                if( EntryIt != ContentEntries.end() ) {
                    TEST((*EntryIt).ArchType == Resource::AT_FileSystem,"GetDirectoryContents-ThirdArchiveType");
                    TEST((*EntryIt).EntType == Resource::ET_File,"GetDirectoryContents-ThirdEntryType");
                    TEST((*EntryIt).Name == "ContentTestFile3.txt","GetDirectoryContents-ThirdName");
                    TEST((*EntryIt).Size == FileData3.size(),"GetDirectoryContents-ThirdSize");
                }else{
                    TEST(false,"GetDirectoryContents-ThirdArchiveType");
                    TEST(false,"GetDirectoryContents-ThirdEntryType");
                    TEST(false,"GetDirectoryContents-ThirdName");
                    TEST(false,"GetDirectoryContents-ThirdSize");
                }
                EntryIt = std::find_if(ContentEntries.begin(),ContentEntries.end(),[](const ArchiveEntry& Entry){
                    return ( Entry.Name == "ContentTestFile4.txt" );
                });
                TEST(EntryIt != ContentEntries.end(),"GetDirectoryContents-FourthFound");
                if( EntryIt != ContentEntries.end() ) {
                    TEST((*EntryIt).ArchType == Resource::AT_FileSystem,"GetDirectoryContents-FourthArchiveType");
                    TEST((*EntryIt).EntType == Resource::ET_File,"GetDirectoryContents-FourthEntryType");
                    TEST((*EntryIt).Name == "ContentTestFile4.txt","GetDirectoryContents-FourthName");
                    TEST((*EntryIt).Size == FileData4.size(),"GetDirectoryContents-FourthSize");
                }else{
                    TEST(false,"GetDirectoryContents-FourthArchiveType");
                    TEST(false,"GetDirectoryContents-FourthEntryType");
                    TEST(false,"GetDirectoryContents-FourthName");
                    TEST(false,"GetDirectoryContents-FourthSize");
                }

                Resource::RemoveFile("./Content/ContentTestFile2.txt");
                Resource::RemoveFile("./Content/ContentTestFile3.txt");
                Resource::RemoveFile("./Content/ContentTestFile4.txt");
                Resource::RemoveDirectory("Content/TestDir/");
            }// GetDirectoryContents
            Resource::RemoveDirectory("Content/");
        }

        /// @brief Tests only directory utility functions.
        /// @param TestDirString The directory to attempt to create and remove.
        /// @details The functions under test are required to test one another. This creates an unusual
        /// set of checks that need to happen and some situations in which tests will be skipped.
        void TestDirectoryUtilities(const String& TestDirString)
        {
            Boole Exists = Resource::DirectoryExists(TestDirString);
            TestOutput << "Testing whether directory exists: " << Exists << std::endl;
            if(Exists) {
                TestOutput << "Removing " << TestDirString << std::endl;
                Resource::RemoveDirectory(TestDirString);
            }else{
                TestOutput << "No cleanup required" << std::endl;
            }

            Exists = Resource::DirectoryExists(TestDirString);
            if(Exists) {
                TEST(false, "RemoveDirectory");
                TEST_RESULT(Testing::Skipped, "CreateDirectory");
            }else{
                TestOutput << "Attempting to create a directory: " << TestDirString << std::endl;
                TEST(Resource::CreateDirectory(TestDirString), "CreateDirectory");
                Exists = Resource::DirectoryExists(TestDirString);
                TEST(Exists, "DirectoryExists");
                TestOutput << "Did it work: " << Exists << std::endl;

                if(Exists) {
                    TestOutput << "Testing removal of: " << TestDirString << std::endl;
                    Resource::RemoveDirectory(TestDirString);
                    Exists = Resource::DirectoryExists(TestDirString);
                    TEST(!Exists, "RemoveDirectory");
                    TestOutput << "Did it work: " << !Exists << std::endl;
                }else{
                    TEST_RESULT(Testing::Skipped, "RemoveDirectory");
                }
            }

            TestOutput << "Testing the creation of multiple directories in a path." << std::endl;
            String BaseDir = "./";
            String Depth1 = "Depth1/";
            String Depth2 = "Depth2/";
            String Depth3 = "Depth3/";
            String DepthDir = BaseDir + Depth1 + Depth2 + Depth3;

            TestOutput << "Creating directory path: " << DepthDir << std::endl;
            TEST(Resource::CreateDirectoryPath(DepthDir),"CreateDirectoryPath");
            TEST(Resource::DirectoryExists(DepthDir),"VerifyCreateDirectoryPath");
            TestOutput << "Cleaning up directories." << std::endl;
            Resource::RemoveDirectory(BaseDir + Depth1 + Depth2 + Depth3);
            Resource::RemoveDirectory(BaseDir + Depth1 + Depth2);
            Resource::RemoveDirectory(BaseDir + Depth1);
        }

        /// @brief Verify the basename and dirname functions does their jobs right
        /// set of checks that need to happen and some situations in which tests will be skipped.
        void TestBaseAndDirName()
        {
            TestOutput << "Testing GetDirName: " << std::endl
                 << "Passing:\t\"File.txt\"\n\tExpecting:\t\"\"\n\tActual:\t\"" << Resource::GetDirName("File.txt") << "\"" << std::endl
                 << "Passing:\t\"c:\\untitled doc.txt\"\n\tExpecting:\t\"c:\\\"\n\tActual:\t\"" << Resource::GetDirName("c:\\untitled doc.txt") << "\"" << std::endl
                 << "Passing:\t\"\"\n\tExpecting:\t\"\"\n\tActual:\t\"" << Resource::GetDirName("") << "\"" << std::endl
                 << "Passing:\t\"/a/b/c\"\n\tExpecting:\t\"/a/b/\"\n\tActual:\t\"" << Resource::GetDirName("/a/b/c") << "\"" << std::endl
                 << "Passing:\t\"/a/b/c/\"\n\tExpecting:\t\"/a/b/c/\"\n\tActual:\t\"" << Resource::GetDirName("/a/b/c/") << "\"" << std::endl
                 << std::endl;
            Test(Resource::GetDirName("File.txt")=="","GetDirName-ShouldBeEmptyCStr");
            Test(Resource::GetDirName("c:\\untitled doc.txt")=="c:\\","GetDirName-WindowsCStr");
            Test(Resource::GetDirName("")=="","GetDirName-EmptyNotCrashCStr");
            Test(Resource::GetDirName("/a/b/c")=="/a/b/","GetDirName-UnixCStr");
            Test(Resource::GetDirName("/a/b/c/")=="/a/b/c/","GetDirName-UnixDirCStr");
            Test(Resource::GetDirName(String("File.txt"))=="","GetDirName-ShouldBeEmpty");
            Test(Resource::GetDirName(String("c:\\untitled doc.txt"))=="c:\\","GetDirName-Windows");
            Test(Resource::GetDirName(String(""))=="","GetDirName-EmptyNotCrash");
            Test(Resource::GetDirName(String("/a/b/c"))=="/a/b/","GetDirName-Unix");
            Test(Resource::GetDirName(String("/a/b/c/"))=="/a/b/c/","GetDirName-UnixDir");

            TestOutput << "Testing GetBaseName: " << std::endl
                 << "Passing:\t\"File.txt\"\n\tExpecting:\t\"File.txt\"\n\tActual:\t\"" << Resource::GetBaseName("File.txt") << "\"" << std::endl
                 << "Passing:\t\"c:\\untitled doc.txt\"\n\tExpecting:\t\"untitled doc.txt\"\n\tActual:\t\"" << Resource::GetBaseName("c:\\untitled doc.txt") << "\"" << std::endl
                 << "Passing:\t\"\"\n\tExpecting:\t\"\"\n\tActual:\t\"" << Resource::GetBaseName("") << "\"" << std::endl
                 << "Passing:\t\"/a/b/c\"\n\tExpecting:\t\"c\"\n\tActual:\t\"" << Resource::GetBaseName("/a/b/c") << "\"" << std::endl
                 << "Passing:\t\"/a/b/c/\"\n\tExpecting:\t\"\"\n\tActual:\t\"" << Resource::GetBaseName("/a/b/c/") << "\"" << std::endl
                 << std::endl << std::endl;
            Test(Resource::GetBaseName("File.txt")=="File.txt","GetBaseName-ShouldCopyCStr");
            Test(Resource::GetBaseName("c:\\untitled doc.txt")=="untitled doc.txt","GetBaseName-WindowsCStr");
            Test(Resource::GetBaseName("")=="","GetBaseName-EmptyNotCrashCStr");
            Test(Resource::GetBaseName("/a/b/c")=="c","GetBaseName-UnixCStr");
            Test(Resource::GetBaseName("/a/b/c/")=="","GetBaseName-UnixDirCStr");
            Test(Resource::GetBaseName(String("File.txt"))=="File.txt","GetBaseName-ShouldCopy");
            Test(Resource::GetBaseName(String("c:\\untitled doc.txt"))=="untitled doc.txt","GetBaseName-Windows");
            Test(Resource::GetBaseName(String(""))=="","GetBaseName-EmptyNotCrash");
            Test(Resource::GetBaseName(String("/a/b/c"))=="c","GetBaseName-Unix");
            Test(Resource::GetBaseName(String("/a/b/c/"))=="","GetBaseName-UnixDir");
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
            TEST(Resource::GetExecutableDirFromArg(1,PtrwPath) == String(""),"GetExeNoExewin");
            TEST(Resource::GetExecutableDirFromArg(1,PtruPath) == String(""),"GetExeNoExenix");
            TEST(Resource::GetExecutableDirFromArg(1,PtrReasonablew) == String("c:\\fungamedir\\"),"GetExeValidwin");
            TEST(Resource::GetExecutableDirFromArg(1,PtrReasonableu) == String("/usr/share/bin/"),"GetExeValidnix");

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

            TEST_WARN(ArgTime<SyscallTime,"ArgIsFastest")
        }

        void TestPath()
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
            SamplePath << "/a/b/c" << Resource::GetPathSeparator()
                       << "/bin" << Resource::GetPathSeparator()
                       << Resource::GetPathSeparator();

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

        void TestChangeDir()
        {
            TestOutput << "Testing Directory Changing." << std::endl;
            Resource::CreateDirectory("Change");
            String StartDir(Resource::GetWorkingDirectory());
            String TargetDir(StartDir+Resource::GetPlatformDirectorySeparator()+String("Change"));

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
        }

        /// @brief This is called when Automatic tests are run
        void RunAutomaticTests()
        {
            String TestDirString(TestingScratchDir);
            TestDirString += "resourcetesting";

            try{
                TestDirectoryUtilities(TestDirString);
            } catch (IOException& e) {
                TestOutput << "Error testing directory creation and removal: " << e.what() << std::endl;
            }
            TestOutput << std::endl;

            TestFileUtilities();

            TestDirectoryContents();

            TestBaseAndDirName();

            TestGetExeDir();

            TestPath();

            TestChangeDir();

            //TestOutput << "Attempting to create a ResourceManager" << std::endl;
            //ResourceManager res;
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


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
#ifndef _pathutilitiestests_h
#define _pathutilitiestests_h

#include "mezztest.h"

#include "Resource/pathutilities.h"

/// @file
/// @brief A few tests of some utilities that work with strings relating to a resource path.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A few tests covering utilities that work with strings relating to a resource path.
class pathutilitiestests : public UnitTestGroup
{
public:
    /// @copydoc Testing::UnitTestGroup::Name
    /// @return Returns a String containing "PathUtilities"
    virtual String Name()
        { return String("PathUtilities"); }

    /// @brief This is called when Automatic tests are run
    void RunAutomaticTests()
    {
        {// Dir and Base Name
            TEST(Resource::GetDirName("File.txt")=="","GetDirName-ShouldBeEmptyCStr");
            TEST(Resource::GetDirName("c:\\untitled doc.txt")=="c:\\","GetDirName-WindowsCStr");
            TEST(Resource::GetDirName("")=="","GetDirName-EmptyNotCrashCStr");
            TEST(Resource::GetDirName("/a/b/c")=="/a/b/","GetDirName-UnixCStr");
            TEST(Resource::GetDirName("/a/b/c/")=="/a/b/c/","GetDirName-UnixDirCStr");
            TEST(Resource::GetDirName(String("File.txt"))=="","GetDirName-ShouldBeEmpty");
            TEST(Resource::GetDirName(String("c:\\untitled doc.txt"))=="c:\\","GetDirName-Windows");
            TEST(Resource::GetDirName(String(""))=="","GetDirName-EmptyNotCrash");
            TEST(Resource::GetDirName(String("/a/b/c"))=="/a/b/","GetDirName-Unix");
            TEST(Resource::GetDirName(String("/a/b/c/"))=="/a/b/c/","GetDirName-UnixDir");

            TEST(Resource::GetBaseName("File.txt")=="File.txt","GetBaseName-ShouldCopyCStr");
            TEST(Resource::GetBaseName("c:\\untitled doc.txt")=="untitled doc.txt","GetBaseName-WindowsCStr");
            TEST(Resource::GetBaseName("")=="","GetBaseName-EmptyNotCrashCStr");
            TEST(Resource::GetBaseName("/a/b/c")=="c","GetBaseName-UnixCStr");
            TEST(Resource::GetBaseName("/a/b/c/")=="","GetBaseName-UnixDirCStr");
            TEST(Resource::GetBaseName(String("File.txt"))=="File.txt","GetBaseName-ShouldCopy");
            TEST(Resource::GetBaseName(String("c:\\untitled doc.txt"))=="untitled doc.txt","GetBaseName-Windows");
            TEST(Resource::GetBaseName(String(""))=="","GetBaseName-EmptyNotCrash");
            TEST(Resource::GetBaseName(String("/a/b/c"))=="c","GetBaseName-Unix");
            TEST(Resource::GetBaseName(String("/a/b/c/"))=="","GetBaseName-UnixDir");
        }// Dir and Base Name

        {// Separators
            TEST(Resource::IsDirectorySeparator('/'),"IsDirectorySeparator(const_Char8)-Pass");
            TEST(!Resource::IsDirectorySeparator('@'),"IsDirectorySeparator(const_Char8)-Fail");
            TEST(Resource::IsDirectorySeparator_Posix('/'),"IsDirectorySeparator_Posix(const_Char8)-Pass");
            TEST(!Resource::IsDirectorySeparator_Posix('\\'),"IsDirectorySeparator_Posix(const_Char8)-Fail");
            TEST(Resource::IsDirectorySeparator_Windows('\\'),"IsDirectorySeparator_Windows(const_Char8)-Pass");
            TEST(!Resource::IsDirectorySeparator_Windows('/'),"IsDirectorySeparator_Windows(const_Char8)-Fail");
            TEST(Resource::GetDirectorySeparator_Posix() == '/',"GetDirectorySeparator_Posix()");
            TEST(Resource::GetDirectorySeparator_Windows() == '\\',"GetDirectorySeparator_Windows()");
            TEST(Resource::GetDirectorySeparator_Universal() == '/',"GetDirectorySeparator_Universal()");
        #ifdef MEZZ_WINDOWS
            TEST(Resource::IsDirectorySeparator_Host('\\'),"IsDirectorySeparator_Host(const_Char8)-Pass");
            TEST(!Resource::IsDirectorySeparator_Host('/'),"IsDirectorySeparator_Host(const_Char8)-Fail");
            TEST(Resource::GetDirectorySeparator_Host() == '\\',"GetDirectorySeparator_Host()");
        #else
            TEST(Resource::IsDirectorySeparator_Host('/'),"IsDirectorySeparator_Host(const_Char8)-Pass");
            TEST(!Resource::IsDirectorySeparator_Host('\\'),"IsDirectorySeparator_Host(const_Char8)-Fail");
            TEST(Resource::GetDirectorySeparator_Host() == '/',"GetDirectorySeparator_Host()");
        #endif

            TEST(Resource::IsPathSeparator(';'),"IsPathSeparator(const_Char8)-Pass");
            TEST(!Resource::IsPathSeparator('&'),"IsPathSeparator(const_Char8)-Fail");
            TEST(Resource::IsPathSeparator_Posix(':'),"IsPathSeparator_Posix(const_Char8)-Pass");
            TEST(!Resource::IsPathSeparator_Posix(';'),"IsPathSeparator_Posix(const_Char8)-Fail");
            TEST(Resource::IsPathSeparator_Windows(';'),"IsPathSeparator_Windows(const_Char8)-Pass");
            TEST(!Resource::IsPathSeparator_Windows(':'),"IsPathSeparator_Windows(const_Char8)-Fail");
            TEST(Resource::GetPathSeparator_Posix() == ':',"GetPathSeparator_Posix()");
            TEST(Resource::GetPathSeparator_Windows() == ';',"GetPathSeparator_Windows()");
        #ifdef MEZZ_WINDOWS
            TEST(Resource::IsPathSeparator_Host(';'),"IsPathSeparator_Host(const_Char8)-Pass");
            TEST(!Resource::IsPathSeparator_Host(':'),"IsPathSeparator_Host(const_Char8)-Fail");
            TEST(Resource::GetPathSeparator_Host() == ';',"GetPathSeparator_Host()");
        #else
            TEST(Resource::IsPathSeparator_Host(':'),"IsPathSeparator_Host(const_Char8)-Pass");
            TEST(!Resource::IsPathSeparator_Host(';'),"IsPathSeparator_Host(const_Char8)-Fail");
            TEST(Resource::GetPathSeparator_Host() == ':',"GetPathSeparator_Host()");
        #endif
        }// Separators

        {// Absolute and Relative Paths
            const String WinAbsPath("C:\\MinGW\\");
            const String PosixAbsPath("/etc/IDontEvenKnow/");
            const String WinDotRelPath(".\\SubDir\\");
            const String DotRelPath("./SubDir/");
            const String NadaRelPath("SubDir/");

            TEST(Resource::IsPathAbsolute(WinAbsPath),"IsPathAbsolute(const_String&)-Pass");
            TEST(!Resource::IsPathAbsolute(DotRelPath),"IsPathAbsolute(const_String&)-Fail");
            TEST(Resource::IsPathAbsolute_Posix(PosixAbsPath),"IsPathAbsolute_Posix(const_String&)-Pass");
            TEST(!Resource::IsPathAbsolute_Posix(NadaRelPath),"IsPathAbsolute_Posix(const_String&)-Fail");
            TEST(Resource::IsPathAbsolute_Windows(WinAbsPath),"IsPathAbsolute_Windows(const_String&)-Pass");
            TEST(!Resource::IsPathAbsolute_Windows(WinDotRelPath),"IsPathAbsolute_Windows(const_String&)-Fail");

            TEST(Resource::IsPathRelative(NadaRelPath),"IsPathRelative(const_String&)-Pass");
            TEST(!Resource::IsPathRelative(WinAbsPath),"IsPathRelative(const_String&)-Fail");
            TEST(Resource::IsPathRelative_Posix(DotRelPath),"IsPathRelative_Posix(const_String&)-Pass");
            TEST(!Resource::IsPathRelative_Posix(WinDotRelPath),"IsPathRelative_Posix(const_String&)-Fail");
            TEST(Resource::IsPathRelative_Windows(WinDotRelPath),"IsPathRelative_Windows(const_String&)-Pass");
            TEST(!Resource::IsPathRelative_Windows(PosixAbsPath),"IsPathRelative_Windows(const_String&)-Fail");

        #ifdef MEZZ_WINDOWS
            TEST(Resource::IsPathAbsolute_Host(WinAbsPath),"IsPathAbsolute_Host(const_String&)-Pass");
            TEST(!Resource::IsPathAbsolute_Host(PosixAbsPath),"IsPathAbsolute_Host(const_String&)-Fail");
            TEST(Resource::IsPathRelative_Host(WinDotRelPath),"IsPathRelative_Host(const_String&)-Pass");
            TEST(!Resource::IsPathRelative_Host(PosixAbsPath),"IsPathRelative_Host(const_String&)-Fail");
        #else
            TEST(Resource::IsPathAbsolute_Host(PosixAbsPath),"IsPathAbsolute_Host(const_String&)-Pass");
            TEST(!Resource::IsPathAbsolute_Host(WinAbsPath),"IsPathAbsolute_Host(const_String&)-Fail");
            TEST(Resource::IsPathRelative_Host(DotRelPath),"IsPathRelative_Host(const_String&)-Pass");
            TEST(!Resource::IsPathRelative_Host(WinDotRelPath),"IsPathRelative_Host(const_String&)-Fail");
        #endif
        }// Absolute and Relative Paths

        {// Path Checks
            const String DepthDirOne("C:\\FirstDir\\SecondDir\\ThirdDir\\");
            const String DepthDirTwo("./FirstDir/../../../");
            const String DepthDirThree("../../FirstDir/SecondDir/");
            const String DepthDirFour("/user/home/./");

            TEST( Resource::GetDirectoryDepth(DepthDirOne,false) == 3,
                  "GetDirectoryDepth(const_String&,const_Boole)-First" );
            TEST( Resource::GetDirectoryDepth(DepthDirTwo,false) == -2,
                  "GetDirectoryDepth(const_String&,const_Boole)-Second" );
            TEST( Resource::GetDirectoryDepth(DepthDirThree,false) == 0,
                  "GetDirectoryDepth(const_String&,const_Boole)-Third" );
            TEST( Resource::GetDirectoryDepth(DepthDirFour,false) == 2,
                  "GetDirectoryDepth(const_String&,const_Boole)-Fourth" );
            TEST( Resource::GetDirectoryDepth(DepthDirTwo,true) == -1,
                  "GetDirectoryDepth(const_String&,const_Boole)-Fifth" );
            TEST( Resource::GetDirectoryDepth(DepthDirThree,true) == -1,
                  "GetDirectoryDepth(const_String&,const_Boole)-Sixth" );

            const String BaseDirOne("/user/home/");
            const String BaseDirTwo("C:\\MinGW\\bin\\");
            const String BaseDirThree("home/");

            TEST( Resource::IsSubPath(BaseDirOne,"/user/home/code/"),
                  "IsSubPath(const_String&,const_String&)-First-Pass" );
            TEST( !Resource::IsSubPath(BaseDirOne,"/etc/home/"),
                  "IsSubPath(const_String&,const_String&)-First-Fail" );
            TEST( Resource::IsSubPath(BaseDirTwo,"C:\\MinGW\\bin\\include\\"),
                  "IsSubPath(const_String&,const_String&)-Second-Pass" );
            TEST( !Resource::IsSubPath(BaseDirTwo,"C:\\System32\\"),
                  "IsSubPath(const_String&,const_String&)-Second-Fail" );
            TEST( Resource::IsSubPath(BaseDirThree,"home/code/"),
                  "IsSubPath(const_String&,const_String&)-Third-Pass" );
            TEST( !Resource::IsSubPath(BaseDirThree,"home/../"),
                  "IsSubPath(const_String&,const_String&)-Third-Fail" );

            TEST_THROW(ExceptionFactory<ExceptionBase::PARAMETERS_EXCEPTION>::Type,
                       Resource::IsSubPath(BaseDirOne,BaseDirThree),
                       "IsSubPath(const_String&,const_String&)-Absolute/Relative-Throw");
            TEST_THROW(ExceptionFactory<ExceptionBase::PARAMETERS_EXCEPTION>::Type,
                       Resource::IsSubPath(BaseDirThree,BaseDirOne),
                       "IsSubPath(const_String&,const_String&)-Relative/Absolute-Throw");
        }// Path Checks

        {// Path Utilities
            StringVector PathBuildingOne = { "Users", "Person", "Code", "Project" };
            StringVector PathBuildingTwo = { "user", "home", "code" };
            StringVector PathBuildingThree = { "data", "common" };
            const String PathBuildingOneResult("C:\\Users\\Person\\Code\\Project\\TestFile.txt");
            const String PathBuildingTwoResult("/user/home/code/");
            const String PathBuildingThreeResult("data/common/TestJPG.jpg");

            TEST( Resource::BuildPath("C:\\",PathBuildingOne,"TestFile.txt",true) == PathBuildingOneResult,
                  "BuildPath(const_String&,const_StringVector&,const_String&,const_Boole)-First" );
            TEST( Resource::BuildPath("/",PathBuildingTwo,String(),false) == PathBuildingTwoResult,
                  "BuildPath(const_String&,const_StringVector&,const_String&,const_Boole)-Second" );
            TEST( Resource::BuildPath(String(),PathBuildingThree,"TestJPG.jpg",false) == PathBuildingThreeResult,
                  "BuildPath(const_String&,const_StringVector&,const_String&,const_Boole)-Third" );

            const String DotSegPathOne("./data/common/");
            const String DotSegPathOneResult("data/common/");
            const String DotSegPathTwo("OtherData/./common/license/../../");
            const String DotSegPathTwoResult("OtherData/");
            const String DotSegPathThree("MoreData/.././../");
            const String DotSegPathThreeResult("../");
            const String DotSegPathFour("C:\\Users\\..\\..\\");
            const String DotSegPathFourResult("C:\\");

            TEST( Resource::RemoveDotSegments(DotSegPathOne) == DotSegPathOneResult,
                  "RemoveDotSegments(const_String&)-First" );
            TEST( Resource::RemoveDotSegments(DotSegPathTwo) == DotSegPathTwoResult,
                  "RemoveDotSegments(const_String&)-Second" );
            TEST( Resource::RemoveDotSegments(DotSegPathThree) == DotSegPathThreeResult,
                  "RemoveDotSegments(const_String&)-Third" );
            TEST( Resource::RemoveDotSegments(DotSegPathFour) == DotSegPathFourResult,
                  "RemoveDotSegments(const_String&)-Fourth" );

            const String CombinePathOne("C:\\Users\\Person\\Desktop\\");
            const String CombinePathTwo("home/");
            const String CombinePathThree("/etc/dir");
            const String CombineNameOne("TestText.txt");
            const String CombineNameTwo("TestJPG.jpg");

            TEST( Resource::CombinePathAndFileName(CombinePathOne,CombineNameOne) == "C:\\Users\\Person\\Desktop\\TestText.txt",
                  "CombinePathAndFileName(const_String&,const_String&)-First" );
            TEST( Resource::CombinePathAndFileName(CombinePathOne,CombineNameTwo) == "C:\\Users\\Person\\Desktop\\TestJPG.jpg",
                  "CombinePathAndFileName(const_String&,const_String&)-Second" );
            TEST( Resource::CombinePathAndFileName(CombinePathTwo,CombineNameOne) == "home/TestText.txt",
                  "CombinePathAndFileName(const_String&,const_String&)-Third" );
            TEST( Resource::CombinePathAndFileName(CombinePathTwo,CombineNameTwo) == "home/TestJPG.jpg",
                  "CombinePathAndFileName(const_String&,const_String&)-Fourth" );
            TEST( Resource::CombinePathAndFileName(CombinePathThree,CombineNameOne) == "/etc/dir/TestText.txt",
                  "CombinePathAndFileName(const_String&,const_String&)-Fifth" );
            TEST( Resource::CombinePathAndFileName(CombinePathThree,CombineNameTwo) == "/etc/dir/TestJPG.jpg",
                  "CombinePathAndFileName(const_String&,const_String&)-Sixth" );
        }// Path Utilities
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};

#endif

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
#ifndef _ziparchivereadertests_h
#define _ziparchivereadertests_h

#include "mezztest.h"

#include "Resource/ziparchivereader.h"

/// @file
/// @brief This file contains the series of tests to be used with the ZipArchiveReader and support classes.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

namespace ZipTestConstants
{
    enum Constants
    {
        FirstFileSize = 73,
        FirstFileCompSize = 60,
        SecondFileSize = 116,
        SecondFileCompSize = 72,
        SubDirFileSize = 86,
        SubDirFileCompSize = 74
    };
}

/// @brief A small series of sample tests testing the reading of zip archives.
class ziparchivereadertests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "ZipReader"
    virtual String Name()
        { return String("ZipReader"); }

    /// @brief Test most features of the ZipArchiveReader.
    void RunAutomaticTests()
    {
        using namespace Mezzanine::Resource;

        const String ArchivePath("data/ArchiveTestData.zip");
        const String FirstFilePath("FirstTestFile.txt");
        const String SecondFilePath("SecondTestFile.txt");
        const String SubDirFilePath("TestFolder/SubDirTestFile.txt");
        const String FolderPath("TestFolder/");
        const String FirstFileContents("This is the first test file line.\n"
                                       "This is a second line in the test file.");
        const String SecondFileContents("Second test file, first line.\n"
                                        "Still the second test file, second line.\n"
                                        "Third and final line of the second test file.");
        const String SubDirFileContents("This file is in a subdirectory.  "
                                        "Multiple sentences deliberately put on a single line.");

        ZipArchiveReader TestReader;
        TEST( !TestReader.IsOpen(),
              "ZipArchiveReader::ZipArchiveReader()" );

        TestReader.Open(ArchivePath);
        TEST( TestReader.IsOpen(),
              "ZipArchiveReader::Open(const_String&)" );

        TEST( TestReader.DirectoryExists("TestFolder/"),
              "ZipArchiveReader::DirectoryExists(const_String&)-Pass" );
        TEST( !TestReader.DirectoryExists("DoesNotExist/"),
              "ZipArchiveReader::DirectoryExists(const_String&)-Fail" );

        TEST( TestReader.FileExists("FirstTestFile.txt"),
              "ZipArchiveReader::FileExists(const_String&)-Pass" );
        TEST( !TestReader.FileExists("DoesNotExist.mp4"),
              "ZipArchiveReader::FileExists(const_String&)-Fail" );

        Int64 EntryCount = TestReader.GetEntryCount();
        TEST( EntryCount == 4,
              "ZipArchiveReader::GetEntryCount()_const" );

        // Order of entries within an archive are not guaranteed to be in any order, so we gotta sort ourselves.
        std::array<ArchiveEntryPtr,4> IndexResults = { nullptr, nullptr, nullptr, nullptr };
        for( UInt64 Idx = 0 ; Idx < static_cast<UInt64>(EntryCount) ; ++Idx )
            { IndexResults[Idx] = std::move( TestReader.GetEntry(Idx) ); }
        auto PtrResultsSorter = [](const ArchiveEntryPtr& Left, const ArchiveEntryPtr& Right) {
            return std::lexicographical_compare(Left->Name.begin(),Left->Name.end(),
                                                Right->Name.begin(),Right->Name.end());
        };
        std::sort(IndexResults.begin(),IndexResults.end(),PtrResultsSorter);
        TEST( IndexResults[0]->Name == FirstFilePath,
              "ZipArchiveReader::GetEntry(const_UInt64)_const-Result1-Name" );
        TEST( IndexResults[0]->Size == ZipTestConstants::FirstFileSize,
              "ZipArchiveReader::GetEntry(const_UInt64)_const-Result1-Size" );
        TEST( IndexResults[0]->CompressedSize == ZipTestConstants::FirstFileCompSize,
              "ZipArchiveReader::GetEntry(const_UInt64)_const-Result1-CompSize" );
        TEST( IndexResults[1]->Name == SecondFilePath,
              "ZipArchiveReader::GetEntry(const_UInt64)_const-Result2-Name" );
        TEST( IndexResults[1]->Size == ZipTestConstants::SecondFileSize,
              "ZipArchiveReader::GetEntry(const_UInt64)_const-Result2-Size" );
        TEST( IndexResults[1]->CompressedSize == ZipTestConstants::SecondFileCompSize,
              "ZipArchiveReader::GetEntry(const_UInt64)_const-Result2-CompSize" );
        TEST( IndexResults[2]->Name == FolderPath,
              "ZipArchiveReader::GetEntry(const_UInt64)_const-Result3-Name" );
        TEST( IndexResults[2]->Size == 0,
              "ZipArchiveReader::GetEntry(const_UInt64)_const-Result3-Size" );
        TEST( IndexResults[2]->CompressedSize == 0,
              "ZipArchiveReader::GetEntry(const_UInt64)_const-Result3-CompSize" );
        TEST( IndexResults[3]->Name == SubDirFilePath,
              "ZipArchiveReader::GetEntry(const_UInt64)_const-Result4-Name" );
        TEST( IndexResults[3]->Size == ZipTestConstants::SubDirFileSize,
              "ZipArchiveReader::GetEntry(const_UInt64)_const-Result4-Size" );
        TEST( IndexResults[3]->CompressedSize == ZipTestConstants::SubDirFileCompSize,
              "ZipArchiveReader::GetEntry(const_UInt64)_const-Result4-CompSize" );

        ArchiveEntryPtr BaseFileTest = std::move( TestReader.GetEntry(FirstFilePath) );
        TEST( BaseFileTest->Name == FirstFilePath,
              "ZipArchiveReader::GetEntry(const_String&)_const-BaseFile-Name" );
        TEST( BaseFileTest->Size == ZipTestConstants::FirstFileSize,
              "ZipArchiveReader::GetEntry(const_String&)_const-BaseFile-Size" );
        ArchiveEntryPtr SubDirTest = std::move( TestReader.GetEntry(SubDirFilePath) );
        TEST( SubDirTest->Name == SubDirFilePath,
              "ZipArchiveReader::GetEntry(const_String&)_const-SubDir-Name" );
        TEST( SubDirTest->Size == ZipTestConstants::SubDirFileSize,
              "ZipArchiveReader::GetEntry(const_String&)_const-SubDir-Size" );
        TEST( nullptr == TestReader.GetEntry("NotAFile.jpg"),
              "ZipArchiveReader::GetEntry(const_String&)_const-Fail" );

        auto VecResultsSorter = [](const ArchiveEntry& Left, const ArchiveEntry& Right) {
            return std::lexicographical_compare(Left.Name.begin(),Left.Name.end(),
                                                Right.Name.begin(),Right.Name.end());
        };
        ArchiveEntryVector PatternOne = std::move( TestReader.GetEntries("*S*",false) );
        std::sort(PatternOne.begin(),PatternOne.end(),VecResultsSorter);
        TEST( PatternOne.size() == 2,
              "ZipArchiveReader::GetEntries(const_String&,const_Boole)_const-PatternOne-Count" );
        TEST( PatternOne[0].Name == SecondFilePath,
              "ZipArchiveReader::GetEntries(const_String&,const_Boole)_const-PatternOne-Result1" );
        TEST( PatternOne[1].Name == SubDirFilePath,
              "ZipArchiveReader::GetEntries(const_String&,const_Boole)_const-PatternOne-Result2" );

        ArchiveEntryVector PatternTwo = std::move( TestReader.GetEntries("*Test*",true) );
        std::sort(PatternTwo.begin(),PatternTwo.end(),VecResultsSorter);
        TEST( PatternTwo.size() == 3,
              "ZipArchiveReader::GetEntries(const_String&,const_Boole)_const-PatternTwo-Count" );
        TEST( PatternTwo[0].Name == FirstFilePath,
              "ZipArchiveReader::GetEntries(const_String&,const_Boole)_const-PatternTwo-Result1" );
        TEST( PatternTwo[1].Name == SecondFilePath,
              "ZipArchiveReader::GetEntries(const_String&,const_Boole)_const-PatternTwo-Result2" );
        TEST( PatternTwo[2].Name == SubDirFilePath,
              "ZipArchiveReader::GetEntries(const_String&,const_Boole)_const-PatternTwo-Result3" );

        //
        // Streaming
        //

        auto Upcast = [](IStreamPtr Ptr) -> ZipIStreamPtr {
            return std::static_pointer_cast<ZipIStream>(Ptr);
        };

        {// Streaming - First File
            ZipIStreamPtr FirstTestStream = Upcast( TestReader.OpenIStream(FirstFilePath,SF_Read,false) );
            TEST( FirstTestStream->IsOpenToFile(),
                  "ZipIStream-Construct/Open-First" );
            TEST( FirstTestStream->GetStreamIdentifier() == FirstFilePath,
                  "ZipIStream::GetStreamIdentifier()_const-First" );
            TEST( FirstTestStream->GetStreamFlags() == SF_Read,
                  "ZipIStream::GetStreamFlags()_const-First" );
            TEST( FirstTestStream->GetCompressedSize() == ZipTestConstants::FirstFileCompSize,
                  "ZipIStream::GetCompressedSize()_const-First" );
            TEST( FirstTestStream->GetUncompressedSize() == ZipTestConstants::FirstFileSize,
                  "ZipIStream::GetUncompressedSize()_const-First" );
            TEST( !FirstTestStream->CanSeek(),
                  "ZipIStream::CanSeek()_const-First" );
            TEST( FirstTestStream->GetSize() == ZipTestConstants::FirstFileSize,
                  "ZipIStream::GetSize()_const-First" );

            char FirstTestBuffer[ZipTestConstants::FirstFileSize + 1] = {};
            FirstTestStream->read(FirstTestBuffer,ZipTestConstants::FirstFileSize);
            TEST( FirstFileContents == FirstTestBuffer,
                  "ZipIStream::read(char*,const_size_t)-First" );

            FirstTestStream->CloseFile();
            TEST( !FirstTestStream->IsOpenToFile(),
                  "ZipIStream::CloseFile()-First" );
        }// Streaming - First File

        {// Streaming - Second File
            ZipIStreamPtr SecondTestStream = Upcast( TestReader.OpenIStream(SecondFilePath,SF_Read,false) );
            TEST( SecondTestStream->IsOpenToFile(),
                  "ZipIStream-Construct/Open-Second" );
            TEST( SecondTestStream->GetStreamIdentifier() == SecondFilePath,
                  "ZipIStream::GetStreamIdentifier()_const-Second" );
            TEST( SecondTestStream->GetStreamFlags() == SF_Read,
                  "ZipIStream::GetStreamFlags()_const-Second" );
            TEST( SecondTestStream->GetCompressedSize() == ZipTestConstants::SecondFileCompSize,
                  "ZipIStream::GetCompressedSize()_const-Second" );
            TEST( SecondTestStream->GetUncompressedSize() == ZipTestConstants::SecondFileSize,
                  "ZipIStream::GetUncompressedSize()_const-Second" );
            TEST( !SecondTestStream->CanSeek(),
                  "ZipIStream::CanSeek()_const-Second" );
            TEST( SecondTestStream->GetSize() == ZipTestConstants::SecondFileSize,
                  "ZipIStream::GetSize()_const-Second" );

            char SecondTestBuffer[ZipTestConstants::SecondFileSize + 1] = {};
            SecondTestStream->read(SecondTestBuffer,ZipTestConstants::SecondFileSize);
            TEST( SecondFileContents == SecondTestBuffer,
                  "ZipIStream::read(char*,const_size_t)-Second" );

            SecondTestStream->CloseFile();
            TEST( !SecondTestStream->IsOpenToFile(),
                  "ZipIStream::CloseFile()-Second" );
        }// Streaming - Second File

        {// Streaming - SubDir File
            ZipIStreamPtr SubDirTestStream = Upcast( TestReader.OpenIStream(SubDirFilePath,SF_Read,false) );
            TEST( SubDirTestStream->IsOpenToFile(),
                  "ZipIStream-Construct/Open-SubDir" );
            TEST( SubDirTestStream->GetStreamIdentifier() == SubDirFilePath,
                  "ZipIStream::GetStreamIdentifier()_const-SubDir" );
            TEST( SubDirTestStream->GetStreamFlags() == SF_Read,
                  "ZipIStream::GetStreamFlags()_const-SubDir" );
            TEST( SubDirTestStream->GetCompressedSize() == ZipTestConstants::SubDirFileCompSize,
                  "ZipIStream::GetCompressedSize()_const-SubDir" );
            TEST( SubDirTestStream->GetUncompressedSize() == ZipTestConstants::SubDirFileSize,
                  "ZipIStream::GetUncompressedSize()_const-SubDir" );
            TEST( !SubDirTestStream->CanSeek(),
                  "ZipIStream::CanSeek()_const-SubDir" );
            TEST( SubDirTestStream->GetSize() == ZipTestConstants::SubDirFileSize,
                  "ZipIStream::GetSize()_const-SubDir" );

            char SubDirTestBuffer[ZipTestConstants::SubDirFileSize + 1] = {};
            SubDirTestStream->read(SubDirTestBuffer,ZipTestConstants::SubDirFileSize);
            TEST( SubDirFileContents == SubDirTestBuffer,
                  "ZipIStream::read(char*,const_size_t)-SubDir" );

            SubDirTestStream->CloseFile();
            TEST( !SubDirTestStream->IsOpenToFile(),
                  "ZipIStream::CloseFile()-SubDir" );
        }// Streaming - SubDir File

        TestReader.Close();
        TEST( !TestReader.IsOpen(),
              "ZipArchiveReader::Close()" );
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//ziparchivereadertests

#endif

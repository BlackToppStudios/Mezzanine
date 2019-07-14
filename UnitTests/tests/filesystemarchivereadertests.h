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
#ifndef _filesystemarchivereadertests_h
#define _filesystemarchivereadertests_h

#include "mezztest.h"

#include "Resource/filesystemarchivereader.h"

/// @file
/// @brief This file contains the series of tests to be used with the FileSystemArchiveReader and support classes.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

namespace FSTestConstants
{
    enum Constants
    {
        FirstFileSize = 73,
        SecondFileSize = 116,
        SubDirFileSize = 86
    };
}

/// @brief A small series of sample tests testing the reading filesystem files through the archive api.
class filesystemarchivereadertests : public UnitTestGroup
{
public:
    /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
    /// @return Returns a String containing "FileSystemReader"
    virtual String Name()
        { return String("FileSystemReader"); }

    /// @brief Test most features of the FileSystemArchiveReader.
    void RunAutomaticTests()
    {
        using namespace Mezzanine::Resource;

        const String ArchivePath("data/ArchiveTest/");
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

        FileSystemArchiveReader TestReader;
        TEST( !TestReader.IsOpen(),
              "FileSystemArchiveReader::FileSystemArchiveReader()" );

        TEST( TestReader.GetIdentifier().empty(),
              "FileSystemArchiveReader::GetIdentifier()_const-Empty" );
        #ifdef MEZZ_WINDOWS
        TEST( TestReader.IsCaseSensitive() == false,
              "FileSystemArchiveReader::IsCaseSensitive()_const" );
        #else
        TEST( TestReader.IsCaseSensitive() == true,
              "FileSystemArchiveReader::IsCaseSensitive()_const" );
        #endif

        String BlankStr;
        TestReader.Open(ArchivePath,BlankStr);
        TEST( TestReader.IsOpen(),
              "FileSystemArchiveReader::Open(const_String&)" );

        TEST( TestReader.GetIdentifier() == ArchivePath,
              "FileSystemArchiveReader::GetIdentifier()_const" );

        TEST( TestReader.DirectoryExists("TestFolder/"),
              "FileSystemArchiveReader::DirectoryExists(const_String&)-Pass" );
        TEST( !TestReader.DirectoryExists("DoesNotExist/"),
              "FileSystemArchiveReader::DirectoryExists(const_String&)-Fail" );

        TEST( TestReader.FileExists("FirstTestFile.txt"),
              "FileSystemArchiveReader::FileExists(const_String&)-Pass" );
        TEST( !TestReader.FileExists("DoesNotExist.mp4"),
              "FileSystemArchiveReader::FileExists(const_String&)-Fail" );

        TEST( TestReader.GetEntryCount() == -1,
              "FileSystemArchiveReader::GetEntryCount()_const");

        TEST( TestReader.GetEntry(1u) == nullptr,
              "FileSystemArchiveReader::GetEntry(const_UInt64)_const-Result1" );
        TEST( TestReader.GetEntry(10u) == nullptr,
              "FileSystemArchiveReader::GetEntry(const_UInt64)_const-Result2" );
        TEST( TestReader.GetEntry(100u) == nullptr,
              "FileSystemArchiveReader::GetEntry(const_UInt64)_const-Result3" );
        TEST( TestReader.GetEntry(1000u) == nullptr,
              "FileSystemArchiveReader::GetEntry(const_UInt64)_const-Result4" );

        // Below is commented out because filesystem readers don't have index access on account of
        // lacking any cache of files in their path.  If that functionality is added this block
        // can be un-commented.
        /*
        Int64 EntryCount = TestReader.GetEntryCount();
        TEST( EntryCount == 4,
              "FileSystemArchiveReader::GetEntryCount()_const" );

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
              "FileSystemArchiveReader::GetEntry(const_UInt64)_const-Result1-Name" );
        TEST( IndexResults[0]->Size == FSTestConstants::FirstFileSize,
              "FileSystemArchiveReader::GetEntry(const_UInt64)_const-Result1-Size" );
        TEST( IndexResults[0]->CompressedSize == FSTestConstants::FirstFileSize,
              "FileSystemArchiveReader::GetEntry(const_UInt64)_const-Result1-CompSize" );
        TEST( IndexResults[1]->Name == SecondFilePath,
              "FileSystemArchiveReader::GetEntry(const_UInt64)_const-Result2-Name" );
        TEST( IndexResults[1]->Size == FSTestConstants::SecondFileSize,
              "FileSystemArchiveReader::GetEntry(const_UInt64)_const-Result2-Size" );
        TEST( IndexResults[1]->CompressedSize == FSTestConstants::SecondFileSize,
              "FileSystemArchiveReader::GetEntry(const_UInt64)_const-Result2-CompSize" );
        TEST( IndexResults[2]->Name == FolderPath,
              "FileSystemArchiveReader::GetEntry(const_UInt64)_const-Result3-Name" );
        TEST( IndexResults[2]->Size == 0,
              "FileSystemArchiveReader::GetEntry(const_UInt64)_const-Result3-Size" );
        TEST( IndexResults[2]->CompressedSize == 0,
              "FileSystemArchiveReader::GetEntry(const_UInt64)_const-Result3-CompSize" );
        TEST( IndexResults[3]->Name == SubDirFilePath,
              "FileSystemArchiveReader::GetEntry(const_UInt64)_const-Result4-Name" );
        TEST( IndexResults[3]->Size == FSTestConstants::SubDirFileSize,
              "FileSystemArchiveReader::GetEntry(const_UInt64)_const-Result4-Size" );
        TEST( IndexResults[3]->CompressedSize == FSTestConstants::SubDirFileSize,
              "FileSystemArchiveReader::GetEntry(const_UInt64)_const-Result4-CompSize" );
        */

        ArchiveEntryPtr BaseFileTest = std::move( TestReader.GetEntry(FirstFilePath) );
        TEST( BaseFileTest->Name == FirstFilePath,
              "FileSystemArchiveReader::GetEntry(const_String&)_const-BaseFile-Name" );
        TEST( BaseFileTest->Size == FSTestConstants::FirstFileSize,
              "FileSystemArchiveReader::GetEntry(const_String&)_const-BaseFile-Size" );
        ArchiveEntryPtr SubDirTest = std::move( TestReader.GetEntry(SubDirFilePath) );
        TEST( SubDirTest->Name == SubDirFilePath,
              "FileSystemArchiveReader::GetEntry(const_String&)_const-SubDir-Name" );
        TEST( SubDirTest->Size == FSTestConstants::SubDirFileSize,
              "FileSystemArchiveReader::GetEntry(const_String&)_const-SubDir-Size" );
        TEST( nullptr == TestReader.GetEntry("NotAFile.jpg"),
              "FileSystemArchiveReader::GetEntry(const_String&)_const-Fail" );

        auto VecResultsSorter = [](const ArchiveEntry& Left, const ArchiveEntry& Right) {
            return std::lexicographical_compare(Left.Name.begin(),Left.Name.end(),
                                                Right.Name.begin(),Right.Name.end());
        };
        ArchiveEntryVector PatternOne = std::move( TestReader.GetEntries("*S*",false) );
        std::sort(PatternOne.begin(),PatternOne.end(),VecResultsSorter);
        TEST( PatternOne.size() == 2,
              "FileSystemArchiveReader::GetEntries(const_String&,const_Boole)_const-PatternOne-Count" );
        TEST( PatternOne[0].Name == SecondFilePath,
              "FileSystemArchiveReader::GetEntries(const_String&,const_Boole)_const-PatternOne-Result1" );
        TEST( PatternOne[1].Name == SubDirFilePath,
              "FileSystemArchiveReader::GetEntries(const_String&,const_Boole)_const-PatternOne-Result2" );

        ArchiveEntryVector PatternTwo = std::move( TestReader.GetEntries("*Test*",true) );
        std::sort(PatternTwo.begin(),PatternTwo.end(),VecResultsSorter);
        TEST( PatternTwo.size() == 3,
              "FileSystemArchiveReader::GetEntries(const_String&,const_Boole)_const-PatternTwo-Count" );
        TEST( PatternTwo[0].Name == FirstFilePath,
              "FileSystemArchiveReader::GetEntries(const_String&,const_Boole)_const-PatternTwo-Result1" );
        TEST( PatternTwo[1].Name == SecondFilePath,
              "FileSystemArchiveReader::GetEntries(const_String&,const_Boole)_const-PatternTwo-Result2" );
        TEST( PatternTwo[2].Name == SubDirFilePath,
              "FileSystemArchiveReader::GetEntries(const_String&,const_Boole)_const-PatternTwo-Result3" );

        //
        // Streaming
        //

        auto Upcast = [](IStreamPtr Ptr) -> FileIStreamPtr {
            return std::static_pointer_cast<FileIStream>(Ptr);
        };

        {// Streaming - First File
            FileIStreamPtr FirstTestStream = Upcast( TestReader.OpenIStream(FirstFilePath,SF_Read,false) );
            TEST( FirstTestStream->IsOpenToFile(),
                  "FileIStream-Construct/Open-First" );
            TEST( FirstTestStream->GetIdentifier() == FirstFilePath,
                  "FileIStream::GetStreamIdentifier()_const-First" );
            TEST( FirstTestStream->GetFullIdentifier() == ( ArchivePath + FirstFilePath ),
                  "FileIStream::GetFullIdentifier()_const-First" );
            TEST( FirstTestStream->GetStreamFlags() == SF_Read,
                  "FileIStream::GetStreamFlags()_const-First" );
            TEST( FirstTestStream->CanSeek(),
                  "FileIStream::CanSeek()_const-First" );
            TEST( FirstTestStream->GetSize() == FSTestConstants::FirstFileSize,
                  "FileIStream::GetSize()_const-First" );

            char FirstTestBuffer[FSTestConstants::FirstFileSize + 1] = {};
            FirstTestStream->read(FirstTestBuffer,FSTestConstants::FirstFileSize);
            TEST( FirstFileContents == FirstTestBuffer,
                  "FileIStream::read(char*,const_size_t)-First" );

            FirstTestStream->CloseFile();
            TEST( !FirstTestStream->IsOpenToFile(),
                  "FileIStream::CloseFile()-First" );
        }// Streaming - First File

        {// Streaming - Second File
            FileIStreamPtr SecondTestStream = Upcast( TestReader.OpenIStream(SecondFilePath,SF_Read,false) );
            TEST( SecondTestStream->IsOpenToFile(),
                  "FileIStream-Construct/Open-Second" );
            TEST( SecondTestStream->GetIdentifier() == SecondFilePath,
                  "FileIStream::GetStreamIdentifier()_const-Second" );
            TEST( SecondTestStream->GetFullIdentifier() == ( ArchivePath + SecondFilePath ),
                  "FileIStream::GetFullIdentifier()_const-Second" );
            TEST( SecondTestStream->GetStreamFlags() == SF_Read,
                  "FileIStream::GetStreamFlags()_const-Second" );
            TEST( SecondTestStream->CanSeek(),
                  "FileIStream::CanSeek()_const-Second" );
            TEST( SecondTestStream->GetSize() == FSTestConstants::SecondFileSize,
                  "FileIStream::GetSize()_const-Second" );

            char SecondTestBuffer[FSTestConstants::SecondFileSize + 1] = {};
            SecondTestStream->read(SecondTestBuffer,FSTestConstants::SecondFileSize);
            TEST( SecondFileContents == SecondTestBuffer,
                  "FileIStream::read(char*,const_size_t)-Second" );

            SecondTestStream->CloseFile();
            TEST( !SecondTestStream->IsOpenToFile(),
                  "FileIStream::CloseFile()-Second" );
        }// Streaming - Second File

        {// Streaming - SubDir File
            FileIStreamPtr SubDirTestStream = Upcast( TestReader.OpenIStream(SubDirFilePath,SF_Read,false) );
            TEST( SubDirTestStream->IsOpenToFile(),
                  "FileIStream-Construct/Open-SubDir" );
            TEST( SubDirTestStream->GetIdentifier() == SubDirFilePath,
                  "FileIStream::GetStreamIdentifier()_const-SubDir" );
            TEST( SubDirTestStream->GetFullIdentifier() == ( ArchivePath + SubDirFilePath ),
                  "FileIStream::GetFullIdentifier()_const-SubDir" );
            TEST( SubDirTestStream->GetStreamFlags() == SF_Read,
                  "FileIStream::GetStreamFlags()_const-SubDir" );
            TEST( SubDirTestStream->CanSeek(),
                  "FileIStream::CanSeek()_const-SubDir" );
            TEST( SubDirTestStream->GetSize() == FSTestConstants::SubDirFileSize,
                  "FileIStream::GetSize()_const-SubDir" );

            char SubDirTestBuffer[FSTestConstants::SubDirFileSize + 1] = {};
            SubDirTestStream->read(SubDirTestBuffer,FSTestConstants::SubDirFileSize);
            TEST( SubDirFileContents == SubDirTestBuffer,
                  "FileIStream::read(char*,const_size_t)-SubDir" );

            SubDirTestStream->CloseFile();
            TEST( !SubDirTestStream->IsOpenToFile(),
                  "FileIStream::CloseFile()-SubDir" );
        }// Streaming - SubDir File

        TestReader.Close();
        TEST( !TestReader.IsOpen(),
              "FileSystemArchiveReader::Close()" );
    }

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};//filesystemarchivereadertests

#endif

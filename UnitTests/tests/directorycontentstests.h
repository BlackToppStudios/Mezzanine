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
#ifndef _directorycontentstests_h
#define _directorycontentstests_h

#include "mezztest.h"

#include "Resource/directorycontents.h"

/// @file
/// @brief A few tests of some utilities that retrieve the contents of directories.

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A few tests covering utilities that retrieve the contents of directories.
class directorycontentstests : public UnitTestGroup
{
public:
    /// @copydoc Testing::UnitTestGroup::Name
    /// @return Returns a String containing "DirectoryContents"
    virtual String Name()
        { return String("DirectoryContents"); }

    /// @brief This is called when Automatic tests are run
    void RunAutomaticTests()
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

    /// @brief Since RunAutomaticTests is implemented so is this.
    /// @return returns true
    virtual bool HasAutomaticTests() const
        { return true; }
};

#endif

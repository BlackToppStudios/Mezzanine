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
#ifndef _hashedstringtests_h
#define _hashedstringtests_h

#include "mezztest.h"
#include <hashedstring.h>
#include <serialization.h>
#include <Threading/systemcalls.h>

#include <stdexcept> //only used to throw for TEST_THROW
#include <algorithm>
#include <cstdlib>

/// @file
/// @brief This file should be used as template/example for building future Unit Tests

using namespace Mezzanine;
using namespace Mezzanine::Testing;
using namespace std;

/// @brief A small series of sample tests, which can be used as a boilerplate so creating new test groups
class hashedstringtests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "BoilerPlate"
        virtual String Name()
            { return String("HashedString"); }

        /// @brief This is called when Automatic tests are run
        void RunAutomaticTests()
        {
            TestOutput << "Creating a group of strings with sections of Book I of the Illiad by Homer for string comparison" << endl;
            String Empty("");
            String LineOne("Sing, O goddess, the anger of Achilles son of Peleus, that brought countless ills upon the Achaeans. Many a brave soul did it send hurrying down to Hades, and many a hero did it yield a prey to dogs and vultures, for so were the counsels of Jove fulfilled from the day on which the son of Atreus, king of men, and great Achilles, first fell out with one another.");
            String LineTwo("And which of the gods was it that set them on to quarrel? It was the son of Jove and Leto; for he was angry with the king and sent a pestilence upon the host to plague the people, because the son of Atreus had dishonoured Chryses his priest. Now Chryses had come to the ships of the Achaeans to free his daughter, and had brought with him a great ransom: moreover he bore in his hand the sceptre of Apollo wreathed with a suppliant's wreath and he besought the Achaeans, but most of all the two sons of Atreus, who were their chiefs. ");
            String LineThree("\"Sons of Atreus,\" he cried, \"and all other Achaeans, may the gods who dwell in Olympus grant you to sack the city of Priam, and to reach your homes in safety; but free my daughter, and accept a ransom for her, in reverence to Apollo, son of Jove.\"");
            TestOutput << "String Line 1: " << LineOne << endl;
            TestOutput << "String Line 2: " << LineTwo << endl;
            TestOutput << "String Line 3: " << LineThree << endl;

            HashedString32 HashedEmpty(Empty);
            HashedString32 HashedLineOne(LineOne);
            HashedString32 HashedLineTwo(LineTwo);
            HashedString32 HashedLineThree(LineThree);
            TestOutput << "Hased Empty: " << HashedEmpty<< endl;
            TestOutput << "Hashed Line 1: " << HashedLineOne << endl;
            TestOutput << "Hashed Line 2: " << HashedLineTwo << endl;
            TestOutput << "Hashed Line 3: " << HashedLineThree << endl;


            TestOutput << endl << "Tesing equality and inequality." << endl;
            TEST(HashedString32(Empty)==HashedEmpty,"operator==1");
            TEST(HashedString32(LineTwo)==HashedLineTwo,"operator==2");
            TEST(HashedString32(LineThree)==HashedLineThree,"operator==3");
            TEST(HashedString32(LineThree)==HashedLineThree,"operator==3");
            TEST(HashedString32(LineTwo)!=HashedLineOne,"operator!=");

            String HashSerializedEmpty("<HashedString32 Version=\"1\" Hash=\"1364076727\" String=\"\" />");
            String HashSerializedLineOne("<HashedString32 Version=\"1\" Hash=\"1279907206\" String=\"Sing, O goddess, the anger of Achilles son of Peleus, that brought countless ills upon the Achaeans. Many a brave soul did it send hurrying down to Hades, and many a hero did it yield a prey to dogs and vultures, for so were the counsels of Jove fulfilled from the day on which the son of Atreus, king of men, and great Achilles, first fell out with one another.\" />");
            String HashSerializedLineTwo("<HashedString32 Version=\"1\" Hash=\"500193140\" String=\"And which of the gods was it that set them on to quarrel? It was the son of Jove and Leto; for he was angry with the king and sent a pestilence upon the host to plague the people, because the son of Atreus had dishonoured Chryses his priest. Now Chryses had come to the ships of the Achaeans to free his daughter, and had brought with him a great ransom: moreover he bore in his hand the sceptre of Apollo wreathed with a suppliant's wreath and he besought the Achaeans, but most of all the two sons of Atreus, who were their chiefs. \" />");
            String HashSerializedLineThree("<HashedString32 Version=\"1\" Hash=\"382875990\" String=\"&quot;Sons of Atreus,&quot; he cried, &quot;and all other Achaeans, may the gods who dwell in Olympus grant you to sack the city of Priam, and to reach your homes in safety; but free my daughter, and accept a ransom for her, in reverence to Apollo, son of Jove.&quot;\" />");

            TestOutput << endl << "Serializing and deserializing." << endl;
            TEST(HashSerializedEmpty==ToString(HashedEmpty),"Serialization1");
            TEST(HashSerializedLineOne==ToString(HashedLineOne),"Serialization2");
            TEST(HashSerializedLineTwo==ToString(HashedLineTwo),"Serialization3");
            TEST(HashSerializedLineThree==ToString(HashedLineThree),"Serialization4");

            HashedString32 Temp;
            TEST(Temp.IsEmpty(),"DefaultEmpty");
            StringStream EmptyStream;
            EmptyStream << HashSerializedEmpty;
            DeSerialize(EmptyStream,Temp);
            TEST(Temp==HashedEmpty,"Deserialization1");

            StringStream OneStream;
            OneStream << HashSerializedLineOne;
            DeSerialize(OneStream,Temp);
            TEST(Temp==HashedLineOne,"Deserialization2");

            StringStream TwoStream;
            TwoStream << HashSerializedLineTwo;
            DeSerialize(TwoStream,Temp);
            TEST(Temp==HashedLineTwo,"Deserialization3");

            StringStream ThreeStream;
            ThreeStream << HashSerializedLineThree;
            DeSerialize(ThreeStream,Temp);
            TEST(Temp==HashedLineThree,"Deserialization4");
            TEST(Temp.IsEmpty()==false,"NotEmpty");

            TestOutput << endl << "Testing Getting and Setting String and Hash." << endl;
            TEST(HashedString32::EmptyHash==HashedEmpty.GetHash(),"GetHash");
            Temp.SetString("");
            TEST(HashedString32::EmptyHash==Temp.GetHash(),"SetString");
            TEST(""==Temp.GetString(),"GetString");
            TEST(Temp.IsEmpty(),"SetStringEmpty");

            TestOutput << endl << "Comparing Hashes From LineTwo: " << HashedLineTwo.GetHash() << " and LineThree: " << HashedLineThree.GetHash() << endl;
            TEST(HashedLineThree<HashedLineTwo,"operator<");
            TEST(false==(HashedLineThree>HashedLineTwo),"operator>");

            TestOutput << endl << "SizeOf Int32: " << sizeof(Int32) << "\tSizeOf String: " << sizeof(String) << "\tSizeOf HashedString32: " << sizeof(HashedString32) << endl;
            TEST(std::max(sizeof(Int32),sizeof(String))*2>=sizeof(HashedString32) ,"Size");

            const Integer AmountToSort = 1000000;
            TestOutput << endl << "Benchmarking creation of " << AmountToSort << " Strings and Hashed Strings." << endl;

            MaxInt PreloadString = GetTimeStamp();
            std::vector<String> UnsortedStrings;
            UnsortedStrings.reserve(AmountToSort);
            for(Integer Counter = AmountToSort; Counter!=0; Counter--)
            {
                UnsortedStrings.push_back(String("Padding To Make Strings bigger")+ToString(Counter));
                //TestOutput << String("Padding To Make Strings bigger")+ToString(Counter) << endl;
            }
            MaxInt PostloadString = GetTimeStamp();
            std::sort(UnsortedStrings.begin(),UnsortedStrings.end());
            MaxInt PostSortString = GetTimeStamp();
            TestOutput << "To defeat optimization the String in the array is: " << *UnsortedStrings.begin() << endl;

            MaxInt PreloadHash = GetTimeStamp();
            std::vector<HashedString32> UnsortedHashes;
            UnsortedHashes.reserve(AmountToSort);
            for(Integer Counter = AmountToSort; Counter!=0; Counter--)
            {
                UnsortedHashes.push_back(String("Padding To Make Strings bigger")+ToString(Counter));
                //TestOutput << String("Padding To Make Strings bigger")+ToString(Counter) << endl;
            }
            MaxInt PostloadHash = GetTimeStamp();
            std::sort(UnsortedHashes.begin(),UnsortedHashes.end());
            MaxInt PostSortHash = GetTimeStamp();
            TestOutput << "To defeat optimization the Hashed String in the array is: " << *UnsortedHashes.begin() << endl;

            TestOutput << "String sort took " << (PostSortString-PreloadString) << " Total microseconds. With " << (PostloadString-PreloadString) << " in data creation and " << (PostSortString-PostloadString) << " in the actual sorting." << endl ;
            TestOutput << "Hash sort took " << (PostSortHash-PreloadHash) << " Total microseconds. With " << (PostloadHash-PreloadHash) << " in data creation and " << (PostSortHash-PostloadHash) << " in the actual sorting." << endl ;

            TEST_WARN((PostSortString-PostloadString)>(PostSortHash-PostloadHash),"SortSpeed")
            TEST_WARN((PostSortString-PreloadString)>(PostSortHash-PreloadHash),"SpeedWithCreation");

            TestOutput << endl << "Randomizing sort results from previous benchamark and using for set insertion benchmark." << endl;

            for(Integer Counter = 0; AmountToSort>Counter; Counter++)
            {
                Whole Random=rand()%AmountToSort;
                std::swap(UnsortedStrings[Counter], UnsortedStrings[Random]);
                std::swap(UnsortedHashes[Counter], UnsortedHashes[Random]);
            }

            //for(Integer Counter = 0; AmountToSort>Counter; Counter++)
            //     { TestOutput << UnsortedStrings[Counter] << " \t" << UnsortedHashes[Counter] << endl; }

            MaxInt PreStringSet = GetTimeStamp();
            std::set<String> Strings;
            for(Integer Counter = 0; AmountToSort>Counter; Counter++)
                { Strings.insert(UnsortedStrings[Counter]); }
            MaxInt PostStringSet = GetTimeStamp();
            TestOutput << "To defeat overoptimizing the String in the set is: " << *Strings.begin() << endl;

            MaxInt PreHashedSet = GetTimeStamp();
            std::set<HashedString32> Hashes;
            for(Integer Counter = 0; AmountToSort>Counter; Counter++)
                { Hashes.insert(UnsortedHashes[Counter]); }
            MaxInt PostHashedSet = GetTimeStamp();
            TestOutput << "To defeat overoptimizing the Hashes in the set is: " << *Hashes.begin() << endl;

            TestOutput << "String sort by set insertion took (in microseconds): " << (PostStringSet-PreStringSet) << endl;
            TestOutput << "Hashed sort by set insertion took (in microseconds): " << (PostHashedSet-PreHashedSet) << endl;
            TEST_WARN((PostStringSet-PreStringSet)>(PostHashedSet-PreHashedSet),"SetSpeed")
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }

};

#endif


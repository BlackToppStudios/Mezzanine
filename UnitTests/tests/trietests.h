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
#ifndef _trietests_h
#define _trietests_h

#include "mezztest.h"
#include <trie.h>

#include <stdexcept> //only used to throw for TEST_THROW

/// @file
/// @brief This file should be used as template/example for building future Unit Tests

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A small series of sample tests, which can be used as a boilerplate so creating new test groups
class trietests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "BoilerPlate"
        virtual String Name()
            { return String("Trie"); }

        /// @brief This is called when Automatic tests are run
        void RunAutomaticTests()
        {
            TestOutput << "Creating a Trie with a few things that start with 'p' like 'pi', 'pow' and 'pine'." << endl;
            Trie<char,String> CommandTrie(0);
            CommandTrie.insert("pow","function");
            CommandTrie.insert("pi","number");
            CommandTrie.insert("pine","object");

            TestOutput << "Creating a trie::iterator from startsWith('p') and trie iterating through it" << endl;
            Trie<char,String>::iterator Iter = CommandTrie.startsWith("p");
            TEST(String(Iter->first)==String("pi"),"IterationStartsWithpFirst1");
            TEST(String(*(Iter->second))==String("number"),"IterationStartsWithpSecond1");
            TestOutput << Iter->first << ", " << *(Iter->second) << endl;
            Iter++;
            TEST(String(Iter->first)==String("pine"),"IterationStartsWithpFirst2");
            TEST(String(*(Iter->second))==String("object"),"IterationStartsWithpSecond2");
            TestOutput << Iter->first << ", " << *(Iter->second) << endl;
            Iter++;
            TEST(String(Iter->first)==String("pow"),"IterationStartsWithpFirst3");
            TEST(String(*(Iter->second))==String("function"),"IterationStartsWithpSecond3");
            TestOutput << Iter->first << ", " << *(Iter->second) << endl;
            Iter++;
            TEST(Iter==CommandTrie.end(),"IterationStartsWithpEnd");


            TestOutput << "Creating a trie::iterator from startsWith('pi') and trie iterating through it" << endl;
            Iter = CommandTrie.startsWith("pi");
            TEST(String(Iter->first)==String("pi"),"IterationStartsWithpiFirst1");
            TEST(String(*(Iter->second))==String("number"),"IterationStartsWithpiSecond1");
            TestOutput << Iter->first << ", " << *(Iter->second) << endl;
            Iter++;
            TEST(String(Iter->first)==String("pine"),"IterationStartsWithpiFirst2");
            TEST(String(*(Iter->second))==String("object"),"IterationStartsWithpiSecond2");
            TestOutput << Iter->first << ", " << *(Iter->second) << endl;
            Iter++;
            TEST(Iter==CommandTrie.end(),"IterationStartsWithpiEnd");



            TestOutput << "Creating a trie::iterator from startsWith('pin') and trie iterating through it" << endl;
            Iter = CommandTrie.startsWith("pin");
            TEST(String(Iter->first)==String("pine"),"IterationStartsWithpinFirst");
            TEST(String(*(Iter->second))==String("object"),"IterationStartsWithpinSecond");
            TestOutput << Iter->first << ", " << *(Iter->second) << endl;
            Iter++;
            TEST(Iter==CommandTrie.end(),"IterationStartsWithpinEnd");
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }

};

#endif


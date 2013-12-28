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
#ifndef _eventstests_h
#define _eventstests_h

#include "mezztest.h"
#include "eventsubscriber.h"
#include "eventpublisher.h"

#include <stdexcept> //only used to throw for TEST_THROW
#include <ostream>

class TestFunctor : public FunctorSubscriberSlot::FunctorDefinition
{
    public:
        ostream& Output;

        TestFunctor(ostream& Out)
            : Output(Out)
        {

        }

        virtual void operator()(const EventArguments& Args)
        {
            Output << "Called from Functor" << endl;
        }
};

class TestPublisher: public EventPublisher
{
    public:

        TestPublisher()
            { AddEvent("test"); }

        void DoTest()
        {
            EventArguments Stuff("test");
            this->FireEvent(Stuff);
        }
};

/// @file
/// @brief This file should be used as template/example for building future Unit Tests

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A small series of sample tests, which can be used as a boilerplate so creating new test groups
class eventstests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "BoilerPlate"
        virtual String Name()
            { return String("Events"); }

        /// @brief This is called when Automatic tests are run
        void RunAutomaticTests()
        {
            //TestFunctor Func(TestOutput);

            //TestPublisher Pub;


            // Why does one of these segfault and the other does not?
            //EventSubscriberSlot* temp = Pub.Subscribe("test", &Func, false);
            //Pub.Subscribe("test", &Func, false);


            //Pub.DoTest();
            //FunctorSubscriber Sub;

        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }


};

#endif


//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _worldnodetests_h
#define _worldnodetests_h

#include "main.h"

class WorldNodeTests : public UnitTestGroup
{
    public:
        virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {

            // These tests need a review and refactor after the attachable changes
            /*TestResult temp=Failed;

            if (RunAutomaticTests)
            {
                AddTestResult("WorldNode::operator<<", Unknown);         // We add something now in case it crashes partway through.
                AddTestResult("WorldNode::operator>>(std::stream,WorldNode)", Unknown);
                AddTestResult("WorldNode::AttachObject", Unknown);
                AddTestResult("WorldNode::AttachTo", Unknown);
                AddTestResult("WorldNode::DetachFrom", Unknown);

                try
                {
                    StartEngine();
                    TheMessage ="WorldNode Tests";
                    WorldNode* Test0 = SceneManager::GetSingletonPtr()->CreateFreeNode("Alpha", Vector3(0,0,0),Vector3(10,0,0));
                    WorldNode* Test1 = SceneManager::GetSingletonPtr()->CreateFreeNode("Beta", Vector3(0,0,0),Vector3(1,0,0));
                    WorldNode* Test2 = SceneManager::GetSingletonPtr()->CreateFreeNode("Gamma", Vector3(2,0,0),Vector3(1,0,0));


                    AddTestResult("WorldNode::AttachObject", Failed);
                    AddTestResult("WorldNode::AttachTo", Failed);
                    Test1->AttachObject(Test2);
                    Test0->AttachObject(Test1);
                    AddTestResult("WorldNode::AttachObject", Success, UnitTestGroup::OverWriteIfMoreSuccessful);
                    AddTestResult("WorldNode::AttachTo", Success, UnitTestGroup::OverWriteIfMoreSuccessful);

                    std::stringstream XMLstream;
                    AddTestResult("WorldNode::operator<<", Failed, UnitTestGroup::OverWriteIfMoreSuccessful);
                    XMLstream << *Test1;
                    if (XMLstream.str() == String("<WorldNode Version=\"1\" Name=\"Beta\" AttachedTo=\"Alpha\"><Attached Name=\"Gamma\"/><Orientation><Quaternion Version=\"1\" X=\"0\" Y=\"0.707107\" Z=\"0\" W=\"0.707107\" /></Orientation><Location><Vector3 Version=\"1\" X=\"1\" Y=\"0\" Z=\"0\"/></Location></WorldNode>"))
                        { AddTestResult("WorldNode::operator<<", Success, UnitTestGroup::OverWriteIfMoreSuccessful); }
                    std::cout << *Test1 << std::endl;
                    AddTestResult("WorldNode::DetachFrom", Failed);
                    Test2->DetachFrom();
                    Test1->DetachFrom();
                    if(0==Test2->GetAttachedTo()&&0==Test1->GetAttachedTo())
                        { AddTestResult("WorldNode::DetachFrom", Success, UnitTestGroup::OverWriteIfMoreSuccessful); }

                    WorldNode* Test3 = SceneManager::GetSingletonPtr()->CreateFreeNode("Omega", Vector3(0,0,0),Vector3(0,0,0));
                    AddTestResult("WorldNode::operator>>(std::stream,WorldNode)", Failed, UnitTestGroup::OverWriteIfMoreSuccessful);
                    XMLstream >> *Test3;
                    std::cout << *Test1 << std::endl;
                    std::cout << *Test3 << std::endl;
                    if (ToString(*Test3) == String("<WorldNode Version=\"1\" Name=\"Omega\" AttachedTo=\"Alpha\"><Attached Name=\"Gamma\"/><Orientation><Quaternion Version=\"1\" X=\"0\" Y=\"0.707107\" Z=\"0\" W=\"0.707107\" /></Orientation><Location><Vector3 Version=\"1\" X=\"1\" Y=\"0\" Z=\"0\"/></Location></WorldNode>"))
                        { AddTestResult("WorldNode::operator>>(std::stream,WorldNode)", Success, UnitTestGroup::OverWriteIfMoreSuccessful); }

                    StartCountdown(2);
                    TheEntresol->MainLoop();
                }catch(Ogre::Exception e){
                    std::cerr << "Ogre::Exception" << e.what() << std::endl;
                }catch(Exception e){
                    std::cerr << "Mezzanine::Exception" << e.what() << std::endl;
                }catch(std::exception e){
                    std::cerr << "std::exception" << e.what() << std::endl;
                }

                StopEngine();


            }else{
                AddTestResult("WorldNode::operator<<", Skipped);
                AddTestResult("WorldNode::operator>>(std::stream,WorldNode)", Skipped);
                AddTestResult("WorldNode::AttachObject", Skipped);
                AddTestResult("WorldNode::AttachTo", Skipped);
                AddTestResult("WorldNode::DetachFrom", Skipped);
            }


            if (RunInteractiveTests)
            {

                //temp = GetTestAnswer( Mezzanine::String(" (or other config tool)? "));
                //AddTestResult("CompilerFlag::MEZZDEBUG", temp);

            }else{
                //AddTestResult("CompilerFlag::MEZZDEBUG", Skipped);

            }*/

        }
};
#endif

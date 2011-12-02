//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _particleffecttests_h
#define _particleffecttests_h

#include "main.h"

#include <Ogre.h>

class ParticleEffectTests : public UnitTestGroup
{
    public:
        virtual TestResult RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            TestResult temp=Failed;

            if (RunAutomaticTests)
            {
                AddTestResult("ParticleEffect::operator<<", Unknown);         // We add something now in case it crashes partway through.
                AddTestResult("ParticleEffect::operator>>(std::stream,ParticleEffect)", Unknown);
                AddTestResult("ParticleEffect::AttachTo", Unknown);
                AddTestResult("ParticleEffect::DetachFrom", Unknown);
                AddTestResult("ParticleEffect::SetLocation", Unknown);
                AddTestResult("ParticleEffect::GetLocation", Unknown);
                AddTestResult("ParticleEffect::SetOrientation", Unknown);
                AddTestResult("ParticleEffect::GetOrientation", Unknown);

                try
                {
                    StartEngine();
                    TheMessage="ParticleEffect Tests";
                    WorldNode* Test0 = SceneManager::GetSingletonPtr()->CreateFreeNode("Alpha", Vector3(0,0,0),Vector3(10,0,0));
                    ParticleEffect* Test1 = SceneManager::GetSingletonPtr()->CreateParticleEffect("Beta", "Examples/GreenyNimbus");

                    AddTestResult("ParticleEffect::SetOrientation", Failed, UnitTestGroup::OverWrite);
                    AddTestResult("ParticleEffect::GetOrientation", Failed, UnitTestGroup::OverWrite);
                    Test1->SetOrientation(Quaternion(0.426162,0.473514,0.520865,0.568217));
                    if(Test1->GetOrientation().GetNormalizedCopy()==Quaternion(0.426162,0.473514,0.520865,0.568217).GetNormalizedCopy())
                    {
                        AddTestResult("ParticleEffect::SetOrientation", Success, UnitTestGroup::OverWriteIfMoreSuccessful);
                        AddTestResult("ParticleEffect::GetOrientation", Success, UnitTestGroup::OverWriteIfMoreSuccessful);
                    }

                    AddTestResult("ParticleEffect::SetLocation", Failed, UnitTestGroup::OverWrite);
                    AddTestResult("ParticleEffect::GetLocation", Failed, UnitTestGroup::OverWrite);
                    Test1->SetLocation(Vector3(1,2,3));
                    if(Test1->GetLocation()==Vector3(1,2,3))
                    {
                        AddTestResult("ParticleEffect::SetLocation", Success, UnitTestGroup::OverWriteIfMoreSuccessful);
                        AddTestResult("ParticleEffect::GetLocation", Success, UnitTestGroup::OverWriteIfMoreSuccessful);
                    }

                    AddTestResult("ParticleEffect::AttachTo", Failed, UnitTestGroup::OverWrite);
                    Test1->AttachTo(Test0);
                    AddTestResult("ParticleEffect::AttachTo", Success, UnitTestGroup::OverWriteIfMoreSuccessful);

                    stringstream XMLstream;
                    AddTestResult("ParticleEffect::operator<<", Failed, UnitTestGroup::OverWrite);
                    XMLstream << *Test1;
                    if (XMLstream.str() == String("<ParticleEffect Version=\"1\" Name=\"Beta\" AttachedTo=\"Alpha\" Enabled=\"0\"><Orientation><Quaternion Version=\"1\" X=\"0.426162\" Y=\"0.473514\" Z=\"0.520865\" W=\"0.568217\" /></Orientation><Location><Vector3 Version=\"1\" X=\"1\" Y=\"2\" Z=\"3\"/></Location></ParticleEffect>"))
                        { AddTestResult("ParticleEffect::operator<<", Success, UnitTestGroup::OverWriteIfMoreSuccessful); }
                    AddTestResult("ParticleEffect::DetachFrom", Failed, UnitTestGroup::OverWrite);
                    Test1->DetachFrom();
                    if(0==Test1->GetAttachedTo())
                        { AddTestResult("ParticleEffect::DetachFrom", Success, UnitTestGroup::OverWriteIfMoreSuccessful); }

                    ParticleEffect* Test3 = SceneManager::GetSingletonPtr()->CreateParticleEffect("Omega", "Examples/GreenyNimbus");
                    AddTestResult("ParticleEffect::operator>>(std::stream,ParticleEffect)", Failed, UnitTestGroup::OverWriteIfMoreSuccessful);
                    XMLstream >> *Test3;
                    if (ToString(*Test3) == String("<ParticleEffect Version=\"1\" Name=\"Omega\" AttachedTo=\"Alpha\" Enabled=\"0\"><Orientation><Quaternion Version=\"1\" X=\"0.426162\" Y=\"0.473514\" Z=\"0.520865\" W=\"0.568217\" /></Orientation><Location><Vector3 Version=\"1\" X=\"1\" Y=\"2\" Z=\"3\"/></Location></ParticleEffect>"))
                        { AddTestResult("ParticleEffect::operator>>(std::stream,ParticleEffect)", Success, UnitTestGroup::OverWriteIfMoreSuccessful); }

                    StartCountdown(2);
                    TheWorld->MainLoop();
                }catch(Ogre::Exception e){
                    cerr << "Ogre::Exception" << e.what() << endl;
                }catch(Exception e){
                    cerr << "Mezzanine::Exception" << e.what() << endl;
                }catch(exception e){
                    cerr << "std::exception" << e.what() << endl;
                }

                StopEngine();

            }else{
                AddTestResult("ParticleEffect::operator<<", Skipped);
                AddTestResult("ParticleEffect::operator>>(std::stream,ParticleEffect)", Skipped);
                AddTestResult("ParticleEffect::AttachTo", Skipped);
                AddTestResult("ParticleEffect::DetachFrom", Skipped);
                AddTestResult("ParticleEffect::SetLocation", Skipped);
                AddTestResult("ParticleEffect::GetLocation", Skipped);
                AddTestResult("ParticleEffect::SetOrientation", Skipped);
                AddTestResult("ParticleEffect::GetOrientation", Skipped);
            }


            if (RunInteractiveTests)
            {

                //temp = GetTestAnswer( Mezzanine::StringCat(" (or other config tool)? "));
                //AddTestResult("CompilerFlag::MEZZDEBUG", temp);

            }else{
                //AddTestResult("CompilerFlag::MEZZDEBUG", Skipped);

            }

        }
};
#endif

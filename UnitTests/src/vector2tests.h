//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _vector2tests_h
#define _vector2tests_h

#include "main.h"

#include <Ogre.h>

using namespace Mezzanine;

class Vector2Tests : public UnitTestGroup
{
    public:
        virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            if (RunAutomaticTests)
            {
                TestResult temp;
                Vector2 Vec=Vector2();

                if(Vec.X==0.0 && Vec.Y==0.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector2::Vector2()", temp);                          //test

                Vec=Vector2(2.0,1.0);
                if(Vec.X==2.0 && Vec.Y==1.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector2::Vector2(Real,Real)", temp);                 //test

                Ogre::Vector2 Ovec(Vec.GetOgreVector2());
                if(Ovec.x==2.0 && Ovec.y==1.0)             //Make sure that terms which might be confused use different values and are tested for for clearly.
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector2::GetOgreVector2()", temp);                   //test

                Ovec = Ogre::Vector2(3.0,4.0);
                Vec.ExtractOgreVector2(Ovec);
                if(Vec.X==3.0 && Vec.Y==4.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector2::ExtractOgreVector2()", temp);                 //test

                Vector2 Vec2(3.0,4.0);
                Vector2 Vec3(1.0,2.0);
                if(Vec2==Vec && !(Vec3==Vec))       //note that I don't use the != operator, we would hate to get confused test results because of a simple oversight
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector2::operator==(Vector2)", temp);                    //test

                if(!(Vec2!=Vec) && Vec3!=Vec)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector2::operator!=(Vector2)", temp);                    //test

                Ogre::Vector2 Ovec2(1.0,2.0);
                if(!(Vec==Ovec2) && Vec==Ovec)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector2::operator==(Ogre::Vector2)", temp);              //test

                if(Vec!=Ovec2 && !(Vec!=Ovec))
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector2::operator!=(Ogre::Vector2)", temp);              //test

                Vec=Vec*2.0;
                if(Vec.X==6.0 && Vec.Y==8.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector2::operator*(Real)", temp);              //test

                Vec=Vec/2.0;
                if(Vec.X==3.0 && Vec.Y==4.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector2::operator/(Real)", temp);              //test

                Vec*=2.0;
                if(Vec.X==6.0 && Vec.Y==8.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector2::operator*=(Real)", temp);              //test

                Vec/=2.0;
                if(Vec.X==3.0 && Vec.Y==4.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector2::operator/=(Real)", temp);              //test

                Vec2.X=2;
                Vec2.Y=3;
                Vec=Vec+Vec2;
                if(Vec.X==5.0 && Vec.Y==7.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector2::operator+(Vector2)", temp);              //test

                Vec=Vec*Vec2;
                if(Vec.X==10.0 && Vec.Y==21.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector2::operator*(Vector2)", temp);              //test

                Vec=Vec/Vec2;
                if(Vec.X==5.0 && Vec.Y==7.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector2::operator/(Vector2)", temp);              //test

                Vec=Vec-Vec2;
                if(Vec.X==3.0 && Vec.Y==4.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector2::operator-(Vector2)", temp);              //test


                #ifdef MEZZXML
                std::stringstream XMLstream;
                XMLstream<<Vec;
                Mezzanine::String Actual(XMLstream.str());
                Mezzanine::String Ideal("<Vector2 Version=\"1\" X=\"3\" Y=\"4\" />");
                std::cout << XMLstream.str();
                if(Actual==Ideal)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector2::operator<<(ostream,Vector2)", temp);            //test

                try
                {
                    Vector2 Vec4;
                    XMLstream >> Vec4;
                    if(Vec4==Vec)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                }catch (Mezzanine::Exception e){
                    std::cerr << "Failure in: Vector2::operator>>(istream,Vector2)" << std::endl <<"\t Mezzanine::Exception: " << e.what();
                    temp=Failed;
                }
                AddTestResult("Vector2::operator>>(istream,Vector2)", temp);            //test

                try
                {
                    Vector2 Vec5;
                    xml::Document XMLdoc;
                    XMLdoc.Load("<Vector2 Version=\"1\" X=\"3\" Y=\"4\" />");
                    XMLdoc.GetFirstChild()>>Vec5;
                    if(Vec5==Vec)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                }catch (Mezzanine::Exception e){
                    std::cerr << "Failure in: Vector2::operator>>(xml::Node,Vector2)" << std::endl <<"\t Mezzanine::Exception: " << e.what();
                    temp=Failed;
                }
                #endif
                AddTestResult("Vector2::operator>>(xml::Node,Vector2)", temp);          //test

            }else{
                AddTestResult("Vector2::Vector2()", Skipped);
                AddTestResult("Vector2::Vector2(Real,Real)", Skipped);
                AddTestResult("Vector2::GetOgreVector2()", Skipped);
                AddTestResult("Vector2::ExtractOgreVector2()", Skipped);
                AddTestResult("Vector2::operator==(Vector2)", Skipped);
                AddTestResult("Vector2::operator!=(Vector2)", Skipped);
                AddTestResult("Vector2::operator==(Ogre::Vector2)", Skipped);
                AddTestResult("Vector2::operator!=(Ogre::Vector2)", Skipped);
                AddTestResult("Vector2::operator*(Real)", Skipped);
                AddTestResult("Vector2::operator/(Real)", Skipped);
                AddTestResult("Vector2::operator*=(Real)", Skipped);
                AddTestResult("Vector2::operator/=(Real)", Skipped);
                AddTestResult("Vector2::operator+(Vector2)", Skipped);
                AddTestResult("Vector2::operator-(Vector2)", Skipped);
                AddTestResult("Vector2::operator*(Vector2)", Skipped);
                AddTestResult("Vector2::operator/(Vector2)", Skipped);
                #ifdef MEZZXML
                AddTestResult("Vector2::operator<<(ostream,Vector2)", Skipped);
                AddTestResult("Vector2::operator>>(istream,Vector2)", Skipped);
                AddTestResult("Vector2::operator>>(xml::Node,Vector2)", Skipped);
                #endif
            }


        }
};

#endif


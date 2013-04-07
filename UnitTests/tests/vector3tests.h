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
#ifndef _vector3tests_h
#define _vector3tests_h

#include "main.h"

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>
#include <cAudio.h>

using namespace Mezzanine;
using namespace Mezzanine::Testing;

class vector3tests : public UnitTestGroup
{
    public:
        virtual String Name()
            { return String("vector"); }

        virtual void RunTests(bool RunAutomaticTests, bool RunInteractiveTests)
        {
            if (RunAutomaticTests)
            {
                TestResult temp;
                Vector3 Vec=Vector3();

                Vec[0]=1.0; //use the non-const
                Vec[1]=2.0;
                Vec[2]=3.0;
                if(Vec[0]==1.0 && Vec[1]==2.0 && Vec[2]==3.0) // use the const
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::GetAxisValue(Integer)", temp);

                Vec=Vector3();
                Vec[Axis_X]=1.0; //use the non-const
                Vec[Axis_Y]=2.0;
                Vec[Axis_Z]=3.0;
                if(Vec[Axis_X]==1.0 && Vec[Axis_Y]==2.0 && Vec[Axis_Z]==3.0) // use the const
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::GetAxisValue(StandardAxis)", temp);
                // we are only calling 2 of these 8 functions

                Vec=Vector3();
                if(Vec.X==0.0 && Vec.Y==0.0 && Vec.Z==0.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::Vector3()", temp);                          //test

                Vec=Vector3(2.0,1.0,3.0);
                if(Vec.X==2.0 && Vec.Y==1.0 && Vec.Z==3.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::Vector3(Real,Real,Real)", temp);            //test

                Vec=Vector3(Ogre::Vector3(4.0,5.0,6.0));
                if(Vec.X==4.0 && Vec.Y==5.0 && Vec.Z==6.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::Vector3(Ogre::Vector3)", temp);            //test

                Vec=Vector3(btVector3(7.0,8.0,9.0));
                if(Vec.X==7.0 && Vec.Y==8.0 && Vec.Z==9.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::Vector3(btVector3)", temp);            //test

                Vec=Vector3(cAudio::cVector3(10.0,11.0,12.0));
                if(Vec.X==10.0 && Vec.Y==11.0 && Vec.Z==12.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::Vector3(cAudio::cVector3)", temp);            //test

                Vector3 Vec2(Vec);
                if(Vec2.X==10.0 && Vec2.Y==11.0 && Vec2.Z==12.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::Vector3(Vector3)", temp);                       //test

                //Vector3(XML::Node OneNode);

                if(Vector3::Unit_X().X==1.0 && Vector3::Unit_X().Y==0.0 && Vector3::Unit_X().Z==0.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::Unit_X()", temp);                               //test

                if(Vector3::Unit_Y().X==0.0 && Vector3::Unit_Y().Y==1.0 && Vector3::Unit_Y().Z==0.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::Unit_Y()", temp);                               //test

                 if(Vector3::Unit_Z().X==0.0 && Vector3::Unit_Z().Y==0.0 && Vector3::Unit_Z().Z==1.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::Unit_Z()", temp);                               //test

                if(Vector3::Neg_Unit_X().X==-1.0 && Vector3::Neg_Unit_X().Y==0.0 && Vector3::Neg_Unit_X().Z==0.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::Neg_Unit_X()", temp);                               //test

                if(Vector3::Neg_Unit_Y().X==0.0 && Vector3::Neg_Unit_Y().Y==-1.0 && Vector3::Neg_Unit_Y().Z==0.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::Neg_Unit_Y()", temp);                               //test

                if(Vector3::Neg_Unit_Z().X==0.0 && Vector3::Neg_Unit_Z().Y==0.0 && Vector3::Neg_Unit_Z().Z==-1.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::Neg_Unit_Z()", temp);                               //test

		//UnitOnAxis(StandardAxis Axis)
		//IsStandardUnitAxis() const;

                {
                    Vector3 Vec2(0.0,0.0,0.0);
                    Vec2=btVector3(10.0,11.0,12.0);
                    if(Vec.X==10.0 && Vec.Y==11.0 && Vec.Z==12.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator=(btVector3)", temp);            //test
                }

                {
                    Vector3 Vec2(0.0,0.0,0.0);
                    Vec2=Ogre::Vector3(10.0,11.0,12.0);
                    if(Vec.X==10.0 && Vec.Y==11.0 && Vec.Z==12.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator=(Ogre::Vector3)", temp);            //test
                }

                {
                    Vector3 Vec2(0.0,0.0,0.0);
                    Vec2=cAudio::cVector3(10.0,11.0,12.0);
                    if(Vec.X==10.0 && Vec.Y==11.0 && Vec.Z==12.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator=(cAudio::cVector3)", temp);            //test
                }

                Vec=Vector3(2.0,1.0,3.0);
                Vec= -Vec;
                if(Vec.X==-2.0 && Vec.Y==-1.0 && Vec.Z==-3.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::operator-()", temp);                               //test

                Vec=Vector3(2.0,1.0,3.0);
                Vec=Vec*2;
                if(Vec.X==4.0 && Vec.Y==2.0 && Vec.Z==6.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::operator*(Real)", temp);            //test

                Vec=Vector3(4.0,2.0,6.0);
                Vec=Vec/2;
                if(Vec.X==2.0 && Vec.Y==1.0 && Vec.Z==3.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::operator/(Real)", temp);            //test

                Vec=Vector3(2.0,1.0,3.0);
                Vec*=2.0;
                if(Vec.X==4.0 && Vec.Y==2.0 && Vec.Z==6.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::operator*=(Real)", temp);            //test

                Vec=Vector3(4.0,2.0,6.0);
                Vec/=2.0;
                if(Vec.X==2.0 && Vec.Y==1.0 && Vec.Z==3.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::operator/=(Real)", temp);            //test

                {
                    Vector3 Vec3(4.0,5.0,6.0);
                    Vector3 Vec4(1.0,2.0,3.0);
                    Vector3 Vec5(1.0,2.0,3.0);
                    Vector3 Vec6(0.0,2.0,3.0);
                    Vector3 Vec7(1.0,0.0,3.0);
                    Vector3 Vec8(1.0,2.0,0.0);
                    if(Vec4==Vec5 && !(Vec4==Vec3) && !(Vec4==Vec6) && !(Vec4==Vec7) && !(Vec4==Vec8))
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator==(Vector3)", temp);                    //test

                    if(!(Vec4!=Vec5) && Vec4!=Vec3 && Vec4!=Vec6 && Vec4!=Vec7 && Vec4!=Vec8)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator!=(Vector3)", temp);                    //test
                }

                {
                    btVector3 Vec3(4.0,5.0,6.0);
                    Vector3 Vec4(1.0,2.0,3.0);
                    btVector3 Vec5(1.0,2.0,3.0);
                    btVector3 Vec6(0.0,2.0,3.0);
                    btVector3 Vec7(1.0,0.0,3.0);
                    btVector3 Vec8(1.0,2.0,0.0);
                    if(Vec4==Vec5 && !(Vec4==Vec3) && !(Vec4==Vec6) && !(Vec4==Vec7) && !(Vec4==Vec8))
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator==(btVector3)", temp);                    //test

                    if(!(Vec4!=Vec5) && Vec4!=Vec3 && Vec4!=Vec6 && Vec4!=Vec7 && Vec4!=Vec8)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator!=(btVector3)", temp);                    //test
                }

                {
                    Ogre::Vector3 Vec3(4.0,5.0,6.0);
                    Vector3 Vec4(1.0,2.0,3.0);
                    Ogre::Vector3 Vec5(1.0,2.0,3.0);
                    Ogre::Vector3 Vec6(0.0,2.0,3.0);
                    Ogre::Vector3 Vec7(1.0,0.0,3.0);
                    Ogre::Vector3 Vec8(1.0,2.0,0.0);
                    if(Vec4==Vec5 && !(Vec4==Vec3) && !(Vec4==Vec6) && !(Vec4==Vec7) && !(Vec4==Vec8))
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator==(Ogre::Vector3)", temp);                    //test

                    if(!(Vec4!=Vec5) && Vec4!=Vec3 && Vec4!=Vec6 && Vec4!=Vec7 && Vec4!=Vec8)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator!=(Ogre::Vector3)", temp);                    //test
                }

                {
                    cAudio::cVector3 Vec3(4.0,5.0,6.0);
                    Vector3 Vec4(1.0,2.0,3.0);
                    cAudio::cVector3 Vec5(1.0,2.0,3.0);
                    cAudio::cVector3 Vec6(0.0,2.0,3.0);
                    cAudio::cVector3 Vec7(1.0,0.0,3.0);
                    cAudio::cVector3 Vec8(1.0,2.0,0.0);
                    if(Vec4==Vec5 && !(Vec4==Vec3) && !(Vec4==Vec6) && !(Vec4==Vec7) && !(Vec4==Vec8))
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator==(cAudio::cVector3)", temp);                    //test

                    if(!(Vec4!=Vec5) && Vec4!=Vec3 && Vec4!=Vec6 && Vec4!=Vec7 && Vec4!=Vec8)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator!=(cAudio::cVector3)", temp);                    //test
                }

                {
                    Vector3 Vec4(3.0,4.0,5.0);
                    Vector3 Vec5(7.0,9.0,11.0);
                    Vector3 Vec6;

                    Vec6=Vec4+Vec5;
                    if(Vec6.X==10.0 && Vec6.Y==13.0 && Vec6.Z==16.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator+(Vector3)", temp);                             //test

                    Vec6=Vector3(0,0,0);
                    Vec6=Vec4-Vec5;
                    if(Vec6.X==-4.0 && Vec6.Y==-5.0 && Vec6.Z==-6.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator-(Vector3)", temp);                             //test

                    Vec6=Vector3(0,0,0);
                    Vec6=Vec4*Vec5;
                    if(Vec6.X==21.0 && Vec6.Y==36.0 && Vec6.Z==55.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator*(Vector3)", temp);                             //test

                    Vec4=Vector3(21.0,36.0,55.0);
                    Vec6=Vector3(0,0,0);
                    Vec6=Vec4/Vec5;
                    if(Vec6.X==3.0 && Vec6.Y==4.0 && Vec6.Z==5.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator/(Vector3)", temp);                             //test
                }

                {
                    Vector3 Vec4(3.0,4.0,5.0);
                    btVector3 Vec5(7.0,9.0,11.0);
                    Vector3 Vec6;

                    Vec6=Vec4+Vec5;
                    if(Vec6.X==10.0 && Vec6.Y==13.0 && Vec6.Z==16.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator+(btVector3)", temp);                             //test

                    Vec6=Vector3(0,0,0);
                    Vec6=Vec4-Vec5;
                    if(Vec6.X==-4.0 && Vec6.Y==-5.0 && Vec6.Z==-6.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator-(btVector3)", temp);                             //test

                    Vec6=Vector3(0,0,0);
                    Vec6=Vec4*Vec5;
                    if(Vec6.X==21.0 && Vec6.Y==36.0 && Vec6.Z==55.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator*(btVector3)", temp);                             //test

                    Vec4=Vector3(21.0,36.0,55.0);
                    Vec6=Vector3(0,0,0);
                    Vec6=Vec4/Vec5;
                    if(Vec6.X==3.0 && Vec6.Y==4.0 && Vec6.Z==5.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator/(btVector3)", temp);                             //test
                }

                {
                    Vector3 Vec4(3.0,4.0,5.0);
                    Ogre::Vector3 Vec5(7.0,9.0,11.0);
                    Vector3 Vec6;

                    Vec6=Vec4+Vec5;
                    if(Vec6.X==10.0 && Vec6.Y==13.0 && Vec6.Z==16.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator+(Ogre::Vector3)", temp);                             //test

                    Vec6=Vector3(0,0,0);
                    Vec6=Vec4-Vec5;
                    if(Vec6.X==-4.0 && Vec6.Y==-5.0 && Vec6.Z==-6.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator-(Ogre::Vector3)", temp);                             //test

                    Vec6=Vector3(0,0,0);
                    Vec6=Vec4*Vec5;
                    if(Vec6.X==21.0 && Vec6.Y==36.0 && Vec6.Z==55.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator*(Ogre::Vector3)", temp);                             //test

                    Vec4=Vector3(21.0,36.0,55.0);
                    Vec6=Vector3(0,0,0);
                    Vec6=Vec4/Vec5;
                    if(Vec6.X==3.0 && Vec6.Y==4.0 && Vec6.Z==5.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator/(Ogre::Vector3)", temp);                             //test
                }

                {
                    Vector3 Vec4(3.0,4.0,5.0);
                    cAudio::cVector3 Vec5(7.0,9.0,11.0);
                    Vector3 Vec6;

                    Vec6=Vec4+Vec5;
                    if(Vec6.X==10.0 && Vec6.Y==13.0 && Vec6.Z==16.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator+(cAudio::cVector3)", temp);                             //test

                    Vec6=Vector3(0,0,0);
                    Vec6=Vec4-Vec5;
                    if(Vec6.X==-4.0 && Vec6.Y==-5.0 && Vec6.Z==-6.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator-(cAudio::cVector3)", temp);                             //test

                    Vec6=Vector3(0,0,0);
                    Vec6=Vec4*Vec5;
                    if(Vec6.X==21.0 && Vec6.Y==36.0 && Vec6.Z==55.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator*(cAudio::cVector3)", temp);                             //test

                    Vec4=Vector3(21.0,36.0,55.0);
                    Vec6=Vector3(0,0,0);
                    Vec6=Vec4/Vec5;
                    if(Vec6.X==3.0 && Vec6.Y==4.0 && Vec6.Z==5.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator/(cAudio::cVector3)", temp);                             //test
                }

                {
                    Vector3 Vec4(1.0,2.0,3.0);
                    Vector3 Vec5(4.0,5.0,6.0);
                    Vector3 Vec6;
                    // 1,2,3 . 4,5,6
                    // 2*6-3*5 = -3
                    // 3*4-1*6 = 6
                    // 1*5-2*4 = -3
                    Vec6=Vec4.CrossProduct(Vec5);
                    if(Vec6.X==-3.0 && Vec6.Y==6.0 && Vec6.Z==-3.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::CrossProduct(Vector3)", temp);                                  //test
                }

                {
                    Vector3 Vec4(1.0,2.0,3.0);
                    Vector3 Vec5(4.0,5.0,6.0);
                    // 1,2,3 . 4,5,6
                    // 1*4 + 2*5 + 3*6 = 32
                    Real Prod=Vec4.DotProduct(Vec5);
                    if(Prod==32.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::DotProduct(Vector3)", temp);                                  //test
                }

                {
                    Vector3 Vec4(112.0,0.0,0.0);
                    Vector3 Vec5(0.0,23.0,0.0);
                    Vector3 Vec6(0.0,0.0,-7000.0);
                    Vector3 VecX(1.0,0.0,0.0);
                    Vector3 VecY(0.0,1.0,0.0);
                    Vector3 VecZ(0.0,0.0,-1.0);

                    Vec4.Normalize(); Vec6.Normalize(); Vec5.Normalize();
                    if(VecX==Vec4 && VecY==Vec5 && VecZ==Vec6)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::Normalize()", temp);                                  //test
                }


                {
                    Vector3 Vec4(112.0,0.0,0.0);
                    Vector3 Vec5(0.0,23.0,0.0);
                    Vector3 Vec6(0.0,0.0,-7000.0);
                    Vector3 VecX(1.0,0.0,0.0);
                    Vector3 VecY(0.0,1.0,0.0);
                    Vector3 VecZ(0.0,0.0,-1.0);

                    if(VecX==Vec4.GetNormal() && VecY==Vec5.GetNormal() && VecZ==Vec6.GetNormal())
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::GetNormal()", temp);                                //test
                }

                {
                    Vector3 Vec4(112.0,0.0,0.0);
                    Vector3 Vec5(0.0,23.0,0.0);
                    Vector3 Vec6(0.0,0.0,-7000.0);
                    Vector3 VecX(1.0,0.0,0.0);
                    Vector3 VecY(0.0,1.0,0.0);
                    Vector3 VecZ(0.0,0.0,-1.0);
                    Vector3 VecD(0.0,0.0,0.0);

                    if(VecX==VecD.GetDirection(Vec4) && VecY==VecD.GetDirection(Vec5) && VecZ==VecD.GetDirection(Vec6))
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::GetDirection(Vector3)", temp);                                //test
                }

                {
                    Vector3 Vec4(20.0,0.0,0.0);
                    Vector3 Vec5(0.0,5.0,0.0);
                    Vector3 Vec6(0.0,0.0,-50.0);
                    Vector3 Vec0(0.0,0.0,0.0);
                    Vector3 VecX(0.05,0.0,0.0);
                    Vector3 VecY(0.0,0.2,0.0);
                    Vector3 VecZ(0.0,0.0,-0.02);
                    Vector3 VecO(0.0,0.0,0.0);

                    Vec4.Inverse(); Vec5.Inverse(); Vec6.Inverse(); VecO.Inverse();
                    if(Vec4==VecX && Vec5==VecY && Vec6==VecZ && Vec0==VecO)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::Inverse()", temp);                                //test
                }

                Vec=Vector3(2.0,1.0,3.0);
                btVector3 Bvec(Vec.GetBulletVector3());
                if(Bvec.getX()==2.0 && Bvec.getY()==1.0 && Bvec.getZ()==3.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::GetBulletVector3()", temp);                   //test

                Bvec = btVector3(4.0,5.0,6.0);
                Vec.ExtractBulletVector3(Bvec);
                if(Vec.X==4.0 && Vec.Y==5.0 && Vec.Z==6.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::ExtractBulletVector3()", temp);                 //test

                Vec=Vector3(2.0,1.0,3.0);
                Ogre::Vector3 Ovec(Vec.GetOgreVector3());
                if(Ovec.x==2.0 && Ovec.y==1.0 && Ovec.z==3.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::GetOgreVector3()", temp);                   //test

                Ovec = Ogre::Vector3(4.0,5.0,6.0);
                Vec.ExtractOgreVector3(Ovec);
                if(Vec.X==4.0 && Vec.Y==5.0 && Vec.Z==6.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::ExtractOgreVector3()", temp);                 //test

                Vec=Vector3(2.0,1.0,3.0);
                cAudio::cVector3 Avec(Vec.GetcAudioVector3());
                if(Avec.x==2.0 && Avec.y==1.0 && Avec.z==3.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::GetcAudioVector3()", temp);                   //test

                Avec = cAudio::cVector3(4.0,5.0,6.0);
                Vec.ExtractcAudioVector3(Avec);
                if(Vec.X==4.0 && Vec.Y==5.0 && Vec.Z==6.0)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::ExtractcAudioVector3()", temp);                 //test

                {
                    Vector3 Vec4(5.0,5.0,5.0);
                    Vector3 Vec5(10.0,5.0,5.0);
                    Vector3 Vec6(5.0,20.0,5.0);
                    Vector3 Vec7(5.0,5.0,-50.0);
                    if(Vec4.Distance(Vec5)==5.0 && Vec4.Distance(Vec6)==15.0 && Vec4.Distance(Vec7)==55.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::Distance(Vector3)", temp);                 //test
                }

                {
                    Vector3 Vec4(3.0,4.0,5.0);
                    btVector3 Vec5(7.0,9.0,11.0);
                    Vector3 Vec6;

                    Vec6=Vec5+Vec4;
                    if(Vec6.X==10.0 && Vec6.Y==13.0 && Vec6.Z==16.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator+(btVector3,Vector3)", temp);                             //test

                    Vec6=Vector3(0,0,0);
                    Vec6=Vec5-Vec4;
                    if(Vec6.X==4.0 && Vec6.Y==5.0 && Vec6.Z==6.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator-(btVector3,Vector3)", temp);                             //test

                    Vec6=Vector3(0,0,0);
                    Vec6=Vec5*Vec4;
                    if(Vec6.X==21.0 && Vec6.Y==36.0 && Vec6.Z==55.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator*(btVector3,Vector3)", temp);                             //test

                    Vec5=btVector3(21.0,36.0,55.0);
                    Vec6=Vector3(0,0,0);
                    Vec6=Vec5/Vec4;
                    if(Vec6.X==7.0 && Vec6.Y==9.0 && Vec6.Z==11.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator/(btVector3,Vector3)", temp);                             //test
                }

                {
                    Vector3 Vec4(3.0,4.0,5.0);
                    Ogre::Vector3 Vec5(7.0,9.0,11.0);
                    Vector3 Vec6;

                    Vec6=Vec5+Vec4;
                    if(Vec6.X==10.0 && Vec6.Y==13.0 && Vec6.Z==16.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator+(Ogre::Vector3,Vector3)", temp);                             //test

                    Vec6=Vector3(0,0,0);
                    Vec6=Vec5-Vec4;
                    if(Vec6.X==4.0 && Vec6.Y==5.0 && Vec6.Z==6.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator-(Ogre::Vector3,Vector3)", temp);                             //test

                    Vec6=Vector3(0,0,0);
                    Vec6=Vec5*Vec4;
                    if(Vec6.X==21.0 && Vec6.Y==36.0 && Vec6.Z==55.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator*(Ogre::Vector3,Vector3)", temp);                             //test

                    Vec5=Ogre::Vector3(21.0,36.0,55.0);
                    Vec6=Vector3(0,0,0);
                    Vec6=Vec5/Vec4;
                    if(Vec6.X==7.0 && Vec6.Y==9.0 && Vec6.Z==11.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator/(Ogre::Vector3,Vector3)", temp);                             //test
                }

                {
                    Vector3 Vec4(3.0,4.0,5.0);
                    cAudio::cVector3 Vec5(7.0,9.0,11.0);
                    Vector3 Vec6;

                    Vec6=Vec5+Vec4;
                    if(Vec6.X==10.0 && Vec6.Y==13.0 && Vec6.Z==16.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator+(cAudio::cVector3,Vector3)", temp);                             //test

                    Vec6=Vector3(0,0,0);
                    Vec6=Vec5-Vec4;
                    if(Vec6.X==4.0 && Vec6.Y==5.0 && Vec6.Z==6.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator-(cAudio::cVector3,Vector3)", temp);                             //test

                    Vec6=Vector3(0,0,0);
                    Vec6=Vec5*Vec4;
                    if(Vec6.X==21.0 && Vec6.Y==36.0 && Vec6.Z==55.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator*(cAudio::cVector3,Vector3)", temp);                             //test

                    Vec5=cAudio::cVector3(21.0,36.0,55.0);
                    Vec6=Vector3(0,0,0);
                    Vec6=Vec5/Vec4;
                    if(Vec6.X==7.0 && Vec6.Y==9.0 && Vec6.Z==11.0)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator/(cAudio::cVector3,Vector3)", temp);                             //test
                }

                temp = Skipped;
                Vec=Vector3(3.0,4.0,5.0);
                std::stringstream XMLstream;
                XMLstream<<Vec;
                Mezzanine::String Actual(XMLstream.str());
                Mezzanine::String Ideal("<Vector3 Version=\"1\" X=\"3\" Y=\"4\" Z=\"5\" />");
                if(Actual==Ideal)
                    { temp=Success; }
                else
                    { temp=Failed; }
                AddTestResult("Vector3::operator<<(ostream,Vector3)", temp);            //test

                temp = Skipped;
                try
                {
                    Vector3 Vec4;
                    XMLstream >> Vec4;
                    if(Vec4==Vec)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                }catch (Mezzanine::Exception& Ex){
                    std::cerr << "Failure in: Vector3::operator>>(istream,Vector3)" << std::endl <<"\t Mezzanine::Exception: " << Ex.GetCompleteMessage();
                    temp=Failed;
                }
                AddTestResult("Vector3::operator>>(istream,Vector3)", temp);            //test


                temp = Skipped;
                try
                {
                    Vector3 Vec5;
                    XML::Document XMLdoc;
                    XMLdoc.Load("<Vector3 Version=\"1\" X=\"3\" Y=\"4\" Z=\"5\"/>");
                    XMLdoc.GetFirstChild()>>Vec5;
                    if(Vec5==Vec)
                        { temp=Success; }
                    else
                        { temp=Failed; }
                }catch (Mezzanine::Exception& Ex){
                    std::cerr << "Failure in: Vector3::operator>>(XML::Node,Vector3)" << std::endl <<"\t Mezzanine::Exception: " << Ex.GetCompleteMessage();
                    temp=Failed;
                }
                AddTestResult("Vector3::operator>>(XML::Node,Vector3)", temp);          //test

                {
                    Ogre::Vector3 Vec0(0.0, 0.0, 0.0);

                    Mezzanine::Vector3 Vec4(1.5, 3.1, 7.2);
                    btVector3 Vec5(2.3, 9.4, 6.3);
                    cAudio::cVector3 Vec6(0.3, 3.6, 4.6);

                    Vec0<<Vec4;
                    if(Vector3(Vec0)==Vector3(Vec4))
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator<<(Ogre::Vector3,Vector3)", temp);          //test

                    Vec0<<Vec5;
                    if(Vector3(Vec0)==Vector3(Vec5))
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator<<(Ogre::Vector3,btVector3)", temp);          //test

                    Vec0<<Vec6;
                    if(Vector3(Vec0)==Vector3(Vec6))
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator<<(Ogre::Vector3,cAudio::cVector3)", temp);          //test
                }

                {
                    Mezzanine::Vector3 Vec0(0.0, 0.0, 0.0);

                    Ogre::Vector3 Vec4(1.5, 3.1, 7.2);
                    btVector3 Vec5(2.3, 9.4, 6.3);
                    cAudio::cVector3 Vec6(0.3, 3.6, 4.6);

                    Vec0<<Vec4;
                    if(Vector3(Vec0)==Vector3(Vec4))
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator<<(Vector3,Ogre::Vector3)", temp);          //test

                    Vec0<<Vec5;
                    if(Vector3(Vec0)==Vector3(Vec5))
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator<<(Vector3,btVector3)", temp);          //test

                    Vec0<<Vec6;
                    if(Vector3(Vec0)==Vector3(Vec6))
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator<<(Vector3,cAudio::cVector3)", temp);          //test
                }

                {
                    btVector3 Vec0(0.0, 0.0, 0.0);

                    Ogre::Vector3 Vec4(1.5, 3.1, 7.2);
                    Vector3 Vec5(2.3, 9.4, 6.3);
                    cAudio::cVector3 Vec6(0.3, 3.6, 4.6);

                    Vec0<<Vec4;
                    if(Vector3(Vec0)==Vector3(Vec4))
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator<<(btVector3,Ogre::Vector3)", temp);          //test

                    Vec0<<Vec5;
                    if(Vector3(Vec0)==Vector3(Vec5))
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator<<(btVector3,Vector3)", temp);          //test

                    Vec0<<Vec6;
                    if(Vector3(Vec0)==Vector3(Vec6))
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator<<(btVector3,cAudio::cVector3)", temp);          //test
                }

                {
                    cAudio::cVector3 Vec0(0.0, 0.0, 0.0);

                    Ogre::Vector3 Vec4(1.5, 3.1, 7.2);
                    Vector3 Vec5(2.3, 9.4, 6.3);
                    btVector3 Vec6(0.3, 3.6, 4.6);

                    Vec0<<Vec4;
                    if(Vector3(Vec0)==Vector3(Vec4))
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator<<(cAudio::cVector3,Ogre::Vector3)", temp);          //test

                    Vec0<<Vec5;
                    if(Vector3(Vec0)==Vector3(Vec5))
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator<<(cAudio::cVector3,Vector3)", temp);          //test

                    Vec0<<Vec6;
                    if(Vector3(Vec0)==Vector3(Vec6))
                        { temp=Success; }
                    else
                        { temp=Failed; }
                    AddTestResult("Vector3::operator<<(cAudio::cVector3,btVector3)", temp);          //test
                }

            }else{
                AddTestResult("Vector3::GetAxisValue(Integer)", Skipped);
                AddTestResult("Vector3::GetAxisValue(StandardAxis)", Skipped);
                AddTestResult("Vector3::Vector3()", Skipped);
                AddTestResult("Vector3::Vector3(Real,Real,Real)", Skipped);
                AddTestResult("Vector3::Vector3(Ogre::Vector3)", Skipped);
                AddTestResult("Vector3::Vector3(btVector3)", Skipped);
                AddTestResult("Vector3::Vector3(Vector3)", Skipped);
                AddTestResult("Vector3::Vector3(cAudio::cVector3)", Skipped);
                AddTestResult("Vector3::Unit_X()", Skipped);
                AddTestResult("Vector3::Unit_Y()", Skipped);
                AddTestResult("Vector3::Unit_Z()", Skipped);
                AddTestResult("Vector3::Neg_Unit_X()", Skipped);
                AddTestResult("Vector3::Neg_Unit_Y()", Skipped);
                AddTestResult("Vector3::Neg_Unit_Z()", Skipped);
                AddTestResult("Vector3::operator=(cAudio::cVector3)", Skipped);
                AddTestResult("Vector3::operator=(Ogre::Vector3)", Skipped);
                AddTestResult("Vector3::operator=(btVector3)", Skipped);
                AddTestResult("Vector3::operator-()", Skipped);
                AddTestResult("Vector3::operator/=(Real)", Skipped);
                AddTestResult("Vector3::operator*=(Real)", Skipped);
                AddTestResult("Vector3::operator/(Real)", Skipped);
                AddTestResult("Vector3::operator*(Real)", Skipped);
                AddTestResult("Vector3::operator==(Ogre::Vector3)", Skipped);
                AddTestResult("Vector3::operator!=(Ogre::Vector3)", Skipped);
                AddTestResult("Vector3::operator==(cAudio::cVector3)", Skipped);
                AddTestResult("Vector3::operator!=(cAudio::cVector3)", Skipped);
                AddTestResult("Vector3::operator==(btVector3)", Skipped);
                AddTestResult("Vector3::operator!=(btVector3)", Skipped);
                AddTestResult("Vector3::operator==(Vector3)", Skipped);
                AddTestResult("Vector3::operator!=(Vector3)", Skipped);
                AddTestResult("Vector3::operator+(Vector3)", Skipped);
                AddTestResult("Vector3::operator-(Vector3)", Skipped);
                AddTestResult("Vector3::operator*(Vector3)", Skipped);
                AddTestResult("Vector3::operator/(Vector3)", Skipped);
                AddTestResult("Vector3::operator+(btVector3)", Skipped);
                AddTestResult("Vector3::operator-(btVector3)", Skipped);
                AddTestResult("Vector3::operator*(btVector3)", Skipped);
                AddTestResult("Vector3::operator/(btVector3)", Skipped);
                AddTestResult("Vector3::operator+(Ogre::Vector3)", Skipped);
                AddTestResult("Vector3::operator-(Ogre::Vector3)", Skipped);
                AddTestResult("Vector3::operator*(Ogre::Vector3)", Skipped);
                AddTestResult("Vector3::operator/(Ogre::Vector3)", Skipped);
                AddTestResult("Vector3::operator+(cAudio::cVector3)", Skipped);
                AddTestResult("Vector3::operator-(cAudio::cVector3)", Skipped);
                AddTestResult("Vector3::operator*(cAudio::cVector3)", Skipped);
                AddTestResult("Vector3::operator/(cAudio::cVector3)", Skipped);
                AddTestResult("Vector3::CrossProduct(Vector3)", Skipped);
                AddTestResult("Vector3::DotProduct(Vector3)", Skipped);
                AddTestResult("Vector3::GetNormal()", Skipped);
                AddTestResult("Vector3::Normalize()", Skipped);
                AddTestResult("Vector3::GetDirection(Vector3)", Skipped);
                AddTestResult("Vector3::Inverse()", Skipped);
                AddTestResult("Vector3::GetBulletVector3()", Skipped);
                AddTestResult("Vector3::ExtractBulletVector3()", Skipped);
                AddTestResult("Vector3::GetOgreVector3()", Skipped);
                AddTestResult("Vector3::ExtractOgreVector3()", Skipped);
                AddTestResult("Vector3::GetcAudioVector3()", Skipped);
                AddTestResult("Vector3::ExtractcAudioVector3()", Skipped);
                AddTestResult("Vector3::Distance(Vector3)", Skipped);
                AddTestResult("Vector3::operator+(btVector3,Vector3)", Skipped);
                AddTestResult("Vector3::operator-(btVector3,Vector3)", Skipped);
                AddTestResult("Vector3::operator*(btVector3,Vector3)", Skipped);
                AddTestResult("Vector3::operator/(btVector3,Vector3)", Skipped);
                AddTestResult("Vector3::operator+(Ogre::Vector3,Vector3)", Skipped);
                AddTestResult("Vector3::operator-(Ogre::Vector3,Vector3)", Skipped);
                AddTestResult("Vector3::operator*(Ogre::Vector3,Vector3)", Skipped);
                AddTestResult("Vector3::operator/(Ogre::Vector3,Vector3)", Skipped);
                AddTestResult("Vector3::operator+(cAudio::cVector3,Vector3)", Skipped);
                AddTestResult("Vector3::operator-(cAudio::cVector3,Vector3)", Skipped);
                AddTestResult("Vector3::operator*(cAudio::cVector3,Vector3)", Skipped);
                AddTestResult("Vector3::operator/(cAudio::cVector3,Vector3)", Skipped);
                AddTestResult("Vector3::operator<<(ostream,Vector3)", Skipped);
                AddTestResult("Vector3::operator>>(istream,Vector3)", Skipped);
                AddTestResult("Vector3::operator>>(XML::Node,Vector3)", Skipped);
                AddTestResult("Vector3::operator<<(cAudio::cVector3,Ogre::Vector3)", Skipped);
                AddTestResult("Vector3::operator<<(cAudio::cVector3,Vector3)", Skipped);
                AddTestResult("Vector3::operator<<(cAudio::cVector3,btVector3)", Skipped);
                AddTestResult("Vector3::operator<<(Ogre::Vector3,Vector3)", Skipped);
                AddTestResult("Vector3::operator<<(Ogre::Vector3,btVector3)", Skipped);
                AddTestResult("Vector3::operator<<(Ogre::Vector3,cAudio::cVector3)", Skipped);
                AddTestResult("Vector3::operator<<(Vector3,Ogre::Vector3)", Skipped);
                AddTestResult("Vector3::operator<<(Vector3,btVector3)", Skipped);
                AddTestResult("Vector3::operator<<(Vector3,cAudio::cVector3)", Skipped);
                AddTestResult("Vector3::operator<<(btVector3,Ogre::Vector3)", Skipped);
                AddTestResult("Vector3::operator<<(btVector3,Vector3)", Skipped);
                AddTestResult("Vector3::operator<<(btVector3,cAudio::cVector3)", Skipped);
            }
        }
};

#endif


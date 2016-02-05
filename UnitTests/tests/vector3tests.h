// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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

#include "mezztest.h"

#include "mezzanine.h"

#include <Ogre.h>
#include <btBulletDynamicsCommon.h>
//#include <cAudio.h>

using namespace Mezzanine;
using namespace Mezzanine::Testing;

/// @brief A large number of test of the Vector3 class, nearly complete coverage.
class vector3tests : public UnitTestGroup
{
    public:
        /// @copydoc Mezzanine::Testing::UnitTestGroup::Name
        /// @return Returns a String containing "vector3"
        virtual String Name()
            { return String("Vector3"); }

        /// @brief Test most features on the vector3
        void RunAutomaticTests()
        {
            {
                TestOutput << "Checking sizeof Vector3 is stored efficiently, size is: " << sizeof(Vector3) << endl;
                TEST((sizeof(Mezzanine::Vector3::ChannelType)*3)==sizeof(Vector3), "sizeof");
                TestOutput << Vector3(1,2,3).__str__() << endl;
                TestOutput << Vector3(1,2,3) << endl;
                TestOutput << ToString(Vector3(1,2,3)) << endl;

            }

            {
                Vector3 Vec;
                Vec[0]=1.0; //use the non-const
                Vec[1]=2.0;
                Vec[2]=3.0;
                TEST(Vec[0]==1.0 && Vec[1]==2.0 && Vec[2]==3.0, "operator[](Whole)") // use the const
            }

            {
                Vector3 Vec;
                Vec.GetAxisValue(0)=4.0; // Non-const
                Vec.GetAxisValue(1)=5.0;
                Vec.GetAxisValue(2)=6.0;
                TEST(Vec.GetAxisValue(0)==4.0 && Vec.GetAxisValue(1)==5.0 && Vec.GetAxisValue(2)==6.0,"GetAxisValue(Whole)"); // Const
            }

            {
                Vector3 Vec;
                Vec[Axis_X]=1.0; //use the non-const
                Vec[Axis_Y]=2.0;
                Vec[Axis_Z]=3.0;
                TEST(Vec[Axis_X]==1.0 && Vec[Axis_Y]==2.0 && Vec[Axis_Z]==3.0, "operator[](StandardAxis)") // use the const
            }

            {
                Vector3 Vec;
                Vec.GetAxisValue(Axis_X)=7.0; // Non-const
                Vec.GetAxisValue(Axis_Y)=8.0;
                Vec.GetAxisValue(Axis_Z)=9.0;
                TEST(Vec.GetAxisValue(Axis_X)==7.0 && Vec.GetAxisValue(Axis_Y)==8.0 && Vec.GetAxisValue(Axis_Z)==9.0,"GetAxisValue(StandardAxis)"); // Const
            }

            {
                Vector3 Vec;
                TEST(Vec.X==0.0 && Vec.Y==0.0 && Vec.Z==0.0, "Vector3()");
            }

            {
                Vector3 Vec(2.0,1.0,3.0);
                TEST(Vec.X==2.0 && Vec.Y==1.0 && Vec.Z==3.0, "Vector3(Real,Real,Real)");
            }

            {
                Vector3 Vec(Ogre::Vector3(4.0,5.0,6.0));
                TEST(Vec.X==4.0 && Vec.Y==5.0 && Vec.Z==6.0, "Vector3(Ogre::Vector3)");
            }

            {
                Vector3 Vec=Vector3(btVector3(7.0,8.0,9.0));
                TEST(Vec.X==7.0 && Vec.Y==8.0 && Vec.Z==9.0, "Vector3(btVector3)");
            }

            {
                Vector3 Vec2(Vector3(10.0,11.0,12.0));
                TEST(Vec2.X==10.0 && Vec2.Y==11.0 && Vec2.Z==12.0,"Vector3(Vector3)");
            }

            {
                XML::Document XMLdoc;
                XMLdoc.Load("<Vector3 Version=\"1\" X=\"13\" Y=\"15\" Z=\"17\"/>");
                Vector3 VecNode(XMLdoc.GetFirstChild());
                TEST(VecNode.X==13.0 && VecNode.Y==15.0 && VecNode.Z==17.0, "Vector3(XML::Node)");
            }

            TEST(Vector3::Unit_X().X==1.0 && Vector3::Unit_X().Y==0.0 && Vector3::Unit_X().Z==0.0, "Unit_X()");
            TEST(Vector3::Unit_Y().X==0.0 && Vector3::Unit_Y().Y==1.0 && Vector3::Unit_Y().Z==0.0, "Unit_Y()");
            TEST(Vector3::Unit_Z().X==0.0 && Vector3::Unit_Z().Y==0.0 && Vector3::Unit_Z().Z==1.0, "Unit_Z()");

            TEST(Vector3::Neg_Unit_X().X==-1.0 && Vector3::Neg_Unit_X().Y==0.0  && Vector3::Neg_Unit_X().Z==0.0 , "Neg_Unit_X()");
            TEST(Vector3::Neg_Unit_Y().X==0.0  && Vector3::Neg_Unit_Y().Y==-1.0 && Vector3::Neg_Unit_Y().Z==0.0 , "Neg_Unit_Y()");
            TEST(Vector3::Neg_Unit_Z().X==0.0  && Vector3::Neg_Unit_Z().Y==0.0  && Vector3::Neg_Unit_Z().Z==-1.0, "Neg_Unit_Z()");

            TEST( Vector3(1.0,0.0,0.0) == Vector3::UnitOnAxis(Axis_X) &&
                  Vector3(0.0,1.0,0.0) == Vector3::UnitOnAxis(Axis_Y) &&
                  Vector3(0.0,0.0,1.0) == Vector3::UnitOnAxis(Axis_Z),
                  "UnitOnAxis()");

            TEST( Vector3(1.0,0.0,0.0).IsStandardUnitAxis() == Axis_X &&
                  Vector3(0.0,1.0,0.0).IsStandardUnitAxis() == Axis_Y &&
                  Vector3(0.0,0.0,1.0).IsStandardUnitAxis() == Axis_Z &&
                  Vector3(1.5,0.0,0.0).IsStandardUnitAxis() == Axis_Invalid &&
                  Vector3(0.5,0.0,0.0).IsStandardUnitAxis() == Axis_Invalid &&
                  Vector3(1.0,1.0,0.0).IsStandardUnitAxis() == Axis_Invalid &&
                  Vector3(0.0,1.0,1.0).IsStandardUnitAxis() == Axis_Invalid &&
                  Vector3(1.0,0.0,1.0).IsStandardUnitAxis() == Axis_Invalid &&
                  Vector3(0.0,0.0,0.0).IsStandardUnitAxis() == Axis_Invalid &&
                  Vector3(1.0,1.0,1.0).IsStandardUnitAxis() == Axis_Invalid,
                  "IsStandardUnitAxis()");

            {
                Vector3 Vec(0.0,0.0,0.0);
                Vec=btVector3(10.0,11.0,12.0);
                TEST(Vec.X==10.0 && Vec.Y==11.0 && Vec.Z==12.0, "operator=(btVector3)");
            }

            {
                Vector3 Vec(0.0,0.0,0.0);
                Vec=Ogre::Vector3(10.0,11.0,12.0);
                TEST(Vec.X==10.0 && Vec.Y==11.0 && Vec.Z==12.0, "operator=(Ogre::Vector3)");
            }

            {
                Vector3 Vec(2.0,1.0,3.0);
                Vec= -Vec;
                TEST(Vec.X==-2.0 && Vec.Y==-1.0 && Vec.Z==-3.0, "operator-()");
            }

            {
                Vector3 Vec(2.0,1.0,3.0);
                Vec=Vec*2;
                TEST(Vec.X==4.0 && Vec.Y==2.0 && Vec.Z==6.0, "operator*(Real)");
            }

            {
                Vector3 Vec(4.0,2.0,6.0);
                Vec=Vec/2;
                TEST(Vec.X==2.0 && Vec.Y==1.0 && Vec.Z==3.0, "operator/(Real)");
            }

            {
                Vector3 Vec(2.0,1.0,3.0);
                Vec*=2.0;
                TEST(Vec.X==4.0 && Vec.Y==2.0 && Vec.Z==6.0, "operator*=(Real)");
            }

            {
                Vector3 Vec(4.0,2.0,6.0);
                Vec/=2.0;
                TEST(Vec.X==2.0 && Vec.Y==1.0 && Vec.Z==3.0, "operator/=(Real)");
            }

            {
                Vector3 VecA(1.0,2.0,3.0);
                Vector3 Vec1(1.0,2.0,3.0);
                Vector3 Vec2(4.0,5.0,6.0);
                Vector3 Vec3(0.0,2.0,3.0);
                Vector3 Vec4(1.0,0.0,3.0);
                Vector3 Vec5(1.0,2.0,0.0);
                TEST(VecA==Vec1 && !(VecA==Vec2) && !(VecA==Vec3) && !(VecA==Vec4) && !(VecA==Vec5), "operator==(Vector3)");
                TEST(!(VecA!=Vec1) && VecA!=Vec2 && VecA!=Vec3 && VecA!=Vec4 && VecA!=Vec5, "operator!=(Vector3)")
            }

            {
                btVector3 Vec1(4.0,5.0,6.0);
                Vector3 VecA(1.0,2.0,3.0);
                btVector3 Vec2(1.0,2.0,3.0);
                btVector3 Vec3(0.0,2.0,3.0);
                btVector3 Vec4(1.0,0.0,3.0);
                btVector3 Vec5(1.0,2.0,0.0);
                TEST(VecA==Vec2 && !(VecA==Vec1) && !(VecA==Vec3) && !(VecA==Vec4) && !(VecA==Vec5), "operator==(btVector3)");
                TEST(!(VecA!=Vec2) && VecA!=Vec1 && VecA!=Vec3 && VecA!=Vec4 && VecA!=Vec5, "operator!=(btVector3)");
            }

            {
                Vector3 VecA(1.0,2.0,3.0);
                Ogre::Vector3 Vec1(1.0,2.0,3.0);
                Ogre::Vector3 Vec2(4.0,5.0,6.0);
                Ogre::Vector3 Vec3(0.0,2.0,3.0);
                Ogre::Vector3 Vec4(1.0,0.0,3.0);
                Ogre::Vector3 Vec5(1.0,2.0,0.0);
                TEST(VecA==Vec1 && !(VecA==Vec2) && !(VecA==Vec3) && !(VecA==Vec4) && !(VecA==Vec5), "operator==(Ogre::Vector3)");
                TEST(!(VecA!=Vec1) && VecA!=Vec2 && VecA!=Vec3 && VecA!=Vec4 && VecA!=Vec5, "operator==(Ogre::Vector3)");
            }

            {
                Vector3 VecQ(1.0,2.0,3.0);
                Vector3 Vec1(1.0,2.0,3.0);
                Vector3 Vec2(0.0,5.0,6.0);
                Vector3 Vec3(0.0,2.0,3.0);
                Vector3 Vec4(1.0,0.0,3.0);
                Vector3 Vec5(1.0,2.0,0.0);
                Vector3 Vec6(2.0,3.0,4.0);
                TEST(VecQ<=Vec1 && !(VecQ<=Vec2) && !(VecQ<=Vec3) && !(VecQ<=Vec4) && !(VecQ<=Vec5) && (VecQ<=Vec6), "operator<=");
                TEST(VecQ>=Vec1 && !(VecQ>=Vec2) && (VecQ>=Vec3) && (VecQ>=Vec4) && (VecQ>=Vec5) && !(VecQ>=Vec6), "operator>=");
            }

            {
                Vector3 Vec1(3.0,4.0,5.0);
                Vector3 Vec2(7.0,9.0,11.0);
                Vector3 VecAnswer;

                VecAnswer=Vec1+Vec2;
                TEST(VecAnswer.X==10.0 && VecAnswer.Y==13.0 && VecAnswer.Z==16.0, "operator+(Vector3)");

                VecAnswer=Vector3(0,0,0);
                VecAnswer=Vec1-Vec2;
                TEST(VecAnswer.X==-4.0 && VecAnswer.Y==-5.0 && VecAnswer.Z==-6.0, "operator-(Vector3)");

                VecAnswer=Vector3(0,0,0);
                VecAnswer=Vec1*Vec2;
                TEST(VecAnswer.X==21.0 && VecAnswer.Y==36.0 && VecAnswer.Z==55.0, "operator*(Vector3)");

                Vec1=Vector3(21.0,36.0,55.0);
                VecAnswer=Vector3(0,0,0);
                VecAnswer=Vec1/Vec2;
                TEST(VecAnswer.X==3.0 && VecAnswer.Y==4.0 && VecAnswer.Z==5.0,"operator/(Vector3)");
            }

            {
                Vector3 Vec1(3.0,4.0,5.0);
                btVector3 Vec2(7.0,9.0,11.0);
                Vector3 VecAnswer;

                VecAnswer=Vec1+Vec2;
                TEST(VecAnswer.X==10.0 && VecAnswer.Y==13.0 && VecAnswer.Z==16.0, "operator+(btVector3)");

                VecAnswer=Vector3(0,0,0);
                VecAnswer=Vec1-Vec2;
                TEST(VecAnswer.X==-4.0 && VecAnswer.Y==-5.0 && VecAnswer.Z==-6.0, "operator-(btVector3)");

                VecAnswer=Vector3(0,0,0);
                VecAnswer=Vec1*Vec2;
                TEST(VecAnswer.X==21.0 && VecAnswer.Y==36.0 && VecAnswer.Z==55.0, "operator*(btVector3)");

                Vec1=Vector3(21.0,36.0,55.0);
                VecAnswer=Vector3(0,0,0);
                VecAnswer=Vec1/Vec2;
                TEST(VecAnswer.X==3.0 && VecAnswer.Y==4.0 && VecAnswer.Z==5.0, "operator/(btVector3)");
            }

            {
                Vector3 Vec1(3.0,4.0,5.0);
                Ogre::Vector3 Vec2(7.0,9.0,11.0);
                Vector3 VecAnswer;

                VecAnswer=Vec1+Vec2;
                TEST(VecAnswer.X==10.0 && VecAnswer.Y==13.0 && VecAnswer.Z==16.0, "operator+(Ogre::Vector3)");

                VecAnswer=Vector3(0,0,0);
                VecAnswer=Vec1-Vec2;
                TEST(VecAnswer.X==-4.0 && VecAnswer.Y==-5.0 && VecAnswer.Z==-6.0, "operator-(Ogre::Vector3)");

                VecAnswer=Vector3(0,0,0);
                VecAnswer=Vec1*Vec2;
                TEST(VecAnswer.X==21.0 && VecAnswer.Y==36.0 && VecAnswer.Z==55.0, "operator*(Ogre::Vector3)");

                Vec1=Vector3(21.0,36.0,55.0);
                VecAnswer=Vector3(0,0,0);
                VecAnswer=Vec1/Vec2;
                TEST(VecAnswer.X==3.0 && VecAnswer.Y==4.0 && VecAnswer.Z==5.0, "operator/(Ogre::Vector3)");
            }

            {
                Vector3 Vec1(1.0,2.0,3.0);
                Vector3 Vec2(4.0,5.0,6.0);
                Vector3 VecAnswer;
                // 1,2,3 . 4,5,6
                // 2*6-3*5 = -3
                // 3*4-1*6 = 6
                // 1*5-2*4 = -3
                VecAnswer=Vec1.CrossProduct(Vec2);
                TEST(VecAnswer.X==-3.0 && VecAnswer.Y==6.0 && VecAnswer.Z==-3.0, "CrossProduct(Vector3)");
            }

            {
                Vector3 Vec1(1.0,2.0,3.0);
                Vector3 Vec2(4.0,5.0,6.0);
                // 1,2,3 . 4,5,6
                // 1*4 + 2*5 + 3*6 = 32
                Real Prod=Vec1.DotProduct(Vec2);
                TEST(Prod==32.0,"DotProduct(Vector3)");
            }

            {
                Vector3 Vec1(112.0,0.0,0.0);
                Vector3 Vec2(0.0,23.0,0.0);
                Vector3 Vec3(0.0,0.0,-7000.0);
                Vector3 VecX(1.0,0.0,0.0);
                Vector3 VecY(0.0,1.0,0.0);
                Vector3 VecZ(0.0,0.0,-1.0);

                Vec1.Normalize(); Vec3.Normalize(); Vec2.Normalize();
                TEST(VecX==Vec1 && VecY==Vec2 && VecZ==Vec3, "Normalize()");
            }

            {
                Vector3 Vec1(112.0,0.0,0.0);
                Vector3 Vec2(0.0,23.0,0.0);
                Vector3 Vec3(0.0,0.0,-7000.0);
                Vector3 VecX(1.0,0.0,0.0);
                Vector3 VecY(0.0,1.0,0.0);
                Vector3 VecZ(0.0,0.0,-1.0);

                TEST(VecX==Vec1.GetNormal() && VecY==Vec2.GetNormal() && VecZ==Vec3.GetNormal(), "GetNormal()");
            }


            {
                Vector3 Vec1(1.0,2.0,3.0);
                Vector3 VecX(3.0,1.0,2.0);

                TEST(VecX==Vec1.Permute(), "Permute()");
            }


            {
                Vector3 Vec1(1.0,2.0,3.0);
                Vector3 VecX(3.0,1.0,2.0);

                TEST(VecX==Vec1.GetPermute(), "GetPermute()");
            }


            {
                Vector3 Vec1(1.0,2.0,3.0);
                Vector3 VecX(2.0,3.0,1.0);

                TEST(VecX==Vec1.AntiPermute(), "AntiPermute()");
            }


            {
                Vector3 Vec1(1.0,2.0,3.0);
                Vector3 VecX(2.0,3.0,1.0);

                TEST(VecX==Vec1.GetAntiPermute(), "GetAntiPermute()");
            }

            {
                Vector3 Vec1(112.0,0.0,0.0);
                Vector3 Vec2(0.0,23.0,0.0);
                Vector3 Vec3(0.0,0.0,-7000.0);
                Vector3 VecX(1.0,0.0,0.0);
                Vector3 VecY(0.0,1.0,0.0);
                Vector3 VecZ(0.0,0.0,-1.0);
                Vector3 VecD(0.0,0.0,0.0);

                TEST(VecX==VecD.GetDirection(Vec1) && VecY==VecD.GetDirection(Vec2) && VecZ==VecD.GetDirection(Vec3), "GetDirection(Vector3)");
            }

            {
                Vector3 Vec1(5.0,0.0,0.0);
                Vector3 Vec2(0.0,0.0,5.0);
                Vector3 VecX(-3.0,3.0,0.0);
                Vector3 VecY(3.0,3.0,0.0);

                Boole Result = Vec1.IsPerpendicular(Vec2) && VecX.IsPerpendicular(VecY);
                TEST(Result, "IsPerpendicular(Vector3&)");

                Vector3 PerpTest1(45.0,28.0,36.0);
                Vector3 PerpTest2 = PerpTest1.Perpendicular();
                TEST(Result && PerpTest1.IsPerpendicular(PerpTest2) ,"Perpendicular()");
            }

            {
                Vector3 Vec1(20.0,0.0,0.0);
                Vector3 Vec2(0.0,5.0,0.0);
                Vector3 Vec3(0.0,0.0,-50.0);
                Vector3 Vec0(0.0,0.0,0.0);
                Vector3 VecX(0.05,0.0,0.0);
                Vector3 VecY(0.0,0.2,0.0);
                Vector3 VecZ(0.0,0.0,-0.02);
                Vector3 VecO(0.0,0.0,0.0);

                Vec1.Inverse(); Vec2.Inverse(); Vec3.Inverse(); VecO.Inverse();
                TEST(Vec1==VecX && Vec2==VecY && Vec3==VecZ && Vec0==VecO, "Inverse()");
            }

            {
                Vector3 Vec(2.0,1.0,3.0);
                btVector3 Bvec(Vec.GetBulletVector3());
                TEST(Bvec.getX()==2.0 && Bvec.getY()==1.0 && Bvec.getZ()==3.0, "GetBulletVector3()");
                Bvec = btVector3(4.0,5.0,6.0);
                Vec.ExtractBulletVector3(Bvec);
                TEST(Vec.X==4.0 && Vec.Y==5.0 && Vec.Z==6.0,"ExtractBulletVector3()");
            }

            {
                Vector3 Vec(2.0,1.0,3.0);
                Ogre::Vector3 Ovec(Vec.GetOgreVector3());
                TEST(Ovec.x==2.0 && Ovec.y==1.0 && Ovec.z==3.0,"GetOgreVector3()");

                Ovec = Ogre::Vector3(4.0,5.0,6.0);
                Vec.ExtractOgreVector3(Ovec);
                TEST(Vec.X==4.0 && Vec.Y==5.0 && Vec.Z==6.0, "ExtractOgreVector3()");
            }

            {
                Vector3 VecOrigin(5.0,5.0,5.0);
                Vector3 VecX(10.0,5.0,5.0);
                Vector3 VecY(5.0,20.0,5.0);
                Vector3 VecZ(5.0,5.0,-50.0);
                TEST(VecOrigin.Distance(VecX)==5.0 && VecOrigin.Distance(VecY)==15.0 && VecOrigin.Distance(VecZ)==55.0, "Distance(Vector3)");
            }

            {
                Vector3 Vec1(3.0,4.0,5.0);
                btVector3 Vec2(7.0,9.0,11.0);
                Vector3 VecAnswer;

                VecAnswer=Vec2+Vec1;
                TEST(VecAnswer.X==10.0 && VecAnswer.Y==13.0 && VecAnswer.Z==16.0, "operator+(btVector3,Vector3)");

                VecAnswer=Vector3(0,0,0);
                VecAnswer=Vec2-Vec1;
                TEST(VecAnswer.X==4.0 && VecAnswer.Y==5.0 && VecAnswer.Z==6.0, "operator-(btVector3,Vector3)");

                VecAnswer=Vector3(0,0,0);
                VecAnswer=Vec2*Vec1;
                TEST(VecAnswer.X==21.0 && VecAnswer.Y==36.0 && VecAnswer.Z==55.0, "operator*(btVector3,Vector3)");

                Vec2=btVector3(21.0,36.0,55.0);
                VecAnswer=Vector3(0,0,0);
                VecAnswer=Vec2/Vec1;
                TEST(VecAnswer.X==7.0 && VecAnswer.Y==9.0 && VecAnswer.Z==11.0, "operator/(btVector3,Vector3)");
            }

            {
                Vector3 Vec1(3.0,4.0,5.0);
                Ogre::Vector3 Vec2(7.0,9.0,11.0);
                Vector3 VecAnswer;

                VecAnswer=Vec2+Vec1;
                TEST(VecAnswer.X==10.0 && VecAnswer.Y==13.0 && VecAnswer.Z==16.0, "operator+(Ogre::Vector3,Vector3)");

                VecAnswer=Vector3(0,0,0);
                VecAnswer=Vec2-Vec1;
                TEST(VecAnswer.X==4.0 && VecAnswer.Y==5.0 && VecAnswer.Z==6.0, "operator-(Ogre::Vector3,Vector3)");

                VecAnswer=Vector3(0,0,0);
                VecAnswer=Vec2*Vec1;
                TEST(VecAnswer.X==21.0 && VecAnswer.Y==36.0 && VecAnswer.Z==55.0, "operator*(Ogre::Vector3,Vector3)");

                Vec2=Ogre::Vector3(21.0,36.0,55.0);
                VecAnswer=Vector3(0,0,0);
                VecAnswer=Vec2/Vec1;
                TEST(VecAnswer.X==7.0 && VecAnswer.Y==9.0 && VecAnswer.Z==11.0, "operator/(Ogre::Vector3,Vector3)");
            }

            {
                Vector3 Vec(3.0,4.0,5.0);
                std::stringstream XMLstream;
                XMLstream<<Vec;
                Mezzanine::String Actual(XMLstream.str());
                Mezzanine::String Ideal("<Vector3 Version=\"1\" X=\"3\" Y=\"4\" Z=\"5\" />");
                TEST(Actual==Ideal,"operator<<(ostream,Vector3)");

                try
                {
                    Vector3 VecStream;
                    XMLstream >> VecStream;
                    TEST(VecStream==Vec,"operator>>(istream,Vector3)");
                }catch (Mezzanine::ExceptionBase& Ex){
                    TestError << "Failure in: Vector3::operator>>(istream,Vector3)" << std::endl <<"\t Mezzanine::Exception: " << Ex.GetCompleteMessage();
                    TEST_RESULT(Testing::Failed,"operator>>(istream,Vector3)");
                }

                try
                {
                    Vector3 VecLoad;
                    XML::Document XMLdoc;
                    XMLdoc.Load("<Vector3 Version=\"1\" X=\"3\" Y=\"4\" Z=\"5\"/>");
                    XMLdoc.GetFirstChild()>>VecLoad;
                    TEST(VecLoad==Vec, "operator>>(XML::Node,Vector3)");
                }catch (Mezzanine::ExceptionBase& Ex){
                    TestError << "Failure in: Vector3::operator>>(XML::Node,Vector3)" << std::endl <<"\t Mezzanine::Exception: " << Ex.GetCompleteMessage();
                    TEST_RESULT(Testing::Failed,"operator>>(XML::Node,Vector3)");
                }
            }

            {
                Ogre::Vector3 OVec(0.0, 0.0, 0.0);

                Vector3 MezzVec(1.5, 3.1, 7.2);
                btVector3 BVec(2.3, 9.4, 6.3);

                OVec<<MezzVec;
                TEST(Vector3(OVec)==Vector3(MezzVec), "operator<<(Ogre::Vector3,Vector3)");

                OVec<<BVec;
                TEST(Vector3(OVec)==Vector3(BVec), "<<(Ogre::Vector3,btVector3)");
            }

            {
                Mezzanine::Vector3 MezzVec(0.0, 0.0, 0.0);

                Ogre::Vector3 OVec(1.5, 3.1, 7.2);
                btVector3 BVec(2.3, 9.4, 6.3);
                // ©Audio::cVector3 Vec6(0.3, 3.6, 4.6);

                MezzVec<<OVec;
                TEST(Vector3(MezzVec)==Vector3(OVec), "operator<<(Vector3,Ogre::Vector3)");

                MezzVec<<BVec;
                TEST(Vector3(MezzVec)==Vector3(BVec), "operator<<(Vector3,btVector3)");
            }

            {
                btVector3 BVec(0.0, 0.0, 0.0);

                Ogre::Vector3 OVec(1.5, 3.1, 7.2);
                Vector3 MezzVec(2.3, 9.4, 6.3);

                BVec<<OVec;
                TEST(Vector3(BVec)==Vector3(OVec), "operator<<(btVector3,Ogre::Vector3)");

                BVec<<MezzVec;
                TEST(Vector3(BVec)==Vector3(MezzVec), "operator<<(btVector3,Vector3)");
            }

            /// @todo Write Unit tests for Length, SquaredDistance, IsZeroLength, GetRotationToAxis, Zero, SetValues, ProtoDeSerialize, ProtoSerialize, SerializableName
        }

        /// @brief Since RunAutomaticTests is implemented so is this.
        /// @return returns true
        virtual bool HasAutomaticTests() const
            { return true; }
};

#endif


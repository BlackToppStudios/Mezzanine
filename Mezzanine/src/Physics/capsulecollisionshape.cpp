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
#ifndef _physicscapsulecollisionshape_cpp
#define _physicscapsulecollisionshape_cpp

#include "Physics/capsulecollisionshape.h"
#include "collisionshapemanager.h"
#include "stringtool.h"

#include "btBulletDynamicsCommon.h"

namespace Mezzanine
{
    namespace Physics
    {
        /////////////////////////////////////////
        // CapsuleCollisionShape Functions

        void CapsuleCollisionShape::Construct(const String& Name, const Real& Radius, const Real& Height, StandardAxis UpAxis)
        {
            this->Name = Name;
            btCapsuleShape* CapsuleShape = 0;
            if(Axis_Y == UpAxis) CapsuleShape = new btCapsuleShape(Radius,Height);
            else if(Axis_X == UpAxis) CapsuleShape = new btCapsuleShapeX(Radius,Height);
            else if(Axis_Z == UpAxis) CapsuleShape = new btCapsuleShapeZ(Radius,Height);
            else { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Non-supported up Axis passed into CapsuleCollisionShape constructor."); }
            SetPointers(CapsuleShape);
        }

        CapsuleCollisionShape::CapsuleCollisionShape(const String& Name, const Real& Radius, const Real& Height, const Vector3& UpAxis)
            { this->Construct(Name,Radius,Height,UpAxis.IsStandardUnitAxis()); }

        CapsuleCollisionShape::CapsuleCollisionShape(const String& Name, const Real& Radius, const Real& Height, StandardAxis UpAxis)
            { this->Construct(Name,Radius,Height,UpAxis); }

        CapsuleCollisionShape::CapsuleCollisionShape(XML::Node OneNode)
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                XML::Attribute OneName = OneNode.GetChild("PrimitiveCollisionShape").GetChild("CollisionShape").GetAttribute("Name");               // get name
                if(!OneName) { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Could not find Name Attribute on CollsionShape Node during preparation for deserialization"); }
                String Name_(OneName.AsString());

                XML::Attribute Axis = OneNode.GetAttribute("Axis");
                if (!Axis) { DeSerializeError("find Axis Attribute",CapsuleCollisionShape::GetSerializableName()); }
                /*
                XML::Attribute Radius = OneNode.GetAttribute("Radius");
                if (!Radius) { DeSerializeError("find Radius Attribute",CapsuleCollisionShape::GetSerializableName()); }
                XML::Attribute Height = OneNode.GetAttribute("Height");
                if (!Height) { DeSerializeError("find Height Attribute",CapsuleCollisionShape::GetSerializableName()); }
                //SetPointers(new CapsuleCollisionShape(Name_,Radius.AsReal(),Height.AsReal(), (StandardAxis)Axis.AsInteger());  // make and deserialize the shape
                this->Construct(Name_,Radius.AsReal(),Height.AsReal(),(StandardAxis)Axis.AsInteger());
                */
                this->Construct(Name_,0,0,(StandardAxis)Axis.AsInteger());

                this->ProtoDeSerialize(OneNode);

            }else{
                DeSerializeError("find usable serialization version",CapsuleCollisionShape::GetSerializableName());
            }
        }

        CapsuleCollisionShape::CapsuleCollisionShape(const String& Name, btCapsuleShape* BulletShape)
        {
            this->Name = Name;
            SetPointers(BulletShape);
        }

        CapsuleCollisionShape::~CapsuleCollisionShape()
        {
            delete GetBulletCapsuleShape();
        }

        Real CapsuleCollisionShape::GetRadius() const
            { return GetBulletCapsuleShape()->getRadius(); }
        Real CapsuleCollisionShape::GetHeight() const
            { return (GetBulletCapsuleShape()->getHalfHeight() * 2.0); }

        Real CapsuleCollisionShape::GetCleanRadius() const
            { return (this->GetRadius()+GetBulletCapsuleShape()->getMargin()) / GetScaling()[(StandardAxis)((this->GetUpStandardAxis()+2)%3)] - GetBulletCapsuleShape()->getMargin(); }
            //{ return GetRadiusWithMargin() / GetScaling()[(StandardAxis)((this->GetUpStandardAxis()+2)%3)] - GetBulletCapsuleShape()->getMargin(); }
        Real CapsuleCollisionShape::GetCleanHeight() const
            //{ return 2.0* ((GetBulletCapsuleShape()->getHalfHeight()+GetBulletCapsuleShape()->getMargin()) / GetScaling()[this->GetUpStandardAxis()] - GetBulletCapsuleShape()->getMargin()); }
            { return 2.0* ((GetBulletCapsuleShape()->getHalfHeight()) / GetScaling()[this->GetUpStandardAxis()]); }
        /* Real CapsuleCollisionShape::GetRadiusWithMargin() const
            { return this->GetRadius()+GetBulletCapsuleShape()->getMargin(); }*/

        Vector3 CapsuleCollisionShape::GetUpAxis() const
            { return Vector3::UnitOnAxis( (StandardAxis)GetBulletCapsuleShape()->getUpAxis() ); }

        StandardAxis CapsuleCollisionShape::GetUpStandardAxis() const
            { return (StandardAxis)GetBulletCapsuleShape()->getUpAxis(); }

        CollisionShape::ShapeType CapsuleCollisionShape::GetType() const
        {
            return CollisionShape::ST_Capsule;
        }

        btCapsuleShape* CapsuleCollisionShape::GetBulletCapsuleShape() const
            { return static_cast<btCapsuleShape*>(ShapeBase); }

        void CapsuleCollisionShape::ProtoSerialize(XML::Node& CurrentRoot) const
        {
            XML::Node CollisionNode = CurrentRoot.AppendChild(this->CapsuleCollisionShape::GetSerializableName());
            if (!CollisionNode) { SerializeError("create CollisionNode",this->CapsuleCollisionShape::GetSerializableName());}

            XML::Attribute Version = CollisionNode.AppendAttribute("Version");
            if (Version)
                { Version.SetValue(1); }
            else
                { SerializeError("Create Version Attribute", GetSerializableName()); }
/*
            XML::Attribute RadiusAttr = CollisionNode.AppendAttribute("Radius");
            if (RadiusAttr)
                { RadiusAttr.SetValue(this->GetCleanRadius()); }
            else
                { SerializeError("Create RadiusAttr Attribute", GetSerializableName()); }

            XML::Attribute HeightAttr = CollisionNode.AppendAttribute("Height");
            if (HeightAttr)
                { HeightAttr.SetValue(this->GetCleanHeight()); }
            else
                { SerializeError("Create HeightAttr Attribute", GetSerializableName()); }
*/
            XML::Attribute AxisAttr = CollisionNode.AppendAttribute("Axis");
            if (AxisAttr)
                { AxisAttr.SetValue(this->GetUpStandardAxis()); }
            else
                { SerializeError("Create AxisAttr Attribute", GetSerializableName()); }

            this->PrimitiveCollisionShape::ProtoSerialize(CollisionNode);
        }

        void CapsuleCollisionShape::ProtoDeSerialize(const XML::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->CapsuleCollisionShape::GetSerializableName() )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    XML::Node CollisionNode = OneNode.GetChild(this->PrimitiveCollisionShape::GetSerializableName());
                    if(!CollisionNode)
                        { DeSerializeError("locate PrimitiveCollisionShape node",GetSerializableName()); }
                    this->PrimitiveCollisionShape::ProtoDeSerialize(CollisionNode);
                }else{
                    DeSerializeError("find usable serialization version",GetSerializableName());
                }
            }else{
                DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),GetSerializableName());
            }
        }

        String CapsuleCollisionShape::GetSerializableName()
            {   return String("CapsuleCollisionShape"); }
    }//Physics
}//Mezzanine

    std::ostream& operator << (std::ostream& stream, const Mezzanine::Physics::CapsuleCollisionShape& ShapeToSerialize)
        { Mezzanine::Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, Mezzanine::Physics::CapsuleCollisionShape& x)
        { return Mezzanine::DeSerialize(stream, x); }

    void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::CapsuleCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

#endif

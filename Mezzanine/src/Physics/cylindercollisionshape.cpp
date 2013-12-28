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
#ifndef _physicscylindercollisionshape_cpp
#define _physicscylindercollisionshape_cpp

#include "Physics/cylindercollisionshape.h"
#include "collisionshapemanager.h"
#include "stringtool.h"

#include "btBulletDynamicsCommon.h"

namespace Mezzanine
{
    namespace Physics
    {
        /////////////////////////////////////////
        // CylinderCollisionShape Functions

        void CylinderCollisionShape::Construct(const String& Name, const Vector3& HalfExtents, StandardAxis UpAxis)
        {
            btCylinderShape* CylinderShape;

            switch (UpAxis)
            {
                case Axis_Y:
                    CylinderShape = new btCylinderShape(HalfExtents.GetBulletVector3());
                    break;
                case Axis_X:
                    CylinderShape = new btCylinderShapeX(HalfExtents.GetBulletVector3());
                    break;
                case Axis_Z:
                    CylinderShape = new btCylinderShapeZ(HalfExtents.GetBulletVector3());
                    break;
                default: { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Non-supported up Axis passed into CylinderCollisionShape constructor."); }
            }
            Construct(Name, CylinderShape);
        }

        void CylinderCollisionShape::Construct(const String& Name, btCylinderShape* BulletShape)
        {
            this->Name = Name;
            SetPointers(BulletShape);
        }

        CylinderCollisionShape::CylinderCollisionShape(const String& Name, const Real& Radius, const Real& Height, const Vector3& UpAxis)
            { Construct(Name, CreateHalfExtents(Radius,Height,UpAxis.IsStandardUnitAxis()), UpAxis.IsStandardUnitAxis()); }

        CylinderCollisionShape::CylinderCollisionShape(const String& Name, const Real& Radius, const Real& Height, StandardAxis UpAxis)
            { Construct(Name, CreateHalfExtents(Radius,Height,UpAxis), UpAxis); }

        CylinderCollisionShape::CylinderCollisionShape(const String& Name, const Vector3& HalfExtents, const Vector3& UpAxis)
            { Construct(Name, HalfExtents, UpAxis.IsStandardUnitAxis()); }

        CylinderCollisionShape::CylinderCollisionShape(const String& Name, const Vector3& HalfExtents, StandardAxis UpAxis)
            { Construct(Name, HalfExtents, UpAxis); }

        CylinderCollisionShape::CylinderCollisionShape(const String& Name, btCylinderShape* BulletShape)
            { Construct(Name,BulletShape); }

        CylinderCollisionShape::CylinderCollisionShape(XML::Node OneNode)
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                XML::Attribute OneName = OneNode.GetChild("PrimitiveCollisionShape").GetChild("CollisionShape").GetAttribute("Name");               // get name
                if(!OneName) { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Could not find Name Attribute on CollsionShape Node during preparation for deserialization"); }

                /*XML::Node HalfExtentsNode = OneNode.GetChild("HalfExtents").GetFirstChild();
                if (!HalfExtentsNode) { DeSerializeError("find HalfExtentsNode",CylinderCollisionShape::SerializableName()); }
                SetPointers(new CylinderShape(Vector3(HalfExtentsNode).GetBulletVector3()));
                // */

                XML::Attribute Axis = OneNode.GetAttribute("Axis");
                if (!Axis) { DeSerializeError("find Axis Attribute",CylinderCollisionShape::SerializableName()); }

                Construct(OneName.AsString(), Vector3(), (StandardAxis)Axis.AsInteger());

                this->ProtoDeSerialize(OneNode);
            }else{
                DeSerializeError("find usable serialization version", CylinderCollisionShape::SerializableName());
            }
        }

        CylinderCollisionShape::~CylinderCollisionShape()
        {
            delete GetBulletCylinderShape();
        }

        Vector3 CylinderCollisionShape::CreateHalfExtents(const Real& Radius, const Real& Height, StandardAxis UpAxis)
        {
            switch (UpAxis)
            {
                case Axis_Y: return Vector3(Radius,Height,Radius);
                case Axis_X: return Vector3(Height,Radius,Radius);
                case Axis_Z: return Vector3(Radius,Radius,Height);
                default: { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Non-supported up Axis passed into CylinderCollisionShape::CreateHalfExtents."); }
            }
        }

        Vector3 CylinderCollisionShape::GetHalfExtents() const
        {
            Vector3 HalfExtents(GetBulletCylinderShape()->getHalfExtentsWithoutMargin());
            return HalfExtents;
        }

        Vector3 CylinderCollisionShape::GetHalfExtentsWithMargin() const
        {
            Vector3 HalfExtents(GetBulletCylinderShape()->getHalfExtentsWithMargin());
            return HalfExtents;
        }

        Real CylinderCollisionShape::GetRadius() const
            { return GetBulletCylinderShape()->getRadius(); }

        Vector3 CylinderCollisionShape::GetUpAxis() const
            { return Vector3::UnitOnAxis((StandardAxis)GetBulletCylinderShape()->getUpAxis()); }

        CollisionShape::ShapeType CylinderCollisionShape::GetType() const
            { return CollisionShape::ST_Cylinder; }

        btCylinderShape* CylinderCollisionShape::GetBulletCylinderShape() const
            { return static_cast<btCylinderShape*>(ShapeBase); }

        void CylinderCollisionShape::ProtoSerialize(XML::Node& CurrentRoot) const
        {
            XML::Node CollisionNode = CurrentRoot.AppendChild(this->CylinderCollisionShape::SerializableName());
            if (!CollisionNode) { SerializeError("create CollisionNode",this->CylinderCollisionShape::SerializableName());}

            /*
            XML::Node HalfExtentsNode = CollisionNode.AppendChild("HalfExtents");
            if (!HalfExtentsNode) { SerializeError("create HalfExtentsNode",this->CylinderCollisionShape::SerializableName());}
            //this->GetHalfExtents().ProtoSerialize(HalfExtentsNode);
            this->GetCleanHalfExtents().ProtoSerialize(HalfExtentsNode);
            */

            XML::Attribute Version = CollisionNode.AppendAttribute("Version");
            if (Version)
                { Version.SetValue(1); }
            else
                { SerializeError("Create Version Attribute", SerializableName()); }

            XML::Attribute Axis = CollisionNode.AppendAttribute("Axis");
            if (Axis)
                { Axis.SetValue((StandardAxis)GetBulletCylinderShape()->getUpAxis()); }
            else
                { SerializeError("Create Axis Attribute", SerializableName()); }

            this->PrimitiveCollisionShape::ProtoSerialize(CollisionNode);
        }

        void CylinderCollisionShape::ProtoDeSerialize(const XML::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->CylinderCollisionShape::SerializableName() )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    XML::Node CollisionNode = OneNode.GetChild(this->PrimitiveCollisionShape::SerializableName());
                    if(!CollisionNode)
                        { DeSerializeError("locate PrimitiveCollisionShape node",SerializableName()); }
                    this->PrimitiveCollisionShape::ProtoDeSerialize(CollisionNode);
                }else{
                    DeSerializeError("find usable serialization version",SerializableName());
                }
            }else{
                DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
            }
        }

        String CylinderCollisionShape::SerializableName()
            {   return String("CylinderCollisionShape"); }

    }//Physics
}//Mezzanine

std::ostream& operator << (std::ostream& stream, const Mezzanine::Physics::CylinderCollisionShape& ShapeToSerialize)
    { Mezzanine::Serialize(stream, ShapeToSerialize); return stream; }

std::istream& operator >> (std::istream& stream, Mezzanine::Physics::CylinderCollisionShape& x)
    { return Mezzanine::DeSerialize(stream, x); }

void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::CylinderCollisionShape& x)
    { x.ProtoDeSerialize(OneNode); }

#endif

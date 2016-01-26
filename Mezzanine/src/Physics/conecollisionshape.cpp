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
#ifndef _physicsconecollisionshape_cpp
#define _physicsconecollisionshape_cpp

#include "Physics/conecollisionshape.h"
#include "collisionshapemanager.h"
#include "stringtool.h"

#include "btBulletDynamicsCommon.h"

namespace Mezzanine
{
    namespace Physics
    {
        /////////////////////////////////////////
        // ConeCollisionShape Functions

        void ConeCollisionShape::Construct(const String& Name, const Real& Radius, const Real& Height, StandardAxis UpAxis)
        {
            this->Name = Name;
            btConeShape* ConeShape;
            if(Axis_Y == UpAxis) ConeShape = new btConeShape(Radius,Height);
            else if(Axis_X == UpAxis) ConeShape = new btConeShapeX(Radius,Height);
            else if(Axis_Z == UpAxis) ConeShape = new btConeShapeZ(Radius,Height);
            else { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Non-supported up StandardAxis passed into ConeCollisionShape constructor."); }
            SetPointers(ConeShape);
            this->GetBulletConeShape()->setImplicitShapeDimensions(Vector3(0,0,0).GetBulletVector3());
        }

        Vector3 ConeCollisionShape::GetAxisMathBS() const
        {
            Vector3 Results;
            switch (GetUpStandardAxis())
            {
                case 0:
                        Results[0] = 1;
                        Results[1] = 0;
                        Results[2] = 2;
                    break;
                case 1:
                        Results[0] = 0;
                        Results[1] = 1;
                        Results[2] = 2;
                    break;
                case 2:
                        Results[0] = 0;
                        Results[1] = 2;
                        Results[2] = 1;
                    break;
                default:
                    { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Non-supported up StandardAxis passed into ConeCollisionShape::GetAxisMathBS()."); }
            }

            return Results;
        }

        ConeCollisionShape::ConeCollisionShape(const String& Name, const Real& Radius, const Real& Height, const Vector3& UpAxis)
            { Construct(Name, Radius, Height, UpAxis.IsStandardUnitAxis()); }

        ConeCollisionShape::ConeCollisionShape(const String& Name, const Real& Radius, const Real& Height, StandardAxis UpAxis)
            { Construct(Name, Radius, Height, UpAxis); }

        ConeCollisionShape::ConeCollisionShape(XML::Node OneNode)
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                XML::Attribute OneName = OneNode.GetChild("PrimitiveCollisionShape").GetChild("CollisionShape").GetAttribute("Name");               // get name
                if(!OneName) { MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Could not find Name Attribute on CollsionShape Node during preparation for deserialization"); }
                String Name_(OneName.AsString());

                XML::Attribute Radius = OneNode.GetAttribute("Radius");                                                                             // Find Attributes
                if (!Radius) { DeSerializeError("find Radius Attribute",ConeCollisionShape::GetSerializableName()); }
                XML::Attribute Axis = OneNode.GetAttribute("Axis");
                if (!Axis) { DeSerializeError("find Axis Attribute",ConeCollisionShape::GetSerializableName()); }
                XML::Attribute Height = OneNode.GetAttribute("Height");
                if (!Height) { DeSerializeError("find Height Attribute",ConeCollisionShape::GetSerializableName()); }

                this->Construct(Name_,Radius.AsReal(),Height.AsReal(), (StandardAxis)Axis.AsInteger());        // make and deserialize the shape
                this->ProtoDeSerialize(OneNode);
            }else{
                DeSerializeError("find usable serialization version",ConeCollisionShape::GetSerializableName());
            }
        }

        ConeCollisionShape::ConeCollisionShape(const String& Name, btConeShape* BulletShape)
        {
            this->Name = Name;
            SetPointers(BulletShape);
            this->GetBulletConeShape()->setImplicitShapeDimensions(Vector3(0,0,0).GetBulletVector3());
        }

        ConeCollisionShape::~ConeCollisionShape()
        {
            delete GetBulletConeShape();
        }

        Real ConeCollisionShape::GetRadius() const
        {
            return GetBulletConeShape()->getRadius();
        }

        Real ConeCollisionShape::GetHeight() const
        {
            return GetBulletConeShape()->getHeight();
        }

        Real ConeCollisionShape::GetCleanRadius() const
        {
            return (GetRadius()) / GetRadiusScaling();
        }

        Real ConeCollisionShape::GetCleanHeight() const
        {
            return GetHeight() / GetHeightScaling();
        }

        Real ConeCollisionShape::GetRadiusScaling() const
            { return (GetScaling()[GetAxisMathBS()[0]]+GetScaling()[GetAxisMathBS()[2]])/2.0; }

        Real ConeCollisionShape::GetHeightScaling() const
            { return GetScaling()[GetUpStandardAxis()]; }

        Vector3 ConeCollisionShape::GetUpAxis() const
            { return Vector3::UnitOnAxis( (StandardAxis)GetBulletConeShape()->getConeUpIndex() ); }

        StandardAxis ConeCollisionShape::GetUpStandardAxis() const
            { return (StandardAxis)GetBulletConeShape()->getConeUpIndex(); }


        CollisionShape::ShapeType ConeCollisionShape::GetType() const
        {
            return CollisionShape::ST_Cone;
        }

        btConeShape* ConeCollisionShape::GetBulletConeShape() const
            { return static_cast<btConeShape*>(ShapeBase); }

        void ConeCollisionShape::ProtoSerialize(XML::Node& CurrentRoot) const
        {
            XML::Node CollisionNode = CurrentRoot.AppendChild(this->ConeCollisionShape::GetSerializableName());
            if (!CollisionNode) { SerializeError("create CollisionNode",this->ConeCollisionShape::GetSerializableName());}

            XML::Attribute Version = CollisionNode.AppendAttribute("Version");
            if (Version)
                { Version.SetValue(1); }
            else
                { SerializeError("Create Version Attribute", GetSerializableName()); }

            XML::Attribute RadiusAttr = CollisionNode.AppendAttribute("Radius");
            if (RadiusAttr)
                { RadiusAttr.SetValue(this->GetCleanRadius()); }
            else
                { SerializeError("Create RadiusAttr Attribute", GetSerializableName()); }

            /*XML::Attribute DirtyRadius = CollisionNode.AppendAttribute("DirtyRadius");
            if (DirtyRadius)
                { DirtyRadius.SetValue(this->GetRadius()); }
            else
                { SerializeError("Create DirtyRadius Attribute", GetSerializableName()); }*/

            XML::Attribute HeightAttr = CollisionNode.AppendAttribute("Height");
            if (HeightAttr)
                { HeightAttr.SetValue(this->GetCleanHeight()); }
            else
                { SerializeError("Create HeightAttr Attribute", GetSerializableName()); }

            XML::Attribute AxisAttr = CollisionNode.AppendAttribute("Axis");
            if (AxisAttr)
                { AxisAttr.SetValue(this->GetUpStandardAxis()); }
            else
                { SerializeError("Create AxisAttr Attribute", GetSerializableName()); }

            this->PrimitiveCollisionShape::ProtoSerialize(CollisionNode);
        }

        void ConeCollisionShape::ProtoDeSerialize(const XML::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->ConeCollisionShape::GetSerializableName() )
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

        String ConeCollisionShape::GetSerializableName()
            {   return String("ConeCollisionShape"); }
    }//Physics
}//Mezzanine

    std::ostream& operator << (std::ostream& stream, const Mezzanine::Physics::ConeCollisionShape& ShapeToSerialize)
        { Mezzanine::Serialize(stream, ShapeToSerialize); return stream; }

    std::istream& operator >> (std::istream& stream, Mezzanine::Physics::ConeCollisionShape& x)
        { return Mezzanine::DeSerialize(stream, x); }

    void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::ConeCollisionShape& x)
        { x.ProtoDeSerialize(OneNode); }

#endif

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
#ifndef _physicsplanecollisionshape_cpp
#define _physicsplanecollisionshape_cpp

#include "Physics/planecollisionshape.h"
#include "collisionshapemanager.h"
#include "stringtool.h"

#include "btBulletDynamicsCommon.h"

namespace Mezzanine
{
    namespace Physics
    {
        /////////////////////////////////////////
        // PlaneCollisionShape Functions

        PlaneCollisionShape::PlaneCollisionShape(const String& Name, const Plane& Other)
        {
            this->Name = Name;
            this->PlaneShape = new btStaticPlaneShape(Other.Normal.GetBulletVector3(),-(Other.Distance));
            this->SetPointers(this->PlaneShape);
        }

        PlaneCollisionShape::PlaneCollisionShape(const String& Name, const Vector3& Norm, const Real Constant)
        {
            this->Name = Name;
            this->PlaneShape = new btStaticPlaneShape(Norm.GetBulletVector3(),Constant);
            this->SetPointers(this->PlaneShape);
        }

        PlaneCollisionShape::PlaneCollisionShape(const String& Name, btStaticPlaneShape* BulletShape)
        {
            this->Name = Name;
            this->PlaneShape = BulletShape;
            this->SetPointers(this->PlaneShape);
        }

        PlaneCollisionShape::PlaneCollisionShape(const XML::Node& XMLNode)
        {
            this->ProtoDeSerialize(XMLNode);
        }

        PlaneCollisionShape::~PlaneCollisionShape()
            { delete this->PlaneShape; }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Vector3 PlaneCollisionShape::GetNormal() const
            { return Vector3( this->PlaneShape->getPlaneNormal() ); }

        Real PlaneCollisionShape::GetConstant() const
            { return this->PlaneShape->getPlaneConstant(); }

        CollisionShape::ShapeType PlaneCollisionShape::GetType() const
            { return CollisionShape::ST_Plane; }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialization

        void PlaneCollisionShape::ProtoSerialize(XML::Node& CurrentRoot) const
        {
            XML::Node CollisionNode = CurrentRoot.AppendChild(this->PlaneCollisionShape::GetSerializableName());
            if (!CollisionNode) { SerializeError("create CollisionNode",this->PlaneCollisionShape::GetSerializableName());}

            XML::Attribute Version = CollisionNode.AppendAttribute("Version");
            XML::Attribute Constant = CollisionNode.AppendAttribute("Constant");
            XML::Node Normal = CollisionNode.AppendChild("Normal");
            if (Version && Normal && Constant) {
                Version.SetValue(1);
                Constant.SetValue(this->PlaneShape->getPlaneConstant());

                Vector3 Norm(this->PlaneShape->getPlaneNormal());
                Norm.ProtoSerialize(Normal);
            }else{
                SerializeError("Create Version Attribute", GetSerializableName());
            }

            this->FieldCollisionShape::ProtoSerialize(CollisionNode);
        }

        void PlaneCollisionShape::ProtoDeSerialize(const XML::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->PlaneCollisionShape::GetSerializableName() )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    XML::Attribute Constant = OneNode.GetAttribute("Constant");
                    if(Constant)
                        { this->PlaneShape->setPlaneConstant( Constant.AsReal() ); }

                    XML::Node Normal = OneNode.GetChild("Normal").GetFirstChild();
                    if(Normal)
                        { this->PlaneShape->setPlaneNormal( Vector3(Normal).GetBulletVector3() ); }

                    XML::Node CollisionNode = OneNode.GetChild(this->FieldCollisionShape::GetSerializableName());
                    if(!CollisionNode)
                        { DeSerializeError("locate FieldCollisionShape node",GetSerializableName()); }
                    this->FieldCollisionShape::ProtoDeSerialize(CollisionNode);

                }else{
                    DeSerializeError("find usable serialization version",GetSerializableName());
                }
            }else{
                DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),GetSerializableName());
            }
        }

        String PlaneCollisionShape::GetSerializableName()
            { return "PlaneCollisionShape"; }
    }//Physics
}//Mezzanine

std::ostream& operator << (std::ostream& stream, const Mezzanine::Physics::PlaneCollisionShape& ShapeToSerialize)
    { Mezzanine::Serialize(stream, ShapeToSerialize); return stream; }

std::istream& operator >> (std::istream& stream, Mezzanine::Physics::PlaneCollisionShape& x)
    { return Mezzanine::DeSerialize(stream, x); }

void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::PlaneCollisionShape& x)
    { x.ProtoDeSerialize(OneNode); }

#endif

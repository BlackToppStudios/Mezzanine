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
#ifndef _physicsboxcollisionshape_cpp
#define _physicsboxcollisionshape_cpp

#include "Physics/boxcollisionshape.h"
#include "collisionshapemanager.h"
#include "stringtool.h"

#include "btBulletDynamicsCommon.h"

namespace Mezzanine
{
    namespace Physics
    {
        /////////////////////////////////////////
        // BoxCollisionShape Functions

        BoxCollisionShape::BoxCollisionShape(const String& Name, const Vector3& HalfExtents)
        {
            this->Name = Name;
            SetPointers(new btBoxShape(HalfExtents.GetBulletVector3()));
        }

        BoxCollisionShape::BoxCollisionShape(const String& Name, btBoxShape* BulletShape)
        {
            this->Name = Name;
            SetPointers(BulletShape);
        }

        BoxCollisionShape::BoxCollisionShape(XML::Node OneNode)
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                XML::Attribute OneName = OneNode.GetChild("PrimitiveCollisionShape").GetChild("CollisionShape").GetAttribute("Name");               // get name
                if(!OneName) { MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Could not find Name Attribute on CollsionShape Node during preparation for deserialization"); }
                this->Name=OneName.AsString();

                /*XML::Node HalfExtentsNode = OneNode.GetChild("HalfExtents").GetFirstChild();
                if (!HalfExtentsNode) { DeSerializeError("find HalfExtentsNode",BoxCollisionShape::SerializableName()); }
                SetPointers(new btBoxShape(Vector3(HalfExtentsNode).GetBulletVector3()));
                // */
                SetPointers(new btBoxShape(Vector3().GetBulletVector3()));

                this->ProtoDeSerialize(OneNode);
            }else{
                DeSerializeError("find usable serialization version",BoxCollisionShape::SerializableName());
            }
        }

        BoxCollisionShape::~BoxCollisionShape()
        {
            delete GetBulletBoxShape();
        }

        Vector3 BoxCollisionShape::GetCleanHalfExtents() const
        {
            return (this->GetHalfExtentsWithMargin()/this->GetScaling());
        }

        Vector3 BoxCollisionShape::GetHalfExtents() const
        {
            return Vector3(GetBulletBoxShape()->getHalfExtentsWithoutMargin());
        }

        Vector3 BoxCollisionShape::GetHalfExtentsWithMargin() const
        {
            return Vector3(GetBulletBoxShape()->getHalfExtentsWithMargin());
        }

        Boole BoxCollisionShape::IsInside(const Vector3& Location, const Real& Tolerance) const
        {
            return GetBulletBoxShape()->isInside(Location.GetBulletVector3(),Tolerance);
        }

        CollisionShape::ShapeType BoxCollisionShape::GetType() const
        {
            return CollisionShape::ST_Box;
        }

        btBoxShape* BoxCollisionShape::GetBulletBoxShape() const
            { return static_cast<btBoxShape*>(ShapeBase); }

        void BoxCollisionShape::ProtoSerialize(XML::Node& CurrentRoot) const
        {
            XML::Node CollisionNode = CurrentRoot.AppendChild(this->BoxCollisionShape::SerializableName());
            if (!CollisionNode) { SerializeError("create CollisionNode",this->BoxCollisionShape::SerializableName());}

            /*
            XML::Node HalfExtentsNode = CollisionNode.AppendChild("HalfExtents");
            if (!HalfExtentsNode) { SerializeError("create HalfExtentsNode",this->BoxCollisionShape::SerializableName());}
            //this->GetHalfExtents().ProtoSerialize(HalfExtentsNode);
            this->GetCleanHalfExtents().ProtoSerialize(HalfExtentsNode);
            */

            XML::Attribute Version = CollisionNode.AppendAttribute("Version");
            if (Version)
                { Version.SetValue(1); }
            else
                { SerializeError("Create Version Attribute", SerializableName()); }

            this->PrimitiveCollisionShape::ProtoSerialize(CollisionNode);
        }

        void BoxCollisionShape::ProtoDeSerialize(const XML::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->BoxCollisionShape::SerializableName() )
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

        String BoxCollisionShape::SerializableName()
            {   return String("BoxCollisionShape"); }

    }//Physics
}//Mezzanine

std::ostream& operator << (std::ostream& stream, const Mezzanine::Physics::BoxCollisionShape& ShapeToSerialize)
    { Mezzanine::Serialize(stream, ShapeToSerialize); return stream; }

std::istream& operator >> (std::istream& stream, Mezzanine::Physics::BoxCollisionShape& x)
    { return Mezzanine::DeSerialize(stream, x); }

void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::BoxCollisionShape& x)
    { x.ProtoDeSerialize(OneNode); }

#endif

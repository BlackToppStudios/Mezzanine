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
#ifndef _physicsprimitivecollisionshape_cpp
#define _physicsprimitivecollisionshape_cpp

#include "Physics/primitivecollisionshape.h"
#include "collisionshapemanager.h"
#include "stringtool.h"

#include "btBulletDynamicsCommon.h"

namespace Mezzanine
{
    namespace Physics
    {
        /////////////////////////////////////////
        // PrimitiveCollisionShape Functions

        PrimitiveCollisionShape::PrimitiveCollisionShape()
        {
        }

        PrimitiveCollisionShape::~PrimitiveCollisionShape()
        {
        }

        void PrimitiveCollisionShape::SetPointers(btConvexInternalShape* Shape)
            { ShapeBase = Shape; }

        btConvexInternalShape* PrimitiveCollisionShape::GetBulletConvexShape() const
            { return static_cast<btConvexInternalShape*>(ShapeBase); }

        void PrimitiveCollisionShape::ProtoSerialize(XML::Node& CurrentRoot) const
        {
            XML::Node CollisionNode = CurrentRoot.AppendChild(this->PrimitiveCollisionShape::SerializableName());
            if (!CollisionNode) { SerializeError("create CollisionNode",this->PrimitiveCollisionShape::SerializableName());}

            XML::Attribute Version = CollisionNode.AppendAttribute("Version");
            if (!Version)
                { SerializeError("Create Version Attribute", SerializableName()); }
            Version.SetValue(1);

            XML::Node ImplicitShape = CollisionNode.AppendChild("ImplicitShape");
            if (!ImplicitShape)
                { SerializeError("Create ImplicitShape Node", SerializableName()); }
            Vector3(GetBulletConvexShape()->getImplicitShapeDimensions()).ProtoSerialize(ImplicitShape);

            this->CollisionShape::ProtoSerialize(CollisionNode);

        }

        void PrimitiveCollisionShape::ProtoDeSerialize(const XML::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->PrimitiveCollisionShape::SerializableName() )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    XML::Node CollisionNode = OneNode.GetChild("CollisionShape");
                    if(!CollisionNode)
                        { DeSerializeError("locate CollisionShape node",SerializableName()); }
                    this->CollisionShape::ProtoDeSerialize(CollisionNode);

                    XML::Node ImplicitShape = OneNode.GetChild("ImplicitShape").GetFirstChild();
                    if(!ImplicitShape)
                        { DeSerializeError("locate ImplicitShape node",SerializableName()); }
                    GetBulletConvexShape()->setImplicitShapeDimensions(Vector3(ImplicitShape).GetBulletVector3());
                }else{
                    DeSerializeError("find usable serialization version",SerializableName());
                }
            }else{
                DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),SerializableName());
            }
        }

        String PrimitiveCollisionShape::SerializableName()
            {   return String("PrimitiveCollisionShape"); }

    }//Physics
}//Mezzanine

std::ostream& operator << (std::ostream& stream, const Mezzanine::Physics::PrimitiveCollisionShape& ShapeToSerialize)
    { Mezzanine::Serialize(stream, ShapeToSerialize); return stream; }

std::istream& operator >> (std::istream& stream, Mezzanine::Physics::PrimitiveCollisionShape& x)
    { return Mezzanine::DeSerialize(stream, x); }

void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::PrimitiveCollisionShape& x)
    { x.ProtoDeSerialize(OneNode); }

#endif

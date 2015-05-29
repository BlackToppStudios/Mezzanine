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
#ifndef _physicsfieldcollisionshape_cpp
#define _physicsfieldcollisionshape_cpp

#include "Physics/fieldcollisionshape.h"
#include "collisionshapemanager.h"
#include "stringtool.h"

#include "btBulletDynamicsCommon.h"

namespace Mezzanine
{
    namespace Physics
    {
        /////////////////////////////////////////
        // FieldCollisionShape Functions
        FieldCollisionShape::FieldCollisionShape()
        {
        }

        FieldCollisionShape::~FieldCollisionShape()
        {
        }

        void FieldCollisionShape::SetPointers(btConcaveShape* Shape)
            { ShapeBase = Shape; }

        btConcaveShape* FieldCollisionShape::GetBulletConcaveShape() const
            { return static_cast<btConcaveShape*>(ShapeBase); }

        void FieldCollisionShape::ProtoSerialize(XML::Node& CurrentRoot) const
        {
            XML::Node CollisionNode = CurrentRoot.AppendChild(this->FieldCollisionShape::GetSerializableName());
            if (!CollisionNode) { SerializeError("create CollisionNode",this->FieldCollisionShape::GetSerializableName());}

            XML::Attribute Version = CollisionNode.AppendAttribute("Version");
            if (Version)
                { Version.SetValue(1); }
            else
                { SerializeError("Create Version Attribute", GetSerializableName()); }

            this->CollisionShape::ProtoSerialize(CollisionNode);

        }

        void FieldCollisionShape::ProtoDeSerialize(const XML::Node& OneNode)
        {
            if ( Mezzanine::String(OneNode.Name())==this->FieldCollisionShape::GetSerializableName() )
            {
                if(OneNode.GetAttribute("Version").AsInt() == 1)
                {
                    XML::Node CollisionNode = OneNode.GetChild("CollisionShape");
                    if(!CollisionNode)
                        { DeSerializeError("locate CollisionShape node",GetSerializableName()); }
                    this->CollisionShape::ProtoDeSerialize(CollisionNode);
                }else{
                    DeSerializeError("find usable serialization version",GetSerializableName());
                }
            }else{
                DeSerializeError(String("find correct class to deserialize, found a ")+OneNode.Name(),GetSerializableName());
            }
        }

        String FieldCollisionShape::GetSerializableName()
            {   return String("FieldCollisionShape"); }
    }//Physics
}//Mezzanine

std::ostream& operator << (std::ostream& stream, const Mezzanine::Physics::FieldCollisionShape& ShapeToSerialize)
    { Mezzanine::Serialize(stream, ShapeToSerialize); return stream; }

std::istream& operator >> (std::istream& stream, Mezzanine::Physics::FieldCollisionShape& x)
    { return Mezzanine::DeSerialize(stream, x); }

void operator >> (const Mezzanine::XML::Node& OneNode, Mezzanine::Physics::FieldCollisionShape& x)
    { x.ProtoDeSerialize(OneNode); }

#endif

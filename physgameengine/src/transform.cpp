//© Copyright 2010 - 2011 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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
#ifndef _transform_cpp
#define _transform_cpp

#include "serialization.h"
#include "transform.h"

#include <btBulletDynamicsCommon.h>



namespace phys
{
    Transform::Transform(const Vector3& Vec, const Quaternion& Quat):
        Location(Vec),
        Rotation(Quat)
    {}

    Transform::Transform(const btTransform& Btt)
        { this->ExtractBulletTransform(Btt); }

    Transform::Transform(const Transform& TheOther)
        { this->operator=(TheOther); }

    btTransform Transform::GetBulletTransform() const
    {
        return btTransform( this->Rotation.GetBulletQuaternion(),
                            this->Location.GetBulletVector3()
                            );
    }

    void Transform::ExtractBulletTransform(const btTransform& temp)
    {
        this->Location.ExtractBulletVector3(temp.getOrigin());
        this->Rotation.ExtractBulletQuaternion(temp.getRotation());
    }


    Transform& Transform::operator= (const Transform& rhs)
    {
        if( this == &rhs ) // Are we assigning to ourselves?!?!?!?
        {
            return *this;
        }else{
            this->Location = rhs.Location;
            this->Rotation = rhs.Rotation;
        }
    }

    Transform& Transform::operator= (const btTransform& rhs)
    {
        this->Location.ExtractBulletVector3(rhs.getOrigin());
        this->Rotation.ExtractBulletQuaternion(rhs.getRotation());
        return *this;
    }

    void Transform::ProtoSerialize(xml::Node& CurrentRoot) const
    {
        xml::Node TransformNode = CurrentRoot.AppendChild(SerializableName());                     // The base node all the base constraint stuff will go in
        if (!TransformNode)
            { SerializeError("Create TransformNode", SerializableName()); }

        this->Location.ProtoSerialize(TransformNode);
        this->Rotation.ProtoSerialize(TransformNode);
    }

    void Transform::ProtoDeSerialize(const xml::Node& OneNode)
    {
        if ( phys::String(OneNode.Name())==phys::String(SerializableName()) )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                this->Location.ProtoDeSerialize(OneNode.GetChild("Vector3"));
                this->Rotation.ProtoDeSerialize(OneNode.GetChild("Quaternion"));
            }else{
                throw( phys::Exception(StringCat("Incompatible XML Version for ",SerializableName(),": Not Version 1")) );
            }
        }else{
            throw( phys::Exception(phys::StringCat("Attempting to deserialize a ",SerializableName(),", found a ", OneNode.Name())));
        }
    }

    String Transform::SerializableName() const
        { return String("Transform"); }

}

btTransform& operator<< (btTransform& lhs, const phys::Transform& rhs)
{
    lhs.setOrigin(rhs.Location.GetBulletVector3());
    lhs.setRotation(rhs.Rotation.GetBulletQuaternion());
    return lhs;
}

phys::Transform& operator<< (phys::Transform& lhs, const btTransform& rhs)
    { return lhs=rhs; }


#endif

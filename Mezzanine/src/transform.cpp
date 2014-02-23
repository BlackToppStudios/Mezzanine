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
#ifndef _transform_cpp
#define _transform_cpp

#include "serialization.h"
#include "stringtool.h"
#include "transform.h"
#include "exception.h"

#include <btBulletDynamicsCommon.h>



namespace Mezzanine
{
    Transform::Transform(const Vector3& Vec, const Quaternion& Quat):
        Location(Vec),
        Rotation(Quat)
    {}

    Transform::Transform(const btTransform& Btt)
        { this->ExtractBulletTransform(Btt); }

    Transform::Transform(const Transform& TheOther)
        { this->operator=(TheOther); }

    void Transform::SetIdentity()
    {
        this->Location.Zero();
        this->Rotation.SetIdentity();
    }

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
        if( this != &rhs ) // Are we assigning to ourselves?!?!?!?
        {
            this->Location = rhs.Location;
            this->Rotation = rhs.Rotation;
        }
        return *this;
    }

    Transform& Transform::operator= (const btTransform& rhs)
    {
        this->Location.ExtractBulletVector3(rhs.getOrigin());
        this->Rotation.ExtractBulletQuaternion(rhs.getRotation());
        return *this;
    }

    void Transform::ProtoSerialize(XML::Node& CurrentRoot) const
    {
        XML::Node TransformNode = CurrentRoot.AppendChild(SerializableName());                     // The base node all the base constraint stuff will go in
        if (!TransformNode)
            { SerializeError("Create TransformNode", SerializableName()); }

        Mezzanine::XML::Attribute Version = TransformNode.AppendAttribute("Version");                            // Version
        if (!Version)
            { SerializeError("Create Version", SerializableName()); }
        Version.SetValue(1);

        this->Location.ProtoSerialize(TransformNode);
        this->Rotation.ProtoSerialize(TransformNode);
    }

    void Transform::ProtoDeSerialize(const XML::Node& OneNode)
    {
        if ( Mezzanine::String(OneNode.Name())==Mezzanine::String(SerializableName()) )
        {
            if(OneNode.GetAttribute("Version").AsInt() == 1)
            {
                this->Location.ProtoDeSerialize(OneNode.GetChild("Vector3"));
                this->Rotation.ProtoDeSerialize(OneNode.GetChild("Quaternion"));
            }else{
                MEZZ_EXCEPTION(Exception::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + SerializableName() + ": Not Version 1.");
            }
        }else{
            MEZZ_EXCEPTION(Exception::II_IDENTITY_INVALID_EXCEPTION,"Attempting to deserialize a " + SerializableName() + ", found a " + String(OneNode.Name()));
        }
    }

    String Transform::SerializableName()
        { return String("Transform"); }

    Transform Transform::operator- (const Transform& rhs) const
        { return Transform(this->Location-rhs.Location,this->Rotation-rhs.Rotation); }

    Transform Transform::operator+ (const Transform& rhs) const
        { return Transform(this->Location+rhs.Location, this->Rotation+rhs.Rotation); }

    Transform Transform::operator* (Real rhs) const
        { return Transform(this->Location*rhs,this->Rotation*rhs); }

    Transform Transform::operator/ (Real rhs) const
        { return Transform(this->Location/rhs,this->Rotation/rhs); }

    Boolean Transform::operator<= (const Transform& rhs) const
        { return this->Location<=rhs.Location && this->Rotation<=rhs.Rotation; }

    Boolean Transform::operator>= (const Transform& rhs) const
        { return this->Location>=rhs.Location && this->Rotation>=rhs.Rotation; }
}


std::ostream& operator << (std::ostream& stream, const Mezzanine::Transform& rhs)
{
    Serialize(stream,rhs);
    return stream;
}

btTransform& operator<< (btTransform& lhs, const Mezzanine::Transform& rhs)
{
    lhs.setOrigin(rhs.Location.GetBulletVector3());
    lhs.setRotation(rhs.Rotation.GetBulletQuaternion());
    return lhs;
}

Mezzanine::Transform& operator<< (Mezzanine::Transform& lhs, const btTransform& rhs)
    { return lhs=rhs; }


#endif

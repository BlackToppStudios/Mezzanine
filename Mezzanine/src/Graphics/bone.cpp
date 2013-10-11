//© Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _graphicsbone_cpp
#define _graphicsbone_cpp

#include "Graphics/skeleton.h"
#include "Graphics/bone.h"

#include <Ogre.h>

namespace Mezzanine
{
    namespace Graphics
    {
        namespace
        {
            /// @internal
            /// @brief Converts the transform space used by this class to the internal type.
            Ogre::Node::TransformSpace ConvertTransformSpace(const Mezzanine::TransformSpace Space)
            {
                switch(Space)
                {
                    case Mezzanine::TS_Local:  return Ogre::Node::TS_LOCAL;
                    case Mezzanine::TS_Parent: return Ogre::Node::TS_PARENT;
                    case Mezzanine::TS_World:  return Ogre::Node::TS_WORLD;
                }
                return Ogre::Node::TS_WORLD;
            }
        }

        Bone::Bone(Skeleton* HostSkel, Ogre::Bone* InternalBone)
            : GraphicsBone(InternalBone),
              Host(HostSkel)
        {
            Ogre::Any OgreRef(this);
            GraphicsBone->setUserAny(OgreRef);
        }

        Bone::~Bone()
        {
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        Skeleton* Bone::GetHost() const
            { return this->Host; }

        const String& Bone::GetName() const
            { return this->GraphicsBone->getName(); }

        UInt16 Bone::GetHandle() const
            { return this->GraphicsBone->getHandle(); }

        void Bone::SetManuallyControlled(bool Manual)
            { this->GraphicsBone->setManuallyControlled(Manual); }

        bool Bone::GetManuallyControlled() const
            { return this->GraphicsBone->isManuallyControlled(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Child Methods

        Bone* Bone::CreateChild(const UInt16 Handle, const Vector3& Trans, const Quaternion& Rot)
            { return this->Host->_CreateBoneWrapper( this->GraphicsBone->createChild( Handle, Trans.GetOgreVector3(), Rot.GetOgreQuaternion() ) ); }

        UInt16 Bone::GetNumChildren() const
            { return this->GraphicsBone->numChildren(); }

        Bone* Bone::GetChild(const UInt16 Index) const
            { return Ogre::any_cast<Bone*>( this->GraphicsBone->getChild(Index)->getUserAny() ); }

        Bone* Bone::GetChild(const String& Name) const
            { return Ogre::any_cast<Bone*>( this->GraphicsBone->getChild(Name)->getUserAny() ); }

        void Bone::RemoveChild(Bone* ToBeRemoved)
            { this->GraphicsBone->removeChild( ToBeRemoved->_GetInternalBone() ); }

        void Bone::RemoveChild(const UInt16 Index)
            { this->GraphicsBone->removeChild( Index ); }

        void Bone::RemoveChild(const String& Name)
            { this->GraphicsBone->removeChild( Name ); }

        ///////////////////////////////////////////////////////////////////////////////
        // Transform Methods

        void Bone::SetLocation(const Vector3& Loc)
            { this->GraphicsBone->setPosition( Loc.GetOgreVector3() ); }

        void Bone::SetLocation(const Real X, const Real Y, const Real Z)
            { this->GraphicsBone->setPosition(X,Y,Z); }

        Vector3 Bone::GetLocation() const
            { return Vector3( this->GraphicsBone->getPosition() ); }

        void Bone::SetOrientation(const Quaternion& Ori)
            { this->GraphicsBone->setOrientation( Ori.GetOgreQuaternion() ); }

        void Bone::SetOrientation(const Real X, const Real Y, const Real Z, const Real W)
            { this->GraphicsBone->setOrientation(W,X,Y,Z); }

        Quaternion Bone::GetOrientation() const
            { return Quaternion( this->GraphicsBone->getOrientation() ); }

        void Bone::SetScale(const Vector3& Sc)
            { this->GraphicsBone->setScale( Sc.GetOgreVector3() ); }

        void Bone::SetScale(const Real X, const Real Y, const Real Z)
            { this->GraphicsBone->setScale(X,Y,Z); }

        Vector3 Bone::GetScale() const
            { return Vector3( this->GraphicsBone->getScale() ); }

        void Bone::Translate(const Vector3& Trans, const Mezzanine::TransformSpace Space)
            { this->GraphicsBone->translate( Trans.GetOgreVector3(), ConvertTransformSpace(Space) ); }

        void Bone::Translate(const Real X, const Real Y, const Real Z, const Mezzanine::TransformSpace Space)
            { this->GraphicsBone->translate( X, Y, Z, ConvertTransformSpace(Space) ); }

        void Bone::Yaw(const Real Angle, const Mezzanine::TransformSpace Space)
            { this->GraphicsBone->yaw( Ogre::Radian(Angle), ConvertTransformSpace(Space) ); }

        void Bone::Pitch(const Real Angle, const Mezzanine::TransformSpace Space)
            { this->GraphicsBone->pitch( Ogre::Radian(Angle), ConvertTransformSpace(Space) ); }

        void Bone::Roll(const Real Angle, const Mezzanine::TransformSpace Space)
            { this->GraphicsBone->roll( Ogre::Radian(Angle), ConvertTransformSpace(Space) ); }

        void Bone::Rotate(const Vector3& Axis, const Real Angle, const Mezzanine::TransformSpace Space)
            { this->GraphicsBone->rotate( Axis.GetOgreVector3(), Ogre::Radian(Angle), ConvertTransformSpace(Space) ); }

        void Bone::Rotate(const Quaternion& Rotation, const Mezzanine::TransformSpace Space)
            { this->GraphicsBone->rotate( Rotation.GetOgreQuaternion(), ConvertTransformSpace(Space) ); }

        void Bone::Scale(const Vector3& Scale)
            { this->GraphicsBone->scale( Scale.GetOgreVector3() ); }

        void Bone::Scale(const Real X, const Real Y, const Real Z)
            { this->GraphicsBone->scale(X,Y,Z); }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods

        Ogre::Bone* Bone::_GetInternalBone() const
            { return this->GraphicsBone; }
    }//Graphics
}//Mezzanine

#endif

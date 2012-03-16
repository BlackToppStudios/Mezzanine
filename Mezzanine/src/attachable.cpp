//© Copyright 2010 - 2011 BlackTopp Studios Inc.
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
#ifndef _attachable_cpp
#define _attachable_cpp

#include "attachable.h"
#include "worldnode.h"
#include "world.h"

#include <Ogre.h>

/// @file attachable.cpp
/// @brief Contains the Mezzanine::Attachable Class and Mezzanine::Attachable::AttachableElement enumeration implementations

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // AttachableBase Methods

    AttachableBase::AttachableBase()
        : Updating(false)
    {
    }

    AttachableBase::~AttachableBase()
    {
    }

    bool AttachableBase::GetUpdating(AttachableBase* AB) const
    {
        return (AB ? AB->Updating : false);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Conversion Functions

    Vector3 AttachableBase::ConvertLocalToGlobal(const Vector3& Location) const
    {
        return (this->GetOrientation() * Location * this->GetScaling()) + this->GetLocation();
    }

    Vector3 AttachableBase::ConvertGlobalToLocal(const Vector3& Location) const
    {
        return this->GetOrientation().GetInverse() * (Location - this->GetLocation()) / this->GetScaling();
    }

    Quaternion AttachableBase::ConvertLocalToGlobal(const Quaternion& Orientation) const
    {
        return this->GetOrientation() * Orientation;
    }

    Quaternion AttachableBase::ConvertGlobalToLocal(const Quaternion& Orientation) const
    {
        return this->GetOrientation().GetInverse() * Orientation;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    ///////////////////////////////////////////////////////////////////////////////
    // AttachableParent Methods

    AttachableParent::AttachableParent()
    {
    }

    AttachableParent::~AttachableParent()
    {
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Attachment child management

    void AttachableParent::AttachObject(AttachableChild* Target)
    {
        if((AttachableBase*)Target == (AttachableBase*)this) //don't be stoopid, can't attach to yourself
            return;
        if(Target->Parent)
        {
            if(Target->Parent == this) return;
            else World::GetWorldPointer()->LogAndThrow(Exception("Cannot attach object to multiple AttachableParent instances."));
        }

        Attached.push_back(Target);
        Target->Parent = this;

        Target->SetLocation( this->ConvertLocalToGlobal( Target->GetLocalLocation() ) );
        Target->SetOrientation( this->ConvertLocalToGlobal( Target->GetLocalOrientation() ) );
    }

    void AttachableParent::DetachObject(AttachableChild* Target)
    {
        for( AttachableIterator it = Attached.begin() ; it != Attached.end() ; ++it )
        {
            if(Target == (*it))
            {
                Attached.erase(it);
                return;
            }
        }
        Target->Parent = NULL;
    }

    void AttachableParent::DetachAll()
        { Attached.clear(); }

    Whole AttachableParent::GetNumAttached() const
        { return Attached.size(); }

    AttachableChild* AttachableParent::GetAttached(const Whole& Index) const
        { return Attached.at(Index); }

    AttachableParent::AttachableIterator AttachableParent::BeginChild()
        { return Attached.begin(); }

    AttachableParent::AttachableIterator AttachableParent::EndChild()
        { return Attached.end(); }

    AttachableParent::ConstAttachableIterator AttachableParent::BeginChild() const
        { return Attached.begin(); }

    AttachableParent::ConstAttachableIterator AttachableParent::EndChild() const
        { return Attached.end(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void AttachableParent::_RecalculateAllChildTransforms()
    {
        if( Attached.empty() )
            return;
        Updating = true;
        for( AttachableIterator it = Attached.begin() ; it != Attached.end() ; ++it )
        {
            (*it)->_RecalculateGlobalTransform(true);
        }
        Updating = false;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // AttachableChild Methods

    AttachableChild::AttachableChild()
        : Parent(NULL),
          LocalTransformDirty(false),
          GlobalTransformDirty(false)
    {
        LocalXform.SetIdentity();
    }

    AttachableChild::~AttachableChild()
    {
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility Functions

    AttachableParent* AttachableChild::GetParent() const
    {
        return Parent;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Transform Functions

    Vector3 AttachableChild::GetLocalLocation() const
    {
        return LocalXform.Location;
    }

    Quaternion AttachableChild::GetLocalOrientation() const
    {
        return LocalXform.Rotation;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Internal Methods

    void AttachableChild::_RecalculateGlobalTransform(bool FromParent)
    {
        // Set data for parent updates appropriately.
        // Can't assign directly in case "FromParent" is false and we need an update, such as when SetLocation() is invoked directly.
        if(FromParent)
            GlobalTransformDirty = true;

        if(Parent && GlobalTransformDirty)
        {
            this->SetOrientation( Parent->ConvertLocalToGlobal(LocalXform.Rotation) );
            this->SetLocation( Parent->ConvertLocalToGlobal(LocalXform.Location) );
        }
        GlobalTransformDirty = false;
    }

    void AttachableChild::_RecalculateLocalTransform()
    {
        if(Parent && LocalTransformDirty)
        {
            this->LocalXform.Location = Parent->ConvertGlobalToLocal( this->GetLocation() );
            this->LocalXform.Rotation = Parent->ConvertGlobalToLocal( this->GetOrientation() );
        }
        LocalTransformDirty = false;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Pure Virtual Functions

    // The coding that goes into pure virtual functions is not for inexperienced eyes such
    // as yours. Take your training to the Mountaintop monanstary and learn the ancient ways of
    // c++ from the Tibetan monks you find there.  Godspeed. //*/
}//Mezzanine

#endif

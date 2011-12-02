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
#include "transform.h"
#include "worldnode.h"
#include "world.h"

/// @file attachable.cpp
/// @brief Contains the Mezzanine::Attachable Class and Mezzanine::Attachable::AttachableElement enumeration implementations

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // Set data

    void Attachable::SetAttachedTo(Attachable* Targeter, Mezzanine::WorldNode* Target)
        { Targeter->AttachedTo = Target; }


    ///////////////////////////////////////////////////////////////////////////////
    // Construction

    Attachable::Attachable()
        { AttachedTo = 0; }

    Attachable::Attachable(Mezzanine::WorldNode* AttachedToWorldNode)
        { this->SetAttachedTo(this,AttachedToWorldNode); }

    Attachable::~Attachable()
        { }

    ///////////////////////////////////////////////////////////////////////////////
    // Attachment management

    WorldNode* Attachable::GetAttachedTo() const
        { return this->AttachedTo; }

    void Attachable::AttachTo(Mezzanine::WorldNode* Target)
    {
        if (AttachedTo)
            { World::GetWorldPointer()->LogAndThrow(Exception("Already Attached to another WorldNode.")); }
        Target->AttachObject(this);
        SetAttachedTo(this,Target);
    }

    void Attachable::DetachFrom()
    {
        AttachedTo->DetachObject(this);
        SetAttachedTo(this,0);
    }

    void Attachable::SetTransform(const Transform& Trans)
    {
        this->SetLocation(Trans.Location);
        this->SetOrientation(Trans.Rotation);
    }

    Transform Attachable::GetTransform() const
    {
        return Transform(this->GetLocation(),this->GetOrientation());
    }

    /// @todo consider adding rotate to Attachable
    /// @todo consider adding LookAt to Attachable

    ///////////////////////////////////////////////////////////////////////////////
    // Pure Virtual Functions

    // The coding that goes into pure virtual functions is not for inexperienced eyes such
    // as yours. Take your training to the Mountaintop monanstary and learn the ancient ways of
    // c++ from the Tibetan monks you find there.  Godspeed.
}//Mezzanine

#endif

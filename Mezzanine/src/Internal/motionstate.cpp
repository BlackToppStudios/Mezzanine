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
#ifndef _internalmotionstate_cpp
#define _internalmotionstate_cpp

#include "Internal/motionstate.h.cpp"
#include "Physics/rigidproxy.h"
#include "exception.h"

#include <algorithm>

// Keeps this file form being documented by doxygen
/// @cond 0

namespace Mezzanine
{
    namespace Internal
    {
        ///////////////////////////////////////////////////////////////////////////////
        // SimpleProxyMotionState methods

        SimpleProxyMotionState::SimpleProxyMotionState() :
            ParentObject(NULL),
            SyncObject(NULL)
            { this->WorldTrans.setIdentity(); }

        SimpleProxyMotionState::SimpleProxyMotionState(Physics::RigidProxy* PO, WorldProxy* WP) :
            ParentObject(PO),
            SyncObject(NULL)
            { this->WorldTrans.setIdentity();  this->SetSyncObject(WP); }

        SimpleProxyMotionState::~SimpleProxyMotionState()
            {  }

        void SimpleProxyMotionState::SetParentObject(Physics::RigidProxy* PO)
            { this->ParentObject = PO; }

        void SimpleProxyMotionState::SetSyncObject(WorldProxy* WP)
        {
            UInt32 ProxType = WP->GetProxyType();
            if( ProxType != Mezzanine::PT_Physics_RigidProxy || ProxType != Mezzanine::PT_Physics_SoftProxy ) {
                this->SyncObject = WP;
            }else{
                MEZZ_EXCEPTION(Exception::PARAMETERS_EXCEPTION,"Attempting to create motionstate sync object between two physics objects that do not support this configuration.");
            }
        }

        void SimpleProxyMotionState::SetPosition(const Vector3& Position)
            { this->WorldTrans.setOrigin( Position.GetBulletVector3() ); }

        void SimpleProxyMotionState::SetOrientation(const Quaternion& Orientation)
            { this->WorldTrans.setRotation(Orientation.GetBulletQuaternion()); }

        void SimpleProxyMotionState::getWorldTransform(btTransform& worldTrans) const
            { worldTrans = this->WorldTrans; }

        void SimpleProxyMotionState::setWorldTransform(const btTransform& worldTrans)
        {
            this->WorldTrans = worldTrans;

            if( this->ParentObject != NULL && this->SyncObject != NULL ) {
                this->SyncObject->SetLocation( Vector3( worldTrans.getOrigin() ) );
                this->SyncObject->SetOrientation( Quaternion( worldTrans.getRotation() ) );
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // MultiProxyMotionState methods

        MultiProxyMotionState::MultiProxyMotionState() :
            ParentObject(NULL)
            { this->WorldTrans.setIdentity(); }

        MultiProxyMotionState::MultiProxyMotionState(Physics::RigidProxy* PO) :
            ParentObject(PO)
            { this->WorldTrans.setIdentity(); }

        MultiProxyMotionState::~MultiProxyMotionState()
            { this->RemoveAllSyncProxies(); }

        void MultiProxyMotionState::SetParentObject(Physics::RigidProxy* PO)
            { this->ParentObject = PO; }

        void MultiProxyMotionState::AddSyncProxy(WorldProxy* ToBeAdded)
        {
            ProxyIterator ProxIt = std::find( this->SyncObjects.begin(), this->SyncObjects.end(), ToBeAdded );
            if( ProxIt == this->SyncObjects.end() )
                this->SyncObjects.push_back( ToBeAdded );
        }

        WorldProxy* MultiProxyMotionState::GetSyncProxy(const UInt32 Index) const
            { return this->SyncObjects.at(Index); }

        UInt32 MultiProxyMotionState::GetNumSyncProxies() const
            { return this->SyncObjects.size(); }

        void MultiProxyMotionState::RemoveSyncProxy(WorldProxy* ToBeRemoved)
        {
            ProxyIterator ProxIt = std::find( this->SyncObjects.begin(), this->SyncObjects.end(), ToBeRemoved );
            if( ProxIt != this->SyncObjects.end() )
                this->SyncObjects.erase( ProxIt );
        }

        void MultiProxyMotionState::RemoveAllSyncProxies()
            { this->SyncObjects.clear(); }

        void MultiProxyMotionState::SetPosition(const Vector3& Position)
            { this->WorldTrans.setOrigin( Position.GetBulletVector3() ); }

        void MultiProxyMotionState::SetOrientation(const Quaternion& Orientation)
            { this->WorldTrans.setRotation(Orientation.GetBulletQuaternion()); }

        void MultiProxyMotionState::getWorldTransform(btTransform& worldTrans) const
            { worldTrans = this->WorldTrans; }

        void MultiProxyMotionState::setWorldTransform(const btTransform& worldTrans)
        {
            this->WorldTrans = worldTrans;

            if( this->ParentObject != NULL ) {
                Vector3 TargetLocation( worldTrans.getOrigin() );
                Quaternion TargetOrientation( worldTrans.getRotation() );

                for( ProxyIterator ProxIt = this->SyncObjects.begin() ; ProxIt != this->SyncObjects.end() ; ++ProxIt )
                {
                    (*ProxIt)->SetLocation( TargetLocation );
                    (*ProxIt)->SetOrientation( TargetOrientation );
                }
            }
        }
    }//Internal
}//Mezzanine

/// @endcond

#endif

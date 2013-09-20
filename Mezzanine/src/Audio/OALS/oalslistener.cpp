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
// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio-ZLIBLicense.txt
#ifndef _audiolistener_cpp
#define _audiolistener_cpp

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#define OALS_STRUCTS_DECLARED

#include "Audio/OALS/oalslistener.h"
#include "Audio/OALS/oalssoundscapemanager.h"

#include "enumerations.h"
#include "exception.h"

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            Listener::Listener(ALCcontext* ListenContext, SoundScapeManager* Creator) :
                MPU(1.0),
                VolumeModifier(1.0),
                Manager(Creator),
                Context(ListenContext),
                InWorld(false)
            {
                this->MakeCurrent();
                alListenerf(AL_GAIN,0.0);
            }

            Listener::~Listener()
            {
            }

            void Listener::ConvertBuffer(Real* OrientationArray)
            {
                // Use the same defaults as the rest of the system
                Vector3 NewUp = Orientation * Vector3::Unit_Y();
                Vector3 NewForward = Orientation * Vector3::Neg_Unit_Z();
                OrientationArray[0] = NewUp.X;
                OrientationArray[1] = NewUp.Y;
                OrientationArray[2] = NewUp.Z;
                OrientationArray[3] = NewForward.X;
                OrientationArray[4] = NewForward.Y;
                OrientationArray[5] = NewForward.Z;
            }

            void Listener::MakeCurrent()
            {
                ALCcontext* CurrContext = alcGetCurrentContext();
                if( CurrContext != this->Context )
                    alcMakeContextCurrent(this->Context);
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            Mezzanine::ProxyType Listener::GetProxyType() const
            {
                return Mezzanine::PT_Audio_Listener;
            }

            void Listener::SetVelocity(const Vector3& Vel)
            {
                if( this->Velocity != Vel )
                {
                    this->MakeCurrent();
                    alListener3f(AL_VELOCITY,Vel.X,Vel.Y,Vel.Z);
                    this->Velocity = Vel;
                }
            }

            Vector3 Listener::GetVelocity() const
            {
                return this->Velocity;
            }

            void Listener::SetVolumeModifier(const Real Vol)
            {
                if( this->VolumeModifier != Vol )
                {
                    if( this->IsInWorld() ) {
                        this->MakeCurrent();
                        alListenerf(AL_GAIN,Vol);
                    }
                    this->VolumeModifier = Vol;
                }
            }

            Real Listener::GetVolumeModifier() const
            {
                return ( this->InWorld ? this->VolumeModifier : 0 );
            }

            void Listener::SetMetersPerUnit(const Real Meters)
            {
                if( this->MPU != Meters )
                {
                    this->MakeCurrent();
                    alListenerf(AL_METERS_PER_UNIT,Meters);
                    this->MPU = Meters;
                }
            }

            Real Listener::GetMetersPerUnit() const
            {
                return this->MPU;
            }

            void Listener::AddToWorld()
            {
                if( !this->IsInWorld() ) {
                    this->MakeCurrent();
                    alListenerf(AL_GAIN,this->VolumeModifier);
                    this->InWorld = true;
                }
            }

            void Listener::RemoveFromWorld()
            {
                if( this->IsInWorld() ) {
                    this->MakeCurrent();
                    alListenerf(AL_GAIN,0.0);
                    this->InWorld = false;
                }
            }

            bool Listener::IsInWorld() const
            {
                return this->InWorld;
            }

            WorldManager* Listener::GetCreator() const
            {
                return this->Manager;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Transform Methods

            void Listener::SetLocation(const Vector3& Loc)
            {
                if( this->Location != Loc )
                {
                    this->MakeCurrent();
                    alListener3f(AL_POSITION,Loc.X,Loc.Y,Loc.Z);
                    this->Location = Loc;
                }
            }

            void Listener::SetLocation(const Real X, const Real Y, const Real Z)
            {
                Vector3 TempVec(X,Y,Z);
                this->SetLocation(TempVec);
            }

            Vector3 Listener::GetLocation() const
            {
                return this->Location;
            }

            void Listener::SetOrientation(const Quaternion& Ori)
            {
                if( this->Orientation != Ori )
                {
                    this->MakeCurrent();
                    Real ALOrient[6];
                    this->ConvertBuffer(ALOrient);
                    alListenerfv(AL_ORIENTATION,ALOrient);
                    this->Orientation = Ori;
                }
            }

            void Listener::SetOrientation(const Real X, const Real Y, const Real Z, const Real W)
            {
                Quaternion TempQuat(X,Y,Z,W);
                this->SetOrientation(TempQuat);
            }

            Quaternion Listener::GetOrientation() const
            {
                return this->Orientation;
            }

            void Listener::SetScale(const Vector3& Sc)
            {
                // Currently you can't scale a listener
            }

            void Listener::SetScale(const Real X, const Real Y, const Real Z)
            {
                // Currently you can't scale a listener
            }

            Vector3 Listener::GetScale() const
            {
                // Currently you can't scale a listener
                return Vector3(1,1,1);
            }

            void Listener::Translate(const Vector3& Trans)
            {
                this->SetLocation( this->GetLocation() + Trans );
                this->SetVelocity(Trans);
            }

            void Listener::Translate(const Real X, const Real Y, const Real Z)
            {
                Vector3 Trans(X,Y,Z);
                this->SetLocation( this->GetLocation() + Trans );
                this->SetVelocity(Trans);
            }

            void Listener::Yaw(const Real Angle)
            {
                Quaternion NewRot = this->Orientation * Quaternion(Angle,Vector3::Unit_Y());
                this->SetOrientation(NewRot);
            }

            void Listener::Pitch(const Real Angle)
            {
                Quaternion NewRot = this->Orientation * Quaternion(Angle,Vector3::Unit_X());
                this->SetOrientation(NewRot);
            }

            void Listener::Roll(const Real Angle)
            {
                Quaternion NewRot = this->Orientation * Quaternion(Angle,Vector3::Unit_Z());
                this->SetOrientation(NewRot);
            }

            void Listener::Rotate(const Vector3& Axis, const Real Angle)
            {
                Quaternion NewRot = this->Orientation * Quaternion(Angle,Axis);
                this->SetOrientation(NewRot);
            }

            void Listener::Rotate(const Quaternion& Rotation)
            {
                Quaternion NewRot = this->Orientation * Rotation;
                this->SetOrientation(NewRot);
            }

            void Listener::Scale(const Vector3& Scale)
            {
                // Currently you can't scale a listener
            }

            void Listener::Scale(const Real X, const Real Y, const Real Z)
            {
                // Currently you can't scale a listener
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            void Listener::ProtoSerialize(XML::Node& ParentNode) const
            {
                /// @todo Implement this.
                MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"Serialization not yet implemented for Listeners.");
            }

            void Listener::ProtoDeSerialize(const XML::Node& SelfRoot)
            {
                /// @todo Implement this.
                MEZZ_EXCEPTION(Exception::NOT_IMPLEMENTED_EXCEPTION,"Serialization not yet implemented for Listeners.");
            }

            String Listener::GetDerivedSerializableName() const
            {
                return this->Listener::SerializableName();
            }

            String Listener::SerializableName()
            {
                return "OALSListener";
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            ALCcontext* Listener::_GetListenerContext() const
            {
                return this->Context;
            }
        }//OALS
    }//Audio
}//Mezzanine

#endif

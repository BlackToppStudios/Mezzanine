// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef _audiooalssoundproxy_cpp
#define _audiooalssoundproxy_cpp

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#define OALS_STRUCTS_DECLARED

#include "Audio/OALS/oalssoundproxy.h"
#include "Audio/OALS/oalssoundscapemanager.h"
#include "Audio/OALS/oalsfilter.h"
#include "Audio/OALS/oalseffect.h"
#include "Audio/OALS/oalsdefines.h"
#include "exception.h"
#include "serialization.h"

#include "Audio/audiomanager.h"
#include "Audio/decoderfactory.h"

#include <algorithm>
#include <cstring>
#include <memory>
#include <limits>

namespace
{
    /// @internal
    /// @brief Converts the BitConfig value to the internal type.
    /// @param Config The Mezzanine type to be converted.
    /// @return Returns the internal type corresponding to the provided Mezzanine type.
    ALenum ConvertBitConfigEnum(Mezzanine::Audio::BitConfig Config)
    {
        switch(Config)
		{
            case Mezzanine::Audio::BC_8Bit_Mono:     return AL_FORMAT_MONO8;      break;
            case Mezzanine::Audio::BC_16Bit_Mono:    return AL_FORMAT_MONO16;     break;
            case Mezzanine::Audio::BC_8Bit_Stereo:   return AL_FORMAT_STEREO8;    break;
            case Mezzanine::Audio::BC_16Bit_Stereo:  return AL_FORMAT_STEREO16;   break;
            default:                                 return AL_FORMAT_MONO8;      break;
		}
    }
}

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            /// @brief This enum describes a set of Boole options common for objects playing back audio in a 3D world.
            enum ProxyState
            {
                PS_Playing = 1,
                PS_Paused  = 2,
                PS_Stopped = 4,
                PS_Looping = 8,
                PS_InWorld = 16
            };//PlaybackState

            ///////////////////////////////////////////////////////////////////////////////
            // Sound Methods

            OALS::SoundProxy::SoundProxy(const UInt16 Type, const UInt32 ID, iDecoder* Decode, const ContextContainer& Contexts, SoundScapeManager* Creator) :
                Audio::SoundProxy(ID),
                Manager(Creator),
                SoundFilter(NULL),
                SoundDecoder(Decode),
                DopplerStrength(1.0),
                OuterConeAngle(360.0),
                OuterConeVolume(0.0),
                InnerConeAngle(360.0),
                MinDist(1.0),
                MaxDist(std::numeric_limits<Real>::max()),
                RolloffFactor(1.0),
                BaseVolume(1.0),
                MinVolume(0.0),
                MaxVolume(1.0),
                SoundPitch(1.0),
                SType(Type),
                State(OALS::PS_Stopped),
                DirectSound(false)
            {
                this->DopplerVelocity.Zero();
                this->Velocity.Zero();
                this->Location.Zero();
                this->Orientation.SetIdentity();

                // Allocate our space for contexts and create our sources
                this->ContextsAndSources.resize(OALS_MAX_LISTENERS_PER_MANAGER,ContextSourcePair(NULL,0));
                for( UInt32 Index = 0 ; Index < OALS_MAX_LISTENERS_PER_MANAGER ; ++Index )
                {
                    this->ContextsAndSources[Index].first = Contexts[Index];
                    if( this->ContextsAndSources[Index].first ) {
                        // Make the necessary context current
                        this->MakeCurrent( this->ContextsAndSources[Index].first );
                        // Create our source
                        alGenSources( 1, &(this->ContextsAndSources[Index].second) );
                        // Tell our source it'll be playing in 3D mode
                        alSourcei( this->ContextsAndSources[Index].second, AL_SOURCE_RELATIVE, false );
                    }
                }

                // Create the storage space for our effect slots
                Effects.resize(OALS_SOURCE_MAX_EFFECT_SLOTS,NULL);

                // Create and place our buffers
                alGenBuffers(OALS_SOURCE_NUM_BUFFERS,&Buffers[0]);
            }

            OALS::SoundProxy::~SoundProxy()
            {
                for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                {
                    if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                        this->MakeCurrent( (*CSI).first );
                        // Stop the source
                        alSourceStop((*CSI).second);
                        // Eradicate the source
                        alDeleteSources(1,&((*CSI).second));
                    }
                }

                // Eradicate the buffers
                alDeleteBuffers(OALS_SOURCE_NUM_BUFFERS,&Buffers[0]);
                // Clean out our decoder
                delete this->SoundDecoder;
            }

            void OALS::SoundProxy::ProtoSerializeImpl(XML::Node& SelfRoot) const
            {
                this->WorldProxy::ProtoSerializeImpl(SelfRoot);
            }

            void OALS::SoundProxy::ProtoDeSerializeImpl(const XML::Node& SelfRoot)
            {
                this->WorldProxy::ProtoDeSerializeImpl(SelfRoot);
            }

            void OALS::SoundProxy::MakeCurrent(ALCcontext* Context)
            {
                ALCcontext* CurrContext = alcGetCurrentContext();
                if( CurrContext != Context )
                    alcMakeContextCurrent(Context);
            }

            Boole OALS::SoundProxy::StreamToBuffer(const UInt32 Buffer)
            {
                if( this->SoundDecoder ) {
                    UInt32 TotalRead = 0;
                    UInt8 ErrorCount = 0;
                    Char8 TempBuffer[OALS_SOURCE_BUFFER_SIZE];
                    while( TotalRead < OALS_SOURCE_BUFFER_SIZE )
                    {
                        Char8 TempBuffer2[OALS_SOURCE_BUFFER_SIZE];
                        UInt32 ActualRead = this->SoundDecoder->ReadAudioData(TempBuffer2,OALS_SOURCE_BUFFER_SIZE - TotalRead);
                        if( ActualRead > 0 ) {
                            memcpy(TempBuffer+TotalRead,TempBuffer2,ActualRead);
                            TotalRead += ActualRead;
                        }else if(ActualRead < 0) {
                            ++ErrorCount;
                            if( ErrorCount >= 3 ) {
                                this->Stop();
                                return false;
                                //break;
                            }
                        }else if(ActualRead == 0 ) {
                            if(this->IsLooping()) this->SoundDecoder->SetPosition(0,false);
                            else break;
                        }
                    }

                    if( TotalRead == 0 ) {
                        return false;
                    }
                    alBufferData(Buffer,ConvertBitConfigEnum(this->SoundDecoder->GetBitConfiguration()),TempBuffer,TotalRead,this->SoundDecoder->GetFrequency());
                    return true;
                }
                return false;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            Mezzanine::ProxyType OALS::SoundProxy::GetProxyType() const
                { return Mezzanine::PT_Audio_SoundProxy; }

            Boole OALS::SoundProxy::IsValid() const
                { return ( this->SoundDecoder && this->SoundDecoder->GetStream() ); }//add parameters to check the sources

            UInt16 OALS::SoundProxy::GetType() const
                { return this->SType; }

            iDecoder* OALS::SoundProxy::GetDecoder() const
                { return this->SoundDecoder; }

            void OALS::SoundProxy::SetPitch(const Real Pitch)
            {
                if( this->SoundPitch != Pitch ) {
                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                            this->MakeCurrent( (*CSI).first );
                            alSourcef((*CSI).second,AL_PITCH,Pitch);
                        }
                    }
                    this->SoundPitch = Pitch;
                }
            }

            Real OALS::SoundProxy::GetPitch() const
            {
                return this->SoundPitch;
            }

            void OALS::SoundProxy::SetStream(Resource::DataStreamPtr Stream, const Audio::Encoding Encode)
            {
                iDecoderFactory* Factory = AudioManager::GetSingletonPtr()->GetDecoderFactory(Encode);
                if( Factory != NULL ) {
                    if( this->SoundDecoder ) {
                        delete this->SoundDecoder;
                        this->SoundDecoder = NULL;
                    }
                    this->SoundDecoder = Factory->CreateDecoder(Stream);
                }
            }

            void OALS::SoundProxy::SetStream(const UInt16 Type, Resource::DataStreamPtr Stream, const Audio::Encoding Encode)
            {
                this->SType = Type;
                this->SetStream(Stream,Encode);
            }

            void OALS::SoundProxy::SetStream(iDecoder* Decode)
            {
                if( this->SoundDecoder ) {
                    delete SoundDecoder;
                    SoundDecoder = NULL;
                }
                this->SoundDecoder = Decode;
            }

            void OALS::SoundProxy::SetStream(const UInt16 Type, iDecoder* Decode)
            {
                this->SType = Type;
                this->SetStream(Decode);
            }

            void OALS::SoundProxy::SetDirectSound(Boole Enable)
            {
                if( this->DirectSound != Enable ) {
                    // This is unfortuately hackish
                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                            this->MakeCurrent( (*CSI).first );

                            if( Enable ) {
                                Vector3 NewDirection = this->Orientation * Vector3::Neg_Unit_Z();
                                alSource3f((*CSI).second,AL_DIRECTION,NewDirection.X,NewDirection.Y,NewDirection.Z);
                            }else{
                                alSource3f((*CSI).second,AL_DIRECTION,0,0,0);
                            }
                        }
                    }

                    this->DirectSound = Enable;
                }
            }

            Boole OALS::SoundProxy::IsDirectingSound() const
            {
                return this->DirectSound;
            }

            void OALS::SoundProxy::AddToWorld()
            {
                if( !this->IsInWorld() ) {
                    this->State = ( this->State | OALS::PS_InWorld );
                    if( this->IsPaused() ) {
                        this->Play();
                    }
                }
            }

            void OALS::SoundProxy::RemoveFromWorld()
            {
                if( this->IsInWorld() ) {
                    this->State = ( this->State & ~OALS::PS_InWorld );
                    if( this->IsPlaying() ) {
                        this->Pause();
                    }
                }
            }

            Boole OALS::SoundProxy::IsInWorld() const
            {
                return (this->State & OALS::PS_InWorld);
            }

            WorldManager* OALS::SoundProxy::GetCreator() const
            {
                return this->Manager;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Playback

            Boole OALS::SoundProxy::Play()
            {
                if( !this->IsInWorld() )
                    return false;

                // Setup the buffers for playback
                if( !this->IsPlaying() ) {
                    if( !this->IsPaused() ) {
                        UInt32 QueueSize = 0;
                        /// @todo Currently in this method we iterate over all the sources for each context twice due to the order in which these
                        /// operations have to be done.  Purge the buffers before refreshing them and making them available to the sources, otherwise
                        /// audio artifacts may surface.  If this can be refactored more cleanly somehow, it should be done.
                        for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                        {
                            if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                                this->MakeCurrent( (*CSI).first );
                                alSourcei((*CSI).second,AL_BUFFER,0);
                            }
                        }
                        for( UInt32 BuffIndex = 0 ; BuffIndex < Buffers.size() ; ++BuffIndex )
                        {
                            if( this->StreamToBuffer(Buffers[BuffIndex]) ) ++QueueSize;
                            else return false;
                        }
                        for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                        {
                            if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                                this->MakeCurrent( (*CSI).first );
                                alSourceQueueBuffers((*CSI).second,QueueSize,&Buffers[0]);
                            }
                        }
                    }

                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                            this->MakeCurrent( (*CSI).first );
                            alSourcePlay((*CSI).second);
                        }
                    }
                    // Remove Stopped state if it exists
                    this->State = ( this->State & ~(OALS::PS_Stopped | OALS::PS_Paused) );
                    // Add the Playing state
                    this->State = ( this->State | OALS::PS_Playing );
                }
                return true;
            }

            Boole OALS::SoundProxy::IsPlaying() const
            {
                return (this->State & OALS::PS_Playing);
            }

            void OALS::SoundProxy::Pause()
            {
                if( this->IsPlaying() ) {
                    // Pause the source
                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                            this->MakeCurrent( (*CSI).first );
                            alSourcePause((*CSI).second);
                        }
                    }
                    // Remove Playing state if it exists
                    this->State = ( this->State & ~OALS::PS_Playing );
                    // Add the Paused state
                    this->State = ( this->State | OALS::PS_Paused );
                }
            }

            Boole OALS::SoundProxy::IsPaused() const
            {
                return (this->State & OALS::PS_Paused);
            }

            void OALS::SoundProxy::Stop()
            {
                if( !this->IsStopped() ) {
                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                            this->MakeCurrent( (*CSI).first );
                            // Stop the source
                            alSourceStop((*CSI).second);
                            // Unbind the buffers
                            Int32 BufferCount;
                            ALuint TempBuffer;
                            alGetSourcei((*CSI).second,AL_BUFFERS_PROCESSED,&BufferCount);
                            for( Int32 BuffIndex = 0 ; BuffIndex < BufferCount ; ++BuffIndex )
                                { alSourceUnqueueBuffers((*CSI).second,1,&TempBuffer); }
                        }
                    }
                    // Inform the decoder
                    this->SoundDecoder->SetPosition(0,false);
                    // Remove Playing state if it exists
                    this->State = ( this->State & ~(OALS::PS_Playing | OALS::PS_Paused) );
                    // Add the Stopped state
                    this->State = ( this->State | OALS::PS_Stopped );
                }
            }

            Boole OALS::SoundProxy::IsStopped() const
            {
                return (this->State & OALS::PS_Stopped);
            }

            void OALS::SoundProxy::Loop(Boole ToLoop)
            {
                if(ToLoop) this->State = ( this->State | OALS::PS_Looping );
                else this->State = ( this->State & ~OALS::PS_Looping );
            }

            Boole OALS::SoundProxy::IsLooping() const
            {
                return (this->State & OALS::PS_Looping);
            }

            Boole OALS::SoundProxy::Seek(const Real Seconds, Boole Relative)
            {
                Boole Ret = false;
                if( this->SoundDecoder->IsSeekingSupported() ) {
                    Ret = this->SoundDecoder->Seek(Seconds,Relative);
                }
                return Ret;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Volume Control

            Real OALS::SoundProxy::GetVolume() const
            {
                AudioManager* AudioMan = AudioManager::GetSingletonPtr();
                return ( AudioMan->IsMuted() ? 0 : this->BaseVolume * AudioMan->GetTypeVolume(this->SType) );
            }

            void OALS::SoundProxy::SetBaseVolume(const Real Base)
            {
                this->BaseVolume = Base;
            }

            Real OALS::SoundProxy::GetBaseVolume() const
            {
                return this->BaseVolume;
            }

            void OALS::SoundProxy::SetMinVolume(const Real MinVol)
            {
                if( this->MinVolume != MinVol ) {
                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                            this->MakeCurrent( (*CSI).first );
                            alSourcef((*CSI).second,AL_MIN_GAIN,MinVol);
                        }
                    }
                    this->MinVolume = MinVol;
                }
            }

            Real OALS::SoundProxy::GetMinVolume() const
            {
                return this->MinVolume;
            }

            void OALS::SoundProxy::SetMaxVolume(const Real MaxVol)
            {
                if( this->MaxVolume != MaxVol ) {
                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                            this->MakeCurrent( (*CSI).first );
                            alSourcef((*CSI).second,AL_MAX_GAIN,MaxVol);
                        }
                    }
                    this->MaxVolume = MaxVol;
                }
            }

            Real OALS::SoundProxy::GetMaxVolume() const
            {
                return this->MaxVolume;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // 3 Dimensional Configuration

            void OALS::SoundProxy::SetRolloffFactor(const Real& Rolloff)
            {
                if( this->RolloffFactor != Rolloff ) {
                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                            this->MakeCurrent( (*CSI).first );
                            alSourcef((*CSI).second,AL_ROLLOFF_FACTOR,Rolloff);
                        }
                    }
                    this->RolloffFactor = Rolloff;
                }
            }

            Real OALS::SoundProxy::GetRolloffFactor() const
            {
                return this->RolloffFactor;
            }

            void OALS::SoundProxy::SetMinDistance(const Real& MinDistance)
            {
                if( this->MinDist != MinDistance ) {
                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                            this->MakeCurrent( (*CSI).first );
                            alSourcef((*CSI).second,AL_REFERENCE_DISTANCE,MinDistance);
                        }
                    }
                    this->MinDist = MinDistance;
                }
            }

            Real OALS::SoundProxy::GetMinDistance() const
            {
                return this->MinDist;
            }

            void OALS::SoundProxy::SetMaxDistance(const Real& MaxDistance)
            {
                if( this->MaxDist != MaxDistance ) {
                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                            this->MakeCurrent( (*CSI).first );
                            alSourcef((*CSI).second,AL_MAX_DISTANCE,MaxDistance);
                        }
                    }
                    this->MaxDist = MaxDistance;
                }
            }

            Real OALS::SoundProxy::GetMaxDistance() const
            {
                return this->MaxDist;
            }

            void OALS::SoundProxy::SetInnerConeAngle(const Real& InnerAngle)
            {
                if( this->InnerConeAngle != InnerAngle ) {
                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                            this->MakeCurrent( (*CSI).first );
                            alSourcef((*CSI).second,AL_CONE_INNER_ANGLE,InnerAngle);
                        }
                    }
                    this->InnerConeAngle = InnerAngle;
                }
            }

            Real OALS::SoundProxy::GetInnerConeAngle() const
            {
                return this->InnerConeAngle;
            }

            void OALS::SoundProxy::SetOuterConeAngle(const Real& OuterAngle)
            {
                if( this->OuterConeAngle != OuterAngle ) {
                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                            this->MakeCurrent( (*CSI).first );
                            alSourcef((*CSI).second,AL_CONE_OUTER_ANGLE,OuterAngle);
                        }
                    }
                    this->OuterConeAngle = OuterAngle;
                }
            }

            Real OALS::SoundProxy::GetOuterConeAngle() const
            {
                return this->OuterConeAngle;
            }

            void OALS::SoundProxy::SetOuterConeVolume(const Real& OuterVolume)
            {
                if( this->OuterConeVolume != OuterVolume ) {
                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                            this->MakeCurrent( (*CSI).first );
                            alSourcef((*CSI).second,AL_CONE_OUTER_GAIN,OuterVolume);
                        }
                    }
                    this->OuterConeVolume = OuterVolume;
                }
            }

            Real OALS::SoundProxy::GetOuterConeVolume() const
            {
                return this->OuterConeVolume;
            }

            void OALS::SoundProxy::SetDopplerStrength(const Real& DopStr)
            {
                if( this->DopplerStrength != DopStr ) {
                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                            this->MakeCurrent( (*CSI).first );
                            alSourcef((*CSI).second,AL_DOPPLER_FACTOR,DopStr);
                        }
                    }
                    this->DopplerStrength = DopStr;
                }
            }

            Real OALS::SoundProxy::GetDopplerStrength() const
            {
                return this->DopplerStrength;
            }

            void OALS::SoundProxy::SetDopplerVelocity(const Vector3& DopVel)
            {
                if( this->DopplerVelocity != DopVel ) {
                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                            this->MakeCurrent( (*CSI).first );
                            alSource3f((*CSI).second,AL_DOPPLER_VELOCITY,DopVel.X,DopVel.Y,DopVel.Z);
                        }
                    }
                    this->DopplerVelocity = DopVel;
                }
            }

            Vector3 OALS::SoundProxy::GetDopplerVelocity() const
            {
                return this->DopplerVelocity;
            }

            void OALS::SoundProxy::SetVelocity(const Vector3& Vel)
            {
                if( this->Velocity != Vel ) {
                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                            this->MakeCurrent( (*CSI).first );
                            alSource3f((*CSI).second,AL_VELOCITY,Vel.X,Vel.Y,Vel.Z);
                        }
                    }
                    this->Velocity = Vel;
                }
            }

            Vector3 OALS::SoundProxy::GetVelocity() const
            {
                return this->Velocity;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Transform Methods

            void OALS::SoundProxy::SetLocation(const Vector3& Loc)
            {
                if( this->Location != Loc ) {
                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                            this->MakeCurrent( (*CSI).first );
                            alSource3f((*CSI).second,AL_POSITION,Loc.X,Loc.Y,Loc.Z);
                        }
                    }
                    this->Location = Loc;
                }
            }

            void OALS::SoundProxy::SetLocation(const Real X, const Real Y, const Real Z)
            {
                Vector3 TempVec(X,Y,Z);
                this->SetLocation(TempVec);
            }

            Vector3 OALS::SoundProxy::GetLocation() const
            {
                return this->Location;
            }

            void OALS::SoundProxy::SetOrientation(const Quaternion& Ori)
            {
                if( this->Orientation != Ori ) {
                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                            this->MakeCurrent( (*CSI).first );

                            if( this->DirectSound ) {
                                Vector3 NewDirection = Ori * Vector3::Neg_Unit_Z();
                                alSource3f((*CSI).second,AL_DIRECTION,NewDirection.X,NewDirection.Y,NewDirection.Z);
                            }else{
                                alSource3f((*CSI).second,AL_DIRECTION,0,0,0);
                            }
                        }
                    }
                    this->Orientation = Ori;
                }
            }

            void OALS::SoundProxy::SetOrientation(const Real X, const Real Y, const Real Z, const Real W)
            {
                Quaternion TempQuat(X,Y,Z,W);
                this->SetOrientation(TempQuat);
            }

            Quaternion OALS::SoundProxy::GetOrientation() const
            {
                return this->Orientation;
            }

            void OALS::SoundProxy::SetScale(const Vector3& Sc)
            {
                // Currently you can't scale a sound proxy
            }

            void OALS::SoundProxy::SetScale(const Real X, const Real Y, const Real Z)
            {
                // Currently you can't scale a sound proxy
            }

            Vector3 OALS::SoundProxy::GetScale() const
            {
                // Currently you can't scale a sound proxy
                return Vector3(1,1,1);
            }

            void OALS::SoundProxy::Translate(const Vector3& Trans)
            {
                this->SetLocation( this->GetLocation() + Trans );
                this->SetDopplerVelocity(Trans);
                this->SetVelocity(Trans);
            }

            void OALS::SoundProxy::Translate(const Real X, const Real Y, const Real Z)
            {
                Vector3 Trans(X,Y,Z);
                this->SetLocation( this->GetLocation() + Trans );
                this->SetDopplerVelocity(Trans);
                this->SetVelocity(Trans);
            }

            void OALS::SoundProxy::Yaw(const Real Angle)
            {
                Quaternion NewRot = this->Orientation * Quaternion(Angle,Vector3::Unit_Y());
                this->SetOrientation(NewRot);
            }

            void OALS::SoundProxy::Pitch(const Real Angle)
            {
                Quaternion NewRot = this->Orientation * Quaternion(Angle,Vector3::Unit_X());
                this->SetOrientation(NewRot);
            }

            void OALS::SoundProxy::Roll(const Real Angle)
            {
                Quaternion NewRot = this->Orientation * Quaternion(Angle,Vector3::Unit_Z());
                this->SetOrientation(NewRot);
            }

            void OALS::SoundProxy::Rotate(const Vector3& Axis, const Real Angle)
            {
                Quaternion NewRot = this->Orientation * Quaternion(Angle,Axis);
                this->SetOrientation(NewRot);
            }

            void OALS::SoundProxy::Rotate(const Quaternion& Rotation)
            {
                Quaternion NewRot = this->Orientation * Rotation;
                this->SetOrientation(NewRot);
            }

            void OALS::SoundProxy::Scale(const Vector3& Scale)
            {
                // Currently you can't scale a sound proxy
            }

            void OALS::SoundProxy::Scale(const Real X, const Real Y, const Real Z)
            {
                // Currently you can't scale a sound proxy
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Effects Methods

            Boole OALS::SoundProxy::AttachEffect(const UInt32 Slot, iEffect* Eff)
            {
                if( Eff && Eff->IsValid() == false )
                    return false;

                if( Slot < this->Effects.size() ) {
                    OALS::Effect* BeingAdded = static_cast<OALS::Effect*>(Eff);
                    if( BeingAdded ) {
                        this->Effects[Slot] = BeingAdded;

                        for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                        {
                            if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                                this->MakeCurrent( (*CSI).first );

                                ALuint EffectSlotID = BeingAdded->_GetInternalEffectSlot();
                                ALuint FilterID = ( BeingAdded->GetFilter() != NULL ? static_cast<OALS::Filter*>(BeingAdded->GetFilter())->_GetInternalFilter() : AL_FILTER_NULL );
                                alSource3i((*CSI).second,AL_AUXILIARY_SEND_FILTER,EffectSlotID,Slot,FilterID);
                            }
                        }
                        return true;
                    }
                }
                return false;
            }

            iEffect* OALS::SoundProxy::GetEffect(const UInt32 Slot) const
            {
                if( Slot < this->Effects.size() ) {
                    return Effects.at(Slot);
                }else{
                    return NULL;
                }
            }

            UInt32 OALS::SoundProxy::GetMaxEffectSlots() const
            {
                return this->Effects.size();
            }

            UInt32 OALS::SoundProxy::GetNumEffectSlotsAvailable() const
            {
                UInt32 Available = 0;
                for( ConstEffectIterator EffIt = this->Effects.begin() ; EffIt != this->Effects.end() ; ++EffIt )
                {
                    if( (*EffIt) != NULL )
                        ++Available;
                }
                return Available;
            }

            void OALS::SoundProxy::RemoveEffect(const UInt32 Slot)
            {
                if( Slot < this->Effects.size() ) {
                    this->Effects[Slot] = NULL;

                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                            this->MakeCurrent( (*CSI).first );
                            alSource3i((*CSI).second,AL_AUXILIARY_SEND_FILTER,AL_EFFECTSLOT_NULL,Slot,AL_FILTER_NULL);
                        }
                    }
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Filter Methods

            Boole OALS::SoundProxy::AttachFilter(iFilter* Fil)
            {
                if( Fil && Fil->IsValid() == false )
                    return false;

                this->SoundFilter = static_cast<OALS::Filter*>(Fil);
                if( this->SoundFilter != NULL ) {
                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                            this->MakeCurrent( (*CSI).first );

                            ALuint FilterID = this->SoundFilter->_GetInternalFilter();
                            alSourcei((*CSI).second,AL_DIRECT_FILTER,FilterID);
                        }
                    }
                }
                return true;
            }

            iFilter* OALS::SoundProxy::GetFilter() const
            {
                return this->SoundFilter;
            }

            void OALS::SoundProxy::RemoveFilter()
            {
                if( this->SoundFilter != NULL ) {
                    this->SoundFilter = NULL;

                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        if( (*CSI).first != NULL && (*CSI).second != 0 ) {
                            this->MakeCurrent( (*CSI).first );
                            alSourcei((*CSI).second,AL_DIRECT_FILTER,AL_FILTER_NULL);
                        }
                    }
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Serialization

            void OALS::SoundProxy::ProtoSerializeProperties(XML::Node& SelfRoot) const
            {
                this->WorldProxy::ProtoSerializeProperties(SelfRoot);

                XML::Node PropertiesNode = SelfRoot.AppendChild( OALS::SoundProxy::GetSerializableName() + "Properties" );

                if( PropertiesNode.AppendAttribute("Version").SetValue("1") &&
                    PropertiesNode.AppendAttribute("DopplerStrength").SetValue( this->GetDopplerStrength() ) &&
                    PropertiesNode.AppendAttribute("OuterConeAngle").SetValue( this->GetOuterConeAngle() ) &&
                    PropertiesNode.AppendAttribute("OuterConeVolume").SetValue( this->GetOuterConeVolume() ) &&
                    PropertiesNode.AppendAttribute("InnerConeAngle").SetValue( this->GetInnerConeAngle() ) &&
                    PropertiesNode.AppendAttribute("MinDist").SetValue( this->GetMinDistance() ) &&
                    PropertiesNode.AppendAttribute("MaxDist").SetValue( this->GetMaxDistance() ) &&
                    PropertiesNode.AppendAttribute("RolloffFactor").SetValue( this->GetRolloffFactor() ) &&
                    PropertiesNode.AppendAttribute("BaseVolume").SetValue( this->GetBaseVolume() ) &&
                    PropertiesNode.AppendAttribute("MinVolume").SetValue( this->GetMinVolume() ) &&
                    PropertiesNode.AppendAttribute("MaxVolume").SetValue( this->GetMaxVolume() ) &&
                    PropertiesNode.AppendAttribute("SoundPitch").SetValue( this->GetPitch() ) &&
                    PropertiesNode.AppendAttribute("SType").SetValue( this->GetType() ) &&
                    PropertiesNode.AppendAttribute("State").SetValue( this->State ) &&
                    PropertiesNode.AppendAttribute("DirectSound").SetValue( this->IsDirectingSound() ) )
                {
                    XML::Node VelocityNode = PropertiesNode.AppendChild("Velocity");
                    this->GetVelocity().ProtoSerialize( VelocityNode );
                    XML::Node DopplerVelocityNode = PropertiesNode.AppendChild("DopplerVelocity");
                    this->GetDopplerVelocity().ProtoSerialize( DopplerVelocityNode );

                    return;
                }else{
                    SerializeError("Create XML Attribute Values",OALS::SoundProxy::GetSerializableName() + "Properties",true);
                }
            }

            void OALS::SoundProxy::ProtoSerializeDecoder(XML::Node& SelfRoot) const
            {
                MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"Decoder Serialization not currently implemented.")
            }

            void OALS::SoundProxy::ProtoSerializeFilter(XML::Node& SelfRoot) const
            {
                MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"Filter Serialization not currently implemented.")
            }

            void OALS::SoundProxy::ProtoSerializeEffects(XML::Node& SelfRoot) const
            {
                MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"Effect Serialization not currently implemented.")
            }

            void OALS::SoundProxy::ProtoDeSerializeProperties(const XML::Node& SelfRoot)
            {
                this->WorldProxy::ProtoDeSerializeProperties(SelfRoot);

                XML::Attribute CurrAttrib;
                XML::Node PropertiesNode = SelfRoot.GetChild( OALS::SoundProxy::GetSerializableName() + "Properties" );

                if( !PropertiesNode.Empty() ) {
                    if(PropertiesNode.GetAttribute("Version").AsInt() == 1) {
                        CurrAttrib = PropertiesNode.GetAttribute("DopplerStrength");
                        if( !CurrAttrib.Empty() )
                            this->SetDopplerStrength( CurrAttrib.AsReal() );

                        CurrAttrib = PropertiesNode.GetAttribute("OuterConeAngle");
                        if( !CurrAttrib.Empty() )
                            this->SetOuterConeAngle( CurrAttrib.AsReal() );

                        CurrAttrib = PropertiesNode.GetAttribute("OuterConeVolume");
                        if( !CurrAttrib.Empty() )
                            this->SetOuterConeVolume( CurrAttrib.AsReal() );

                        CurrAttrib = PropertiesNode.GetAttribute("InnerConeAngle");
                        if( !CurrAttrib.Empty() )
                            this->SetInnerConeAngle( CurrAttrib.AsReal() );

                        CurrAttrib = PropertiesNode.GetAttribute("MinDist");
                        if( !CurrAttrib.Empty() )
                            this->SetMinDistance( CurrAttrib.AsReal() );

                        CurrAttrib = PropertiesNode.GetAttribute("MaxDist");
                        if( !CurrAttrib.Empty() )
                            this->SetMaxDistance( CurrAttrib.AsReal() );

                        CurrAttrib = PropertiesNode.GetAttribute("RolloffFactor");
                        if( !CurrAttrib.Empty() )
                            this->SetRolloffFactor( CurrAttrib.AsReal() );

                        CurrAttrib = PropertiesNode.GetAttribute("BaseVolume");
                        if( !CurrAttrib.Empty() )
                            this->SetBaseVolume( CurrAttrib.AsReal() );

                        CurrAttrib = PropertiesNode.GetAttribute("MinVolume");
                        if( !CurrAttrib.Empty() )
                            this->SetMinVolume( CurrAttrib.AsReal() );

                        CurrAttrib = PropertiesNode.GetAttribute("MaxVolume");
                        if( !CurrAttrib.Empty() )
                            this->SetMaxVolume( CurrAttrib.AsReal() );

                        CurrAttrib = PropertiesNode.GetAttribute("SoundPitch");
                        if( !CurrAttrib.Empty() )
                            this->SetPitch( CurrAttrib.AsReal() );

                        CurrAttrib = PropertiesNode.GetAttribute("SType");
                        if( !CurrAttrib.Empty() )
                            this->SType = CurrAttrib.AsUint();

                        CurrAttrib = PropertiesNode.GetAttribute("State");
                        if( !CurrAttrib.Empty() )
                            this->State = CurrAttrib.AsUint();

                        CurrAttrib = PropertiesNode.GetAttribute("DirectSound");
                        if( !CurrAttrib.Empty() )
                            this->SetDirectSound( CurrAttrib.AsBool() );

                        // Get the properties that need their own nodes
                        XML::Node VelocityNode = PropertiesNode.GetChild("Velocity").GetFirstChild();
                        if( !VelocityNode.Empty() ) {
                            Vector3 Vel(VelocityNode);
                            this->SetVelocity(Vel);
                        }

                        XML::Node DopplerVelocityNode = PropertiesNode.GetChild("DopplerVelocity").GetFirstChild();
                        if( !DopplerVelocityNode.Empty() ) {
                            Vector3 DopVel(DopplerVelocityNode);
                            this->SetDopplerVelocity(DopVel);
                        }
                    }else{
                        MEZZ_EXCEPTION(ExceptionBase::INVALID_VERSION_EXCEPTION,"Incompatible XML Version for " + (OALS::SoundProxy::GetSerializableName() + "Properties" ) + ": Not Version 1.");
                    }
                }else{
                    MEZZ_EXCEPTION(ExceptionBase::II_IDENTITY_NOT_FOUND_EXCEPTION,OALS::SoundProxy::GetSerializableName() + "Properties" + " was not found in the provided XML node, which was expected.");
                }
            }

            void OALS::SoundProxy::ProtoDeSerializeDecoder(const XML::Node& SelfRoot)
            {
                MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"Decoder Serialization not currently implemented.")
            }

            void OALS::SoundProxy::ProtoDeSerializeFilter(const XML::Node& SelfRoot)
            {
                MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"Filter Serialization not currently implemented.")
            }

            void OALS::SoundProxy::ProtoDeSerializeEffects(const XML::Node& SelfRoot)
            {
                MEZZ_EXCEPTION(ExceptionBase::NOT_IMPLEMENTED_EXCEPTION,"Effect Serialization not currently implemented.")
            }

            String OALS::SoundProxy::GetDerivedSerializableName() const
                { return this->OALS::SoundProxy::GetSerializableName(); }

            String OALS::SoundProxy::GetSerializableName()
                { return "OALSSoundProxy"; }

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            void OALS::SoundProxy::_Update()
            {
                for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                {
                    this->MakeCurrent( (*CSI).first );
                    // Update our volume
                    alSourcef( (*CSI).second, AL_GAIN, this->GetVolume() );

                    // Update our filter
                    if( this->SoundFilter != NULL && this->SoundFilter->_IsDirty() ) {
                        ALuint FilterID = this->SoundFilter->_GetInternalFilter();
                        alSourcei( (*CSI).second, AL_DIRECT_FILTER, FilterID );
                    }

                    // Update our effects
                    for( UInt32 Index = 0 ; Index < this->Effects.size() ; ++Index )
                    {
                        OALS::Effect* CurrEffect = this->Effects.at(Index);
                        if( CurrEffect != NULL && CurrEffect->_IsDirty() ) {
                            //ALuint EffectID = CurrEffect->_GetInternalEffect();
                            ALuint EffectSlotID = CurrEffect->_GetInternalEffectSlot();
                            ALuint FilterID = ( CurrEffect->GetFilter() != NULL ? static_cast<OALS::Filter*>(CurrEffect->GetFilter())->_GetInternalFilter() : AL_FILTER_NULL );
                            alSource3i( (*CSI).second, AL_AUXILIARY_SEND_FILTER, EffectSlotID, Index, FilterID);
                        }
                    }
                }

                // Update our streaming buffers
                this->_UpdateBuffers();
            }

            Boole OALS::SoundProxy::_UpdateBuffers()
            {
                Int32 Processed = 0;
                Boole Active = true;
                if( this->IsPlaying() ) {
                    // Get the minimum non-zero number of processed buffers across all sources
                    for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                    {
                        Int32 TempProcessed = 0;
                        this->MakeCurrent( (*CSI).first );
                        alGetSourcei( (*CSI).second, AL_BUFFERS_PROCESSED, &Processed );
                        if( TempProcessed > 0 )
                            Processed = std::min(Processed,TempProcessed);
                    }

                    while( Processed-- )
                    {
                        UInt32 Buffer = 0;
                        for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                        {
                            this->MakeCurrent( (*CSI).first );
                            alSourceUnqueueBuffers( (*CSI).second, 1, &Buffer );
                        }

                        Active = StreamToBuffer(Buffer);
                        if( Active ) {
                            for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                            {
                                this->MakeCurrent( (*CSI).first );
                                alSourceQueueBuffers( (*CSI).second, 1, &Buffer );
                            }
                        }
                    }
                }

                // This seems like the cleanest way to do this, but if one is stopped, stop all of them.
                ALenum OALSState;
                for( ContextSourceIterator CSI = this->ContextsAndSources.begin() ; CSI != this->ContextsAndSources.end() ; ++CSI )
                {
                    this->MakeCurrent( (*CSI).first );
                    alGetSourcei( (*CSI).second, AL_SOURCE_STATE, &OALSState );
                    if( OALSState == AL_STOPPED && !IsStopped() ) {
                        this->SoundDecoder->SetPosition(0,false);
                        this->State = ( this->IsLooping() ? OALS::PS_Stopped | OALS::PS_Looping : OALS::PS_Stopped );
                        break;
                    }
                }
                return Active;
            }

            void OALS::SoundProxy::_OnContextCreated(const UInt32 Index, ALCcontext* Context)
            {
                if( Index < this->ContextsAndSources.size() ) {
                    ALCcontext* OldContext = this->ContextsAndSources[Index].first;
                    //UInt32 SourceID = this->ContextsAndSources[Index].second;

                    if( OldContext != NULL )
                        { MEZZ_EXCEPTION(ExceptionBase::INVALID_STATE_EXCEPTION,"Attempting to write over an existing context."); }

                    this->MakeCurrent(Context);
                    this->ContextsAndSources[Index].first = Context;

                    alGenSources( 1, &(this->ContextsAndSources[Index].second) );
                    alSourcei( this->ContextsAndSources[Index].second, AL_SOURCE_RELATIVE, false );
                }
            }

            void OALS::SoundProxy::_OnContextDestroyed(const UInt32 Index, ALCcontext* Context)
            {
                if( Index < this->ContextsAndSources.size() ) {
                    ALCcontext* Context = this->ContextsAndSources[Index].first;
                    UInt32 SourceID = this->ContextsAndSources[Index].second;

                    if( Context != NULL && SourceID != 0 ) {
                        this->MakeCurrent(Context);
                        alSourceStop(SourceID);
                        alDeleteSources(1,&SourceID);

                        this->ContextsAndSources[Index].first = NULL;
                        this->ContextsAndSources[Index].second = 0;
                    }
                }
            }
        }//OALS
    }//Audio
}//Mezzanine

#endif

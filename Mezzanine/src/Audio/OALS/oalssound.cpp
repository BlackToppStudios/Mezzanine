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
#ifndef _audiooalssound_cpp
#define _audiooalssound_cpp

#include "Audio/OALS/oalssound.h"
#include "Audio/OALS/filter.h"
#include "Audio/OALS/effect.h"
#include "Audio/OALS/oalsdefines.h"
#include "exception.h"

#include "Audio/audiomanager.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

#include <algorithm>
#include <memory>

namespace
{
    /// @internal
    /// @brief Converts the BitConfig value to the internal type.
    /// @param Config The Mezzanine type to be converted.
    /// @return Returns the internal type corresponding to the provided Mezzanine type.
    ALenum ConvertBitConfigEnum(Mezzanine::Audio::BitConfig Config)
    {
        switch(format)
		{
            case Mezzanine::Audio::BC_8Bit_Mono:
                return AL_FORMAT_MONO8;
            case Mezzanine::Audio::BC_16Bit_Mono:
                return AL_FORMAT_MONO16;
            case Mezzanine::Audio::BC_8Bit_Stereo:
                return AL_FORMAT_STEREO8;
            case Mezzanine::Audio::BC_16Bit_Stereo:
                return AL_FORMAT_STEREO16;
            default:
                return AL_FORMAT_MONO8;
		}
    }
}

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            Sound::Sound(const UInt8 Type, const String& Name, iDecoder* Decode, ALCcontext* PlayContext)
                : Context(PlayContext),
                  SoundFilter(NULL),
                  SoundDecoder(Decode),
                  SType(Type),
                  State(iPlaybackable::PS_Stopped),
                  SoundPitch(1.0),
                  BaseVolume(1.0),
                  MinVolume(0.0),
                  MaxVolume(1.0),
                  InternalSource(0),
                  SoundName(Name)
            {
                // Set the context, so we can safely do all our business with the sources
                this->MakeCurrent();
                // Create the storage space for our buffer handles
                Buffers.resize(OALS_SOURCE_NUM_BUFFERS,0);
                // Create the storage space for our effect slots
                UInt32 SupportedSlots = 0;
                ALCdevice* Device = alcGetContextsDevice(Context);
                alcGetIntegerv(Device,ALC_MAX_AUXILIARY_SENDS,1,&SupportedSlots);
                Effects.resize(std::min(SupportedSlots,OALS_SOURCE_MAX_EFFECT_SLOTS),NULL);

                // Create and place our buffers
                alGenBuffers(OALS_SOURCE_NUM_BUFFERS,&Buffers[0]);
                // Create our source
                alGenSources(1,&InternalSource);
                // Tell our source it'll be playing in 2D mode
                alSourcei(InternalSource,AL_SOURCE_RELATIVE,true);
            }

            Sound::~Sound()
            {
                // Stop the source
                alSourceStop(InternalSource);
                // Eradicate the source
                alDeleteSources(1,&InternalSource);
                // Eradicate the buffers
                alDeleteBuffers(OALS_SOURCE_NUM_BUFFERS,&Buffers[0]);
            }

            void Sound::MakeCurrent()
            {
                ALCcontext* CurrContext = alcGetCurrentContext();
                if( CurrContext != this->Context )
                    alcMakeContextCurrent(this->Context);
            }

            bool Sound::StreamToBuffer(const UInt32 Buffer)
            {
                if(this->Decode)
                {
                    UInt32 TotalRead = 0;
                    UInt8 ErrorCount = 0;
                    Char8 TempBuffer[OALS_SOURCE_BUFFER_SIZE];
                    while( TotalRead < OALS_SOURCE_BUFFER_SIZE )
                    {
                        Char8 TempBuffer2[OALS_SOURCE_BUFFER_SIZE];
                        UInt32 ActualRead = this->Decode->ReadAudioData(TempBuffer2,OALS_SOURCE_BUFFER_SIZE - TotalRead);
                        if(ActualRead > 0)
                        {
                            memcpy(TempBuffer+TotalRead,TempBuffer2,ActualRead);
                            TotalRead += ActualRead;
                        }
                        else if(ActualRead < 0)
                        {
                            ++ErrorCount;
                            if(ErrorCount >= 3)
                            {
                                this->Stop();
                                break;
                            }
                        }
                        else if(ActualRead == 0)
                        {
                            if(this->IsLooping()) this->Decode->SetPosition(0,false);
                            else break;
                        }
                    }

                    if(TotalRead == 0)
                    {
                        return false;
                    }
                    alBufferData(Buffer,ConvertBitConfigEnum(this->Decode->GetBitConfiguration()),TempBuffer,TotalRead,this->Decode->GetFrequency());
                    return true;
                }
                return false;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            const String& Sound::GetName() const
            {
                return this->SoundName;
            }

            UInt16 Sound::GetType() const
            {
                return this->SType;
            }

            iDecoder* Sound::GetDecoder() const
            {
                return this->SoundDecoder
            }

            void Sound::SetPitch(const Real Pitch)
            {
                if( this->SoundPitch != Pitch )
                {
                    this->MakeCurrent();
                    alSourcef(this->InternalSource,AL_PITCH,Pitch);
                    this->SoundPitch = Pitch;
                }
            }

            Real Sound::GetPitch() const
            {
                return this->SoundPitch;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Playback

            bool Sound::Play()
            {
                if( !IsPaused() )
                {
                    UInt32 QueueSize = 0;
                    alSourcei(InternalSource,AL_BUFFER,0);
                    for( UInt32 BuffIndex = 0 ; BuffIndex < Buffers.size() ; ++BuffIndex )
                    {
                        if( this->StreamToBuffer(Buffers[BuffIndex]) ) ++QueueSize;
                        else return false;
                    }
                    alSourceQueueBuffers(this->InternalSource,QueueSize,&Buffers[0]);
                }

                alSourcePlay(this->InternalSource);
                this->State = ( this->IsLooping() ? iPlaybackable::PS_Playing | iPlaybackable::PS_Looping : iPlaybackable::PS_Playing );
                return true;
            }

            bool Sound::IsPlaying() const
            {
                return (this->State & iPlaybackable::PS_Playing);
            }

            void Sound::Pause()
            {
                // Pause the source
                alSourcePause(this->InternalSource);
                this->State = ( this->IsLooping() ? iPlaybackable::PS_Paused | iPlaybackable::PS_Looping : iPlaybackable::PS_Paused );
            }

            bool Sound::IsPaused() const
            {
                return (this->State & iPlaybackable::PS_Paused);
            }

            void Sound::Stop()
            {
                // Stop the source
                alSourceStop(this->InternalSource);
                // Inform the decoder
                this->Decoder->SetPosition(0,false);
                this->State = ( this->IsLooping() ? iPlaybackable::PS_Stopped | iPlaybackable::PS_Looping : iPlaybackable::PS_Stopped );
            }

            bool Sound::IsStopped() const
            {
                return (this->State & iPlaybackable::PS_Stopped);
            }

            void Sound::Loop(bool ToLoop)
            {
                if(ToLoop) this->State = ( this->State | iPlaybackable::PS_Looping );
                else this->State = ( this->State & ~iPlaybackable::PS_Looping );
            }

            bool Sound::IsLooping() const
            {
                return (this->State & iPlaybackable::PS_Looping);
            }

            bool Sound::Seek(const Real Seconds, bool Relative)
            {
                bool Ret = false;
                if( this->Decode->IsSeekingSupported() )
                {
                    Ret = this->Decode->Seek(Seconds,Relative);
                }
                return Ret;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Volume Control

            Real Sound::GetVolume() const
            {
                return this->BaseVolume * AudioManager::GetSingletonPtr()->GetTypeVolume(this->SType);
            }

            void Sound::SetBaseVolume(const Real Base)
            {
                this->BaseVolume = Base;
            }

            Real Sound::GetBaseVolume() const
            {
                return this->BaseVolume;
            }

            void Sound::SetMinVolume(const Real MinVol)
            {
                if( this->MinVolume != MinVol )
                {
                    this->MakeCurrent();
                    alSourcef(this->InternalSource,AL_MIN_GAIN,MinVol);
                    this->MinVolume = MinVol;
                }
            }

            Real Sound::GetMinVolume() const
            {
                return this->MinVolume;
            }

            void Sound::SetMaxVolume(const Real MaxVol)
            {
                if( this->MaxVolume != MaxVol )
                {
                    this->MakeCurrent();
                    alSourcef(this->InternalSource,AL_MAX_GAIN,MaxVol);
                    this->MaxVolume = MaxVol;
                }
            }

            Real Sound::GetMaxVolume() const
            {
                return this->MaxVolume;
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Effects Methods

            bool Sound::AttachEffect(const UInt32 Slot, iEffect* Eff)
            {
                if( Eff && Eff->IsValid() == false )
                    return false;

                if( Slot < this->Effects.size() )
                {
                    OALS::Effect* BeingAdded = static_cast<OALS::Effect*>(Eff);
                    if( BeingAdded )
                    {
                        this->Effects[Slot] = BeingAdded;

                        ALuint EffectSlotID = BeingAdded->_GetInternalEffectSlot();
                        ALuint FilterID = ( BeingAdded->GetFilter() != NULL ? BeingAdded->GetFilter()->_GetInternalFilter() : AL_FILTER_NULL );
                        alSource3i(this->InternalSource,AL_AUXILIARY_SEND_FILTER,EffectSlotID,Index,FilterID);
                    }
                }
                return false;
            }

            iEffect* GetEffect(const UInt32 Slot) const
            {
                if( Slot < this->Effects.size() ) {
                    return Effects.at(Slot);
                }else{
                    return NULL;
                }
            }

            UInt32 Sound::GetNumEffectSlotsAvailable() const
            {
                return this->Effects.size();
            }

            void Sound::RemoveEffect(const UInt32 Slot)
            {
                if( Slot < this->Effects.size() )
                {
                    this->Effects[Slot] = NULL;
                    alSource3i(this->InternalSource,AL_AUXILIARY_SEND_FILTER,AL_EFFECTSLOT_NULL,Slot,AL_FILTER_NULL);
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Filter Methods

            bool Sound::AttachFilter(iFilter* Fil)
            {
                if( Fil && Fil->IsValid() == false )
                    return false;

                this->SoundFilter = static_cast<OALS::Filter*>(Fil);
                if( this->SoundFilter != NULL )
                {
                    ALuint FilterID = this->SoundFilter->_GetInternalFilter();
                    alSourcei(this->InternalSource,AL_DIRECT_FILTER,FilterID);
                }
                return true;
            }

            iFilter* Sound::GetFilter() const
            {
                return this->SoundFilter;
            }

            void Sound::RemoveFilter()
            {
                if( this->SoundFilter != NULL )
                {
                    this->SoundFilter = NULL;
                    alSourcei(this->InternalSource,AL_DIRECT_FILTER,AL_FILTER_NULL);
                }
            }

            ///////////////////////////////////////////////////////////////////////////////
            // Internal Methods

            void Sound::_Update()
            {
                this->MakeCurrent();
                // Update our volume
                alSourcef(this->InternalSource,AL_GAIN,this->GetVolume());

                // Update our filter
                if( this->SoundFilter != NULL && this->SoundFilter->_IsDirty() )
                {
                    ALuint FilterID = this->SoundFilter->_GetInternalFilter();
                    alSourcei(this->InternalSource,AL_DIRECT_FILTER,FilterID);
                }

                // Update our effects
                for( UInt32 Index = 0 ; Index < this->Effects.size() ; ++Index )
                {
                    OALS::Effect* CurrEffect = this->Effects.at(Index);
                    if( CurrEffect != NULL && CurrEffect->_IsDirty() )
                    {
                        //ALuint EffectID = CurrEffect->_GetInternalEffect();
                        ALuint EffectSlotID = CurrEffect->_GetInternalEffectSlot();
                        ALuint FilterID = ( CurrEffect->GetFilter() != NULL ? CurrEffect->GetFilter()->_GetInternalFilter() : AL_FILTER_NULL );
                        alSource3i(this->InternalSource,AL_AUXILIARY_SEND_FILTER,EffectSlotID,Index,FilterID);
                    }
                }

                // Update our streaming buffers
                this->_UpdateBuffers();
            }

            bool Sound::_UpdateBuffers()
            {
                UInt32 Processed = 0;
                bool Active = true;
                if( this->IsPlaying() )
                {
                    alGetSourcei(this->InternalSource,AL_BUFFERS_PROCESSED,&Processed);

                    while( Processed-- )
                    {
                        UInt32 Buffer = 0;
                        alSourceUnqueueBuffers(this->InternalSource,1,&Buffer);
                        Active = StreamToBuffer(Buffer);

                        if( Active )
                            alSourceQueueBuffers(this->InternalSource,1,&Buffer);
                    }
                }

                ALenum OALSState;
                alGetSourcei(this->InternalSource,AL_SOURCE_STATE,&OALSState);
                if( OALSState == AL_STOPPED && !IsStopped() )
                {
                    this->Decode->SetPosition(0,false);
                }
                return Active;
            }
        }//OALS
    }//Audio
}//Mezzanine

#endif

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
#ifndef _audioaudiomanager_cpp
#define _audioaudiomanager_cpp

#include "Audio/audiomanager.h"
#include "Audio/recorder.h"
#include "Audio/sound.h"
#include "Audio/effectshandler.h"
#include "Audio/soundscapemanager.h"
#include "Audio/decoder.h"
#include "Audio/decoderfactory.h"
#include "exception.h"

#include "Audio/rawdecoderfactory.h"
#include "Audio/vorbisdecoderfactory.h"
#include "Audio/wavdecoderfactory.h"

namespace Mezzanine
{
    namespace Audio
    {
        template<> AudioManager* Singleton<AudioManager>::SingletonPtr = 0;

        AudioManager::AudioManager()
        {
#ifdef ENABLE_RAW_ENCODE
            this->AddDecoderFactory(new RawDecoderFactory());
#endif
#ifdef ENABLE_VORBIS_ENCODE
            this->AddDecoderFactory(new VorbisDecoderFactory());
#endif
#ifdef ENABLE_WAV_ENCODE
            this->AddDecoderFactory(new WavDecoderFactory());
#endif
        }

        AudioManager::AudioManager(XML::Node& XMLNode)
        {
        }

        AudioManager::~AudioManager()
            { this->DestroyAllDecoderFactories(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        ///////////////////////////////////////////////////////////////////////////////
        // Sound Management

        ///////////////////////////////////////////////////////////////////////////////
        // Convenience Creation Methods

        iSound* AudioManager::CreateAmbientSound()
            { return this->CreateSound(Audio::ST_Ambient); }
        iSound* AudioManager::CreateDialogSound()
            { return this->CreateSound(Audio::ST_Dialog); }
        iSound* AudioManager::CreateEffectSound()
            { return this->CreateSound(Audio::ST_Effect); }
        iSound* AudioManager::CreateMusicSound()
            { return this->CreateSound(Audio::ST_Music); }

        iSound* AudioManager::CreateAmbientSound(Resource::DataStreamPtr Stream, const Audio::Encoding Encode)
            { return this->CreateSound(Audio::ST_Ambient,Stream,Encode); }
        iSound* AudioManager::CreateDialogSound(Resource::DataStreamPtr Stream, const Audio::Encoding Encode)
            { return this->CreateSound(Audio::ST_Dialog,Stream,Encode); }
        iSound* AudioManager::CreateEffectSound(Resource::DataStreamPtr Stream, const Audio::Encoding Encode)
            { return this->CreateSound(Audio::ST_Effect,Stream,Encode); }
        iSound* AudioManager::CreateMusicSound(Resource::DataStreamPtr Stream, const Audio::Encoding Encode)
            { return this->CreateSound(Audio::ST_Music,Stream,Encode); }

        iSound* AudioManager::CreateAmbientSound(Resource::DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config)
            { return this->CreateSound(Audio::ST_Ambient,Stream,Frequency,Config); }
        iSound* AudioManager::CreateDialogSound(Resource::DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config)
            { return this->CreateSound(Audio::ST_Dialog,Stream,Frequency,Config); }
        iSound* AudioManager::CreateEffectSound(Resource::DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config)
            { return this->CreateSound(Audio::ST_Effect,Stream,Frequency,Config); }
        iSound* AudioManager::CreateMusicSound(Resource::DataStreamPtr Stream, const UInt32 Frequency, const Audio::BitConfig Config)
            { return this->CreateSound(Audio::ST_Music,Stream,Frequency,Config); }

        iSound* AudioManager::CreateAmbientSound(const String& FileName, const String& Group)
            { return this->CreateSound(Audio::ST_Ambient,FileName,Group); }
        iSound* AudioManager::CreateDialogSound(const String& FileName, const String& Group)
            { return this->CreateSound(Audio::ST_Dialog,FileName,Group); }
        iSound* AudioManager::CreateEffectSound(const String& FileName, const String& Group)
            { return this->CreateSound(Audio::ST_Effect,FileName,Group); }
        iSound* AudioManager::CreateMusicSound(const String& FileName, const String& Group)
            { return this->CreateSound(Audio::ST_Music,FileName,Group); }

        iSound* AudioManager::CreateAmbientSound(const String& StreamName, Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode)
            { return this->CreateSound(Audio::ST_Ambient,StreamName,Buffer,Length,Encode); }
        iSound* AudioManager::CreateDialogSound(const String& StreamName, Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode)
            { return this->CreateSound(Audio::ST_Dialog,StreamName,Buffer,Length,Encode); }
        iSound* AudioManager::CreateEffectSound(const String& StreamName, Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode)
            { return this->CreateSound(Audio::ST_Effect,StreamName,Buffer,Length,Encode); }
        iSound* AudioManager::CreateMusicSound(const String& StreamName, Char8* Buffer, const UInt32 Length, const Audio::Encoding Encode)
            { return this->CreateSound(Audio::ST_Music,StreamName,Buffer,Length,Encode); }

        iSound* AudioManager::CreateAmbientSound(const String& StreamName, Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config)
            { return this->CreateSound(Audio::ST_Ambient,StreamName,Buffer,Length,Frequency,Config); }
        iSound* AudioManager::CreateDialogSound(const String& StreamName, Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config)
            { return this->CreateSound(Audio::ST_Dialog,StreamName,Buffer,Length,Frequency,Config); }
        iSound* AudioManager::CreateEffectSound(const String& StreamName, Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config)
            { return this->CreateSound(Audio::ST_Effect,StreamName,Buffer,Length,Frequency,Config); }
        iSound* AudioManager::CreateMusicSound(const String& StreamName, Char8* Buffer, const UInt32 Length, const UInt32 Frequency, const Audio::BitConfig Config)
            { return this->CreateSound(Audio::ST_Music,StreamName,Buffer,Length,Frequency,Config); }

        ///////////////////////////////////////////////////////////////////////////////
        // Volume Management

        ///////////////////////////////////////////////////////////////////////////////
        // Convenience Type Volume Methods

        void AudioManager::SetAmbientVolume(const Real& Ambient)
            { this->SetTypeVolume(Audio::ST_Ambient,Ambient); }
        Real AudioManager::GetAmbientVolume() const
            { return this->GetTypeVolume(Audio::ST_Ambient); }
        void AudioManager::MuteAmbient(bool Enable)
            { this->MuteType(Audio::ST_Ambient,Enable); }
        bool AudioManager::IsAmbientMuted() const
            { return this->IsTypeMuted(Audio::ST_Ambient); }

        void AudioManager::SetDialogVolume(const Real& Dialog)
            { this->SetTypeVolume(Audio::ST_Dialog,Dialog); }
        Real AudioManager::GetDialogVolume() const
            { return this->GetTypeVolume(Audio::ST_Dialog); }
        void AudioManager::MuteDialog(bool Enable)
            { this->MuteType(Audio::ST_Dialog,Enable); }
        bool AudioManager::IsDialogMuted() const
            { return this->IsTypeMuted(Audio::ST_Dialog); }

        void AudioManager::SetEffectVolume(const Real& Effect)
            { this->SetTypeVolume(Audio::ST_Effect,Effect); }
        Real AudioManager::GetEffectVolume() const
            { return this->GetTypeVolume(Audio::ST_Effect); }
        void AudioManager::MuteEffect(bool Enable)
            { this->MuteType(Audio::ST_Effect,Enable); }
        bool AudioManager::IsEffectMuted() const
            { return this->IsTypeMuted(Audio::ST_Effect); }

        void AudioManager::SetMusicVolume(const Real& Music)
            { this->SetTypeVolume(Audio::ST_Music,Music); }
        Real AudioManager::GetMusicVolume() const
            { return this->GetTypeVolume(Audio::ST_Music); }
        void AudioManager::MuteMusic(bool Enable)
            { this->MuteType(Audio::ST_Music,Enable); }
        bool AudioManager::IsMusicMuted() const
            { return this->IsTypeMuted(Audio::ST_Music); }

        ///////////////////////////////////////////////////////////////////////////////
        // Playback Device Management

        ///////////////////////////////////////////////////////////////////////////////
        // Recording Device Management

        ///////////////////////////////////////////////////////////////////////////////
        // Decoder Management

        void AudioManager::AddDecoderFactory(iDecoderFactory* ToBeAdded)
        {
            this->DecoderFactories.insert(std::pair<Audio::Encoding,iDecoderFactory*>(ToBeAdded->GetSupportedEncoding(),ToBeAdded));
            //std::pair<DecoderFactoryIterator,bool> Result = this->DecoderFactories.insert(std::pair<Audio::Encoding,iDecoderFactory*>(ToBeAdded->GetSupportedEncoding(),ToBeAdded));
            //return Result.first;
        }

        bool AudioManager::DecoderFactoryExists(const Audio::Encoding Encode)
        {
            return this->DecoderFactories.count(Encode);
        }

        iDecoderFactory* AudioManager::GetDecoderFactory(const Audio::Encoding Encode)
        {
            DecoderFactoryIterator FactIt = this->DecoderFactories.find(Encode);
            if( FactIt != this->DecoderFactories.end() ) return (*FactIt).second;
            else return NULL;
        }

        void AudioManager::RemoveDecoderFactory(iDecoderFactory* ToBeRemoved)
        {
            this->RemoveDecoderFactory(ToBeRemoved->GetSupportedEncoding());
        }

        void AudioManager::RemoveDecoderFactory(const Audio::Encoding Encode)
        {
            this->DecoderFactories.erase(Encode);
        }

        void AudioManager::RemoveAllDecoderFactories()
        {
            this->DecoderFactories.clear();
        }

        void AudioManager::DestroyDecoderFactory(iDecoderFactory* ToBeDestroyed)
        {
            this->DestroyDecoderFactory(ToBeDestroyed->GetSupportedEncoding());
        }

        void AudioManager::DestroyDecoderFactory(const Audio::Encoding Encode)
        {
            DecoderFactoryIterator FactIt = this->DecoderFactories.find(Encode);
            if( FactIt != this->DecoderFactories.end() )
            {
                delete (*FactIt).second;
                this->DecoderFactories.erase(FactIt);
            }
        }

        void AudioManager::DestroyAllDecoderFactories()
        {
            for( DecoderFactoryIterator FactIt = this->DecoderFactories.begin() ; FactIt != this->DecoderFactories.end() ; ++FactIt )
            {
                delete (*FactIt).second;
            }
            this->DecoderFactories.clear();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Inherited from Managerbase

        ManagerBase::ManagerType AudioManager::GetInterfaceType() const
            { return ManagerBase::AudioManager; }

        ///////////////////////////////////////////////////////////////////////////////
        // Internal Methods
    }//Audio
}//Mezzanine

#endif

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
#ifndef _audiomanager_cpp
#define _audiomanager_cpp

#include "audiomanager.h"
#include "audiosound.h"
#include "audiolistener.h"
#include "audiosoundset.h"
#include "world.h"
#include <cAudio.h>

namespace phys
{
    AudioManager::AudioManager(bool DefaultSettings)
        : AmbientVolume(1.0),
          DialogVolume(1.0),
          EffectVolume(1.0),
          MusicVolume(1.0),
          MasterVolume(1.0)
    {
        cAudioManager = cAudio::createAudioManager(DefaultSettings);
        Listener = new Audio::Listener(cAudioManager->getListener());
        this->Priority = 10;
    }

    AudioManager::~AudioManager()
    {
        #ifdef PHYSDEBUG
        this->GameWorld->Log("Destructing Sound Manager");
        #endif
        DestroyAllSounds();
        cAudio::destroyAudioManager(cAudioManager);
    }

    Audio::Sound* AudioManager::CreateAmbientSound(ConstString& SoundName, ConstString& FileName, ConstString& Group)
    {
        Audio::Sound* pSound = new Audio::Sound(SoundName,FileName,Group,Audio::ST_Ambient);
        Sounds.insert(std::pair<String,Audio::Sound*>(SoundName,pSound));
        AmbientSounds.push_back(pSound);
        return pSound;
    }

    Audio::Sound* AudioManager::CreateDialogSound(ConstString& SoundName, ConstString& FileName, ConstString& Group)
    {
        Audio::Sound* pSound = new Audio::Sound(SoundName,FileName,Group,Audio::ST_Dialog);
        Sounds.insert(std::pair<String,Audio::Sound*>(SoundName,pSound));
        DialogSounds.push_back(pSound);
        return pSound;
    }

    Audio::Sound* AudioManager::CreateEffectSound(ConstString& SoundName, ConstString& FileName, ConstString& Group)
    {
        Audio::Sound* pSound = new Audio::Sound(SoundName,FileName,Group,Audio::ST_Effect);
        Sounds.insert(std::pair<String,Audio::Sound*>(SoundName,pSound));
        EffectSounds.push_back(pSound);
        return pSound;
    }

    Audio::Sound* AudioManager::CreateMusicSound(ConstString& SoundName, ConstString& FileName, ConstString& Group)
    {
        Audio::Sound* pSound = new Audio::Sound(SoundName,FileName,Group,Audio::ST_Music);
        Sounds.insert(std::pair<String,Audio::Sound*>(SoundName,pSound));
        MusicSounds.push_back(pSound);
        return pSound;
    }

    Audio::Sound* AudioManager::GetSoundByName(const String& SoundName) const
    {
        cAudio::IAudioSource* Source = cAudioManager->getSoundByName(SoundName.c_str());
        if(Source)
        {
            Audio::Sound* SoundInst = new Audio::Sound(Source);
            return SoundInst;
        }
        return NULL;
    }

    void AudioManager::DestroySound(Audio::Sound* SoundName)
    {
        cAudioManager->release(SoundName->SoundSource);
    }

    void AudioManager::DestroyAllSounds()
    {
        cAudioManager->releaseAllSources();
    }

    Audio::SoundSet* AudioManager::CreateSoundSet(const String& SoundSetName)
    {
        Audio::SoundSet* Set = new Audio::SoundSet();
        this->SoundSets[SoundSetName] = Set;
        return Set;
    }

    Audio::SoundSet* AudioManager::GetSoundSet(const String& SoundSetName) const
    {
        std::map< String, Audio::SoundSet* >::const_iterator SS = this->SoundSets.find(SoundSetName);
        if(SS != SoundSets.end()) return (*SS).second;
        else return NULL;
    }

    void AudioManager::AddSoundToSoundSet(const String& SoundSetName, Audio::Sound* SoundInst)
    {
        Audio::SoundSet* Set = this->SoundSets[SoundSetName];
        Set->push_back(SoundInst);
    }

    void AudioManager::SetAmbientVolume(const Real& Ambient)
    {
        AmbientVolume = Ambient;
        for( std::vector<Audio::Sound*>::iterator it = AmbientSounds.begin() ; it != AmbientSounds.end() ; it++ )
        {
            (*it)->UpdateVolume();
        }
    }

    Real AudioManager::GetAmbientVolume() const
    {
        return AmbientVolume;
    }

    void AudioManager::SetDialogVolume(const Real& Dialog)
    {
        DialogVolume = Dialog;
        for( std::vector<Audio::Sound*>::iterator it = DialogSounds.begin() ; it != DialogSounds.end() ; it++ )
        {
            (*it)->UpdateVolume();
        }
    }

    Real AudioManager::GetDialogVolume() const
    {
        return DialogVolume;
    }

    void AudioManager::SetEffectVolume(const Real& Effect)
    {
        EffectVolume = Effect;
        for( std::vector<Audio::Sound*>::iterator it = EffectSounds.begin() ; it != EffectSounds.end() ; it++ )
        {
            (*it)->UpdateVolume();
        }
    }

    Real AudioManager::GetEffectVolume() const
    {
        return EffectVolume;
    }

    void AudioManager::SetMusicVolume(const Real& Music)
    {
        MusicVolume = Music;
        for( std::vector<Audio::Sound*>::iterator it = MusicSounds.begin() ; it != MusicSounds.end() ; it++ )
        {
            (*it)->UpdateVolume();
        }
    }

    Real AudioManager::GetMusicVolume() const
    {
        return MusicVolume;
    }

    void AudioManager::SetMasterVolume(const Real& Master)
    {
        MasterVolume = Master;
        for( std::map<String,Audio::Sound*>::iterator it = Sounds.begin() ; it != Sounds.end() ; it++ )
        {
            (*it).second->UpdateVolume();
        }
    }

    Real AudioManager::GetMasterVolume() const
    {
        return MasterVolume;
    }

    String AudioManager::GetAvailableDeviceNameByIndex(const Whole& Index) const
    {
        const char* Name = cAudioManager->getAvailableDeviceName(Index);
        String Device(Name);
        return Device;
    }

    String AudioManager::GetDefaultDeviceName() const
    {
        const char* Name = cAudioManager->getDefaultDeviceName();
        String Device(Name);
        return Device;
    }

    Whole AudioManager::GetAvailableDeviceCount() const
    {
        return cAudioManager->getAvailableDeviceCount();
    }

    std::stringstream* AudioManager::GetLogs() const
    {
        return 0; //cAudioManager->GetLogs();
    }

    void AudioManager::ClearLogs()
    {
        //cAudioManager->ClearLogs();
    }

    void AudioManager::InitializeManager(ConstString &DeviceName, int OutputFrequency, int EAXEffectSlots)
    {
        cAudioManager->initialize(DeviceName.c_str(), OutputFrequency, EAXEffectSlots);
    }

    Audio::Listener* AudioManager::GetListener() const
    {
        return Listener;
    }

    cAudio::IAudioManager* AudioManager::GetcAudioManager() const
    {
        return cAudioManager;
    }

    void AudioManager::Initialize()
        {}

    void AudioManager::DoMainLoopItems()
        {}

    ManagerBase::ManagerTypeName AudioManager::GetType() const
        { return ManagerBase::AudioManager; }
}//phys

#endif

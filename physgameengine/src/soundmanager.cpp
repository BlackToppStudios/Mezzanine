//© Copyright 2010 BlackTopp Studios Inc.
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
#ifndef _soundmanager_cpp
#define _soundmanager_cpp

#include "soundmanager.h"
#include "world.h"
#include <cAudio.h>

namespace phys
{
    SoundManager::SoundManager(World* ParentWorld, bool DefaultSettings) : ManagerBase(ParentWorld)
    {
        AudioManager = cAudio::createAudioManager(DefaultSettings);
        AudioListener = AudioManager->getListener();
        this->Priority = 10;
    }

    SoundManager::~SoundManager()
    {
        cAudio::destroyAudioManager(AudioManager);
    }

    void SoundManager::Initialize()
    {
    }

    void SoundManager::DoMainLoopItems()
    {
    }

    void SoundManager::InitializeManager(String &DeviceName, int OutputFrequency, int EAXEffectSlots)
    {
        AudioManager->initialize(DeviceName.c_str(), OutputFrequency, EAXEffectSlots);
    }

    Sound* SoundManager::CreateSound(String &SoundName, String &FilePath, bool Stream)
    {
        #define PHYSDEBUG
        #ifdef PHYSDEBUG
        this->GameWorld->LogStream<< "Entering: SoundManager::CreateSound( " << SoundName << ", " << FilePath << ", " << Stream << ");" << endl;
        this->GameWorld->Log("Attempting to create Sound");
        #endif
        cAudio::IAudioSource* IAudio = AudioManager->create(SoundName.c_str(), FilePath.c_str(), Stream);
        Sound* pSound = new Sound(IAudio, AudioManager);
        return pSound;
    }

    Sound* SoundManager::CreateSoundFromMemory(String &SoundName, const char* Data, int Size, String &Extension)
    {
        cAudio::IAudioSource* IAudio = AudioManager->createFromMemory(SoundName.c_str(), Data, Size, Extension.c_str());
        Sound* pSound = new Sound(IAudio, AudioManager);
        return pSound;
    }

    /*Sound* SoundManager::CreateSoundFromRaw(String &SoundName, const char* Data, int Size, Whole Frequency, Whole Quality)
    {
        if(Quality < 1 || Quality >4)
        {
            GameWorld->Log("Invalid Sound Quality attempted.")
            Sound* pSound = NULL;
            return pSound;
        }
        if(Quality==1)
        {
            cAudio::IAudioSource* IAudio = AudioManager->createFromMemory(SoundName.c_str(), Data, Size, Frequency, cAudio::EAF_8BIT_MONO);
        }
        if(Quality==2)
        {
            cAudio::IAudioSource* IAudio = AudioManager->createFromMemory(SoundName.c_str(), Data, Size, Frequency, cAudio::EAF_8BIT_Stereo);
        }
        if(Quality==3)
        {
            cAudio::IAudioSource* IAudio = AudioManager->createFromMemory(SoundName.c_str(), Data, Size, Frequency, cAudio::EAF_16BIT_MONO);
        }
        if(Quality==4)LogStream
        {
            cAudio::IAudioSource* IAudio = AudioManager->createFromMemory(SoundName.c_str(), Data, Size, Frequency, cAudio::EAF_16BIT_Stereo);
        }
        Sound* pSound = new Sound(IAudio, AudioManager);
        return pSound;
    }*/

    void SoundManager::DestroySound(Sound* SoundName)
    {
        AudioManager->release(SoundName->SoundSource);
    }

    void SoundManager::DestroyAllSounds()
    {
        AudioManager->releaseAllSources();
    }

    Sound* SoundManager::GetSoundByName(String SoundName)
    {
        cAudio::IAudioSource* Source = AudioManager->getSoundByName(SoundName.c_str());
        if(Source)
        {
            Sound* SoundInst = new Sound(Source, AudioManager);
            return SoundInst;
        }
        return NULL;
    }

    SoundSet* SoundManager::CreateSoundSet(String SoundSetName)
    {
        SoundSet* Set = new SoundSet();
        this->SoundSets[SoundSetName] = Set;
        return Set;
    }

    SoundSet* SoundManager::GetSoundSet(String SoundSetName)
    {
        SoundSet* Set = this->SoundSets[SoundSetName];
        return Set;
    }

    void SoundManager::AddSoundToSoundSet(String SoundSetName, Sound* SoundInst)
    {
        SoundSet* Set = this->SoundSets[SoundSetName];
        Set->push_back(SoundInst);
    }

    String SoundManager::GetAvailableDeviceNameByIndex(Whole Index)
    {
        const char* Name = AudioManager->getAvailableDeviceName(Index);
        String Device(Name);
        return Device;
    }

    String SoundManager::GetDefaultDeviceName()
    {
        const char* Name = AudioManager->getDefaultDeviceName();
        String Device(Name);
        return Device;
    }

    Whole SoundManager::GetAvailableDeviceCount()
    {
        return AudioManager->getAvailableDeviceCount();
    }

    SoundListener* SoundManager::GetListener()
    {
        SoundListener* List = new SoundListener(AudioListener);
        return List;
    }

    ManagerBase::ManagerTypeName SoundManager::GetType() const
        { return ManagerBase::SoundManager; }

    std::stringstream* SoundManager::GetLogs()
    {
        return 0; //AudioManager->GetLogs();
    }

    void SoundManager::ClearLogs()
    {
        //AudioManager->ClearLogs();
    }
}

#endif

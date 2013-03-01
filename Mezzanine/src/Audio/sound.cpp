//© Copyright 2010 - 2012 BlackTopp Studios Inc.
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
#ifndef _audiosound_cpp
#define _audiosound_cpp

#include "Audio/sound.h"
#include "audiomanager.h"
#include "entresol.h"
#include "exception.h"
#include <cAudio.h>
#include <Ogre.h>

namespace Mezzanine
{
    namespace Audio
    {
        Sound::Sound(ConstString& SoundName, ConstString& FileName, ConstString& Group, Audio::SoundType SType)
            : Type(SType),
              BaseVolume(1.0)
        {
            Manager = AudioManager::GetSingletonPtr();
            String Extension = FileName.substr(FileName.find_last_of(".")+1);
            Ogre::DataStreamPtr Stream = Ogre::ResourceGroupManager::getSingleton().openResource(FileName,Group);
            char* buffer = new char[Stream->size()];
            Stream->read((void*)buffer, Stream->size());

            SoundSource = Manager->GetcAudioManager()->createFromMemory(SoundName.c_str(),buffer,Stream->size(),Extension.c_str());
            if (!SoundSource)
                { MEZZ_EXCEPTION(Exception::INTERNAL_EXCEPTION,"Failed to create sound from file."); }

            delete[] buffer;
            UpdateVolume();
        }

        Sound::~Sound()
        {
            Manager->GetcAudioManager()->release(SoundSource);
        }

        Real Sound::GetTypeVolume() const
        {
            switch(Type)
            {
                case Audio::ST_Ambient: return Manager->GetAmbientVolume();
                case Audio::ST_Dialog: return Manager->GetDialogVolume();
                case Audio::ST_Effect: return Manager->GetEffectVolume();
                case Audio::ST_Music: return Manager->GetMusicVolume();
                default: return 0;
            }
        }

        void Sound::Play()
        {
            SoundSource->play();
        }

        void Sound::Play2d(bool Loop)
        {
            SoundSource->play2d(Loop);
        }

        void Sound::Play3d(const Vector3& Position, Real SoundStrength, bool Loop)
        {
            SoundSource->play3d(Position.GetcAudioVector3(), SoundStrength, Loop);
        }

        void Sound::Pause()
        {
            SoundSource->pause();
        }

        void Sound::Stop()
        {
            SoundSource->stop();
        }

        void Sound::Loop(bool ToLoop)
        {
            SoundSource->loop(ToLoop);
        }

        void Sound::Seek(const Real& Seconds, bool Relative)
        {
            SoundSource->seek(Seconds, Relative);
        }

        bool Sound::IsPlaying() const
        {
            return SoundSource->isPlaying();
        }

        bool Sound::IsPaused() const
        {
            return SoundSource->isPaused();
        }

        bool Sound::IsStopped() const
        {
            return SoundSource->isStopped();
        }

        bool Sound::IsLooping() const
        {
            return SoundSource->isLooping();
        }

        void Sound::SetBaseVolume(const Real& Base)
        {
            BaseVolume = Base;
        }

        Real Sound::GetBaseVolume() const
        {
            return BaseVolume;
        }

        Real Sound::GetVolume() const
        {
            return BaseVolume * GetTypeVolume() * Manager->GetMasterVolume();
        }

        void Sound::SetMinVolume(const Real& MinVolume)
        {
            SoundSource->setMinVolume(MinVolume);
        }

        Real Sound::GetMinVolume() const
        {
            return SoundSource->getMinVolume();
        }

        void Sound::SetMaxVolume(const Real& MaxVolume)
        {
            SoundSource->setMaxVolume(MaxVolume);
        }

        Real Sound::GetMaxVolume() const
        {
            return SoundSource->getMaxVolume();
        }

        void Sound::UpdateVolume()
        {
            SoundSource->setVolume(BaseVolume * GetTypeVolume() * Manager->GetMasterVolume());
        }

        Real Sound::GetTotalAudioTime() const
        {
            return SoundSource->getTotalAudioTime();
        }

        int Sound::GetTotalAudioSize() const
        {
            return SoundSource->getTotalAudioSize();
        }

        int Sound::GetCompressedAudioSize() const
        {
            return SoundSource->getCompressedAudioSize();
        }

        Real Sound::GetCurrentAudioTime() const
        {
            return SoundSource->getCurrentAudioTime();
        }

        int Sound::GetCurrentAudioPosition() const
        {
            return SoundSource->getCurrentAudioPosition();
        }

        int Sound::GetCurrentCompressedAudioPosition() const
        {
            return SoundSource->getCurrentCompressedAudioPosition();
        }

        String Sound::GetName() const
        {
            return Name;
        }

        Audio::SoundType Sound::GetType() const
        {
            return Type;
        }

        void Sound::SetPitch(const Real& Pitch)
        {
            SoundSource->setPitch(Pitch);
        }

        Real Sound::GetPitch() const
        {
            return SoundSource->getPitch();
        }

        void Sound::Move(const Vector3& Position)
        {
            SoundSource->move(Position.GetcAudioVector3());
        }

        void Sound::SetPosition(const Vector3& Position)
        {
            SoundSource->setPosition(Position.GetcAudioVector3());
        }

        Vector3 Sound::GetPosition() const
        {
            Vector3 Pos(SoundSource->getPosition());
            return Pos;
        }

        void Sound::SetVelocity(const Vector3& Velocity)
        {
            SoundSource->setVelocity(Velocity.GetcAudioVector3());
        }

        Vector3 Sound::GetVelocity() const
        {
            Vector3 Vel(SoundSource->getVelocity());
            return Vel;
        }

        void Sound::SetDirection(const Vector3& Direction)
        {
            SoundSource->setDirection(Direction.GetcAudioVector3());
        }

        Vector3 Sound::GetDirection() const
        {
            Vector3 Dir(SoundSource->getDirection());
            return Dir;
        }

        void Sound::SetRolloffFactor(const Real& Rolloff)
        {
            SoundSource->setRolloffFactor(Rolloff);
        }

        Real Sound::GetRolloffFactor() const
        {
            return SoundSource->getRolloffFactor();
        }

        void Sound::SetStrength(const Real& SoundStrength)
        {
            SoundSource->setStrength(SoundStrength);
        }

        Real Sound::GetStrength() const
        {
            return SoundSource->getStrength();
        }

        void Sound::SetMinDistance(const Real& MinDistance)
        {
            SoundSource->setMinDistance(MinDistance);
        }

        Real Sound::GetMinDistance() const
        {
            return SoundSource->getMinDistance();
        }

        void Sound::SetMaxDistance(const Real& MaxDistance)
        {
            SoundSource->setMaxDistance(MaxDistance);
        }

        Real Sound::GetMaxDistance() const
        {
            return SoundSource->getMaxDistance();
        }

        void Sound::SetInnerConeAngle(const Real& InnerAngle)
        {
            SoundSource->setInnerConeAngle(InnerAngle);
        }

        Real Sound::GetInnerConeAngle() const
        {
            return SoundSource->getInnerConeAngle();
        }

        void Sound::SetOuterConeAngle(const Real& OuterAngle)
        {
            SoundSource->setOuterConeAngle(OuterAngle);
        }

        Real Sound::GetOuterConeAngle() const
        {
            return SoundSource->getOuterConeAngle();
        }

        void Sound::SetOuterConeVolume(const Real& OuterVolume)
        {
            SoundSource->setOuterConeVolume(OuterVolume);
        }

        Real Sound::GetOuterConeVolume() const
        {
            return SoundSource->getOuterConeVolume();
        }

        void Sound::SetDopplerStrength(const Real& DopStr)
        {
            SoundSource->setDopplerStrength(DopStr);
        }

        Real Sound::GetDopplerStrength() const
        {
            return SoundSource->getDopplerStrength();
        }

        void Sound::SetDopplerVelocity(const Vector3& Velocity)
        {
            SoundSource->setDopplerVelocity(Velocity.GetcAudioVector3());
        }

        Vector3 Sound::GetDopplerVelocity() const
        {
            Vector3 DopVel(SoundSource->getDopplerVelocity());
            return DopVel;
        }
    }//Audio
}//Mezzanine

#endif

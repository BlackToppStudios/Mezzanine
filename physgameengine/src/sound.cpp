//© Copyright 2010 Joseph Toppi and John Blackwood
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
#ifndef _sound_cpp
#define _sound_cpp

#include "sound.h"
#include <cAudio.h>

namespace phys
{
    Sound::Sound(cAudio::IAudioSource* Source, cAudio::IAudioManager* manager)
    {
        SoundSource = Source;
        Manager = manager;
    }

    Sound::~Sound()
    {
        Manager->release(SoundSource);
    }

    void Sound::Play()
    {
        SoundSource->play();
    }

    void Sound::Play2d(bool Loop)
    {
        SoundSource->play2d(Loop);
    }

    void Sound::Play3d(Vector3 Position, Real SoundStrength, bool Loop)
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

    void Sound::Seek(Real Seconds, bool Relative)
    {
        SoundSource->seek(Seconds, Relative);
    }

    Real Sound::GetTotalAudioTime()
    {
        return SoundSource->getTotalAudioTime();
    }

    int Sound::GetTotalAudioSize()
    {
        return SoundSource->getTotalAudioSize();
    }

    int Sound::GetCompressedAudioSize()
    {
        return SoundSource->getCompressedAudioSize();
    }

    Real Sound::GetCurrentAudioTime()
    {
        return SoundSource->getCurrentAudioTime();
    }

    int Sound::GetCurrentAudioPosition()
    {
        return SoundSource->getCurrentAudioPosition();
    }

    int Sound::GetCurrentCompressedAudioPosition()
    {
        return SoundSource->getCurrentCompressedAudioPosition();
    }

    bool Sound::IsPlaying()
    {
        return SoundSource->isPlaying();
    }

    bool Sound::IsPaused()
    {
        return SoundSource->isPaused();
    }

    bool Sound::IsStopped()
    {
        return SoundSource->isStopped();
    }

    bool Sound::IsLooping()
    {
        return SoundSource->isLooping();
    }

    void Sound::SetPosition(Vector3 Position)
    {
        SoundSource->setPosition(Position.GetcAudioVector3());
    }

    void Sound::SetVelocity(Vector3 Velocity)
    {
        SoundSource->setVelocity(Velocity.GetcAudioVector3());
    }

    void Sound::SetDirection(Vector3 Direction)
    {
        SoundSource->setDirection(Direction.GetcAudioVector3());
    }

    void Sound::SetRolloffFactor(Real Rolloff)
    {
        SoundSource->setRolloffFactor(Rolloff);
    }

    void Sound::SetStrength(Real SoundStrength)
    {
        SoundSource->setStrength(SoundStrength);
    }

    void Sound::SetMinDistance(Real MinDistance)
    {
        SoundSource->setMinDistance(MinDistance);
    }

    void Sound::SetMaxDistance(Real MaxDistance)
    {
        SoundSource->setMaxDistance(MaxDistance);
    }

    void Sound::SetPitch(Real Pitch)
    {
        SoundSource->setPitch(Pitch);
    }

    void Sound::SetVolume(Real Volume)
    {
        SoundSource->setVolume(Volume);
    }

    void Sound::SetMinVolume(Real MinVolume)
    {
        SoundSource->setMinVolume(MinVolume);
    }

    void Sound::SetMaxVolume(Real MaxVolume)
    {
        SoundSource->setMaxVolume(MaxVolume);
    }

    void Sound::SetInnerConeAngle(Real InnerAngle)
    {
        SoundSource->setInnerConeAngle(InnerAngle);
    }

    void Sound::SetOuterConeAngle(Real OuterAngle)
    {
        SoundSource->setOuterConeAngle(OuterAngle);
    }

    void Sound::SetOuterConeVolume(Real OuterVolume)
    {
        SoundSource->setOuterConeVolume(OuterVolume);
    }

    void Sound::SetDopplerStrength(Real DopStr)
    {
        SoundSource->setDopplerStrength(DopStr);
    }

    void Sound::SetDopplerVelocity(Vector3 Velocity)
    {
        SoundSource->setDopplerVelocity(Velocity.GetcAudioVector3());
    }

    void Sound::Move(Vector3 Position)
    {
        SoundSource->move(Position.GetcAudioVector3());
    }

    Vector3 Sound::GetPosition()
    {
        Vector3 Pos(SoundSource->getPosition());
        return Pos;
    }

    Vector3 Sound::GetVelocity()
    {
        Vector3 Vel(SoundSource->getVelocity());
        return Vel;
    }

    Vector3 Sound::GetDirection()
    {
        Vector3 Dir(SoundSource->getDirection());
        return Dir;
    }

    Real Sound::GetRolloffFactor()
    {
        return SoundSource->getRolloffFactor();
    }

    Real Sound::GetStrength()
    {
        return SoundSource->getStrength();
    }

    Real Sound::GetMinDistance()
    {
        return SoundSource->getMinDistance();
    }

    Real Sound::GetMaxDistance()
    {
        return SoundSource->getMaxDistance();
    }

    Real Sound::GetPitch()
    {
        return SoundSource->getPitch();
    }

    Real Sound::GetVolume()
    {
        return SoundSource->getVolume();
    }

    Real Sound::GetMinVolume()
    {
        return SoundSource->getMinVolume();
    }

    Real Sound::GetMaxVolume()
    {
        return SoundSource->getMaxVolume();
    }

    Real Sound::GetInnerConeAngle()
    {
        return SoundSource->getInnerConeAngle();
    }

    Real Sound::GetOuterConeAngle()
    {
        return SoundSource->getOuterConeAngle();
    }

    Real Sound::GetOuterConeVolume()
    {
        return SoundSource->getOuterConeVolume();
    }

    Real Sound::GetDopplerStrength()
    {
        return SoundSource->getDopplerStrength();
    }

    Vector3 Sound::GetDopplerVelocity()
    {
        Vector3 DopVel(SoundSource->getDopplerVelocity());
        return DopVel;
    }
}

#endif

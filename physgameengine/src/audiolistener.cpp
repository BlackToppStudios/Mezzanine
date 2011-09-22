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
#ifndef _audiolistener_cpp
#define _audiolistener_cpp

#include "audiolistener.h"
#include <cAudio.h>

namespace phys
{
    namespace Audio
    {
        Listener::Listener(cAudio::IListener* Listener)
        {
            AudioListener = Listener;
        }

        Listener::~Listener()
        {
        }

        void Listener::SetPosition(Vector3 Position)
        {
            AudioListener->setPosition(Position.GetcAudioVector3());
        }

        void Listener::SetDirection(Vector3 Direction)
        {
            AudioListener->setDirection(Direction.GetcAudioVector3());
        }

        void Listener::SetUpVector(Vector3 Up)
        {
            AudioListener->setUpVector(Up.GetcAudioVector3());
        }

        void Listener::SetVelocity(Vector3 Velocity)
        {
            AudioListener->setVelocity(Velocity.GetcAudioVector3());
        }

        void Listener::SetMasterVolume(Real Volume)
        {
            AudioListener->setMasterVolume(Volume);
        }

        void Listener::Move(Vector3 Position)
        {
            AudioListener->move(Position.GetcAudioVector3());
        }

        Vector3 Listener::GetPosition()
        {
            Vector3 Position(AudioListener->getPosition());
            return Position;
        }

        Vector3 Listener::GetDirection()
        {
            Vector3 Direction(AudioListener->getDirection());
            return Direction;
        }

        Vector3 Listener::GetUpVector()
        {
            Vector3 Up(AudioListener->getUpVector());
            return Up;
        }

        Vector3 Listener::GetVelocity()
        {
            Vector3 Velocity(AudioListener->getVelocity());
            return Velocity;
        }

        Real Listener::GetMasterVolume()
        {
            return AudioListener->getMasterVolume();
        }

        void Listener::SetMetersPerUnit(Real Meters)
        {
            //AudioListener->setMetersPerUnit(Meters);
        }

        Real Listener::GetMetersPerUnit()
        {
            return 0;//AudioListener->getMetersPerUnit();
        }
    }//Audio
}//phys

#endif

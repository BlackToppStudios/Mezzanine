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
#ifndef _soundlistener_h
#define _soundlistener_h

#include "crossplatformexport.h"
#include "vector3.h"

namespace cAudio
{
    class IListener;
}

namespace phys
{
    namespace Audio
    {
        class AudioManager;
        ///////////////////////////////////////////////////////////////////////////////
        /// @class SoundListener
        /// @headerfile soundlistener.h
        /// @brief This is the listener class used for 3D sound.
        /// @details This is the listener class used for determining how 3D sound is played.
        ///////////////////////////////////////////////////////////////////////////////
        class PHYS_LIB SoundListener
        {
            protected:
                cAudio::IListener* AudioListener;
            public:
                /// @brief Class constructor.  Internal use only.
                /// @details The Class constructor.  This is an internal function and shouldn't be called manually.
                /// @param Listener Pointer to the internal listener.
                SoundListener(cAudio::IListener* Listener);
                /// @brief Class destructor.
                /// @details The class destructor.
                ~SoundListener();
                /// @brief Sets the listener position.
                /// @details Sets the listeners position.
                /// @param Position The new position for the listener.
                void SetPosition(Vector3 Position);
                /// @brief Sets the direction the listener is facing.
                /// @details Sets the direction the listener is facing.
                /// @param Direction The new direction for the listener.
                void SetDirection(Vector3 Direction);
                /// @brief Sets the up direction for the listener.
                /// @details Sets the direction that is up relative to the orientation of the listener.  Default: +Y
                /// @param Up The new upward vector to be used by the listener.
                void SetUpVector(Vector3 Up);
                /// @brief Sets the velocity of the listener.
                /// @details Sets the listeners velocity, for use in determining doppler effects.
                /// @param Velocity The new velocity for the listener.
                void SetVelocity(Vector3 Velocity);
                /// @brief Sets the master volume for all sources heard by this listener.
                /// @details This function will scale the volume for all sources heard by this listener by the
                /// amount provided.
                /// @param Volume The amount by which to scale all sound sources heard.
                void SetMasterVolume(Real Volume);
                /// @brief Moves the listener to a new location.
                /// @details This function will set a new position and velocity of the listener.
                /// @param Position The new position of the listener.
                void Move(Vector3 Position);
                /// @brief Gets the current position of the listener.
                /// @details This function will get the current position of the listener.
                /// @return Returns the current position of the listener.
                Vector3 GetPosition();
                /// @brief Gets the current direction of the listener.
                /// @details This function will get the current direction of the listener.
                /// @return Returns the current direction of the listener.
                Vector3 GetDirection();
                /// @brief Sets the upward orientation of the listener.
                /// @details Sets which direction is up for the listener
                /// @return Returns the current up orientation of the listener.
                Vector3 GetUpVector();
                /// @brief Gets the current velocity of the listener.
                /// @details This function will get the current velocity of the listener.
                /// @return Returns the current velocity of the listener.
                Vector3 GetVelocity();
                /// @brief Gets the master volume used to scale sound sources.
                /// @details This function will get the master volume, which is used to scale the volume
                /// of all sound sources heard by this listener.
                /// @return Returns the current scale used for adjusting the volume of all sources heard by
                /// this listener.
                Real GetMasterVolume();
                /// @brief Sets the scale to be used by the manager for this listener.
                /// @details This function will set the engines scale for use with sounds.
                /// Internally the engine assumes all units are in meters.  If that is not the
                /// case then this function should be called to set the scale.
                /// @param Meters The amount of meters that are in one unit of your games scale.
                void SetMetersPerUnit(Real Meters);
                /// @brief Gets the scale used by the manager for this listener.
                /// @details This function will get the amount of meters per unit used in your
                /// game/simulation scale.  The engine assumes you use meters, so default is 1.0.
                /// @return Returns the amount of meters per unit in your scale.
                Real GetMetersPerUnit();
        };// sound listener
    }//Audio
}//phys

#endif

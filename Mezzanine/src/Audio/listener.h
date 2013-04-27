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
#ifndef _audiolistener_h
#define _audiolistener_h

#include "datatypes.h"

namespace Mezzanine
{
    namespace Audio
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is an interface class for a listener (such as a player) in the 3D audio world.
        /// @details
        ///////////////////////////////////////
        class iListener
        {
            public:
                /// @brief Class constructor.
                iListener() {}
                /// @brief Class destructor.
                virtual ~iListener() {}

                ///////////////////////////////////////////////////////////////////////////////
                // Convenience Methods

                /// @brief Convenience method for setting the location and velocity of this listener.
                /// @param Loc The 3D location to move this listener to.
                virtual void Move(const Vector3& Loc) = 0;

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief Sets a new location for this listener.
                /// @note This will not set the velocity, which also needs to be set for proper doppler effects.  Use @ref iListener::Move for that.
                /// @param Loc The 3D location to be set.
                virtual void SetLocation(const Vector3& Loc) = 0;
                /// @brief Gets the current location of this listener.
                /// @return Returns a Vector3 containing the current position of this listener.
                virtual Vector3 GetLocation() const = 0;
                /// @brief Sets the moving velocity of this listener.
                /// @param Vel The velocity this listener is to be given.
                virtual void SetVelocity(const Vector3& Vel) = 0;
                /// @brief Gets the current velocity of this listener.
                /// @return Returns a Vector3 containing the current velocity of this listener.
                virtual Vector3 GetVelocity() const = 0;
                /// @brief Sets the direction the listener is facing.
                /// @param Dir The direction this listener is to face.
                virtual void SetDirection(const Vector3& Dir) = 0;
                /// @brief Gets the current direction this listener is facing.
                /// @return Returns a Vector3 containing the direction this listener is facing.
                virtual Vector3 GetDirection() const = 0;

                /// @brief Sets which direction is to be considered "Up".
                /// @note Default: (0,1,0).
                /// @param Up The direction to be considered "Up".
                virtual void SetUpDirection(const Vector3& Up) = 0;
                /// @brief Gets the currently set direction that is considered "Up".
                /// @return Returns a Vector3 containing the direction currently considered "Up" to this listener.
                virtual Vector3 GetUpDirection() const = 0;
                /// @brief Sets the modifier to use that will adjust the volume of all audio heard by this listener.
                /// @param Vol The modifier to be set.  Range 0.0 to +Inf.
                virtual void SetVolumeModifier(const Real Vol) = 0;
                /// @brief Gets the modifier applied to all audio heard by this listener.
                /// @return Returns the currently set volume modifier.
                virtual Real GetVolumeModifier() const = 0;
                /// @brief Sets the number of meters in a single unit of world space.
                /// @param Meters The number of meters in a single unit of world space.
                virtual void SetMetersPerUnit(const Real Meters) = 0;
                /// @brief Gets the currently set number of meters in a single unit of world space.
                /// @return Returns a Real representing the number of meters in a unit of world space.
                virtual Real GetMetersPerUnit() const = 0;
        };//iListener
    }//Audio
}//Mezzanine

#endif

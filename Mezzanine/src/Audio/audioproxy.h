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
#ifndef _audioaudioproxy_h
#define _audioaudioproxy_h

#include "worldproxy.h"
#include "Audio/sound.h"

namespace Mezzanine
{
    namespace Audio
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a proxy class for representing a sound being played in 3D space.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB AudioProxy : public WorldProxy, public iSound
        {
            protected:
            public:
                /// @brief Class constructor.
                AudioProxy();
                /// @brief Class destructor.
                virtual ~AudioProxy();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                ///////////////////////////////////////////////////////////////////////////////
                // 3 Dimensional Configuration

                /// @brief Sets the Rolloff factor used to attenuate the sound over a distance.
                /// @details This function will set how much the sound source will attenuate over
                /// a distance.  Larger values will make the sound attenuate faster/shorter distnaces,
                /// smaller values will make the sound carry better.
                /// @param Rolloff The factor at which the sound will attenuate.
                virtual void SetRolloffFactor(const Real& Rolloff) = 0;
                /// @brief Gets the Rolloff factor of the sound source.
                /// @details This function will get the factor used in attenuating the source over distance.
                /// @return Returns the factor used in attenuating the source over distance.
                virtual Real GetRolloffFactor() const = 0;
                /// @brief Sets the strength of the sound source.
                /// @details This function will set the strength of the sound, which will determine how well
                /// the sound from this sound source will carry over a distance.
                /// @param SoundStrength The strength of the sound.  Default: 1.0
                virtual void SetStrength(const Real& SoundStrength) = 0;
                /// @brief Gets the Strength of the sound source.
                /// @details This function will get the strength of the source, which determines how well
                /// the sound carries over a distance.
                /// @return Returns the strength of the source.
                virtual Real GetStrength() const = 0;
                /// @brief Sets the distance from the sound source where attenuation will start.
                /// @details This function will set the distance from the sound source where attenuation
                /// effects will start being applied.
                /// @param MinDistance The distance at which attenuation effects start being applied.
                virtual void SetMinDistance(const Real& MinDistance) = 0;
                /// @brief Gets the distance at which sound attenuation will start.
                /// @return Returns the distance from the source where attenuation will start.
                virtual Real GetMinDistance() const = 0;
                /// @brief Sets the distance from the sound source where attenuation will stop.
                /// @details This function will set the distance from the sound source where attenuation
                /// effects will stop being applied.
                /// @param MaxDistance The distance at which attenuation effects stop being applied.
                virtual void SetMaxDistance(const Real& MaxDistance) = 0;
                /// @brief Gets the distance at which sound attenuation will stop.
                /// @return Returns the distance from the source where attenuation will stop.
                virtual Real GetMaxDistance() const = 0;
                /// @brief Sets the inner cone angle of the sound source if you want the sound to be projected as a cone.
                /// @details This function will set the angle for the inner cone which the sound will be projected.  The
                /// cone will open up in the direction set by SetDirection().  Sounds are at their loudest only if the
                /// listener is within the inner cone.
                /// @param InnerAngle The angle of the inner cone.  Range is: 0.0 to 360.0.  Default: 360.0 (broadcast)
                virtual void SetInnerConeAngle(const Real& InnerAngle) = 0;
                /// @brief Gets the inner cone angle of the sound source.
                /// @return Returns the angle of the inner sound cone of the source.
                virtual Real GetInnerConeAngle() const = 0;
                /// @brief Sets the outer cone angle of the sound source if you want the sound to be projected as a cone.
                /// @details This function will set the angle for the outer cone which the sound will be projected.  The
                /// cone will open up in the direction set by SetDirection().  Sounds connot be heard if the listener is
                /// outside the outer cone.
                /// @param OuterAngle The angle of the outer cone.  Range is: 0.0 to 360.0.  Default: 360.0 (broadcast)
                virtual void SetOuterConeAngle(const Real& OuterAngle) = 0;
                /// @brief Gets the outer cone angle of the sound source.
                /// @return Returns the angle of the outer sound cone of the source.
                virtual Real GetOuterConeAngle() const = 0;
                /// @brief Sets how much the volume is scaled in the outer cone.
                /// @details This function will set how much the volume is scaled by for sounds in the outer
                /// cone of the sound source.
                /// @param OuterVolume The scale for volume for sounds in the outer cone.
                virtual void SetOuterConeVolume(const Real& OuterVolume) = 0;
                /// @brief Gets the outer cone volume of the sound source.
                /// @return Returns how much the volume of the source is scaled in the outer cone.
                virtual Real GetOuterConeVolume() const = 0;
                /// @brief Sets the doppler strength, which impacts the doppler effect.
                /// @details This function will set the doppler strength of the sound source, which can
                /// enhance or diminish the doppler effect given off by this sound source.  Default: 1.0
                /// @param DopStr The doppler strength to be applied to this sound source.
                virtual void SetDopplerStrength(const Real& DopStr) = 0;
                /// @brief Gets the Doppler Strength of the sound.
                /// @return Returns the doppler strength.
                virtual Real GetDopplerStrength() const = 0;
                /// @brief Sets the doppler velocity vector.
                /// @details In most cases you shouldn't need to call this as this is handled by the engine.
                /// Is called every time position, velocity, or direction is altered.
                /// @param Velocity A vector3 representing the doppler velocity to be applied.
                virtual void SetDopplerVelocity(const Vector3& Velocity) = 0;
                /// @brief Gets the doppler velocity vector.
                /// @return Returns the override for the doppler velocity vector.
                virtual Vector3 GetDopplerVelocity() const = 0;
        };//AudioProxy
    }//Audio
}//Mezzanine

#endif

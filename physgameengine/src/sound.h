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
#ifndef _sound_h
#define _sound_h

#include "vector3.h"
#include "audioenumerations.h"

namespace cAudio
{
    class IAudioManager;
    class IAudioSource;
}

namespace phys
{
    class AudioManager;
    namespace Audio
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @class Sound
        /// @headerfile sound.h
        /// @brief This is an instance of a sound that can be played and manipulated.
        /// @details This is a an instance of a sound that can be played, have effects
        /// applied to it, paused, stopped, triggered and other such crazyness.
        ///////////////////////////////////////////////////////////////////////////////
        class PHYS_LIB Sound
        {
            protected:
                friend class phys::AudioManager;
                cAudio::IAudioSource* SoundSource;
                AudioManager* Manager;
                Audio::SoundType Type;
                Real BaseVolume;
                /// @internal
                /// @brief Class Constructor.  Internal use only.
                Sound();
                /// @internal
                /// @brief Class Constructor.  Internal use only.
                Sound(cAudio::IAudioSource* Source);
                /// @internal
                /// @brief Gets the volume of this sound type.
                virtual Real GetTypeVolume() const;
            public:
                /// @brief Class constructor.
                /// @param SoundName The name of the Sound instance.
                /// @param FileName The name of the file.
                /// @param Group The resource group in which the file resides.
                /// @param SType The type of sound that this instance is.  See enum Audio::SoundType for more info.
                Sound(ConstString& SoundName, ConstString& FileName, ConstString& Group, Audio::SoundType SType);
                /// @brief Class destructor.
                /// @details The class destructor.
                virtual ~Sound();

                ///////////////////////////////////////////////////////////////////////////////
                // Playback

                /// @brief Plays the sound.
                /// @details Plays the sound with the same settings as the last time it was played.
                virtual void Play();
                /// @brief Plays the sound in 2D mode.
                /// @details This will play the sound without altering it by it's position in 3D space.  Ideal
                /// for music and ambient sounds.
                /// @param Loop Whether or not the sound should restart once it's done playing.
                virtual void Play2d(bool Loop = false);
                /// @brief Plays the sound in 3D mode.
                /// @details This will play the sound and alter it's properties based on it's location in 3D space.
                /// Sounds can sound like they are coming more from one direction then another, etc..
                /// @param Position The location in 3D space where the sound originates.
                /// @param SoundStrength The higher the sound strength, the further away you can hear the sound, not
                /// the same as volume.
                /// @param Loop Whether or not the sound should restart once it's done playing.
                virtual void Play3d(const Vector3& Position, Real SoundStrength=1.0, bool Loop = false);
                /// @brief Pauses the sound.
                /// @details Pauses the sound.
                virtual void Pause();
                /// @brief Stops the sound.
                /// @details Stops the sound.
                virtual void Stop();
                /// @brief Sets whether this sound should loop.
                /// @details Sets whether this sound should restart once it's done playing.
                /// @param ToLoop Whether or not this sound should restart
                virtual void Loop(bool ToLoop);
                /// @brief Allows you to go to a specific point in the sound.
                /// @details This function will allow you to jump ahead or rewind to any specific
                /// point in the sound.
                /// @param Seconds How many seconds into the sound you want to jump to.
                /// @param Relative Whether or not you want the jump to be relative to where the sound
                /// is at the time of calling this function.
                virtual void Seek(const Real& Seconds, bool Relative = true);
                /// @brief Checks to see if the sound is currently playing.
                /// @details Checks to see if the sound is currently playing.
                /// @return Returns whether or not the sound is currently playing.
                virtual bool IsPlaying() const;
                /// @brief Checks to see if the sound is currently paused.
                /// @details Checks to see if the sound is currently paused.
                /// @return Returns whether or not the sound is currently paused.
                virtual bool IsPaused() const;
                /// @brief Checks to see if the sound is currently stopped.
                /// @details Checks to see if the sound is currently stopped.
                /// @return Returns whether or not the sound is currently stopped.
                virtual bool IsStopped() const;
                /// @brief Checks to see if the sound is currently set to loop.
                /// @details Checks to see if the sound is currently set to loop.
                /// @return Returns whether or not the sound is currently set to loop.
                virtual bool IsLooping() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Volume

                /// @brief Sets the current volume of the sound source before effects and other volume settings.
                /// @details This function will set the current volume of the sound source before effects
                /// (like attenuation) are applied.
                /// @param Base The volume of the sound source to be applied.
                virtual void SetBaseVolume(const Real& Base);
                /// @brief Gets the current volume of the sound source.
                /// @details This function will get the current volume of the sound source before effects and other volume settings are applied.
                /// @return Returns the source volume before attenuation and other effects.
                virtual Real GetBaseVolume() const;
                /// @brief Gets the current volume of the sound source after all volume settings are applied.
                /// @return Returns a Real representing the volume of this sound after all volume settings are applied.
                virtual Real GetVolume() const;
                /// @brief Sets the minimum volume the sound source can achieve.
                /// @details This function will set the minimum volume the sound source can achieve after
                /// effects(like attenuation) have been applied.
                /// @param MinVolume The minimum volume allowed for the sound source.
                virtual void SetMinVolume(const Real& MinVolume);
                /// @brief Gets the minimum volume of the sound source.
                /// @return Returns the minimum volume that the source can be attenuated to.
                virtual Real GetMinVolume() const;
                /// @brief Sets the maximum volume the sound source can achieve.
                /// @details This function will set the maximum volume the sound source can achieve after
                /// effects(like attenuation) have been applied.
                /// @param MaxVolume The maximum volume allowed for the sound source.
                virtual void SetMaxVolume(const Real& MaxVolume);
                /// @brief Gets the Maximum volume of the sound source.
                /// @return Returns the maximum volume that the source can achieve.
                virtual Real GetMaxVolume() const;
                /// @brief Calculates all volume settings and applies them to this sound.
                virtual void UpdateVolume();

                ///////////////////////////////////////////////////////////////////////////////
                // Sound Stats

                /// @brief Gets the total size of the sound in seconds.
                /// @details This function will return the amount of seconds of the sound.
                /// @return Returns a Real that is the size in seconds.
                virtual Real GetTotalAudioTime() const;
                /// @brief Gets the total size of the sound in memory.
                /// @details This function will return the total size of the sound in memory.
                /// @return Returns a Real that is the size in memory.
                virtual int GetTotalAudioSize() const;
                /// @brief Gets the original compressed size of the sound.
                /// @details This function will return the compressed size of the sound.
                /// @return Returns a Real that is the original compressed size.
                virtual int GetCompressedAudioSize() const;
                /// @brief Gets the current position in the audio stream in seconds.
                /// @details This function will get the current position of the sound in the audio stream.
                /// @return Returns the current position in the audio stream in seconds.
                virtual Real GetCurrentAudioTime() const;
                /// @brief Gets the current position in the audio stream in bytes.
                /// @details This function will get the current position of the sound in the audio stream.
                /// @return Returns the current position in the audio stream in bytes.
                virtual int GetCurrentAudioPosition() const;
                /// @brief Gets the current position in the original audio stream in bytes.
                /// @details This function will get the current position of the sound in the original audio stream.
                /// @return Returns the current position in the original audio stream in bytes.
                virtual int GetCurrentCompressedAudioPosition() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @brief Sets the pitch of the sound source.
                /// @details This function will set the pitch of the sound source.  Note: higher values
                /// will speed up the playback of the sound.  Default: 1.0
                /// @param Pitch The new pitch of the sound.
                virtual void SetPitch(const Real& Pitch);
                /// @brief Gets the pitch of the sound source.
                /// @details This function will get the pitch of the sound source.
                /// @return Returns the pitch of the source.
                virtual Real GetPitch() const;
                /// @brief Moves the sound source.
                /// @details This function will set both the position and velocity of the sound source.
                /// @param Position The new position of the sound source.
                virtual void Move(const Vector3& Position);

                ///////////////////////////////////////////////////////////////////////////////
                // 3 Dimensions

                /// @brief Sets the position of the sound source.
                /// @details This function will set the position of the source to be used
                /// with the sound in 3D mode.
                /// @param Position The new position of the sound source.
                virtual void SetPosition(const Vector3& Position);
                /// @brief Gets the objects position.
                /// @details Gets the objects current position.
                /// @return Returns the objects current position.
                virtual Vector3 GetPosition() const;
                /// @brief Sets the velocity of the sound source.
                /// @details This function will set the velocity of the source to be used
                /// with the sound in 3D mode.
                /// @param Velocity The new velocity of the sound source.
                virtual void SetVelocity(const Vector3& Velocity);
                /// @brief Gets the objects velocity.
                /// @details Gets the objects current velocity.
                /// @return Returns the objects current velocity.
                virtual Vector3 GetVelocity() const;
                /// @brief Sets the direction the sound source is facing.
                /// @details This function will set the direction the sound source is facing
                /// to be used with the sound in 3D mode.
                /// @param Direction The new direction of the sound source.
                virtual void SetDirection(const Vector3& Direction);
                /// @brief Gets the objects direction.
                /// @details Gets the objects current direction.
                /// @return Returns the objects current direction.
                virtual Vector3 GetDirection() const;
                /// @brief Sets the Rolloff factor used to attenuate the sound over a distance.
                /// @details This function will set how much the sound source will attenuate over
                /// a distance.  Larger values will make the sound attenuate faster/shorter distnaces,
                /// smaller values will make the sound carry better.
                /// @param Rolloff The factor at which the sound will attenuate.
                virtual void SetRolloffFactor(const Real& Rolloff);
                /// @brief Gets the Rolloff factor of the sound source.
                /// @details This function will get the factor used in attenuating the source over distance.
                /// @return Returns the factor used in attenuating the source over distance.
                virtual Real GetRolloffFactor() const;
                /// @brief Sets the strength of the sound source.
                /// @details This function will set the strength of the sound, which will determine how well
                /// the sound from this sound source will carry over a distance.
                /// @param SoundStrength The strength of the sound.  Default: 1.0
                virtual void SetStrength(const Real& SoundStrength);
                /// @brief Gets the Strength of the sound source.
                /// @details This function will get the strength of the source, which determines how well
                /// the sound carries over a distance.
                /// @return Returns the strength of the source.
                virtual Real GetStrength() const;
                /// @brief Sets the distance from the sound source where attenuation will start.
                /// @details This function will set the distance from the sound source where attenuation
                /// effects will start being applied.
                /// @param MinDistance The distance at which attenuation effects start being applied.
                virtual void SetMinDistance(const Real& MinDistance);
                /// @brief Gets the distance at which sound attenuation will start.
                /// @details This function will get the distance at which sound attenuation will start.
                /// @return Returns the distance from the source where attenuation will start.
                virtual Real GetMinDistance() const;
                /// @brief Sets the distance from the sound source where attenuation will stop.
                /// @details This function will set the distance from the sound source where attenuation
                /// effects will stop being applied.
                /// @param MaxDistance The distance at which attenuation effects stop being applied.
                virtual void SetMaxDistance(const Real& MaxDistance);
                /// @brief Gets the distance at which sound attenuation will stop.
                /// @details This function will get the distance at which sound attenuation will stop.
                /// @return Returns the distance from the source where attenuation will stop.
                virtual Real GetMaxDistance() const;
                /// @brief Sets the inner cone angle of the sound source if you want the sound to be projected as a cone.
                /// @details This function will set the angle for the inner cone which the sound will be projected.  The
                /// cone will open up in the direction set by SetDirection().  Sounds are at their loudest only if the
                /// listener is within the inner cone.
                /// @param InnerAngle The angle of the inner cone.  Range is: 0.0 to 360.0.  Default: 360.0 (broadcast)
                virtual void SetInnerConeAngle(const Real& InnerAngle);
                /// @brief Gets the inner cone angle of the sound source.
                /// @details This function will get the inner cone angle of the sound source.  See SetInnerConeAngle().
                /// @return Returns the angle of the inner sound cone of the source.
                virtual Real GetInnerConeAngle() const;
                /// @brief Sets the outer cone angle of the sound source if you want the sound to be projected as a cone.
                /// @details This function will set the angle for the outer cone which the sound will be projected.  The
                /// cone will open up in the direction set by SetDirection().  Sounds connot be heard if the listener is
                /// outside the outer cone.
                /// @param OuterAngle The angle of the outer cone.  Range is: 0.0 to 360.0.  Default: 360.0 (broadcast)
                virtual void SetOuterConeAngle(const Real& OuterAngle);
                /// @brief Gets the outer cone angle of the sound source.
                /// @details This function will get the outer cone angle of the sound source.  See SetOuterConeAngle().
                /// @return Returns the angle of the outer sound cone of the source.
                virtual Real GetOuterConeAngle() const;
                /// @brief Sets how much the volume is scaled in the outer cone.
                /// @details This function will set how much the volume is scaled by for sounds in the outer
                /// cone of the sound source.
                /// @param OuterVolume The scale for volume for sounds in the outer cone.
                virtual void SetOuterConeVolume(const Real& OuterVolume);
                /// @brief Gets the outer cone volume of the sound source.
                /// @details This function will get the outer cone volume of the sound source.  See SetOuterConeVolume().
                /// @return Returns how much the volume of the source is scaled in the outer cone.
                virtual Real GetOuterConeVolume() const;
                /// @brief Sets the doppler strength, which impacts the doppler effect.
                /// @details This function will set the doppler strength of the sound source, which can
                /// enhance or diminish the doppler effect given off by this sound source.  Default: 1.0
                /// @param DopStr The doppler strength to be applied to this sound source.
                virtual void SetDopplerStrength(const Real& DopStr);
                /// @brief Gets the Doppler Strength of the sound.
                /// @details This function will get the doppler strength, which enhances or diminishes the doppler effect.
                /// @return Returns the doppler strength.
                virtual Real GetDopplerStrength() const;
                /// @brief Sets the doppler velocity vector.
                /// @details In most cases you shouldn't need to call this as this is handled by the engine.
                /// Is called every time position, velocity, or direction is altered.
                /// @param Velocity A vector3 representing the doppler velocity to be applied.
                virtual void SetDopplerVelocity(const Vector3& Velocity);
                /// @brief Gets the doppler velocity vector.
                /// @details This function will get the override for the doppler velocity vector.
                /// @return Returns the override for the doppler velocity vector.
                virtual Vector3 GetDopplerVelocity() const;
        }; // sound
    }//Audio
}//phys

#endif

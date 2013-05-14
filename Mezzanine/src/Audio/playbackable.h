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
#ifndef _audioplaybackable_h
#define _audioplaybackable_h

#include "datatypes.h"
#include "Audio/audioenumerations.h"
#include "Audio/decoder.h"

namespace Mezzanine
{
    namespace Audio
    {
        class iEffect;
        class iFilter;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is an interface class for any audio object intended for playback.
        /// @details The iPlaybackable class is the base class for 2D and 3D sound playback classes.  It contains all the basic
        /// methods needed to manipulate playback of the sound, methods for effects, methods for filters, independent
        /// volume, and a decoder which keeps track of all of it's audio stream specific data.
        ///////////////////////////////////////
        class iPlaybackable
        {
        public:
            /// @enum PlaybackState
            /// @brief This enum describes a set of boolean options common for objects playing back audio.
            enum PlaybackState
            {
                PS_Playing = 1,
                PS_Paused  = 2,
                PS_Stopped = 4,
                PS_Looping = 8
            };//PlaybackState
        public:
            /// @brief Class constructor.
            iPlaybackable() {  }
            /// @brief Class destructor.
            virtual ~iPlaybackable() {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets the sound type of this sound.
            /// @return Returns a SoundType enum value that is the type of this sound.
            virtual UInt16 GetType() const = 0;
            /// @brief Gets the decoder that belongs to this sound.
            /// @return Returns a pointer to the decoder being used by this sound for playback.
            virtual iDecoder* GetDecoder() const = 0;
            /// @brief Sets the pitch of the sound source.
            /// @note Higher values will speed up the playback of the sound.  Default: 1.0
            /// @param Pitch The new pitch of the sound.
            virtual void SetPitch(const Real Pitch) = 0;
            /// @brief Gets the pitch of the sound source.
            /// @return Returns the pitch of the source.
            virtual Real GetPitch() const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Playback

            /// @brief Plays the sound with it's current configuration.
            /// @return Returns true if the sound succussfully started playing, false if there was an error.
            virtual bool Play() = 0;
            /// @brief Gets whether or not the sound is currently playing.
            /// @return Returns true if the sound is playing, false otherwise.
            virtual bool IsPlaying() const = 0;
            /// @brief Pauses playback of the sound at it's current position in the stream.
            virtual void Pause() = 0;
            /// @brief Gets whether or not the sound is currently paused.
            /// @return Returns true if the sound is paused, false otherwise.
            virtual bool IsPaused() const = 0;
            /// @brief Stops playback of the sound and resets it's position back to the start.
            virtual void Stop() = 0;
            /// @brief Gets whether or not the sound is currently stopped.
            /// @return Returns true if the sound is stopped, false otherwise.
            virtual bool IsStopped() const = 0;
            /// @brief Sets whether the playback of the sound should loop or not.
            /// @param ToLoop Whether to restart the sound when the end of playback is reached.
            virtual void Loop(bool ToLoop) = 0;
            /// @brief Checks to see if the sound is currently set to loop.
            /// @return Returns true if this sound will restart from the beginning when it finishes playing back.
            virtual bool IsLooping() const = 0;

            /// @brief Sets the current position of the stream from which to playback audio.
            /// @note May not be supported by all codecs.
            /// @param Seconds Number of seconds to seek.
            /// @param Relative Whether to seek from the current position or the start of the stream.
            /// @return Returns true on success, False if the codec does not support seeking.
            virtual bool Seek(const Real Seconds, bool Relative = false) = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Sound Stream Stats

            /// @brief Gets the length of the stream in seconds.
            /// @return Returns the total amount of time needed to playback the sound in seconds.
            virtual Real GetTotalTime() const
                { this->GetDecoder()->GetTotalTime(); }
            /// @brief Gets the current time position in the stream.
            /// @return Returns the current position in the stream in seconds.
            virtual Real GetCurrentTime() const
                { this->GetDecoder()->GetCurrentTime(); }
            /// @brief Gets the size of the decoded audio source in use.
            /// @return Returns the size of the decoded audio source.
            virtual UInt32 GetTotalSize() const
                { this->GetDecoder()->GetTotalSize(); }
            /// @brief Gets the size of the encoded audio source in use.
            /// @return Returns the size of the encoded audio source.
            virtual UInt32 GetCompressedSize() const
                { this->GetDecoder()->GetCompressedSize(); }
            /// @brief Gets the sounds current position in the decoded audio source.
            /// @return Returns the current position in the decoded audio source in bytes.
            virtual UInt32 GetCurrentPosition() const
                { this->GetDecoder()->GetCurrentPosition(); }
            /// @brief Gets the sounds current position in the encoded audio source.
            /// @return Returns the current position in the encoded audio source in bytes.
            virtual UInt32 GetCurrentCompressedPosition() const
                { this->GetDecoder()->GetCurrentCompressedPosition(); }

            ///////////////////////////////////////////////////////////////////////////////
            // Volume Control

            /// @brief Gets the current volume of the sound source after all volume settings are applied.
            /// @note The internal constructs for sound will be updated of any changes to the volume of this object when the _Update() method is called.
            /// @return Returns a Real representing the volume of this sound after all volume settings are applied.
            virtual Real GetVolume() const = 0;
            /// @brief Sets the current volume of the sound source before effects and other volume settings.
            /// @details This function will set the current volume of the sound source before effects
            /// (like attenuation) are applied.
            /// @param Base The volume of the sound source to be applied.
            virtual void SetBaseVolume(const Real Base) = 0;
            /// @brief Gets the current volume of the sound source.
            /// @details This function will get the current volume of the sound source before effects and other volume settings are applied.
            /// @return Returns the source volume before attenuation and other effects.
            virtual Real GetBaseVolume() const = 0;
            /// @brief Sets the minimum volume the sound source can achieve.
            /// @details This function will set the minimum volume the sound source can achieve after
            /// effects(like attenuation) have been applied.
            /// @param MinVol The minimum volume allowed for the sound source.
            virtual void SetMinVolume(const Real MinVol) = 0;
            /// @brief Gets the minimum volume of the sound source.
            /// @return Returns the minimum volume that the source can be attenuated to.
            virtual Real GetMinVolume() const = 0;
            /// @brief Sets the maximum volume the sound source can achieve.
            /// @details This function will set the maximum volume the sound source can achieve after
            /// effects(like attenuation) have been applied.
            /// @param MaxVol The maximum volume allowed for the sound source.
            virtual void SetMaxVolume(const Real MaxVol) = 0;
            /// @brief Gets the Maximum volume of the sound source.
            /// @return Returns the maximum volume that the source can achieve.
            virtual Real GetMaxVolume() const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Effects Methods

            /// @brief Attaches an effect to this sound.
            /// @remarks Valid Slot Range: 0 to @ref iSound::GetNumEffectSlotsAvailable()
            /// @param Slot The slot into which the effect will be attached.
            /// @param Eff The effect to be attached.
            /// @return Returns true if the effect was successfully attached, false otherwise.
            virtual bool AttachEffect(const UInt32 Slot, iEffect* Eff) = 0;
            /// @brief Gets the max number of effects that can be attached to this sound.
            /// @return Returns the maximum number of effects this sound can support.
            virtual UInt32 GetNumEffectSlotsAvailable() const = 0;
            /// @brief Removes the effect in the specified slot.
            /// @remarks Valid Slot Range: 0 to @ref iSound::GetNumEffectSlotsAvailable()
            /// @param Slot The slot to remove the occupying effect.
            virtual void RemoveEffect(const UInt32 Slot) = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Filter Methods

            /// @brief Attaches a filter to this sound that will operate on the direct feed, separate from any effects.
            /// @note This will remove any previously attached filter.  Only one filter can be attached at a given time.
            /// @param Fil The filter to be attached.
            /// @return Returns true if the filter was successfully attached, false otherwise.
            virtual bool AttachFilter(iFilter* Fil) = 0;
            /// @brief Gets the filter currently being used by this object.
            /// @return Returns a pointer to the currently attached filter, or NULL if there isn't one.
            virtual iFilter* GetFilter() const = 0;
            /// @brief Removes any currently attached filters.
            virtual void RemoveFilter() = 0;
        };//iPlaybackable
    }//Audio
}//Mezzanine

#endif

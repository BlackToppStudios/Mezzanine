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
#ifndef _audiooalssound_h
#define _audiooalssound_h

#include "Audio/sound.h"

// OALS forward declares
struct ALCcontext;

namespace Mezzanine
{
    namespace Audio
    {
        namespace OALS
        {
            class Filter;
            class Effect;
            ///////////////////////////////////////////////////////////////////////////////
            /// @brief This is the OpenALSoft implemenation for a 2D sound.
            /// @details
            ///////////////////////////////////////
            class MEZZ_LIB Sound : public iSound
            {
            public:
                /// @brief Basic container type for the storage of @ref iEffect instances by this class.
                typedef std::vector<OALS::Effect*>       EffectContainer;
                /// @brief Iterator type for @ref iEffect instances stored by this class.
                typedef EffectContainer::iterator        EffectIterator;
                /// @brief Const Iterator type for @ref iEffect instances stored by this class.
                typedef EffectContainer::const_iterator  ConstEffectIterator;
                /// @brief Basic container type for the storage of internal buffer handles.
                typedef std::vector<UInt32>              BufferContainer;
                /// @brief Iterator type for buffer handles stored by this class.
                typedef BufferContainer::iterator        BufferIterator;
                /// @brief Const Iterator type for buffer handles stored by this class.
                typedef BufferContainer::const_iterator  ConstBufferIterator;
            protected:
                /// @internal
                /// @brief The OALS context this sound belongs to.
                ALCcontext* Context;
                /// @internal
                /// @brief The filter processing audio emitted by this sound.
                OALS::Filter* SoundFilter;
                /// @internal
                /// @brief The decoder responsible for tracking this sounds position in the audio stream and decompressing into something we can populate buffers with.
                iDecoder* SoundDecoder;
                /// @internal
                /// @brief The type of sound this is.
                UInt16 SType;
                /// @internal
                /// @brief This is a bitfield storing the the current state of this sound.
                UInt16 State;
                /// @internal
                /// @brief The speed of playback modifier for the sound.  Higher values make playback sound more high pitched.
                Real SoundPitch;
                /// @internal
                /// @brief The base volume applied to this sound alone, before external modifiers.
                Real BaseVolume;
                /// @internal
                /// @brief The minimum volume this sound is allowed to reach, regardless of other settings.
                Real MinVolume;
                /// @internal
                /// @brief The maximum volume this sound is allowed to reach, regardless of other settings.
                Real MaxVolume;
                /// @internal
                /// @brief Handle to the internal OALS source representing this sound.
                UInt32 InternalSource;
                /// @internal
                /// @brief This is an identifier containing the identity of this sound.
                String SoundName;
                /// @internal
                /// @brief This stores the handles to all the OALS buffers owned by this sound.
                BufferContainer Buffers;
                /// @internal
                /// @brief This stores the effects currently being applied to audio emitted by this sound.
                EffectContainer Effects;

                /// @internal
                /// @brief Makes the context this listener belongs to the current context.
                void MakeCurrent();
                /// @internal
                /// @brief Streams the content from the decoder into an OALS buffer.
                bool StreamToBuffer(const UInt32 Buffer);
            public:
                /// @brief Class constructor.
                /// @param Type The type of sound to initialize.  See @ref Audio::SoundType enum for the basic values.
                /// @param Name The name to be given to this sound.
                /// @param Decode A pointer to the decoder assigned to this sound.
                /// @param PlayContext A pointer to the context responsible for decoder audio streams for this sound.
                Sound(const UInt16 Type, const String& Name, iDecoder* Decode, ALCcontext* PlayContext);
                /// @brief Class destructor.
                virtual ~Sound();

                ///////////////////////////////////////////////////////////////////////////////
                // Utility

                /// @copydoc iSound::GetName() const
                virtual const String& GetName() const;
                /// @copydoc iPlaybackable::GetType() const
                virtual UInt16 GetType() const;
                /// @copydoc iPlaybackable::GetDecoder() const
                virtual iDecoder* GetDecoder() const;
                /// @copydoc iPlaybackable::SetPitch(const Real Pitch)
                virtual void SetPitch(const Real Pitch);
                /// @copydoc iPlaybackable::GetPitch() const
                virtual Real GetPitch() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Playback

                /// @copydoc iPlaybackable::Play()
                virtual bool Play();
                /// @copydoc iPlaybackable::IsPlaying() const
                virtual bool IsPlaying() const;
                /// @copydoc iPlaybackable::Pause()
                virtual void Pause();
                /// @copydoc iPlaybackable::IsPaused() const
                virtual bool IsPaused() const;
                /// @copydoc iPlaybackable::Stop()
                virtual void Stop();
                /// @copydoc iPlaybackable::IsStopped() const
                virtual bool IsStopped() const;
                /// @copydoc iPlaybackable::Loop(bool ToLoop)
                virtual void Loop(bool ToLoop);
                /// @copydoc iPlaybackable::IsLooping() const
                virtual bool IsLooping() const;

                /// @copydoc iPlaybackable::Seek(const Real Seconds, bool Relative)
                virtual bool Seek(const Real Seconds, bool Relative = false);

                ///////////////////////////////////////////////////////////////////////////////
                // Volume Control

                /// @copydoc iPlaybackable::GetVolume() const
                virtual Real GetVolume() const;
                /// @copydoc iPlaybackable::SetBaseVolume(const Real Base)
                virtual void SetBaseVolume(const Real Base);
                /// @copydoc iPlaybackable::GetBaseVolume() const
                virtual Real GetBaseVolume() const;
                /// @copydoc iPlaybackable::SetMinVolume(const Real MinVolume)
                virtual void SetMinVolume(const Real MinVol);
                /// @copydoc iPlaybackable::GetMinVolume() const
                virtual Real GetMinVolume() const;
                /// @copydoc iPlaybackable::SetMaxVolume(const Real MaxVolume)
                virtual void SetMaxVolume(const Real MaxVol);
                /// @copydoc iPlaybackable::GetMaxVolume() const
                virtual Real GetMaxVolume() const;

                ///////////////////////////////////////////////////////////////////////////////
                // Effects Methods

                /// @copydoc iPlaybackable::AttachEffect(const UInt32 Slot, iEffect* Eff)
                virtual bool AttachEffect(const UInt32 Slot, iEffect* Eff);
                /// @copydoc iPlaybackable::GetEffect(const UInt32 Slot) const
                virtual iEffect* GetEffect(const UInt32 Slot) const;
                /// @copydoc iPlaybackable::GetNumEffectSlotsAvailable() const
                virtual UInt32 GetNumEffectSlotsAvailable() const;
                /// @copydoc iPlaybackable::RemoveEffect(const UInt32 Slot)
                virtual void RemoveEffect(const UInt32 Slot);

                ///////////////////////////////////////////////////////////////////////////////
                // Filter Methods

                /// @copydoc iPlaybackable::AttachFilter(iFilter* Fil)
                virtual bool AttachFilter(iFilter* Fil);
                /// @copydoc iPlaybackable::GetFilter() const
                virtual iFilter* GetFilter() const;
                /// @copydoc iPlaybackable::RemoveFilter()
                virtual void RemoveFilter();

                ///////////////////////////////////////////////////////////////////////////////
                // Internal Methods

                /// @internal
                /// @brief Updates all the necessary components of this sound.
                virtual void _Update();
                /// @internal
                /// @brief Updates the buffers for this sound as necessary for seemless playback.
                /// @return Returns true of there were no errors updating, false if there were.
                virtual bool _UpdateBuffers();
            };//Sound
        }//OALS
    }//Audio
}//Mezzanine

#endif

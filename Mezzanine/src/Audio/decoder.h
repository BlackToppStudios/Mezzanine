// © Copyright 2010 - 2014 BlackTopp Studios Inc.
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
// Copyright (c) 2008-2010 Raynaldo (Wildicv) Rivera, Joshua (Dark_Kilauea) Jones
// This file is part of the "cAudio Engine"
// For conditions of distribution and use, see copyright notice in cAudio-ZLIBLicense.txt
#ifndef _audiodecoder_h
#define _audiodecoder_h

#include "datatypes.h"
#include "Audio/audioenumerations.h"

#include "Resource/datastream.h"

namespace Mezzanine
{
    namespace Audio
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is an interface class for the decoding of audio from a stream.
        /// @details
        ///////////////////////////////////////
        class iDecoder
        {
        public:
            /// @brief Class constructor.
            iDecoder() {  }
            /// @brief Class destructor.
            virtual ~iDecoder() {  }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Gets whether or not the decoder is ready to be used.
            /// @note On failure the issue likely lies with the stream being an improper format/encoding.
            /// @return Returns true if this decoder is ready for playback, false otherwise.
            virtual bool IsValid() = 0;
            /// @brief Gets the encoding supported by this decoder.
            /// @return Returns an @ref Audio::Encoding value representing the encoding supported by this decoder.
            virtual Audio::Encoding GetEncoding() const = 0;
            /// @brief Gets whether or not seeking is supported.
            /// @return Returns true if you can skip to a specific point in the stream, false if you are stuck waiting.
            virtual bool IsSeekingSupported() = 0;
            /// @brief Gets the Bit Configuration used to decode the audio stream.
            /// @return Returns the Bit Configuration currently being used to decode this stream.
            virtual Audio::BitConfig GetBitConfiguration() const = 0;
            /// @brief Gets the frequency used to decode the audio stream.
            /// @return Returns the frequency (or sample rate) currently being used to decode this stream.
            virtual UInt32 GetFrequency() const = 0;
            /// @brief Gets the stream being decoded.
            /// @return Returns a shared pointer to the DataStream being decoded.
            virtual Resource::DataStreamPtr GetStream() const = 0;

            /// @brief Gets the sample size based on the decoders current configuration.
            /// @return Returns a UInt32 representing the size of a single sample from the underlying stream.
            virtual UInt32 GetSampleSize() const
            {
                switch(this->GetBitConfiguration())
                {
                    case Mezzanine::Audio::BC_8Bit_Mono:     return 1;  break;
                    case Mezzanine::Audio::BC_8Bit_Stereo:   return 2;  break;
                    case Mezzanine::Audio::BC_16Bit_Mono:    return 2;  break;
                    case Mezzanine::Audio::BC_16Bit_Stereo:  return 4;  break;
                    case Mezzanine::Audio::BC_24Bit_Mono:    return 3;  break;
                    case Mezzanine::Audio::BC_24Bit_Stereo:  return 6;  break;
                    default: return -1;
                }
            }

            /// @brief Sets the position (in bytes) of the stream.
            /// @param Position The number of bytes to move(if relative) or the actual position in the stream to set.
            /// @param Relative Whether or not to move from the current position.  If false this will set from the beginning.
            /// @return Returns true if the position was successfully set, false otherwise.
            virtual bool SetPosition(Int32 Position, bool Relative) = 0;
            /// @brief Moves the current time position in the stream.
            /// @param Seconds The position in seconds to move to in the stream.
            /// @param Relative Whether or not to move from the current position.  If false this will seek from the beginning.
            /// @return Returns true if the position was successfully moved, false otherwise.
            virtual bool Seek(const Real Seconds, bool Relative) = 0;

            /// @brief Reads from the audio stream and writes what is read to a buffer.
            /// @param Output The buffer to write to when reading the audio stream.
            /// @param Amount The number of bytes desired to be read from the audio stream.
            /// @return Returns the number of bytes successfully read from the audio stream.
            virtual UInt32 ReadAudioData(void* Output, UInt32 Amount) = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Stream Stats

            /// @brief Gets the length of the stream in seconds.
            /// @return Returns the total amount of time needed to playback the sound in seconds.
            virtual Real GetTotalTime() const = 0;
            /// @brief Gets the current time position in the stream.
            /// @return Returns the current position in the stream in seconds.
            virtual Real GetCurrentTime() const = 0;
            /// @brief Gets the size of the decoded audio source in use.
            /// @return Returns the size of the decoded audio source.
            virtual UInt32 GetTotalSize() const = 0;
            /// @brief Gets the size of the encoded audio source in use.
            /// @return Returns the size of the encoded audio source.
            virtual UInt32 GetCompressedSize() const = 0;
            /// @brief Gets the sounds current position in the decoded audio source.
            /// @return Returns the current position in the decoded audio source in bytes.
            virtual UInt32 GetCurrentPosition() const = 0;
            /// @brief Gets the sounds current position in the encoded audio source.
            /// @return Returns the current position in the encoded audio source in bytes.
            virtual UInt32 GetCurrentCompressedPosition() const = 0;
        };//iDecoder
    }//Audio
}//Mezzanine

#endif

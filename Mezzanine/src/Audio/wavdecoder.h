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
#ifndef _audiowavdecoder_h
#define _audiowavdecoder_h

#ifdef ENABLE_WAV_ENCODE

#include "Audio/decoder.h"

namespace Mezzanine
{
    namespace Audio
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a @ref iDecoder implementation for the wav encoding.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB WavDecoder : public iDecoder
        {
        protected:
            /// @internal
            /// @brief This is a shared pointer to the stream being decoded.
            Resource::DataStreamPtr WavStream;
            /// @internal
            /// @brief Tracks and stores the validity of the Wav stream.
            bool Valid;
            /// @internal
            /// @brief The number of channels in this wav file.
            UInt16 Channels;
            /// @internal
            /// @brief The byte alignment of the waveform data in this wav file.
            UInt16 BlockAlign;
            /// @internal
            /// @brief The size of each sample in this wav file.
            UInt16 BitsPerSample;
            /// @internal
            /// @brief The frequency of the wav file.
            Int32 SampleRate;
            /// @internal
            /// @brief Redundant wav data expressing the rate of platback for the wav file.
            Int32 ByteRate;
            /// @internal
            /// @brief The end point of the audio in the wav file.
            Int32 DataSize;
            /// @internal
            /// @brief The start point of the audio in the wav file.
            Int32 DataOffset;
        public:
            /// @brief Class constructor.
            /// @param Stream The stream to decode.
            WavDecoder(Resource::DataStreamPtr Stream);
            /// @brief Class destructor.
            virtual ~WavDecoder();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc iDecoder::IsValid()
            virtual bool IsValid();
            /// @copydoc iDecoder::GetEncoding() const
            virtual Audio::Encoding GetEncoding() const;
            /// @copydoc iDecoder::IsSeekingSupported()
            virtual bool IsSeekingSupported();
            /// @copydoc iDecoder::GetBitConfiguration() const
            virtual Audio::BitConfig GetBitConfiguration() const;
            /// @copydoc iDecoder::GetFrequency() const
            virtual UInt32 GetFrequency() const;
            /// @copydoc iDecoder::GetStream() const
            virtual Resource::DataStreamPtr GetStream() const;

            /// @copydoc iDecoder::SetPosition(Int32 Position, bool Relative)
            virtual bool SetPosition(Int32 Position, bool Relative);
            /// @copydoc iDecoder::Seek(const Real Seconds, bool Relative)
            virtual bool Seek(const Real Seconds, bool Relative);

            /// @copydoc iDecoder::ReadAudioData(void* Output, UInt32 Amount)
            virtual UInt32 ReadAudioData(void* Output, UInt32 Amount);

            ///////////////////////////////////////////////////////////////////////////////
            // Stream Stats

            /// @copydoc iDecoder::GetTotalTime() const
            virtual Real GetTotalTime() const;
            /// @copydoc iDecoder::GetCurrentTime() const
            virtual Real GetCurrentTime() const;
            /// @copydoc iDecoder::GetTotalSize() const
            virtual UInt32 GetTotalSize() const;
            /// @copydoc iDecoder::GetCompressedSize() const
            virtual UInt32 GetCompressedSize() const;
            /// @copydoc iDecoder::GetCurrentPosition() const
            virtual UInt32 GetCurrentPosition() const;
            /// @copydoc iDecoder::GetCurrentCompressedPosition() const
            virtual UInt32 GetCurrentCompressedPosition() const;
        };//WavDecoder
    }//Audio
}//Mezzanine

#endif //ENABLE_WAV_ENCODE

#endif

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
#ifndef _audiovorbisdecoder_h
#define _audiovorbisdecoder_h

#ifdef ENABLE_VORBIS_ENCODE

#include "Audio/decoder.h"

namespace Mezzanine
{
    namespace Audio
    {
        class VorbisDecoderInternalData;
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a @ref iDecoder implementation for the vorbis encoding.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB VorbisDecoder : public iDecoder
        {
        protected:
            /// @internal
            /// @brief A pointer to all the internal data needed for working with the vorbis library.
            VorbisDecoderInternalData* VDID;
            /// @internal
            /// @brief This is a shared pointer to the stream being decoded.
            Resource::DataStreamPtr VorbisStream;
            /// @internal
            /// @brief Tracks and stores the validity of the Vorbis stream.
            Boole Valid;
        public:
            /// @brief Class constructor.
            /// @param Stream The stream to decode.
            VorbisDecoder(Resource::DataStreamPtr Stream);
            /// @brief Class destructor.
            virtual ~VorbisDecoder();

            ///////////////////////////////////////////////////////////////////////////////
            // Additional Vorbis Functionality

            /// @brief Gets a vorbis file comment by index.
            /// @param Index The index of the vorbis file comment to retrieve.
            /// @return Returns a string containing the vorbis file comment at the specified index.
            virtual String GetUserComment(const UInt32 Index);
            /// @brief Gets the number of user comments that have been inserted into this vorbis file.
            /// @return Returns the number of user comments in this vorbis file.
            virtual UInt32 GetNumUserComments() const;

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @copydoc iDecoder::IsValid()
            virtual Boole IsValid();
            /// @copydoc iDecoder::GetEncoding() const
            virtual Audio::Encoding GetEncoding() const;
            /// @copydoc iDecoder::IsSeekingSupported()
            virtual Boole IsSeekingSupported();
            /// @copydoc iDecoder::GetBitConfiguration() const
            virtual Audio::BitConfig GetBitConfiguration() const;
            /// @copydoc iDecoder::GetFrequency() const
            virtual UInt32 GetFrequency() const;
            /// @copydoc iDecoder::GetStream() const
            virtual Resource::DataStreamPtr GetStream() const;

            /// @copydoc iDecoder::SetPosition(Int32 Position, Boole Relative)
            virtual Boole SetPosition(Int32 Position, Boole Relative);
            /// @copydoc iDecoder::Seek(const Real Seconds, Boole Relative)
            virtual Boole Seek(const Real Seconds, Boole Relative);

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
        };//VorbisDecoder
    }//Audio
}//Mezzanine

#endif //ENABLE_VORBIS_ENCODE

#endif

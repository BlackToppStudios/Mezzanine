// © Copyright 2010 - 2013 BlackTopp Studios Inc.
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
#ifndef _audiorawdecoderfactory_h
#define _audiorawdecoderfactory_h

#ifdef ENABLE_RAW_ENCODE

#include "Audio/decoderfactory.h"
#include "Audio/rawdecoder.h"

namespace Mezzanine
{
    namespace Audio
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a @ref iDecoderFactory implementation for un-encoded data.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB RawDecoderFactory : public iDecoderFactory
        {
        protected:
        public:
            /// @brief Class constructor.
            RawDecoderFactory() {  }
            /// @brief Class destructor.
            virtual ~RawDecoderFactory() {  }

            /// @copydoc iDecoderFactory::CreateDecoder(Resource::DataStreamPtr)
            virtual iDecoder* CreateDecoder(Resource::DataStreamPtr Stream)
                { return new RawDecoder(Stream); }
            /// @copydoc iDecoderFactory::CreateDecoder(Resource::DataStreamPtr)
            /// @param Freq The frequency of the audio being decoded.
            /// @param Config The bit configuration of the audio being decoded.
            virtual iDecoder* CreateDecoder(Resource::DataStreamPtr Stream, const UInt32 Freq, const Audio::BitConfig Config)
                { return new RawDecoder(Stream,Freq,Config); }
            /// @copydoc iDecoderFactory::GetSupportedEncoding() const
            virtual Audio::Encoding GetSupportedEncoding() const
                { return Audio::Enc_RAW; }
        };//RawDecoderFactory
    }//Audio
}//Mezzanine

#endif //ENABLE_RAW_ENCODE

#endif

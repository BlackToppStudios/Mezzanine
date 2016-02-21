// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef _audiorawdecoder_cpp
#define _audiorawdecoder_cpp

#ifdef ENABLE_RAW_ENCODE

#include "Audio/rawdecoder.h"

namespace Mezzanine
{
    namespace Audio
    {
        RawDecoder::RawDecoder(Resource::DataStreamPtr Stream, const UInt32 Freq, const Audio::BitConfig Config) :
            RawStream(Stream),
            RawStreamSize(0),
            RawStreamPos(0),
            Frequency(Freq),
            BitConfiguration(Config)
        {
            this->RawStream->seekg(0,std::ios_base::end);
            this->RawStreamSize = this->RawStream->tellg();
            this->RawStream->seekg(0);
        }

        RawDecoder::~RawDecoder()
            {  }

        void RawDecoder::ClearStreamErrors()
        {
            if( this->RawStream->eof() ) {
                this->RawStream->clear( this->RawStream->rdstate() ^ ( std::ios::eofbit | std::ios::failbit ) );
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Boole RawDecoder::IsValid()
        {
            return ( RawStream );
        }

        Audio::Encoding RawDecoder::GetEncoding() const
        {
            return Audio::Enc_RAW;
        }

        Boole RawDecoder::IsSeekingSupported()
        {
            return true;
        }

        Audio::BitConfig RawDecoder::GetBitConfiguration() const
        {
            return this->BitConfiguration;
        }

        UInt32 RawDecoder::GetFrequency() const
        {
            return this->Frequency;
        }

        Resource::DataStreamPtr RawDecoder::GetStream() const
        {
            return this->RawStream;
        }

        Boole RawDecoder::IsEndOfStream() const
        {
            return ( this->RawStream->eof() || this->RawStream->tellg() >= this->RawStreamSize );
        }

        Boole RawDecoder::SetPosition(Int32 Position, const Boole Relative)
        {
            this->RawStream->seekg(Position,( Relative ? std::ios_base::cur : std::ios_base::beg ));
            return true;
        }

        Boole RawDecoder::Seek(const Real Seconds, const Boole Relative)
        {
            Int32 Pos = static_cast<Int32>( Seconds * static_cast<Real>(this->Frequency) * static_cast<Real>( this->GetSampleSize() ) );
            return this->SetPosition(Pos,Relative);
        }

        UInt32 RawDecoder::ReadAudioData(void* Output, UInt32 Amount)
        {
            this->RawStream->read(static_cast<char*>(Output),Amount);
            return this->RawStream->gcount();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Stream Stats

        Real RawDecoder::GetTotalTime() const
            { return static_cast<Real>( this->RawStreamSize ) / ( static_cast<Real>(this->Frequency) * static_cast<Real>( this->GetSampleSize() ) ); }

        Real RawDecoder::GetCurrentTime() const
            { return static_cast<Real>( this->RawStream->tellg() ) / ( static_cast<Real>(this->Frequency) * static_cast<Real>( this->GetSampleSize() ) ); }

        UInt32 RawDecoder::GetTotalSize() const
            { return this->RawStreamSize; }

        UInt32 RawDecoder::GetCompressedSize() const
            { return this->RawStreamSize; }

        UInt32 RawDecoder::GetCurrentPosition() const
            { return this->RawStream->tellg(); }

        UInt32 RawDecoder::GetCurrentCompressedPosition() const
            { return this->RawStream->tellg(); }
    }//Audio
}//Mezzanine

#endif //ENABLE_RAW_ENCODE

#endif

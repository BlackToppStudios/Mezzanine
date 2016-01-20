// © Copyright 2010 - 2015 BlackTopp Studios Inc.
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
#ifndef _audiovorbisdecoder_cpp
#define _audiovorbisdecoder_cpp

#ifdef ENABLE_VORBIS_ENCODE

#include "Audio/vorbisdecoder.h"

#include "Resource/datastream.h"

#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

namespace
{
    /// @internal
    /// @brief Convenience method to check for and clear EoF flags that may be encountered during decoding.
    void ClearEoF(Mezzanine::Resource::DataStream* Stream)
    {
        if( Stream->eof() ) {
            Stream->clear( Stream->rdstate() ^ ( std::ios::eofbit | std::ios::failbit ) );
        }
    }

    /// @internal
    /// @brief The Vorbis read callback.
    size_t VorbisRead(void *ptr, size_t byteSize,size_t sizeToRead, void *datasource)
    {
        Mezzanine::Resource::DataStream* Stream = static_cast<Mezzanine::Resource::DataStream*>(datasource);
        Stream->read( static_cast<char*>(ptr), byteSize * sizeToRead );
        return Stream->gcount();
    }

    /// @internal
    /// @brief The Vorbis seek callback (set position).
    int VorbisSeek(void *datasource,ogg_int64_t offset,int whence)
    {
        Mezzanine::Resource::DataStream* Stream = static_cast<Mezzanine::Resource::DataStream*>(datasource);
        ClearEoF(Stream);
        switch(whence)
        {
            case SEEK_SET:    Stream->seekg(offset,std::ios_base::beg);    break;
            case SEEK_CUR:    Stream->seekg(offset,std::ios_base::cur);    break;
            case SEEK_END:    Stream->seekg(offset,std::ios_base::end);    break;
        };
        return ( Stream->good() ? 0 : -1 );
    }

    /// @internal
    /// @brief The Vorbis tell callback (retrieve position).
    long VorbisTell(void *datasource)
    {
        Mezzanine::Resource::DataStream* Stream = static_cast<Mezzanine::Resource::DataStream*>(datasource);
        ClearEoF(Stream);
        return Stream->tellg();
    }

    /// @internal
	/// @brief The stream close callback.
	int VorbisClose(void *datasource)
	{
		return 0;
	}
}

namespace Mezzanine
{
    namespace Audio
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @brief Internal convenience class for the storage of Vorbis structs needed for Vorbis operations.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB VorbisDecoderInternalData
        {
        protected:
        public:
            ///////////////////////////////////////////////////////////////////////////////
            // Publid Data Members

            /// @brief Format Information from the file.
            vorbis_info* VorbisInfo;
            /// @brief User comments embedded in the file.
            vorbis_comment* VorbisComments;
            /// @brief Mandatory callbacks for working with our streams.
            ov_callbacks VorbisCallbacks;
            /// @brief The main audio encoding.
            OggVorbis_File VorbisFile;

            ///////////////////////////////////////////////////////////////////////////////
            // Construction and destruction

            /// @brief Class constructor.
            VorbisDecoderInternalData()
            {
                this->VorbisCallbacks.read_func = VorbisRead;
                this->VorbisCallbacks.seek_func = VorbisSeek;
                this->VorbisCallbacks.tell_func = VorbisTell;
                this->VorbisCallbacks.close_func = VorbisClose;
            }
            /// @brief Class destructor.
            ~VorbisDecoderInternalData() {  }
        };//VorbisDecoderInternalData

        ///////////////////////////////////////////////////////////////////////////////
        // VorbisDecoder Methods

        VorbisDecoder::VorbisDecoder(Resource::DataStreamPtr Stream) :
            VorbisStream(Stream),
            VorbisStreamSize(0),
            VorbisStreamPos(0),
            Valid(false)
        {
            this->VorbisStream->seekg(0,std::ios_base::end);
            this->VorbisStreamSize = this->VorbisStream->tellg();
            this->VorbisStream->seekg(0);
            this->VDID = new VorbisDecoderInternalData();
            this->Valid = ( ov_open_callbacks(VorbisStream.get(),&(this->VDID->VorbisFile),NULL,0,this->VDID->VorbisCallbacks) == 0 );

            if( this->Valid ) {
                this->VDID->VorbisInfo = ov_info( &(this->VDID->VorbisFile), -1 );
                this->VDID->VorbisComments = ov_comment( &(this->VDID->VorbisFile), -1 );
                this->VorbisStreamPos = this->VorbisStream->tellg();
            }
        }

        VorbisDecoder::~VorbisDecoder()
        {
            ov_clear( &(this->VDID->VorbisFile) );
        }

        void VorbisDecoder::ClearStreamErrors()
        {
            if( this->VorbisStream->eof() ) {
                this->VorbisStream->clear( this->VorbisStream->rdstate() ^ ( std::ios::eofbit | std::ios::failbit ) );
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Additional Vorbis Functionality

        String VorbisDecoder::GetUserComment(const UInt32 Index)
        {
            Char8* Comment = this->VDID->VorbisComments->user_comments[Index];
            Integer CommentLength = this->VDID->VorbisComments->comment_lengths[Index];
            return String(Comment,CommentLength);
        }

        UInt32 VorbisDecoder::GetNumUserComments() const
        {
            return this->VDID->VorbisComments->comments;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        Boole VorbisDecoder::IsValid()
        {
            return this->Valid;
        }

        Audio::Encoding VorbisDecoder::GetEncoding() const
        {
            return Audio::Enc_VORBIS;
        }

        Boole VorbisDecoder::IsSeekingSupported()
        {
            if( this->Valid ) return (ov_seekable( &(this->VDID->VorbisFile) ) != 0);
            else return false;
        }

        Audio::BitConfig VorbisDecoder::GetBitConfiguration() const
        {
            if( this->Valid ) {
                switch( this->VDID->VorbisInfo->channels )
                {
                    case 1:  return Audio::BC_16Bit_Mono;    break;
                    case 2:  return Audio::BC_16Bit_Stereo;  break;
                }
            }

            return Audio::BC_8Bit_Mono;
        }

        UInt32 VorbisDecoder::GetFrequency() const
        {
            if( this->Valid ) return this->VDID->VorbisInfo->rate;
            else return 0;
        }

        Resource::DataStreamPtr VorbisDecoder::GetStream() const
        {
            return this->VorbisStream;
        }

        Boole VorbisDecoder::IsEndOfStream() const
        {
            return ( this->VorbisStream->eof() || this->VorbisStream->tellg() >= this->VorbisStreamSize );
        }

        Boole VorbisDecoder::SetPosition(Int32 Position, const Boole Relative)
        {
            if( this->IsSeekingSupported() ) {
                if( Relative ) {
                    Real CurrPos = ov_raw_tell( &(this->VDID->VorbisFile) );
                    if( ov_raw_seek( &(this->VDID->VorbisFile), CurrPos + Position ) == 0 ) {
                        this->VorbisStreamPos = this->VorbisStream->tellg();
                        return true;
                    }
                }else{
                    if( ov_raw_seek( &(this->VDID->VorbisFile), Position ) == 0 ) {
                        this->VorbisStreamPos = this->VorbisStream->tellg();
                        return true;
                    }
                }
            }
            return false;
        }

        Boole VorbisDecoder::Seek(const Real Seconds, const Boole Relative)
        {
            if( this->IsSeekingSupported() ) {
                if( Relative ) {
                    Real CurrTime = ov_time_tell( &(this->VDID->VorbisFile) );
                    if( ov_time_seek( &(this->VDID->VorbisFile), CurrTime + Seconds ) == 0 ) {
                        this->VorbisStreamPos = this->VorbisStream->tellg();
                        return true;
                    }
                }else{
                    if( ov_time_seek( &(this->VDID->VorbisFile), Seconds ) == 0 ) {
                        this->VorbisStreamPos = this->VorbisStream->tellg();
                        return true;
                    }
                }
            }
            return false;
        }

        UInt32 VorbisDecoder::ReadAudioData(void* Output, UInt32 Amount)
        {
            if( this->Valid ) {
                if( this->VorbisStreamPos != this->VorbisStream->tellg() ) {
                    this->ClearStreamErrors();
                    this->VorbisStream->seekg( this->VorbisStreamPos );
                }
                Integer Temp = 0;
                Integer Ret = ov_read( &(this->VDID->VorbisFile), (Char8*)Output, Amount, 0, 2, 1, &Temp );
                this->VorbisStreamPos = this->VorbisStream->tellg();
                return ( Ret > 0 ? Ret : 0 );
            }else{
                return 0;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Stream Stats

        Real VorbisDecoder::GetTotalTime() const
            { return ov_time_total( &(this->VDID->VorbisFile), -1 ); }

        Real VorbisDecoder::GetCurrentTime() const
            { return ov_time_tell( &(this->VDID->VorbisFile) ); }

        UInt32 VorbisDecoder::GetTotalSize() const
            { return ov_pcm_total( &(this->VDID->VorbisFile), -1 ) * this->VDID->VorbisInfo->channels; }

        UInt32 VorbisDecoder::GetCompressedSize() const
            { return ov_raw_total( &(this->VDID->VorbisFile), -1 ); }

        UInt32 VorbisDecoder::GetCurrentPosition() const
            { return ov_pcm_tell( &(this->VDID->VorbisFile) ) * this->VDID->VorbisInfo->channels; }

        UInt32 VorbisDecoder::GetCurrentCompressedPosition() const
            { return ov_raw_tell( &(this->VDID->VorbisFile) ); }
    }//Audio
}//Mezzanine

#endif //ENABLE_VORBIS_ENCODE

#endif

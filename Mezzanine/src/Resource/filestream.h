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
#ifndef _resourcefilestream_h
#define _resourcefilestream_h

#include "Resource/datastream.h"

/// @file
/// @brief Declaration of FileStream

namespace Mezzanine
{
    namespace Resource
    {
#ifdef USENEWDATASTREAM
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This represents a stream to a file on disk using the C++ file stream API.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB FileStream : public std::fstream, public DataStream
        {
            public:
                /// @brief Class constructor.
                FileStream();
                /// @brief Class destructor.
                ~FileStream();
        };//FileStream
#else //USENEWDATASTREAM
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This represents a stream to a file on disk using the C++ file stream API.
        /// @details
        ///////////////////////////////////////
        class MEZZ_LIB FileStream : public DataStream
        {
            protected:
                std::fstream* StandardStream;
            public:
                /// @brief Class constructor.
                /// @param Stream The stream to the file being streamed to/from.
                /// @param Mode The flags to use when initializing the stream.
                FileStream(std::fstream* Stream, const UInt16 Flags = DataStream::SF_Read);
                /// @brief Self loading constructor.
                /// @param FileName The name of the file to be loaded.  This will also become the name of the stream.
                /// @param Path The path to the file being loaded.
                /// @param Mode The flags to use when initializing the stream.
                FileStream(const String& FileName, const String& Path, const UInt16 Flags = DataStream::SF_Read);
                /// @brief Class destructor.
                virtual ~FileStream();

                ///////////////////////////////////////////////////////////////////////////////
                // Stream Access and Manipulation

                /// @copydoc DataStream::Read(void* Buffer, const size_t& Count)
                virtual size_t Read(void* Buffer, const size_t& Count);
                /// @copydoc DataStream::Write(const void* Buffer, const size_t& Count)
                virtual size_t Write(const void* Buffer, const size_t& Count);

                /// @copydoc DataStream::Advance(const StreamOff Count)
                virtual void Advance(const StreamOff Count);
                /// @copydoc DataStream::SetStreamPosition(StreamPos Position)
                virtual void SetStreamPosition(StreamPos Position);
                /// @copydoc DataStream::SetStreamPosition(StreamOff Offset, SeekOrigin Origin)
                virtual void SetStreamPosition(StreamOff Offset, SeekOrigin Origin);
                /// @copydoc DataStream::GetStreamPosition(Boole Read = true)
                virtual StreamPos GetStreamPosition(Boole Read = true);
                /// @copydoc DataStream::EoF() const
                virtual Boole EoF() const;
                /// @copydoc DataStream::Close()
                virtual void Close();
        };//FileStream
#endif //USENEWDATASTREAM
    }//Resource
}//Mezzanine

#endif

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
#ifndef _ogredatastreambuf_h_cpp
#define _ogredatastreambuf_h_cpp

// Keeps this file from being documented by doxygen
/// @cond DontDocumentInternal

#include "entresol.h"

#include <streambuf>

#include <Ogre.h>

namespace Mezzanine
{
    namespace Internal
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @internal
        /// @class OgreDataStreamBuf
        /// @headerfile ogredatastreambuf.h.cpp
        /// @brief Used in the conversion of Ogre::DataStream into std::stream
        /// @details
        ///////////////////////////////////////
        class OgreDataStreamBuf : public std::streambuf
        {
            protected:
                /// @brief a shard_ptr to the internal Ogre Datastream
                Ogre::DataStreamPtr OgreStream;

                /// @brief The size of chunks to grab from files at once defaults to 4Kb
                std::streampos LoadAtOnce;

                /// @brief The ammount of bytes that should be load before a given start point when loading.
                /// @details When read from a data source, the amount of bytes set in LoadAtOnce will be loaded.
                /// By default when loading a certain item this will start at the specified point and load the
                /// file after that point. This says howmany bytes should be loaded before that default starting
                /// point. If all streams are simply read, then this should be 0, if there are plans to manuever around
                /// the stream, then this should be higher, based on how far back you plan on reading. By default
                /// this assumes a low value of 128 bytes.
                std::streampos SeekBackOnload;

                /// @brief The Maximum percentage of the buffer to be filled with seekback contents
                /// @details Whenever SeekBackOnload is use this value will be used to determine if too much of the buffer is being used.
                /// This defaults to 50%
                Real MaxSeekBack;

                /// @brief The message to use in pre bounds exception errors
                String BeforeStreamError;

                /// @brief The message to use in Past bounds exceptions
                String AfterStreamError;

                ///////////////////////////////////////////////////////////////////////////////
                // Methods inherited form std::streambuf
                ///////////////////////////////////////

                // Locales
                /// @brief Does nothing, implemented for ease of future coding
                /// @param loc A completely ignored value.
                void imbue ( const std::locale & loc )
                    {}

                //Buffer mgmt and position
                /// @brief Creates the initial buffer used to begin
                /// @param s The Place to put the buffer
                /// @param n The size of the new buffer
                /// @return Returns this on success, of 0/NULL on failure
                /// @exception This can throw out of memory exceptions
                virtual std::streambuf* setbuf (char* s, std::streamsize n);

                /// @brief Moves the internal pointer around
                /// @param off the ammount to move the pointer.
                /// @param way from the begining, from the end, or from the current point
                /// @param which this only supports ios_base::in, and will not work with ios_base::out
                virtual std::streampos seekoff ( std::streamoff off, std::ios_base::seekdir way, std::ios_base::openmode which = std::ios_base::in );

                /// @brief Moves internal pointer to specificied.
                /// @param sp The place to move the pointer too.
                /// @param which this only supports ios_base::in, and will not work with ios_base::out.
                virtual std::streampos seekpos ( std::streampos sp, std::ios_base::openmode which = std::ios_base::in );

                /// @brief Does nothing, but is required and returns success
                /// @details due to the nature of the ogre data streams this does not need to and syncing on the fly is extremely easy.
                virtual int sync()
                    {return 0;}

                //Input Functions
                /// @brief Should get the amount of characters left in the sequence
                /// @returns -1 if no estimate could be made, other wise this returns an estimate of the amount of bytes in the buffer
                virtual std::streamsize showmanyc();

                /// @brief Gets a sequence of characters
                /// @param s a Pointer to where the characters should go
                /// @param n How many characters
                /// @return This returns the amount of characters retrieved
                virtual std::streamsize xsgetn(char* s, std::streamsize n);

                /// @brief Can in theory be called by read operations, but probably wont because of our xsgetn implemention
                /// @return always returns EOF
                virtual int underflow();

                // Default implmentation is ideal
                // @brief Calls underflow()
                // @return whatever underflow() returns.
                //virtual int uflow();

                /// @brief Someone rewinded too far and now we need to backload the buffer
                /// @param c Ignored, required for compatibility
                /// @return EOF on failure and the current character pointer otherwise
                virtual int pbackfail ( int c = EOF );

                /// @brief Crashes, in ostream bufs it is expected to put data into the stream
                /// @param s a Pointer to the characters
                /// @param n How many characters
                /// @return This returns the amount of characters inserted
                /// @detail currently unimplimented
                virtual std::streamsize xsputn(const char_type* s, std::streamsize n);

                /// @brief Crashes, in ostreambufs, this is expected to write characters out
                /// @param c A character to write at the end of the output
                /// @return the amountof characters written
                virtual int overflow ( int c = EOF );

                ///////////////////////////////////////////////////////////////////////////////
                // Non-Virtual Methods
                ///////////////////////////////////////

                /// @brief Sets the internal buffer to the Specified location in the stream.
                /// @param Destination The place in the Stream to go to.
                /// @details This uses LoadAtOnce to determine the size of the buffer, and SeekBackOnload to determine
                /// how far into the internal buffer should be placed. This does every kind of error checking that can be
                /// for this stage of stream management/creation.
                void SetInternalBuffer(const std::streampos& Destination);

                /// @internal
                /// @brief Used to set the internal buffer with fewer assumptions.
                /// @param BeginPtr a char pointer to the begining of the internal buffer.
                /// @param BufferSize How big is the internal buffer is in bytes/chars.
                /// @param Destination Where in the stream the current pointer
                /// @details This does some error checking to attempt to determine if the default amount of backfill
                /// is appropriate. Then this calles the very detailed SetInternalBuffer to do the Dirty work.This is
                /// intended to only be called from OgreDataStreamBuf::SetInternalBuffer(streampos)
                void SetInternalBuffer(char* BeginPtr, const Whole& BufferSize, const std::streampos& Destination);

                /// @internal
                /// @brief Used to set the internal buffer without all the error checking, and in a very precise fashion.
                /// @param BeginPtr a char pointer to the begining of the internal buffer.
                /// @param BufferSize How big is the internal buffer is in bytes/chars.
                /// @param Destination Where in the stream the current pointer
                /// @param BackFill How much to load before the destination
                /// @details This is the ideal set of information for filling our mostly sized base variable size buffer. This performs almost
                /// no error checking. After being run this leaves the ogre pointer at the end of the read input. This is intended to only be
                /// called from OgreDataStreamBuf::SetInternalBuffer()
                void SetInternalBuffer(char* BeginPtr, const Whole& BufferSize, const std::streampos& Destination, const std::streampos& BackFill);

                /// @brief This checks if a given point is in the internal buffer or not
                /// @param BeginPoint This is checked to see if this is inside the buffer
                /// @param EndPoint If 0, this is ignored, otherwise this is checked if it is inside the buffer
                /// @return This returns True if BeginPoint is inside the internal buffer, and EndPoint is 0 is EndPoint is inside the buffer. Other wise this returns false
                bool CheckInternalBuffer(const std::streampos& BeginPoint, const std::streampos& EndPoint=0);

                /// @brief Check if the given points are in the stream
                /// @param BeginPoint This is checked to see if this is inside the stream
                /// @param EndPoint If 0, this is ignored, otherwise this is checked if it is inside the stream
                bool CheckStream(const Whole& BeginPoint, const Whole& EndPoint=0);
            public:

                /// @brief constructor
                /// @param Datum A pointer to the Ogre Datastream that this stream will use
                OgreDataStreamBuf(const Ogre::DataStreamPtr& Datum) : OgreStream(Datum),
                LoadAtOnce(4096), SeekBackOnload(128), MaxSeekBack(0.5),
                BeforeStreamError("Beyond Stream Bounds: Before Stream begins"),
                AfterStreamError("Beyond Stream Bounds: After end of known stream")
                {
                    #ifdef MEZZDEBUG
                    Entresol::GetSingletonPtr()->Log("Entering/Exiting OgreDataStreamBuf Constructor");
                    #endif
                }

                /// @brief Can this be read from
                /// @return A bool true if it can be read from
                bool Readable();

                /// @brief Can this be written to
                /// @return A bool true if it can be written to
                bool Writeable();

                /// @brief Where are you in the current Stream
                /// @returns The Number of bytes preceding the next byte to be read.
                Whole GetCurrentLocation();
        };
    }// /internal
}// /Mezz

/// @endcond

#endif


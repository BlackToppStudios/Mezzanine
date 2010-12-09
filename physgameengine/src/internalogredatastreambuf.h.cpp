//© Copyright 2010 BlackTopp Studios Inc.
/* This file is part of The PhysGame Engine.

    The PhysGame Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The PhysGame Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The PhysGame Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of The PhysGame anyone, including companies who wish to
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

// Keeps this file form being documented by doxygen
/// @cond 0

#include <streambuf>

#include <Ogre.h>

#define PHYSDEBUG
#ifdef PHYSDEBUG
#include "world.h"
#endif

namespace phys
{
    namespace internal
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
                Whole LoadAtOnce;

                /// @brief The ammount of bytes that should be load before a given start point when loading.
                /// @details When read from a data source, the amount of bytes set in LoadAtOnce will be loaded.
                /// By default when loading a certain item this will start at the specified point and load the
                /// file after that point. This says howmany bytes should be loaded before that default starting
                /// point. If all streams are simply read, then this should be 0, if there is manuever around the
                /// stream, then this should be higher, based on how far back you plan on reading. By default
                /// this assumes a low value of 128 bytes.
                Whole SeekBackOnload;

            public:

                /// @brief constructor
                /// @param Datum A pointer to the Ogre Datastream that this stream will use
                OgreDataStreamBuf(const Ogre::DataStreamPtr& Datum) :OgreStream(Datum), LoadAtOnce(4096), SeekBackOnload(128)
                {
                    #ifdef PHYSDEBUG
                    World::GetWorldPointer()->Log("Entering/Exiting OgreDataStreamBuf Constructor");
                    #endif
                }

                ///////////////////////////////////////////////////////////////////////////////
                // Methods inherited form std::streambuf
                ///////////////////////////////////////

                // Locales
                /// @brief Does nothing, implemented for ease of future coding
                /// @param loc A completely ignored value.
                void imbue ( const locale & loc )
                    {}

                //Buffer mgmt and position
                /// @brief Creates the initial buffer used to begin
                /// @param s The Place to put the buffer
                /// @param n The size of the new buffer
                /// @return Returns this on success, of 0/NULL on failure
                /// @exception This can throw out of memory exceptions
                virtual streambuf* setbuf (char* s, streamsize n);

                /// @brief Moves the internal pointer around
                /// @param off the ammount to move the pointer.
                /// @param way from the begining, from the end, or from the current point
                /// @param which this only supports ios_base::in, and will not work with ios_base::out
                virtual streampos seekoff ( streamoff off, ios_base::seekdir way, ios_base::openmode which = ios_base::in );

                /// @brief Moves internal pointer to specificied.
                /// @param sp The place to move the pointer too.
                /// @param which this only supports ios_base::in, and will not work with ios_base::out.
                virtual streampos seekpos ( streampos sp, ios_base::openmode which = ios_base::in );

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
                /// @brief Can this be read from
                /// @return A bool true if it can be read from
                bool Readable();

                /// @brief Can this be written to
                /// @return A bool true if it can be written to
                bool Writeable();
        };
    }// /internal
}// /phys

/// @endcond

#endif



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
#ifndef _ogredatastreambuf_cpp
#define _ogredatastreambuf_cpp

#include "internalogredatastreambuf.h.cpp"
#include "exception.h"

// Keeps this file form being documented by doxygen
/// @cond 0

namespace phys
{
    namespace internal
    {

        const String FailWriteNoAccess("Failed to write to readonly stream.");
        const String FailReadNoAccess("Failed to read from unreadable stream (could be write-only).");

        streambuf* OgreDataStreamBuf::setbuf (char* s, streamsize n)
        {
            #ifdef PHYSDEBUG
            World::GetWorldPointer()->Log("Entering OgreDataStreamBuf::setbuf()");
            #endif

            if ( 0==n ) //They want unbuffered input, well they can't have it
                { return NULL; }
            s = new char [n];
            this->OgreStream->read(s,n);

            return this;
        }

        streampos OgreDataStreamBuf::seekoff ( streamoff off, ios_base::seekdir way, ios_base::openmode which)
        {
            #ifdef PHYSDEBUG
            World::GetWorldPointer()->Log("Entering OgreDataStreamBuf::seekoff()");
            #endif
            if (ios_base::out & which)
                { return -1; }

            switch(way)
            {
                case ios_base::beg :
                    #ifdef PHYSDEBUG
                    World::GetWorldPointer()->Log("Exiting OgreDataStreamBuf::seekoff() via Begin seek logic");
                    #endif
                    return this->seekpos((streampos)off,which);

                case ios_base::cur :
                    {
                        #ifdef PHYSDEBUG
                        World::GetWorldPointer()->Log("Exiting OgreDataStreamBuf::seekoff() via current location seek logic");
                        #endif
                        Whole BufferDiff = this->egptr()-this->gptr();
                        if ( (off>0 && BufferDiff < off) || (off<0 && this->egptr()+off>this->eback() ) )                          // if the offset is still is still inside the loaded stuff
                        {
                            setg(this->eback(), this->gptr()+off, this->egptr());       //just move the pointer the amount of the offset
                            return this->OgreStream->tell()-BufferDiff;                 // and figure out how fow into the ogre stream we are less the amount of unread data
                        }else{
                            return this->seekpos(this->OgreStream->tell()-BufferDiff, which);   //Otherwise just use existing logic to fix the buffer
                        }
                    }

                case ios_base::end :
                    #ifdef PHYSDEBUG
                    World::GetWorldPointer()->Log("Exiting OgreDataStreamBuf::seekoff() via end seek logic");
                    #endif
                    if( (this->OgreStream->size()!=0) && (this->OgreStream->size()-off > 0))
                    {
                        this->seekpos(this->OgreStream->size()-off, which);
                    }else{
                        return -1;
                    }

                default:
                    return -1;
            }

        }

        streampos OgreDataStreamBuf::seekpos ( streampos sp, ios_base::openmode which )
        {
            #ifdef PHYSDEBUG
            World::GetWorldPointer()->Log("Entering OgreDataStreamBuf::seekpos()");
            #endif
            if (ios_base::in & which)
                { return -1; }

            /// @todo do some fancy math to see if we can just move the current pointer around in the buffer

            char* Buffer = new char [this->LoadAtOnce];
            this->OgreStream->seek(sp-(streampos)SeekBackOnload);
            this->OgreStream->read(Buffer, this->LoadAtOnce);

            this->setg(Buffer, Buffer+this->SeekBackOnload, Buffer+this->LoadAtOnce);
            return sp;
            #ifdef PHYSDEBUG
            World::GetWorldPointer()->Log("Exiting OgreDataStreamBuf::seekpos()");
            #endif
        }


        std::streamsize OgreDataStreamBuf::showmanyc (void)
        {
            #ifdef PHYSDEBUG
            World::GetWorldPointer()->Log("Entering OgreDataStreamBuf::showmanyc()");
            #endif
            if(0==this->OgreStream->size())
            {
                #ifdef PHYSDEBUG
                World::GetWorldPointer()->Log("Exiting OgreDataStreamBuf::showmanyc() returning -1");
                #endif
                //Ogre Docs state 0 is a special case
                return -1;
            }else{
                #ifdef PHYSDEBUG
                World::GetWorldPointer()->LogStream << "Exiting OgreDataStreamBuf::showmanyc() returning " << this->OgreStream->size();
                #endif
                return static_cast<signed int>(this->OgreStream->size());
            }
        }

        std::streamsize OgreDataStreamBuf::xsgetn(char* s, std::streamsize n)
        {
            #ifdef PHYSDEBUG
            World::GetWorldPointer()->Log("Entering/exiting OgreDataStreamBuf::xsgetn(char* s, std::streamsize n)");
            #endif

            if(this->Readable())
            {
                return this->OgreStream->read(s,n);
            }else{
                World::GetWorldPointer()->LogAndThrow(FailReadNoAccess);
            }
        }

        int OgreDataStreamBuf::underflow()
        {
            #ifdef PHYSDEBUG
            World::GetWorldPointer()->Log("Entering OgreDataStreamBuf::underflow()");
            World::GetWorldPointer()->LogStream << "\tSeekBackOnload: " << this->SeekBackOnload << endl;
            #endif

            Whole BufferSize = this->egptr() - this->eback();
            if(BufferSize < this->SeekBackOnload)
            {
                //seekback is too large
                if(2 <= this->SeekBackOnload)
                    { this->SeekBackOnload *= 0.5; }
                else
                {
                    this->SeekBackOnload = 0;
                }
                #ifdef PHYSDEBUG
                World::GetWorldPointer()->LogStream << "\tAdjusted SeekBackOnload: " << this->SeekBackOnload << endl;
                World::GetWorldPointer()->LogStream << "\tBufferSize: " << BufferSize << endl;
                World::GetWorldPointer()->Log("Exiting OgreDataStreamBuf::underflow() After adjusting SeekBackOnload");
                #endif
                if (this->SeekBackOnload)
                    { return traits_type::eof(); }
                return this->underflow();
            }else{
                if (this->OgreStream->tell() > this->SeekBackOnload )       // are we too close to the beginning of the ogre stream
                {
                    this->OgreStream->seek( this->OgreStream->tell()-this->SeekBackOnload ); //we are in the , just keep moving
                    this->OgreStream->read( this->eback(), BufferSize);
                    this->setg( this->eback(), this->eback()+this->SeekBackOnload, this->egptr() );
                }else{
                    Whole DistanceFromStreamBegin = (this->SeekBackOnload - this->OgreStream->tell());
                    this->OgreStream->seek( 0 );                            // yes we are, rewind to the beginning
                    this->OgreStream->read( this->eback(), BufferSize);
                    this->setg( this->eback(), this->eback()+DistanceFromStreamBegin, this->egptr() );
                }
                #ifdef PHYSDEBUG
                World::GetWorldPointer()->Log("Exiting OgreDataStreamBuf::underflow() After performing requested loading");
                #endif
                return Toint(this->gptr());
            }

            #ifdef PHYSDEBUG
            World::GetWorldPointer()->Log("Exiting OgreDataStreamBuf::underflow() After mysterious failure");
            #endif
            return traits_type::eof();
        }

        /*int OgreDataStreamBuf::uflow()
        {
            #ifdef PHYSDEBUG
            World::GetWorldPointer()->Log("Entering/exiting OgreDataStreamBuf::uflow()");
            #endif
            return underflow();
        }*/

        int OgreDataStreamBuf::pbackfail ( int c )
        {
            #ifdef PHYSDEBUG
            World::GetWorldPointer()->Log("Entering/exiting OgreDataStreamBuf::pbackfail()");
            #endif

            Whole BufferSize = this->egptr() - this->eback();
            if(BufferSize < this->SeekBackOnload)
            {
                if(2 <= this->SeekBackOnload)                 //seekback is too large
                    { this->SeekBackOnload *= 0.5; }
                else
                    { return traits_type::eof(); }            //seekback is too small
                #ifdef PHYSDEBUG
                World::GetWorldPointer()->Log("Exiting OgreDataStreamBuf::pbackfail() After adjusting SeekBackOnload");
                #endif
                return this->pbackfail(c);
            }else{
                if (this->OgreStream->tell() > this->SeekBackOnload )                        // are we too close to the beginning of the ogre stream
                {
                    this->OgreStream->seek( this->OgreStream->tell()-this->SeekBackOnload ); //we are in the , just keep moving
                    this->OgreStream->read( this->eback(), BufferSize);
                    this->setg( this->eback(), this->eback()+this->SeekBackOnload, this->egptr() );
                }else{
                    Whole DistanceFromStreamBegin = (this->SeekBackOnload - this->OgreStream->tell());
                    if (0==DistanceFromStreamBegin)                                         //Fail becuase we are at the beginning
                        { return traits_type::eof(); }
                    this->OgreStream->seek( 0 );                                            // yes we are, rewind to the beginning
                    this->OgreStream->read( this->eback(), BufferSize);
                    this->setg( this->eback(), this->eback()+DistanceFromStreamBegin, this->egptr() );
                }

                #ifdef PHYSDEBUG
                World::GetWorldPointer()->Log("Exiting OgreDataStreamBuf::pbackfail() After performing requested loading");
                #endif
                return Toint(this->gptr());
            }

            #ifdef PHYSDEBUG
            World::GetWorldPointer()->Log("Exiting OgreDataStreamBuf::pbackfail() After mysterious failure");
            #endif
            return traits_type::eof();
        }

        std::streamsize OgreDataStreamBuf::xsputn(const char_type* s, std::streamsize n)
        {
            #ifdef PHYSDEBUG
            World::GetWorldPointer()->Log("Entering/exiting OgreDataStreamBuf::xsputn(const char_type* s, std::streamsize n)");
            #endif
            World::GetWorldPointer()->Log("Cannot write to an Ogre::DataStream, with OgreDataStreamBuf");
            return -1;
        }

        int OgreDataStreamBuf::overflow ( int c )
        {
            #ifdef PHYSDEBUG
            World::GetWorldPointer()->Log("Entering/exiting OgreDataStreamBuf::overflow()");
            #endif
            World::GetWorldPointer()->Log("Cannot write to an Ogre::DataStream, with OgreDataStreamBuf");
            return -1;
        }

        bool OgreDataStreamBuf::Readable()
        {
            return this->OgreStream->isReadable();
        }

        bool OgreDataStreamBuf::Writeable()
        {
            return this->OgreStream->isWriteable();
        }

    }// /internal
}// /phys

/// @endcond

#endif

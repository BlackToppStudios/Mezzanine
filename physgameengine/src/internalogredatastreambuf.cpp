
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
                            //setg(this->eback(), this->gptr()+off, this->egptr());       //just move the pointer the amount of the offset
                            this->seekpos(this->OgreStream->tell()+off, which);
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
            if (this->eback()==NULL)
                { delete[] this->eback(); }
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
            World::GetWorldPointer()->LogStream << "Entering OgreDataStreamBuf::xsgetn(char* s=" << Toint(s) << ", std::streamsize n=" << n << ")"; World::GetWorldPointer()->Log();
            #endif

            Whole Current = this->GetCurrentLocation();
            Whole Destination = this->GetCurrentLocation()+n;
            if(this->CheckInternalBuffer( Current, Destination ))
            {                                                  /// @todo Do some Buffer Math instead of reloading everything we are already near. in xsgetn
                std::streamsize BytesRetrieved = this->OgreStream->read(s,n);
                this->SetInternalBuffer(Destination);
                #ifdef PHYSDEBUG
                World::GetWorldPointer()->LogStream << "Exiting OgreDataStreamBuf::xsgetn() After calling this->SetInternalBuffer(" << Destination << ") and returning" << BytesRetrieved; World::GetWorldPointer()->Log();
                #endif
                return BytesRetrieved;
            }else{
                if(this->CheckStream(Destination))
                {
                    std::streamsize BytesRetrieved = this->OgreStream->read(s,n);
                    this->SetInternalBuffer(Destination);
                    #ifdef PHYSDEBUG
                    World::GetWorldPointer()->LogStream << "Exiting OgreDataStreamBuf::xsgetn() After calling this->SetInternalBuffer(" << Destination << ") and returning" << BytesRetrieved; World::GetWorldPointer()->Log();
                    #endif
                    return BytesRetrieved;
                }else{
                    #ifdef PHYSDEBUG
                    World::GetWorldPointer()->LogStream << "Exiting OgreDataStreamBuf::xsgetn() and returning EOF"; World::GetWorldPointer()->Log();
                    #endif
                    return traits_type::eof();
                }
            }
        }

        int OgreDataStreamBuf::underflow()
        {
            #ifdef PHYSDEBUG
            World::GetWorldPointer()->Log("Entering OgreDataStreamBuf::underflow()");
            World::GetWorldPointer()->LogStream << "\tSeekBackOnload: " << this->SeekBackOnload << endl;
            #endif

            Whole BufferSize = this->egptr() - this->eback();
            if( 0==BufferSize )
            {
                this->seekpos(0);
                BufferSize = this->egptr() - this->eback();
            }

            if(BufferSize < this->SeekBackOnload)
            {
                //seekback is too large
                if( (0.5*BufferSize<=this->SeekBackOnload) && (2<=this->SeekBackOnload))
                    { this->SeekBackOnload = this->SeekBackOnload * 0.5; }
                else
                    { this->SeekBackOnload = 0; }
                #ifdef PHYSDEBUG
                World::GetWorldPointer()->LogStream << "\tAdjusted SeekBackOnload: " << this->SeekBackOnload << endl;
                World::GetWorldPointer()->LogStream << "\tBufferSize: " << BufferSize << "=" << Toint(this->egptr()) << "-" << Toint(this->eback()) << endl;
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
                    Whole DistanceFromStreamBegin = (this->SeekBackOnload - (streampos)this->OgreStream->tell());
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
            World::GetWorldPointer()->Log("Entering OgreDataStreamBuf::pbackfail()");
            #endif
            Whole Destination = this->GetCurrentLocation()-1;
            if(this->CheckInternalBuffer(Destination))
            {
                this->SetInternalBuffer(Destination);   /// @todo Do some Buffer Math instead of reloading everything we are already near. in pbackfail
                return Toint(this->gptr());
            }else{
                if(this->CheckStream(Destination))
                {
                    this->SetInternalBuffer(Destination);
                    return Toint(this->gptr());
                }else{
                    return traits_type::eof();
                }
            }
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

        void OgreDataStreamBuf::SetInternalBuffer(const streampos& Destination)
        {
            #ifdef PHYSDEBUG
            World::GetWorldPointer()->LogStream << "Entering OgreDataStreamBuf::SetInternalBuffer( streampos " << Destination <<" )" ; World::GetWorldPointer()->Log();
            #endif

            if( Destination < 0)                                                            // Basic Error Checking
                { throw (Exception("Beyond Stream Bounds: Before Stream begins")); }
            if( Destination > this->OgreStream->size() && this->OgreStream->size()!=0 )
                { throw (Exception("Beyond Stream Bounds: After end of known stream")); }

            Whole BufferSize;
            if (this->egptr() == 0 || this->egptr() == 0 || this->eback())
                { BufferSize=0; }
            else
                { BufferSize = this->egptr() - this->eback(); }
            const Whole StreamSize = this->OgreStream->size();

            if (BufferSize == this->LoadAtOnce)                                             //Bufferexists and it is the right size, just Load data
            {
                this->SetInternalBuffer(this->eback(), this->LoadAtOnce, Destination);
            }else if(0==BufferSize){                                                        //Buffer Doesn't exist, create new one and fill it
                if(this->LoadAtOnce < StreamSize)
                {
                    char* Buffer = new char [this->LoadAtOnce];                                 //What is left to load is simply one chunk of many, lets one one of default size
                    this->SetInternalBuffer(Buffer, this->LoadAtOnce, Destination);
                }else{
                    char* Buffer = new char [StreamSize];                                       //Want to load more than exists, simply load what exists.
                    this->SetInternalBuffer(Buffer, StreamSize, Destination);
                }
            }else{                                                                          //Buffer does exist and it is all wrong, Nuke and Pave(Most likely)
                if(StreamSize==BufferSize)                                                      //Maybe we set it because the stream is too small
                    { this->SetInternalBuffer(this->eback(), BufferSize, Destination); }
                else                                                                            //not likely, this thin is some wierd size, and must be nuked
                {
                    #ifdef PHYSDEBUG
                    World::GetWorldPointer()->Log("Mysterious Buffer Size, Deleting and creating fresh" ) ;
                    #endif
                    delete [] this->eback();
                    this->setg(0,0,0);
                    this->SetInternalBuffer(Destination);
                }
            }
            #ifdef PHYSDEBUG
            World::GetWorldPointer()->Log("Exiting OgreDataStreamBuf::SetInternalBuffer( streampos )" ) ;
            #endif
        }

        //OgreDataStreamBuf::SetInternalBuffer(BeginPtr=0, BufferSize=76793, Destination=10000)
        void OgreDataStreamBuf::SetInternalBuffer(char* BeginPtr, const Whole& BufferSize, const streampos& Destination)
        {
            #ifdef PHYSDEBUG
            World::GetWorldPointer()->LogStream << "Entering OgreDataStreamBuf::SetInternalBuffer(BeginPtr=" << Tounsignedint(BeginPtr) << ", BufferSize=" << BufferSize << ", Destination=" << Destination << ")"; World::GetWorldPointer()->Log();
            #endif
            if (this->SeekBackOnload > (this->MaxSeekBack * BufferSize) )
                { this->SeekBackOnload = this->MaxSeekBack * BufferSize; }

            Whole StreamSize = this->OgreStream->size();
            if (Destination<this->SeekBackOnload)                                           //But wait are we too close to the beginning?
            {
                #ifdef PHYSDEBUG
                World::GetWorldPointer()->LogStream << "Exiting OgreDataStreamBuf::SetInternalBuffer(char*,Whole,streampos) calling SetInternalBuffer(char*,Whole,streampos,streampos) because we were too close to the beginning of the stream. " ; World::GetWorldPointer()->Log();
                #endif
                this->SetInternalBuffer(BeginPtr, BufferSize, Destination, Destination);
            }
            else if (StreamSize-(BufferSize-this->SeekBackOnload)<Destination)              //Or maybe too close to the end?
            {
                #ifdef PHYSDEBUG
                World::GetWorldPointer()->LogStream << "Exiting OgreDataStreamBuf::SetInternalBuffer(char*,Whole,streampos) calling SetInternalBuffer(char*,Whole,streampos,streampos) because we were too close to the end of the stream. Determined by (StreamSize-(BufferSize-this->SeekBackOnload) < Destination)/("<< StreamSize << "-(" << BufferSize << "-" << this->SeekBackOnload << ") < " << Destination << ")" ; World::GetWorldPointer()->Log();
                #endif
                this->SetInternalBuffer(BeginPtr, BufferSize, Destination, this->LoadAtOnce-(static_cast<streampos>(StreamSize)-Destination) );
            }
            else                                                                            //We must be in the middle
            {
                #ifdef PHYSDEBUG
                World::GetWorldPointer()->LogStream << "Exiting OgreDataStreamBuf::SetInternalBuffer(char*,Whole,streampos) calling SetInternalBuffer(char*,Whole,streampos,streampos) because we were in the middle of the stream. " ; World::GetWorldPointer()->Log();
                #endif
                this->SetInternalBuffer(BeginPtr, BufferSize, Destination, this->SeekBackOnload);
            }
        }

        void OgreDataStreamBuf::SetInternalBuffer(char* BeginPtr, const Whole& BufferSize, const streampos& Destination, const streampos& BackFill)
        {
            #ifdef PHYSDEBUG
            World::GetWorldPointer()->LogStream << "Entering OgreDataStreamBuf::SetInternalBuffer(BeginPtr=" << Tounsignedint(BeginPtr) << ", BufferSize=" << BufferSize << ", Destination=" << Destination << ", BackFill=" << BackFill << ")" ; World::GetWorldPointer()->Log();
            #endif
            this->OgreStream->seek(Destination-BackFill);
            this->OgreStream->read(BeginPtr, BufferSize);
            this->setg(BeginPtr, BeginPtr+BackFill, BeginPtr+BufferSize);
        }

        bool OgreDataStreamBuf::CheckInternalBuffer(const streampos& BeginPoint, const streampos& EndPoint)
        {
            #ifdef PHYSDEBUG
            World::GetWorldPointer()->LogStream << "Entering OgreDataStreamBuf::CheckInternalBuffer( streampos " << BeginPoint << ", streampos " << EndPoint << " )"; World::GetWorldPointer()->Log();
            #endif

            if (this->egptr() == 0 || this->egptr() == 0 || this->eback())
            {
                #ifdef PHYSDEBUG
                World::GetWorldPointer()->LogStream << "Exiting OgreDataStreamBuf::CheckInternalBuffer()=0";World::GetWorldPointer()->Log();
                #endif
                return 0;
            }

            //Load with raw memory locations whose values are functionally random, but relative locations are the same from the OgreStream
            char* CharBufferStart = this->eback();
            char* CharBufferEnd = this->egptr();
            Whole BufferSize = CharBufferStart - CharBufferEnd;
            #ifdef PHYSDEBUG
            World::GetWorldPointer()->LogStream << "Raw Pointers - CharBufferStart=" << Tounsignedint(CharBufferStart) << ", CharBufferEnd=" << Tounsignedint(CharBufferEnd) << " and BufferSize=" << BufferSize; World::GetWorldPointer()->Log();
            #endif

            // This function assumes that the cursor in the Ogre stream is the equal to egptr()
            Whole WhereIsBufferEnd = this->OgreStream->tell();
            Whole BufferEnd = this->OgreStream->tell();
            Whole BufferStart = BufferEnd - BufferSize;
            #ifdef PHYSDEBUG
            World::GetWorldPointer()->LogStream << "Stream Locations - BufferStart=" << BufferStart << ", BufferEnd=" << BufferEnd << ", Stream End=" << this->OgreStream->size() ;
            World::GetWorldPointer()->Log();
            #endif

            if( BufferStart<BeginPoint && BeginPoint<BufferEnd )
            {
                if( 0==EndPoint || (BufferStart<BeginPoint && BeginPoint<BufferEnd) )
                {
                    #ifdef PHYSDEBUG
                    World::GetWorldPointer()->LogStream << "Exiting OgreDataStreamBuf::CheckInternalBuffer() - Returning True" ; World::GetWorldPointer()->Log();
                    #endif
                    return true;
                }else{
                    #ifdef PHYSDEBUG
                    World::GetWorldPointer()->LogStream << "Exiting OgreDataStreamBuf::CheckInternalBuffer() - Returning False" ; World::GetWorldPointer()->Log();
                    #endif
                    return false;
                }
            }else{
                #ifdef PHYSDEBUG
                World::GetWorldPointer()->LogStream << "Exiting OgreDataStreamBuf::CheckInternalBuffer() - Returning False" ; World::GetWorldPointer()->Log();
                #endif
                return false;
            }
        }

        bool OgreDataStreamBuf::CheckStream(const Whole& BeginPoint, const Whole& EndPoint)
        {
            #ifdef PHYSDEBUG
            World::GetWorldPointer()->LogStream << "Entering OgreDataStreamBuf::CheckStream( Whole " << BeginPoint << ", Whole " << EndPoint << " )" ;
            World::GetWorldPointer()->Log();
            #endif

            if( 0<BeginPoint && BeginPoint<this->OgreStream->size() )
            {
                if( 0==EndPoint || (0<BeginPoint && BeginPoint<this->OgreStream->size()) )
                {
                    #ifdef PHYSDEBUG
                    World::GetWorldPointer()->Log( "Exiting OgreDataStreamBuf::CheckStream() - Returning True" );
                    #endif
                    return true;
                }else{
                    #ifdef PHYSDEBUG
                    World::GetWorldPointer()->Log( "Exiting OgreDataStreamBuf::CheckStream() - Returning False" );
                    #endif
                    return false;
                }
            }else{
                #ifdef PHYSDEBUG
                World::GetWorldPointer()->Log( "Exiting OgreDataStreamBuf::CheckStream() - Returning False");
                #endif
                return false;
            }
        }

        bool OgreDataStreamBuf::Readable()
        {
            return this->OgreStream->isReadable();
        }

        bool OgreDataStreamBuf::Writeable()
        {
            return this->OgreStream->isWriteable();
        }

        Whole OgreDataStreamBuf::GetCurrentLocation()
        {
            #ifdef PHYSDEBUG
            World::GetWorldPointer()->LogStream << "Entering OgreDataStreamBuf::GetCurrentLocation()"; World::GetWorldPointer()->Log();
            #endif

            if (this->egptr() == 0 || this->egptr() == 0 || this->eback())
            {
                #ifdef PHYSDEBUG
                World::GetWorldPointer()->LogStream << "Exiting OgreDataStreamBuf::GetCurrentLocation()=0";World::GetWorldPointer()->Log();
                #endif
                return 0;
            }

            #ifdef PHYSDEBUG
            World::GetWorldPointer()->LogStream << "Exiting OgreDataStreamBuf::GetCurrentLocation() =" << (this->OgreStream->tell() - (this->egptr()-this->gptr())); World::GetWorldPointer()->Log();World::GetWorldPointer()->Log();
            #endif
            return this->OgreStream->tell() - (this->egptr()-this->gptr());
        }

    }// /internal
}// /phys

/// @endcond

#endif

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
#ifndef _internaliostreamwrapper_cpp
#define _internaliostreamwrapper_cpp

// Keeps this file from being documented by doxygen
/// @cond DontDocumentInternal

#include "Internal/iostreamwrapper.h.cpp"

namespace Mezzanine
{
    namespace Internal
    {
        IOStreamWrapper::IOStreamWrapper(std::iostream* ToBeWrapped, const Boole Clean) :
            Stream(ToBeWrapped),
            CleanUp(Clean)
            {  }

        IOStreamWrapper::~IOStreamWrapper()
            { if( this->CleanUp ) delete this->Stream; }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        size_t IOStreamWrapper::read(void* buf, size_t count)
        {
            this->Stream->read(static_cast<char*>(buf),count);
            return this->Stream->gcount();
        }

        size_t IOStreamWrapper::write(const void* buf, size_t count)
        {
            this->Stream->write(static_cast<const char*>(buf),count);
            return ( this->Stream->fail() ? 0 : count );
        }

        void IOStreamWrapper::skip(long count)
        {
            this->Stream->seekg(count,std::ios_base::cur);
            this->Stream->seekp(count,std::ios_base::cur);
        }

        void IOStreamWrapper::seek(size_t pos)
        {
            this->Stream->seekg(pos);
            this->Stream->seekp(pos);
        }

        size_t IOStreamWrapper::tell() const
            { return this->Stream->tellg(); }

        bool IOStreamWrapper::eof() const
            { return this->Stream->eof(); }

        void IOStreamWrapper::close()
            {  }
    }//Internal
}//Nezzanine

/// @endcond

#endif

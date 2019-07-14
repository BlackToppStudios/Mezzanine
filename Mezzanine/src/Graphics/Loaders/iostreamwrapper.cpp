// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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

#include "Graphics/Loaders/iostreamwrapper.h.cpp"

namespace Mezzanine
{
    namespace Graphics
    {
        /*
        ///////////////////////////////////////////////////////////////////////////////
        // FStreamAdapter Methods

        FStreamAdapter::FStreamAdapter(const char* FileName, ios_base::openmode Mode) :
            std::fstream(FileName,Mode)
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        size_t FStreamAdapter::read(void* buf, size_t count)
        {
            this->std::fstream::read(static_cast<char*>(buf),count);
            return this->std::fstream::gcount();
        }

        size_t FStreamAdapter::write(const void* buf, size_t count)
        {
            this->std::fstream::write(static_cast<const char*>(buf),count);
            return ( this->std::fstream::fail() ? 0 : count );
        }

        void FStreamAdapter::skip(long count)
        {
            this->std::fstream::seekg(count,std::ios_base::cur);
            this->std::fstream::seekp(count,std::ios_base::cur);
        }

        void FStreamAdapter::seek(size_t pos)
        {
            this->std::fstream::seekg(pos);
            this->std::fstream::seekp(pos);
        }

        size_t FStreamAdapter::tell() const
            { this->std::fstream::tellg(); }

        bool FStreamAdapter::eof() const
            { this->std::fstream::eof(); }

        void FStreamAdapter::close()
            { this->std::fstream::close(); }

        ///////////////////////////////////////////////////////////////////////////////
        // IFStreamAdapter Methods

        IFStreamAdapter::IFStreamAdapter(const char* FileName, ios_base::openmode Mode) :
            std::ifstream(FileName,Mode)
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        size_t IFStreamAdapter::read(void* buf, size_t count)
        size_t IFStreamAdapter::write(const void* buf, size_t count)
        void IFStreamAdapter::skip(long count)
        void IFStreamAdapter::seek(size_t pos)
        size_t IFStreamAdapter::tell() const
        bool IFStreamAdapter::eof() const
        void IFStreamAdapter::close()

        ///////////////////////////////////////////////////////////////////////////////
        // OFStreamAdapter Methods

        OFStreamAdapter::OFStreamAdapter(const char* FileName, ios_base::openmode Mode) :
            std::ofstream(FileName,Mode)
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        size_t OFStreamAdapter::read(void* buf, size_t count)
        size_t OFStreamAdapter::write(const void* buf, size_t count)
        void OFStreamAdapter::skip(long count)
        void OFStreamAdapter::seek(size_t pos)
        size_t OFStreamAdapter::tell() const
        bool OFStreamAdapter::eof() const
        void OFStreamAdapter::close()
        */

        ///////////////////////////////////////////////////////////////////////////////
        // IOStreamWrapper Methods

        IOStreamWrapper::IOStreamWrapper(DataStreamPtr ToBeWrapped) :
            Stream(ToBeWrapped)
            {  }

        IOStreamWrapper::~IOStreamWrapper()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        size_t IOStreamWrapper::read(void* buf, size_t count)
            { return this->Stream->Read(buf,count); }

        size_t IOStreamWrapper::write(const void* buf, size_t count)
            { return this->Stream->Write(buf,count); }

        void IOStreamWrapper::skip(long count)
            { this->Stream->SetStreamPosition(count,SO_Current); }

        void IOStreamWrapper::seek(size_t pos)
            { this->Stream->SetStreamPosition(pos,SO_Beginning); }

        size_t IOStreamWrapper::tell() const
            { return this->Stream->GetStreamPosition(); }

        bool IOStreamWrapper::eof() const
            { return this->Stream->EoF(); }

        void IOStreamWrapper::close()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // IStreamWrapper Methods

        IStreamWrapper::IStreamWrapper(IStreamPtr ToBeWrapped) :
            Stream(ToBeWrapped)
            {  }

        IStreamWrapper::~IStreamWrapper()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        size_t IStreamWrapper::read(void* buf, size_t count)
            { return this->Stream->Read(buf,count); }

        size_t IStreamWrapper::write(const void* buf, size_t count)
            { return 0; }

        void IStreamWrapper::skip(long count)
            { this->Stream->SetReadPosition(count,SO_Current);  }

        void IStreamWrapper::seek(size_t pos)
            { this->Stream->SetReadPosition(pos,SO_Beginning); }

        size_t IStreamWrapper::tell() const
            { return this->Stream->GetReadPosition(); }

        bool IStreamWrapper::eof() const
            { return this->Stream->EoF(); }

        void IStreamWrapper::close()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // OStreamWrapper Methods

        OStreamWrapper::OStreamWrapper(OStreamPtr ToBeWrapped) :
            Stream(ToBeWrapped)
            {  }

        OStreamWrapper::~OStreamWrapper()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        size_t OStreamWrapper::read(void* buf, size_t count)
        {
            return 0;
        }

        size_t OStreamWrapper::write(const void* buf, size_t count)
        {
            return this->Stream->Write(buf,count);
        }

        void OStreamWrapper::skip(long count)
            {  this->Stream->SetWritePosition(count,SO_Current); }

        void OStreamWrapper::seek(size_t pos)
            { this->Stream->SetWritePosition(pos,SO_Beginning); }

        size_t OStreamWrapper::tell() const
            { return this->Stream->GetWritePosition(); }

        bool OStreamWrapper::eof() const
            { return this->Stream->EoF(); }

        void OStreamWrapper::close()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // STDIOStreamWrapper Methods

        STDIOStreamWrapper::STDIOStreamWrapper(std::iostream* ToBeWrapped, const Boole Clean) :
            Stream(ToBeWrapped),
            CleanUp(Clean)
            {  }

        STDIOStreamWrapper::~STDIOStreamWrapper()
            { if( this->CleanUp ) delete this->Stream; }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        size_t STDIOStreamWrapper::read(void* buf, size_t count)
        {
            this->Stream->read(static_cast<char*>(buf),count);
            return this->Stream->gcount();
        }

        size_t STDIOStreamWrapper::write(const void* buf, size_t count)
        {
            this->Stream->write(static_cast<const char*>(buf),count);
            return ( this->Stream->fail() ? 0 : count );
        }

        void STDIOStreamWrapper::skip(long count)
        {
            this->Stream->seekg(count,std::ios_base::cur);
            this->Stream->seekp(count,std::ios_base::cur);
        }

        void STDIOStreamWrapper::seek(size_t pos)
        {
            this->Stream->seekg(pos);
            this->Stream->seekp(pos);
        }

        size_t STDIOStreamWrapper::tell() const
            { return this->Stream->tellg(); }

        bool STDIOStreamWrapper::eof() const
            { return this->Stream->eof(); }

        void STDIOStreamWrapper::close()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // STDIStreamWrapper Methods

        STDIStreamWrapper::STDIStreamWrapper(std::istream* ToBeWrapped, const Boole Clean) :
            Stream(ToBeWrapped),
            CleanUp(Clean)
            {  }

        STDIStreamWrapper::~STDIStreamWrapper()
            { if( this->CleanUp ) delete this->Stream; }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        size_t STDIStreamWrapper::read(void* buf, size_t count)
        {
            this->Stream->read(static_cast<char*>(buf),count);
            return this->Stream->gcount();
        }

        size_t STDIStreamWrapper::write(const void* buf, size_t count)
        {
            return 0;
        }

        void STDIStreamWrapper::skip(long count)
            { this->Stream->seekg(count,std::ios_base::cur);  }

        void STDIStreamWrapper::seek(size_t pos)
            { this->Stream->seekg(pos); }

        size_t STDIStreamWrapper::tell() const
            { return this->Stream->tellg(); }

        bool STDIStreamWrapper::eof() const
            { return this->Stream->eof(); }

        void STDIStreamWrapper::close()
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // STDOStreamWrapper Methods

        STDOStreamWrapper::STDOStreamWrapper(std::ostream* ToBeWrapped, const Boole Clean) :
            Stream(ToBeWrapped),
            CleanUp(Clean)
            {  }

        STDOStreamWrapper::~STDOStreamWrapper()
            { if( this->CleanUp ) delete this->Stream; }

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        size_t STDOStreamWrapper::read(void* buf, size_t count)
        {
            return 0;
        }

        size_t STDOStreamWrapper::write(const void* buf, size_t count)
        {
            this->Stream->write(static_cast<const char*>(buf),count);
            return ( this->Stream->fail() ? 0 : count );
        }

        void STDOStreamWrapper::skip(long count)
            {  this->Stream->seekp(count,std::ios_base::cur); }

        void STDOStreamWrapper::seek(size_t pos)
            { this->Stream->seekp(pos); }

        size_t STDOStreamWrapper::tell() const
            { return this->Stream->tellp(); }

        bool STDOStreamWrapper::eof() const
            { return this->Stream->eof(); }

        void STDOStreamWrapper::close()
            {  }
    }//Graphics
}//Nezzanine

/// @endcond

#endif

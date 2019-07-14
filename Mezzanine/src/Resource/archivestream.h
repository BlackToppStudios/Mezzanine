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
#ifndef _resourcearchivestream_h
#define _resourcearchivestream_h

#include "filestream.h"
#include "memorystream.h"

/// @file
/// @brief Declaration of ArchiveStream

namespace Mezzanine
{
    namespace Resource
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief A stream buffer object for compressed file archives.
        ///////////////////////////////////////
        class MEZZ_LIB ArchiveStreamBuffer : public std::streambuf
        {
        protected:
            /// @brief A pointer to the stream containing the raw data of the archive.
            DataStreamPtr Stream;
        public:
            /// @brief FileStream constructor.
            /// @param StreamPtr The stream to the raw data of the archive.
            ArchiveStreamBuffer(FileStreamPtr StreamPtr);
            /// @brief MemoryStream constructor.
            /// @param StreamPtr The stream to the raw data of the archive.
            ArchiveStreamBuffer(MemoryStreamPtr StreamPtr);
            /// @brief Class destructor.
            virtual ~ArchiveStreamBuffer();

            ///////////////////////////////////////////////////////////////////////////////
            // Utility
        };//ArchiveStreamBuffer

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief An Input stream for compressed file archives.
        ///////////////////////////////////////
        template<typename BufferType>
        class MEZZ_LIB ArchiveIStream : public Mezzanine::IStream
        {
        protected:
            /// @brief A buffer or buffer meta-data structure to the archive that will be streamed.
            BufferType ArchiveBuffer;
            /// @brief The unique name of the group this stream was created from.
            String GroupName;
        public:
            /// @brief Variadic constructor.
            /// @tparam ArgTypes The types for each argument to be passed to the buffer used.
            /// @param Args The arguments that will be used to initialize the internal archive buffer.
            template<class... ArgTypes>
            ArchiveIStream(ArgTypes&&... Args) :
                IStream(&ArchiveBuffer),
                ArchiveBuffer(std::forward<ArgTypes>(Args)...)
                {  }
            /// @brief Class destructor.
            virtual ~ArchiveIStream()
                { if( this->IsOpenToFile() ) this->CloseFile(); }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Opens this stream to a file in an archive.
            /// @remarks The StreamFlags value SF_Write will be ignored if used.
            /// @exception It is very likely the method invoked on the streambuf class will throw if something goes
            /// wrong.  Be sure to consult the documentation of the used streambuf class to see what can be thrown.
            /// @param Identifier The combined name and path to the file to be opened.
            /// @param Group The unique name of the AssetGroup this stream belongs to (or can be empty).
            /// @param Flags The configuration to open the file with.  Use StreamFlags enum values for this field.
            /// @param Raw False to decompress on the fly (normal behavior), true to read data as it exists on disk.
            void OpenFile(const String& Identifier, const String& Group,
                          const Whole Flags = Mezzanine::SF_Read, const Boole Raw = false)
            {
                const Whole FilteredFlags = Flags & ~Mezzanine::SF_Write;
                this->ArchiveBuffer.OpenFile(Identifier,FilteredFlags,Raw);
                this->GroupName = Group;
            }
            /// @brief Opens this stream to a password protected file in an archive.
            /// @remarks The StreamFlags value SF_Write will be ignored if used.
            /// @exception It is very likely the method invoked on the streambuf class will throw if something goes
            /// wrong.  Be sure to consult the documentation of the used streambuf class to see what can be thrown.
            /// @param Identifier The combined name and path to the file to be opened.
            /// @param Group The unique name of the AssetGroup this stream belongs to (or can be empty).
            /// @param Password The password to access the file.
            /// @param Flags The configuration to open the file with.  Use StreamFlags enum values for this field.
            /// @param Raw False to decompress on the fly (normal behavior), true to read data as it exists on disk.
            void OpenEncryptedFile(const String& Identifier, const String& Group, const String& Password,
                                   const Whole Flags = Mezzanine::SF_Read, const Boole Raw = false)
            {
                const Whole FilteredFlags = Flags & ~Mezzanine::SF_Write;
                this->ArchiveBuffer.OpenEncryptedFile(Identifier,Password,FilteredFlags,Raw);
                this->GroupName = Group;
            }
            /// @brief Gets whether or not this stream is currently open to a archive.
            /// @return Returns true if this is streaming to/from a archive.  False otherwise.
            Boole IsOpenToFile() const
                { return this->ArchiveBuffer.IsOpenToFile(); }
            /// @brief Closes the archive that is currently opened.
            void CloseFile()
                { this->ArchiveBuffer.CloseFile(); }

            /// @brief Gets the flags that were used to open the archive.
            /// @remarks If this stream is not open to a archive it will return Resource::SF_None.
            /// @return Returns a bitfield describing the flags used to open the archive.
            Whole GetStreamFlags() const
                { return this->ArchiveBuffer.GetStreamFlags(); }

            /// @brief Gets the compressed size of the data in the stream.
            /// @return Returns a StreamSize indicating the raw size of the stream in bytes.
            StreamSize GetCompressedSize() const
                { return this->ArchiveBuffer.GetCompressedSize(); }
            /// @brief Gets the uncompressed size of the data in the stream.
            /// @return Returns a StreamSize indicating the size of the data in the stream before it was compressed.
            StreamSize GetUncompressedSize() const
                { return this->ArchiveBuffer.GetUncompressedSize(); }

            ///////////////////////////////////////////////////////////////////////////////
            // Stream Base Operations

            /// @copydoc StreamBase::GetIdentifier() const
            String GetIdentifier() const override
                { return std::move( this->ArchiveBuffer.GetIdentifier() ); }
            /// @copydoc StreamBase::GetGroup() const
            String GetGroup() const override
                { return this->GroupName; }

            /// @copydoc StreamBase::GetSize() const
            StreamSize GetSize() const override
                { return this->ArchiveBuffer.GetSize(); }
            /// @copydoc StreamBase::CanSeek() const
            Boole CanSeek() const override
                { return this->ArchiveBuffer.CanSeek(); }
            /// @copydoc StreamBase::IsEncrypted() const
            Boole IsEncrypted() const override
                { return this->ArchiveBuffer.IsEncrypted(); }
            /// @copydoc StreamBase::IsRaw() const
            Boole IsRaw() const override
                { return this->ArchiveBuffer.IsRaw(); }
        };//ArchiveIStream

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief An Output stream for compressed file archives.
        ///////////////////////////////////////
        template<typename BufferType>
        class MEZZ_LIB ArchiveOStream : public Mezzanine::OStream
        {
        protected:
            /// @brief A buffer or buffer meta-data structure to the archive that will be streamed.
            BufferType ArchiveBuffer;
        public:
            /// @brief Variadic constructor.
            /// @tparam ArgTypes The types for each argument to be passed to the buffer used.
            /// @param Args The arguments that will be used to initialize the internal archive buffer.
            template<class... ArgTypes>
            ArchiveOStream(ArgTypes&&... Args) :
                ArchiveBuffer(std::forward<ArgTypes>(Args)...)
                {  }
            /// @brief Class destructor.
            virtual ~ArchiveOStream()
                { if( this->IsOpenToFile() ) this->CloseFile(); }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Opens this stream to a file in an archive.
            /// @remarks The StreamFlags value SF_Read will be ignored if used.
            /// @exception It is very likely the method invoked on the streambuf class will throw if something goes
            /// wrong.  Be sure to consult the documentation of the used streambuf class to see what can be thrown.
            /// @param Identifier The combined name and path to the file to be opened.
            /// @param Group The unique name of the AssetGroup this stream belongs to (or can be empty).
            /// @param Flags The configuration to open the file with.  Use StreamFlags enum values for this field.
            /// @param Raw False to decompress on the fly (normal behavior), true to read data as it exists on disk.
            void OpenFile(const String& Identifier, const String& Group,
                          const Whole Flags = Mezzanine::SF_Write, const Boole Raw = false)
            {
                const Whole FilteredFlags = Flags & ~Mezzanine::SF_Read;
                this->OpenFile(Identifier,FilteredFlags,Raw);
                this->GroupName = Group;
            }
            /// @brief Opens this stream to a password protected file in an archive.
            /// @remarks The StreamFlags value SF_Read will be ignored if used.
            /// @exception It is very likely the method invoked on the streambuf class will throw if something goes
            /// wrong.  Be sure to consult the documentation of the used streambuf class to see what can be thrown.
            /// @param Identifier The combined name and path to the file to be opened.
            /// @param Group The unique name of the AssetGroup this stream belongs to (or can be empty).
            /// @param Password The password to access the file.
            /// @param Flags The configuration to open the file with.  Use StreamFlags enum values for this field.
            /// @param Raw False to decompress on the fly (normal behavior), true to read data as it exists on disk.
            void OpenEncryptedFile(const String& Identifier, const String& Group, const String& Password,
                                   const Whole Flags = Mezzanine::SF_Write, const Boole Raw = false)
            {
                const Whole FilteredFlags = Flags & ~Mezzanine::SF_Read;
                this->OpenEncryptedFile(Identifier,Password,FilteredFlags,Raw);
                this->GroupName = Group;
            }
            /// @brief Gets whether or not this stream is currently open to a archive.
            /// @return Returns true if this is streaming to/from a archive.  False otherwise.
            Boole IsOpenToFile() const
                { return this->ArchiveBuffer.IsOpenToFile(); }
            /// @brief Closes the archive that is currently opened.
            void CloseFile()
                { this->ArchiveBuffer.CloseFile(); }

            /// @brief Gets the flags that were used to open the archive.
            /// @remarks If this stream is not open to a archive it will return Resource::SF_None.
            /// @return Returns a bitfield describing the flags used to open the archive.
            Whole GetStreamFlags() const
                { return this->ArchiveBuffer.GetStreamFlags(); }

            /// @brief Gets the compressed size of the data in the stream.
            /// @return Returns a StreamSize indicating the raw size of the stream in bytes.
            StreamSize GetCompressedSize() const
                { return this->ArchiveBuffer.GetCompressedSize(); }
            /// @brief Gets the uncompressed size of the data in the stream.
            /// @return Returns a StreamSize indicating the size of the data in the stream before it was compressed.
            StreamSize GetUncompressedSize() const
                { return this->ArchiveBuffer.GetUncompressedSize(); }

            ///////////////////////////////////////////////////////////////////////////////
            // Stream Base Operations

            /// @copydoc StreamBase::GetIdentifier() const
            String GetIdentifier() const override
                { return this->ArchiveBuffer.GetStreamIdentifier(); }
            /// @copydoc StreamBase::GetGroup() const
            String GetGroup() const override
                { return this->GroupName; }

            /// @copydoc StreamBase::GetSize() const
            StreamSize GetSize() const override
                { return this->ArchiveBuffer.GetSize(); }
            /// @copydoc StreamBase::CanSeek() const
            Boole CanSeek() const override
                { return this->ArchiveBuffer.CanSeek(); }
            /// @copydoc StreamBase::IsEncrypted() const
            Boole IsEncrypted() const override
                { return this->ArchiveBuffer.IsEncrypted(); }
            /// @copydoc StreamBase::IsRaw() const
            Boole IsRaw() const override
                { return this->ArchiveBuffer.IsRaw(); }
        };//ArchiveOStream

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief An I/O stream for compressed file archives.
        ///////////////////////////////////////
        template<typename BufferType>
        class MEZZ_LIB ArchiveStream : public Mezzanine::IOStream
        {
        protected:
            /// @brief A buffer or buffer meta-data structure to the archive that will be streamed.
            BufferType ArchiveBuffer;
        public:
            /// @brief Variadic constructor.
            /// @tparam ArgTypes The types for each argument to be passed to the buffer used.
            /// @param Args The arguments that will be used to initialize the internal archive buffer.
            template<class... ArgTypes>
            ArchiveStream(ArgTypes&&... Args) :
                ArchiveBuffer(std::forward<ArgTypes>(Args)...)
                {  }
            /// @brief Class destructor.
            virtual ~ArchiveStream()
                { if( this->IsOpenToFile() ) this->CloseFile(); }

            ///////////////////////////////////////////////////////////////////////////////
            // Utility

            /// @brief Opens this stream to a file in an archive.
            /// @exception It is very likely the method invoked on the streambuf class will throw if something goes
            /// wrong.  Be sure to consult the documentation of the used streambuf class to see what can be thrown.
            /// @param Identifier The combined name and path to the file to be opened.
            /// @param Group The unique name of the AssetGroup this stream belongs to (or can be empty).
            /// @param Flags The configuration to open the file with.  Use StreamFlags enum values for this field.
            /// @param Raw False to decompress on the fly (normal behavior), true to read data as it exists on disk.
            void OpenFile(const String& Identifier, const String& Group,
                          const Whole Flags = Mezzanine::SF_ReadWrite, const Boole Raw = false)
            {
                this->OpenFile(Identifier,Flags,Raw);
                this->GroupName = Group;
            }
            /// @brief Opens this stream to a password protected file in an archive.
            /// @exception It is very likely the method invoked on the streambuf class will throw if something goes
            /// wrong.  Be sure to consult the documentation of the used streambuf class to see what can be thrown.
            /// @param Identifier The combined name and path to the file to be opened.
            /// @param Group The unique name of the AssetGroup this stream belongs to (or can be empty).
            /// @param Password The password to access the file.
            /// @param Flags The configuration to open the file with.  Use StreamFlags enum values for this field.
            /// @param Raw False to decompress on the fly (normal behavior), true to read data as it exists on disk.
            void OpenEncryptedFile(const String& Identifier, const String& Group, const String& Password,
                                   const Whole Flags = Mezzanine::SF_ReadWrite, const Boole Raw = false)
            {
                this->OpenEncryptedFile(Identifier,Password,Flags,Raw);
                this->GroupName = Group;
            }
            /// @brief Gets whether or not this stream is currently open to a archive.
            /// @return Returns true if this is streaming to/from a archive.  False otherwise.
            Boole IsOpenToFile() const
                { return this->ArchiveBuffer.IsOpenToFile(); }
            /// @brief Closes the archive that is currently opened.
            void CloseFile()
                { this->ArchiveBuffer.CloseFile(); }

            /// @brief Gets the flags that were used to open the archive.
            /// @remarks If this stream is not open to a archive it will return Resource::SF_None.
            /// @return Returns a bitfield describing the flags used to open the archive.
            Whole GetStreamFlags() const
                { return this->ArchiveBuffer.GetStreamFlags(); }

            /// @brief Gets the compressed size of the data in the stream.
            /// @return Returns a StreamSize indicating the raw size of the stream in bytes.
            StreamSize GetCompressedSize() const
                { return this->ArchiveBuffer.GetCompressedSize(); }
            /// @brief Gets the uncompressed size of the data in the stream.
            /// @return Returns a StreamSize indicating the size of the data in the stream before it was compressed.
            StreamSize GetUncompressedSize() const
                { return this->ArchiveBuffer.GetUncompressedSize(); }

            ///////////////////////////////////////////////////////////////////////////////
            // Stream Base Operations

            /// @copydoc StreamBase::GetIdentifier() const
            String GetIdentifier() const override
                { return std::move( this->ArchiveBuffer.GetStreamIdentifier() ); }
            /// @copydoc StreamBase::GetGroup() const
            String GetGroup() const override
                { return this->GroupName; }

            /// @copydoc StreamBase::GetSize() const
            StreamSize GetSize() const override
                { return this->ArchiveBuffer.GetSize(); }
            /// @copydoc StreamBase::CanSeek() const
            Boole CanSeek() const override
                { return this->ArchiveBuffer.CanSeek(); }
            /// @copydoc StreamBase::IsEncrypted() const
            Boole IsEncrypted() const override
                { return this->ArchiveBuffer.IsEncrypted(); }
            /// @copydoc StreamBase::IsRaw() const
            Boole IsRaw() const override
                { return this->ArchiveBuffer.IsRaw(); }
        };//ArchiveStream

        /// @brief Convenience type for an Archive input stream in a std::shared_ptr.
        template<typename BufferType>
        using ArchiveIStreamPtr = std::shared_ptr< ArchiveIStream<BufferType> >;
        /// @brief Convenience type for an Archive output stream in a std::shared_ptr.
        template<typename BufferType>
        using ArchiveOStreamPtr = std::shared_ptr< ArchiveOStream<BufferType> >;
        /// @brief This is a convenience type for an Archive stream in a std::shared_ptr.
        template<typename BufferType>
        using ArchiveStreamPtr = std::shared_ptr< ArchiveStream<BufferType> >;
    }//Resource
}//Mezzanine

#endif

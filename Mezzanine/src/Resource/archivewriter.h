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
#ifndef _resourcearchivewriter_h
#define _resourcearchivewriter_h

#include "datastream.h"
#include "Resource/resourceenumerations.h"

namespace Mezzanine
{
    namespace Resource
    {
        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a POD class containing the parameters used for compressing data.
        ///////////////////////////////////////
        struct CompressParameters
        {
            /// @param CompressType The type of compression to be applied when writing to the file.
            Whole CompressType;
            /// @param CompressLevel The amount of compression to apply.  Different compression methods
            /// support different levels.  Consult compression method documentation for supported levels.
            Integer CompressLevel;
        };

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a POD class containing the parameters used for encrypting data.
        ///////////////////////////////////////
        struct EncryptParameters
        {
            /// @param EncryptType The type of encryption to be applied when writing to the file.
            Whole EncryptType;
            /// @param The password/key to set for decrypting the data.
            String EncryptPassword;
        };

        ///////////////////////////////////////////////////////////////////////////////
        /// @brief This is a base class for collections of files that can be written/created.
        ///////////////////////////////////////
        class MEZZ_LIB ArchiveWriter
        {
        protected:
        public:
            /// @brief Class constructor.
            ArchiveWriter() = default;
            /// @brief Class destructor.
            virtual ~ArchiveWriter() = default;

            ///////////////////////////////////////////////////////////////////////////////
            // Open / Close

            /// @brief Opens an archive for IO operations.
            /// @param Identifier The path and name to the archive to be opened.
            /// @param Flags A bitmask of the options to open the archive with.
            virtual void Open(const String& Identifier, const Whole Flags) = 0;
            /// @brief Open an archive to a memory buffer.
            /// @param Identifier A string identifying the buffer to be used as the archive.
            /// @param Buffer A pointer to the buffer containing the archive.
            /// @param BufferSize The size of the buffer container the archive.
            /// @param Flags A bitmask of the options to open the archive with.
            /// @param Owner A bool to indicate if the archive should become the owner of the buffer, deleting it when it closes.
            virtual void Open(const String& Identifier, Char8* Buffer, const size_t BufferSize, const Whole Flags, const Boole Owner) = 0;
            /// @brief Gets whether or not this archive is open for I/O operations.
            /// @return Returns true if this archive is open and in use, false otherwise.
            virtual Boole IsOpen() const = 0;
            /// @brief Closes the archive and removes all streams to it.
            /// @remarks The archive will be set back to a pre-open state and can be reopened.
            virtual void Close() = 0;

            /// @brief Gets the configuration flags used to open the archive.
            /// @return Returns the bitmask that was used to open the archive.
            virtual Whole GetFlags() const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // File and Directory Query

            /// @brief Checks to see if a directory exists within the archive.
            /// @param DirectoryPath The path to the directory (or just name of the directory if it's at the root) to check for.
            /// @return Returns true if the operation was successful, false otherwise.
            virtual Boole DirectoryExists(const String& DirectoryPath) const = 0;
            /// @brief Checks to see if a file exists within the archive.
            /// @param PathAndFile The path and name of the file (or just name of the file if it's at the root) to check for.
            /// @return Returns true if the operation was successful, false otherwise.
            virtual Boole FileExists(const String& PathAndFile) const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Streaming

            /// @brief Opens a stream to a resource in the archive.
            /// @param Identifier The unique identifier for the resource in the archive.
            /// @param Flags A bitmask of the options to open the stream with.  See StreamFlags enum for information on valid flags.
            /// @return Returns a pointer to the opened stream.
            virtual OStreamPtr OpenOStream(const char* Identifier, const Whole Flags) = 0;
            /// @brief Opens a stream to a resource in the archive.
            /// @param Identifier The unique identifier for the resource in the archive.
            /// @param Flags A bitmask of the options to open the stream with.  See StreamFlags enum for information on valid flags.
            /// @return Returns a pointer to the opened stream.
            virtual OStreamPtr OpenOStream(const String& Identifier, const Whole Flags) = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Default Parameters

            /// @brief Sets the default compression parameters to use when not specified while calling CreateFile.
            /// @remarks These parameters are only used when calling CreateFile overloads that don't accept compression
            /// parameters.  For the overloads that do, the parameters provided in that invocation will be used.
            /// @param CompParams The default parameters to apply to any new file added to the archive.
            virtual void SetDefaultCompressionParameters(const CompressParameters& CompParams) = 0;
            /// @brief Gets the default compression parameters to use when not specified while calling CreateFile.
            /// @return Returns the current default compression parameters for any new file added to the archive.
            virtual CompressParameters GetDefaultCompressionParameters() const = 0;
            /// @brief Sets the default encryption parameters to use when not specified while calling CreateFile.
            /// @remarks These parameters are only used when calling CreateFile overloads that don't accept encryption
            /// parameters.  For the overloads that do, the parameters provided in that invocation will be used.
            /// @param EncryptParams The default parameters to apply to any new file added to the archive.
            virtual void SetDefaultEncryptionParameters(const EncryptParameters& EncryptParams) = 0;
            /// @brief Gets the default encryption parameters to use when not specified while calling CreateFile.
            /// @return Returns the current default encryption parameters for any new file added to the archive.
            virtual EncryptParameters GetDefaultEncryptionParameters() const = 0;

            ///////////////////////////////////////////////////////////////////////////////
            // Writing

            /// @brief Creates a new empty directory.
            /// @param DirectoryPath The path to the directory that will be created.
            /// @return Returns true if the operation was successful, false otherwise.
            virtual Boole CreateDirectory(const String& DirectoryPath) = 0;
            /// @brief Creates a new file and writes the contents of a stream to it.
            /// @param PathAndFile The path and name of the file to be created.
            /// @param Input The stream containing the data that will be written to the file.
            /// @return Returns true if the operation was successful, false otherwise.
            virtual Boole CreateFile(const String& PathAndFile,
                                     std::istream& Input) = 0;
            /// @brief Creates a new file and writes the contents of a stream to it.
            /// @remarks CompressType and EncryptType parameters are ignored by filesystem archives.  See derived
            /// Archive documentation for supported values.
            /// @param PathAndFile The path and name of the file to be created.
            /// @param Input The stream containing the data that will be written to the file.
            /// @param CompParams The parameters for the compression to be applied when writing to the file.
            /// @return Returns true if the operation was successful, false otherwise.
            virtual Boole CreateFile(const String& PathAndFile,
                                     std::istream& Input,
                                     const CompressParameters& CompParams) = 0;
            /// @brief Creates a new file and writes the contents of a stream to it.
            /// @remarks CompressType and EncryptType parameters are ignored by filesystem archives.  See derived
            /// Archive documentation for supported values.
            /// @param PathAndFile The path and name of the file to be created.
            /// @param Input The stream containing the data that will be written to the file.
            /// @param CompParams The parameters for the compression to be applied when writing to the file.
            /// @param EncryptParams The parameters for the encryption to be applied when writing to the file.
            /// @return Returns true if the operation was successful, false otherwise.
            virtual Boole CreateFile(const String& PathAndFile,
                                     std::istream& Input,
                                     const CompressParameters& CompParams,
                                     const EncryptParameters& EncryptParams) = 0;
            /// @brief Creates a new file and writes the contents of a buffer to it.
            /// @remarks CompressType and EncryptType parameters are ignored by filesystem archives.
            /// @param PathAndFile The path and name of the file to be created.
            /// @param Buffer The buffer that will be written to the file.
            /// @param BufSize The size of the buffer to write to the file.
            /// @return Returns true if the operation was successful, false otherwise.
            virtual Boole CreateFile(const String& PathAndFile,
                                     void* Buffer,
                                     const Whole BufSize) = 0;
            /// @brief Creates a new file and writes the contents of a buffer to it.
            /// @remarks CompressType and EncryptType parameters are ignored by filesystem archives.
            /// @param PathAndFile The path and name of the file to be created.
            /// @param Buffer The buffer that will be written to the file.
            /// @param BufSize The size of the buffer to write to the file.
            /// @param CompParams The parameters for the compression to be applied when writing to the file.
            /// @return Returns true if the operation was successful, false otherwise.
            virtual Boole CreateFile(const String& PathAndFile,
                                     void* Buffer,
                                     const Whole BufSize,
                                     const CompressParameters& CompParams) = 0;
            /// @brief Creates a new file and writes the contents of a buffer to it.
            /// @remarks CompressType and EncryptType parameters are ignored by filesystem archives.
            /// @param PathAndFile The path and name of the file to be created.
            /// @param Buffer The buffer that will be written to the file.
            /// @param BufSize The size of the buffer to write to the file.
            /// @param CompParams The parameters for the compression to be applied when writing to the file.
            /// @param EncryptParams The parameters for the encryption to be applied when writing to the file.
            /// @return Returns true if the operation was successful, false otherwise.
            virtual Boole CreateFile(const String& PathAndFile,
                                     void* Buffer,
                                     const Whole BufSize,
                                     const CompressParameters& CompParams,
                                     const EncryptParameters& EncryptParams) = 0;
            /// @brief Deletes an existing file.
            /// @param PathAndFile The path and name of the file to be deleted.
            /// @return Returns true if the operation was successful, false otherwise.
            virtual Boole DeleteFile(const String& PathAndFile) = 0;
        };//ArchiveWriter
    }//Resource
}//Mezzanine

#endif

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
#ifndef _filestream_h
#define _filestream_h

#include "datastream.h"

/// @file
/// @brief Declaration of FileStream

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This represents a stream to a file on disk using the C++ file stream API.
    ///////////////////////////////////////
    class MEZZ_LIB FileStream : public Mezzanine::IOStream
    {
    protected:
        /// @internal
        /// @brief The buffer object containing most or all of the functionality for this stream.
        std::filebuf FileBuffer;

        /// @internal
        /// @brief The path and name of the file this stream is currently open to.
        String OpenFileName;
        /// @internal
        /// @brief The type of access this stream has to the file.
        Whole Flags;
        /// @internal
        /// @brief The size of the stream.
        StreamSize Size;
    public:
        /// @brief Blank constructor.
        FileStream();
        /// @brief Open constructor.
        /// @param File The combined name and path to the file to be opened.
        /// @param Mode The configuration to open the file with.
        FileStream(const String& File, const Whole Mode = Mezzanine::SF_Read | Mezzanine::SF_Write);
        /// @brief Split Open constructor.
        /// @param FileName The name of the file to be opened.
        /// @param FilePath The path to the file to be opened.
        /// @param Mode The configuration to open the file with.
        FileStream(const String& FileName, const String& FilePath, const Whole Mode = Mezzanine::SF_Read | Mezzanine::SF_Write);
        /// @brief Class destructor.
        virtual ~FileStream();

        ///////////////////////////////////////////////////////////////////////////////
        // Utility Methods

        /// @brief Opens this stream to a file.
        /// @exception If the stream fails to open the file specified, a IO_FILE_NOT_FOUND_EXCEPTION will be thrown.
        /// @param File The combined name and path to the file to be opened.
        /// @param Mode The configuration to open the file with.
        void OpenFile(const String& File, const Whole Mode = Mezzanine::SF_Read | Mezzanine::SF_Write);
        /// @brief Opens this stream to a file.
        /// @exception If the stream fails to open the file specified, a IO_FILE_NOT_FOUND_EXCEPTION will be thrown.
        /// @param FileName The name of the file to be opened.
        /// @param FilePath The path to the file to be opened.
        /// @param Mode The configuration to open the file with.
        void OpenFile(const String& FileName, const String& FilePath, const Whole Mode = Mezzanine::SF_Read | Mezzanine::SF_Write);
        /// @brief Gets whether or not this stream is currently open to a file.
        /// @return Returns true if this is streaming to/from a file.  False otherwise.
        Boole IsOpenToFile() const;
        /// @brief Closes the file that is currently opened.
        void CloseFile();

        /// @brief Gets the path and name of the file that this stream is currently open to.
        /// @return Returns a const String reference containing the path and name of the currently open file.
        const String& GetFilePathAndName() const;
        /// @brief Gets the flags that were used to open the file.
        /// @return Returns a bitfield describing the flags used to open the file.  If this stream is not open to a file it will return Resource::SF_None.
        Whole GetSteamFlags() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Stream Base Operations

        /// @copydoc StreamBase::GetSize() const
        virtual StreamSize GetSize() const;
    };//FileStream
}//Mezzanine

#endif
